#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <signal.h>
#include <pthread.h>
#include <sys/sysinfo.h>

#define PORT 8080
#define BUFFER_SIZE 4096
#define MAX_CLIENTS 32
#define LOG_FILE "server_activity.log"

#define DEFAULT_USER "syafiq"
#define DEFAULT_PASS "epix2026"

#define C_RESET   "\033[0m"
#define C_BOLD    "\033[1m"
#define C_GREEN   "\033[1;32m"
#define C_CYAN    "\033[1;36m"
#define C_YELLOW  "\033[1;33m"
#define C_RED     "\033[1;31m"
#define C_BLUE    "\033[1;34m"

typedef struct {
    int socket_fd;
    struct sockaddr_in client_addr;
    char username[32];
    int is_authenticated;
} ClientContext;

int server_fd = -1;
unsigned long total_requests = 0;
int active_clients = 0;
time_t start_time;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
ClientContext *client_list[MAX_CLIENTS] = {NULL};

void get_timestamp(char *buf, size_t len) {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(buf, len, "%Y-%m-%d %H:%M:%S", tm_info);
}

void write_log(const char *client_ip, int client_port, const char *category, const char *msg) {
    pthread_mutex_lock(&lock);
    char tbuf[32];
    get_timestamp(tbuf, sizeof(tbuf));
    FILE *fp = fopen(LOG_FILE, "a");
    if (fp) {
        fprintf(fp, "[%s] [%s:%d] [%s] %s\n", tbuf, client_ip, client_port, category, msg);
        fclose(fp);
    }
    pthread_mutex_unlock(&lock);
}

void handle_sigint(int sig) {
    (void)sig;
    char tbuf[32];
    get_timestamp(tbuf, sizeof(tbuf));
    printf("\n%s[%s] [SYSTEM] Catching SIGINT, shutting down server...%s\n", C_RED, tbuf, C_RESET);
    write_log("0.0.0.0", PORT, "SHUTDOWN", "Server shut down cleanly.");
    if (server_fd >= 0) close(server_fd);
    exit(0);
}

void add_client(ClientContext *ctx) {
    pthread_mutex_lock(&lock);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_list[i] == NULL) {
            client_list[i] = ctx;
            active_clients++;
            break;
        }
    }
    pthread_mutex_unlock(&lock);
}

void remove_client(ClientContext *ctx) {
    pthread_mutex_lock(&lock);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_list[i] == ctx) {
            client_list[i] = NULL;
            active_clients--;
            break;
        }
    }
    pthread_mutex_unlock(&lock);
}

void generate_progress_bar(char *bar_str, int percentage, int bar_width) {
    int filled = (percentage * bar_width) / 100;
    int pos = 0;
    bar_str[pos++] = '[';
    for (int i = 0; i < filled; i++) pos += sprintf(bar_str + pos, "¦");
    for (int i = filled; i < bar_width; i++) pos += sprintf(bar_str + pos, "¦");
    sprintf(bar_str + pos, "] %d%%", percentage);
}

void *handle_client(void *arg) {
    ClientContext *ctx = (ClientContext *)arg;
    int sock = ctx->socket_fd;
    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(ctx->client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
    int client_port = ntohs(ctx->client_addr.sin_port);

    add_client(ctx);

    char time_str[32];
    get_timestamp(time_str, sizeof(time_str));
    printf("%s[%s] [+] Connection accepted: %s:%d (Active: %d)%s\n", 
           C_GREEN, time_str, client_ip, client_port, active_clients, C_RESET);

    write_log(client_ip, client_port, "CONNECT", "Client connection accepted.");

    char buffer[BUFFER_SIZE];
    char response[BUFFER_SIZE];

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes = read(sock, buffer, BUFFER_SIZE - 1);
        if (bytes <= 0) break;

        buffer[strcspn(buffer, "\r\n")] = 0;
        if (strlen(buffer) == 0) continue;

        pthread_mutex_lock(&lock);
        total_requests++;
        pthread_mutex_unlock(&lock);

        get_timestamp(time_str, sizeof(time_str));
        printf("%s[%s] [%s:%d (%s)] Cmd: %s\"%s\"%s\n", 
               C_CYAN, time_str, client_ip, client_port, 
               ctx->is_authenticated ? ctx->username : "Guest", 
               C_YELLOW, buffer, C_RESET);

        write_log(client_ip, client_port, "COMMAND", buffer);
        memset(response, 0, BUFFER_SIZE);

        if (strncmp(buffer, "PING ", 5) == 0) {
            snprintf(response, sizeof(response), "PONG %s", buffer + 5);
        }
        else if (strncasecmp(buffer, "LOGIN ", 6) == 0) {
            char u[32] = {0}, p[32] = {0};
            if (sscanf(buffer + 6, "%31s %31s", u, p) == 2) {
                if (strcmp(u, DEFAULT_USER) == 0 && strcmp(p, DEFAULT_PASS) == 0) {
                    ctx->is_authenticated = 1;
                    strncpy(ctx->username, u, sizeof(ctx->username) - 1);
                    snprintf(response, sizeof(response), 
                             "%s[?] AUTHENTICATION SUCCESS%s - Welcome back %s! Access granted.", 
                             C_GREEN, C_RESET, u);
                    write_log(client_ip, client_port, "AUTH", "User authenticated.");
                } else {
                    snprintf(response, sizeof(response), 
                             "%s[?] AUTHENTICATION FAILED%s - Invalid credentials.", C_RED, C_RESET);
                    write_log(client_ip, client_port, "AUTH_FAIL", "Invalid login attempt.");
                }
            } else {
                snprintf(response, sizeof(response), "Usage: LOGIN <username> <password>");
            }
        }
        else if (strcasecmp(buffer, "HELP") == 0) {
            snprintf(response, sizeof(response),
                     "\n+----------------------- AVAILABLE COMMANDS -------------------------+\n"
                     "¦  • Network    : PING, ECHO <msg>, TIME, HOSTNAME                   ¦\n"
                     "¦  • Telemetry  : DASHBOARD, SYSINFO, UPTIME, STATS                  ¦\n"
                     "¦  • Session    : LOGIN <u p>, WHO, COLOR, BYE / EXIT                ¦\n"
                     "+---------------------------------------------------------------------+");
        }
        else if (strcasecmp(buffer, "EXIT") == 0 || strcasecmp(buffer, "BYE") == 0) {
            snprintf(response, sizeof(response), "SERVER: Connection closing. Goodbye!");
            send(sock, response, strlen(response), 0);
            write_log(client_ip, client_port, "DISCONNECT", "Client disconnected.");
            break;
        }
        else if (strncasecmp(buffer, "ECHO ", 5) == 0) {
            snprintf(response, sizeof(response), "REPLY: %s", buffer + 5);
        }
        else if (!ctx->is_authenticated) {
            snprintf(response, sizeof(response), 
                     "%s[LOCKED]%s Please login first! Command: LOGIN %s %s", 
                     C_RED, C_RESET, DEFAULT_USER, DEFAULT_PASS);
        }
        else if (strcasecmp(buffer, "DASHBOARD") == 0 || strcasecmp(buffer, "STATS") == 0) {
            struct sysinfo info;
            sysinfo(&info);
            long total_ram = (info.totalram * info.mem_unit) / (1024 * 1024);
            long free_ram = (info.freeram * info.mem_unit) / (1024 * 1024);
            long used_ram = total_ram - free_ram;
            int ram_pct = (int)((used_ram * 100) / total_ram);
            char bar[64];
            generate_progress_bar(bar, ram_pct, 10);

            time_t diff = time(NULL) - start_time;
            char up_str[32];
            snprintf(up_str, sizeof(up_str), "%02ldh %02ldm %02lds", diff / 3600, (diff % 3600) / 60, diff % 60);

            snprintf(response, sizeof(response),
                     "\n+-[ Network & Sessions ]-------------+ +-[ Kernel Telemetry ]-----------------+\n"
                     "¦ Listening Port : %-18d¦ ¦ Memory Load    : %-18s ¦\n"
                     "¦ Active Clients : %-18d¦ ¦ RAM Capacity   : %4ld MB / %4ld MB ¦\n"
                     "¦ Total Messages : %-18lu¦ ¦ Process Count  : %-18d ¦\n"
                     "¦ Server Uptime  : %-18s¦ ¦ Kernel State   : %sONLINE / HEALTHY%s    ¦\n"
                     "+------------------------------------+ +-------------------------------------+",
                     PORT, bar, active_clients, used_ram, total_ram, total_requests, info.procs, up_str, C_GREEN, C_RESET);
        }
        else if (strcasecmp(buffer, "TIME") == 0) {
            get_timestamp(response, sizeof(response));
        }
        else if (strcasecmp(buffer, "HOSTNAME") == 0) {
            gethostname(response, sizeof(response));
        }
        else if (strcasecmp(buffer, "SYSINFO") == 0) {
            struct sysinfo info;
            sysinfo(&info);
            long total_ram = (info.totalram * info.mem_unit) / (1024 * 1024);
            long free_ram = (info.freeram * info.mem_unit) / (1024 * 1024);
            long used_ram = total_ram - free_ram;
            int percent = (int)((used_ram * 100) / total_ram);
            char bar[128];
            generate_progress_bar(bar, percent, 20);

            snprintf(response, sizeof(response),
                     "\n+-[ Linux Kernel Telemetry (sysinfo) ]------------------------------+\n"
                     "¦ RAM Usage      : %-48s¦\n"
                     "¦ Details        : %ld MB / %ld MB (Free: %ld MB)                   ¦\n"
                     "¦ Active Procs   : %-48d¦\n"
                     "+-------------------------------------------------------------------+",
                     bar, used_ram, total_ram, free_ram, info.procs);
        }
        else if (strcasecmp(buffer, "WHO") == 0) {
            pthread_mutex_lock(&lock);
            char table[BUFFER_SIZE];
            int pos = sprintf(table,
                "\n+----------------------------------------------------------+\n"
                "¦ No ¦ IP Address      ¦ Port  ¦ Username   ¦ Status       ¦\n"
                "+----+-----------------+-------+------------+--------------¦\n");

            int count = 1;
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (client_list[i] != NULL) {
                    char ip[INET_ADDRSTRLEN];
                    inet_ntop(AF_INET, &(client_list[i]->client_addr.sin_addr), ip, INET_ADDRSTRLEN);
                    pos += sprintf(table + pos,
                        "¦ %02d ¦ %-15s ¦ %-5d ¦ %-10s ¦ %-12s ¦\n",
                        count++, ip, ntohs(client_list[i]->client_addr.sin_port),
                        client_list[i]->is_authenticated ? client_list[i]->username : "Guest",
                        client_list[i]->is_authenticated ? "VERIFIED" : "UNVERIFIED");
                }
            }
            sprintf(table + pos, "+----------------------------------------------------------+");
            pthread_mutex_unlock(&lock);
            strncpy(response, table, sizeof(response) - 1);
        }
        else {
            snprintf(response, sizeof(response), 
                     "[ERROR] Unknown command '%.128s'. Type 'HELP' for options.", buffer);
        }

        send(sock, response, strlen(response), 0);
    }

    close(sock);
    remove_client(ctx);

    get_timestamp(time_str, sizeof(time_str));
    printf("%s[%s] [-] Disconnected: %s:%d (Active: %d)%s\n", 
           C_RED, time_str, client_ip, client_port, active_clients, C_RESET);

    write_log(client_ip, client_port, "DISCONNECT", "Connection closed.");
    free(ctx);
    pthread_detach(pthread_self());
    return NULL;
}

int main() {
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);

    signal(SIGINT, handle_sigint);
    start_time = time(NULL);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    char boot_time[32];
    get_timestamp(boot_time, sizeof(boot_time));
    printf("%s=====================================================================%s\n", C_CYAN, C_RESET);
    printf("%s   SECURED MULTI-THREADED TCP SERVER (STATUS: ONLINE)                %s\n", C_GREEN, C_RESET);
    printf("   Listening Port: %d\n", PORT);
    printf("   Boot Time     : %s\n", boot_time);
    printf("   Audit Log File: %s\n", LOG_FILE);
    printf("%s=====================================================================%s\n\n", C_CYAN, C_RESET);

    write_log("0.0.0.0", PORT, "STARTUP", "Server started successfully.");

    while (1) {
        int client_sock = accept(server_fd, (struct sockaddr *)&address, &addrlen);
        if (client_sock < 0) continue;

        ClientContext *ctx = (ClientContext *)calloc(1, sizeof(ClientContext));
        ctx->socket_fd = client_sock;
        ctx->client_addr = address;
        ctx->is_authenticated = 0;
        strcpy(ctx->username, "Guest");

        pthread_t tid;
        if (pthread_create(&tid, NULL, handle_client, (void *)ctx) != 0) {
            perror("thread creation failed");
            close(client_sock);
            free(ctx);
        }
    }

    close(server_fd);
    return 0;
}