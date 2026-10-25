# Introduction to Process Modeling in UNIX/Linux

- Student: Adriana Nafeesa Binti Mohd Miswari
- Student ID: 2025124835
- Course: ITT440 – Network Programming

# 1. Introduction

Process modeling is an important concept in operating systems and network programming. It helps explain how programs execute, how processes are created, and how the operating system manages running tasks. A process is a program that is currently being executed and requires resources such as CPU time and memory.

In UNIX/Linux, processes can be created and managed using system calls such as fork() and wait(). Understanding these concepts helps programmers develop applications that perform tasks in an organized and efficient way .

# 2. Understanding Processes

A process is a program in execution. When a program is loaded into memory and starts running, it becomes a process. The operating system tracks and manages each process during its execution (GeeksforGeeks, 2024).

Each process has several important attributes:

- Process ID (PID): A unique identifier assigned to the process.
- Process state: Indicates whether the process is running, ready, or waiting.
- Memory: Stores the instructions and data used by the process.
- CPU resources: Allows the process to execute instructions.
- Open files and resources: Records resources that the process is using.
- There are two important types of processes in process creation:
- Parent process: The original process that creates another process.
- Child process: The new process created by the parent process.

A parent process can create one or more child processes, forming a process hierarchy. Each child process has its own PID and can execute tasks independently of its parent.

# 3. Process Creation Using fork()

In UNIX/Linux, the fork() system call creates a new child process by duplicating the calling process. After the call, both the parent and child processes continue execution from the point where fork() returns.

The return value of fork() identifies which process is executing:
- 0: The current process is the child process.
- Positive value: The current process is the parent, and the value is the child's PID.
- -1: Process creation failed.

The wait() function allows the parent process to wait for a child process to terminate. This is useful when the parent must wait for the child's task to finish before continuing.

For example, in a Food Ordering System, the parent process can receive an order while the child process prepares the food. The parent can then wait for the child before displaying the final message.

# 4. Comparison Between Parent and Child Processes

|       Feature         |   Parent Process                          |       Child Process                |
|-----------------------|-------------------------------------------|------------------------------------|
|Definition             |Creates another process                    |Created by a parent process         |
|Process ID             |Has its own PID                            |Has a different PID                 |
|Creation               |Already exists before the child is created |Created using fork()                |
|Execution              |Can continue its own tasks                 | Can execute its own tasks          |
|Return value of fork() |Receives the child's PID if successful     |Receives 0                          |
|Synchronization        |Can use wait() to wait for a child         |Performs its task and may terminate |
|Example                |Receives a food order                      |Prepares the food                   |

The parent and child initially execute the same program, but the return value of fork() allows them to follow different execution paths. The operating system schedules their execution, so their tasks may run in an order that is not always predictable.

# 5. Programming Example: Food Ordering System

This example demonstrates process modeling using the C programming language. The parent process receives the food order, while the child process simulates preparing the food.

# 5.1 C Program
```
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
int main() {
    pid_t pid = fork();

    if (pid == 0) {
        printf("Child: Preparing food...\n");
    }
    else if (pid > 0) {
        printf("Parent: Order received.\n");
        wait(NULL);
        printf("Parent: Food is ready!\n");
    }
    else {
        printf("Process creation failed.\n");
    }

    return 0;
```

# 5.2 Explanation of the Program

The program begins by calling fork() to create a child process.

- Step 1: The parent process creates a child using fork().
- Step 2: If fork() returns 0, the child displays the message that food is being prepared.
- Step 3: If fork() returns a positive value, the parent displays that the order has been received.
- Step 4: The parent calls wait(NULL) to wait for the child process to finish.
- Step 5: After the child terminates, the parent displays that the food is ready.
- Step 6: If fork() fails, the program displays an error message.

# 5.3 How to Compile and Run

Save the program as food_order.c. Open a UNIX/Linux terminal and execute the following commands:

```
gcc food_order.c -o food_order
./food_order
```

This compiles the C program and runs the resulting executable.

# 5.4 Example Output

```
Parent: Order received.
Child: Preparing food...
Parent: Food is ready!
```

The output demonstrates how the parent and child processes perform separate tasks. The parent waits for the child to finish before displaying the final message.

# 5.5 Application of Process Modeling

The Food Ordering System illustrates how process modeling can represent tasks in a real-world application.

- The parent process represents the order management function.
- The child process represents the food preparation function.
- The fork() function creates a separate process for food preparation.
- The wait() function coordinates the completion of the preparation task.

This is a simplified demonstration. A real food ordering application would require additional features such as customer input, order details, payment processing, and communication between different system components.

# 6. Conclusion

Process modeling is an important concept for understanding how operating systems create, manage, and coordinate running programs. It helps explain multitasking, resource management, process synchronization, and the relationship between parent and child processes.

Although process creation provides benefits such as task separation and process isolation, it can also introduce resource overhead and programming complexity. The Food Ordering System demonstrates how the fork() and wait() system calls can be used to create and coordinate processes in UNIX/Linux.

In conclusion, understanding process modeling provides a foundation for learning operating systems and developing applications that manage multiple tasks effectively.

# References
GeeksforGeeks. (2023, September 7). Fork system call in operating system. GeeksforGeeks. https://www.geeksforgeeks.org/operating-systems/fork-system-call-in-operating-system/

Process model: Definition, explanation, and use cases | vation ventures. (2025). Vationventures.Com. https://www.vationventures.com/glossary/process-model-definition-explanation-and-use-cases

GeeksforGeeks. (2024, December 30). Process in operating system. GeeksforGeeks. https://www.geeksforgeeks.org/operating-systems/process-in-operating-system/











