#include <unistd.h> // getcwd() - get current working directory
                    // chdir() - change working directory
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h> // lstat(), chmod()
#include <fcntl.h>
#include <utime.h>
#include <time.h>
#include <sys/time.h>

/* buffer size is set to 4096 because thats the tyical sector size on a disk.
increasing the buffer size beyond 4096 has little positive effect. */
const int BUFFER = 4096;

/* maximum path length limitation */
const int MAX_PATH = 260;

// directory replication using dfs or bfs to search is equivalent
void searchDirectory(char *sourcePath, char *targetPath);

// copy files
void copyFile(const char *source_file, const char *dst_file);

// copy directory
void copyDirectory(const char *source_dir, const char *dst_dir);

// copy soft link
// void copy_sln(const char *src_file, const char *dst_file);

// Modify the file attributes to be consistent with the source file
void changeAttributes(const char *source, const char *dst);

// modify path
void changePath(char *source, char *cat);

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        printf("usage: ./mycp <source-directory> <targetDirectory>/n");
        return -1;
    }

    char *current_dir = getcwd(NULL, 0);

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

    // check if the source file exists and open the directory

    if ((dir = opendir(source)) == NULL) //  opendir - open a directory
    {
        printf("Error: Source directory not found/n");
    }

    // Create target directory (if there is no target directory )
    if ((dir = opendir(target)) == NULL)
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

    // printf("Searching directory:    %s\n", getcwd(NULL, 0));
    DIR *source_dir = opendir(sourcePath);
    DIR *target_dir = opendir(targetPath);
    struct dirent *entry = NULL;
    struct stat fileAttributeBuffer;
    struct utimbuf timebuf;
    char buffer[BUFFER];

    while ((entry = readdir(source_dir)) != NULL)
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

            // call copy function
            copyFile(source, target);

            int sourceData = open(source, 0_RDONLY);
            if (sourceData == -1)
            {
                printf("Error: Failed to open file./n");
            }

            while(read(sourceData, buffer, BUFFER)) > 0)
            {
                write(targetData)
            }

            strcpy(source, sourceDir);
            strcpy(target, targetDir);
        }
    }
}

void copyFile(const char *source_file, const char *target_file)
{
    int source_fd = open(source_file, O_RDONLY);
    int target_fd = creat(target_file, O_WRONLY);

    unsigned char buf[1024];
    while (read(source_fd, buf, sizeof(buf)) > 0)
    {
        write(target_fd, buf, sizeof(buf));
    }

    changeAttributes(source_file, target_file);

    close(source_fd);
    close(target_fd);
}

void copy_sln(const char *source_file, const char *dst_file)
{
    char buf[1024];
    memset(buf, 0, sizeof(buf));
    int len = 0;
    if ((len = readlink(source_file, buf, sizeof(buf))) > 0)
    {
        printf("%s\n", buf);
        if (symlink(buf, dst_file) == -1)
        {
            perror("symlink");
        }
    }
    changeAttributes(source_file, dst_file);
}

void changeAttributes(const char *source, const char *target)
{
    struct stat attr_of_source;
    lstat(source, &attr_of_source);
    // Modify file attributes
    chmod(dst, attr_of_source.st_mode); // change permission of directory
    // Modify file user group
    chown(dst, attr_of_source.st_uid, attr_of_source.st_gid);

    // Modify file access, modification time
    struct timeval time_buf[2];
    time_buf[1].tv_sec = attr_of_source.st_mtime;
    time_buf[0].tv_sec = attr_of_source.st_atime;
    if (lutimes(dst, time_buf) == -1)
    {
        printf("%s\n", dst);
        perror("lutimes");
    }

    struct utimbuf tbuf;
    tbuf.actime = attr_of_source.st_atime;
    tbuf.modtime = attr_of_source.st_mtime;
    utime(dst, &tbuf);

    struct stat dst_attr_of_source;
    lstat(dst, &dst_attr_of_source);
    if (dst_attr_of_source.st_mtime != attr_of_source.st_mtime)
        printf("%s : %d\n", dst, attr_of_source.st_mtime);

    mkdir(target, fileAttributeBuffer.st_mode);
    timebuf.actime = fileAttributeBuffer.st_atime;
    timebuf.modtime = fileAttributeBuffer.st_mtime;

    // int utime(const char *filename, const struct utimbuf *times);
    utime(target, &timebuf); // change file last access and modification times
}

void changePath(char *source, char *cat)
{
    strcat(source, (char *)"/");
    strcat(source, cat);
}