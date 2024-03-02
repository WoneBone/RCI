//Este ficheiro serve para funçoes de identificação de comandos 
#include "header.h"
extern int errcode,mid;
extern char *mIP, *mTCP;
extern struct node succ, sucsuc, pred;

int what_serv(int fd, char *mess){
    char code_word[100];
    sscanf(mess,"%s",code_word);
    if (strcmp(code_word,"ENTRY")==0){
        sscanf(mess,"%s %d %s %s",code_word,&pred.id,pred.ip,pred.port);
        pred.fd=fd;
        return 0;
    }
    if (strcmp(code_word,"SUCC")==0){
        sscanf(mess,"%s %d %s %s",code_word,&sucsuc.id,sucsuc.ip,sucsuc.port);
        return 0;
    }
    if (strcmp(code_word,"PRED")==0){
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
       
        return 0;
    }
    if (strcmp(code_word,"exit")==0 || strcmp(code_word,"x")==0){
       
        return 0;
    }
    return 0;
}
