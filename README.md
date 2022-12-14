# BIT 100073007 Operating Systems Course

# Experiment Requirements

1.  Reason for failure (25 points)
    
    1.  No less than 1,000 words
        
    2.  PDF format
        
2.  Experiment reports of experiments 1-5 (7 points)
    
3.  Experiment videos of experiments 1-5 (8 points)
    
    1.  Selfie with face + student ID
        
    2.  Program source code
        
    3.  Vocally explain program ideas
        
    4.  Run the program
        
    5.  Explanation of program results (Experiment 1 can be explained with 
screenshots)

# Introduction

The purpose of the assignment is to understand

-   Linux programming fundamentals
    
    -   common command and tools when programming
        
    -   APIs: process control, file operations, signals, pipes, shared 
memory, semaphores
        
-   Win32 APIs
    
    -   process control, memory management file manipulation
        

The assignment includes

-   Compiling the Linux kernel
    
-   Process Control (Linux and Windows)
    
-   Process synchronization (Linux and Windows)
    
-   File Operations (Linux and Windows)
    

### Background

An operating system is a piece of software used to manage a computer’s 
resources for its users and applications. The dominant operating systems 
in the industry are Microsoft Windows, macOS, and various Linux 
distributions.

  
  

One of the biggest challenges when programming is portability between 
operating systems. In many cases, code that works on one operating system 
may not work on another. To improve compatibility between operating 
systems, the POSIX (Portable Operating System Interface) standard was 
introduced. It defines both the system and user-level application 
programming interfaces(API) for software portability with variants of Unix 
and other operating systems. Windows unfortunately isn’t POSIX compliant. 
In this experiment, we will explore the differences between programming in 
a POSIX-compliant system like Linux and a non-compliant system like 
Windows.

A system call (syscall) is the programmatic way in which a computer 
program requests a service from the kernel of the operating system on 
which it is executed. This may include the creation and execution of 
processes. They provide an essential interface between a process and the 
operating system. Most systems provide an API to trigger the syscall 
service.

On POSIX-compliant systems the system call API is usually part of an 
implementation of the C library (libc), such as glibc, that provides 
wrapper functions for the system calls, often named the same as the system 
calls they invoke. For example, _fork_ and _execve_ are C library 
functions that in turn execute instructions that invoke the _fork_ and 
_exec_ system calls.

Win32 is an API available in the Microsoft Windows operating systems. It 
provides (among many other things) access to the basic resources available 
to a Windows system including file systems, devices,  **processes**, 
**threads**, and error handling. These function inside kernel32.dll and 
KernelBase.dll on 32 and 64 bit Windows. The Win32 system services are 
well documented by Microsoft here: 
[https://docs.microsoft.com/en-us/windows/win32/system-services](https://docs.microsoft.com/en-us/windows/win32/system-services)

### Required Environment

I will be implementing all assignment solutions in C language. The 
environment requirements are different for compiling C in Linux and 
Windows.

The term compiler refers to the piece of software that converts source 
code from a high-level programming language to a low-level programming 
language (machine-level code) to build an executable program file.

Additional requirements may be needed for each experiment. I will cover 
these in the coming chapters.

**Linux**

In Linux, the GCC compiler is needed It is a collection of compilers and 
libraries for programs written in C, C++, Fortran, etc... It is a part of 
the build-essential package in Ubuntu repositories. The build-essential 
package comes with five separate packages including the GCC compiler. 
These five packages include:

- GCC: The GCC compiler for the compilation of programs written in C 
language.
    
-   G++” The GNU C++ compiler for compilation of programs written in C++ 
language
    
-   libc6-dev: The GNU library files package. The development libraries 
and header files needed to know how to compile the C program in Linux.
    
-   Make: This tool reads a file called “makefile” which tells the 
compiler how to do its job
    
-   dpkg-dev: used to package programs for Debian-based systems.
    

**Windows**

To compile C in Windows Build Tools for Visual Studio needs to be 
installed. Visual Studio is a powerful integrated development environment 
that supports a full-featured editor, resource managers, debuggers, and 
compilers for many languages and platforms.

To use the command-line environment the developer command prompt window is 
needed. It is a regular command prompt window that has all the required 
environment variable set. 
