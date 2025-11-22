#include <stdio.h>
#include <stdlib.h>

#define frame 256 // 256 bytes per frame
#define num_frame 8 // 8 frames
#define num_pages 16 // 16 pages

int pageTable[num_pages];
char memory[num_frame][frame];

void initializePageTable() {
    for (int i = 0; i < num_pages; i++) {
        pageTable[i] = -1;
    }
}

void loadPage(int pageNumber, int frameNumber) {
    if (pageNumber >= num_pages) {
        printf("Error: Page number out of range.\n");
        return;
    }
    pageTable[pageNumber] = frameNumber;
    for (int i = 0; i < frame; i++) {
        memory[frameNumber][i] = 'A' + pageNumber; // loading page content
    }
    printf("Page %d loaded into frame %d.\n", pageNumber, frameNumber);
}

void displayPageTable() {
    printf("Page Table:\n");
    for (int i = 0; i < num_pages; i++) {
        if (pageTable[i] != -1) {
            printf("Page %d -> Frame %d\n", i, pageTable[i]);
        } else {
            printf("Page %d -> Not Loaded\n", i);
        }
    }
}

void accessPage(int pageNumber) {
    if (pageNumber >= num_pages) {
        printf("Error: Page number out of range.\n");
        return;
    }
    int frameNumber = pageTable[pageNumber];
    if (frameNumber == -1) {
        printf("Page %d is not loaded in memory.\n", pageNumber);
    } else {
        printf("Accessing page %d in frame %d: Content starts with '%c'\n", pageNumber, frameNumber, memory[frameNumber][0]);
    }
}

int main() {
    initializePageTable();
    loadPage(0, 0);
    loadPage(1, 1);
    loadPage(2, 2);
    loadPage(3, 3);
    displayPageTable();
    accessPage(0);
    accessPage(2);
    accessPage(4);
    return 0;
}
