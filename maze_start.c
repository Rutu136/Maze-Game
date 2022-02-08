/** 
 * COMP 2160 SECTION[A1]
 * INSTRUCTOR : [Mehdi Niknam]
 * NAME       : [RUTUKUMAR BARVALIYA]
 * ASSIGNMENT : [Assignment 2]
 * QUESTION   : [ solving a maze]
 * 
 * PURPOSE    : [ We are doing backtracking and path-finding in this assignment 
 *                to solve the maze problem and get the mouse at the right place ]
 * 
 **/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

//-------------------------------------------------------------------------------------
// CONSTANTS and TYPES
//-------------------------------------------------------------------------------------

#define MAX_DIMENSION 20

// constant definitions for the different cell states
const char WALL    = '1';
const char SPACE   = '0';
const char VISITED = '.';
const char MOUSE   = 'm';
const char EXIT    = 'e';

typedef enum BOOL { false, true } Boolean;

struct CELL
{
  int row;
  int column;
};
typedef struct CELL Cell;

typedef struct CELL_NODE CellNode;
struct CELL_NODE
{
  Cell     cell;
  CellNode *next;
};

//-------------------------------------------------------------------------------------
// VARIABLES
//-------------------------------------------------------------------------------------

CellNode *top = NULL;

// a 2D array used to store the maze
char maze[MAX_DIMENSION][MAX_DIMENSION];
int mazeRows;
int mazeCols;

// holds the location of the mouse and escape hatch
Cell mouse;
Cell escape;

//-------------------------------------------------------------------------------------
// PROTOTYPES
//-------------------------------------------------------------------------------------
// basic cell manipulation

// returns true if the cells are at the same position in our maze
Boolean equalCells(const Cell cell1, const Cell cell2);
// returns a new cell object
Cell makeCell(const int row, const int col);
// returns true if the cell is within our maze
Boolean validCell(const Cell theCell);

// routines for managing our backtracking

// returns true if there are no more cells to try
Boolean noMoreCells();
// returns the next cell to try for a path out of the maze
Cell nextCell();
// introduces a new cell to try
void addCell(const Cell cell);

void printMaze();
void loadMaze();

// returns true if there's a solution to the maze
Boolean solveMaze();

// our invariant checker
void checkState();

//-------------------------------------------------------------------------------------
// FUNCTIONS
//-------------------------------------------------------------------------------------

int main( int argc, char *argv[] )
{   
    loadMaze();      

    if ( solveMaze() )
      printf( "The mouse is free!!!!\n" );
    else
      printf( "The mouse is trapped!!!!\n" );
    
    printf( "\nEnd of processing\n" );
  return EXIT_SUCCESS;
}


//////////////////////////////////////////////
// Cell routines
//////////////////////////////////////////////

/**
 * check the cell
 * check the whole map bounds
 * if size of row or col then return false
 */
Boolean validCell(const Cell theCell){
    
    //checking the conditions
    if((theCell.row < mazeRows)&&(theCell.column < mazeCols))
    {
        return true;
    }//end if
    else
    {
        return false;
    }//end else
}//end validCell

//we are just initialize the cell here in the map
//and return the cell it self.
Cell makeCell(const int row, const int col){
    
    Cell temp ;
    //assign the row 
    temp.row = row;
    //assign the col
    temp.column=col;
    
    return temp;
}//end makeCell

/**
 * 
 * this method will be helpful later in code
 * that to check the mouse and escape point should
 * match or not.
 * if they are matching then we are successful solve the maze. 
 */
Boolean equalCells(const Cell cell1, const Cell cell2){ //-----------
    
    //check the validation first.
    if(validCell(cell1)&&validCell(cell2))
    {
        //if row and column getting at the same point then return false.
        if(cell1.row == cell2.row && cell1.column == cell2.column)
        {
            return false;
        }//end if
        else 
        {
            return true;
        }//end else
    }//end if
    else
    {
      return false;
    }//end else
}//end equalCells

//checking that the map and list is empty or not.!!!
Boolean noMoreCells(){

    return (NULL ==top);
}//end noMoreCell

//////////////////////////////////////////////
// List routines
//////////////////////////////////////////////

//this method will add the cell to the map
void addCell(const Cell cell){

    //create a temp node to allocate the memory
    CellNode *temp=(CellNode*) malloc(sizeof(CellNode));

    if(temp != NULL)
    {
        if(top == NULL)
        {
            temp ->cell = cell;
            temp ->next = NULL;
            top = temp;
        }//end if
        else
        {
            temp ->cell = cell;
            temp ->next = top;
            top = temp;
        }//end else
    }//end if 
    else
    {
        temp = NULL;
        free(temp);
    }//end else

}//end addCell

//to get move onto the next cell
Cell nextCell() {

    Cell newCell ;
    
    newCell = top->cell;
    top = top->next;
    
    return  newCell;       
}//end nextCell

//////////////////////////////////////////////
// Maze routines
//////////////////////////////////////////////


//------------------------------------------------------
// myRoutine
//
// PURPOSE: It takes the input line by line and then process it by tokenizing it
// (You can change the file name in fptr to check for other files)
// INPUT PARAMETERS: None
// Describe those parameters which accept data values.
// OUTPUT PARAMETERS: mazeRows, mazeCols , and the maze itself
// Describe those parameters which return values.
// none
// Include the value returned by the routine if not void.
// Some of these may be the same as the input
// parameters.
//------------------------------------------------------

//to get input from the user 
//it will load the maze txt file
void loadMaze()
{
    char line[1000];
    char* tok;
    int i=0;
    int row=0;
    int col;
    while(fgets(line,1000,stdin)!=NULL )
    {
        col=0;
        tok = strtok(line, " ");
        while(tok!=NULL && i==1)
        {
            maze[row][col]= *tok;
            tok=strtok(NULL, " ");
            col++;
        }
        row++;
        while(tok!=NULL && i!=1)
        {
            mazeRows = atoi(tok);
            tok=strtok(NULL, " ");
            mazeCols = atoi(tok);
            i=1;
            row=0;
        }
    }
    
    for(int i=0;i<mazeRows;i++)
    {
        for(int j=0;j<mazeCols;j++)
        {
            if(maze[i][j] == MOUSE)
            {
                mouse = makeCell(i,j);
            }//if
            else if(maze[i][j] == EXIT)
            {
                escape = makeCell(i,j);
            }//else if
        }//for
    }//for
    assert(validCell(mouse));
    assert(validCell(escape));
}

//for printing the maze
void printMaze(){
printf("\n");
    for(int i=0;i<mazeRows;i++)
    {
        for(int j=0;j<mazeCols;j++)
        {
            printf("%c",maze[i][j]);
        }//for
        printf("\n");
    }//for
printf("\n");
}//end printMaze

//getting the path and backtrack algorithm
Boolean solveMaze(){

    Cell curr = mouse;
    printMaze();
    
    checkState();

    while(equalCells(curr,escape))
    {
    
        maze[curr.row][curr.column] = VISITED;
        printMaze();
        
        if(maze[curr.row+1][curr.column]==SPACE || maze[curr.row+1][curr.column] == EXIT)
        {
            addCell(makeCell(curr.row+1,curr.column));
        }//end if
        if(maze[curr.row][curr.column+1] == SPACE || maze[curr.row][curr.column+1] == EXIT)
        {
            addCell(makeCell(curr.row,curr.column+1));
        }//end if
        if(maze[curr.row-1][curr.column] == SPACE || maze[curr.row-1][curr.column] == EXIT)
        {
            addCell(makeCell(curr.row-1,curr.column));
        }//end if
        if(maze[curr.row][curr.column-1] == SPACE  || maze[curr.row][curr.column-1] == EXIT)
        {
            addCell(makeCell(curr.row,curr.column-1));
        }//end if
        
        if(noMoreCells())
        {
           printf("the mouse is trapped: we tried all routes and failed to find an escape");
           return false;
        }//end if 
        else
        {         
            curr = nextCell(); 
        }//end else
       
    }//end while
    
    return true;
}   

/*--------------------------------------------------
 * assertion routine
 * ------------------------------------------------- 
 */

void checkState()
{
    assert(mazeRows < MAX_DIMENSION || mazeCols < MAX_DIMENSION);
}
