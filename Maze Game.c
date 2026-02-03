/*

Tyler Hansen
115069679
Final Project

*/

#include <conio.h>			//needed for '_getch()'
#include <stdio.h>			//needed for more operations in our code
#include <stdlib.h>			//needed for 'rand()' and 'srand()'
#include <time.h>			//needed for 'time()' and 'difftime()'

//call all of our functions at the top so that we can use them in the main code
void initializeMaze();		//load the maze from an input file
void displayMaze();			//display the maze
void placePointRandomly();	//get a random posititon for the points
void placeAntRandomly();	//set Lily’s random starting position
void moveAnt(int move); 	//move Lily based on arrow key input and set the time
void displayInfo(time_t start_t, time_t end_t);			//this will allow us to display the current amount of time left along with the amount of points you currently have

//initialize all the variables globally so that we can use them in all of our functions without having to constantly initilize them
int rows = 16, cols = 16, i, j;
char maze[16][16];
int cur_r, cur_c;
int point_r, point_c;
int points = 0;
int *x = &cur_r;
int *y = &cur_c;

int main()
{	
	//call to our maze so that we can print it out 
	initializeMaze();
	displayMaze();
	
	//prompt the user to start the game
	printf("\nYou are the 'X'");
	printf("\nPress any key to begin");
	printf("\nYour starting location will randomly generate");
	
	//seed the random number generator
	srand(time(NULL));
	
	//start the timer
	time_t start_t, end_t;
	time(&start_t);
	
	//this allows us to randomly place the ant each time we run the code
	//if this was in the initialzeMaze function, it would spawn in the same place each time
	placeAntRandomly();
	
	//this loop will run so that the game will run until the time limit is over
    while (difftime(end_t, start_t) < 60.0)
    {
        // this will get the input from the user and move the ant for the given time
        char c = _getch();
        moveAnt(c);
        time(&end_t);

        //this will display the information we desire from the function we created that is defined below
        displayInfo(start_t, end_t);
    }
    
    //60 seconds is up so the game is over and we will display this and show the total points
    printf("\e[1;H\e[2J");
    printf("\n\nGAME OVER!!\n");
    printf("Total Points: %d", points);

    //prompt user to press # to end the program
    //I added this potion of the code so that the user does not accidently close the program after the 60 seconds
    printf("\n\nPress '#' to end the program\n");

	//thi loop will run so that we make sure the user presses '#' to end the program
    while (1)
    {
        char c = _getch();
        
        if (c == '#')
        {
            break;
        }
    }

    return 0;
}

//load the maze from an input file named maze.txt and place it into a 2D array so we can display it
void initializeMaze()
{
	//read from the maze file
    FILE *f = fopen("maze.txt", "r");
    if (f == NULL)
    {
        printf("Cannot open file");
        exit(1);
    }

    //read the maze from the file
    for (cur_r = 0; cur_r < rows; cur_r++)
    {
        for (cur_c = 0; cur_c < cols; cur_c++)
        {
           fscanf(f, "%c", &(maze)[cur_r][cur_c]);
        }
    }

	//close the file after opening it
    fclose(f);
}

//this will allow us to update and display the maze
void displayMaze()
{
	for(i = 0; i < rows; ++i)
	{
		for(j = 0; j < cols; ++j)
		{
			printf("%c", maze[i][j]);
		}
	}
}

//this will randomly place Lily into the maze
void placeAntRandomly()
{	
    do 
    {
    	//we use the pointers and rand function so that we can store it into memory
        *x = rand() % rows;
        *y = rand() % cols;
    }
    while (maze[*x][*y] != ' ');

	//this will print out Lily into the place we just randomly generated
    maze[*x][*y] = ':)';
}

//this will randomly place a point into the maze
void placePointRandomly()
{
	do
	{
		//this will allow us to randomly place a point value into the maze so we can collect it
		point_r = rand() % (rows);
		point_c = rand() % (cols);
	}
	
	while(maze[point_r][point_c] != ' ');
	
	//this will allow us to print out numbers between 1 and 9 since no one wants to collect 0 points
	maze[point_r][point_c] = '1' + rand() % 9;
}

//this will allow us to move Lily throughout the maze and collect points for the 60 seconds
void moveAnt(int move)
{
	//this will allow us to replace a new position with a current position
	int new_r = cur_r;
	int new_c = cur_c;
	
	//get the arrow key press
	switch(move)
	{
		//moving up
		//we make it new_r-- since we are moving up which is technically a lesser value than if we were to move down
		case 72:
		case 'w':
			new_r--;
			break;
		
		//moving down
		//we make it new_r++ since we are moving down which is technically a greater value than if we were to move up
		case 80:
		case 's':
			new_r++;
			break;
		
		//moving right
		case 77:
		case 'd':
			new_c++;
			break;
			
		//moving left
		case 75:
		case 'a':
			new_c--;
			break;
		
		//if anything else is pressed, it won't do anything
		default:
			break;
	}
	
	//makes sure that we are within the bounds of the maze that we created
	if(new_r >= 0 && new_r < rows && new_c >= 0 && new_c < cols && maze[new_r][new_c] != '*')
	{
		//now we make another if statement so that we can see if what we are moving into is a point
		if(maze[new_r][new_c] >= '0' && maze[new_r][new_c] <= '9')
		{
			//collects the points and adds it to our point count
			int collectedPoints = maze[new_r][new_c] - '0';
			points += collectedPoints;
			
			//this will generate new points after we collect the given points
			placePointRandomly();
			
			//clear the space that we just collected points from
			maze[new_r][new_c] = ' ';
		}
		
		//tnis will allow us to clear the old position and update it with the new position
		maze[cur_r][cur_c] = ' ';
		cur_r = new_r;
		cur_c = new_c;
		maze[cur_r][cur_c] = 'X';
		
		//clear the console before we display the new maze
		printf("\e[1;H\e[2J");
		displayMaze(maze, new_r, new_c);
	}
}

//this function will allow us to display the amount of time the user has left and how many points they have collected so far
void displayInfo(time_t start_t, time_t end_t)
{
	printf("\n\nPoints: %d\n", points);
	printf("Time Remaining: %.2f seconds\n", 60 - difftime(end_t, start_t));
}