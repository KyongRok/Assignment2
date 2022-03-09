/**
 * Full Name: KyongRok Kim 215813413
 * CourseID: EECS3221
 * Discription:
 * initiate = 1, request = 2, compute = 3, release = 4, terminate = 5.
 * ex: initiate 1 1 4 will be 1 1 1 4.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "banker.h"
#include "function.h"

int main (int argc , char* argv[]){

    FILE* fp = fopen(argv[1] , "r");
    struct resource_manager res_man;
    fscanf(fp , "%d" , &res_man.num_task);
    if(res_man.num_task <= 0){
        printf("Error number of task less or equal to 0");
        fclose(fp);
        exit(1);
    }
    fscanf(fp , "%d" , &res_man.num_resource);
    if(res_man.num_resource <= 0){
        printf("Error number of resources less or equal to 0");
        fclose(fp);
        exit(1);
    }

    res_man.add_value = (int*) malloc(res_man.num_resource * sizeof(int));
    for(int i = 0; i < res_man.num_resource; i++){
        fscanf(fp , "%d" , &res_man.add_value[i]);
        //free add_value.
    }
    struct process* process = (struct process*) malloc(sizeof(struct process));
    for(int i = 0; i < res_man.num_task; i++){
        int temp = i;
        process[i].pid = temp+1;
        process[i].state = 0;
        process[i].initial_claim = (int*) calloc(res_man.num_task , sizeof(int));
        process[i].allocated = (int*) calloc(res_man.num_task , sizeof(int));
    }
        //initiallize double pointers to 0 using calloc.
    struct instruction* instruction = malloc(sizeof(struct instruction));
    instruction->instruction = (int**) calloc(100 , sizeof(int*));
    for(int j = 0; j < 100; j++){
        instruction->instruction[j] = (int*) calloc(4 , sizeof(int));
    }
    
    for(int i = 0; i < 100; i++){
        for(int j = 0; j < 4; j++){
            if(j == 0){
                char temp[20];
                if(fscanf(fp , "%s" , temp) != 1){
                    break;
                }

                if(strcmp(temp , "initiate") == 0){

                    instruction->instruction[i][j] = 1;
                }else if(strcmp(temp , "request") == 0){

                    instruction->instruction[i][j] = 2;
                }else if(strcmp(temp , "compute") == 0){

                    instruction->instruction[i][j] = 3;
                }else if(strcmp(temp , "release") == 0){

                    instruction->instruction[i][j] = 4;
                }else if(strcmp(temp , "terminate") == 0){

                    instruction->instruction[i][j] = 5;
                }
            }else{
                int temp;
                if(fscanf(fp , "%d" , &temp) != 1){
                    break;
                }
                instruction->instruction[i][j] = temp;
            }
        }
    }
    banker_algo(process , res_man , instruction);

    //start of code block for free
    //leaks -atExit -- ./a.out input  < to check memory leak (MacOs terminal)
    free(res_man.add_value);
    for(int i = 0; i < res_man.num_task; i++){
        free(process[i].initial_claim);
        free(process[i].allocated);
    }
    free(process);
    for(int j = 0; j < 100; j++){
        free(instruction->instruction[j]);
    }
    free(instruction->instruction);
    free(instruction);
    //end of code block for free
    fclose(fp);
}


void banker_algo(struct process* p , struct resource_manager res_man , struct instruction* inst){
    //while all process terminate 
    //give cycle to forloop -> with number of task,
    //if compute.... 줫같네 ㅅㅂ...
    int clock = 0;
    //initiation
    int initiate_flag = -1;
    for(int i = 0; i < res_man.num_resource; i++){
        for(int j = 0; j < 100; j++){
            for(int k = 0; k < 4; k++){
                if(k == 0 && inst->instruction[j][k] == 1 && inst->instruction[j][k+2] == i){
                    initiate(&p , &res_man , inst->instruction[j][k+1] , inst->instruction[j][k+2] , inst->instruction[j][k+3]);
                    if(initiate_flag != i){
                        clock++;
                        initiate_flag = i;
                    }
                }
            }
        }
    }
    //end of initiation.
    //initiation of same resource type is done in 1 cycle, hence if it goes to another
    //resource type, then clock++; initiate_flag check if it goes to another resource type
    
    //request


}

void initiate(struct process** p1 , struct resource_manager* res_man1 , int process_id , int resource_type , int resource_amount){
    struct process* p = *p1;
    struct resource_manager res_man = *res_man1;
    
    for(int i  = 0; i < res_man.num_task; i++){
        if(p[i].pid == process_id){
            if( resource_amount > res_man.add_value[resource_type]){
                printf("Initial claim of %d is bigger than resource type %d", resource_amount , resource_type);
                printf("abort process %d" , p[i].pid);
                p[i].state = 2;
            }else{
                p[i].initial_claim[resource_type] = resource_amount;
            } 
        }
    }
}




