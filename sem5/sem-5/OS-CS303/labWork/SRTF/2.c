#include <stdio.h>
#include <stdbool.h>

#define MAX_PROCESSES 100

int main()
{
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    int at[MAX_PROCESSES], bt1[MAX_PROCESSES], io_bt[MAX_PROCESSES], bt2[MAX_PROCESSES], ct[MAX_PROCESSES], wt[MAX_PROCESSES], tat[MAX_PROCESSES];
    int remaining_bt1[MAX_PROCESSES], remaining_bt2[MAX_PROCESSES];
    bool is_blocked[MAX_PROCESSES] = {false};
    int block_end_time[MAX_PROCESSES] = {0};
    int completed = 0, time = 0;
    int total_wt = 0, total_tat = 0;

    // Take input for AT, BT1, I/O BT, BT2
    for (int i = 0; i < n; i++)
    {
        printf("Enter [AT, BT1, I/O BT, BT2] for Process %d: ", i + 1);
        scanf("%d %d %d %d", &at[i], &bt1[i], &io_bt[i], &bt2[i]);
        remaining_bt1[i] = bt1[i];
        remaining_bt2[i] = bt2[i];
    }

    while (completed < n)
    {
        int shortest = -1;
        int min_bt = 9999;

        for (int i = 0; i < n; i++)
        {
            if (at[i] <= time && !is_blocked[i])
            {
                if (remaining_bt1[i] > 0 && remaining_bt1[i] < min_bt)
                {
                    min_bt = remaining_bt1[i];
                    shortest = i;
                }
                else if (remaining_bt1[i] == 0 && remaining_bt2[i] > 0 && remaining_bt2[i] < min_bt)
                {
                    min_bt = remaining_bt2[i];
                    shortest = i;
                }
            }
        }

        if (shortest == -1)
        {
            time++;
        }
        else
        {
            if (remaining_bt1[shortest] > 0)
            {
                remaining_bt1[shortest]--;
                time++;

                if (remaining_bt1[shortest] == 0)
                {
                    is_blocked[shortest] = true;
                    block_end_time[shortest] = time + io_bt[shortest];
                }
            }
            else
            {
                remaining_bt2[shortest]--;
                time++;

                if (remaining_bt2[shortest] == 0)
                {
                    completed++;
                    ct[shortest] = time;
                    tat[shortest] = ct[shortest] - at[shortest];
                    wt[shortest] = tat[shortest] - bt1[shortest] - io_bt[shortest] - bt2[shortest];
                    total_wt += wt[shortest];
                    total_tat += tat[shortest];
                }
            }
        }

        // Check if any blocked process can be unblocked
        for (int i = 0; i < n; i++)
        {
            if (is_blocked[i] && time >= block_end_time[i])
            {
                is_blocked[i] = false;
            }
        }
    }

    // Output
    printf("\nProcess\tAT\tBT1\tI/O\tBT2\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++)
    {
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", i + 1, at[i], bt1[i], io_bt[i], bt2[i], ct[i], tat[i], wt[i]);
    }

    printf("\nAverage Turnaround Time: %.2f", (float)total_tat / n);
    printf("\nAverage Waiting Time: %.2f\n", (float)total_wt / n);

    return 0;
}