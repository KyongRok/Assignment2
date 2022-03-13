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
    struct resource_manager res_man1;
    fscanf(fp , "%d" , &res_man.num_task);
    res_man1.num_task = res_man.num_task;
    if(res_man.num_task <= 0){
        printf("Error number of task less or equal to 0");
        fclose(fp);
        exit(1);
    }
    fscanf(fp , "%d" , &res_man.num_resource);
    res_man1.num_resource = res_man.num_resource;
    if(res_man.num_resource <= 0){
        printf("Error number of resources less or equal to 0");
        fclose(fp);
        exit(1);
    }

    res_man.add_value = (int*) malloc(res_man.num_resource * sizeof(int));
    res_man1.add_value = (int*) malloc(res_man1.num_resource * sizeof(int));
    for(int i = 0; i < res_man.num_resource; i++){
        fscanf(fp , "%d" , &res_man.add_value[i]);
        res_man1.add_value[i] = res_man.add_value[i];
    }
    struct process* process = (struct process*) malloc(sizeof(struct process) * res_man.num_task);
    struct process* process1 = (struct process*) malloc(sizeof(struct process) * res_man1.num_task);
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
    for(int i = 0; i < res_man1.num_task; i++){
        int temp = i;
        process1[i].pid = temp+1;
        process1[i].state = 1;
        process1[i].priority = 0;
        process1[i].wait_time = 0;
        process1[i].terminate_time = 0;
        process1[i].allocated = (int*) calloc(res_man1.num_task , sizeof(int));
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
    for(int i = 0 ; i < res_man1.num_task; i++){
        process1[i].task = (int**) calloc(20 , sizeof(int*));
        for(int j = 0; j < 20; j++){
            process1[i].task[j] = (int*) calloc(4 , sizeof(int));
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

                        process1[i].task[t][k] = instruction->instruction[j][k];
                        process1[i].task[t][k+1] = instruction->instruction[j][k+1];
                        process1[i].task[t][k+2] = instruction->instruction[j][k+2];
                        process1[i].task[t][k+3] = instruction->instruction[j][k+3];
                        t++;
                    }
                }
            }
        }
    }
    //used 2 different array of process/resource_manager because running 1 algorithm
    //will change the values in the array of process (since using pointer)

    optimistic(process1 , res_man1);
    banker_algo(process , res_man , instruction);

    double wait_t = 0;
    double term_t = 0;
    double percent = 0;
    int total_wait = 0;
    int total_term = 0;
    double wait_t1 = 0;
    double term_t1 = 0;
    double percent1 = 0;
    int total_wait1 = 0;
    int total_term1 = 0;
        printf("           FIFO               ");
        printf("        Banker's    \n");
    for(int i = 0; i < res_man1.num_task; i++){
            if(process1[i].state != 2){
                total_wait += process1[i].wait_time;
                total_term += process1[i].terminate_time;
                wait_t = (double) process1[i].wait_time;
                term_t = (double) process1[i].terminate_time;
                percent = (wait_t / term_t)*100;
                percent = round(percent);
            }
            if(process[i].state != 2){
                total_wait1 += process[i].wait_time;
                total_term1 += process[i].terminate_time;
                wait_t1 = (double) process[i].wait_time;
                term_t1 = (double) process[i].terminate_time;
                percent1 = (wait_t1 / term_t1)*100;
                percent1 = round(percent1);
            }
        if(process1[i].state != 2){
            printf("Task %d",process1[i].pid);
            printf("    %d" , process1[i].terminate_time);
            printf("    %d" , process1[i].wait_time);
            printf("    %d%%          " ,(int) percent);
        }
        if(process1[i].state == 2){
            printf("Task %d", process1[i].pid);
            printf("      ABORTED              ");
        }
        if(process[i].state != 2){
            printf("Task %d",process[i].pid);
            printf("    %d" , process[i].terminate_time);
            printf("    %d" , process[i].wait_time);
            printf("    %d%%\n" ,(int) percent1);
        }
        if(process[i].state == 2){
            printf("Task %d",process[i].pid);
            printf("      ABORTED      \n");
        }
    }
    double print = ((double) total_wait / (double) total_term)*100;
    print = round(print);
    double print1 = ((double) total_wait1 / (double) total_term1)*100;
    print1 = round(print1);
    printf("Total     %d    %d    %d%%" , total_term , total_wait , (int) print );
    printf("          Total     %d    %d    %d%%\n" , total_term1 , total_wait1 , (int) print1 );



    
    //start of code block for free
    //leaks -atExit -- ./a.out input  < to check memory leak (MacOS terminal)
    free(res_man.add_value);
    free(res_man1.add_value);
    for(int i = 0; i < res_man.num_task; i++){
        for(int j = 0; j < 20; j++){
            free(process[i].task[j]);
            free(process1[i].task[j]);
        }
        free(process[i].task);
        free(process1[i].task);
    }
    for(int i = 0; i < res_man.num_task; i++){
        free(process[i].initial_claim);
        free(process[i].allocated);
        free(process1[i].allocated);
    }
    free(process);
    free(process1);
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
}

void initiate(struct process** p1 , struct resource_manager* res_man1 , int process_id , int resource_type , int resource_amount){
    struct process* p = *p1;
    struct resource_manager res_man = *res_man1;
    
    for(int i  = 0; i < res_man.num_task; i++){
        if(p[i].pid == process_id){
            if( resource_amount > res_man.add_value[resource_type - 1]){
                //resource initiation is greater than banker's resource
                //abort the task (call abort function)
                printf("Banker aborts task%d before run begins:\n", p[i].pid);
                printf("claim for resource %d (%d) exceeds number of units present (%d)\n" , resource_type , resource_amount , res_man1->add_value[resource_type-1]);
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
        printf("task %d request more than it's initial claim, aboring task %d\n" , p1.pid , p1.pid);
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

void abort_task1(struct process* p , struct collector* col, int num_resource){
    for(int i = 0; i < num_resource; i++){
        int temp = p->allocated[i];
        p->allocated[i] = 0;
        col->resource_collect[i] += temp;
    }
    //abort task, releases all resources to collector
}

int compute(struct process* p){
    for(int i = 0; i < 20; i++){
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

void optimistic(struct process* process , struct resource_manager res_man){
    
    struct collector collector;
    collector.resource_collect = calloc(res_man.num_task , sizeof(int));
    int terminate = 0;
    int clock = 0;
    //add ender
    int initial[res_man.num_resource];
    for(int i = 0; i < res_man.num_resource; i++){
        initial[i] = res_man.add_value[i];
    }

    while(terminate < 20){
        int deadlock_detect = 0;
        for(int i = 0; i < res_man.num_resource; i++){
            int temp = collector.resource_collect[i];
            collector.resource_collect[i] = 0;
            res_man.add_value[i] += temp;
        }
        //collector gives to resource manager

        for(int i = 0; i < res_man.num_task; i++){
            for(int j = 0; j < 20; j++){
                if(process[i].state != 2 && process[i].state != 3){
                    if(process[i].task[j][0] == 1){
                        //initiate
                        process[i].task[j][0] = 0;
                        j = 20;
                    }else if(process[i].task[j][0] == 2){
                        //request_optimistic function call
                        int request_flag = request_optimistic(&process[i] , res_man,initial[process[i].task[j][2] -1]);
                        if(request_flag == 1){
                            //printf("here1");
                            process[i].task[j][0] = 0;
                            int resource_type = process[i].task[j][2] -1;
                            int request_amount = process[i].task[j][3];
                            process[i].allocated[resource_type] += request_amount;
                            res_man.add_value[resource_type] -= request_amount;
                            process[i].priority = 0;
                            j = 20;
                        }else if(request_flag == 0){
                            deadlock_detect++;
                            process[i].wait_time++;
                            process[i].priority++;
                            j = 20;
                            //if deadlock_detect == numer of task, that means all tasks are waiting
                            //hence deadlock
                        }else if(request_flag == 2){
                            abort_task1(&process[i] , &collector,res_man.num_resource);
                            process[i].state = 2;
                            j = 20;
                        }
                    }else if(process[i].task[j][0] == 3){
                        int compute_flag = compute(&process[i]);
                        if(compute_flag == 1){
                            process[i].priority = 0;
                            j = 20;
                            //need to compute more
                        }else if(compute_flag == 0){
                            process[i].task[j][0] = 0;
                            process[i].priority = 0;
                            j = 20;
                            //finish compute
                        }
                    }else if(process[i].task[j][0] == 4){
                        //release function call
                        int resource_type = process[i].task[j][2];
                        int resource_amount = process[i].task[j][3];
                        release(&collector , &process, process[i].pid ,resource_type , resource_amount);
                        process[i].task[j][0] = 0;
                        j = 20;
                    }else if(process[i].task[j][0] == 5){
                        //terminate
                        process[i].task[j][0] = 0;
                        process[i].terminate_time = clock;
                        process[i].state = 3;
                        j = 20;
                    }
                }
            }
        }
        //check for deadlock
        int running_task = 0;
        for(int i = 0; i < res_man.num_task; i++){
            if(process[i].state != 2 && process[i].state != 3){
                running_task++;
            }
        }

        if(deadlock_detect == running_task){
            //abort first task
            int need[res_man.num_resource];
            for(int i = 0; i < res_man.num_resource; i++){
                need[i] = 0;
            }
            int dead = 0;
            while(dead != running_task){
                //since lowest id has to be aborted first, sort by id
                qsort(process , res_man.num_task , sizeof(struct process),sort_by_id);
                for(int i = 0; i < res_man.num_task; i++){
                    if(process[i].state != 2 && process[i].state != 3){
                        abort_task1(&process[i] , &collector , res_man.num_resource);
                        process[i].state = 2;
                        break;
                    }
                }
                //sort back to priority
                qsort(process , res_man.num_task , sizeof(struct process),sort_by_priority);
                for(int i = 0; i < res_man.num_task; i++){
                    if(process[i].state != 2 && process[i].state != 3){
                        for(int j = 0; j < 20; j++){
                            if(process[i].task[j][0] == 2){
                                int resource_type = process[i].task[j][2];
                                int resource_amount = process[i].task[j][3];
                                need[resource_type - 1] = resource_amount;
                                j = 20;
                                //if there is any task that can be granted, stop aborting
                                for(int i = 0; i < res_man.num_resource; i++){
                                    if(res_man.add_value[i] + collector.resource_collect[i]  >= need[i]){
                                        dead = running_task;
                                    }
                                }
                            }
                        }
                    }
                }
                //dead++;
            }
            
        }
        qsort(process , res_man.num_task , sizeof(struct process),sort_by_priority);
        clock++;
        terminate++;
    }
    qsort(process , res_man.num_task , sizeof(struct process),sort_by_id);
    free(collector.resource_collect);
}

int request_optimistic(struct process* p , struct resource_manager res_man,int initial){
    for(int i = 0; i < 20; i++){
        if(p->task[i][0] == 2){
            int resource_type = p->task[i][2] -1;
            int request_amount = p->task[i][3];
            int alloc = 0;
            
            if(res_man.add_value[resource_type] >= request_amount){
                //request granted
                return 1;
            }else if(initial < request_amount){
                //abort task since request too much
                return 2;
            }else{
                //wait
                return 0;
            }
        }
    }
    //return 1 if request granted, else 0.
    //return 2 when it failed to find task
    return 2;
}

int sort_by_id(const void* a, const void* b){
    struct process p1 = *((struct process*) a);
    struct process p2 = *((struct process*) b);
        return (p1.pid - p2.pid);
    
    //sort in ascdending order of id
}





