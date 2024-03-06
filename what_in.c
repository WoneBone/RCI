//Este ficheiro serve para funçoes de identificação de comandos 
#include "header.h"
extern int errcode,mid,n;
extern char *mIP, *mTCP;
extern struct node succ, sucsuc, pred;

int what_serv(int fd, char *mess){ //TCP SERVER SIDE
    char code_word[100],trash[500],trashp[500];
    int n;
    strcpy(trash,mess);
    sscanf(mess,"%s",code_word);
    if (strcmp(code_word,"ENTRY")==0){ //ENTRY RECEIVED FROM OUTSIDE
        if (pred.id==-1){ //ME ALONE
            sscanf(mess,"%s %d %s %s",code_word,&pred.id,pred.ip,pred.port);
            pred.fd=fd; //U MY PRED NOW
            sucsuc.id=mid; // ME SUCSUC ME CAUSE ME WAS ALONE :'(
            strcpy(sucsuc.ip,mIP);
            strcpy(sucsuc.port,mTCP);
        }else{ //NOT ALONE! :D life is good...
            puts(mess);
            n=write(pred.fd,mess,strlen(mess) + 1); //REPLAY ENTRY MSG TO MY PRED
            if(n==-1)/*error*/ exit(1);
            sscanf(mess,"%s %d %s %s",code_word,&pred.id,pred.ip,pred.port);
            pred.fd=fd;
        }
        
        if (succ.id==-1){ //IF ALONE, NEW SUC IS NEW PRED and I TCP conect to his server as a client
            succ.id=pred.id;
            strcpy(succ.ip,pred.ip);
            strcpy(succ.port,pred.port);
            succ.fd = tcp_client(succ.ip, succ.port);
            sprintf(trashp,"PRED %d\n",mid);
            n=write(succ.fd,trashp,strlen(trashp)+1);
            if(n==-1)/*error*/ exit(1);
        }
        sprintf(trash,"SUCC %d %s %s\n",succ.id,succ.ip,succ.port); // I TELL NEW GUY WHO IS HIS SUCSUC
        
        n=write(fd,trash,strlen(trash)+1);
        if(n==-1)/*error*/ exit(1);
        
        return 0;
    }
    if (strcmp(code_word,"PRED")==0){ //SOMEONE TOLD ME THEY ARE MY PRED
        sscanf(mess,"%s %d",code_word,&pred.id);
        pred.fd=fd;
        return 0;
    }
    if (strcmp(code_word,"CHORD")==0){
        printf("tie the knot well");
        return 0;
    }
    return 1;
}

int what_clit(int fd, char *mess){
    char code_word[100],trash[500];
    int n;
    strcpy(trash,mess);
    sscanf(mess,"%s",code_word);
    if (strcmp(code_word,"ENTRY")==0){ //NEW GUY IN FRONT OF ME
        sscanf(mess,"%s %d %s %s",code_word,&succ.id,succ.ip,succ.port);
        succ.fd=fd;//DUVIDA, ISTO NAO FAZ NADA, EU ESTOU RECEBER MSG DO MEU SUCC MAS O MEU SUCC VAI PASSAR A SER QUEM ENTROU,
        //LOGO TENHO DE FAZER CONNECTION com succ.port (estou a vir para aqui com whatclit(succ.fd) logo fd = succ.fd)
        n=write(succ.fd,"PRED %d\n",mid);//I TELL NEW GUY IM BEHIND HIM
        if(n==-1)/*error*/ exit(1);
        sprintf(trash,"SUCC %d %s %s\n",succ.id,succ.ip,succ.port); //aviso o meu pred do seu novo SUCSUC
        n=write(pred.fd,trash,sizeof(trash));
        if(n==-1)/*error*/ exit(1);
        return 0;
    }
    if (strcmp(code_word,"SUCC")==0){
        sscanf(mess,"%s %d %s %s",code_word,&sucsuc.id,sucsuc.ip,sucsuc.port);
        return 0;
    }
    if (strcmp(code_word,"PRED")==0){
        sscanf(mess,"%s %d",code_word,&pred.id); //duvida como client nunca recebo um PRED right?
        pred.fd=fd;
        return 0;
    }
    
    return 1;
}

int what_std(char *std_in,struct addrinfo *res){
    char code_word[100],succIP[100],succTCP[100],show[100];
    int ring,id,succid;
    sscanf(std_in,"%s",code_word);
    if (strcmp(code_word,"join")==0 || strcmp(code_word,"j")==0){
        sscanf(std_in,"%s %d %d",code_word,&ring,&id);
        mid=join(ring,id,res); 
        
        return 0;
    }
    if (strcmp(code_word,"direct")==0 || strcmp(code_word,"dj")==0){
        sscanf(std_in,"%s %d %d %s %s",code_word,&id,&succid,succIP,succTCP);
        d_join(id,succid,succIP,succTCP);
        return 0;
    }
    if (strcmp(code_word,"chord")==0 || strcmp(code_word,"c")==0){
       printf("tie the knot well");
        return 0;
    }
    if (strcmp(code_word,"remove")==0 || strcmp(code_word,"rc")==0){
       
        return 0;
    }
    if (((sscanf(std_in,"%s %s",code_word,show)==2)&&(strcmp(show,"topology")==0))||(strcmp(code_word,"st")==0)){
        
        printf("Estado do protocolo topologico:\n");
        printf("Utilizador:\n");
        if(mid!=-1){
            printf("\tid-%d\n",mid);
        }else{
            printf("\tid-Não definido\n");
        }
        printf("\tIP-%s\n",mIP);
        printf("\tPort-%s\n",mTCP);
        if (succ.id==-1){
            printf("Sucessor ainda não definido\n");
        }else{
            printf("Sucessor:\n");//conmo vou ter id dos outros 
            printf("\tIP-%s\n",succ.ip);
            printf("\tPort-%s\n",succ.port);
        }
        if (sucsuc.id==-1){
            printf("Segundo sucessor ainda não definido\n");
        }else{
            printf("Segundo sucessor:\n");//conmo vou ter id dos outros 
            printf("\tIP-%s\n",sucsuc.ip);
            printf("\tPort-%s\n",sucsuc.port);
        }
        //falta corda
        return 0;
    }
    
    
    if (strcmp(code_word,"show")==0 || strcmp(code_word,"sr")==0){
       
        return 0;
    }
    if (strcmp(code_word,"show")==0 || strcmp(code_word,"sp")==0){
       
        return 0;
    }
    if (strcmp(code_word,"show")==0 || strcmp(code_word,"sf")==0){
       
        return 0;
    }
    if (strcmp(code_word,"message")==0 || strcmp(code_word,"m")==0){
       
        return 0;
    }
    if (strcmp(code_word,"leave")==0 || strcmp(code_word,"l")==0){
       leave(ring, mid, res);
        return 0;
    }
    if (strcmp(code_word,"exit")==0 || strcmp(code_word,"x")==0){
       
        return 0;
    }
    return 0;
}

