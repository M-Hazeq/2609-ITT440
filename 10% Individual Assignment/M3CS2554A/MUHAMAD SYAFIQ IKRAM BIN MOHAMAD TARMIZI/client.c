#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/time.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 4096
#define DEFAULT_USER "syafiq"

#define C_RESET   "\033[0m"
#define C_BOLD    "\033[1m"
#define C_RED     "\033[1;31m"
#define C_GREEN   "\033[1;32m"
#define C_YELLOW  "\033[1;33m"
#define C_CYAN    "\033[1;36m"

typedef struct {
    int id;
    char name[32];
    char code[32];
} ColorTheme;

ColorTheme themes[] = {
    {1,  "CYAN NEON",      "\033[36m"},
    {2,  "MATRIX GREEN",   "\033[32m"},
    {3,  "SUNSET MAGENTA", "\033[35m"},
    {4,  "CYBER YELLOW",   "\033[33m"},
    {5,  "CRIMSON RED",    "\033[31m"},
    {6,  "ELECTRIC BLUE",  "\033[34m"},
    {7,  "LIME BRIGHT",    "\033[92m"},
    {8,  "ICE BLUE",       "\033[96m"},
    {9,  "PINK NEON",      "\033[95m"},
    {10, "FLAME ORANGE",   "\033[38;2;255;120;0m"},
    {11, "ROYAL PURPLE",   "\033[38;2;160;32;240m"},
    {12, "GOLDEN AMBER",   "\033[38;2;255;191;0m"},
    {13, "MINT PASTEL",    "\033[38;2;152;251;152m"},
    {14, "HOT CORAL",      "\033[38;2;255;110;110m"},
    {15, "PURE WHITE",     "\033[97m"}
};

int total_themes = sizeof(themes) / sizeof(themes[0]);
char current_color[32] = "\033[36m";
char current_name[32] = "CYAN NEON";
int is_logged_in = 0;
char session_user[32] = "Guest";

void print_banner() {
    printf("%s%s", current_color, C_BOLD);
    printf("  ¦¦¦¦¦¦¦+¦¦+   ¦¦+ ¦¦¦¦¦+ ¦¦¦¦¦¦¦+¦¦+ ¦¦¦¦¦¦+ \n");
    printf("  ¦¦+----++¦¦+ ¦¦++¦¦+--¦¦+¦¦+----+¦¦¦¦¦+---¦¦+\n");
    printf("  ¦¦¦¦¦¦¦+ +¦¦¦¦++ ¦¦¦¦¦¦¦¦¦¦¦¦¦+  ¦¦¦¦¦¦   ¦¦¦\n");
    printf("  +----¦¦¦  +¦¦++  ¦¦+--¦¦¦¦¦+--+  ¦¦¦¦¦¦__ ¦¦¦\n");
    printf("  ¦¦¦¦¦¦¦¦   ¦¦¦   ¦¦¦  ¦¦¦¦¦¦     ¦¦¦+¦¦¦¦¦¦++\n");
    printf("  +------+   +-+   +-+  +-++-+     +-+ +--¯¯-+ \n");
    printf("       [ SECURED TCP CLIENT TERMINAL CONSOLE ]         \n");
    printf("+------------------------------------------------------------------+\n");
    printf("¦  Developer    : MUHAMAD SYAFIQ IKRAM BIN MOHAMAD TARMIZI         ¦\n");
    printf("¦  Student ID   : 2024266172 (Group: M3CS2554A)                    ¦\n");
    printf("¦  Active Theme : %-49s¦\n", current_name);
    printf("¦------------------------------------------------------------------¦\n");
    printf("¦  Access Gate  : Type 'LOGIN syafiq epix2026' to unlock session   ¦\n");
    printf("¦  Dashboard    : Type 'DASHBOARD' for dual-card system telemetry  ¦\n");
    printf("+------------------------------------------------------------------+\n%s", C_RESET);
}

void select_theme() {
    printf("\n%s+------------------------ COLOR PALETTES ------------------------+%s\n", C_BOLD, C_RESET);
    for (int i = 0; i < total_themes; i++) {
        printf("¦  %2d. %s%-20s%s ", themes[i].id, themes[i].code, themes[i].name, C_RESET);
        if ((i + 1) % 2 == 0 || i == total_themes - 1) {
            if (i == total_themes - 1 && total_themes % 2 != 0) printf("%25s¦\n", "");
            else printf(" ¦\n");
        }
    }
    printf("¦  99. %s[?] RANDOM SELECTION%s                                    ¦\n", C_BOLD, C_RESET);
    printf("+------------------------------------------------------------------+\n");
    printf("Select theme index (1-%d or 99): ", total_themes);

    char choice[16];
    if (fgets(choice, sizeof(choice), stdin) != NULL) {
        choice[strcspn(choice, "\r\n")] = 0;
        int sel = atoi(choice);

        if (sel == 99) {
            srand(time(NULL));
            int r = rand() % total_themes;
            strcpy(current_color, themes[r].code);
            strcpy(current_name, themes[r].name);
        } else if (sel >= 1 && sel <= total_themes) {
            strcpy(current_color, themes[sel - 1].code);
            strcpy(current_name, themes[sel - 1].name);
        } else {
            printf("\n%s[!] Invalid input. Theme unchanged.%s\n", C_YELLOW, C_RESET);
            return;
        }
    }

    printf("\033[H\033[J");
    print_banner();
}

long long get_microtime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return ((long long)tv.tv_sec * 1000000) + tv.tv_usec;
}

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket creation failed");
        return 1;
    }

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("%s[?] Connection refused on %s:%d. Is server running?%s\n", 
               C_RED, SERVER_IP, PORT, C_RESET);
        close(sock);
        return 1;
    }

    printf("\033[H\033[J");
    print_banner();

    char command[BUFFER_SIZE];
    char buffer[BUFFER_SIZE];

    while (1) {
        printf("\n%s---[ ? ONLINE | %s:%d | USER: %s (%s) ]---%s\n", 
               current_color, SERVER_IP, PORT, session_user, 
               is_logged_in ? "AUTH" : "LOCKED", C_RESET);
        printf("%s%s[Syafiq@Terminal]»%s Enter command: ", current_color, C_BOLD, C_RESET);

        if (fgets(command, sizeof(command), stdin) == NULL) break;

        command[strcspn(command, "\r\n")] = 0;
        if (strlen(command) == 0) continue;

        if (strcasecmp(command, "COLOR") == 0 || strcasecmp(command, "THEME") == 0) {
            select_theme();
            continue;
        }

        if (strcasecmp(command, "PING") == 0) {
            long long t_start = get_microtime();
            char ping_pkt[64];
            snprintf(ping_pkt, sizeof(ping_pkt), "PING %lld", t_start);
            send(sock, ping_pkt, strlen(ping_pkt), 0);

            memset(buffer, 0, BUFFER_SIZE);
            int bytes = read(sock, buffer, BUFFER_SIZE - 1);
            long long t_end = get_microtime();

            if (bytes > 0 && strncmp(buffer, "PONG ", 5) == 0) {
                double rtt_ms = (t_end - t_start) / 1000.0;
                char badge[64];
                if (rtt_ms < 1.0) {
                    snprintf(badge, sizeof(badge), "%s[EXCELLENT ?????]%s", C_GREEN, C_RESET);
                } else if (rtt_ms < 10.0) {
                    snprintf(badge, sizeof(badge), "%s[MODERATE  ?????]%s", C_CYAN, C_RESET);
                } else {
                    snprintf(badge, sizeof(badge), "%s[POOR      ?????]%s", C_YELLOW, C_RESET);
                }

                printf("%s+-[ Network Latency Diagnostic (RTT) ]------------------------------+%s\n", current_color, C_RESET);
                printf("¦ Server Response : %s\n", buffer);
                printf("¦ Latency Time    : %.3f ms  %s\n", rtt_ms, badge);
                printf("%s+-------------------------------------------------------------------+%s\n", current_color, C_RESET);
            }
            continue;
        }

        send(sock, command, strlen(command), 0);

        if (strcasecmp(command, "EXIT") == 0 || strcasecmp(command, "BYE") == 0) {
            printf("%s[!] Closing connection session...%s\n", C_YELLOW, C_RESET);
            break;
        }

        memset(buffer, 0, BUFFER_SIZE);
        int bytes = read(sock, buffer, BUFFER_SIZE - 1);
        if (bytes <= 0) {
            printf("%s[?] Server terminated the connection!%s\n", C_RED, C_RESET);
            break;
        }

        if (strstr(buffer, "AUTHENTICATION SUCCESS") != NULL) {
            is_logged_in = 1;
            strcpy(session_user, DEFAULT_USER);
        }

        printf("%s+-[ Server Response ]-----------------------------------------------%s\n", current_color, C_RESET);
        printf("¦ %s\n", buffer);
        printf("%s+------------------------------------------------------------------%s\n", current_color, C_RESET);
    }

    close(sock);
    return 0;
}