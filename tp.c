//ficheiro com operações em tabelas e paths
#include "header.h"

//tabelas e 2 listas globais? xd
struct Path routingTable[MAX_CLIENTS-1][MAX_CLIENTS-1]; 
int rowIndices[MAX_CLIENTS];     // List that maps rows to destination IDs
int columnIndices[MAX_CLIENTS];  // List that maps columns to source IDs
struct Path sptable[MAX_CLIENTS];                   
int expeditionTable[MAX_CLIENTS];    

struct Path initPath() {
    struct Path path;
    for (int i = 0; i < MAX_CLIENTS; i++) {
        path.route[i] = -1;
    }
    path.size = 0;
    return path;
}

int dest(struct Path path) {
    if (path.size > 0) {
        return path.route[path.size - 1]; 
    }
    return -1; 
}

int source(struct Path path) {
    if (path.size > 1) {
        return path.route[1]; 
    }
    return -1;
}

struct Path addNode(struct Path path, int nodeId) {
    if (nodeId >= 0 && nodeId < MAX_CLIENTS && path.size < MAX_CLIENTS) {
        for (int i = 0; i < path.size; i++) {
            if (path.route[i] == nodeId) {
                return path;
            }
        }
        path.route[path.size++] = nodeId;
    }
    return path;
}

void initRT() {
    for (int i = 0; i < MAX_CLIENTS - 1; i++) {
        for (int j = 0; j < MAX_CLIENTS - 1; j++) {
            routingTable[i][j] = initPath();
        }
    }
}

void initSPT() {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        sptable[i] = initPath();
    }
}

void initEXP() {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        expeditionTable[i] = -1;
    }
}


void initIndexLists() {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        rowIndices[i] = -1;
        columnIndices[i] = -1;
    }
}

int findOrAssignIndex(int list[], int nodeId) { // Find or assign the next free index for a given node ID in a specified list
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (list[i] == nodeId) {
            return i;  
        }
        if (list[i] == -1) {
            list[i] = nodeId;  
            return i;
        }
    }
    return -1;  // List is full or Node ID already exists
}

void updateRT(struct Path path) { //update routing table after receiving ROUTE INFO
    destinationId = dest(path);
    sourceId = source(path);
    
    int rowIndex = findOrAssignIndex(rowIndices, destinationId);
    int columnIndex = findOrAssignIndex(columnIndices, sourceId);

    if (rowIndex != -1 && columnIndex != -1) {
        routingTable[rowIndex][columnIndex] = path;
    } else {
        printf("Error: Unable to assign a new route in the routing table.\n");
    }
}


void updateSPT() { //after updateRT after receiving ROUTE INFO
    for (int i = 0; i < MAX_CLIENTS - 1; i++) {
        sptable[i] = initPath();
        for (int j = 0; j < MAX_CLIENTS - 1; j++) {
            struct Path currentPath = routingTable[i][j];
            if (currentPath.size > 0 && (sptable[i].size == 0 || currentPath.size < sptable[i].size)) {
                sptable[i] = currentPath;
            }
        }
    }
}

void updateEXP() {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (sptable[i].size > 1) {
            expeditionTable[i] = sptable[i].route[1]; 
        }
    }
}