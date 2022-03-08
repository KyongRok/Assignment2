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

    struct process* p = malloc(res_man.num_task * sizeof(struct process));
    for(int i = 0; i < res_man.num_task; i++){
        p[i].pid = i+1;
        p->instruction = malloc(10 * sizeof(int*));
        for(int j = 0; j < 10; j++){
            p->instruction[j] = NULL;
        }
        //free process's instruction**
    }


    printf("%d %d" , res_man.num_task , res_man.num_resource);
    for(int i = 0; i < res_man.num_resource; i++){
        printf(" %d" , res_man.add_value[i]);
    }
    for(int i = 0; i < res_man.num_task; i++){
        printf(" %d" , p[i].pid);
    }


    free(res_man.add_value);
    fclose(fp);
}
