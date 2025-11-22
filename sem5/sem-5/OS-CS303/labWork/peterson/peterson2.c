#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdint.h>
#include <inttypes.h>  // For PRIdPTR

#define NUM_PROCESSES 2

// Shared variables for Peterson's Algorithm
bool flag[NUM_PROCESSES];
int turn;

// Shared resource (critical section variable)
int shared_counter = 0;

// Entry section for Peterson's Algorithm
void enter_critical_section(intptr_t process_id) {
    intptr_t other = 1 - process_id; // For two processes, other process is the opposite of current
    flag[process_id] = true;        // Declare intent to enter the critical section
    turn = other;                   // Give the turn to the other process
    // Wait until the other process does not need the critical section or it is your turn
    while (flag[other] && turn == other) {
        // Busy wait
    }
}

// Exit section for Peterson's Algorithm
void exit_critical_section(intptr_t process_id) {
    flag[process_id] = false; // Indicate that the process is done with the critical section
}

// Critical section: A function to modify shared resource
void critical_section(intptr_t process_id) {
    // Begin critical section
    shared_counter++;
    printf("Process %" PRIdPTR " in critical section. Shared counter: %d\n", process_id, shared_counter);
    // Simulate some work in the critical section
    sleep(1);
    // End critical section
}

// Function run by each thread (process)
void* process(void* arg) {
    intptr_t process_id = (intptr_t)arg;
    for (int i = 0; i < 5; i++) { // Repeat 5 times
        // Entry section
        enter_critical_section(process_id);
        // Critical section
        critical_section(process_id);
        // Exit section
        exit_critical_section(process_id);
        // Remainder section (simulate some work outside critical section)
        printf("Process %" PRIdPTR " in remainder section.\n", process_id);
        sleep(1); // Simulate some work outside the critical section
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_PROCESSES];
    intptr_t process_ids[NUM_PROCESSES] = {0, 1};
    // Initialize the flags to false initially
    flag[0] = flag[1] = false;
    turn = 0;
    // Create two threads (representing two processes)
    for (int i = 0; i < NUM_PROCESSES; i++) {
        pthread_create(&threads[i], NULL, process, (void*)process_ids[i]);
    }
    // Wait for both threads to complete
    for (int i = 0; i < NUM_PROCESSES; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("Final shared counter value: %d\n", shared_counter);
    return 0;
}