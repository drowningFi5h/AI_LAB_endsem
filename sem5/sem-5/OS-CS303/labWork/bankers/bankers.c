// Kunj Thakkar 
// 202251142
#include<stdio.h>
#include<stdlib.h>
#define N 10 // max number of resources and max number of processes are 10
int total_resources[N];
int available_resources[N];
struct process 
{
    int pid;
    int max[N];
    int alloc[N];    
    int need[N];
};
struct node {
  int pid;
  struct node* next;
};
struct queue{
  struct node* front;
  struct node* rear;
};
struct queue* createQueue(){
  struct queue* q = (struct queue*)malloc(sizeof(struct queue));
  q-> front = q -> rear = NULL;
  return q;
}
void enqueue(struct queue* q, int pid){
  struct node* n = (struct node*)malloc(sizeof(struct node));
  n -> pid = pid;
  n -> next = NULL;
  if(q -> rear == NULL){
    q -> front = q -> rear = n;
    return ;
  }
  q -> rear -> next = n;
  return;
}
int dequeue(struct queue* q){
  if(q -> front == NULL){
    return -1;
  }
  struct node* n = q -> front;
  int id = n -> pid;
  q -> front = q -> front -> next;

  if(q -> front == NULL){
    q -> rear = NULL;
  }
  free(n);
  return id;
}
void print_process_details(struct process total_process[], int num_processes, int num_resources) {
    printf("Process details:\n");
    for (int i = 0; i < num_processes; i++) {
        printf("Process ID: %d\n", total_process[i].pid);
        printf("Maximum Resources: ");
        for (int j = 0; j < num_resources; j++) {
            printf("%d ", total_process[i].max[j]);
        }
        printf("\nAllocated Resources: ");
        for (int j = 0; j < num_resources; j++) {
            printf("%d ", total_process[i].alloc[j]);
        }
        printf("\nNeeded Resources: ");
        for (int j = 0; j < num_resources; j++) {
            printf("%d ", total_process[i].need[j]);
        }
        printf("\n\n");
    }
}
int main(){
    struct process total_process[N];
    int num_processes,num_resources;
    printf("Enter the number of processes : ");
    scanf("%d",&num_processes);
    printf("Enter the number of resouces : ");
    scanf("%d",&num_resources);
    printf("Enter total available resource : \n");
    for(int i = 0;i<num_resources;i++){
        scanf("%d",total_resources[i]);
    }
    printf("Start entering details of processes : \n");
    for(int i = 0;i<num_processes;i++){
        printf("Enter process id of process %d : ",i+1);
        scanf("%d",&total_process[i].pid);
        printf("Enter maximum resources required by process %d",i+1);
        for(int j = 0;j<num_resources;j++){
            scanf("%d",&total_process[i].max[j]);
        }
        printf("Enter allocated resources of process %d",i+1);
        for(int j = 0;j<num_resources;j++){
            scanf("%d",&total_process[i].alloc[j]);
        }
    }
    // calculating the needs of all processes
    for(int i = 0;i<num_processes;i++){
        for(int j = 0;j<num_resources;j++){
            total_process[i].need[j] = total_process[i].max[j] - total_process[i].alloc[j];
        }
    }
    print_process_details(total_process, num_processes, num_resources);
    return 0;
}