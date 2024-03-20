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
			trash[strlen(trash)+1]='\0';
			trash[strlen(trash)]='\n';
            n=write(pred.fd,trash,strlen(trash) ); //REPLAY ENTRY MSG TO MY PRED
            if(n==-1)/*error*/ exit(1);
           
            pred.fd=fd;
            if (sucsuc.id != mid ) { //repara que o meu sucsuc so da update quando eu receber o pred portanto por agora sou eu se eu estivesse sozinho com outro node
              removeNodeCol(pred.id);   
            }
            
            sscanf(mess,"%s %d %s %s",code_word,&pred.id,pred.ip,pred.port);
            
        }
        
        if (succ.id==-1){ //IF ALONE, NEW SUC IS NEW PRED and I TCP conect to his server as a client
            succ.id=pred.id;
            strcpy(succ.ip,pred.ip);
            strcpy(succ.port,pred.port);
            succ.fd = tcp_client(succ.ip, succ.port);
            sprintf(trashp,"PRED %02d\n",mid);
            n=write(succ.fd,trashp,strlen(trashp));
            if(n==-1)/*error*/ exit(1);
        }
        sprintf(trash,"SUCC %02d %s %s\n",succ.id,succ.ip,succ.port); // I TELL NEW GUY WHO IS HIS SUCSUC
        
        n=write(fd,trash,strlen(trash));
        if(n==-1)/*error*/ exit(1);
        routall(pred.fd);

        return 0;
    }
    if (strcmp(code_word,"PRED")==0){ //SOMEONE TOLD ME THEY ARE MY PRED
		if(pred.id != -1){
			close(pred.fd);
			removeNodeCol(pred.id);
		}
        sscanf(mess,"%s %d",code_word,&pred.id);
        pred.fd=fd;
        if (sucsuc.fd != -1){
             sprintf(trash,"SUCC %02d %s %s\n",succ.id,succ.ip,succ.port); // I TELL my new pred his new sucsuc
        
            n=write(fd,trash,strlen(trash));
            if(n==-1)/*error*/ exit(1);

        }
		routall(pred.fd);
        return 0;
    }
    if (strcmp(code_word,"CHORD")==0){
        printf("tie the knot well");
		//routall missing
        return 0;
    }
    if(strcmp(code_word,"ROUTE")==0){
       int dst,org,i=0,j=0,sscan=0;
        char caminho[500];
        char *token;
        struct Path new_path;
        
        sscan=sscanf(mess,"%s %d %d %s",code_word,&org,&dst,caminho);
        if (sscan==4){
            token=strtok(caminho,"-");
            if (token==NULL){
                new_path.route[0]=((caminho[0]-'0')*10)+(caminho[1]-'0');
            }else{
                while (token!=NULL){
                    new_path.route[i]=((token[0]-'0')*10)+(token[1]-'0');
                    if (new_path.route[i] == mid){
						if(dst != mid){
							new_path.size = 0;	
							new_path.route[0] = org;
							new_path.route[1] = dst;
							updateRT(new_path);
							printf("Caminho ciclico\n");
						}
                        return 0;
                    } 
                    token=strtok(NULL,"-");
                    i++;
                }
            }
            new_path.size=i;
        }else if (sscan==3){
            new_path.size=0;
            new_path.route[0]=org;
            new_path.route[1]=dst;
        }
        updateRT(new_path);
        return 0;
    }
    return 1;
}

int what_clit(int fd, char *mess){
    char code_word[100],trash[500],pred_mess[500];
    int n;
    strcpy(trash,mess);
    sscanf(mess,"%s",code_word);
    if (strcmp(code_word,"ENTRY")==0){ //NEW GUY IN FRONT OF ME
        sucsuc.fd = succ.fd;
        sucsuc.id = succ.id;
        strcpy(sucsuc.ip, succ.ip);
        strcpy(sucsuc.port, succ.port);

        sscanf(mess,"%s %d %s %s",code_word,&succ.id,succ.ip,succ.port);
        close(succ.fd);
        
        succ.fd = tcp_client(succ.ip, succ.port);
        if (pred.id != sucsuc.id) {
            removeNodeCol(succ.id);
        }
        

        sprintf(pred_mess,"PRED %d\n",mid);
        n=write(succ.fd,pred_mess,strlen(pred_mess));//I TELL NEW GUY IM BEHIND HIM
        if(n==-1)/*error*/ exit(1);
        sprintf(trash,"SUCC %d %s %s\n",succ.id,succ.ip,succ.port); //aviso o meu pred do seu novo SUCSUC
        n=write(pred.fd,trash,sizeof(trash));
        if(n==-1)/*error*/ exit(1);

		routall(succ.fd);
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
    if(strcmp(code_word,"ROUTE")==0 ){
        int dst,org,i=0,j=0,sscan=0;
        char caminho[500],t[2]="-";
        char *token;
        struct Path new_path;
        
        sscan=sscanf(mess,"%s %d %d %s",code_word,&org,&dst,caminho);
        if (sscan==4){
            token=strtok(caminho,t);
            if (token==NULL){
                new_path.route[0]=((caminho[0]-'0')*10)+(caminho[1]-'0');
            }else{
                while (token!=NULL){
                    new_path.route[i]=((token[0]-'0')*10)+(token[1]-'0');
                    if (new_path.route[i] == mid){
						if(dst != mid){
							new_path.size = 0;
							new_path.route[0] = org;
							new_path.route[1] = dst;
							updateRT(new_path);
							printf("Caminho ciclico\n");
						}
                        return 0;
                    } 
                    token=strtok(NULL,t);
                    i++;
                }
            }
            new_path.size=i;
        }else if (sscan==3){
            new_path.size=0;
            new_path.route[0]=org;
            new_path.route[1]=dst;
        }
        updateRT(new_path);
        return 0;
    }
    
    return 1;
}

int what_std(char *std_in,struct addrinfo *res){
    char code_word[100],succIP[100],succTCP[100],show[100];
    int ring,id,succid,  n;
    sscanf(std_in,"%s",code_word);
    if (strcmp(code_word,"join")==0 || strcmp(code_word,"j")==0){
        sscanf(std_in,"%s %d %d",code_word,&ring,&id);
        mid=join(ring,id,res);
		if(succ.id > 0){
			sprintf(code_word, "ROUTE %d %d %d\n", mid, mid, mid);
			n = write(succ.fd, code_word, strlen(code_word));
			if(n < 0) exit(-1);
		}
        return 0;
    }
    if (strcmp(code_word,"direct")==0 || strcmp(code_word,"dj")==0){
        sscanf(std_in,"%s %d %d %s %s",code_word,&id,&succid,succIP,succTCP);
		mid = id;
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
            printf("\tId: %d\n",succ.id);
            printf("\tIP-%s\n",succ.ip);
            printf("\tPort-%s\n",succ.port);
        }
        if (sucsuc.id==-1){
            printf("Segundo sucessor ainda não definido\n");
        }else{
            printf("Segundo sucessor:\n");//conmo vou ter id dos outros 
            printf("\tId: %d\n",sucsuc.id);
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
       leave(mid, res);
        return 0;
    }
    if (strcmp(code_word,"exit")==0 || strcmp(code_word,"x")==0){
       
        return 0;
    }
	if (strcmp(code_word,"print")==0 || strcmp(code_word,"p")==0){
       	prtRoute();
        return 0;
    }
    return 0;
}

