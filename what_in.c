//Este ficheiro serve para funçoes de identificação de comandos 
#include "header.h"
#include "tp.h"
extern int errcode,mid,n;
extern char *mIP, *mTCP;
extern struct node succ, sucsuc, pred,my_chord;

int what_serv(int fd, char *mess){ //TCP SERVER SIDE
    char code_word[100],trash[500],trashp[500],carta[500];
    char *ret;
    int n,org,dst,i,new_chordid, p_; 
    struct node *pp;
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
			strcat(trash, "\n");
            n=write(pred.fd,trash,strlen(trash) ); //REPLAY ENTRY MSG TO MY PRED
            if(n==-1)/*error*/ exit(1);
           	
			//Delay mb here??
			p_ = pred.id;
			pred.id = -1;
			close(pred.fd);
            if (succ.id != p_ ) { //Se succ.id == pred.id eu não perco efetivamente uma adjacencia
              removeCol(p_);   
            }
            
            sscanf(mess,"%s %d %s %s",code_word,&pred.id,pred.ip,pred.port);
            pred.fd=fd;
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
			p_ = pred.id;
			pred.id = -1;
			close(pred.fd);
			removeCol(p_);
		}
        sscanf(mess,"%s %d",code_word,&pred.id);
        pred.fd=fd;
        if (sucsuc.fd != -1){
            sprintf(trash,"SUCC %02d %s %s\n",succ.id,succ.ip,succ.port); // I TELL my new pred his new sucsuc
        
            n=write(fd,trash,strlen(trash));
            if(n==-1)/*error*/ exit(1);
        }
		else sucsuc.fd = 1;
		if(succ.id != pred.id)
			routall(pred.fd);
        return 0;
    }
    if (strcmp(code_word,"CHORD")==0){
        n=sscanf(mess,"%s %d",code_word,&new_chordid);
        if (n!=2){
            return 1;
        }
		pp = (struct node *)malloc(sizeof(struct node));
		if(pp == NULL)/*no memory*/exit(5);

		pp->id = new_chordid;
		pp->fd = fd;
        Fire_Link = insertUnsortedLinkedList(Fire_Link, (Item) pp);
        routall(pp->fd);
		return 0;
    }
    if(strcmp(code_word,"ROUTE")==0 ){
        int dst,org,i=0,sscan=0;
        char caminho[500],t[2]="-";
        char *token;
        path new_path;
        
        sscan=sscanf(mess,"%s %d %d %s",code_word,&org,&dst,caminho);
        if (sscan==4){
            token=strtok(caminho,t);
            if (token==NULL){
                new_path.route[0]=atoi(caminho);
				i++;
            }else{
                while (token!=NULL){
                    new_path.route[i]=atoi(token);
                    if (new_path.route[i] == mid){
						if(dst != mid){
							new_path.size = 0;
							new_path.route[0] = org;
							new_path.route[1] = dst;
							updateRT(new_path);
                            /*if (dst != pred.id && dst != succ.id){
                                adj_route(new_path);
                            }*/
							
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
    if (strcmp(code_word,"CHAT")==0 ){
        sscanf(mess,"%s %d %d %s",code_word,&org,&dst,carta);
		
        ret=strchr(mess,' ');
        if (ret==NULL){
                return 1;
        }
        ret++;
        for(i=0;i<2;i++){
            ret=strchr(ret,' ');
            if (ret==NULL){
                return 1;
            }
            ret++;
            strcpy(carta,ret);
        }
        if(dst==mid){
            printf("Mensagem de chat recebida do nó %02d-%s\n",org,carta);
            return 0;
        }
		
		int send = findFd(ETable[dst]);
        if (send==-1){
            printf("Não consegui encontrar caminho\n");
        }else{
            n=ctt(org,dst,send,carta);
        }
        
        return 0;
    }
    return 1;
}

int what_clit(int fd, char *mess){
    char code_word[100],trash[500],pred_mess[500],carta[500];
    char *ret;
    int n,org,dst,i, p_;
    strcpy(trash,mess);
    sscanf(mess,"%s",code_word);
    if (strcmp(code_word,"ENTRY")==0){ //NEW GUY IN FRONT OF ME
        sucsuc.id = succ.id;
        strcpy(sucsuc.ip, succ.ip);
        strcpy(sucsuc.port, succ.port);
		
		p_ = succ.id;
		succ.id =-1;
        close(succ.fd);
       
        if (pred.id != p_) {
            removeCol(p_);
        }
        
        sscanf(mess,"%s %d %s %s",code_word,&succ.id,succ.ip,succ.port);
        succ.fd = tcp_client(succ.ip, succ.port);

        sprintf(pred_mess,"PRED %d\n",mid);
        n=write(succ.fd,pred_mess,strlen(pred_mess));//I TELL NEW GUY IM BEHIND HIM
        if(n==-1)/*error*/ exit(1);
        sprintf(trash,"SUCC %d %s %s\n",succ.id,succ.ip,succ.port); //aviso o meu pred do seu novo SUCSUC
        n=write(pred.fd,trash,strlen(trash));
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
        int dst,org,i=0,sscan=0;
        char caminho[500],t[2]="-";
        char *token;
        path new_path;
        
        sscan=sscanf(mess,"%s %d %d %s",code_word,&org,&dst,caminho);
        if (sscan==4){
            token=strtok(caminho,t);
            if (token==NULL){
                new_path.route[0]=atoi(caminho);
				i++;
            }else{
                while (token!=NULL){
                    new_path.route[i]=atoi(token);
                    if (new_path.route[i] == mid){
						if(dst != mid){
							new_path.size = 0;
							new_path.route[0] = org;
							new_path.route[1] = dst;
							updateRT(new_path);
                            /*if (dst != pred.id && dst != succ.id){
                                adj_route(new_path);
                            }*/
							
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
    if (strcmp(code_word,"CHAT")==0 ){
        sscanf(mess,"%s %d %d %s",code_word,&org,&dst,carta);
		
        ret=strchr(mess,' ');
        if (ret==NULL){
                return 1;
            }
        ret++;
        for(i=0;i<2;i++){
            ret=strchr(ret,' ');
            if (ret==NULL){
                return 1;
            }
            ret++;
            strcpy(carta,ret);
        }
        if(dst==mid){
            printf("Mensagem de chat recebida de %02d-%s\n",org,carta);
            return 0;
        }
		int send = findFd(ETable[dst]);
        if (send==-1){
            printf("Não consegui encontrar caminho\n");
        }else{
            n=ctt(org,dst,send,carta);
        }

        return 0;
    }
    return 1;
}

int what_std(char *std_in,struct addrinfo *res){
    char code_word[100],succIP[100],succTCP[100],show[100],chat[500];
    char *ret;
    int ring,id,succid,n,dst,i,p_;
    struct node new_chord;
    
    sscanf(std_in,"%s",code_word);
    if (strcmp(code_word,"join")==0 || strcmp(code_word,"j")==0){
        
        n = sscanf(std_in,"%s %d %d",code_word,&ring,&id);
        if (n!= 3) {  
            printf("Sintax Errada \n");
            return 2;
        }
        if (ring < 000 || ring > 999) {
           printf("Ring Inválido \n");
           return 2; 
        }
        if (id < 0 || id > 99) {
           printf("ID Inválido \n");
           return 2; 
        }
        mid=join(ring,id,res);	
        return 0;
    }
    if (sscanf(std_in, "%s %s", code_word, show) == 2){
        if(strcmp(code_word, "dj") == 0){
            n = sscanf(std_in,"%s %d %d %s %s",code_word,&id,&succid,succIP,succTCP);
            if (n!= 5) {  
                printf("Sintax Errada \n");
                return 2;}
        }else if ((strcmp(code_word, "direct") == 0) && strcmp(show, "join") == 0){
             n = sscanf(std_in,"%s %s %d %d %s %s",code_word, show, &id,&succid,succIP,succTCP);
            if (n!= 6) {  
                printf("Sintax Errada \n");
                return 2;}
        }
        if (id < 000 || ring > 999) {
        printf("Ring Inválido \n");
        return 2; 
        }
        
        if (id < 0 || id > 99) {
        printf("ID Inválido \n");
        return 2; 
        }
        mid = id;
        d_join(id,succid,succIP,succTCP);
        return 0;
    }
    if (strcmp(code_word,"chord")==0 || strcmp(code_word,"c")==0){
        n=sscanf(std_in,"%s %d",code_word,&id);
        if ((n=!2)){
            printf("Comando para criar corda com formato errado\n");
            return 1;
        }
        if (mid==id){
            printf("Não é possivel criar corda para mim próprio\n");
        }
        if(id==succ.id && succ.id>0){
            printf("Não é possivel criar corda para o meu sucessor\n");
            return 1;
        }
        if(id==pred.id && succ.id>0){
            printf("Não é possivel criar corda com o meu predecessor\n");
            return 1;
        }
        if(id==my_chord.id && my_chord.id>0){
            printf("Já possui uma corda, remova antes\n");
            return 1;
        }

        n=check_serv(res,id,new_chord);
        if(n==1){
            return 1;
        }
        my_chord.fd=tcp_client(my_chord.ip,my_chord.port);
        sprintf(code_word,"CHORD %d\n",mid);
        n=write(my_chord.fd,code_word,strlen(code_word));
        if(n < 0) exit(-1);
		routall(my_chord.fd);

        return 0;
    }
    if (strcmp(code_word,"remove")==0 || strcmp(code_word,"rc")==0){
        n = sscanf(std_in,"%s %d",code_word,&id);
        if (n!= 3) {  
            printf("Sintax Errada \n");
            return 2;
        }
        if (id==my_chord.id){
            p_=my_chord.id;
            my_chord.id=-1;
            close(my_chord.fd);
			n = findFd(p_);
            if ( n == -1) {     
            	removeCol(p_); }
            printf("Corda com o nó %d removida\n",p_);
            return 0;   
        }
        printf("Não existe uma corda com o nó %d",id);
        return 1;
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
        if (my_chord.id==-1){
            printf("Corda ainda não definida\n");
        }else{
            printf("Corda:\n");//conmo vou ter id dos outros 
            printf("\tId: %d\n",my_chord.id);
            printf("\tIP-%s\n",my_chord.ip);
            printf("\tPort-%s\n",my_chord.port);
        //falta corda
        
        }
        return 0;
    }
    
    if (((sscanf(std_in,"%s %s",code_word,show)==2)&&(strcmp(show,"routing")==0))||(strcmp(code_word,"sr")==0)){
        printRT();
        fflush(stdout);
        return 0;
    }
    if (((sscanf(std_in,"%s %s",code_word,show)==2)&&(strcmp(show,"path")==0))||(strcmp(code_word,"sp")==0)){
        printSP();
        fflush(stdout);
        return 0;
    }
    if (((sscanf(std_in,"%s %s",code_word,show)==2)&&(strcmp(show,"fowarding")==0))||(strcmp(code_word,"sf")==0)){
        printET();
        fflush(stdout);
        return 0;
    }
    if (strcmp(code_word,"message")==0 || strcmp(code_word,"m")==0){
        if (succ.id<0){
            printf("Não existe nó de destino possivel para enviar a mensagem\n");
        }
        n=sscanf(std_in,"%s %d %s ",code_word,&dst,chat);
        if(n!=3){
            return 2;
        }
        if (dst==mid){
            printf("Mensagem da minha conciência: %s",chat);
            return 0;
        }
        
        ret=strchr(std_in,' ');
        ret++;
        for(i=0;i<1;i++){
            ret=strchr(ret,' ');
            if (ret==NULL){
                return 1;
            }
            ret++;
            strcpy(chat,ret);
        }

		int send = findFd(ETable[dst]);
        if (send==-1){
            printf("Não consegui encontrar caminho\n");
        }else{
            n=ctt(mid,dst,send,chat);
        }
        

        return 0;
    }
    if (strcmp(code_word,"leave")==0 || strcmp(code_word,"l")==0){
       leave(mid, res);
        return 0;
    }
    if (strcmp(code_word,"exit")==0 || strcmp(code_word,"x")==0){
        if(mid>0){
            leave(mid,res);
        }
        w=0;
        return 0;
    }
    if (strcmp(code_word,"help")==0){
        printf("Comandos disponiveis:\n \t join (j) ring id\n \t direct join (dj) id succid succIP succTCP\n \t chord (c) i\n \t remove chord (rc)\n \t show topology (st)\n \t show routing (sr) dest\n \t show path (sp) dest\n \t show forwarding (sf)\n \t message (m) dest message\n \t leave (l)\n \t exit (x) \n");
        return 0;
    }
	if (strcmp(code_word,"print")==0 || strcmp(code_word,"p")==0){
       	prtRoute();
        return 0;
    }
    return 2;
}

int what_noose(int fd, char *mess){  //Receive stuff in CHORD
    char code_word[100],trash[500],carta[500];
    char *ret;
    int org,dst,i; 
    strcpy(trash,mess);
    sscanf(mess,"%s",code_word);
  
    if(strcmp(code_word,"ROUTE")==0 ){
        int dst,org,i=0,sscan=0;
        char caminho[500],t[2]="-";
        char *token;
        path new_path;
        
        sscan=sscanf(mess,"%s %d %d %s",code_word,&org,&dst,caminho);
        if (sscan==4){
            token=strtok(caminho,t);
            if (token==NULL){
                new_path.route[0]=atoi(caminho);
				i++;
            }else{
                while (token!=NULL){
                    new_path.route[i]=atoi(token);
                    if (new_path.route[i] == mid){
						if(dst != mid){
							new_path.size = 0;
							new_path.route[0] = org;
							new_path.route[1] = dst;
							updateRT(new_path);
                            /*if (dst != pred.id && dst != succ.id){
                                adj_route(new_path);
                            }*/
							
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
    if (strcmp(code_word,"CHAT")==0 ){
        sscanf(mess,"%s %d %d %s",code_word,&org,&dst,carta);
		puts(mess);
        ret=strchr(mess,' ');
        if (ret==NULL){
                return 1;
        }
        ret++;
        for(i=0;i<2;i++){
            ret=strchr(ret,' ');
            if (ret==NULL){
                return 1;
            }
            ret++;
            strcpy(carta,ret);
        }
        if(dst==mid){
            printf("Mensagem de chat recebida do nó %02d-%s\n",org,carta);
            return 0;
        }
		
		int send = findFd(ETable[dst]);
        if (send==-1){
            printf("Não consegui encontrar caminho\n");
        }else{
            i=ctt(org,dst,send,carta);
        }
        
        return 0;
    }
    return 1;
}



