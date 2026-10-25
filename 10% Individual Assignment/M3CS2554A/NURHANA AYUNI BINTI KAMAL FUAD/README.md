# NURHANA AYUNI BINTI KAMAL FUAD
## 2025304821
## CDCS2554A

# FUNDAMENTALS OF SOCKET PROGRAMMING

## Introduction

Every time you send a WhatsApp message, open a website, or join an online game, two programs somewhere are having a conversation. They need a reliable way to find each other and swap data, and that is what **socket programming** provides.

A socket is one end of a two-way communication link between two programs on a network. It works like a doorway: your program sends data out through it and receives data back through it. Socket programming is the practice of writing code that creates, uses, and closes these doorways.

## What Exactly Is a Socket?

A socket is identified by two things:

- **An IP address**, which says *which device* on the network to reach.
- **A port number**, which says *which program* on that device should get the data.

Written together, they look like `192.168.1.1:8080`. A useful analogy is an apartment building. The IP address is the building's street address, and the port number is the apartment number. Without both, the "mail" wouldn't reach the right person.

Ports let one computer run many network services at once. A web server typically uses port 80 for HTTP or 443 for HTTPS, while an email server uses different ports.

> **Important note:** A socket is always the combination of IP address + port number. Neither one alone is enough to identify a communication endpoint.

## The Client-Server Model

Socket communication almost always follows the **client-server model**:

- The **server** sits and waits, listening for incoming connections on a specific address and port.
- The **client** starts the conversation by reaching out to the server.
- The client sends a request, and the server processes it and sends back a response.

A restaurant works the same way. The kitchen (server) is open and waiting, and the customer (client) walks in and places an order.

## Types of Sockets

Different applications need different things. Some need every byte to arrive perfectly, while others just need speed. This is why there are different socket types.

**1. Stream Sockets (TCP)**
These provide a reliable, connection-based flow of data that arrives in the same order it was sent. It is like a phone call: you connect first, then talk, and you can tell if the line drops.

**2. Datagram Sockets (UDP)**
These are connectionless. Data is sent as individual packets with no setup, and the packets can arrive out of order or get lost. It is like dropping letters into a mailbox: fast and simple, but with no guarantee of delivery or order.

**3. Raw Sockets**
These bypass the standard TCP/UDP handling and give direct access to lower network layers. They are used for building custom protocols or network analysis tools, and they usually require elevated privileges.

| Feature | TCP (Stream) | UDP (Datagram) |
|---|---|---|
| Connection | Required before sending | Not required |
| Reliability | Reliable | Unreliable |
| Order of data | Guaranteed | Not guaranteed |
| Speed | Slower | Faster |
| Analogy | Phone call | Mailbox |

## How a TCP Connection Is Built, Step by Step

The process follows a fixed sequence of function calls.

### On the server side

1. **`socket()`** creates the socket, specifying the address family (such as IPv4), the type (TCP or UDP), and the protocol.
2. **`setsockopt()`** is optional but helpful. It allows the address and port to be reused, which prevents the common "address already in use" error.
3. **`bind()`** attaches the socket to a specific IP address and port, like claiming a phone number.
4. **`listen()`** switches the socket into passive mode so it waits for clients. It also sets a *backlog*, the maximum number of pending connections allowed in the queue.
5. **`accept()`** takes the first waiting connection from the queue and creates a **new socket** dedicated to that client.
6. **`send()` / `recv()`** (or `read()` / `write()`) are used to exchange data.
7. **`close()`** releases the socket and frees system resources.

### On the client side

1. **`socket()`** creates the socket.
2. **`connect()`** reaches out to the server's IP address and port.
3. **`send()` / `recv()`** exchange data.
4. **`close()`** ends the connection.

### UDP Is Simpler

Because UDP has no connection, there is no `listen()`, `accept()`, or `connect()` involved. Both sides use `socket()`, the server also calls `bind()`, and data moves with **`sendto()`** and **`recvfrom()`**.

## Video Demonstration

The video demo can be viewed via this YouTube link: [ITT440 ASSIGNMENT 1](https://youtu.be/Wh5w80pYD6o?si=7IyuAQQZdDWmeHtN)

## A Simple Real Example (Source Code in C)

To see all of this working together, here is a small TCP program in C. The server waits on port 8080. The client connects to it, sends "Hello from client," and the server replies "Hello from server." Both then close their sockets.

It looks small, but it contains the full life cycle of every networked application.

### server.c

```c
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080
int main(int argc, char const* argv[])
{
    int server_fd, new_socket;
    ssize_t valread;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);
    char buffer[1024] = { 0 };
    char* hello = "Hello from server";

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr*)&address,
             sizeof(address))
        < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket
         = accept(server_fd, (struct sockaddr*)&address,
                  &addrlen))
        < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    // subtract 1 for the null
    // terminator at the end
    valread = read(new_socket, buffer,
                   1024 - 1);
    printf("%s\n", buffer);
    send(new_socket, hello, strlen(hello), 0);
    printf("Hello message sent\n");

    // closing the connected socket
    close(new_socket);

    // closing the listening socket
    close(server_fd);
    return 0;
}
```

### client.c

```c
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

int main(int argc, char const* argv[])
{
    int status, valread, client_fd;
    struct sockaddr_in serv_addr;
    char* hello = "Hello from client";
    char buffer[1024] = { 0 };
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)
        <= 0) {
        printf(
            "\nInvalid address/ Address not supported \n");
        return -1;
    }

    if ((status
         = connect(client_fd, (struct sockaddr*)&serv_addr,
                   sizeof(serv_addr)))
        < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    // subtract 1 for the null
    // terminator at the end
    send(client_fd, hello, strlen(hello), 0);
    printf("Hello message sent\n");
    valread = read(client_fd, buffer,
                   1024 - 1);
    printf("%s\n", buffer);

    // closing the connected socket
    close(client_fd);
    return 0;
}
```

### How to Compile and Run

Open two terminal windows.

```bash
# Compile both programs
gcc server.c -o server
gcc client.c -o client

# Terminal 1: start the server first
./server

# Terminal 2: then start the client
./client
```

### Expected Output

**Server terminal:**

```
Hello from client
Hello message sent
```

**Client terminal:**

```
Hello message sent
Hello from server
```

## Code Walkthrough

> **Important notes:**
> - Every key call is wrapped in an error check (`< 0`). If something fails, the program reports it with `perror()` or `printf()` and exits. Good socket code always checks for failure.
> - The buffer is 1024 bytes, but the code reads only `1024 - 1` bytes. This leaves one byte free for the string's null terminator (`\0`), which prevents printing garbage.
> - `read()` is a **blocking** call. The program waits until data arrives.

## Sockets in Other Languages

The idea is the same everywhere, only the syntax changes.

- **C** uses functions such as `socket()` and `bind()` directly.
- **Python** wraps them into a `socket` module with the same logic.
- **Java** uses `Socket` and `ServerSocket` for connection-oriented (TCP) communication, and `DatagramSocket` and `DatagramPacket` for connectionless (UDP) communication.

## Where Socket Programming Is Used

- **Web:** Browsers and web servers talk over TCP sockets using HTTP.
- **Email:** Protocols like SMTP, POP3, and IMAP send and retrieve messages through sockets.
- **File transfer:** FTP and SFTP use sockets to move files.
- **Online gaming:** TCP keeps important data accurate, while UDP keeps real-time action fast.
- **Chat apps:** Messaging services keep persistent connections open so messages arrive instantly.
- **Network monitoring:** Admin tools use sockets to query device information.
- **Inter-process communication (IPC):** Sockets also let programs on the *same* machine talk to each other.

## Common Problems and Fixes

| Problem | Cause | Fix |
|---|---|---|
| **Connection failure** | Client uses the wrong IP address or port, or the server isn't running yet | Double-check the address and port, and start the server first |
| **Port binding error** | Another application is already using the port | Use a different port, or close the other application |
| **Program seems frozen** | Sockets are *blocking* by default, so `accept()` and `read()` wait indefinitely | Use non-blocking mode if needed |
| **Connection refused** | The pending-connection queue (backlog) is full | Increase the backlog or handle clients faster |

## Handling Many Clients

The example above serves exactly one client and then exits. Real servers rarely work this way. Modern servers handle multiple clients at once using **threads**, **separate processes**, or **asynchronous programming**. Without one of these, a server would have to finish with one client before it could even start on the next.

## Conclusion

Socket programming is the quiet foundation beneath almost everything we do online. At its core, it is simple: create an endpoint, give it an address, and let two programs exchange data, reliably through TCP or quickly through UDP. The hello example shows the whole pattern in miniature, and once you understand the client-server flow and the role of each function, the rest of network programming is built on top of it.

## Sources

- GeeksforGeeks, *Socket Programming in C* (source of the example code) — https://www.geeksforgeeks.org/c/socket-programming-cc/
- GeeksforGeeks, *Socket in Computer Network* — https://www.geeksforgeeks.org/computer-networks/socket-in-computer-network/
- JumpCloud, *What is Socket Programming?* — https://jumpcloud.com/it-index/what-is-socket-programming
- Scaler Topics, *Socket Programming in Computer Network* — https://www.scaler.com/topics/computer-network/socket-programming/
