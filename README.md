# WaterSort-Solver
## How to run the program
In the project root directory
1. Compile waterSort.cpp
```
g++ waterSort.cpp -o waterSort
```
2. Execute the compiled file
```
./waterSort 4 3 true
```
The above command will make the program random generate a 4 tubes graph with 3 tubes filled with water, and the third argument will decide to print or not to print the solving steps.

## Demo
![image](https://user-images.githubusercontent.com/35865970/156897890-fffbe8bc-f58e-4512-92d3-853909fd6cec.png)


## Problem Specification
```
Instance: Integer “n” for number of colors and Integer “m” for number of tubes, with n < m. A 4 * m matrix in which columns denote tubes and each entry stores its corresponding color, and the values of the last two columns are all set to zero to denote that they are empty. 
Solution Format:  A Boolean value which denotes whether the water sort puzzle is solvable
Constraints:
     We define that xTop is the index of x’s top color entry, i.e., x_0=...=x_(xTop-1)= blank and x_xTop!=blank.
     We define that xRemain is the remaining capacity of x, i.e., If x is empty, xRemain=4. If x is full, xRemain=0. Otherwise, xRemain will be the maximum number which fulfills that x_0=...=x_(xRemain-1)=blank and x_xRemain!=blank.
     We define that xTopConsecutive is the maximum amount of top consecutive water of x, i.e., xTopConsecutive is the maximum number such that x_xTop=...=x_(xTop+xTopConsecutive-1).
Can only move water from tube u to tube v if u_top=v_top or v is empty, and the amount of water of the movement will be equal to min(uTopConsecutive, vRemain).
Objective: To solve the water sort puzzle if it is solvable. 
```
## Algorithm Solution
```
We define “state” as a snapshot of the water distribution in tubes.
     Description: 
    We use backtracking and memorization to solve this problem. We will use a set to store all the states we have met before and check whether the state is the final desired state or there is a next state which will lead to the desired final state recursively. If the next state has been visited, we will not solve it but think of it as not being able to reach the desired final state. If the next state hasn’t been visited, we will check it recursively. If all of the next states are not able to reach the desired final states, we will return false. Otherwise, we will return true.
```
