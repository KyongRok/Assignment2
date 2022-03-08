/**
 * Full Name: KyongRok Kim 215813413
 * CourseID: EECS3221
 * Discription:
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deadlock.h"
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

    res_man.add_value = malloc(res_man.num_resource * sizeof(int));
    for(int i = 0; i < res_man.num_resource; i++){
        fscanf(fp , "%d" , &res_man.add_value[i]);
        //free add_value.
    }
    
    struct process* p = malloc(sizeof(struct process) * res_man.num_task);
    for(int i = 0; i < res_man.num_task; i++){
        //initiallize 2D array to 0 using calloc.
        p[i].instruction = (int**) calloc(10 , sizeof(int*));
        for(int j = 0; j < 10; j++){
            p[i].instruction[j] = (int*) calloc(4 , sizeof(int));
        }
    }
    for(int i = 0; i < res_man.num_task; i++){
        p[i].pid = i+1;
        for(int j = 0; j < 10; j++){
            int flag = 0;
            for(int k = 0; k < 4; k++){
                if(k == 0){
                    char temp[20];
                    fscanf(fp , "%s" , temp);
                    if(strcmp(temp , "initiate") == 0){
                        p[i].instruction[j][k] = 1;
                    }else if(strcmp(temp , "request") == 0){
                        p[i].instruction[j][k] = 2;
                    }else if(strcmp(temp , "compute") == 0){
                        p[i].instruction[j][k] = 3;
                    }else if(strcmp(temp , "release") == 0){
                        p[i].instruction[j][k] = 4;
                    }else if(strcmp(temp , "terminate") == 0){
                        p[i].instruction[j][k] = 5;
                        flag = 1;
                        //when it reads terminate, that means next reading will be a new process
                    }
                }else{
                    int temp;
                    fscanf(fp , "%d" , &temp);
                    p[i].instruction[j][k] = temp;
                }
            }
            if(flag == 1){
                    j = 10;
                }
        }
    }



    printf("%d %d" , res_man.num_task , res_man.num_resource);
    for(int i = 0; i < res_man.num_resource; i++){
        printf(" %d" , res_man.add_value[i]);
    }
    printf("\n");
    for(int i = 0; i < res_man.num_task; i++){
        printf("%d\n" , p[i].pid);
    }
    for(int i = 0; i < res_man.num_task; i++){
        for(int j  = 0; j < 10; j++){
            for(int k = 0; k < 4; k++){
                printf("%d" , p[i].instruction[j][k]);
            }
            printf("\n");
        }
    }


    //start of code block for free
    free(res_man.add_value);
    for(int i = 0; i < res_man.num_task; i++){
        for(int j = 0; j < 10; j++){
            free(p[i].instruction[j]);
        }
    }
    for(int i = 0; i < res_man.num_task; i++){
        free(p[i].instruction);
    }
    free(p);
    //end of code block for free
    fclose(fp);
}


