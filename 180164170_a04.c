int safty_check(int Available[],int safe_seq[]){
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
            safe_seq[o++] = n;
            m=-1;
        }
    }
    int i = 0;
    while(is_safe[i] != 1){
            return 0;
            i++;
    }
    
    return 1;
}
void request(int order,int q[],int Available[],int safe_seq[]){
    if(comp(q,customers[order].Need)==0){
        printf("requests fail\n");
        return 0;
    }
    else if(comp(q,Available)==1){
        try_all(order,q,Available);
        if(safty_check(Available,safe_seq)==0){
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
        customers[order].Need[i] -= q[r];
    }
}

int comp(int one[],int two[]){
    int i = 0;
    while(i < 4 && one[i]<=two[i]){
          i++;
    }
    if(i<4 || one[i]>two[i]){
          return 0;
    }
         return 1;
}
int release(int order,int r[],int Available[]){//release resources 
    if(comp(r,customers[order].Allocation)==1){
        int w = 0;
        while(w< 4){
            customers[order].Allocation[w] -= r[w];
            Available[w] += r[w];
            w++;
        }
      
        printf("The resources have been released successfully\n");

    }
    else{
        printf("invalid\n");
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
