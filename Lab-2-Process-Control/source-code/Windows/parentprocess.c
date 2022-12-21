#include <windows.h>
#include <stdio.h>
#include <tchar.h>

void _tmain(int argc, TCHAR *argv[])
{

    SYSTEMTIME st, et; // pointer to a SYSTEMTIME structure to receive the current system date and time.

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
	
	// argv[2] = "childprocess";

   printf("\nRUNNING PARENT PROCESS\n");
   printf("------------------------------\n");

    if (argc != 2) /* argc should be 2 for correct execution */
    {
        printf("Usage in parentprocess: %s [cmdline]\n", argv[0]);
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

    GetSystemTime(&st);
    printf("The child process start time is: %02dh:%02dm:%02ds.%02dms\n", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

    // Wait until child process exits.
    WaitForSingleObject(pi.hProcess, INFINITE);
    
    GetSystemTime(&et);
    printf("The child process end time is: %02dh:%02dm:%02ds.%02dms\n", et.wHour, et.wMinute, et.wSecond, et.wMilliseconds);

    printf("The child process elapsed time is: %02ds.%02dms\n", et.wSecond-st.wSecond, et.wMilliseconds-st.wMilliseconds);

    // Close process and thread handles.
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}