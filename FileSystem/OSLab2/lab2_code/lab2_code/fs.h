#pragma once
#include <iostream>
#include <cstdint>
#include "disk.h"
#include <string>
#include<vector>
using namespace std;

#ifndef __FS_H__
#define __FS_H__

#define ROOT_BLOCK 0
#define FAT_BLOCK 1
#define FAT_FREE 0
#define FAT_EOF -1
#define FAT_Size BLOCK_SIZE/2
#define TABLE_SIZE BLOCK_SIZE/sizeof(dir_entry)

#define TYPE_FILE 0
#define TYPE_DIR 1
#define READ 0x04
#define WRITE 0x02
#define EXECUTE 0x01

struct dir_entry {
    char file_name[56]; // name of the file / sub-directory
    uint32_t size; // size of the file in bytes
    uint16_t first_blk; // index in the FAT for the first block of the file
    uint8_t type; // directory (1) or file (0)
    uint8_t access_rights; // read (0x04), write (0x02), execute (0x01)
};

class FS {
private:
    Disk disk;
    // size of a FAT entry is 2 bytes
    dir_entry root_directory_table[TABLE_SIZE];
    dir_entry curr_directory_table[TABLE_SIZE];
    int16_t fat[FAT_Size];
    int currentDir;
    
    std::string wd_path;


public:
    string splitString(string &lines, int block, int capacity, string &temp);
    void printOutRootDirectory(string fileName);
    void zeroOutDisk();
    void formatDirectoryTable();
    void writeToDisk(vector<string> lines, int freeblocks[], int blocksrequired);
    bool checkIfFileExist(dir_entry root_directory_table[],string fileName,int type);
    bool checkIfFileExistRoot(dir_entry root_directory_table[],string fileName,int type);
    int getsBlocksRequired(int size);
    void emptyRootBlock(dir_entry root_directory_table[],int block);
    void updateDisk();
    void zeroOutDiskBlock(int block);
    bool checkSpaceInDisk(int spaceWanted);
    vector<string> splitNameIntoFolders(string fileName);
    void printOutTable(dir_entry curr_directory_table[]);
    bool checkIfDestinationAndCurrentPathAreSame(string destpath);
    // void lsRoot();

    // string splitStringsInSize4096(string lines, int block);
    FS();
    ~FS();
    // formats the disk, i.e., creates an empty file system
    int format();
    // create <filepath> creates a new file on the disk, the data content is
    // written on the following rows (ended with an empty row)
    int create(std::string filepath);
    // cat <filepath> reads the content of a file and prints it on the screen
    int cat(std::string filepath);
    // ls lists the content in the current directory (files and sub-directories)
    int ls();

    // cp <sourcepath> <destpath> makes an exact copy of the file
    // <sourcepath> to a new file <destpath>
    int cp(std::string sourcepath, std::string destpath);
    // mv <sourcepath> <destpath> renames the file <sourcepath> to the name <destpath>,
    // or moves the file <sourcepath> to the directory <destpath> (if dest is a directory)
    int mv(std::string sourcepath, std::string destpath);
    // rm <filepath> removes / deletes the file <filepath>
    int rm(std::string filepath);
    // append <filepath1> <filepath2> appends the contents of file <filepath1> to
    // the end of file <filepath2>. The file <filepath1> is unchanged.
    int append(std::string filepath1, std::string filepath2);

    // mkdir <dirpath> creates a new sub-directory with the name <dirpath>
    // in the current directory
    int mkdir(std::string dirpath);
    // cd <dirpath> changes the current (working) directory to the directory named <dirpath>
    int cd(std::string dirpath);
    // pwd prints the full path, i.e., from the root directory, to the current
    // directory, including the current directory name
    int pwd();

    // chmod <accessrights> <filepath> changes the access rights for the
    // file <filepath> to <accessrights>.
    int chmod(std::string accessrights, std::string filepath);
};

#endif // __FS_H__