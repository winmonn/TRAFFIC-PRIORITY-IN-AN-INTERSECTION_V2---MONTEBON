#include <stdio.h> 
#include <stdlib.h>

#define MAX 5

typedef enum {
    MainLaneStraight = 0, DivLaneStraight, MainLaneLeft, DivLaneLeft, MainLaneRight, DivLaneRight, MainPedestrian, DivPedestrian
} TrafficLanes;

typedef struct {
    int time;
    TrafficLanes lane;
} Traffic;

typedef struct {
    Traffic data[MAX];
    int lastIndex;
} PriorityQueue;

Traffic createTraffic(int time, TrafficLanes lane) {
    Traffic traffic;
    traffic.time = time;
    traffic.lane = lane;
    return traffic;
}

void displayTraffic(Traffic* t) {
    printf("%-20s %-10s\n", "Time", "Lane");
    for (int i = 0; i < MAX; i++) {
        printf("%-20d %-10d\n", t[i].time, t[i].lane);
    }
}

void enqueueTraffic(PriorityQueue* pq, Traffic t) {
    if (pq->lastIndex < MAX - 1) {
        pq->data[++pq->lastIndex] = t;

        int childIndex = pq->lastIndex;
        int parentIndex = (childIndex - 1) / 2;

        while (childIndex != 0 && pq->data[childIndex].lane < pq->data[parentIndex].lane) {
            Traffic temp = pq->data[childIndex];
            pq->data[childIndex] = pq->data[parentIndex];
            pq->data[parentIndex] = temp;

            childIndex = parentIndex;
            parentIndex = (childIndex - 1) / 2;
        }
    } 
}

int calculateTotalTime(PriorityQueue pq) {
    int total = 0;
    int index = 0;
    FILE* filePtr = fopen("Dequeue.dat", "wb");
    for (int i = 0; i <= pq.lastIndex; i++) {
        if (pq.data[index].lane != MainPedestrian && pq.data[index].lane != DivPedestrian) {
            total += pq.data[index].time;
            if (filePtr != NULL) {
                fwrite(&pq.data[index], sizeof(Traffic), 1, filePtr);
            }
        } 
        index = (index + 1) % MAX;
    }
    fclose(filePtr);
    return total;
}

void displayQueue(PriorityQueue* pq) {
    int index = 0;
    printf("Priority Queue:\n");
    for (int i = 0; i <= pq->lastIndex; i++) {
        printf("Lane: %d, Time: %d\n", pq->data[index].lane, pq->data[index].time);
        index = (index + 1) % MAX;
    }
}

int main() {
    Traffic trafficData[MAX];
    PriorityQueue priorityQueue = {.lastIndex = -1};

    trafficData[0] = createTraffic(5, MainPedestrian);
    trafficData[1] = createTraffic(9, MainLaneRight);
    trafficData[2] = createTraffic(20, MainLaneStraight);
    trafficData[3] = createTraffic(31, DivLaneRight);
    trafficData[4] = createTraffic(60, DivLaneStraight);

    FILE* filePtr = fopen("traffic.dat", "wb");
    if (filePtr != NULL) {
        fwrite(&trafficData, sizeof(Traffic), MAX, filePtr);
    }
    fclose(filePtr);

    printf("Traffic List:\n");
    displayTraffic(trafficData);

    for (int i = 0; i < MAX; i++) {
        enqueueTraffic(&priorityQueue, trafficData[i]);
    }
    displayQueue(&priorityQueue);

    int result = calculateTotalTime(priorityQueue);
    printf("The total time before a main pedestrian can cross is: %d\n", result);

    FILE* resultFile = fopen("traffic_result.dat", "wb");
    if (resultFile != NULL) {
        fprintf(resultFile, "The total time before a main pedestrian can cross is: %d\n", result);
    }
    fclose(resultFile);
}
