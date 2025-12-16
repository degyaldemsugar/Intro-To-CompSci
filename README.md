# Student Grade Calculator & To-Do List Manager

## Project Overview
This repository contains two C programs developed as part of an Introduction to Programming course end-of-term project. Both programs demonstrate fundamental programming concepts including data structures, file I/O, memory management, and user interaction through console-based interfaces.

---

## Program A: Student Grade Calculator (`A.c`)

### What It Does
A comprehensive student grade management system that allows users to:
- Add students with their names and test scores (0-100)
- Automatically calculate and assign letter grades (A-F)
- Delete students from the system
- View the top-performing student
- Calculate and display class averages
- Save student data to files for persistent storage
- Load previously saved student data

### How to Run
1. **Compile the program:**
   ```bash
   gcc A.c -o student_grades
   ```

2. **Run the executable:**
   ```bash
   ./student_grades
   ```

3. **Follow the on-screen prompts:**
   - Choose whether to load an existing save file
   - Select options 1-5 to manage students:
     - `1`: Add a new student
     - `2`: Delete a student
     - `3`: Display top student
     - `4`: Display class average
     - `5`: Quit and save

### Features
- **Grade Calculation:** Automatically converts numeric scores to letter grades
- **Data Persistence:** Save/load functionality with file signature verification
- **Input Validation:** Robust error checking for user inputs
- **Memory Management:** Proper allocation and deallocation of linked list nodes
- **File Protection:** Warns before overwriting existing files

---

## Program B: To-Do List Manager (`B.c`)

### What It Does
A task management application that enables users to:
- Create tasks with names and descriptions
- Automatically timestamp each task with creation time
- Mark tasks as complete or incomplete
- Remove tasks from the list
- View all tasks in a formatted table
- Save task lists to files
- Load previously saved task lists

### How to Run
1. **Compile the program:**
   ```bash
   gcc B.c -o todo_list
   ```

2. **Run the executable:**
   ```bash
   ./todo_list
   ```

3. **Follow the on-screen prompts:**
   - Choose whether to load an existing save file
   - Select options 1-5 to manage tasks:
     - `1`: Add a task
     - `2`: Remove a task
     - `3`: View all tasks
     - `4`: Mark task as complete
     - `5`: Exit and save

### Features
- **Automatic Timestamps:** Each task records its creation time
- **Status Tracking:** Mark tasks as complete/incomplete
- **Formatted Display:** Professional table layout for viewing tasks
- **Data Persistence:** Save/load functionality with file validation
- **Flexible Input:** Handles multi-word task names and descriptions

---

## What I Learned

### Data Structures
- Implemented **linked lists** from scratch using structs and pointers
- Managed dynamic memory allocation with `malloc()` and proper cleanup with `free()`
- Used `enum` types for cleaner, more readable code (grades and completion status)

### File I/O
- Learned to read from and write to files using `fopen()`, `fprintf()`, `fgets()`, and `fscanf()`
- Implemented file signature verification for data integrity
- Built a file existence checker using directory operations

### Input Validation
- Created robust input handling to prevent buffer overflows
- Developed a `flush_input_buffer()` function to handle invalid inputs
- Implemented loops to ensure users provide valid data

### Memory Management
- Practiced proper allocation and deallocation patterns
- Created a `free_memory()` function to prevent memory leaks
- Handled edge cases like empty lists

### String Manipulation
- Used `strcpy()`, `strcat()`, `strcmp()`, and `strncmp()` for string operations
- Learned to handle newline characters from `fgets()`
- Implemented safe string copying with size limits

### Time Functions
- Utilized the `time.h` library to generate timestamps
- Formatted time data for display and storage

### Problem-Solving Approach
- Broke complex problems into smaller functions
- Designed user-friendly menu systems
- Implemented save/load functionality with error checking

---

## Technical Requirements
- **Compiler:** GCC or any C99-compatible compiler
- **Operating System:** Linux/Unix (uses `dirent.h` for directory operations)
- **Libraries Used:** `stdio.h`, `stdlib.h`, `string.h`, `time.h`, `dirent.h`

---

## File Format
Both programs use a custom file format with a signature line (`this_file_is_legit_trust`) to verify file integrity before loading data.

