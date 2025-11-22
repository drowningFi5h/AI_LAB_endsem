#include<stdio.h>
#include<stdlib.h>

typedef struct Process {
  int pid;
} Process;

struct Node {
  Process process;
  struct Node* next;
};

struct Queue {
  struct Node* front;
  struct Node* rear;
};

struct Semaphore {
  int value;
  struct Queue* q;
};

struct Queue* createQueue() {
  struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
  queue->front = queue->rear = NULL;
  return queue;
}

void enqueue(struct Queue* q, Process process) {
  struct Node* temp = (struct Node*)malloc(sizeof(struct Node));
  temp->process = process;
  temp->next = NULL;
  if (q->rear == NULL) {
    q->front = q->rear = temp;
    return;
  }
  q->rear->next = temp;
  q->rear = temp;
}

Process dequeue(struct Queue* q) {
  if (q->front == NULL) {
    Process emptyProcess = {-1}; 
    return emptyProcess;
  }
  struct Node* temp = q->front;
  Process process = temp->process;
  q->front = q->front->next;

  if (q->front == NULL) {
    q->rear = NULL;
  }

  free(temp);
  return process;
}

void P(struct Semaphore* s, Process currentProcess) {
  s->value -= 1;
  if (s->value < 0) {
    enqueue(s->q, currentProcess);
    printf("Process %d is blocked\n", currentProcess.pid);
  } else {
    printf("Process %d continues execution\n", currentProcess.pid);
  }
}

void V(struct Semaphore* s) {
  s->value += 1;
  if (s->value <= 0) {
    Process unblockedProcess = dequeue(s->q);
    if (unblockedProcess.pid != -1) {
      printf("Process %d is woken up\n", unblockedProcess.pid);
    }
  } else {
    printf("No process to wake up\n");
  }
}
 
int main() {
  struct Semaphore s;
  s.value = 1;
  s.q = createQueue();

  Process p1 = {1};
  Process p2 = {2};

  P(&s, p1); 
  P(&s, p2); 
  V(&s);     
  P(&s, p1); 
  P(&s, p2); 
  V(&s);     
  P(&s, p2); 
  P(&s, p1); 
  V(&s); 
  V(&s);     
  return 0;
}
