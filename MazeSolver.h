//
//  MazeSolver.h
//
//  Maze:
//  This class finds a solution to a maze read from an input file
//  and prints a solution to the standard output
//
//  Created by Tiziana Ligorio on 10/1/18.
//  Copyright © 2018 Tiziana Ligorio. All rights reserved.
//

#ifndef MAZE_SOLVER_H_
#define MAZE_SOLVER_H_

#include <string>
#include <fstream>
#include <iostream>
#include<array>
#include<stack>

enum direction {SOUTH, EAST};

struct Position
{
    int row;
    int column;
};

class MazeSolver
{
public:
    
    //constructor
    //pre: input file is in correct format with first two values being integers
    //      followed by valid maze characters in {'_', '*','$'}
    //post: if inuput file cannot be read outputs "Cannot read from input_file"
    //      otherwise sets maze_rows_ and maze_columns_ from the first two values in input file
    //      and allocates two 2-dimesional array of size maze_rows_ and maze_columns_
    //      both maze_ and solution_ are initialized with characters read from input
    MazeSolver(std::string input_file);
    
    // destructor
    //post: deletes maze_ and solution_
    ~MazeSolver();
    
    
    //return: true if maze has been initialized, false otherwise
    bool mazeIsReady();
    
    //pre: maze_ has been initialized with valid character values in {'_', '*','$'}
    //post: solution_ has been marked with '>' signs along the path to the exit
    //      prints "Found the exit!!!" if exit is found
    //      or "This maze has no solution." if no exit could be found
    //return: true if exit is found, false otherwise
    bool solveMaze();
    
   
    //post: prints the solution to the standard output stream
    //      with single space character between each maze character
    //      and each maze row on a new line
    void printSolution();
    

private:
  
    //PRIVATE DATA MEMBERS:
    
    int maze_rows_ = 0;             //the number of rows as read from input file
    int maze_columns_ = 0;          //the number of columns as read from input file
    bool maze_ready = false;        //indicates whether the maze has been initialized from input file
    char** maze_ = nullptr;       //a 2-d character array containing maze characters read from input file
    char** solution_ = nullptr;   //a 2-d character array containing maze characters copied from maze_
                                // and path to exit marked with '>' characters and position backtracked from marked with '@'characters
    std::stack<Position> backtrack_stack_;  //stack used for backtracking
    
    
    //PRIVATE MEMBER FUNCTIONS (helper functions)
    
    //pre: rows and columns are positive integers
    //post: allocates maze_ with rows and columns
    //called by constructor
    void initializeMaze(int rows, int columns);
    
    //pre: maze_ has been allocated with the correct number of rows and columns read from input file
    //post: fills in maze_ with characters read from input file
    //called by constructor
    void fillMaze(std::ifstream& input_stream);
    
    //pre: maze_ has been initialized with valid character values in {'_', '*','$'}
    //     start position is always [0][0]
    //post: initializes solution_ with a copy of maze_
    //      initializes backtrack_stack_  with all viable paths from position [0][0]
    //      and mark the current position as visited ( '>' ) on solution_
    //called by constructor
    void initializeSolution();
    
    //pre: maze_ has been properly initialized
    //post: allocates solution_ to the correct number of rows and columns
    //      and copies the contents of maze_ into solution_
    //called by initializeSolution()
    void  copyMazetoSolution();
    
    //pre: current_position is a valid position on the maze_
    //post: adds all positions extensible from current_position to backtrack_stack_
    //return: true if path was extended, false otherwise
    //called by solveMaze()
    bool extendPath(Position current_position);
    
    //pre: old_position is a Position initialized with row and column to valid positions in maze_ and it is extensible in direction dir
    //return: a new Position on the maze moving in direction dir from old_position
    //called by extendPath()
    Position getNewPosition(Position old_position, direction dir);
    
    //checks if the path can be extended in maze_ from position current_position in direction dir
    //return: true if path can be extended given current_position and dir, false otherwise
    //called by extendPath
    bool isExtensible(Position current_position, direction dir);
    
}; // end MazeSolver

#endif /* MAZE_SOLVER_H_ */
