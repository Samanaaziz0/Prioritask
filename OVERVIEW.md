# Project Overview

**Prioritask** is a console-based to-do list manager that always keeps tasks ordered by importance.

## The problem
Ordinary to-do lists are ordered by when you wrote things down, not by what matters most. You end up scanning the whole list to decide what to do next.

## The solution
Each task is added with a priority from 1 (most urgent) to 10 (least urgent). The program inserts it at the correct position in a sorted linked list, so the most important pending task is always first and can be retrieved instantly with the "Next Task" option.

## What it does
- Adds, lists, completes, and deletes tasks
- Shows only pending tasks on request
- Records a timestamp for every task
- Saves automatically to a text file and reloads on startup

## Concepts demonstrated
- Linked list implementation (sorted insertion, deletion, traversal)
- Dynamic memory management with a destructor to prevent leaks
- File I/O for persistent storage
- Object-oriented design with a menu-driven interface

## Tech
C++17, standard library only. No external dependencies.
