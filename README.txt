Full Name: KyongRok Kim 215813413
CourseID: EECS3221
Description: README.txt

                    **How to compile** 
go to directory where all files are present (banker.c banker.h function.h Makefile)
then type in the command < make >  //without < and >
then type in ./banker <name of input file> //without < and >

                    **Dependancy**
banker.c is the main
banker.h holds the data structure for banker.c 
function.h holds the function headers for banker.c 

                    **Data structure**
resource_manager: holds number of task, number of resource and creates dynamically allocated memory of
number of resources * sizeof int array. in this array index 0 holds number of resources of type 1
index 1 holds type 2... and so on

collector: holds a array size of number of task. in this array index 0 holds number of released resources 
type 1 and index 1 type 2... and so on. It is checked before each cycle starts to check if there was
any released resources from cycle before and puts to available for tasks to use

process: this is the task itself, holds ID, state, priority, terminate time, initial claim, allocated
and task(instruction).

ID represents the task number

State represents the state of task , 1 = able to run, 2 = aborted , 3 = terminated

priority is increased each time the task waits for request, if it's request/release/compute is granted
then priority is set back to 0

terminate time holds the value of when it gets terminated

initial claim is array that holds the initial claim of the task for example, initial 1 1 4
will be stored as 1 1 1 4 in this array

allocated is the array that holds the allocated resource of this task. for example if task 1 holds
0 of type 1, 2 of type2, 1 of type 3 then it is stored as 0 2 1

task is 2D array that holds the instruction.
initiate = 1 , request = 2, compute = 3, release = 4, terminate = 5
example, if request 1 1 3, meaning task 1 request 3 of type 1
so it will be 1 1 1 3 in this 2D array. limitation of my implementation is that each task can 
only hold up to 20 instructions

instruction: is 2D array simillar to task in process's data structure but holds all the instruction
inside the input file. limilation of my implementation is that there can be only in total 100 
instruction for this program to work. Having more than 100 instruction will not give wrong 
output values.

                **Banker.c**
main implementation of the program, for each block of code there is comment in between which
helps exmplain what it each block does

