#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <process.h>
#include <Psapi.h>
#pragma comment(lib, "user32.lib")

// Use to convert bytes to KB
#define DIV 1024

// Specify the width of the field in which to print the numbers.
// The asterisk in the format specifier "%*I64d" takes an integer
// argument and uses it to pad and right justify the number.
#define WIDTH 7

void PrintSystemInfo()
{
   printf("\nSYSTEM INFORMATION");
   printf("\n---------------------\n");

   SYSTEM_INFO siSysInfo;

   // Copy the hardware information to the SYSTEM_INFO structure.

   GetSystemInfo(&siSysInfo);

   // Display the contents of the SYSTEM_INFO structure.

   printf("  OEM ID: %u\n", siSysInfo.dwOemId);
   printf("  Number of processors: %u\n",
          siSysInfo.dwNumberOfProcessors);
   printf("  Page size: %u\n", siSysInfo.dwPageSize);
   printf("  Processor type: %u\n", siSysInfo.dwProcessorType);
   printf("  Minimum application address: %lx\n",
          siSysInfo.lpMinimumApplicationAddress);
   printf("  Maximum application address: %lx\n",
          siSysInfo.lpMaximumApplicationAddress);
   printf("  Active processor mask: %u\n",
          siSysInfo.dwActiveProcessorMask);
}

void PrintMemoryInfo()
{
   printf("\nMEMORY INFORMATION\n");
   printf("------------------------------\n");

   MEMORYSTATUSEX ms = {sizeof(MEMORYSTATUSEX)};

   ms.dwLength = sizeof(ms);

   // Retrieves information about the system's currenT usage of physical memory

   GlobalMemoryStatusEx(&ms);

   printf("Total memory in use: %ld%%\n", ms.dwMemoryLoad);

   printf("\nTotal Physical Memory     : %8.2I64fMB \nAvailable Physical Memory : %8.2I64fMB \nUsed Physical Memory      : %8.2I64fMB \n\n", ms.ullTotalPhys / (1024 * 1024.0), ms.ullAvailPhys / (1024 * 1024.0), ms.ullTotalPhys / (1024 * 1024.0) - ms.ullAvailPhys / (1024 * 1024.0));

   printf("Total Virtual Memory     : %8.2I64fMB \nAvailable Virtual Memory : %8.2I64fMB \nUsed Virtual Memory      : %8.2I64fMB \n\n", ms.ullTotalVirtual / (1024 * 1024.0), ms.ullAvailVirtual / (1024 * 1024.0), ms.ullTotalVirtual / (1024 * 1024.0) - ms.ullAvailVirtual / (1024 * 1024.0));
}

void PrintPerformanceInfo()
{
   printf("\nPERFORMANCE INFORMATION\n");
   printf("------------------------------\n");

   PERFORMANCE_INFORMATION siPerfInfo;

   // Copy the hardware information to the SYSTEM_INFO structure.

   GetPerformanceInfo(&siPerfInfo, siPerfInfo.cb);

   printf("Commit Total\t\t: %d pages\n", siPerfInfo.CommitTotal);
   printf("Commit Limit\t\t: %d pages\n", siPerfInfo.CommitLimit);
   printf("Commit Peak\t\t: %d pages\n", siPerfInfo.CommitPeak);
   printf("Physical Total\t\t: %d pages\n", siPerfInfo.PhysicalTotal);
   printf("Physical Available\t: %d pages\n", siPerfInfo.PhysicalAvailable);
   printf("System Cache\t\t: %d pages\n", siPerfInfo.SystemCache);
   printf("Kernel Total\t\t: %d pages\n", siPerfInfo.KernelTotal);
   printf("Kernel Paged\t\t: %d pages\n", siPerfInfo.KernelPaged);
   printf("Kernel Nonpaged\t\t: %d pages\n", siPerfInfo.KernelNonpaged);
   printf("Page Size\t\t: %d MB\n", siPerfInfo.PageSize / (1024 * 1024.0));
   printf("Handle Count\t\t: %d handles\n", siPerfInfo.HandleCount);
   printf("Process Count\t\t: %d processes\n", siPerfInfo.ProcessCount);
   printf("Thread Count\t\t: %d threads\n", siPerfInfo.ThreadCount);
}

void PrintRunningProcesses( void )
{

   system("tasklist /FI \"IMAGENAME eq parentprocess.exe\""); 

   system("tasklist /FI \"IMAGENAME eq childprocess.exe\"");
}

void _tmain(int argc, TCHAR *argv[])
{

   printf("LAB 4: MEMORY MONITORING\n");

   PrintSystemInfo();
   PrintMemoryInfo();
   PrintPerformanceInfo();

   STARTUPINFO si;
   PROCESS_INFORMATION pi;
   ZeroMemory(&si, sizeof(si));
   si.cb = sizeof(si);
   ZeroMemory(&pi, sizeof(pi));
   if (argc != 3) /* argc should be 2 for correct execution */
   {
      printf("Usage in memorymonitoring: %s [cmdline]\n", argv[0]);
      return;
   }
   // Start the child process.
   if (!CreateProcess(NULL,    // No module name (use command line)
                      argv[1], // Command line
                      NULL,    // Process handle not inheritable
                      NULL,    // Thread handle not inheritable
                      FALSE,   // Set handle inheritance to FALSE
                      0,       // No creation flags
                      NULL,    // Use parent's environment block
                      NULL,    // Use parent's starting directory
                      &si,     // Pointer to STARTUPINFO structure
                      &pi)     // Pointer to PROCESS_INFORMATION structure
   )
   {
      printf("CreateProcess failed (%d).\n", GetLastError());
      return;
   }

   PrintRunningProcesses();

   // Just ask tasklist command

   Sleep(5000);
   
   return 0;
}
