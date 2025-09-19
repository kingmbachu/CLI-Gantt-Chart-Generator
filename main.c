//include header files
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Define constants for task size, number of tasks, and number of months
#define TASK_SIZE 25
#define NO_OF_TASKS 10
#define NO_OF_MONTHS 12

// Clear screen command based on the operating systems
#ifdef _WIN32
    #define CLEAR_SCREEN "cls"
#else
    #define CLEAR_SCREEN "clear"
#endif


// Enum for months ( 1 - January, 2 = February, and so on)
typedef enum {
    JAN = 1, FEB, MAR, APR, MAY, JUN, JUL, AUG, SEP, OCT, NOV, DEC
} Month;

// Struct to represent a task
typedef struct {
    char name[TASK_SIZE];                  // Name of the task
    int startMonth;                       // Start month of the task
    int endMonth;                        // End month of the task
    int dependencies[NO_OF_TASKS];      // Array of task dependencies
    int numDependencies;               // Number of dependencies for the task
} Task;


// Function prototypes
void printAsciiArt();
void welcomeScreen(Task tasks[], int *numOfTasks);
void loadTestTasks(Task tasks[], int *numOfTasks);
void editTask(Task tasks[], int numOfTasks);
void createGantt(Task tasks[], int *numOfTasks);
void displayGantt(Task tasks[], int numOfTasks);
int detectCircularDependency(Task tasks[], int numOfTasks);
int isCyclicUtil(Task tasks[], int current, int visited[], int recursionStack[], int numOfTasks);
int checkCircDependency(Task tasks[], int dependentTaskId, int currentRow, int visitedTasks[]);
void testCriticalPath(Task tasks[], int numOfTasks);
void findCriticalPath(Task tasks[], int current, int numOfTasks, int currentPath[], int chainLength, int *maxLength, int bestPath[], int visited[]);

// Main Function
int main(void) {
    Task tasks[NO_OF_TASKS];  // Array to store tasks
    int numOfTasks = 0;       // Number of tasks currently in the array
    
    printAsciiArt();
    welcomeScreen(tasks, &numOfTasks);  // Start the program by displaying the welcome screen

    return 0;
}

/*
 * This method prints an ASCII art at the start of the execution of this program
 *
 * There is no input or output.
 */
void printAsciiArt() {
    printf("  ________  ________  ________  ________  ________  ________\n");
    printf(" |  ______||  ______||  ______||  ______||  ______||  ______|\n");
    printf(" | |_____  | |_____  | |_____  | |_____  | |_____  | |_____  \n");
    printf(" |_____  | |_____  | |_____  | |_____  | |_____  | |_____  |\n");
    printf("  ______| | ______| | ______| | ______| | ______| | ______| |\n");
    printf(" |________||________||________||________||________||________|\n");
    puts("");
}
/*
* This function displays the welcome menu. The user can choose between creating a Gantt chart
* or to use a preloaded test example.
*
* Input:
* Task tasks[]: the array of Task structures, which contains the name of the task
* the start and end month of the task, number of dependencies of that task and the tasks it depends on.
* int *numOfTasks: a pointer to an integer that stores the number of tasks
*
* There is no output.
*/

void welcomeScreen(Task tasks[], int *numOfTasks) {
    // array to store user's choice
    char choice[10];
    int testOrCreateValid = 0;

    printf("Welcome to the Gantt Generator!\n");
    printf("Would you like to use the \"test\" example or \"create\" your own Gantt Chart from scratch?\n");
    fflush(stdout);

    while (!testOrCreateValid) {
        scanf("%s", choice);
        if (strcmp(choice, "create") == 0) {
            createGantt(tasks, numOfTasks); // Create a custom Gantt chart
            if (*numOfTasks > 0) {
                displayGantt(tasks, *numOfTasks);
            }
            testOrCreateValid = 1;
        } else if (strcmp(choice, "test") == 0) {
            loadTestTasks(tasks, numOfTasks); // Load test tasks
            displayGantt(tasks, *numOfTasks); // Display the Gantt chart
            testOrCreateValid = 1;
        } else {
            printf("Invalid input, type \"test\" for test example or \"create\" to create your own Gantt Chart.\n");
        }
    }

    int doneEditing = 0;
    while (!doneEditing) {
        printf("To create a Gantt chart type \"create\" / If you wish to edit the Gantt please type \"edit\" / If you wish to run a \"test\" or to exit type \"quit\" and then press enter to execute your option.\n");
        fflush(stdout);
        scanf("%s", choice);

        if (strcmp("edit", choice) == 0) {
            if (*numOfTasks == 0) {
                printf("No tasks to edit. Create tasks first.\n");
            } else {
                editTask(tasks, *numOfTasks);
                displayGantt(tasks, *numOfTasks); // Refresh after edit
            }
        } else if (strcmp("test", choice) == 0) {
            if (*numOfTasks == 0) {
                printf("No tasks to test. Create tasks first.\n");
            } else {
                testCriticalPath(tasks, *numOfTasks);
            }
        } else if (strcmp("create", choice) == 0) {
            createGantt(tasks, numOfTasks);
            if (*numOfTasks > 0) {
                displayGantt(tasks, *numOfTasks);
            } 
        } else if (strcmp("quit", choice) == 0) {
            doneEditing = 1;
        } else {
            printf("Invalid input. Try again.\n");
            fflush(stdout);
        }
    }
}

/*
* This function loads test tasks.
*
* Input:
* Task tasks[]: the array of Task structures, which contains the name of the task
* the start and end month of the task, number of dependencies of that task and the tasks it depends on.
* int *numOfTasks: a pointer to an integer that stores the number of tasks
*
* There is no output.
*/

void loadTestTasks(Task tasks[], int *numOfTasks) {
    *numOfTasks = 10; // Set the number of tasks to 10

    // Define test tasks with names, start/end months, and dependencies
    strcpy(tasks[0].name, "Research");
    tasks[0].startMonth = JAN;
    tasks[0].endMonth = MAR;
    tasks[0].numDependencies = 0;

    strcpy(tasks[1].name, "Budget_Planning");
    tasks[1].startMonth = FEB;
    tasks[1].endMonth = MAY;
    tasks[1].numDependencies = 0;

    strcpy(tasks[2].name, "Interior_design");
    tasks[2].startMonth = MAR;
    tasks[2].endMonth = JUN;
    tasks[2].numDependencies = 2;
    tasks[2].dependencies[0] = 0;
    tasks[2].dependencies[1] = 1;

    strcpy(tasks[3].name, "Site_Analysis");
    tasks[3].startMonth = APR;
    tasks[3].endMonth = JUL;
    tasks[3].numDependencies = 1;
    tasks[3].dependencies[0] = 2;

    strcpy(tasks[4].name, "Design_Development");
    tasks[4].startMonth = MAY;
    tasks[4].endMonth = AUG;
    tasks[4].numDependencies = 0;

    strcpy(tasks[5].name, "Fixture_Selection");
    tasks[5].startMonth = JUN;
    tasks[5].endMonth = SEP;
    tasks[5].numDependencies = 1;
    tasks[5].dependencies[0] = 3;

    strcpy(tasks[6].name, "Permits_Approvals");
    tasks[6].startMonth = JUL;
    tasks[6].endMonth = OCT;
    tasks[6].numDependencies = 1;
    tasks[6].dependencies[0] = 5;

    strcpy(tasks[7].name, "Construction_phase");
    tasks[7].startMonth = AUG;
    tasks[7].endMonth = NOV;
    tasks[7].numDependencies = 1;
    tasks[7].dependencies[0] = 6;

    strcpy(tasks[8].name, "Interior_Finishing");
    tasks[8].startMonth = SEP;
    tasks[8].endMonth = DEC;
    tasks[8].numDependencies = 0;

    strcpy(tasks[9].name, "Final_Inspection");
    tasks[9].startMonth = OCT;
    tasks[9].endMonth = DEC;
    tasks[9].numDependencies = 2;
    tasks[9].dependencies[0] = 7;
    tasks[9].dependencies[1] = 8;
}

/*
* This function edits a task of user's choice.
*
* Input:
* Task tasks[]: the array of Task structures, which contains the name of the task
* the start and end month of the task, number of dependencies of that task and the tasks it depends on.
* int numOfTasks: an integer that stores the number of tasks
*
* There is no output.
*/
void editTask(Task tasks[], int numOfTasks) {
    if (numOfTasks == 0) {
        printf("Error: No tasks available to edit.\n");
        return;
    }
    
    int found = 0;
    char taskToEdit[TASK_SIZE];

    printf("Please enter the task name you wish to change exactly: ");
    fflush(stdout);
    scanf("%s", taskToEdit);

    // Search for the task to edit
    for (int i = 0; i < numOfTasks; i++) {
        if (strcmp(taskToEdit, tasks[i].name) == 0) {
            found = 1; // the task was found

            Task tempTask = tasks[i]; // Store the original task in case of errors

            // Prompt the user to update task details
            printf("Please enter the new task name or write its old one (use _ instead of spaces):\n");
            fflush(stdout);
            scanf("%s", tasks[i].name);
            //enter start month
            do {
                printf("Start month (1-12):\n");
                fflush(stdout);
                scanf("%d", &tasks[i].startMonth);
            } while (tasks[i].startMonth < 1 || tasks[i].startMonth > 12); //check that it is valid
            //enter end month
            do {
                printf("End month (1-12):\n");
                fflush(stdout);
                scanf("%d", &tasks[i].endMonth);
            } while (tasks[i].endMonth < 1 || tasks[i].endMonth > 12 || tasks[i].endMonth < tasks[i].startMonth); //check that it is valid
            
            //enter dependencies for each task
            printf("Enter how many dependencies this task has:\n");
            fflush(stdout);
            scanf("%d", &tasks[i].numDependencies);
            for (int j = 0; j < tasks[i].numDependencies; j++) {
                printf("Enter dependent task (enter task number 1-%d):\n", numOfTasks);
                fflush(stdout);
                scanf("%d", &tasks[i].dependencies[j]);
                tasks[i].dependencies[j]--; //decrement the taskId in order to match array's indices
            }
            // Check for circular dependencies after editing
            if (detectCircularDependency(tasks, numOfTasks)) {
                printf("Error: Circular dependency detected! Reverting changes.\n");
                tasks[i] = tempTask; // Revert changes
            } else {
                printf("Task edited successfully.\n");
                // Display updated Gantt chart after successful edit
                displayGantt(tasks, numOfTasks);
            }
            break;
        }
    }
    // print following line, if the user enters invalid task name
    if (!found) {
        printf("Task not found. Please check the name and try again.\n");
    }
}

/*
* This function allows user to create a Gantt chart. It asks the user for the number of tasks, their names
* start and end months as well as dependencies. In case it detects circular dependency, the task
* creation is aborted.
*
* Input:
* Task tasks[]: the array of Task structures, which contains the name of the task
* the start and end month of the task, number of dependencies of that task and the tasks it depends on.
* int *numOfTasks: a pointer to an integer that stores the number of tasks
*
* There is no output.
*/
void createGantt(Task tasks[], int *numOfTasks) {
    printf("How many tasks would you like to add? (1-10)\n");
    fflush(stdout);
    scanf("%d", numOfTasks);

    // Add tasks one by one
    for (int i = 0; i < *numOfTasks; i++) {
        printf("Please enter the task name (use _ instead of spaces):\n");
        fflush(stdout);
        scanf("%s", tasks[i].name);
        //enter start month
        do {
            printf("Start month (1-12):\n");
            fflush(stdout);
            scanf("%d", &tasks[i].startMonth);
        } while (tasks[i].startMonth < 1 || tasks[i].startMonth > 12); //check that it is valid
        //enter end month
        do {
            printf("End month (1-12):\n");
            fflush(stdout);
            scanf("%d", &tasks[i].endMonth);
        } while (tasks[i].endMonth < 1 || tasks[i].endMonth > 12 || tasks[i].endMonth < tasks[i].startMonth); //check that it is valid
        
        //enter dependencies for each task
        printf("Enter how many dependencies this task has:\n");
        fflush(stdout);
        scanf("%d", &tasks[i].numDependencies);
        if (tasks[i].numDependencies != 0) {
            for (int j = 0; j < tasks[i].numDependencies; j++) {
                printf("Enter dependent task (enter task number 1-%d):\n", *numOfTasks);
                fflush(stdout);
                scanf("%d", &tasks[i].dependencies[j]);
                tasks[i].dependencies[j]--; //decrement the taskId in order to match array's indices
            }
        }
    }

    // Check for circular dependency after all tasks are added
    // disable this to prevent this
    // 
    if (detectCircularDependency(tasks, *numOfTasks)) {
        printf("Error: Circular dependency detected! Task creation aborted.\n");
        printf("The circular dependency chain involves the tasks shown above.\n");
        *numOfTasks = 0; // Reset the number of tasks
    } else {
        printf("Tasks created successfully.\n");
    }
        
}

/*
* This function displays the Gantt chart.
*
* Input:
* Task tasks[]: the array of Task structures, which contains the name of the task
* the start and end month of the task, number of dependencies of that task and the tasks it depends on.
* int numOfTasks: an integer that stores the number of tasks
*
* There is no output.
*/
void displayGantt(Task tasks[], int numOfTasks) {
    system(CLEAR_SCREEN); // Clear the console screen
    printf("__________________________________________________________________________________________________________________________________________________________________________________\n");
    printf("                    | January   | February  | March     |  April    |  May      |   June    |   July    |  August   | September |  October  |  November |  December | Dependencies \n");
    printf("----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

    // Display each task's timeline
    for (int i = 0; i < numOfTasks; i++) {
        printf(" %-18s |", tasks[i].name); // left-aligned string within an 18-character wide field.
    // Display "XXX" for each month the task is active
        for (int months = 1; months < NO_OF_MONTHS + 1; months++) {
            if ((months >= tasks[i].startMonth) && (months <= tasks[i].endMonth)) { //if months is between startMonth and endMonth including them, we print XXX
                printf("    XXX    |");
            } else {
                printf("           |"); //otherwise, leave it blank
            }
        }
        // Display task dependencies in a last column
        if (tasks[i].numDependencies > 0) {
            for (int j = 0; j < tasks[i].numDependencies; j++) {
                printf(" %d", tasks[i].dependencies[j] + 1);
            }
        } else {
            printf("             "); // if there are no dependencies, we just leave it blank
        }

        printf("\n----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    }
}

/*
* This function detects circular dependencies.
*
* Input:
* Task tasks[]: the array of Task structures, which contains the name of the task
* the start and end month of the task, number of dependencies of that task and the tasks it depends on.
* int numOfTasks: an integer that stores the number of tasks
*
* Output:
* 0 if there is no circular dependency. 1 if there is a circular dependency.
*/
int detectCircularDependency(Task tasks[], int numOfTasks) {
    int visited[NO_OF_TASKS] = {0};         // array to track visited tasks
    int recursionStack[NO_OF_TASKS] = {0};  // array to track recursion stack
    int result = 0;

    for (int i = 0; i < numOfTasks; i++) {
        if (!visited[i] && isCyclicUtil(tasks, i, visited, recursionStack, numOfTasks)) {
            result = 1;
            break;
        }
    }
    return result;
}
/*
* This function detects cycles in task dependencies. It tracks visited nodes and the current recursion stack to identify
* circular dependencies. Also prints detected circular dependencies to the console.
* 
* Input:
* Task tasks[]: the array of Task structures containing the tasks
* int current: index of the current task being processed
* int visited[]: array tracking visited tasks
* int recursionStack[]: array tracking tasks in the current recursion stack
* int numOfTasks: total number of tasks in the array
*
* Output:
* 0 if there is no circular dependency. 1 if there is a circular dependency.
*/
int isCyclicUtil(Task tasks[], int current, int visited[], int recursionStack[], int numOfTasks) {
    
    if (!visited[current]) { //if we havent visited the current task yet
        visited[current] = 1; // we mark it as visited
        recursionStack[current] = 1; //add the current task to the recursion stack so that we can track the current path


        // loop through all dependencies of the current task
        for (int i = 0; i < tasks[current].numDependencies; i++) {
            //get the index of the dependent task
            int dep = tasks[current].dependencies[i];
            
            // if the dependent task hasn't been visited, recursively check it
            if (!visited[dep]) {
                if (isCyclicUtil(tasks, dep, visited, recursionStack, numOfTasks)) {
                    //if a cycle is detected in the recursive call, print the circular dependency
                    printf("Circular dependency detected: %s (%d) -> %s (%d)\n", 
                           tasks[current].name, current+1, tasks[dep].name, dep+1);
                    return 1; // cycle was found
                }
            }
            // if the dependent task is already in the recursion stack, a cycle exist
            else if (recursionStack[dep]) {
                //print circular dependency
                printf("Circular dependency detected: %s (%d) -> %s (%d)\n", 
                       tasks[current].name, current+1, tasks[dep].name, dep+1);
                return 1; // cycle was found
            }
        }
    }
    
    recursionStack[current] = 0;  // Remove the current task from recursion stack for subsequent checks
    return 0; // no cycle was found
}

/*
* This is a helper function for circular depencdency detection.
*
* Input:
* Task tasks[]: the array of Task structures, which contains the name of the task
* the start and end month of the task, number of dependencies of that task and the tasks it depends on.
* int dependentTaskId: is the task being checked for cycles
* int currentRow: the task whose dependencies are bring processed
* int visitedTasks[]: array marking tasks visited in the current traversal
*
* Output:
* 0 if there is no circular dependency. 1 if there is circular dependency.
*/
int checkCircDependency(Task tasks[], int dependentTaskId, int currentRow, int visitedTasks[]) {
    //initialise a variable length in order to see how many dependencies a task has
    int length = tasks[currentRow].numDependencies;

    // base cases:
    // if the task has no dependencies, return 0 (no cycle)
    if (length == 0) {
        return 0; // No circular dependency
    }
    // if the task is already marked in visitedTasks, a cycle exists. Return 1
    if (visitedTasks[dependentTaskId]) {
        return 1; // Circular dependency found
    }

    visitedTasks[dependentTaskId] = 1; // Mark the task as visited to track the current path
    // Recursively check dependencies
    // For each depenency of the currentRow task:
    //  - get the dependency's index depIndex
    //  - recursively call checkCircDependency with dependentTaskId = depIndex and currentRow = depIndex
    //  - if any recursive call returns 1, pass the cycle detection result upward
    for (int i = 0; i < length; i++) {
        int depIndex = tasks[currentRow].dependencies[i]; //dependencies of the current task
        if (checkCircDependency(tasks, depIndex, depIndex, visitedTasks)) {
            return 1;
        }
    }
    
    // Unmark dependentTaskId in visitedTasks to reset the path for subsequent checks
    visitedTasks[dependentTaskId] = 0;

    return 0; // No circular dependency
}

/*
* This function finds the longest critical path starting from a given task.
* It uses recursion to explore all dependencies and track the longest path.
*
* Input:
* Task tasks[]: the array of Task structures, which contains the name of the task
* the start and end month of the task, number of dependencies of that task and the tasks it depends on.
* int current: Current task index being processed
* int numOfTasks: Total number of tasks
* int currentPath[]: Array to store the current path of task indices
* int chainLength: Path length of the current path
* int *maxLength: Pointer to store the maximum path length found
* int bestPath[]: Array to store the longest path found
* int visited[]: Array to track visited tasks to prevent cycles.
*
* There is no output.
*/
void findCriticalPath(Task tasks[], int current, int numOfTasks, int currentPath[], int chainLength, int *maxLength, int bestPath[], int visited[]){
    //check if this task has already been visited to prevent infinite recursion
    if (visited[current]) {
        return; // abort this branch, if the cycle is detected
    }

    //mark the current task as visited
    visited[current] = 1;

    //add the curent task to the path and increase the chain length
    currentPath[chainLength] = current;
    chainLength++;

    //if the current chain is longer than the longest found so far, update the best path
    if (chainLength > *maxLength) {
        *maxLength = chainLength;
        for (int i = 0; i < chainLength; i++) {
            bestPath[i] = currentPath[i];
        }
    }

    // If this task has dependencies, recursiverly find the longest chain through them
    if (tasks[current].numDependencies > 0){
        for (int i = 0; i < tasks[current].numDependencies; i++){
            int dep = tasks[current].dependencies[i]; //dependency of the current task
            if (dep >= 0 && dep < numOfTasks) {
                findCriticalPath(tasks, dep, numOfTasks, currentPath, chainLength, maxLength, bestPath, visited);
            }
        }
    }

    // mark the task as not visited so that other paths can use it
    visited[current] = 0;
}

/*
 * This function tests and displays the critical path for a user-specified task.
 * It also checks for circular dependencies in the entire task list.
 *
 * Input:
 * Task tasks[]: Array of tasks
 * int numOfTasks: an integer that stores the number of tasks
 *
 * There is no output.
 */
 void testCriticalPath(Task tasks[], int numOfTasks){

    int startTask;
    //receives task number for critical path testing
    printf("Enter the starting task number (1 - %d) for critical path testing: ", numOfTasks);
    fflush(stdout);
    scanf("%d", &startTask);

    // check if the input is valid
    if ((startTask < 1) || (startTask > numOfTasks)){
        printf("Invalid task number. Please enter a value between 1 and %d.\n", numOfTasks);
        return;
    }

    startTask--; // decrement startTask in order to match it with the array's indices

    //create arrays to store paths and track visited tasks
    int currentPath[NO_OF_TASKS] = {0}; //store current path
    int bestPath[NO_OF_TASKS] = {0};    //store the longest path found
    int maxLength = 0;                  //store the value of longest dependency chain
    int visited[NO_OF_TASKS] = {0};     //store the visited tasks to detect cycles

    //call to recursive function to find the longest critical path
    findCriticalPath(tasks, startTask, numOfTasks, currentPath, 0, &maxLength, bestPath, visited); // 0 is chainLength

    //print the critical path
    printf("Critical Path: ");
    for (int i = 0; i < maxLength; i++) {
        printf("%s", tasks[bestPath[i]].name);
        if (i != maxLength - 1) {
            printf(" -> "); // Print arrows between tasks
        }
    }
    printf("\n");
    // Check for circular dependencies
    if (detectCircularDependency(tasks, numOfTasks)) {
        printf("!!! Circular Dependency Found !!!\n");
    } else {
        printf("No circular dependencies found. Critical path is valid.\n");
    }
}