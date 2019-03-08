// g++ main.cpp -o SFIFO -pthread
#include <cstdlib>
#include <iostream>
#include <pthread.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

void printTableHeader();

void printFlowRow(int, int, int);

void printServerRow(int, int, int);

void *flow(void *);

void *server(void *);

class Queue {
public:
    Queue() {
        rear = 0;
        tokenAdded = 0;
        tokenFetched = 0;
        tokenDropped = 0;
    };

    int getRear() { return rear; };

    int getTokenFetched() { return tokenFetched; };

    int getTokenAdded() { return tokenAdded; };

    int getTokenDropped() { return tokenDropped; };

    void addItem(char);

    char removeItem();

private:
    char queue[50];
    int rear;
    int tokenAdded;
    int tokenFetched;
    int tokenDropped;
};

void Queue::addItem(char item) {
    if (rear >= 50) {
        tokenDropped++;
        return;
    }
    queue[rear] = item;
    rear++;
    tokenAdded++;
}

char Queue::removeItem() {
    if (rear <= 0) return 0;
    char tmp = queue[0];
    // Move the queue forward
    for (int i = 0; i < rear - 1; i++) queue[i] = queue[i + 1];
    queue[rear] = 0;
    rear--;
    tokenFetched++;
    return tmp;
}

// Global variables
int maxToken;
int flowInterval;
Queue *queue = new Queue();
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char *argv[]) {
    // Check user input
    if (argc != 3 || atoi(argv[1]) < 1 || atoi(argv[2]) < 1) {
        cout << "Invalid argument supplied !" << endl;
        return 0;
    }
    // Init global variable
    maxToken = atoi(argv[1]);
    flowInterval = atoi(argv[2]);
    // Print initial text
    cout << "The Max token is " << maxToken << " and the interval time for flow is " << flowInterval << endl;
    printTableHeader();
    // Create flow thread
    pthread_t flowT;
    pthread_create(&flowT, NULL, flow, NULL);
    // Create server thread
    pthread_t serverT;
    pthread_create(&serverT, NULL, server, NULL);
    // Wait 2 threads finish
    pthread_join(flowT, NULL);
    pthread_join(serverT, NULL);
    // Display result
    cout << "The total number of tokens that have been fetched by the server is " << queue->getTokenFetched() << endl;
    cout << "The total number of tokens that have been generated by the flow is " << queue->getTokenAdded() << endl;
    cout << "The total number of tokens that have been dropped by the queue is " << queue->getTokenDropped() << endl;
    return 0;
}

// Flow thread Logic
void *flow(void *) {
    while (true) {
        sleep(flowInterval);
        // Lock all resources
        pthread_mutex_lock(&lock);
        // Escape when token count meet the max count
        if (queue->getTokenFetched() >= maxToken) break;
        // Random token counts
        int randomCount = (rand() % 10) + 1;
        // Insert tokens
        // Generate fake char as data to simulate real queue
        for (int i = 0; i < randomCount; i++) queue->addItem((char) (rand() % 26 + 65));
        // Print out result
        printFlowRow(randomCount, queue->getTokenAdded() - 1, queue->getRear());
        // Unlock all resources
        pthread_mutex_unlock(&lock);
    }
    // Force unlock when work done
    pthread_mutex_unlock(&lock);
    pthread_exit(NULL);
}

// Server thread Logic
void *server(void *) {
    while (true) {
        sleep(2);
        // Lock all resources
        pthread_mutex_lock(&lock);
        // Escape when token count meet the max count
        if (queue->getTokenFetched() >= maxToken) break;
        // Random token counts
        int randomCount = (rand() % 20) + 1;
        int remainDiff = maxToken - queue->getTokenFetched();
        int actualCount =
                randomCount > remainDiff ? remainDiff : randomCount > queue->getRear() ? queue->getRear() : randomCount;
        // Remove tokens
        // queue->removeItem() should return the data in the queue
        for (int i = 0; i < actualCount; i++) queue->removeItem();
        // Print out result
        printServerRow(queue->getRear(), actualCount, queue->getTokenFetched());
        // Unlock all resources
        pthread_mutex_unlock(&lock);
    }
    // Force unlock when work done
    pthread_mutex_unlock(&lock);
    pthread_exit(NULL);
}

// Table display template
void printTableHeader() {
    printf("%-35s  %-14s  %-34s\n", "Flow", "Queue", "Server");
    printf("%-11s  %-22s  %-14s  %-13s  %-19s\n", "Token added", "Latest sequence number", "Current Length",
           "Token fetched", "Total Token fetched");
}

void printFlowRow(int tokenAdded, int latestSqNo, int currentLen) {
    printf("%-11d  %-22d  %-14d\n", tokenAdded, latestSqNo, currentLen);
}

void printServerRow(int currentLen, int tokenFetched, int totalTokenFetched) {
    printf("%36s %-14d  %-13d  %-19d\n", "", currentLen, tokenFetched, totalTokenFetched);
}