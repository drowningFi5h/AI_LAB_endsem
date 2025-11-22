#include <stdio.h>
#include <limits.h>

typedef struct {
    int at, cpu1, io, cpu2;
    int ct, tat, wt, rt, remaining_time1, remaining_time2, io_done_time;
    int started, io_started;
} Process;

int all_done(Process p[], int n) {
    for (int i = 0; i < n; i++) {
        if (p[i].remaining_time1 > 0 || p[i].remaining_time2 > 0) {
            return 0;
        }
    }
    return 1;
}

void calculate_times(Process p[], int n) {
    int current_time = 0, completed = 0, min_index;
    int total_tat = 0, total_wt = 0, total_rt = 0;
    
    while (!all_done(p, n)) {
        min_index = -1;
        int min_time = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (p[i].at <= current_time) {
                if (p[i].remaining_time1 > 0 && p[i].remaining_time1 < min_time) {
                    min_time = p[i].remaining_time1;
                    min_index = i;
                } else if (p[i].remaining_time1 == 0 && current_time >= p[i].io_done_time && p[i].remaining_time2 > 0 && p[i].remaining_time2 < min_time) {
                    min_time = p[i].remaining_time2;
                    min_index = i;
                }
            }
        }

        if (min_index == -1) {
            current_time++;
            continue;
        }

        if (p[min_index].remaining_time1 > 0) {
            if (p[min_index].started == 0) {
                p[min_index].rt = current_time - p[min_index].at;
                p[min_index].started = 1;
            }
            current_time++;
            p[min_index].remaining_time1--;

            if (p[min_index].remaining_time1 == 0) {
                p[min_index].io_done_time = current_time + p[min_index].io;
            }
        } else if (p[min_index].remaining_time2 > 0 && current_time >= p[min_index].io_done_time) {
            current_time++;
            p[min_index].remaining_time2--;

            if (p[min_index].remaining_time2 == 0) {
                p[min_index].ct = current_time;
                p[min_index].tat = p[min_index].ct - p[min_index].at;
                p[min_index].wt = p[min_index].tat - p[min_index].cpu1 - p[min_index].cpu2;
                total_tat += p[min_index].tat;
                total_wt += p[min_index].wt;
                total_rt += p[min_index].rt;
                completed++;
            }
        }
    }

    printf("P#\tAT\tCPU1\tIO\tCPU2\tCT\tTAT\tWT\tRT\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", i+1, p[i].at, p[i].cpu1, p[i].io, p[i].cpu2, p[i].ct, p[i].tat, p[i].wt, p[i].rt);
    }

    printf("Average TAT: %.2f\n", (float)total_tat / n);
    printf("Average WT: %.2f\n", (float)total_wt / n);
    printf("Average RT: %.2f\n", (float)total_rt / n);
}

int main() {
    int n;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    Process p[n];

    for (int i = 0; i < n; i++) {
        printf("Enter AT, CPU1, IO, and CPU2 for process %d: ", i+1);
        scanf("%d %d %d %d", &p[i].at, &p[i].cpu1, &p[i].io, &p[i].cpu2);
        p[i].remaining_time1 = p[i].cpu1;
        p[i].remaining_time2 = p[i].cpu2;
        p[i].started = 0;
        p[i].io_started = 0;
    }

    calculate_times(p, n);

    return 0;
}