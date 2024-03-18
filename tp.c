//ficheiro com operações em tabelas e paths
#include "header.h"

//tabelas e 2 listas globais? xd
struct Path routingTable[MAX_CLIENTS-1][MAX_CLIENTS-1]; 
int mapIndices[100];     // List that maps node id to RT id ex: mapIndices(70) = 3
int invIndices[MAX_CLIENTS]; //list that maps RT id to node id ex: invIndices(3) = 70
struct Path sptable[MAX_CLIENTS];                   
int expeditiontable[MAX_CLIENTS];    
extern int mid;

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
    if (path.size > 0) {
        return path.route[0]; 
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
        expeditiontable[i] = -1;
    }
}


void initmapIndices() {
    for (int i = 0; i < 99; i++) {
        mapIndices[i] = -1;
    }
}

void initinvIndices() {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        invIndices[i] = -1;
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
		}
        if (tfull == 1){
            printf("Error: No available index to assign for Node ID %d.\n", nodeID);
            return -1;

        }

    }

   
}

// Remove index from mapIndices
void removeIndex(int nodeID) {
    if (nodeID >= 0 && nodeID < 100) {
        invIndices[mapIndices[nodeID]] = -1;
        mapIndices[nodeID] = -1;
    } else {
        printf("Error: Node ID %d is out of range.\n", nodeID);
    }
}
void updateRT(struct Path path) { 
    int sourceID = source(path);  // Get the source ID from the path
    int destinationID = dest(path);  // Get the destination ID from the path
    if (sourceID == -1 || destinationID == -1) {
        printf("Error: Invalid path source or destination.\n");
        return;
    }

    // Find or assign indices in the RT
    int rowIndex = getOrAssignIndex(destinationID);
    int colIndex = getOrAssignIndex(sourceID);

    if (rowIndex == -1 || colIndex == -1) {
        printf("Error: Unable to find or assign RT index for Node IDs %d -> %d.\n", sourceID, destinationID);
        return;
    }

    routingTable[rowIndex][colIndex] = path;
        
    // Check if the SPT needs updating
    if (sptable[rowIndex].size == 0 || path.size < sptable[rowIndex].size) {
        sptable[rowIndex] = path; // Update the SPT with the new shorter path
        expeditiontable[rowIndex] = sourceID; //update EXP
		adj_route(path);
     }
}
void send_route(struct Path path, int fd){
    int dst=dest(path),i,n;
    char send[500];

	if(dst == -1) dst = mid;

    sprintf(send,"ROUTE %d %d %d", mid, dst, mid);
    for ( i = 0; i <= path.size; i++){
        n=strlen(send);
        if (i==path.size){
           send[n]='\n';
           send[n+1]='\0';
        }else{
            send[n]= '-';
            send[n+1]=(path.route[i]/10)+'0';
            send[n+2]=(path.route[i]%10)+'0';
            send[n+3]='\0';
        }
    }
    n=write(fd,send,strlen(send));
    if(n==-1)/*error*/ exit(1);
    
    return;   
}

void routall(int fd){
	struct Path empty;
	empty.size = 0;
	send_route(empty, fd); //Route mid mid mid

	for(int i = 0; i < MAX_CLIENTS; i++){
		if(sptable[i].size == 0)
			continue;//If not path die
		else send_route(sptable[i], fd);//send path
	}
}

void adj_route(struct Path path){
	if (succ.fd > 0)
		send_route(path, succ.fd);
	if(pred.fd > 0)
		send_route(path, pred.fd);
}

void prtRoute(){
	for(int i = 0; i < (MAX_CLIENTS-1); i++){
		for (int j = 0; j < (MAX_CLIENTS -1); j++){
			if((routingTable[i][j].size == -1) || (routingTable[i][j].size == 0)) continue;
			printf("dest:%d pass-through:%d", source(routingTable[i][j]),dest(routingTable[i][j]));
			for(int k = 0; k < routingTable[i][j].size; k++)
				printf("%d - ", routingTable[i][j].route[k]);
			printf("\n");
		}
		printf("\n");
	}
}
