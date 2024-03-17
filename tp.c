//ficheiro com operações em tabelas e paths
#include "header.h"

//tabelas e 2 listas globais? xd
struct Path routingTable[MAX_CLIENTS-1][MAX_CLIENTS-1]; 
int mapIndices[100];     // List that maps node id to RT id ex: mapIndices(70) = 3
int invIndices[MAX_CLIENTS]; //list that maps RT id to node id ex: invIndices(3) = 70
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
            routingTable[i][j].size = 0;
        }
    }
}

void initSPT() {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        sptable[i].size = 0;
    }
}

void initEXP() {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        expeditionTable[i] = -1;
    }
}


void initmapIndices() {
    for (int i = 0; i < 99; i++) {
        mapIndices[i] = -1;
    }
}

int getOrAssignIndex(int nodeID) {
    
    if (nodeID < 0 || nodeID >= 99) {
        printf("Error: Node ID %d is out of range.\n", nodeID);
        return -1;
    }

    if (mapIndices[nodeID] != -1) { // If already assigned, return the existing index
        return mapIndices[nodeID];
    } else {
        int tfull = 1;
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (invIndices[i] == -1) {
                invIndices[i] = nodeID;
                mapIndices[nodeID] = i;
                tfull = 0;
                return i;
            }
        if (tfull == 1){
            printf("Error: No available index to assign for Node ID %d.\n", nodeID);
            return -1;

        }


        }
    }

   
}

// Remove index from mapIndices
void removeIndex(int nodeID) {
    if (nodeID >= 0 && nodeID < MAX_NODES) {
        invIndices[mapIndices[nodeID]] = -1;
        mapIndices[nodeID] = -1;
    } else {
        printf("Error: Node ID %d is out of range.\n", nodeID);
    }
}
void updateRT(struct Path path) { //update routing table after receiving ROUTE INFO
    destinationId = dest(path);
    sourceId = source(path);
    
    int rowIndex = getOrAssignIndex(rowIndices, destinationId);
    int columnIndex = getOrAssignIndex(columnIndices, sourceId);

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
