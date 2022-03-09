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
    int state;
    int* initial_claim;
    int* allocated;
    //state to check if process is blocked or not 0 = not blocked, 1 = blocked, 2 = aborted
    //array of initial claim; arrary index 0 means resource type 1 index 1 means resource type 2
    //array of allocated resource; array index 0 means resource type 1 index 1 means resource type 2
}process;

typedef struct instruction{
    int** instruction;
    //limitation, there can be only 100 instruction.
    //each instruction[j] is pointing to instruction[j][k] with size 4 of int
    //2D array of instructions
}instruction;





