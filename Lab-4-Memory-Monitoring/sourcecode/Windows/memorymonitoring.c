#include <windows.h>
#include <stdio.h>

void PrintHardDiskInfo()
{
   printf("\nHARD DISK INFORMATION");
   printf("\n---------------------\n");
   printf("s Free : 10dGB Total : 10dGB NTFS\n");

}

void PrintPhysicalMemoryInfo()
{
   printf("\nPHYSICAL MEMORY INFORMATION\n");
   printf("------------------------------\n");
   printf("Total Physical Memory: 10dMB\n");
   printf("Available Physical Memory: 10dMB\n");
   printf("Used Physical Memory: 10dMB\n");
}

void PrintSystemAddressLayout()
{
   printf("\nSYSTEM ADDRESS LAYOUT\n");
   printf("------------------------------\n");
}

void PrintVirtualAddressSpaceLayout()
{
   printf("\nVIRTUAL ADDRESS SPACE LAYOUT\n");
   printf("------------------------------\n");
}

void RunParentProcess()
{
   printf("\nRUNNING PARENT PROCESS\n");
   printf("------------------------------\n");
}

void PrintRunningProcesses()
{
   printf("\nCURRENTLY RUNNING PROCESSES\n");
   printf("------------------------------\n");
}

int main()
{

   printf("LAB 4: MEMORY MONITORING\n");

   PrintHardDiskInfo();
   PrintPhysicalMemoryInfo();
   PrintSystemAddressLayout();
   PrintVirtualAddressSpaceLayout();
   RunParentProcess();
   PrintRunningProcesses();

   return 0;
}
