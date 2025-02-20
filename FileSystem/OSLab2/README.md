(PDF reports avaliable in this repo)
## Lab 2: File Systems

This lab involves working with file systems, focusing on their structure, management, and performance optimization.

### Report on File System Implementation

#### 1. Introduction
The purpose of this lab was to create a file system that supports common operations like creating, deleting, moving, and copying files and directories. The system also enforces permissions and manages disk space. This report explains how we built the system, the decisions we made along the way, and how it handles different tasks.

#### 2. System Design

##### 2.1 Overview
The file system uses a FAT (File Allocation Table) to manage storage blocks and directory tables to organize files and folders. The root directory (/) is the base of the hierarchy, and the system supports subdirectories and basic file operations. Disk space is split into fixed-size blocks, and these are allocated or freed as needed.

##### 2.2 Key Components
- **Directory Entries (dir_entry)**: Store file/folder metadata (name, size, type, etc.).
- **FAT**: Keeps track of which blocks are free, in use, or the end of a file.
- **Directory Tables**: The root directory holds all top-level files, while the current directory table tracks where the user is working.

##### 2.3 Disk Management
The system reads and writes blocks to a simulated disk using the Disk class. Each block is a fixed size, which simplifies how data is stored and accessed.

#### 3. Implementations

##### 3.1 Formatting the Disk
The format function sets up the system by clearing the FAT and directory tables. It initializes all blocks as free (except for the root and FAT blocks) so the disk is ready for use and zeros out all the data on the disk.

##### 3.2 File Operations
- **create**: Adds a new file, allocating disk blocks and linking them in the FAT if the data doesn't fit in one block. File content is stored in these blocks, and metadata is added to the directory table.
- **cat**: Reads a file’s content and prints it, checking for read permissions first.
- **ls**: Lists the contents of the current directory, including names, sizes, types, and permissions.

##### 3.3 Directory Management
- **mkdir**: Creates a new directory, initializing its table with `..` entries that point to the parent directory. Works with relative or absolute paths.
- **cd**: Changes the current directory by updating the directory table to match the target location. Can use local or absolute paths.
- **pwd**: Prints the full path of the current directory by traversing back to the root temporarily and printing the traversed path.

##### 3.4 Copying and Moving Files
- **cp**: Copies a file to a folder or another file, allocating new blocks and writing the copied data. Metadata like size and permissions is duplicated.
- **mv**: Renames or moves a file.

##### 3.5 Permissions
- **chmod**: Updates a file or directory’s permissions (read, write, execute).
- **Checks**: Operations like `cat` and `append` ensure the required permissions are met.

#### 4. Data Structures

##### 4.1 FAT
The FAT maps each block to the next block in a file or marks it as free (FAT_FREE) or the end of a file (FAT_EOF).

##### 4.2 Directory Tables
Directories use tables of `dir_entry` structs to organize their contents. Each entry holds a file or folder’s metadata, such as its name, size, and permissions.
