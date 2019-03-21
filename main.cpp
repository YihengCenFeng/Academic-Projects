#include <iostream>
#include "MazeSolver.h"
int main() {

 MazeSolver solver("inputA.txt");

 if(solver.mazeIsReady())
 {
 solver.solveMaze();

 solver.printSolution();
 }

 return 0;
}
