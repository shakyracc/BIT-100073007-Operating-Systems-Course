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

# 实验要求

1、失败原因（25分）
    
     1.不少于1000字
        
     2.PDF格式
        
2.实验1-5的实验报告（7分）
    
3.实验1-5的实验视频（8分）
    
     1. 人脸自拍+学生证
        
     2.程序源码
        
     3. 口头解释节目思路
        
     4.运行程序
        
     5.程序结果说明（实验1可以截图说明）

＃ 介绍

任务的目的是理解

- Linux 编程基础
    
     - 编程时的常用命令和工具
        
     - API：进程控制、文件操作、信号、管道、共享内存、信号量
        
- Win32 API
    
     - 进程控制，内存管理文件操作
        

任务包括

- 编译 Linux 内核
    
- 过程控制（Linux 和 Windows）
    
- 进程同步（Linux 和 Windows）
    
- 文件操作（Linux 和 Windows）
    

＃＃＃ 背景

操作系统是用于为用户和应用程序管理计算机资源的软件。 
行业中占主导地位的操作系统是 Microsoft Windows、macOS 和各种 Linux 
发行版。

  
  

编程时最大的挑战之一是操作系统之间的可移植性。 
在许多情况下，在一个操作系统上运行的代码可能无法在另一个操作系统上运行。 
为了提高操作系统之间的兼容性，引入了 POSIX（便携式操作系统接口）标准。 
它定义了系统和用户级应用程序编程接口 (API)，用于软件与 Unix 
和其他操作系统的变体的可移植性。 不幸的是，Windows 不兼容 POSIX。 
在这个实验中，我们将探索在 POSIX 兼容系统（如 Linux）和非兼容系统（如 
Windows）中编程之间的差异。

系统调用 (syscall) 
是计算机程序从其执行所在的操作系统内核请求服务的编程方式。 
这可能包括流程的创建和执行。 它们提供进程和操作系统之间的基本接口。 
大多数系统都提供 API 来触发系统调用服务。

在 POSIX 兼容系统上，系统调用 API 通常是 C 库 (libc) 实现的一部分，例如 
glibc，它为系统调用提供包装函数，通常与它们调用的系统调用同名。 
例如，_fork_ 和 _execve_ 是 C 库函数，它们依次执行调用 _fork_ 和 _exec_ 
系统调用的指令。

Win32 是 Microsoft Windows 操作系统中可用的 API。 它（除其他外）提供对 
Windows 
系统可用的基本资源的访问，包括文件系统、设备、**进程**、**线程**和错误处理。 
这些在 32 位和 64 位 Windows 上的 kernel32.dll 和 KernelBase.dll 
中起作用。 Microsoft 在此处详细记录了 Win32 
系统服务：[https://docs.microsoft.com/en-us/windows/win32/system-services](https://docs.microsoft.com/en-us/windows 
/win32/系统服务）

### 所需环境

我将用 C 语言实现所有赋值解决方案。 Linux和Windows编译C的环境要求不同。

术语编译器是指将源代码从高级编程语言转换为低级编程语言（机器级代码）以构建可执行程序文件的软件。

每个实验可能需要额外的要求。 我将在接下来的章节中介绍这些内容。

**Linux**

在 Linux 中，需要 GCC 编译器它是用 C、C++、Fortran 
等编写的程序的编译器和库的集合……它是 Ubuntu 存储库中 build-essential 
包的一部分。 build-essential 包带有五个独立的包，包括 GCC 编译器。 
这五个包包括：

- GCC：GCC 编译器，用于编译用 C 语言编写的程序。
    
- G++” GNU C++ 编译器，用于编译用 C++ 语言编写的程序
    
- libc6-dev：GNU 库文件包。 了解如何在 Linux 中编译 C 
程序所需的开发库和头文件。
    
- Make：这个工具读取一个名为“makefile”的文件，它告诉编译器如何完成它的工作
    
- dpkg-dev：用于为基于 Debian 的系统打包程序。
    

**视窗**

要在 Windows 中编译 C，需要安装 Visual Studio 的构建工具。 Visual Studio 
是一个功能强大的集成开发环境，支持多种语言和平台的全功能编辑器、资源管理器、调试器和编译器。

要使用命令行环境，需要开发人员命令提示符窗口。 
它是一个常规的命令提示符窗口，其中设置了所有必需的环境变量。

#References
    • https://docs.microsoft.com/en-us/cpp/build/walkthrough-compile-a-c-program-on-the-command-line?view=msvc-170 
    • https://www.scaler.com/topics/c/how-to-compile-c-program-in-linux/ 
