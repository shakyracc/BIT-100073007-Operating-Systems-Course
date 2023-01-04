#include <iostream>
#include <unistd.h>    // read(), write() - read/write from/to a file descriptor
#include <sys/types.h> // opendir()
#include <dirent.h>    // opendir(), readdir()
#include <sys/stat.h>  // stat(), mkdir()
#include <utime.h>     // utime()
#include <fcntl.h>     // open(), creat() = open and possibly create a file
#include <string.h>    // strcmp() - compare two strings

/* using namespace std means we use the namespace named std (standard).*/
using namespace std;

/* buffer size is set to 4096 because thats the tyical sector size on a disk.
increasing the buffer size beyond 4096 has little positive effect. */
const int BUFFER = 4096;

/* ??? */
const int MAX_PATH = 260;

void copyFile(char sourceDir[MAX_PATH], char targetDir[MAX_PATH])
{
  int fsourceData = open(sourceDir, O_RDONLY); // int open(const char *pathname, int flags);
  if (fsourceData == -1)                       // On error, -1 is returned.
  {
    printf("Error: Open file failed./n");
  }

  struct stat statbuf;
  struct utimbuf timebuf;
  char buffer[BUFFER];
  int wordbit;

  stat(sourceDir, &statbuf);

  // int creat(const char *pathname, mode_t mode); 
  //st_mode = File type and mode
  int ftargetData = creat(targetDir, statbuf.st_mode); // creat() - open and possibly create a file
  if (ftargetData == -1)
  {
    printf("Error: Create file failed./n"); 
  }

  while ((wordbit = read(fsourceData, buffer, BUFFER)) > 0)
  {
    if (write(ftargetData, buffer, wordbit) != wordbit)
    {
      printf("Error: Write file failed./n"); 
    }

    timebuf.actime = statbuf.st_atime;
    timebuf.modtime = statbuf.st_mtime;
    close(fsourceData);
    close(ftargetData);
  }
}

void copyDirectory(char sourceDir[MAX_PATH], char targetDir[MAX_PATH])
{
  char source[MAX_PATH];
  char target[MAX_PATH];

  /* The stat structure
      struct stat {
        struct timespec st_atim; // Time of last access
        struct timespec st_mtim;  // Time of last modification
        struct timespec st_ctim;  // Time of last status change
      };
  */
  struct stat statbuf;

  /* The utimbuf structure is:
      struct utimbuf {
        time_t actime;   // access time
        time_t modtime;  // modification time
      };
  */
  struct utimbuf timebuf;

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

  DIR *dir;

  strcpy(source, sourceDir); // strcpy - copy a string
  strcpy(target, targetDir);

  dir = opendir(source);

  while ((entry = readdir(dir)) != NULL)
  {

    // Current directory "." Parent directory ".."
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
    {
      continue;
    }
    // folder!
    if (entry->d_type == 4)
    {
      strcat(source, "/");
      strcat(source, entry->d_name);
      strcat(target, "/");
      strcat(target, entry->d_name);

      stat(source, &statbuf);

      mkdir(target, statbuf.st_mode);
      timebuf.actime = statbuf.st_atime;
      timebuf.modtime = statbuf.st_mtime;

      // int utime(const char *filename, const struct utimbuf *times);
      utime(target, &timebuf); // change file last access and modification times

      copyDirectory(source, target);

      strcpy(source, sourceDir);
      strcpy(target, targetDir);
    }
    // not a directory, but a file
    else
    {
      strcat(source, "/");
      strcat(source, entry->d_name);
      strcat(target, "/");
      strcat(target, entry->d_name);

      copyFile(source, target);
      strcpy(source, sourceDir);
      strcpy(target, targetDir);
    }
  }
}

int main(int argc, char const *argv[])
{
  if (argc != 3)
  {
    printf("usage: ./mycp <source-directory> <targetDirectory>/n");
    return -1;
  }

  DIR *dir;
  struct stat statbuf;
  struct utimbuf timebuf;

  /* taking the contents of argv[] and adding them to character array
  source and target for clarity and ease*/

  char source[MAX_PATH];
  char target[MAX_PATH];

  /* sprintf stands for “String print”. Instead of printing on console,
  it store output on char buffer which are specified in sprintf. */
  sprintf(source, "%s", argv[1]);
  sprintf(target, "%s", argv[2]);

  // check if the source file exists and open the directory

  if ((dir = opendir(source)) == NULL) //  opendir - open a directory
  {
    printf("Error: Source directory not found/n");
  }

  // Create object file (if there is no object file)
  if ((dir = opendir(target)) == NULL)
  {

    // Store the attributes of the source file into statbuf
    stat(source, &statbuf); // stat() - get file status

    // Create target file directory
    // st_mode = File type and mode
    mkdir(target, statbuf.st_mode); // mkdir() - create a directory

    // Record the time of file access and modification
    timebuf.actime = statbuf.st_atime;  // access time = time of last access
    timebuf.modtime = statbuf.st_mtime; // modification time = time of last modification

    // Record the access and modification time of the target file
    utime(target, &timebuf);
  }

  // start copying!
  copyDirectory(source, target);

  return 0;
}