#include <stdio.h>
#include <stdlib.h>

struct Process {
    int id;
    int burstTime;
    int remainingTime;
    int waitingTime;
    int turnaroundTime;
    int arrivalTime;
    int responseTime;
    int startTime;
    int completionTime;
    int queueLevel;
};

void calculateEfficiency(struct Process processes[], int n, int timeQuantum) {
    int currentTime = 0;
    int completed = 0;
    int totalWaitingTime = 0;
    int totalTurnaroundTime = 0;
    int totalResponseTime = 0;

    while (completed != n) {
        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= currentTime && processes[i].remainingTime > 0) {
                if (processes[i].remainingTime == processes[i].burstTime) {
                    processes[i].startTime = currentTime;
                    processes[i].responseTime = currentTime - processes[i].arrivalTime;
                }

                if (processes[i].remainingTime > timeQuantum) {
                    currentTime += timeQuantum;
                    processes[i].remainingTime -= timeQuantum;
                } else {
                    currentTime += processes[i].remainingTime;
                    processes[i].remainingTime = 0;
                    processes[i].completionTime = currentTime;
                    processes[i].turnaroundTime = processes[i].completionTime - processes[i].arrivalTime;
                    processes[i].waitingTime = processes[i].turnaroundTime - processes[i].burstTime;

                    totalWaitingTime += processes[i].waitingTime;
                    totalTurnaroundTime += processes[i].turnaroundTime;
                    totalResponseTime += processes[i].responseTime;

                    completed++;
                }
            }
        }
    }

    printf("Average Waiting Time = %.2f\n", (float)totalWaitingTime / n);
    printf("Average Turnaround Time = %.2f\n", (float)totalTurnaroundTime / n);
    printf("Average Response Time = %.2f\n", (float)totalResponseTime / n);
}

int main() {
    int n, timeQuantum;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    struct Process *processes = (struct Process *)malloc(n * sizeof(struct Process));

    for (int i = 0; i < n; i++) {
        processes[i].id = i + 1;
        printf("Enter arrival time for process %d: ", i + 1);
        scanf("%d", &processes[i].arrivalTime);
        printf("Enter burst time for process %d: ", i + 1);
        scanf("%d", &processes[i].burstTime);
        processes[i].remainingTime = processes[i].burstTime;
        processes[i].waitingTime = 0;
        processes[i].turnaroundTime = 0;
        processes[i].responseTime = 0;
        processes[i].startTime = 0;
        processes[i].completionTime = 0;
        processes[i].queueLevel = -1;
    }

    printf("Enter the time quantum: ");
    scanf("%d", &timeQuantum);

    calculateEfficiency(processes, n, timeQuantum);

    free(processes);
    return 0;
}