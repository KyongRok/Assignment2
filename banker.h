/**
 * Full Name: KyongRok Kim 215813413
 * CourseID: EECS3221
 * Discription:
 */

typedef struct resource_manager{
    int num_task;
    int num_resource;
    int* add_value;
    //limitation, value of each resource cannot be more than 9999
}resource_manager;

typedef struct process{
    int pid;
    int* initial_claim;
    int* allocated;
}process;

typedef struct instruction{
    int** instruction;
    //limitation, there can be only 100 instruction.
}instruction;

typedef struct node{
    struct process* p;
    struct node* next;
    //for linkedlist
}node;




