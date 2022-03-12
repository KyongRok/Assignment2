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
#include <math.h>
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
    }
    struct process* process = (struct process*) malloc(sizeof(struct process) * res_man.num_task);
    for(int i = 0; i < res_man.num_task; i++){
        int temp = i;
        process[i].pid = temp+1;
        process[i].state = 1;
        process[i].priority = 0;
        process[i].wait_time = 0;
        process[i].terminate_time = 0;
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
    for(int i = 0; i < res_man.num_task; i++){
        process[i].task = (int**) calloc(20 , sizeof(int*));
        for(int j = 0; j < 20; j++){
            process[i].task[j] = (int*) calloc(4 , sizeof(int));
        }
    }
    //initiate instruction for each process
    for(int i = 0; i < res_man.num_task; i++){
         int t = 0;
        for(int j = 0; j < 100; j++){
            for(int k = 0; k < 4; k++){
                if(k == 0){
                    if(instruction->instruction[j][k+1] == i+1){
                        process[i].task[t][k] = instruction->instruction[j][k];
                        process[i].task[t][k+1] = instruction->instruction[j][k+1];
                        process[i].task[t][k+2] = instruction->instruction[j][k+2];
                        process[i].task[t][k+3] = instruction->instruction[j][k+3];
                        t++;
                    }
                }
            }
        }
    }

    optimistic(process , res_man , instruction);
    banker_algo(process , res_man , instruction);

    //start of code block for free
    //leaks -atExit -- ./a.out input  < to check memory leak (MacOS terminal)
    free(res_man.add_value);
    for(int i = 0; i < res_man.num_task; i++){
        for(int j = 0; j < 20; j++){
            free(process[i].task[j]);
        }
        free(process[i].task);
    }
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
    int clock = 0;
    //initiation
    for(int i = 0; i < res_man.num_resource; i++){
        for(int j = 0; j < 100; j++){
            for(int k = 0; k < 4; k++){
                if(k == 0 && inst->instruction[j][k] == 1){
                    initiate(&p , &res_man , inst->instruction[j][k+1] , inst->instruction[j][k+2] , inst->instruction[j][k+3]);
                    inst->instruction[j][k] = 0;
                }
            }
        }
        clock++;
    }
    //end of initiation.
    //initiation of same resource type is done in 1 cycle, hence if it goes to another
    //resource type, then clock++; initiate_flag check if it goes to another resource type

    //process running
    struct collector collector;
    collector.resource_collect = (int*) calloc(res_man.num_resource , sizeof(int));
    //resource collector, keeps the resources and gives back to banker after 1 cycle

    int terminate = 0;
    int ender = 0;
    while(ender < 3*res_man.num_task){
        ender = 0;
                //collect resource from collector.
                for(int i = 0; i < res_man.num_resource; i++){
                    if(collector.resource_collect[i] != 0){
                        int temp = collector.resource_collect[i];
                        collector.resource_collect[i] = 0;
                        res_man.add_value[i] += temp; 
                    }
                }
                //end of collector
        for(int i = 0; i < res_man.num_task; i++){
            for(int j = 0; j < 20; j++){
                int k = 0;
                while(k != 0){
                    k = p[i].task[j][0];
                }
                if(p[i].task[j][k] > 1 && p[i].state != 2 && p[i].state != 3){
                    if(p[i].task[j][k] == 2){
                            //request
                            //request function returns 2 when task has to be aborted
                        int request_flag = request(&p ,&res_man ,p[i].pid ,p[i].task[j][2],p[i].task[j][3]);
                        if(request_flag == 1){
                            p[i].allocated[p[i].task[j][2] - 1] += p[i].task[j][3];
                            res_man.add_value[p[i].task[j][2] - 1] -= p[i].task[j][3];
                            p[i].priority = 0;
                            p[i].task[j][0] = 0;
                            j = 20;
                            //request accepted
                        }else if(request_flag == 0){
                            p[i].wait_time++;
                            p[i].priority++;
                            j = 20;
                            //request not granted, task is to be waited.
                            //wait time increased
                            //priority increased for sorting.
                            //put state into blocked
                        }else if(request_flag == 2){
                            abort_task1(&p[i] , &collector , res_man.num_resource);
                            p[i].state = 2;
                            j = 20;
                        }
                    }else if(p[i].task[j][0] == 3){
                        int compute_flag = compute(&p[i]);
                        if(compute_flag == 1){
                            p[i].priority = 0;
                            j = 20;
                            //need to compute more
                        }else if(compute_flag == 0){
                            p[i].task[j][0] = 0;
                            p[i].priority = 0;
                            j = 20;
                            //finish compute
                        }
                        
                    }else if(p[i].task[j][0] == 4){
                        //release
                        release(&collector , &p,p[i].pid,p[i].task[j][2],p[i].task[j][3]);
                        p[i].task[j][0] = 0;
                        j = 20;
                    }else if(p[i].task[j][0] == 5){
                        p[i].state = 3;
                        p[i].terminate_time = clock;
                        p[i].task[j][0] = 0;
                        j = 20;
                    }else{
                        j = 20;
                    }
                }
            }
        }
        //sorts in process array first by state then pid
        //blocked tasks are put at the start of the array so that they are looked into first
        qsort(p , res_man.num_task , sizeof(struct process),sort_by_priority);
        
        clock++;
        for(int i = 0; i < res_man.num_task; i++){
            if(p[i].state == 2){
                ender += 3;
            }else{
                ender += p[i].state;
            }
        }
    }
    //end of process running
    //free resource collector.
    free(collector.resource_collect);

    printf("        Banker's    \n");
    double wait_t = 0;
    double term_t = 0;
    double percent = 0;
    int total_wait = 0;
    int total_term = 0;
    for(int i = 0; i < res_man.num_task; i++){
        if(p[i].state != 2){
            total_wait += p[i].wait_time;
            total_term += p[i].terminate_time;
            wait_t = (double) p[i].wait_time;
            term_t = (double) p[i].terminate_time;
            percent = (wait_t / term_t)*100;
            percent = round(percent);
            printf("Task %d",p[i].pid);
            printf("    %d" , p[i].terminate_time);
            printf("    %d" , p[i].wait_time);
            printf("    %d%%\n" ,(int) percent);
        }else{
            printf("Task %d",p[i].pid);
            printf("      ABORTED      \n");
        }
    }
    double print = ((double) total_wait / (double) total_term)*100;
    print = round(print);
    printf("Total    %d    %d    %d%%\n" , total_term , total_wait , (int) print );

}

void initiate(struct process** p1 , struct resource_manager* res_man1 , int process_id , int resource_type , int resource_amount){
    struct process* p = *p1;
    struct resource_manager res_man = *res_man1;
    
    for(int i  = 0; i < res_man.num_task; i++){
        if(p[i].pid == process_id){
            if( resource_amount > res_man.add_value[resource_type - 1]){
                //resource initiation is greater than banker's resource
                //abort the task (call abort function)
                printf("Initial claim of pid: %d , %d requested larger than resource type %d", p[i].pid, resource_amount , resource_type);
                printf(" abort task %d\n" , p[i].pid);
                p[i].state = 2;
            }else{
                //initiation success
                p[i].initial_claim[resource_type - 1] = resource_amount;
            } 
        }
    }
}

int request(struct process** p1 , struct resource_manager* res_man1, int process_id, int resouce_type , int resouce_amount){
    //if success return 1 else return 0.
    struct process* p = *p1;
    struct resource_manager res_man = *res_man1;

    for(int i = 0; i < res_man.num_task; i++){
        if(p[i].pid == process_id){
            int isSafe_flag = isSafe(p[i] , res_man , resouce_type , resouce_amount);
            if(isSafe_flag == 0){
                //request > banker's resource. abort task
                p[i].state = 2;
                //release function call (abort function)
                return 2;
            }else if(isSafe_flag == 2){
                //unsafe, so task is put to wait
                return 0;
            }else{
                //safe
                return 1;
            }
        }
    }
    return 0;
}

int isSafe(struct process p1 , struct resource_manager res_man1, int resouce_type , int resouce_amount){
    //if safe return 1.
    //if unsafe return 2.
    //if task aborted return 0.
    if(p1.initial_claim[resouce_type - 1] < resouce_amount){
        //abort task due to requesting more than initial claim
        printf("task %d request more than it's initial claim, aboring task? %d\n" , p1.pid , p1.pid);
        return 0;
    }else if(p1.allocated[resouce_type - 1] + resouce_amount > p1.initial_claim[resouce_type-1]){
        //abort task because request more than initial claim (allocated + request > initial claim)
        printf("task %d request more than it's initial claim, aboring task %d\n" , p1.pid , p1.pid);
        return 0;
    }else{
        //safe check for other resources as well
        int safe_flag = 0;
        int need[res_man1.num_task];
        for(int i = 0; i < res_man1.num_task; i++){
            need[i] = 0;
        }
        for(int i = 0; i < res_man1.num_resource; i++){
            need[i] = p1.initial_claim[i];
        }
        for(int i = 0; i < res_man1.num_resource; i++){
            if(need[i] - p1.allocated[i] <= res_man1.add_value[i]){
                safe_flag++;
            }
        }
        if(safe_flag == res_man1.num_resource){
            return 1;
        }else{
            return 2;
        }
    }
}

void release(struct collector* col , struct process** p1, int process_id , int resource_type, int resouce_amount){
    struct collector collector = *col;
    struct process* p = *p1;

    int i = 0;
    while(p[i].pid != process_id){
        i++;
    }
    p[i].allocated[resource_type-1] -= resouce_amount;
    collector.resource_collect[resource_type-1] += resouce_amount;
    //release resouce_amount of resource type from task and add them to collector

}

void abort_task1(struct process* p , struct collector* col, int num_task){
    for(int i = 0; i < num_task; i++){
        int temp = p->allocated[i];
        p->allocated[i] = 0;
        col->resource_collect[i] += temp;
    }
    //abort task, releases all resources to collector
}

int compute(struct process* p){
    for(int i = 0; i < 20; i++){
        printf("%d\n" , p->task[i][0]);
        if(p->task[i][0] == 3){
            p->task[i][2] = p->task[i][2] - 1;
            if(p->task[i][2] == 0){
                return 0;
            }else{
                return 1;
            }
        }
    }
    //returns 0 if compute finish, return 1 if it need more cycle to finish compute task
    return 2;
}

int sort_by_priority(const void* a, const void* b){
    struct process p1 = *((struct process*) a);
    struct process p2 = *((struct process*) b);
    if(p1.priority == p2.priority){
        return (p1.pid - p2.pid);
    }else{
        return (p2.priority - p1.priority);
    }
    //decending order of priority. if equal, samller task ID is prefered
}

void optimistic(struct process* process , struct resource_manager res_man , struct instruction* inst){


}



