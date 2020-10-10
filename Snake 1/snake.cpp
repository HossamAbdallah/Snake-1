#include <iostream>
#include <string>
#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include <time.h> 
#include <stdlib.h>

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
using namespace std;


int wiedth, hight, snakeLenght, score = 0, level = 10;
string name;
COORD snake[1000];
COORD food;
COORD screenSize;
bool FoodExist;
HANDLE screen;
bool GameOver =false ;
bool validfood;
enum DIRECTION
{
	LEFT,
	RIGHT,
	UP,
	DOWN,
};

DIRECTION dir;
/*void color()
{
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	int col = 12;

	// color your text in Windows console mode
	// colors are 0=black 1=blue 2=green and so on to 15=white  
	// colorattribute = foreground + background * 16
	// to get red text on yellow use 4 + 14*16 = 228
	// light red on yellow would be 12 + 14*16 = 236

	FlushConsoleInputBuffer(hConsole);
	SetConsoleTextAttribute(hConsole, col);

	//cout << "Color Text";

	//SetConsoleTextAttribute(hConsole, 15); //set back to black background and white text
}*/

void getScreenSize()
{
	screen = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;


	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	wiedth = csbi.srWindow.Right - csbi.srWindow.Left - 15;
	hight = csbi.srWindow.Bottom - csbi.srWindow.Top - 3;

}
void setPosition(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void getlevel()
{
	cout << "choose your level from 1 to 10  : ";
	cin >> level;
	if (level > 10)
	{
		level = 10;
	}
	else if (level < 1)
	{
		level = 1;
	}
}
int speedsnake()
{
	//return level * 100;
	return 1000 / level;

}
void drawBorder()
{
	//system("Color 4C"); 
	//SetConsoleTextAttribute(hConsole, 3);
	//	wiedth = 80, hight = 29;
	setPosition(1, 0);
	for (int i = 0; i < wiedth; i++) //up
	{
		cout << "-";
	}
	cout << endl;

	for (int i = 1; i < hight - 1; i++) //left
	{
		setPosition(0, i);
		cout << "|" << endl;

	}
	cout << endl;

	for (int i = 1; i < hight - 1; i++) //right
	{
		setPosition(wiedth - 1, i);
		cout << "|" << endl;

	}
	setPosition(0, hight - 1);
	for (int i = 0; i < wiedth; i++) // down
	{

		cout << "-";
	}
	cout << endl;
	
}

void initSnake()
{
	snakeLenght = 4;
	dir = RIGHT;                                                          /////// why green underline ??
	for (int i = 0; i < snakeLenght; i++)
	{
		if (i == 0)
		{
			snake[0].X = wiedth / 2;
			snake[0].Y = hight / 2;
			continue;
		}
		snake[i] = { snake[i - 1].X - 1,snake[i - 1].Y };
	}
}	

void printSnake()
{
	for (int i = 0; i < snakeLenght; i++)
	{
		setPosition(snake[i].X, snake[i].Y);
		if (i == 0)
		{
			cout << "H";
			continue;
		}
		else if (i == 1)
		{
			cout << "O";
			continue;
		}
		cout << "S";
	}
}
void clearSnake()
{
	for (int i = 0; i < snakeLenght; i++)
	{
		setPosition(snake[i].X, snake[i].Y);      ////////
		cout << " ";

	}

}
void ShowConsoleCursor(bool showFlag)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO     cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
}


void addFood()
{
	if (FoodExist == true) {
		return;
	}
	 validfood = false;
	while (validfood == false)
	{
		srand((unsigned)time(0));

		food.X = (rand() % (wiedth - 2)) + 1;
		food.Y = (rand() % (hight - 2)) + 1;
		for (int i = 0; i < snakeLenght; i++)
		{
			if (food.X == snake[i].X && food.Y == snake[i].Y) {

				validfood = false;
				return;
			}
			else
				validfood = true;
		}
	}
	setPosition(food.X, food.Y);
	cout << "+";
	FoodExist = true;
}
void checkUserInput()
{
	if (_kbhit())
	{
		_getch;
		switch (_getch())
		{
		case 'a':

			if (dir != RIGHT)
			{
				dir = LEFT;

				break;
			}

		case 'd':
			if (dir != LEFT)
			{
				dir = RIGHT;
				break;
			}

		case  'w':
			if (dir != DOWN)
			{
				dir = UP;
				break;
			}

		case 's':
			if (dir != UP)
			{
				dir = DOWN;
				break;
			}

		}
	}

	
	if (_kbhit())
	{
		_getch;
		switch (_getch())  ////
		{
		case KEY_LEFT:

			if (dir != RIGHT)
			{
				dir = LEFT;

				break;
			}

		case KEY_RIGHT:
			if (dir != LEFT)
			{
				dir = RIGHT;
				break;
			}

		case  KEY_UP :
			if (dir != DOWN)
			{
				dir = UP;
				break;
			}

		case  KEY_DOWN:
			if (dir != UP)
			{
				dir = DOWN;
				break;
			}

		}
	}
}

void checkBorderCollision();
void snakeNextMove()
{
	
	clearSnake();
	
	checkBorderCollision();
	
	COORD temp, prevPoint;
	prevPoint = snake[0];
	switch (dir)
	{
	case LEFT:
		snake[0] = { snake[0].X - 1,snake[0].Y };
		break;
	case RIGHT:
		snake[0] = { snake[0].X + 1,snake[0].Y };
		break;
	case UP:
		snake[0] = { snake[0].X ,snake[0].Y - 1 };
		break;
	case DOWN:
		snake[0] = { snake[0].X ,snake[0].Y + 1 };
		break;
	default:
		break;

		
	}
	
	for (int i = 1; i < snakeLenght; i++)
	{
        temp = snake[i];
		snake[i] = prevPoint;
		prevPoint = temp;
		
	}
	
	
}
void clearScreen()
{
	system("cls");
}
void endGame()
{
	setPosition(wiedth / 2 - 4, 2);
	cout << "Game Over" << endl;
	cout << "Your score : " << score<< endl << endl;

}



void checkBorderCollision()
{
	if (snake[0].X <1 )              //left             // || snake[0].X>wiedth - 2 || snake[0].Y<1 || snake[0].Y>hight - 2)
	{
		snake[0].X = wiedth - 2;
	}
	else if (snake[0].X > wiedth - 2)//right
	{
		snake[0].X = 1;	
	}
	else if (snake[0].Y<=0)           //up
	{
		snake[0].Y = hight - 2;
	}	
	else if(snake[0].Y > hight -2)    //down
	{
		snake[0].Y = 1;
	}
}
void checkFoodEat()
{
	if ((snake[0].X == food.X) && (snake[0].Y == food.Y))
	{
		FoodExist = false;
		++snakeLenght;
		return; 
		
	}
	
}
int increasingScore()
{
	
	if (food.X == snake[0].X && food.Y == snake[0].Y)          //??????????????????
		score += 10;
	setPosition(5, hight );
	cout << "score : " << score;   
	return score;
}

void getUsername()
{
	cout << " Enter name ";
	cin >> name;
}
char playAgain()
{
	char pAgain;
	
	cout << "Do you want to play again y/n ?? ";
	_getch();
	cin >> pAgain;
	if (pAgain == 'y')
	{
		GameOver = false;
	}
	return pAgain;

}
void checkSelfCollision()                              //////////////////////////
{

	for (int i = 1; i < snakeLenght; i++)
	{


		if (snake[0].X == snake[i].X && snake[0].Y == snake[i].Y)
		{
			GameOver = true;
			clearScreen();
			endGame();
			playAgain();
			getlevel();
			clearScreen();
			score = 0;
			initSnake();
			printSnake();
			drawBorder();
			increasingScore();
			validfood = false;
			FoodExist = false;
			addFood();
			checkFoodEat();
		}

	}

}
int main()
{
	//getUsername();
	ShowConsoleCursor(false);
	getScreenSize();
	//getlevel();
	drawBorder();

	//initSnake();
	printSnake();
	
	while (GameOver==false)
	{

		checkUserInput();
		addFood();
		//checkBorderCollision();
		snakeNextMove();
		

		checkFoodEat();
		increasingScore();
		printSnake();
		checkSelfCollision();
		
		
		Sleep(speedsnake());
	}
	
	system ("pause");
	return 0;
}
