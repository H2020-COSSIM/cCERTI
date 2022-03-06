/*
 * ----------------------------------------------------------------------------
 * Synchronization Server. It is used to initiate the HLA Federations.
 * Copyright (c) 2018, H2020 COSSIM.
 * Copyright (c) 2018, Telecommunications Systems Institute.
 * Author: Nikolaos Tampouratzis, ntampouratzis@isc.tuc.gr
 * ----------------------------------------------------------------------------
 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 

#include <signal.h>

#include <iostream>
#include <cstdlib>
#include <deque>

//! Server Functions !//
#define CREATE 0
#define REMOVE 1
#define WRITE 2
#define READ 3
#define READ_GLOBAL 4
#define CLOSE_SERVER 5

//! Additional Server Functions (for Checkpoints) !//
#define CPT_CREATE 6
#define CPT_CREATE_WITH_PATH 7
#define CPT_WRITE 8
#define CPT_READ 9
#define CPT_WRITE_WITH_PATH 10
#define CPT_READ_WITH_PATH 11

#define PATH_LENGTH     100
#define SYS_CLK_LENGTH  20
#define CPU_TYPE_LENGTH 5
#define CMD_LINE_LENGTH 1000

#define MAX_CONNECTIONS 10
#define TCP_PORT 60401
#define CPT_TCP_PORT 60402
#define APOLLON_TCP_PORT 60403

pthread_t tid[3*MAX_CONNECTIONS];

pthread_mutex_t lock1;
pthread_mutex_t lock2;
pthread_mutex_t lock3;

int listenfd  = 0;
int listenfd2 = 0;
int listenfd3 = 0;

typedef struct HLAInitializationRequests{
 int type;
 char name[30];
 int  node;
}HLAInitializationRequest;

typedef struct CPTHLAInitializationRequests{
 int type;
 char name[30];
 int  node;
 char path[PATH_LENGTH];
 char SysClock[SYS_CLK_LENGTH];
 char CpuType[CPU_TYPE_LENGTH];
 char restoreCmd[CMD_LINE_LENGTH];
 bool val;
}CPTHLAInitializationRequest;

typedef struct ServerElements{
  char name[30];
  bool * array;
}ServerElement;

typedef struct CptServerElements{
  char name[30];
  bool * array;
  char ** path;
  char ** SysClock;
  char ** CpuType;
  char ** restoreCmd;
  uint32_t TotalNodes;
}CptServerElement;

std::deque<ServerElement> aDeque;
std::deque<CptServerElement> CptaDeque;

bool ExitVar;

void* HandleFunction(void *arg){
  int *listenfdPtr = (int*)arg;
  int listenfd = *listenfdPtr;
  
  int sum = 0, n;
  bool ret = false;
  while(1)
    {
            
        int connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 
	
	HLAInitializationRequest rqst;
	ServerElement elem;
	n = read(connfd, (void *) &rqst, sizeof(rqst));
	if(n < 0)
	  printf("\n Request error \n");
	
	switch (rqst.type) {
	case CREATE:
	  printf("Server Request: type: CREATE | name: %s | node: %d\n", rqst.name, rqst.node);
	  strcpy(elem.name, rqst.name);
	  elem.array = (bool *) malloc(sizeof(rqst.node));	  
	  for(int i=0;i<rqst.node;i++){
	    elem.array[i] = false;
	  }
	  pthread_mutex_lock(&lock1);
	  aDeque.push_back(elem);
	  pthread_mutex_unlock(&lock1);
	  ret = true;
	  break;
	case REMOVE:
	  printf("Server Request: type: REMOVE | name: %s | node: %d\n", rqst.name, rqst.node);
	  pthread_mutex_lock(&lock1);
	  for(int i=0;i<aDeque.size();i++){
	    elem = aDeque[i];
	    if(strcmp(elem.name,rqst.name)==0){
	      aDeque.erase (aDeque.begin()+i);
	      break;
	    }
	  }
	  pthread_mutex_unlock(&lock1);
	  ret = true;
	  break;
	case WRITE:
	  printf("Server Request: type: WRITE | name: %s | node: %d\n", rqst.name, rqst.node);
	  pthread_mutex_lock(&lock1);
	  for(int i=0;i<aDeque.size();i++){
	    elem = aDeque[i];
	    if(strcmp(elem.name,rqst.name)==0)
	      break;
	  }
	  elem.array[rqst.node] = true;
	  pthread_mutex_unlock(&lock1);
	  ret = true;
	  break;
	case READ:
	  printf("Server Request: type: READ | name: %s | node: %d\n", rqst.name, rqst.node);
	  pthread_mutex_lock(&lock1);
	  for(int i=0;i<aDeque.size();i++){
	    elem = aDeque[i];
	    if(strcmp(elem.name,rqst.name)==0)
	      break;
	  }
	  ret = elem.array[rqst.node];
	  pthread_mutex_unlock(&lock1);
	  break;
	case READ_GLOBAL:
	  printf("Server Request: type: READ_GLOBAL | name: %s | node: %d\n", rqst.name, rqst.node);
	  pthread_mutex_lock(&lock1);
	  for(int i=0;i<aDeque.size();i++){
	    elem = aDeque[i];
	    if(strcmp(elem.name,rqst.name)==0)
	      break;
	  }
	  
	  sum = 0;
	  for(int i = 0;i<rqst.node+1;i++){
	    bool val = elem.array[i];
	    if (val){sum++;}
	  }
	  pthread_mutex_unlock(&lock1);
	  if(sum == (rqst.node))
	    ret = true;
	  else
	    ret = false;
	  break;
	case CLOSE_SERVER:
	  printf("Server Request: type: CLOSE_SERVER \n");
	  ExitVar = true;
	  break;
	default:
	  printf("\n Unknown Request Type \n");
	}

        write(connfd, (const void *) &ret, sizeof(ret)); 	
        close(connfd);
	
     }
  
  return 0;
}

void* CPTHandleFunction(void *arg){
  int *listenfdPtr = (int*)arg;
  int listenfd = *listenfdPtr;
  
  int n;
  bool ret = false;
  char retTotal[PATH_LENGTH+SYS_CLK_LENGTH+CPU_TYPE_LENGTH+CMD_LINE_LENGTH];
  while(1)
    {    
        int connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 
	
	CPTHLAInitializationRequest rqst;
	CptServerElement cpt_elem;
	n = read(connfd, (void *) &rqst, sizeof(rqst));
	if(n < 0)
	  printf("\n CPT Request error \n");
	
	switch (rqst.type) {
	  case CPT_CREATE:
	    printf("Server Request: type: CPT_CREATE | name: %s | node: %d\n", rqst.name, rqst.node);
	    pthread_mutex_lock(&lock2);
	    //! Clean up previous entries with the same name !//
	    for(int i=0;i<CptaDeque.size();i++){
	      cpt_elem = CptaDeque[i];
	      if(strcmp(cpt_elem.name,rqst.name)==0){
		free(cpt_elem.array);
		CptaDeque.erase (CptaDeque.begin()+i);
		break;
	      }
	    }
	    //! Create the new entry !//
	    strcpy(cpt_elem.name, rqst.name);
	    cpt_elem.TotalNodes = rqst.node;
	    cpt_elem.array = (bool *) malloc(rqst.node*sizeof(bool));
	    for(int i=0;i<rqst.node;i++){
	      cpt_elem.array[i] = false;
	    }
	    CptaDeque.push_back(cpt_elem);
	    pthread_mutex_unlock(&lock2);
	    ret = true;
	    break;
	  case CPT_CREATE_WITH_PATH:
	    printf("Server Request: type: CPT_CREATE_WITH_PATH | name: %s | node: %d\n", rqst.name, rqst.node);
	    pthread_mutex_lock(&lock2);
	    //! Clean up previous entries with the same name !//
	    for(int i=0;i<CptaDeque.size();i++){
	      cpt_elem = CptaDeque[i];
	      if(strcmp(cpt_elem.name,rqst.name)==0){
		free(cpt_elem.array);
		for(int j=0;j<cpt_elem.TotalNodes;j++){ free(cpt_elem.path[j]); }
		free(cpt_elem.path);
		for(int j=0;j<cpt_elem.TotalNodes;j++){ free(cpt_elem.SysClock[j]); }
		free(cpt_elem.SysClock);
		for(int j=0;j<cpt_elem.TotalNodes;j++){ free(cpt_elem.CpuType[j]); }
		free(cpt_elem.CpuType);
		for(int j=0;j<cpt_elem.TotalNodes;j++){ free(cpt_elem.restoreCmd[j]); }
		free(cpt_elem.restoreCmd);
		CptaDeque.erase (CptaDeque.begin()+i);
		break;
	      }
	    }
	    //! Create the new entry !//
	    strcpy(cpt_elem.name, rqst.name);
	    cpt_elem.TotalNodes = rqst.node;
	    cpt_elem.array = (bool *) malloc(rqst.node*sizeof(bool));
	    for(int i=0;i<rqst.node;i++){
	      cpt_elem.array[i] = false;
	    }
	    cpt_elem.path = (char **) malloc(rqst.node*sizeof(char *));
	    for(int i=0;i<rqst.node;i++){
	      cpt_elem.path[i] = (char *) malloc(PATH_LENGTH*sizeof(char));
	    }
	    
	    cpt_elem.SysClock = (char **) malloc(rqst.node*sizeof(char *));
	    for(int i=0;i<rqst.node;i++){
	      cpt_elem.SysClock[i] = (char *) malloc(SYS_CLK_LENGTH*sizeof(char));
	    }
	    
	    cpt_elem.CpuType = (char **) malloc(rqst.node*sizeof(char *));
	    for(int i=0;i<rqst.node;i++){
	      cpt_elem.CpuType[i] = (char *) malloc(CPU_TYPE_LENGTH*sizeof(char));
	    }
	    
	    cpt_elem.restoreCmd = (char **) malloc(rqst.node*sizeof(char *));
	    for(int i=0;i<rqst.node;i++){
	      cpt_elem.restoreCmd[i] = (char *) malloc(CMD_LINE_LENGTH*sizeof(char));
	    }
	    
	    CptaDeque.push_back(cpt_elem);
	    pthread_mutex_unlock(&lock2);
	    ret = true;
	    break; 
	    
	    case CPT_WRITE:
	      printf("Server Request: type: CPT_WRITE | name: %s | node: %d | val: %s\n", rqst.name, rqst.node, rqst.val ? "true" : "false");
	      pthread_mutex_lock(&lock2);
	      ret = false;
	      for(int i=0;i<CptaDeque.size();i++){
		cpt_elem = CptaDeque[i];
		if(strcmp(cpt_elem.name,rqst.name)==0){
		  ret = true;
		  break;
		}
	      }
	      if(ret == true)
		cpt_elem.array[rqst.node] = rqst.val;
	      pthread_mutex_unlock(&lock2);
	      break;  
	    case CPT_READ:
	      printf("Server Request: type: CPT_READ | name: %s | node: %d\n", rqst.name, rqst.node);
	      ret = false;
	      pthread_mutex_lock(&lock2);
	      for(int i=0;i<CptaDeque.size();i++){
		cpt_elem = CptaDeque[i];
		if(strcmp(cpt_elem.name,rqst.name)==0){
		  ret = true;
		  break;
		}
	      }
	      if(ret == true)
		ret = cpt_elem.array[rqst.node];
	      pthread_mutex_unlock(&lock2);
	      break;
	    case CPT_WRITE_WITH_PATH:
	      ret = false;
	      if (strcmp(rqst.path, "exitSimulation") == 0)
		printf("Server Request: type: CPT_WRITE_WITH_PATH | name: %s | node: %d | path: %s | val: %d\n", rqst.name, rqst.node, rqst.path, rqst.val);
	      else
		printf("Server Request: type: CPT_WRITE_WITH_PATH | name: %s | node: %d | path: %s | val: %d | SysClock: %s | CpuType: %s | restoreCmd: %s\n", rqst.name, rqst.node, rqst.path, rqst.val, rqst.SysClock, rqst.CpuType, rqst.restoreCmd);
	      pthread_mutex_lock(&lock2);
	      for(int i=0;i<CptaDeque.size();i++){
		cpt_elem = CptaDeque[i];
		if(strcmp(cpt_elem.name,rqst.name)==0){
		  ret = true;
		  break;
		}
	      }
	      if(ret == true){
		cpt_elem.array[rqst.node] = rqst.val;
		memset(cpt_elem.path[rqst.node], '0', sizeof(PATH_LENGTH));
		memset(cpt_elem.SysClock[rqst.node], '0', sizeof(SYS_CLK_LENGTH));
		memset(cpt_elem.CpuType[rqst.node], '0', sizeof(CPU_TYPE_LENGTH));
		memset(cpt_elem.restoreCmd[rqst.node], '0', sizeof(CMD_LINE_LENGTH));
		strcpy(cpt_elem.path[rqst.node], rqst.path);
		strcpy(cpt_elem.SysClock[rqst.node], rqst.SysClock);
		strcpy(cpt_elem.CpuType[rqst.node], rqst.CpuType);
		strcpy(cpt_elem.restoreCmd[rqst.node], rqst.restoreCmd);
	      }
	      pthread_mutex_unlock(&lock2);
	      ret = true;
	      break;  
	    case CPT_READ_WITH_PATH:
	      printf("Server Request: type: CPT_READ_WITH_PATH | name: %s | node: %d\n", rqst.name, rqst.node);
	      ret = false;
	      memset(retTotal, '0', sizeof(retTotal));
	      pthread_mutex_lock(&lock2);
	      for(int i=0;i<CptaDeque.size();i++){
		cpt_elem = CptaDeque[i];
		if(strcmp(cpt_elem.name,rqst.name)==0){
		  ret = true;
		  break;
		}
	      }
	      if(ret == true){
		if(rqst.node>=cpt_elem.TotalNodes){
		  printf("\nERROR: COSSIM Instantiate %d HLA Enable Nodes, while SLB wants to Instantiate %d NodeNum (Only (0-%d) NodeNum are permitted)\n",cpt_elem.TotalNodes, rqst.node, cpt_elem.TotalNodes-1);
		  exit(-1);
		}
		ret = cpt_elem.array[rqst.node];

		strcpy(retTotal,cpt_elem.path[rqst.node]); 
		strcat(retTotal,"&");
		strcat(retTotal,cpt_elem.SysClock[rqst.node]); 
		strcat(retTotal,"&");
		strcat(retTotal,cpt_elem.CpuType[rqst.node]); 
		strcat(retTotal,"&");
		strcat(retTotal,cpt_elem.restoreCmd[rqst.node]); 
	      }
	      pthread_mutex_unlock(&lock2);
	      break;
	    default:
	      printf("\n Unknown Request Type \n");
	}
	
	write(connfd, (const void *) &ret, sizeof(ret)); 
	//! In case of CPT_READ_WITH_PATH command and return value is true, then it send the path of required memory !//
	if ((rqst.type == CPT_READ_WITH_PATH)&&(ret == true)){
	  write(connfd, retTotal, strlen(retTotal)); 
	}
	close(connfd);

     }
  
  return 0;
}


void* ApollonHandleFunction(void *arg){
  int *listenfdPtr = (int*)arg;
  int listenfd = *listenfdPtr;
  
  int n;
  bool ret = false;
  while(1)
    {
            
        int connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 
	
	HLAInitializationRequest rqst;
        char rqst_char[128] = {0};
        char resp_char[128] = {0};
	ServerElement elem;
	n = read(connfd, (void *) &rqst_char, sizeof(rqst_char));
	if(n < 0)
	  printf("\n Request error \n");
	
        /* Split the request string into struct format */
        std::string rqst_str(rqst_char);
        std::string delimiter = "|";
        
        size_t pos = rqst_str.find(delimiter);
        std::string rqst_type = rqst_str.substr(0, pos);
        rqst_str.erase(0, pos + delimiter.length());
        
        pos = rqst_str.find(delimiter);
        std::string rqst_name = rqst_str.substr(0, pos);
        rqst_str.erase(0, pos + delimiter.length());
        std::string rqst_node = rqst_str;
        
        strcpy(rqst.name, rqst_name.c_str());
        rqst.node = std::stoi(rqst_node);
        /* END Split the request string into struct format */
	
        if(rqst_type == "CREATE"){
	  printf("Server Request: type: CREATE | name: %s | node: %d\n", rqst.name, rqst.node);
	  strcpy(elem.name, rqst.name);
	  elem.array = (bool *) malloc(sizeof(rqst.node));	  
	  for(int i=0;i<rqst.node;i++){
	    elem.array[i] = false;
	  }
	  pthread_mutex_lock(&lock3);
	  aDeque.push_back(elem);
	  pthread_mutex_unlock(&lock3);
	  ret = true;
        }
	else if(rqst_type == "REMOVE"){
	  printf("Server Request: type: REMOVE | name: %s | node: %d\n", rqst.name, rqst.node);
	  pthread_mutex_lock(&lock3);
	  for(int i=0;i<aDeque.size();i++){
	    elem = aDeque[i];
	    if(strcmp(elem.name,rqst.name)==0){
	      aDeque.erase (aDeque.begin()+i);
	      break;
	    }
	  }
	  pthread_mutex_unlock(&lock3);
	  ret = true;
        }
	else if(rqst_type == "WRITE"){
	  printf("Server Request: type: WRITE | name: %s | node: %d\n", rqst.name, rqst.node);
	  pthread_mutex_lock(&lock3);
	  for(int i=0;i<aDeque.size();i++){
	    elem = aDeque[i];
	    if(strcmp(elem.name,rqst.name)==0)
	      break;
	  }
	  elem.array[rqst.node] = true;
	  pthread_mutex_unlock(&lock3);
	  ret = true;
        }
	else if(rqst_type == "READ"){
	  printf("Server Request: type: READ | name: %s | node: %d\n", rqst.name, rqst.node);
	  pthread_mutex_lock(&lock3);
	  for(int i=0;i<aDeque.size();i++){
	    elem = aDeque[i];
	    if(strcmp(elem.name,rqst.name)==0)
	      break;
	  }
	  ret = elem.array[rqst.node];
	  pthread_mutex_unlock(&lock3);
        }
        else if(rqst_type == "CLOSE_SERVER"){
	  printf("Server Request: type: CLOSE_SERVER \n");
	  ExitVar = true;
        }
        else{
	  printf("\n Unknown Request Type \n");
	}
	
	if(ret)
            write(connfd, "true" , sizeof("true")); 
        else
            write(connfd, "false" , sizeof("false")); 
        	
        close(connfd);
	
     }
  
  return 0;
}

void intHandler(int dummy) {
    int truee = 1;
    setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&truee,sizeof(int));
    setsockopt(listenfd2,SOL_SOCKET,SO_REUSEADDR,&truee,sizeof(int));
    setsockopt(listenfd3,SOL_SOCKET,SO_REUSEADDR,&truee,sizeof(int));
}


int main(int argc, char *argv[])
{
    ExitVar = false;
    
    struct sockaddr_in serv_addr; 

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(TCP_PORT); 

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

    listen(listenfd, MAX_CONNECTIONS); 
    
    signal(SIGALRM, intHandler);
    
    printf("\n SynchServer up and running (with %d MAX Connections) ...\n\n",MAX_CONNECTIONS);
    
    if (pthread_mutex_init(&lock1, NULL) != 0){
        printf("\n mutex1 init failed\n");
        return 1;
    }
        
    for(int i=0;i<MAX_CONNECTIONS;i++){
      int err = pthread_create(&(tid[i]), NULL, &HandleFunction, &listenfd);
      if (err != 0)
	printf("\ncan't create thread :[%s]", strerror(err)); 
    }
    
    
    
    /* CPT Server Implementation */
    struct sockaddr_in serv_addr2; 

    listenfd2 = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr2, '0', sizeof(serv_addr2));

    serv_addr2.sin_family = AF_INET;
    serv_addr2.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr2.sin_port = htons(CPT_TCP_PORT); 

    bind(listenfd2, (struct sockaddr*)&serv_addr2, sizeof(serv_addr2)); 

    listen(listenfd2, MAX_CONNECTIONS); 
    
    if (pthread_mutex_init(&lock2, NULL) != 0){
        printf("\n mutex2 init failed\n");
        return 1;
    }
        
    for(int i=MAX_CONNECTIONS;i<2*MAX_CONNECTIONS;i++){
      int err = pthread_create(&(tid[i]), NULL, &CPTHandleFunction, &listenfd2);
      if (err != 0)
	printf("\nCPT can't create thread :[%s]", strerror(err)); 
    }
    /* END CPT Server Implementation */
    
    
    /* Apollon Server Implementation */
    struct sockaddr_in serv_addr3; 

    listenfd3 = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr3, '0', sizeof(serv_addr3));

    serv_addr3.sin_family = AF_INET;
    serv_addr3.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr3.sin_port = htons(APOLLON_TCP_PORT); 

    bind(listenfd3, (struct sockaddr*)&serv_addr3, sizeof(serv_addr3)); 

    listen(listenfd3, MAX_CONNECTIONS); 
    
    if (pthread_mutex_init(&lock3, NULL) != 0){
        printf("\n mutex3 init failed\n");
        return 1;
    }
        
    for(int i=2*MAX_CONNECTIONS;i<3*MAX_CONNECTIONS;i++){
      int err = pthread_create(&(tid[i]), NULL, &ApollonHandleFunction, &listenfd3);
      if (err != 0)
	printf("\nApollon can't create thread :[%s]", strerror(err)); 
    }
    /* END Apollon Server Implementation */
    

    while(1){
      if(ExitVar) {break;}
     sleep(1); 
    }
    
    pthread_mutex_destroy(&lock1);
    pthread_mutex_destroy(&lock2);
    pthread_mutex_destroy(&lock3);
    
    
}
