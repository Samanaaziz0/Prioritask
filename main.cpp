#include <iostream>
#include <string>
#include <ctime>
#include <fstream>
using namespace std;

// Structure for a Task
struct Task {
    int id;
    string description;
    int priority; // 1 = highest, 10 = lowest
    bool completed;
    string timestamp;
    Task* next;
    
    Task(int i, string desc, int pri) {
        id = i;
        description = desc;
        priority = pri;
        completed = false;
        next = nullptr;
        
        // Get current time
        time_t now = time(0);
        timestamp = ctime(&now);
        timestamp.pop_back(); // Remove newline
    }
    
    // Constructor for loading from file
    Task(int i, string desc, int pri, bool comp, string ts) {
        id = i;
        description = desc;
        priority = pri;
        completed = comp;
        timestamp = ts;
        next = nullptr;
    }
};

// Priority Todo List Class
class PriorityTodoList {
private:
    Task* head;
    int taskCounter;
    string filename;
    
public:
    PriorityTodoList(string fname = "tasks.txt") {
        head = nullptr;
        taskCounter = 0;
        filename = fname;
        loadFromFile(); // Auto-load tasks when program starts
    }
    
    // Add task in sorted order (by priority)
    void addTask(string description, int priority) {
        if (priority < 1 || priority > 10) {
            cout << "Invalid priority! Use 1-10 (1=highest, 10=lowest)\n";
            return;
        }
        
        Task* newTask = new Task(++taskCounter, description, priority);
        
        // If list is empty or new task has higher priority than head
        if (head == nullptr || head->priority > priority) {
            newTask->next = head;
            head = newTask;
            cout << "Task added successfully with ID: " << taskCounter << endl;
            saveToFile(); // Auto-save after adding
            return;
        }
        
        // Find correct position to insert
        Task* curr = head;
        while(curr->next != nullptr&&curr->next->priority<= priority) {
            curr = curr->next;
        }
        
        newTask->next = curr->next;
        curr->next = newTask;
        cout << "Task added successfully with ID: " << taskCounter << endl;
        saveToFile(); // Auto-save after adding
    }
    
    // Display all tasks
    void displayTasks() {
        if (head == nullptr) {
            cout << "\n=== No tasks in the list ===\n";
            return;
        }
        
        cout << "\n=== PRIORITY TODO LIST ===\n";
        cout << "Format: [ID] Priority | Description | Status | Added\n";
        cout << string(70, '-') << endl;
        
        Task* curr = head;
        while (curr != nullptr) {
            cout << "[" << curr->id << "] ";
            cout << "P" << curr->priority << " | ";
            cout << curr->description << " | ";
            cout << (curr->completed ? " Done" : " Pending") << " | ";
            cout << curr->timestamp << endl;
            curr = curr->next;
        }
        cout << string(70, '-') << endl;
    }
    
    // Get next priority task (highest priority incomplete task)
    void getNextTask() {
        Task* curr = head;
        
        while (curr != nullptr) {
            if (!curr->completed) {
                cout << "\n*** NEXT PRIORITY TASK ***\n";
                cout << "ID: " << curr->id << endl;
                cout << "Priority: " << curr->priority << endl;
                cout << "Description: " << curr->description << endl;
                cout << "Added: " << curr->timestamp << endl;
                return;
            }
            curr = curr->next;
        }
        
        cout << "\nAll tasks completed!\n";
    }
    
    // Mark task as complete
    void completeTask(int id) {
        Task* curr = head;
        
        while (curr != nullptr) {
            if (curr->id == id) {
                if (curr->completed) {
                    cout << "Task already completed!\n";
                } else {
                    curr->completed = true;
                    cout << "Task " << id << " marked as complete!\n";
                    saveToFile(); // Auto-save after completion
                }
                return;
            }
            curr = curr->next;
        }
        
        cout << "Task with ID " << id << " not found!\n";
    }
    
    // Delete a task
    void deleteTask(int id) {
        if (head == nullptr) {
            cout << "List is empty!\n";
            return;
        }
        
        // If head needs to be deleted
        if (head->id == id) {
            Task* temp = head;
            head = head->next;
            delete temp;
            cout << "Task " << id << " deleted successfully!\n";
            saveToFile(); // Auto-save after deletion
            return;
        }
        
        // Find the task to delete
        Task* curr = head;
        while (curr->next != nullptr && curr->next->id != id) {
            curr = curr->next;
        }
        
        if (curr->next == nullptr) {
            cout << "Task with ID " << id << " not found!\n";
            return;
        }
        
        Task* temp = curr->next;
        curr->next = curr->next->next;
        delete temp;
        cout << "Task " << id << " deleted successfully!\n";
        saveToFile(); // Auto-save after deletion
    }
    
    // Display only pending tasks
    void displayPendingTasks() {
        bool found = false;
        Task* curr = head;
        
        cout << "\n=== PENDING TASKS (Priority Order) ===\n";
        while (curr != nullptr) {
            if (!curr->completed) {
                cout << "[" << curr->id << "] P" << curr->priority 
                     << " - " << curr->description << endl;
                found = true;
            }
            curr = curr->next;
        }
        
        if (!found) {
            cout << "No pending tasks!\n";
        }
    }
    
    // Save tasks to file
    void saveToFile() {
        ofstream outFile(filename);
        
        if(!outFile) {
            cout << "Error: Unable to save tasks to file!\n";
            return;
        }
        
        // Save taskCounter first
        outFile<<taskCounter << endl;
        
        // Save each task
        Task* curr = head;
        while (curr != nullptr) {
            outFile << curr->id << endl;
            outFile << curr->description << endl;
            outFile << curr->priority << endl;
            outFile << curr->completed << endl;
            outFile << curr->timestamp << endl;
            curr = curr->next;
        }
        
        outFile.close();
    }
    
    // Load tasks from file
    void loadFromFile() {
        ifstream inFile(filename);
        
        if(!inFile) {
            // File doesn't exist yet (first run)
            return;
        }
        
        // Load taskCounter
        inFile>>taskCounter;
        inFile.ignore(); // Ignore newline after number
        
        // Temporary storage for loading
        int id, priority;
        string description, timestamp;
        bool completed;
        
        while (inFile >> id) {
            inFile.ignore(); // Ignore newline
            getline(inFile, description);
            inFile>>priority;
            inFile>> completed;
            inFile.ignore(); // Ignore newline
            getline(inFile, timestamp);
            
            // Create task and insert in sorted order
            Task* newTask = new Task(id, description, priority, completed, timestamp);
            
            // Insert in sorted order
            if (head == nullptr || head->priority > priority) {
                newTask->next = head;
                head = newTask;
            } else {
                Task* curr = head;
                while (curr->next != nullptr && curr->next->priority <= priority) {
                    curr = curr->next;
                }
                newTask->next = curr->next;
                curr->next = newTask;
            }
        }
        
        inFile.close();
        
        if (head != nullptr) {
            cout << "Tasks loaded from file successfully!\n";
        }
    }
    
    // Manual save option
    void manualSave() {
        saveToFile();
        cout << " Tasks saved to file: " << filename << endl;
    }
    
    // Clear all tasks
    void clearAllTasks() {
        char confirm;
        cout << "Are you sure you want to delete ALL tasks? (y/n): ";
        cin >> confirm;
        
        if (confirm == 'y' || confirm == 'Y') {
            while (head != nullptr) {
                Task* temp = head;
                head = head->next;
                delete temp;
            }
            taskCounter = 0;
            saveToFile();
            cout << "All tasks cleared!\n";
        } else {
            cout << "Operation cancelled.\n";
        }
    }
    
    // Destructor to free memory
    ~PriorityTodoList() {
        Task* curr = head;
        while (curr != nullptr) {
            Task* temp = curr;
            curr = curr->next;
            delete temp;
        }
    }
};

// Main function with menu
int main() {
    PriorityTodoList todoList;
    int choice, priority, id;
    string description;
    
    while (true) {
        cout << "\n====================================\n";
        cout << "  PRIORITY TODO LIST MANAGER  \n";
        cout << "====================================\n";
        cout << "1. Add Task\n";
        cout << "2. View All Tasks\n";
        cout << "3. Get Next Priority Task\n";
        cout << "4. Mark Task as Complete\n";
        cout << "5. Delete Task\n";
        cout << "6. View Pending Tasks Only\n";
        cout << "7. Save Tasks to File\n";
        cout << "8. Clear All Tasks\n";
        cout << "9. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore(); //Clear newline
        
        switch (choice) {
            case 1:
                cout << "Enter task description: ";
                getline(cin, description);
                cout << "Enter priority (1=highest, 10=lowest): ";
                cin >> priority;
                todoList.addTask(description, priority);
                break;
                
            case 2:
                todoList.displayTasks();
                break;
                
            case 3:
                todoList.getNextTask();
                break;
                
            case 4:
                cout << "Enter task ID to mark complete: ";
                cin >> id;
                todoList.completeTask(id);
                break;
                
            case 5:
                cout << "Enter task ID to delete: ";
                cin >> id;
                todoList.deleteTask(id);
                break;
                
            case 6:
                todoList.displayPendingTasks();
                break;
                
            case 7:
                todoList.manualSave();
                break;
                
            case 8:
                todoList.clearAllTasks();
                break;
                
            case 9:
                cout << "Exiting... All changes saved. Goodbye!\n";
                return 0;
                
            default:
                cout << "Invalid choice! Try again.\n";
        }
    }
    
    return 0;
}