#include<stdio.h>

#define N 10 // maximum 10 processes
struct process 
{
    int id;
    int at;
    int bt;
    int ct;
    int tat;
    int wt;
};
void sort(struct process p[], int n){
    for(int i = 0;i<n-1;i++){
        for(int j = i+1;j<n;j++){
            if(p[i].at>p[j].at){
                // swapping
                struct process temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }
}



int main(){
    int n;
    printf("Enter the number of processes: ");
    scanf("%d",&n);
    struct process p[N];
    // initialising process ids from 1 to n
    for(int i = 0;i<n;i++){
        p[i].id = i+1;
    }
    // taking input of processes 
    printf("Start entering processes: \n");
    for(int i = 0;i<n;i++){
        printf("Arrival time[%d]: ",i+1);
        scanf("%d",&p[i].at);
        printf("Burst time[%d]: ",i+1);
        scanf("%d",&p[i].bt);
    }
    // sorting processes based on arrival time 
    sort(p,n);
    int curr_time = 0;
    for(int i = 0;i<n;i++){
        if(curr_time<p[i].at) curr_time+=(p[i].at-curr_time);
        curr_time += p[i].bt;
        p[i].ct = curr_time;
    }
    // computing tat and wt
    for(int i = 0;i<n;i++){
        p[i].tat = p[i].ct-p[i].at;
        p[i].wt = p[i].tat-p[i].bt;
    }
    printf("\n\n\n\n");
    printf("----------Printing gantt chart----------\n");
    // printing gantt chart
    printf("|");
    for(int i = 0;i<n-1;i++){
        printf("P(%d)|",i+1);
    }
    printf("P(%d)|\n",n-1);
    for(int i = 0;i<n;i++){
        printf("    %d",p[i].ct);
    }
    printf("\n\n\n\n");
    // printing processes 
    printf("----------Printing process table----------\n");
    printf("Id\tAT\tBT\tCT\tTAT\tWT\n");
    for(int i = 0;i<n;i++){
        printf("%d\t%d\t%d\t%d\t%d\t%d\n",p[i].id,p[i].at,p[i].bt,p[i].ct,p[i].tat,p[i].wt);
    }
    
    
    return 0;
}
// 5 0 4 1 3 2 5 3 2 4 1 