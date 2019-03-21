/*
 Name: Yiheng Cen Feng
 Course: CSCI 235
 Assignment: Project 4
 Implementation file for a class that finds a solution to a maze read from an input file and prints a solution to the standard output
*/

#include "MazeSolver.h"

//constructor
//pre: input file is in correct format with first two values being integers
//      followed by valid maze characters in {'_', '*','$'}
//post: if inuput file cannot be read outputs "Cannot read from input_file"
//      otherwise sets maze_rows_ and maze_columns_ from the first two values in input file
//      and allocates two 2-dimesional array of size maze_rows_ and maze_columns_
//      both maze_ and solution_ are initialized with characters read from input
MazeSolver::MazeSolver(std::string input_file){
    std::ifstream inFile;
    inFile.open(input_file);
    
    if(inFile.fail()){
        std::cerr << "Cannot read from " << input_file << std::endl;
        exit(1);
    }
    
    inFile>>maze_rows_;
    inFile>>maze_columns_;
    
    initializeMaze(maze_rows_, maze_columns_);
    fillMaze(inFile);
    initializeSolution();
    maze_ready = true;
    
    inFile.close();
}

//pre: rows and columns are positive integers
//post: allocates maze_ with rows and columns
//called by constructor
void MazeSolver::initializeMaze(int rows, int columns){
    maze_ = new char *[rows];
    for(int i = 0; i < rows; i++){
        maze_[i] = new char[columns];
    }
    
}

//pre: maze_ has been allocated with the correct number of rows and columns read from input file
//post: fills in maze_ with characters read from input file
//called by constructor
void MazeSolver::fillMaze(std::ifstream& input_stream){
    char x;
    for(int row = 0; row < maze_rows_ ; row++){
        for(int col = 0; col < maze_columns_ ; col++){
            input_stream>>x;
            maze_[row][col] = x;
        }
    }
        
}

//pre: maze_ has been initialized with valid character values in {'_', '*','$'}
//     start position is always [0][0]
//post: initializes solution_ with a copy of maze_
//      initializes backtrack_stack_  with all viable paths from position [0][0]
//      and mark the current position as visited ( '>' ) on solution_
//called by constructor
void MazeSolver::initializeSolution(){
    solution_ = new char *[maze_rows_];
    for(int i = 0; i < maze_rows_; i++){
        solution_[i] = new char[maze_columns_];
    }
    
    copyMazetoSolution();
    
    solution_[0][0] = '>';
    
    Position current_position;
    current_position.row = 0;
    current_position.column = 0;
    backtrack_stack_.push(current_position);
}

//pre: maze_ has been properly initialized
//post: allocates solution_ to the correct number of rows and columns
//      and copies the contents of maze_ into solution_
//called by initializeSolution()
void MazeSolver::copyMazetoSolution(){
    for(int row = 0; row < maze_rows_ ; row++){
        for(int col = 0; col < maze_columns_ ; col++){
            solution_[row][col] = maze_[row][col];
        }
    }
}

//return: true if maze has been initialized, false otherwise
bool MazeSolver::mazeIsReady(){
    return maze_ready;
}

//pre: maze_ has been initialized with valid character values in {'_', '*','$'}
//post: solution_ has been marked with '>' signs along the path to the exit
//      prints "Found the exit!!!" if exit is found
//      or "This maze has no solution." if no exit could be found
//return: true if exit is found, false otherwise
bool MazeSolver::solveMaze(){
    Position current_position;
    current_position = backtrack_stack_.top();
    while(!backtrack_stack_.empty()){
        if(maze_[current_position.row][current_position.column] == '$'){
            std::cout<<"Found the exit!!!"<<std::endl;
            return true;
        }
        else if(extendPath(current_position) == true){
            solution_[current_position.row][current_position.column] = '>';
            current_position = backtrack_stack_.top();
        }
        else{
            maze_[current_position.row][current_position.column] = 'x';
            solution_[current_position.row][current_position.column] = '@';
            backtrack_stack_.pop();
            if(!backtrack_stack_.empty()){
                current_position = backtrack_stack_.top();
            }
            else{
                std::cout<<"This maze has no solution."<<std::endl;
                return false;
            }
        }
    }
    return false;
}

//pre: current_position is a valid position on the maze_
//post: adds all positions extensible from current_position to backtrack_stack_
//return: true if path was extended, false otherwise
//called by solveMaze()
bool MazeSolver::extendPath(Position current_position){
    bool extended = false;
    
    if(isExtensible(current_position, SOUTH) == true){
        backtrack_stack_.push(getNewPosition(current_position, SOUTH));
        extended = true;
    }
       
    if(isExtensible(current_position, EAST) == true){
        backtrack_stack_.push(getNewPosition(current_position, EAST));
        extended = true;
    }
   
    return extended;
}

//pre: old_position is a Position initialized with row and column to valid positions in maze_ and it is extensible in direction dir
//return: a new Position on the maze moving in direction dir from old_position
//called by extendPath()
Position MazeSolver::getNewPosition(Position old_position, direction dir){
    Position new_position;
    if(dir == SOUTH){
        new_position.row = ((old_position.row)+1);
        new_position.column = old_position.column;
    }
    else{
        new_position.row = old_position.row;
        new_position.column = ((old_position.column)+1);
        
    }
    return new_position;
}

//checks if the path can be extended in maze_ from position current_position in direction dir
//return: true if path can be extended given current_position and dir, false otherwise
//called by extendPath
bool MazeSolver::isExtensible(Position current_position, direction dir){
    if((dir == SOUTH)&&((current_position.row+1) < maze_rows_)){
        if((maze_[(current_position.row)+1][current_position.column] == '_') || (maze_[(current_position.row)+1][current_position.column] == '$')){
            return true;
        }
    }
    else{
        if((current_position.column+1) < maze_columns_){
            if((maze_[current_position.row][(current_position.column)+1] == '_') ||  (maze_[current_position.row][(current_position.column)+1] == '$')){
                return true;
            }
        }
    }
    return false;
}

//post: prints the solution to the standard output stream
//      with single space character between each maze character
//      and each maze row on a new line
void MazeSolver::printSolution(){
    std:: cout<<"The solution to this maze is:"<<std::endl;
    for(int row = 0; row < maze_rows_; row++){
        for(int col = 0; col < maze_columns_; col++){
            std::cout<<solution_[row][col]<<" ";
        }
        std::cout<<std::endl;
    }
}

// destructor
// post: deletes maze_ and solution_
MazeSolver::~MazeSolver(){
    if(mazeIsReady()==1){
        for(int i = 0; i < maze_rows_; i++){
            delete [] maze_[i];
            delete [] solution_[i];
        }
        delete [] maze_;
        delete [] solution_;
    }
}

