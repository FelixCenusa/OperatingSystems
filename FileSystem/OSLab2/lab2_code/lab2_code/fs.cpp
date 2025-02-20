#include "fs.h"
#include <iostream>
#include<vector>
#include<cstring>
using namespace std;

FS::FS()
{
    std::cout << "FS::FS()... Creating file system\n";
    disk.read(0, (uint8_t*)root_directory_table);
    disk.read(0, (uint8_t*)curr_directory_table);
    disk.read(1,(uint8_t*)fat);
    currentDir = 0;
    wd_path = "";

}

FS::~FS()
{

}

// void 
// FS::lsRoot(){
//     cout<<"Name                                                   Size     type "<<endl;
//     for (int i = 0; i < TABLE_SIZE; i++)
//     {
//         if(root_directory_table[i].file_name[0] != '\0'){
//             int a = 0;
//             for (int j = 0; j < 56; j++)
//             {
//                 if(root_directory_table[i].file_name[j] == '\0'){
//                     break;
//                 }
//                 cout<<root_directory_table[i].file_name[j];
//                 a++;
//             }
//             for (int y = a; y < 56; y++)
//             {
//                 cout<<" ";

//             }
//             cout << root_directory_table[i].size << "        "<<static_cast<int>(root_directory_table[i].type)<<endl;
//         }
//     }  
// }

void
FS::writeToDisk(vector<string> lines, int freeblocks[], int blocksrequired)
{
    int capacity = BLOCK_SIZE;
    int currentBlock= 0;
    vector<string> temp;
    for (int i = 0; i < lines.size(); i++)
    {
        lines[i] = lines[i] + "\n"; 
    }
    

    for (int i = 0; i < blocksrequired; i++)
    {
        temp.push_back("");
    }
    
    for (int i = 0; i < lines.size(); i++)
    {
        if (lines[i].size() <= capacity)
        {
            capacity -= lines[i].size();
            //cout<<"Im going to write (" << (uint8_t*)lines[i].c_str()<< ")Into the disk at block: "<< freeblocks[currentBlock]<<endl;
            temp[currentBlock] += lines[i];


        }
        else 
        {
            string space = "";
            space = splitString(lines[i], currentBlock, capacity, temp[currentBlock]);
            currentBlock++;
            lines[i] = space;
            capacity = BLOCK_SIZE;
            i--;
        }
        
        
    }
    //help me fill the rest of the temp variable until it is full so 4096 bytes
    //cout<<"Im going to write DISK (" << (uint8_t*)temp.c_str()<< ")Into the disk at block: "<< freeblocks[currentBlock]<<endl;
    for (int i = 0; i < temp.size(); i++)
    {
        while (temp[i].size() < BLOCK_SIZE)
        {
            temp[i] += '\0';

        }
        disk.write(freeblocks[i], (uint8_t*)temp[i].c_str());
    }
    
}

vector<string>
 FS::splitNameIntoFolders(string fileName){
    int folderAmount = 1;
    for (int i = 1; i < fileName.size(); i++)
    {
        if (fileName[i] == '/')
        {
            folderAmount++;
        } 
    }
    vector<string> temp;
    for (int i = 0; i < folderAmount; i++)
    {
        temp.push_back("");
    }
    int currentFileName = 0;
    int start = 0;
    if (fileName[0] == '/')
    {
        start++;
    }
    for (int i = start; i < fileName.size(); i++)
    {
        if (fileName[i] == '/')
        {
            currentFileName++;
        }
        else{
            temp[currentFileName] += fileName[i];        
        }
    }
    return temp;
}

string FS::splitString(string &lines, int block, int capacity, string &temp)
{
    string haveSpace = "";
    string remainder = "";
    int i = 0;
    while (i <= capacity)
    {
        haveSpace += lines[i];
        i++;
    }
    i--;
    while(i <= lines.size()){
        remainder += lines[i];
        i++;

    }
    temp += haveSpace;
    return remainder;
}
void FS::zeroOutDiskBlock(int block){
    uint8_t zero_buffer[BLOCK_SIZE] = {0};
    disk.write(block, zero_buffer);

}

void FS::zeroOutDisk()
{
    for (int i = 0; i < FAT_Size; i++)
    {
        zeroOutDiskBlock(i);
    }    
}
void FS::emptyRootBlock(dir_entry curr_directory_table[],int block){
    for (int j = 0; j < 56; j++)
        {
            curr_directory_table[block].file_name[j] = '\0';
            /* code */
        }
    curr_directory_table[block].size = 0;
    curr_directory_table[block].first_blk = 0;
    curr_directory_table[block].type = 0;
    curr_directory_table[block].access_rights = 0;
}
void FS::updateDisk(){
    disk.write(currentDir, (uint8_t*)curr_directory_table);
    if (currentDir == 0)
    {
        disk.read(0,(uint8_t*)root_directory_table);
    }
    
    disk.write(0,(uint8_t*)root_directory_table);
    disk.write(1, (uint8_t*)fat);
}

void 
FS::formatDirectoryTable()
{
    std::cout << "FS::formatDirectoryTable()\n";
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        emptyRootBlock(root_directory_table,i);
    }
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        emptyRootBlock(curr_directory_table,i);
    }
}
bool 
FS::checkIfFileExist(dir_entry root_directory_table[],string fileName,int type) {
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        if (curr_directory_table[i].file_name == fileName && curr_directory_table[i].type == type)
        {
            return true;
        }
        
    }
    return false;
    
}
bool 
FS::checkIfFileExistRoot(dir_entry root_directory_table[],string fileName,int type) {
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        if (root_directory_table[i].file_name == fileName && root_directory_table[i].type == type)
        {
            return true;
        }
        
    }
    return false;
    
}


int 
FS::getsBlocksRequired(int size){
    int blocksRequired = size/BLOCK_SIZE; // checks how many blocks are required
    if (size % BLOCK_SIZE > 0)
    {
        blocksRequired++;
    }  
    return blocksRequired;
}
void FS::printOutRootDirectory(string fileName) {
    int firstblk = 0; 
    int fileSize = 0;
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (root_directory_table[i].file_name == fileName) {
            std::cout << "Access right: " << root_directory_table[i].access_rights << std::endl;
            std::cout << "File name: " << root_directory_table[i].file_name << std::endl;
            std::cout << "First Block: " << root_directory_table[i].first_blk << std::endl;
            std::cout << "Size: " << root_directory_table[i].size << std::endl;
            std::cout << "Type: " << root_directory_table[i].type << std::endl;
            firstblk = root_directory_table[i].first_blk; 
            fileSize = getsBlocksRequired(root_directory_table[i].size);
            break;
        }
    }
    std::cout << "Fat: " << firstblk<<endl;
    for (int i = 0; i < fileSize; i++) {
        std::cout << "Fat: " << fat[firstblk] << std::endl;
        firstblk = fat[firstblk];
    }
}

void FS::printOutTable(dir_entry curr_directory_table[]) {
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        if (curr_directory_table[i].file_name[0] != '\0')
        {
            cout<<" fileName:"<<curr_directory_table[i].file_name;
            cout<<" access_rights:"<<curr_directory_table[i].access_rights;
            cout<<" first_blk: "<<curr_directory_table[i].first_blk;
            cout<<" size: "<<curr_directory_table[i].size;
            cout<<" type: "<<curr_directory_table[i].type<<endl;

        }
        
    }
    
}

bool 
FS::checkSpaceInDisk(int spaceWanted){
    bool spaceExist1 = false;
    bool spaceExist2 = false;
    int counter = 0;
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        if (curr_directory_table[i].file_name[0] == '\0')
        {
            spaceExist1 = true;
            break;
        }
    }
    for (int i = 0; i < FAT_Size; i++)
    {
        if (fat[i] == FAT_FREE)
        {
            counter++;
            spaceExist2 = false;
        }
        if (counter >= spaceWanted){
            spaceExist2 = true;
            break;
        }
    }
    return spaceExist1 && spaceExist2;
    
    
}


// formats the disk, i.e., creates an empty file system
int
FS::format()
{
    std::cout << "FS::format()\n";

    zeroOutDisk();

    fat[ROOT_BLOCK] = FAT_EOF;
    fat[FAT_BLOCK] = FAT_EOF;
    for (int i = FAT_BLOCK + 1; i < FAT_Size; i++)
    {
        fat[i] = FAT_FREE;
    }
    currentDir = 0;
    
    formatDirectoryTable();
    updateDisk();
    return 0;
}


int
FS::create(std::string filepath)
{
    std::cout << "FS::create(" << filepath << ")\n";
    bool valid = true;
    vector<string> lines;
    string input;
    int sizeOfText = 0;
    // check if the file name is too long
    if (filepath.length() > 55)
    {
        std::cout << "Error: File name too long\n";
        return -1;
    }
    if (checkIfFileExist(curr_directory_table,filepath,0))
    {
        std::cout << "Error: File already exists\n";
        return -1;
    }

    if (checkSpaceInDisk(1) == false)
    {
        std::cout << "Error: Not enough space on disk\n";
        return -1;

    }
    
    // for (int i = 0; i < TABLE_SIZE; i++)
    // {
    //     if (filepath.compare(curr_directory_table[i].file_name) == 0)
    //     {
    //         valid = false;
    //         t = curr_directory_table[i].type;
    //     }
    // }
    // if (!valid && t == 0)
    // {
    //     std::cout << "Error: File already exists\n";
    //     return -1;
    // }
    cout << "Enter text (press Enter twice to finish):" << std::endl;
    while (getline(cin, input), !input.empty()) {
        lines.push_back(input);
    }
    for (int i = 0; i < lines.size(); i++)
    {
        sizeOfText += lines[i].size();
    }
    sizeOfText += lines.size();
    //  Make sure the file name does not exist already in the directory
    // Find a free entry in the directory table
    int blocksRequired = getsBlocksRequired(sizeOfText);
    int *freeBlocks = new int [blocksRequired]; // array to store the free blocks
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        if(curr_directory_table[i].file_name[0] == '\0')
        {
                int blocksfree = 0;
                bool placeExist = false;
                for (int y = 0; y < FAT_Size; y++)
                {
                    if (blocksfree >= blocksRequired)
                    {
                        placeExist = true;
                        break;
                    }   
                    if(fat[y] == FAT_FREE){
                        freeBlocks[blocksfree] = y;
                        blocksfree++;
                    }
                }
                if (placeExist)
                {
                    for (int i = 0; i < blocksRequired - 1; i++)
                    {
                        fat[freeBlocks[i]] = freeBlocks[i+1];
                    }
                    fat[freeBlocks[blocksRequired-1]] = FAT_EOF;
                    writeToDisk(lines,freeBlocks,blocksRequired);
                    if (blocksRequired == 0)
                    {
                        curr_directory_table[i].first_blk = 0;
                    }
                    else{
                        curr_directory_table[i].first_blk = freeBlocks[0];
                    }
                    for (int j = 0; j < filepath.length(); j++)
                    {
                        curr_directory_table[i].file_name[j] = filepath[j];
                    }
                    curr_directory_table[i].size = sizeOfText;
                    curr_directory_table[i].type = 0;
                    curr_directory_table[i].access_rights = 6; 
                    if (currentDir == 0)
                    {
                        for (int i = 0; i < TABLE_SIZE; i++)
                        {
                            root_directory_table[i] = curr_directory_table[i];
                        }
                        
                    }
                    
                    updateDisk();
                    break;
                } 
                else
                {
                    std::cout << "Error: Not enough space on disk\n";
                    return -1;
                }
        }          
    } 
    return 0;
}

// cat <filepath> reads the content of a file and prints it on the screen
int
FS::cat(std::string filepath)
{
    int crr = currentDir;
    vector<string> files;
    if (filepath[0] == '/')
    {
        files = splitNameIntoFolders(filepath);
        disk.read(0,(uint8_t*)curr_directory_table);
        for (int i = 0; i < files.size()-1; i++)
        {
            //cout<<"Files: "<<files[i]<<endl;
            if (checkIfFileExist(curr_directory_table,files[i],1))
            {
                cd(files[i]);
                
            }
            else{
                disk.read(crr,(uint8_t*)curr_directory_table);
                currentDir = crr;
                cout<<"ERROR: Folder doesnt exist"<<endl;
                return -1;
            }
            
        }
        filepath = files[files.size()-1];
    }
    std::vector<uint8_t> blockText(BLOCK_SIZE); // Adjust BLOCK_SIZE to the size needed
    // Check if file exists
    bool valid = false;
    int fileFirstBlk = 0;
    uint8_t access_rights = 0;
    bool justFileFound = false;    
    if (checkIfFileExist(curr_directory_table,filepath,0))
    {
        for (int i = 0; i < TABLE_SIZE; i++)
        {
            if (filepath == curr_directory_table[i].file_name && curr_directory_table[i].type== 0)
            {
                // Check for read access
                if (!(curr_directory_table[i].access_rights & 0x04))
                {
                    std::cout << "Error: Read access denied.\n";
                    disk.read(crr,(uint8_t*)curr_directory_table);
                    currentDir = crr;
                    return -1;
                }

                valid = true;
                fileFirstBlk = curr_directory_table[i].first_blk;
                access_rights = curr_directory_table[i].access_rights;
                break;
            }
        }
    }
    if (checkIfFileExist(curr_directory_table,filepath,1) && valid != true)
    {
        disk.read(crr,(uint8_t*)curr_directory_table);
        currentDir = crr;
        cout<<"ERROR: folder cant be printed"<<endl;
        return -1;
    }
    

    if (!valid)
    {
        disk.read(crr,(uint8_t*)curr_directory_table);
        currentDir = crr;
        std::cout << "Error: File doesn't exist.\n";
        return -1;
    }

    // Read and print file contents
    int nextBlock = fileFirstBlk;
    while (true)
    {
        if (nextBlock == FAT_EOF)
        {
            break;
        }

        disk.read(nextBlock, &blockText[0]);
        for (auto byte : blockText)
        {
            if (byte == '\0') // Stop at null terminator
                break;
            std::cout << byte; // Assuming byte is a character
        }

        nextBlock = fat[nextBlock];
    }
    //cout<<"THIS IS CRR: "<<crr<<endl;
    disk.read(crr,(uint8_t*)curr_directory_table);
    currentDir = crr;
    //updateDisk();
    return 0;
}

// ls lists the content in the currect directory (files and sub-directories)
int
FS::ls()
{
    std::cout << "name              type       accessrights     size\n";

    // Iterate through the directory table
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        if (curr_directory_table[i].file_name[0] != '\0') // Check if the entry is valid
        {
            // Print name
            std::cout << curr_directory_table[i].file_name;

            // Adjust spacing for name
            int name_length = 0;
            while (name_length < 56 && curr_directory_table[i].file_name[name_length] != '\0') {
                name_length++;
            }
            for (int j = name_length; j < 18; j++) // Pad to 18 characters
            {
                std::cout << " ";
            }

            // Print type (file or dir)
            if (curr_directory_table[i].type == 1)
            {
                std::cout << "dir        ";
            }
            else
            {
                std::cout << "file       ";
            }

            // Print access rights
            uint8_t rights = curr_directory_table[i].access_rights;
            std::cout << ((rights & 0x04) ? "r" : "-");
            std::cout << ((rights & 0x02) ? "w" : "-");
            std::cout << ((rights & 0x01) ? "x" : "-");
            std::cout << "              ";

            // Print size
            if (curr_directory_table[i].type == 1)
            {
                std::cout << "-"; // Directories don't have a size
            }
            else
            {
                std::cout << curr_directory_table[i].size;
            }

            std::cout << "\n"; // Move to the next line
        }
    }

    return 0;
}

// cp <sourcepath> <destpath> makes an exact copy of the file
// <sourcepath> to a new file <destpath>
int
FS::cp(std::string sourcepath, std::string destpath)
{
    int ass = currentDir;
    bool fileExist = false;
    string origin = destpath;
    //bool isFile = true;
    int crr = currentDir;
    vector<string> files;
    int destIsPath = false;
    if (checkIfFileExist(curr_directory_table,sourcepath,0) == false)
    {
        cout << "Error: Original file doesnt exist\n";
        return -1;
    }
    if (checkIfFileExist(curr_directory_table,destpath,1))
    {
        destIsPath = true;
    }
    
    for (int i = 0; i < destpath.length(); i++)
    {
        if (destpath[i] == '/')
        {
            destIsPath = true;
        }
        
    }
    
    if (destIsPath)
    {
        cout<<"Im here"<<endl;
        files = splitNameIntoFolders(destpath);
        if (destpath[0] == '/')
        {
            disk.read(0,(uint8_t*)curr_directory_table);
        }
        
        for (int i = 0; i < files.size()-1; i++)
        {
            //cout<<"Files: "<<files[i]<<endl;
            if (checkIfFileExist(curr_directory_table,files[i],1))
            {
                cd(files[i]);
                
            }
            else{
                disk.read(crr,(uint8_t*)curr_directory_table);
                currentDir = crr;
                cout<<"ERROR: Folder doesnt exist: "<<files[i]<<endl;
                return -1;
            }
            
        }
        destpath = files[files.size()-1];
        fileExist = checkIfFileExist(curr_directory_table,destpath,1);
        disk.read(crr,(uint8_t*)curr_directory_table);
        currentDir = crr;
        //cout<<"FILE EXIST IS : "<< fileExist<<endl;
    }
    

    if (checkIfFileExist(curr_directory_table,destpath,1) || fileExist == true)
    {
        for (int i = 0; i < TABLE_SIZE; i++)
        {
            if (curr_directory_table[i].file_name == destpath && curr_directory_table[i].type == 1 && curr_directory_table[i].access_rights <= 5)
            {
                cout << "You dont have Read and execute premisions" << endl;
                disk.read(crr,(uint8_t*)curr_directory_table);
                currentDir = crr;
                return -1;
            }
            
        }
        
        int fileSize = 0;
        int firstBlk = 0;
        for (int i = 0; i < TABLE_SIZE; i++)
        {
            if (curr_directory_table[i].file_name == sourcepath)
            {
                fileSize = curr_directory_table[i].size;
                firstBlk = curr_directory_table[i].first_blk;
                break;
            }
            
        }
        // Checks how many blocks and gets the blocks that the original file are written into
        int blocksRequired = getsBlocksRequired(fileSize);

        vector<string>Blocks;
        int arr[blocksRequired];
        for (int i = 0; i < blocksRequired; i++)
        {
            arr[i] = firstBlk;
            firstBlk = fat[firstBlk];
            Blocks.push_back(string(BLOCK_SIZE, '\0'));
        }
        //Reads the blocks that the original file are written into
        for (int i = 0; i < blocksRequired; i++)
        {
            disk.read(arr[i],(uint8_t*)&Blocks[i][0]);
        }
        int blocksfree = 0;
        bool placeExist = false;
        int freeBlocks[blocksRequired];
        if (origin[0] == '/')
        {
            cout<<"WE ARE HERE"<<endl;
            for (int i = 0; i < TABLE_SIZE; i++)
            {
                curr_directory_table[i] = root_directory_table[i];
                currentDir = 0;
            }
            
        }
        for (int i = 0; i < files.size(); i++)
        {
            cd(files[i]);   
        }
        
        //cd(destpath);
        if (checkIfFileExist(curr_directory_table,sourcepath,0))
        {
            disk.read(crr,(uint8_t*)curr_directory_table);
            currentDir = crr;
            cout<<"Error "<<endl;
            return -1;
        }
        
        for (int i = 0; i < TABLE_SIZE; i++)
        {
            //IF there is an empty space in the directory
            if(curr_directory_table[i].file_name[0] == '\0')
            {
                //If there is space in the fat
                for (int i = 0; i < FAT_Size; i++)
                {
                    if (blocksfree >= blocksRequired)
                    {
                        placeExist = true;
                        break;
                    }   
                    if(fat[i] == FAT_FREE){
                        freeBlocks[blocksfree] = i;
                        blocksfree++;
                    }
                }
                //Start Pasting...
                if (placeExist)
                    {
                        //Sets the blocks in the fat for the new file
                        for (int i = 0; i < blocksRequired - 1; i++)
                        {
                            fat[freeBlocks[i]] = freeBlocks[i+1];
                        }
                        fat[freeBlocks[blocksRequired-1]] = FAT_EOF;
                        //Write the data from the original to the copy 
                        for (int i = 0; i < blocksRequired; i++)
                        {
                            disk.write(freeBlocks[i],(uint8_t*)Blocks[i].c_str());
                        }
                        //Update the directory and the fat
                        curr_directory_table[i].first_blk = freeBlocks[0];
                        for (int j = 0; j < sourcepath.length(); j++)
                        {
                            curr_directory_table[i].file_name[j] = sourcepath[j];
                        }
                        curr_directory_table[i].size = fileSize;
                        curr_directory_table[i].type = 0;
                        curr_directory_table[i].access_rights = 6; 
                        if (currentDir == 0)
                        {
                            for (int i = 0; i < TABLE_SIZE; i++)
                            {
                                root_directory_table[i] = curr_directory_table[i];
                            }
                            
                        }
                        updateDisk();
                        break;
                    } 
                    // No Space in the disk
                    else
                    {
                        std::cout << "Error: Not enough space on disk\n";
                        disk.read(crr,(uint8_t*)curr_directory_table);
                        currentDir = crr;
                        return -1;
                    }
            }
        }
        if (destpath == ".." || origin[0] == '/')
        {
            for (int i = 0; i < TABLE_SIZE; i++)
            {
                if (curr_directory_table[i].first_blk == ass)
                {
                    cd(curr_directory_table[i].file_name);
                    break;
                }
            }
        }
        else{
            cd("..");
        }
        disk.read(crr,(uint8_t*)curr_directory_table);
        currentDir = crr;
        return 0;
        
    }
    

    if (checkIfFileExist(curr_directory_table,destpath,0) == true)
    {
        cout << "Error: Copy file name allready exist\n";
        return -1;
    }

    //Gets the infomration of the original path
    std::cout << "FS::cp(" << sourcepath << "," << destpath << ")\n";
    int fileSize = 0;
    int firstBlk = 0;
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        if (curr_directory_table[i].file_name == sourcepath)
        {
            fileSize = curr_directory_table[i].size;
            firstBlk = curr_directory_table[i].first_blk;
            break;
        }
        
    }
    // Checks how many blocks and gets the blocks that the original file are written into
    int blocksRequired = getsBlocksRequired(fileSize);

    vector<string>Blocks;
    int arr[blocksRequired];
    for (int i = 0; i < blocksRequired; i++)
    {
        arr[i] = firstBlk;
        firstBlk = fat[firstBlk];
        Blocks.push_back(string(BLOCK_SIZE, '\0'));
    }
    //Reads the blocks that the original file are written into
    for (int i = 0; i < blocksRequired; i++)
    {
        disk.read(arr[i],(uint8_t*)&Blocks[i][0]);
    }
    int blocksfree = 0;
    bool placeExist = false;
    int freeBlocks[blocksRequired];
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        //IF there is an empty space in the directory
        if(curr_directory_table[i].file_name[0] == '\0')
        {
            //If there is space in the fat
            for (int i = 0; i < FAT_Size; i++)
            {
                if (blocksfree >= blocksRequired)
                {
                    placeExist = true;
                    break;
                }   
                if(fat[i] == FAT_FREE){
                    freeBlocks[blocksfree] = i;
                    blocksfree++;
                }
            }
            //Start Pasting...
            if (placeExist)
                {
                    //Sets the blocks in the fat for the new file
                    for (int i = 0; i < blocksRequired - 1; i++)
                    {
                        fat[freeBlocks[i]] = freeBlocks[i+1];
                    }
                    fat[freeBlocks[blocksRequired-1]] = FAT_EOF;
                    //Write the data from the original to the copy 
                    for (int i = 0; i < blocksRequired; i++)
                    {
                        disk.write(freeBlocks[i],(uint8_t*)Blocks[i].c_str());
                    }
                    //Update the directory and the fat
                    curr_directory_table[i].first_blk = freeBlocks[0];
                    for (int j = 0; j < destpath.length(); j++)
                    {
                        curr_directory_table[i].file_name[j] = destpath[j];
                    }
                    curr_directory_table[i].size = fileSize;
                    curr_directory_table[i].type = 0;
                    curr_directory_table[i].access_rights = 6; 
                    updateDisk();
                    break;
                } 
                // No Space in the disk
                else
                {
                    std::cout << "Error: Not enough space on disk\n";
                    return -1;
                }
        }
    }
    return 0;
}

bool FS::checkIfDestinationAndCurrentPathAreSame(std::string destpath) {
    if (!destpath.empty() && destpath[0] == '/') {
        destpath = destpath.substr(1); // just in case idk
    }

    for (int i = 0; i < TABLE_SIZE; i++) {
        if (std::string(curr_directory_table[i].file_name) == destpath) {
            return true;
        }
    }

    for (int i = 0; i < TABLE_SIZE; i++) {
        if (std::string(curr_directory_table[i].file_name) == "..") {
            uint32_t parent_block = curr_directory_table[i].first_blk;

            dir_entry temp_directory_table[TABLE_SIZE];
            disk.read(parent_block, (uint8_t*)temp_directory_table);

            for (int j = 0; j < TABLE_SIZE; j++) {
                if (std::string(temp_directory_table[j].file_name) == destpath) {
                    return true;
                }
            }
        }
    }

    // If destpath does not match any entries, it is not the same path
    return false;
}



// mv <sourcepath> <destpath> renames the file <sourcepath> to the name <destpath>,
// or moves the file <sourcepath> to the directory <destpath> (if dest is a directory)
int
FS::mv(std::string sourcepath, std::string destpath)
{

    bool fileExist = false;
    string origin = destpath;
    int crr = currentDir;
    vector<string> files;
    bool isFolder = false;
    //std::cout << "FS::mv(" << sourcepath << "," << destpath << ")\n";
    for (int i = 0; i < destpath.length(); i++)
    {
        if (destpath[i] == '/')
        {
            isFolder = true;
            break;
        }
        
    }
    if (checkIfFileExist(curr_directory_table,destpath,1))
    {
        isFolder = true;
    }
    
    
    if (isFolder)
    {
        files = splitNameIntoFolders(destpath);
        //fileExist = checkIfFileExistRoot(root_directory_table,destpath,1);
        // if (checkIfDestinationAndCurrentPathAreSame(files[files.size()-1]) == true)
        // {
        //     cout << "Ok.. moving it to the same place lol\n";
        //     return 0;
        // }
    }

    if (isFolder)
    {
        int copyValid = 0;
        cout<<"Im going to press cp "<< sourcepath << " Into: "<< destpath<<endl;
       copyValid = cp(sourcepath,destpath);
        
        // if (destpath[0] != '.')
        // {
        //     //cd("..");
        // }
        // else{
        //     for (int i = 0; i < TABLE_SIZE; i++)
        //     {
        //         if (curr_directory_table[i].first_blk == cur)
        //         {
        //             cd(curr_directory_table[i].file_name);
        //             break;
        //         }
        //     }
        // }
        if (copyValid == 0)
        {
            rm(sourcepath);
        }
        
        //updateDisk();
        //cd("..");
        return 0;
    }
    
    if (destpath.length() > 55)
    {
        std::cout << "Error: File name too long\n";
        return -1;
    }
    if (checkIfFileExist(curr_directory_table,sourcepath,0) == false)
    {
        cout << "Error: file doesnt exist\n";
        return -1;
    }
    if (checkIfFileExist(curr_directory_table,destpath,0) == true)
    {
        cout << "Error: Name is allready taken\n";
        return -1;
    }
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        if (curr_directory_table[i].file_name == sourcepath)
        {
            if (sourcepath.length()>= destpath.length())
            {
                for (int j = 0; j < sourcepath.length(); j++)
                {
                    curr_directory_table[i].file_name[j] = destpath[j];
                }
            }
            else
            {
                for (int j = 0; j < destpath.length(); j++)
                {
                    curr_directory_table[i].file_name[j] = destpath[j];
                }
            }
            
            break;
        }
        
    }
    
    updateDisk();
    return 0;
}

// rm <filepath> removes / deletes the file <filepath>
int
FS::rm(std::string filepath)
{
    //cout<<"CURRENT DIR IS : " << currentDir<<endl;
    //std::cout << "FS::rm(" << filepath << ")\n";
    if (checkIfFileExist(curr_directory_table,filepath,0) == false && checkIfFileExist(curr_directory_table,filepath,1) == false )
    {
        cout<<"ERROR: there is no file to remove"<<endl;
        return -1;
    }
    int nextBlock = 0;
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        if (curr_directory_table[i].file_name == filepath)
        {
            if(static_cast<int>(curr_directory_table[i].type) == 1 ){
                cd(filepath);
                bool fileExist = false;
                for (int i = 1; i < TABLE_SIZE; i++)
                {
                    if (curr_directory_table[i].file_name[0] != '\0')
                    {
                        fileExist = true;
                        break;
                    }
                }
                if (fileExist == false)
                {
                    cd("..");
                }
                else{
                    cd("..");
                    cout<<"ERROR: file not empty"<<endl;
                    return -1;
                }
                
            }
            int blocksRequired = getsBlocksRequired(curr_directory_table[i].size);
            int temp = curr_directory_table[i].first_blk;
            if (temp != 0)
            {
                nextBlock = fat[temp];
                fat[temp] = FAT_FREE;
                zeroOutDiskBlock(temp);
                for (int i = 0; i < blocksRequired-1; i++)
                {
                    if (nextBlock == FAT_EOF)
                    {
                        fat[temp] = FAT_FREE;
                        zeroOutDiskBlock(temp);
                        break;
                    }
                    temp = fat[nextBlock];                               
                    fat[nextBlock] = FAT_FREE;
                    zeroOutDiskBlock(nextBlock);
                    nextBlock = fat[temp];
                }
            }
            
            emptyRootBlock(curr_directory_table,i);
            break;
        }  
    }
    if (currentDir == 0)
    {
        for (int i = 0; i < TABLE_SIZE; i++)
        {
            root_directory_table[i] = curr_directory_table[i];
        }
        
    }
    
    updateDisk();
    return 0;
}
int 
FS::append(std::string filepath1, std::string filepath2)
{
    //std::cout << "FS::append(" << filepath1 << "," << filepath2 << ")\n";

    if (!checkIfFileExist(curr_directory_table, filepath1,0))
    {
        std::cout << "ERROR: " << filepath1 << " does not exist" << std::endl;
        return -1;
    }

    uint8_t access_rights_file1 = 0;
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        if (curr_directory_table[i].file_name == filepath1)
        {
            access_rights_file1 = curr_directory_table[i].access_rights;

            // Check for read access
            if (!(access_rights_file1 & 0x04))
            {
                std::cout << "Error: Read access denied for " << filepath1 << ".\n";
                return -1;
            }
        }
    }


    if (!checkIfFileExist(curr_directory_table, filepath2,0))
    {
        std::cout << "ERROR: " << filepath2 << " does not exist" << std::endl;
        return -1;
    }

    uint8_t access_rights_file2 = 0;
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        if (curr_directory_table[i].file_name == filepath2)
        {
            access_rights_file2 = curr_directory_table[i].access_rights;

            // Check for write access
            if (!(access_rights_file2 & 0x02))
            {
                std::cout << "Error: Write access denied for " << filepath2 << ".\n";
                return -1;
            }
        }
    }


    int file1Size = 0, first1Blk = 0;
    int file2Size = 0, first2Blk = 0;
    int file2Type = 0, file2Access = 0;
    int dir_entry_index = -1;
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        if (curr_directory_table[i].file_name == filepath1)
        {
            file1Size = curr_directory_table[i].size;
            first1Blk = curr_directory_table[i].first_blk;
        }
        if (curr_directory_table[i].file_name == filepath2)
        {
            file2Size = curr_directory_table[i].size;
            first2Blk = curr_directory_table[i].first_blk;
            file2Type = curr_directory_table[i].type;
            file2Access = curr_directory_table[i].access_rights;
            dir_entry_index = i;
        }
    }
    if (dir_entry_index == -1)
    {
        std::cout << "Error: Could not find directory entry for " << filepath2 << std::endl;
        return -1;
    }

    std::vector<int> blocks_file2;
    int curr_blk = first2Blk;
    while (curr_blk != FAT_EOF)
    {
        blocks_file2.push_back(curr_blk);
        curr_blk = fat[curr_blk];
    }

    std::vector<int> blocks_file1;
    curr_blk = first1Blk;
    while (curr_blk != FAT_EOF)
    {
        blocks_file1.push_back(curr_blk);
        curr_blk = fat[curr_blk];
    }

    std::vector<uint8_t> content;
    content.reserve(file2Size + file1Size);

    for (int blk : blocks_file2)
    {
        uint8_t buffer[BLOCK_SIZE];
        disk.read(blk, buffer);
        for (int i = 0; i < BLOCK_SIZE && content.size() < file2Size; i++)
            content.push_back(buffer[i]);
    }

    for (int blk : blocks_file1)
    {
        uint8_t buffer[BLOCK_SIZE];
        disk.read(blk, buffer);
        for (int i = 0; i < BLOCK_SIZE && content.size() < file2Size + file1Size; i++)
            content.push_back(buffer[i]);
    }

    content.resize(file2Size + file1Size);
    rm(filepath2);

    int total_blocks_needed = (content.size() + BLOCK_SIZE - 1) / BLOCK_SIZE;
    std::vector<int> free_blocks;
    for (int i = 0; i < FAT_Size; i++)
    {
        if (fat[i] == FAT_FREE)
        {
            free_blocks.push_back(i);
            if (free_blocks.size() == total_blocks_needed)
                break;
        }
    }

    if (free_blocks.size() < total_blocks_needed)
    {
        std::cout << "Not enough space to append files" << std::endl;
        return -1;
    }

    for (size_t i = 0; i < free_blocks.size(); i++)
    {
        int blk = free_blocks[i];
        uint8_t buffer[BLOCK_SIZE];
        size_t offset = i * BLOCK_SIZE;
        size_t bytes_to_copy = std::min((size_t)BLOCK_SIZE, content.size() - offset);

        for (size_t j = 0; j < bytes_to_copy; j++)
        {
            buffer[j] = content[offset + j];
        }

        for (size_t j = bytes_to_copy; j < BLOCK_SIZE; j++)
        {
            buffer[j] = 0;
        }
        disk.write(blk, buffer);
    }
    for (size_t i = 0; i < free_blocks.size(); i++)
    {
        int blk = free_blocks[i];
        if (i == free_blocks.size() - 1)
        {
            fat[blk] = FAT_EOF;
        }
        else
        {
            fat[blk] = free_blocks[i + 1];
        }
    }

    for (size_t i = 0; i < filepath2.size() && i < sizeof(curr_directory_table[dir_entry_index].file_name); i++)
    {
        curr_directory_table[dir_entry_index].file_name[i] = filepath2[i];
    }

    // Ensure null termination for the file name
    for (size_t i = filepath2.size(); i < sizeof(curr_directory_table[dir_entry_index].file_name); i++)
    {
        curr_directory_table[dir_entry_index].file_name[i] = '\0';
    }

    curr_directory_table[dir_entry_index].first_blk = free_blocks[0];
    curr_directory_table[dir_entry_index].access_rights = file2Access;
    curr_directory_table[dir_entry_index].type = file2Type;
    curr_directory_table[dir_entry_index].size = content.size();

    // 10) Write the updated FAT and directory table back to disk
    updateDisk();

    return 0;
}

// mkdir <dirpath> creates a new sub-directory with the name <dirpath>
// in the current directory 
int
FS::mkdir(std::string dirpath)
{
    
    int folderAmount = 1;
    bool inRootDir = false;
    int parentFolder = 0;
    int crr = currentDir;
    vector<string> fileNames;
    if (dirpath[0] =='/')
    {
        fileNames = splitNameIntoFolders(dirpath);
        for (int i = 0; i < TABLE_SIZE; i++)
        {
            curr_directory_table[i] = root_directory_table[i];
        }
        
        currentDir = 0;
        if (currentDir == 0)
        {
            inRootDir = true;
                
        }
        else{
            cout<<"ERROR: we are not in root_Directory cant use/"<<endl;
            return -1;
        }   
    }
    else{
        fileNames = splitNameIntoFolders(dirpath);
    }
    if (dirpath[dirpath.size()-1] == '/')
    {
        cout<<"Error: Enter directory name to create"<<endl;
        return -1;
    }   
    
    
    int folderParent = currentDir;
    bool DirExist = false;

    for (int i = 0; i < fileNames.size(); i++)
    {
        if (i == fileNames.size()-1 && checkSpaceInDisk(1) == true)
        {
            if (fileNames[i] == "..")
            {
                cout<<"ERROR: cant create file called .."<<endl;
                return -1;
            }
            
            
            if (checkIfFileExist(curr_directory_table,fileNames[i],1) || checkIfFileExist(curr_directory_table,fileNames[i],0))
            {
                cout<<"ERROR: FOLDER ALLREADY EXIST"<<endl;
                return -1;
            }
           DirExist = false;
           for (int j = 0; j < TABLE_SIZE; j++)
            {

                if (curr_directory_table[j].file_name[0] == '\0')
                {
                    dir_entry temp_directory_table[TABLE_SIZE];
                    for (int i = 0; i < TABLE_SIZE; i++)
                    {
                        emptyRootBlock(temp_directory_table,i);
                    }
                    for (int i = 0; i < 2; i++)
                    {
                        temp_directory_table[0].file_name[i] = '.';              
                    }
                    temp_directory_table[0].first_blk = folderParent;
                    temp_directory_table[0].type = 1;
                    temp_directory_table[0].access_rights = 7;

                    //create the folder in the parent directory
                    for (int y = 0; y < fileNames[i].size(); y++)
                    {
                        curr_directory_table[j].file_name[y] = fileNames[i][y];
                    }
                    curr_directory_table[j].access_rights = 7;
                    for (int i = 0; i < FAT_Size; i++)
                    {
                        if (fat[i] == FAT_FREE)
                        {
                            curr_directory_table[j].first_blk = i;
                            fat[i] = FAT_EOF;
                            break;
                        }
                    }
                    curr_directory_table[j].size = 0;
                    curr_directory_table[j].type = 1;
                    disk.write(curr_directory_table[j].first_blk,(uint8_t*)temp_directory_table);
                    disk.write(folderParent,(uint8_t*)curr_directory_table);
                    if (fileNames.size()==1 && currentDir == 0)
                    {
                        disk.read(folderParent,(uint8_t*)root_directory_table);
                    }
                    updateDisk();
                    break;
                }
            }
           break;
        }
        else
        {
            if (checkIfFileExist(curr_directory_table,fileNames[i],1))
            {
                for (int j = 0; j < TABLE_SIZE; j++)
                {
                    if (fileNames[i].compare(curr_directory_table[j].file_name) == 0)
                    {
                        //wd_path += "/"+fileNames[i];
                        DirExist = true;
                        folderParent =  curr_directory_table[j].first_blk;
                        cd(fileNames[i]);
                        break;
                    }   
                }               
            }
            else
            {
                disk.read(crr,(uint8_t*)curr_directory_table);
                currentDir = crr;
                cout<<"ERROR: Folder is missing"<<endl;
                return -1;
            }
            
        }
        
    }
    if (fileNames.size() >1)
    {
        for (int i = fileNames.size()-1; i > 0; i--)
        {
            if (fileNames[i] != "..")
            {
                    //cout<<"Heej"<<endl;
                    cd("..");
            }
            else{
                i--;
            }
        }
    }
    
    return 0;
}

// cd <dirpath> changes the current (working) directory to the directory named <dirpath>
int
FS::cd(std::string dirpath)
{
    //std::cout << "FS::cd(" << dirpath << ")\n";
    int folderParent = 0;
    vector<string>fileNames = splitNameIntoFolders(dirpath);
    if (dirpath[0] == '/')
    {
        //cout<<"HEEJ"<<endl;
        for (int i = 0; i < TABLE_SIZE; i++)
        {
            curr_directory_table[i] = root_directory_table[i];
        }
        currentDir = 0;
    }
    // if (dirpath[0] == '.' && dirpath[1]== '.')
    // {
    //     if (currentDir == 0)
    //     {
    //         //cout<<"ERROR: We have no folder this is root"<<endl;
    //         return -1;
    //     }
    //     // size_t pos = wd_path.find_last_of('/'); // Find the last slash
    //     // if (pos != std::string::npos) {
    //     //     wd_path = wd_path.substr(0, pos); // Keep the part before the last slash
    //     // }
    //     currentDir = curr_directory_table[0].first_blk;
    //     disk.read(curr_directory_table[0].first_blk,(uint8_t*)curr_directory_table);
    //     return 0;
    // }
    for (int i = 0; i < fileNames.size(); i++)
    {
        if (checkIfFileExist(curr_directory_table,fileNames[i],1))
        {
           for (int j = 0; j < TABLE_SIZE; j++)
            {
                if (fileNames[i].compare(curr_directory_table[j].file_name) == 0)
                {
                    //wd_path += "/"+fileNames[i];
                    folderParent =  curr_directory_table[j].first_blk;
                    currentDir = curr_directory_table[j].first_blk;
                    disk.read(curr_directory_table[j].first_blk,(uint8_t*)curr_directory_table);
                    break;
                }   
            }

        }
        else{
            cout<<"ERROR: THERE IS NO FILE WITH THE NAME OF: "<< fileNames[i]<<endl;
            return -1;
        }
        
    }
    return 0;
}


// pwd prints the full path, i.e., from the root directory, to the current
// directory, including the currect directory name
int FS::pwd()
{
    // If we are at root directory
    if (currentDir == 0) {
        std::cout << "/" << std::endl;
        return 0;
    }

    // Temporary buffers for directory entries
    dir_entry current_directory_table[TABLE_SIZE];
    dir_entry parent_directory_table[TABLE_SIZE];

    std::string path = ""; // To construct the path as we traverse
    uint32_t child = currentDir;

    // Traverse upwards until we hit the root (which is block 0)
    while (child != 0) {
        // Read the current directory block
        disk.read(child, (uint8_t*)current_directory_table);

        // Find the parent block by looking for ".."
        uint32_t parent = 0;
        for (int i = 0; i < TABLE_SIZE; i++) {
            std::string fname = current_directory_table[i].file_name;
            if (fname == "..") {
                parent = current_directory_table[i].first_blk;
                break;
            }
        }

        if (parent != 0) {
            disk.read(parent, (uint8_t*)parent_directory_table);

            for (int i = 0; i < TABLE_SIZE; i++) {
                std::string fname = parent_directory_table[i].file_name;
                uint32_t blk = parent_directory_table[i].first_blk;

                if (fname != "." && fname != ".." && blk == child) {
                    path = "/" + fname + path; // Prepend the directory name
                    break;
                }
            }

            // Move upwards
            child = parent;
        } else {
            // If parent is 0, find the name of the current directory in the root directory
            disk.read(0, (uint8_t*)parent_directory_table);

            for (int i = 0; i < TABLE_SIZE; i++) {
                std::string fname = parent_directory_table[i].file_name;
                uint32_t blk = parent_directory_table[i].first_blk;

                if (fname != "." && fname != ".." && blk == child) {
                    path = "/" + fname + path; // Prepend the directory name
                    break;
                }
            }

            // Now we have reached root, break the loop
            break;
        }
    }

    // If path is empty, we are at root, so just print "/"
    if (path.empty()) {
        path = "/";
    }

    // Print the constructed path
    std::cout << path << std::endl;

    return 0;
}

// chmod <accessrights> <filepath> changes the access rights for the
// file <filepath> to <accessrights>.
int
FS::chmod(std::string accessrights, std::string filepath)
{
    std::cout << "FS::chmod(" << accessrights << "," << filepath << ")\n";

    // Convert accessrights from string to integer
    if (accessrights.length() != 1 || (accessrights[0] < '0' || accessrights[0] > '7'))
    {
        std::cout << "Error: Invalid access rights format. Must be a single digit between 0 and 7.\n";
        return -1;
    }
    uint8_t new_rights = accessrights[0] - '0';

    // Search for the file in the root directory table
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        if (filepath == curr_directory_table[i].file_name) // Match file name
        {
            // Update access rights
            curr_directory_table[i].access_rights = new_rights;
            std::cout << "Access rights updated successfully.\n";

            // Update the disk with the new directory table
            updateDisk();
            return 0;
        }
    }

    // File not found
    std::cout << "Error: File not found.\n";
    return -1;
}