#include <unistd.h> 
#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h> // lstat(), chmod()
#include <fcntl.h>
#include <utime.h>

/* buffer size is set to 4096 because thats the tyical sector size on a disk.
increasing the buffer size beyond 4096 has little positive effect. */
const int BUFFER = 4096;

/* maximum path length limitation */
const int MAX_PATH = 260;

void searchDirectory(char sourcePath[MAX_PATH], char targetPath[MAX_PATH]);

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        printf("usage: ./mycp <source-directory> <targetDirectory>/n");
        return -1;
    }

    /* taking the contents of argv[] and adding them to character array source and target for clarity and ease*/
    char source[MAX_PATH], target[MAX_PATH];
    /* sprintf stands for “String print”. Instead of printing on console, it store output on char buffer which are specified in sprintf. */
    sprintf(source, "%s", argv[1]);
    sprintf(target, "%s", argv[2]);

    /*
    int lstat(const char *pathname, struct stat *fileAttributeBuffer);
        const char *pathname 	// The path of the file or directory whose attributes need to be extracted
        struct stat *fileAttributeBuffer 	// The acquired attribute storage buffer
        return int 		        // Returns 0 successfully, otherwise -1
    */
    struct stat fileAttributeBuffer;
    struct utimbuf timebuf; 

    // check if the source file exists and open the directory

    DIR *sourceDir;
    DIR *targetDir;

    if ((sourceDir = opendir(source)) == NULL) //  opendir - open a directory
    {
        printf("Error: Source directory not found/n");
    }

    // Create target directory (if there is no target directory )
    if ((targetDir = opendir(target)) == NULL)
    {
        // Store the attributes of the source file into fileAttributeBuffer
        stat(source, &fileAttributeBuffer); // stat() - get file attributes

        // Create target file directory
        // st_mode = File type and mode
        mkdir(target, fileAttributeBuffer.st_mode); // mkdir() - create a directory

        // Record the time of file access and modification
        timebuf.actime = fileAttributeBuffer.st_atime;  // access time = time of last access
        timebuf.modtime = fileAttributeBuffer.st_mtime; // modification time = time of last modification

        // Record the access and modification time of the target file
        utime(target, &timebuf);
    }

    searchDirectory(source, target);

    return 0;
}

void searchDirectory(char sourcePath[MAX_PATH], char targetPath[MAX_PATH])
{
    char source[MAX_PATH];
    char target[MAX_PATH];

    strcpy(source, sourcePath); // strcpy - copy a string
    strcpy(target, targetPath);

    DIR *sourceDir = opendir(source);
    DIR *targetDir = opendir(target);

      /* the dirent structure 
      struct dirent {
        ino_t          d_ino;       // Inode number 
        off_t          d_off;       // Not an offset; see below 
        unsigned short d_reclen;    // Length of this record 
        unsigned char  d_type;      // Type of file; not supported by all filesystem types 
        char           d_name[256]; // Null-terminated filename 
      };
  */
    struct dirent *entry;

    /* The stat structure
      struct stat {
        struct timespec st_atim; // Time of last access
        struct timespec st_mtim;  // Time of last modification
        struct timespec st_ctim;  // Time of last status change
      };
  */
    struct stat fileAttributeBuffer;

      /* The utimbuf structure is:
      struct utimbuf {
        time_t actime;   // access time
        time_t modtime;  // modification time
      };
  */
    struct utimbuf timebuf;

    char buffer[BUFFER];

    while ((entry = readdir(sourceDir)) != NULL)
    {
        lstat(entry->d_name, &fileAttributeBuffer);
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }
        else if (S_ISDIR(fileAttributeBuffer.st_mode)) // directory
        {
            // change path of source and target
            strcat(source, "/");
            strcat(source, entry->d_name);
            strcat(target, "/");
            strcat(target, entry->d_name);

            stat(source, &fileAttributeBuffer);

            mkdir(target, fileAttributeBuffer.st_mode);
            timebuf.actime = fileAttributeBuffer.st_atime;
            timebuf.modtime = fileAttributeBuffer.st_mtime;

            // int utime(const char *filename, const struct utimbuf *times);
            utime(target, &timebuf); // change file last access and modification times

            // call search functions
            searchDirectory(source, target);

            strcpy(source, sourcePath); // strcpy - copy a string
            strcpy(target, targetPath);
        }
        else if (S_ISREG(fileAttributeBuffer.st_mode)) // normal file
        {
            // change path of source and target
            strcat(source, "/");
            strcat(source, entry->d_name);
            strcat(target, "/");
            strcat(target, entry->d_name);

            // start copy file
            int sourceData = open(source, O_RDONLY);
            if (sourceData == -1)
            {
                printf("Error: Failed to open file./n");
            }

            stat(source, &fileAttributeBuffer);

            int targetData = creat(targetPath, fileAttributeBuffer.st_mode);
            if(targetData == -1)
            {
                printf("Error: File creation failed./n");
            }

            int wordbit;
            while ((wordbit = read(sourceData, buffer, BUFFER)) > 0)
            {
                if (write(targetData, buffer, wordbit) != wordbit)
                {
                printf("Error: Write file failed./n"); 
                }

                timebuf.actime = fileAttributeBuffer.st_atime;
                timebuf.modtime = fileAttributeBuffer.st_mtime;
                close(sourceData);
                close(targetData);

            // end of copy file

            strcpy(source, sourcePath);
            strcpy(target, targetPath);
        }
    }
}
}