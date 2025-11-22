#include <stdio.h>
#include <limits.h>

struct Process {
    int id;
    int burstTime;
    int remainingTime;
    int waitingTime;
    int turnaroundTime;
    int arrivalTime;
    int completionTime; // Add completion time
    int responseTime;    // Add response time
    int firstExecution;  // Track first execution for response time
};

void calculateEfficiency(struct Process processes[], int n, int timeQuantum) {
    int time = 0;
    int completed = 0;
    int totalWaitingTime = 0;
    int totalTurnaroundTime = 0;
    int totalResponseTime = 0;   // Total response time for average calculation
    int totalTime = 0;           // To calculate the efficiency

    while (completed < n) {
        int found = 0;
        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= time && processes[i].remainingTime > 0) {
                found = 1;

                // If it's the first time the process is being scheduled, calculate response time
                if (processes[i].firstExecution == -1) {
                    processes[i].firstExecution = time;
                    processes[i].responseTime = processes[i].firstExecution - processes[i].arrivalTime;
                }

                if (processes[i].remainingTime > timeQuantum) {
                    time += timeQuantum;
                    processes[i].remainingTime -= timeQuantum;
                } else {
                    time += processes[i].remainingTime;
                    processes[i].waitingTime = time - processes[i].burstTime - processes[i].arrivalTime;
                    processes[i].turnaroundTime = time - processes[i].arrivalTime;
                    processes[i].completionTime = time; // Set completion time
                    processes[i].remainingTime = 0;
                    completed++;
                }
            }
        }
        if (!found) {
            time++; // If no process was found to execute, increment time
        }
    }

    totalTime = time;  // Total time is the time after all processes are done

    // Printing the output table with the new Response Time column
    printf("\nProcess\tArrival Time\tBurst Time\tCompletion Time\tWaiting Time\tTurnaround Time\tResponse Time\n");
    for (int i = 0; i < n; i++) {
        totalWaitingTime += processes[i].waitingTime;
        totalTurnaroundTime += processes[i].turnaroundTime;
        totalResponseTime += processes[i].responseTime;
        printf("%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", 
               processes[i].id, 
               processes[i].arrivalTime, 
               processes[i].burstTime, 
               processes[i].completionTime, 
               processes[i].waitingTime, 
               processes[i].turnaroundTime,
               processes[i].responseTime);
    }

    printf("\nAverage Waiting Time: %.2f\n", (float)totalWaitingTime / n);
    printf("Average Turnaround Time: %.2f\n", (float)totalTurnaroundTime / n);
    printf("Average Response Time: %.2f\n", (float)totalResponseTime / n);

    // Calculate and print efficiency
    float efficiency = (float)n / totalTime;
    printf("Efficiency: %.2f\n", efficiency);
}

int main() {
    int n, timeQuantum;

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    struct Process processes[n];

    for (int i = 0; i < n; i++) {
        processes[i].id = i + 1;
        printf("Enter arrival time for process %d: ", i + 1);
        scanf("%d", &processes[i].arrivalTime);
        printf("Enter burst time for process %d: ", i + 1);
        scanf("%d", &processes[i].burstTime);
        processes[i].remainingTime = processes[i].burstTime;
        processes[i].waitingTime = 0;
        processes[i].turnaroundTime = 0;
        processes[i].completionTime = 0;  // Initialize completion time
        processes[i].responseTime = 0;    // Initialize response time
        processes[i].firstExecution = -1; // Initialize first execution time as -1 (not executed yet)
    }

    printf("Enter time quantum: ");
    scanf("%d", &timeQuantum);

    calculateEfficiency(processes, n, timeQuantum);

    return 0;
}
