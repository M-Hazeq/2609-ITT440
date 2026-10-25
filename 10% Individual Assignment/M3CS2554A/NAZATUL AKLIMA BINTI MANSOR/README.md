# NAZATUL AKLIMA BINTI MANSOR
## 2025158451
## M3CDCS2554A

# FUNDAMENTAL OF SOCKET PROGRAMMING

**A socket is the endpoint that lets two programs talk to each other over a network.**

## What Is A Socket?

A socket is one end of a connection between two programs. Data is written into one program's socket and read from another program's socket. The two programs may run on separate computers or on the same machine. Imagine a phone call in which both parties have phones that are connected to one another.


## Client and Server

The **server** waits and provide service.
the **client** starts the contact and asks for the service.

A web browser is a client and the website in visits is a server.

## IP Address and Port

A socket is identified by two things:

The IP **address** tells which computer to reach.
The **port number** tells which program on that computer to reach.

For instance, 192.168.1.1:8080 denotes port 8080 on the 192.168.1.1 computer. Your own machine (localhost) is always indicated by the address 127.0.0.1. Web pages use ports 80 (HTTP) and 443 (HTTPS), which are set aside for common services.


## TCP and UDP

| | TCP | UDP |
|---|---|---|
| Connection | Connects first | No connection |
| Reliability | Data arrives, in order | Data can be lost |
| Speed | Slower | Faster |
| Used for | Web, email, file transfer | Games, live video |

Use TCP when the data must arrive correctly. Use UDP when speed matters more.

## How a TCP connection works

**Server steps:**

1. `socket()` creates the socket.
2. `bind()` attaches it to an address and port.
3. `listen()` waits for connections.
4. `accept()` accepts one client.
5. `send()` and `recv()` send and receive data.
6. `close()` ends the connection.

**Client steps:**

1. `socket()` creates the socket.
2. `connect()` connects to the server.
3. `send()` and `recv()` send and receive data.
4. `close()` ends the connection.

> `accept()` gives back a **new** socket for that client, while the server's original socket keeps listening for other clients.


## Example

The server runs on port 8080 and waits. The client connects and sends `Hello from client`. The server prints it and replies `Hello from server`. Then both close their sockets.

### Files

| File | Description |
|---|---|
| `server8080.c` | TCP server on port 8080 |
| `client8080.c` | TCP client that connects to `127.0.0.1:8080` |

## Video Demonstration

The video demonstration for this socket program can be view at this link: [Socket Programming Video Demonstration](https://youtu.be/InAsQyjdLeE?si=lm1QgfJPzxQvSgjW)

### Source code

#### Server (`server8080.c`)


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
    (void)valread; // avoids the unused variable warning
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

#### Client (`client8080.c`)

The client was written to match the server.

```c
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080
int main(int argc, char const* argv[])
{
    int client_fd;
    ssize_t valread;
    struct sockaddr_in serv_addr;
    char* hello = "Hello from client";
    char buffer[1024] = { 0 };

    // Creating socket file descriptor
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert the IP address 127.0.0.1 to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)

### Compile

```bash
gcc -Wall -o server8080 server8080.c
gcc -Wall -o client8080 client8080.c
```

### Run

Open two terminals. Start the server first.

Terminal 1:

```bash
./server8080
```

Terminal 2:

```bash
./client8080
```

### Expected output

Server:

```
Hello from client
Hello message sent
```

Client:

```
Hello message sent
Hello from server
```

## Limitation

This server handles only one client and then exits. To serve many clients, a server can use threads, separate processes (`fork()`), or `select()`.


## References

- Cooley, M. (2025, May 12). *What is socket programming?* JumpCloud. https://jumpcloud.com/it-index/what-is-socket-programming-2
- Kalita, L. (2014). Socket programming. *International Journal of Computer Science and Information Technologies, 5*(3), 4802-4807. https://www.ijcsit.com/docs/Volume%205/vol5issue03/ijcsit20140503462.pdf
- GeeksforGeeks. (2026, April 22). *Socket programming in C*. https://www.geeksforgeeks.org/c/socket-programming-cc/
- Silver Moon. (2020, August 11). *How to code a server and client in C with sockets on Linux - code examples*. BinaryTides. https://www.binarytides.com/server-client-example-c-sockets-linux/
