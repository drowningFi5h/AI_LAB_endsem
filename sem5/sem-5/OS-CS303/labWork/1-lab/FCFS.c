// FCFS
#include <stdio.h>

int main() {
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    
    int at[n], bt[n], ct[n], tat[n], wt[n], p[n];
    float average_tat = 0, average_wt = 0;
    
    for(int i = 0; i < n; i++) {
        p[i] = i + 1; // Process number
        printf("Arrival time for process %d: ", i+1);
        scanf("%d", &at[i]);
        printf("Burst time for process %d: ", i+1);
        scanf("%d", &bt[i]);
        printf("---------------\n");
    }
    
    // Sort processes based on arrival time using Bubble Sort
    for(int i = 0; i < n-1; i++) {
        for(int j = 0; j < n-i-1; j++) {
            if(at[j] > at[j+1]) {
                // Swap arrival time
                int temp = at[j];
                at[j] = at[j+1];
                at[j+1] = temp;
                
                // Swap burst time
                temp = bt[j];
                bt[j] = bt[j+1];
                bt[j+1] = temp;
                
                // Swap process number
                temp = p[j];
                p[j] = p[j+1];
                p[j+1] = temp;
            }
        }
    }
    
    ct[0] = at[0] + bt[0];
    for(int i = 1; i < n; i++) {
        if(at[i] > ct[i-1]) {
            ct[i] = at[i] + bt[i];
        } else {
            ct[i] = ct[i-1] + bt[i];
        }
    }
    
    for(int i = 0; i < n; i++) {
        tat[i] = ct[i] - at[i];
        wt[i] = tat[i] - bt[i];
    }
    
    for(int i = 0; i < n; i++) {
        average_tat += tat[i];
        average_wt += wt[i];
    }
    
    average_tat /= n;
    average_wt /= n;
    
    printf("\nProcess No.\tAT\tBT\tCT\tTAT\tWT\n");
    for(int i = 0; i < n; i++) {
        printf("%d\t\t%d\t%d\t%d\t%d\t%d\n", p[i], at[i], bt[i], ct[i], tat[i], wt[i]);
    }
    
    printf("\nAverage Turnaround Time: %.2f\n", average_tat);
    printf("Average Waiting Time: %.2f\n", average_wt);
    
    return 0;
}