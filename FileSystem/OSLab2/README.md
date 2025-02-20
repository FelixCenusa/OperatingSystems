# Lab 2: File Systems

**PDF reports available in this repository**

## TL;DR

- **File System Lab:** Build a FAT-based file system supporting standard file and directory operations.
- **Key Operations:** Create, delete, copy, move files/directories, manage disk space, and enforce permissions.
- **Core Components:**
    - **FAT:** Manages fixed-size disk blocks.
    - **Directory Tables:** Organizes file and folder metadata.
- **Disk Simulation:** Uses a simulated disk via a `Disk` class for portability.
- **Documentation:** Detailed explanations and performance analysis are provided in the PDF report within the repository.

This lab involves building a file system that supports common operations like creating, deleting, moving, and copying files and directories. It also enforces permissions and manages disk space efficiently. Developed by Felix Cenusa and Yamen Chams (BTH Software Engineers) on 18‑12‑2024.

## Table of Contents

- [Project Overview](#project-overview)
- [System Design](#system-design)
    - [Overview](#overview)
    - [Key Components](#key-components)
    - [Disk Management](#disk-management)
- [Implementations](#implementations)
    - [Formatting the Disk](#formatting-the-disk)
    - [File Operations](#file-operations)
    - [Directory Management](#directory-management)
    - [Copying and Moving Files](#copying-and-moving-files)
    - [Permissions](#permissions)
- [Data Structures](#data-structures)
    - [File Allocation Table (FAT)](#file-allocation-table-fat)
    - [Directory Tables](#directory-tables)
- [Getting Started](#getting-started)
- [Contributors](#contributors)
- [Additional Resources](#additional-resources)

## Project Overview

The goal of this lab is to design and implement a robust file system that supports essential file and directory operations, enforces permissions, and manages disk space through efficient allocation and deallocation of fixed-size blocks. The system is designed to be portable and demonstrable on any platform using a simulated disk.

## System Design

### Overview

The file system utilizes a FAT (File Allocation Table) to manage storage blocks and directory tables to organize files and folders. The root directory (`/`) forms the base of the hierarchy, supporting subdirectories and standard file operations. Disk space is divided into fixed-size blocks, which are allocated or freed as needed.

### Key Components

- **Directory Entries (`dir_entry`):**  
    Store metadata such as file/folder name, size, type, and permissions.
- **FAT (File Allocation Table):**  
    Tracks which disk blocks are free, in use, or mark the end of a file.
- **Directory Tables:**  
    Organize files and subdirectories; the root directory holds top-level entries, while subdirectories manage their own contents.

### Disk Management

The file system simulates disk operations using a `Disk` class that reads and writes fixed-size blocks. This abstraction ensures that the system can run on any platform while simplifying data storage and access.

## Implementations

### Formatting the Disk

The `format` function initializes the file system:

- Clears the FAT and directory tables.
- Marks all blocks as free (except reserved blocks for the root and FAT).
- Zeros out all disk data, providing a clean slate for subsequent operations.

### File Operations

- **create:**  
    Adds a new file by allocating disk blocks and linking them via the FAT. File content is distributed across blocks, and metadata is recorded in the directory table.
- **cat:**  
    Reads and displays file content after verifying read permissions.
- **ls:**  
    Lists the contents of the current directory, showing names, sizes, types, and permissions.

### Directory Management

- **mkdir:**  
    Creates a new directory and initializes its table with entries (including `..` for the parent directory), supporting both relative and absolute paths.
- **cd:**  
    Changes the current directory by updating the directory table to match the target location.
- **pwd:**  
    Prints the full path of the current directory by traversing back to the root.

### Copying and Moving Files

- **cp:**  
    Copies a file to another location by allocating new blocks and duplicating its data and metadata.
- **mv:**  
    Renames or moves a file within the file system.

### Permissions

- **chmod:**  
    Updates permissions (read, write, execute) for files or directories.
- **Permission Checks:**  
    File operations like `cat` and `append` enforce permission requirements to ensure secure access.

## Data Structures

### File Allocation Table (FAT)

The FAT maps each disk block to the next block in a file:

- Marks blocks as free (`FAT_FREE`), in use, or as the end of a file (`FAT_EOF`).

### Directory Tables

Directories are represented as tables of `dir_entry` structures that capture metadata (name, size, permissions, etc.) and organize the file system's hierarchical structure.

## Getting Started

To run the file system project locally:

1. **Clone the Repository:**
    
    bash
    
    Copy
    
    `git clone https://your-repo-url.git`
    
2. **Compile the Code:**
    - Ensure you have a C compiler installed. For example:
        
        bash
        
        Copy
        
        `gcc -o lab2 filesys.c`
        
3. **Run the Executable:**
    
    bash
    
    Copy
    
    `./lab2`
    
4. **Review the PDF Report:**
    - Detailed explanations and performance analyses are available in the PDF report within this repository.

## Contributors

- **Felix Cenusa**
- **Yamen Chams**  

## Additional Resources

- Detailed PDF Report on File System Implementation avaliable in this repo
