Introduction
This program is a command-line Gantt chart generator written in C.
It allows users to:
Create, edit, and visualise tasks.
Detect circular dependencies.
Calculate the critical path for a project.
The program adheres to the assignment requirements, including the use of structs, enums, recursion, and Git for version control.
This README provides:
An explanation of the code structure.
The purpose and functionality of each function.
How recursion is applied for circular dependency detection and critical path calculation.
How the program meets assignment specifications.
🛠️ Code Structure
1. Structs and Enums
Task Struct
Stores all relevant information about a task:
Task Name — string holding the name.
Start & End Months — integers from 1–12 (January–December).
Dependencies — array of indices for dependent tasks.
Number of Dependencies — integer tracking dependency count.
This struct is central to creating, editing, and displaying tasks.
Enums
The program uses an enum for months (JAN = 1, FEB = 2, ... DEC = 12) to improve readability.
Preloaded Data: Test tasks use the Month enum for clarity.
User Input: Months are handled as numbers 1–12 for simplicity.
2. Key Functions
printAsciiArt
Displays motivational ASCII art spelling “Success” with six "S" shapes.
Symbolism: “Six Cess” → success through teamwork & effort.
welcomeScreen
Acts as the main menu.
Options:
Create a custom Gantt chart.
Use preloaded test tasks.
Edit tasks.
Detect circular dependencies.
Quit program.
Directs users to createGantt or loadTestTasks based on choice.
loadTestTasks
Preloads 10 sample tasks with names, months, and dependencies.
Simulates real-world project data.
Useful for quickly demonstrating functionality.
createGantt
Allows user to create a custom Gantt chart.
Prompts for number of tasks (up to 10).
For each task:
Task name.
Start and end months (validated).
Dependencies.
Detects circular dependencies with detectCircularDependency.
editTask
Lets user modify an existing task.
Update task name, months, or dependencies.
Validates inputs.
Reverts changes if circular dependency is introduced.
displayGantt
Displays tasks in a tabular format.
Columns for months January–December.
Each task shown with "XXX" for active months.
Dependencies listed alongside.
detectCircularDependency
Checks for cycles in dependencies.
Uses two arrays:
visited[] → tracks checked tasks.
recursionStack[] → tracks current recursion path.
Calls isCyclicUtil for cycle detection.
isCyclicUtil (helper)
Recursive function to detect circular dependencies.
Marks current task visited & adds to recursion stack.
Recursively checks dependencies.
If dependency already in stack → cycle detected.
checkCircDependency (alternative helper)
Another recursion-based cycle check.
Returns 1 if cycle detected, 0 otherwise.
Useful for verifying circular dependency logic.
findCriticalPath
Recursive function to find the longest dependency chain (critical path).
Tracks current path vs longest path.
Updates longest path if new longer one is found.
Uses recursion to explore all dependencies.
testCriticalPath
Allows user to check the critical path starting from a chosen task.
Prompts for starting task.
Calls findCriticalPath.
Displays longest chain of tasks.
Also checks for circular dependencies.
3. Recursive Logic
Circular Dependency Detection
Implemented via isCyclicUtil.
Recursively explores dependencies.
Detects cycles when a task appears twice in the recursion stack.
Critical Path Calculation
Implemented via findCriticalPath.
Recursively explores task dependencies.
Keeps track of longest dependency chain.
4. Test Cases
Preloaded tasks simulate a real-world project.
Includes dependencies to test both:
Circular dependency detection.
Critical path calculation.
 Conclusion
This program successfully implements a command-line Gantt chart generator with features for:
Creating and editing tasks.
Detecting circular dependencies.
Calculating the critical path.
Its modular design, clear interface, and error handling make it a reliable project management tool.
The extensive use of recursion highlights strong understanding of algorithms and data structures.
