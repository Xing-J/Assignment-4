//CP386 Assignment 4
//Xing Jia 180164170
//Github URL: https://github.com/Xing-J/Assignment-4

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <time.h>
#include <semaphore.h>

void *thread_run(void *thread);
void Run(int seq[]);
int comp(int one[],int two[]);
void try_all(int order,int q[],int Available[]);

typedef struct Customer{
    int C_ID;
    int Allocation[4];
    int max[4];
    int Need[4];
}Customer;
Customer *customers;
int matrix[5][4]={{6,4,7,3},{4,2,3,2},{2,5,3,3},{6,3,3,2},{5,5,7,5}};
int Available[4];
void request(int order,int q[],int Available[],int safe_sequence[]);
int safty_check(int Available[],int safe_sequence[]);
int release(int n,int rel[],int Available[]);
int status(int Available[]);
int main(int argc, char *argv[]){
	
    printf("Number of Customers: 5\n");
	int safe_sequence[5];
	Customer* current_customers = (Customer*) malloc(sizeof(Customer)*5);
    	customers=current_customers;
	for (int c=0;c<5;c++){
		current_customers[c].C_ID=c;
		for(int r=0;r<4;r++){
			current_customers[c].Allocation[r]=0;
			current_customers[c].max[r]=matrix[c][r];
			current_customers[c].Need[r]=matrix[c][r];
		}
		
	}
	
	int p =1;
	int k=0;
	while(argv[p]!=NULL){
		k = atoi(argv[p]);
		Available[p-1]=k;
		p++;
	}
	printf("Currently Available resources: ");
	for(k=0;k<4;k++){
		printf("%d ",Available[k]);
	}
	printf("\n");
	printf("Maximum resources from file:\n");
	for(int k =0;k<5;k++){
		for(int i = 0;i<4;i++){
			printf("%d ",customers[k].max[i]);
		}
		printf("\n");
	}

	if (safty_check(Available,safe_sequence)==0){
		printf("Error: not safe\n");
		return;
	}else{
		char* command = malloc(sizeof(char*)*300);
		while(1){
			int input[4];
			int thread_id;
			printf("Enter command:\n");
			fgets(command, 300, stdin);
			char* token = strtok(command," ");
			int i =0;
			int mode =0;
			while (token!=NULL){
				if (i==0){
					if(strcmp("RQ",token)==0){
						mode =1;
					}else if (strcmp("RL",token)==0){
						mode = 2;
					}else if (strcmp("Status\n",token)==0){
						mode=3;
					}else if (strcmp("Run\n",token)==0){
						mode= 4;
					}else if (strcmp("Exit\n",token)==0){
						mode =5;
					}else{
						printf("ERROR: PLEASE INPUT RIGHT MODE COMMAND\n");
						return;
					}
					
				}
				else if (i==1){
					thread_id = atoi(token);
					
				}else{
					input[i-2]=atoi(token);
				
				}
				token = strtok(NULL," ");
				i++;
			}
			switch(mode){
				case(1):
					request(thread_id,input,Available,safe_sequence);
					break;
				case(2):
					release(thread_id,input,Available);
					break;
				case(3):
					status(Available);
					break;
				case(4):
					if(safty_check(Available,safe_sequence)==1){
						Run(safe_sequence);
						for(int x =0;x<5;x++){
							pthread_t c_thread;
							pthread_attr_t n_thread;
							pthread_attr_init(&n_thread);

							c_thread = pthread_create(&c_thread,&n_thread,thread_run,&customers[safe_sequence[x]].C_ID);
							sleep(2);
							if(c_thread !=0){
								printf("Thread Failed");
							}
						}pthread_exit(NULL);
					}else{
						printf("Current thread list is not safe, can not perform run threads\n");
						return;
					}
					break;
				case(5):
					printf("Exiting");
					exit(0);
					break;
				default:
					printf("Error, Command is not correct");
					break;


			}		
			
		}
			
			
			
	}
}

int safty_check(int Available[],int safe_sequence[]){
    int temp[4];
    int k = 0;
    while(k<4){
         temp[k] = Available[k];
         k++;
    }

    int o=0;

    int is_safe[5]={0,0,0,0,0}; 
    for(int m=0;m<5;m++){
        if(is_safe[m]==0 && customers[m].Need[0] <= temp[0] && customers[m].Need[1] <= temp[1] && customers[m].Need[2] <= temp[2] && customers[m].Need[3] <= temp[3]){
            temp[0] += customers[m].Allocation[0];
            temp[1] += customers[m].Allocation[1];
            temp[2] += customers[m].Allocation[2];
            temp[3] += customers[m].Allocation[3];
            is_safe[m] = 1;
            safe_sequence[o++] = m;
            m=-1;
        }
    }
	for (int i =0;i<5;i++){
		if(is_safe[i]==0){
			return 0;
		}
	}
    
    return 1;
}
void request(int order,int q[],int Available[],int safe_sequence[]){
	
    if(comp(q,customers[order].Need)==0){
        printf("requests fail\n");
        return 0;
    }
    else if(comp(q,Available)==1){
        try_all(order,q,Available);
        if(safty_check(Available,safe_sequence)==0){
            for(int r=0;r<4;r++){
                 Available[r] = Available[r] + q[r];
                 customers[order].Allocation[r] -= q[r];
                 customers[order].Need[r] += q[r];
            }
            printf("not ok\n");
        }
        else{
            printf("State is safe, and request is satisfied\n");
        }
    }
}
void try_all(int order,int q[],int Available[]){
    for(int r=0;r<4;r++){
        Available[r] = Available[r] - q[r];
        customers[order].Allocation[r] += q[r];
        customers[order].Need[r] -= q[r];
    }
}
int comp(int one[],int two[]){
    int i = 0;
    while(i < 4){
		if(i<4 || one[i]>two[i]){
			break;
        	return 0;
			
    	}
          i++;
    }
    
         return 1;
}
int release(int n,int rel[],int Available[]){
	if(comp(rel,customers[n].Allocation)==1){
		for(int i = 0;i<4;i++){
			customers[n].Allocation[i] -= rel[i];
			Available[i] += rel[i];
		}	
			printf("The resources have been released successfully\n");
		
	}
	else{
		printf("invalid release");
	}
}
int status(int Available[]){
    printf("Available Resources:\n");
    int w = 0;
    while(w<4){
        printf("%d ",Available[w]);
        w++;
    }
    printf("\n");
    printf("Maximum Resources:\n");
    for(int w =0;w<5;w++){
        for(int i=0;i<4;i++){
            printf("%d ",customers[w].max[i]);

        }
        printf("\n");
    }
    printf("\n");
    printf("Allocated Resources:\n");
    for(int k =0;k<5;k++){
        for(int i=0;i<4;i++){
            printf("%d ",customers[k].Allocation[i]);

        }
        printf("\n");
    }
    printf("\n");
    printf("Need Resources:\n");
    for(int k =0;k<5;k++){
        for(int i=0;i<4;i++){
            printf("%d ",customers[k].Need[i]);

        }
        printf("\n");
    }
    printf("\n");
}
void Run(int seq[]){

    printf("Safe Sequence is:");
    for(int i =0;i<5;i++){
        printf("%d ",seq[i]);

    }
    printf("\n");

}

void thread_Start(int *threadID) {
	printf("        Thread has started\n");
	return;
}
void thread_Finish(int *threadID) {
	printf("        Thread has finished\n");
	return;
}
void thread_release(int *threadID) {
	printf("        Thread is realseasing resources\n");
	int idx = *threadID;
	for( int x =0;x<4;x++){
		Available[x]+=customers[idx].Allocation[x];
	}
	return;
}

void *thread_run(void *thread){
  int *thread_id = (int*)thread;
  printf("--> Customer/Thread %d\n",*thread_id);
  printf("        Allocated resources:");
  for (int x = 0; x <4 ;x++){
	  printf(" %d",customers[*thread_id].Allocation[x]);
  }
  printf("\n");
  printf("        Needed:");
	for (int y= 0;y<4;y++){
		printf(" %d",customers[*thread_id].Need[y]);
	}
  printf("\n");
  printf("        Available:");
  for(int z = 0;z<4;z++){
  	printf(" %d",Available[z]);
  }
  printf("\n");
  thread_Start(thread_id);
  thread_Finish(thread_id);
  thread_release(thread_id);
  printf("        New Available:");
  for(int n=0;n<4;n++){
	  printf(" %d",Available[n]);
  }
  printf("\n");
  return 0;
}
