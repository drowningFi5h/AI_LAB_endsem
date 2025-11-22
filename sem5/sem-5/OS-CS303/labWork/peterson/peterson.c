#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>

#define NUM_ITERATIONS 5

int turn;
bool flag[2] = {false, false};  // Flags to indicate if a thread wants to enter critical section
int shared_variable = 0;

void* process(void* arg) {
    pthread_t id = *(pthread_t*)arg;         // Thread ID (0 or 1)
    pthread_t other = id == *(pthread_t*)&flag[0] ? *(pthread_t*)&flag[1] : *(pthread_t*)&flag[0];  // ID of the other thread

    for (int i = 0; i < NUM_ITERATIONS; i++) {
        // Entry section
        flag[id == *(pthread_t*)&flag[0] ? 0 : 1] = true;
        turn = other;
        while (flag[other == *(pthread_t*)&flag[0] ? 0 : 1] && turn == other) {
            // Busy wait (spinlock)
        }

        // Critical section
        printf("Thread %lu is in the critical section.\n", (unsigned long)id);
        shared_variable++;
        printf("Shared Variable: %d\n", shared_variable);
        sleep(1);  // Simulate some work in the critical section

        // Exit section
        flag[id == *(pthread_t*)&flag[0] ? 0 : 1] = false;

        // Remainder section
        printf("Thread %lu is in the remainder section.\n", (unsigned long)id);
        sleep(1);  // Simulate some work in the remainder section
    }

    return NULL;
}

int main() {
    pthread_t t0, t1;

    // Create two threads representing two processes
    pthread_create(&t0, NULL, process, &t0);
    pthread_create(&t1, NULL, process, &t1);

    // Wait for both threads to finish
    pthread_join(t0, NULL);
    pthread_join(t1, NULL);

    printf("Final value of Shared Variable: %d\n", shared_variable);
    return 0;
}
