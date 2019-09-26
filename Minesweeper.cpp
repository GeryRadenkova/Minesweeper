#include<iostream>
#include<stdlib.h> /* srand, rand */
#include<time.h>
#include<iomanip>
#include<Windows.h>

#undef max();

//Constants
const int BEGINNER_SIZE = 8;
const int INTERMEDIATE_SIZE = 12;
const int EXPERT_SIZE = 16;
const int BEGINNER_MINES = 5;
const int INTERMEDIATE_MINES = 15;
const int EXPERT_MINES = 20;
const int MAX_SIZE = 100;

//Prototypes
int** CreateMatrix(int n, int m);
char** CreateCharMatrix(int n, int m);
void FillMatrixWithMines(int** matrix, int mines, int size);
bool IsCoordinatesValid(int y, int x, int size);
void AddDigits(int** matrix, int x, int y, int size);
bool Open(int** matrix, int y, int x, int size, int &countPositionsWithoutMines, char** charMatrix);
void Play(int** matrix, int size, int mines, char** charMatrix);
void PrintMatrix(char** charMatrix, const int size, const int mine_counter);
void PrintMenu();
void clean(int** matrix, int n);
void cleanCharMatrix(char** matrix, int n);
void ClearScreen();
bool GotoXY(unsigned x, unsigned y);
int wherex();
int wherey();


int main() {

	srand(time(NULL));

	PrintMenu();

	int command;
	int** matrix;
	char** charMatrix;


	while (true) {

		std::cout << "Choose command> ";
		std::cin >> command;

		while (!std::cin && !(command >= 0 && command <= 5)) {

			std::cout << "Invalid command.\n";
			std::cout << "Choose command> ";
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cin >> command;
		}

		ClearScreen();

		if (command == 1) {

			matrix = CreateMatrix(BEGINNER_SIZE, BEGINNER_SIZE);
			charMatrix = CreateCharMatrix(BEGINNER_SIZE, BEGINNER_SIZE);
			FillMatrixWithMines(matrix, BEGINNER_MINES, BEGINNER_SIZE);
			PrintMatrix(charMatrix, BEGINNER_SIZE, BEGINNER_MINES);
			Play(matrix, BEGINNER_SIZE, BEGINNER_MINES, charMatrix);

			clean(matrix, BEGINNER_SIZE);
			cleanCharMatrix(charMatrix, BEGINNER_SIZE);

			std::cout << "\n[0] - menu, [5] - exit \n";
		}
		else if (command == 2) {

			matrix = CreateMatrix(INTERMEDIATE_SIZE, INTERMEDIATE_SIZE);
			charMatrix = CreateCharMatrix(INTERMEDIATE_SIZE, INTERMEDIATE_SIZE);
			FillMatrixWithMines(matrix, INTERMEDIATE_MINES, INTERMEDIATE_SIZE);
			PrintMatrix(charMatrix, INTERMEDIATE_SIZE, INTERMEDIATE_MINES);
			Play(matrix, INTERMEDIATE_SIZE, INTERMEDIATE_MINES, charMatrix);

			clean(matrix, INTERMEDIATE_SIZE);
			cleanCharMatrix(charMatrix, INTERMEDIATE_SIZE);

			std::cout << "\n[0] - menu, [5] - exit \n";
		}
		else if (command == 3) {

			matrix = CreateMatrix(EXPERT_SIZE, EXPERT_SIZE);
			charMatrix = CreateCharMatrix(EXPERT_SIZE, EXPERT_SIZE);
			FillMatrixWithMines(matrix, EXPERT_MINES, EXPERT_SIZE);
			PrintMatrix(charMatrix, EXPERT_SIZE, EXPERT_MINES);
			Play(matrix, EXPERT_SIZE, EXPERT_MINES, charMatrix);

			clean(matrix, EXPERT_SIZE);
			cleanCharMatrix(charMatrix, EXPERT_SIZE);

			std::cout << "\n[0] - menu, [5] - exit \n";
		}
		else if (command == 0) {

			PrintMenu();
		}
		else if (command == 4) {

			std::cout << "Minesweeper is a single-player puzzle game."
				<< "The objective of the game is to clear a rectangular board containing hidden \"mines\" or bombs "
				<< "without detonating any of them, with help from clues about the number of neighboring mines in each field."
				<< "The game originates from the 1960s, and has been written for many computing platforms in use today. "
				<< "It has many variations and offshoots.\n";

			std::cout << "\n[0] - menu, [5] - exit \n";
		}
		else if (command == 5) {
			break;
		}
	}

	return 0;
}

void FillMatrixWithMines(int** matrix, int mines, int size) {

	int firstCoordinate;
	int secondCoordinate;

	for (int i = 0; i < mines; i++) {

		do {
			firstCoordinate = rand() % size;
			secondCoordinate = rand() % size;
		} while (matrix[firstCoordinate][secondCoordinate] == 9);

		matrix[firstCoordinate][secondCoordinate] = 9;
		AddDigits(matrix, firstCoordinate, secondCoordinate, size);
	}
}

bool IsCoordinatesValid(int y, int x, int size) {
	return (y >= 0 && y < size && x >= 0 && x < size);
}

void AddDigits(int** matrix, int y, int x, int size) {

	for (int i = y - 1; i <= y + 1; i++) {
		for (int j = x - 1; j <= x + 1; j++) {

			if (IsCoordinatesValid(i, j, size) && matrix[i][j] != 9)
					matrix[i][j]+= 1;
			else 
				continue;
		}
	}
}

bool Open(int** matrix, int y, int x, int size, int &countPositionsWithoutMines, char** charMatrix) {

	//Console Coordinates
	int xCoordinate = 4; //cols
	int yCoordinate = 3; //rows

	if (IsCoordinatesValid(y, x, size) && charMatrix[y][x] == '#') {

		GotoXY(xCoordinate + (2 * x), yCoordinate + y);
		if (matrix[y][x] == 0) {

			std::cout << ' ';
			charMatrix[y][x] = ' ';
			countPositionsWithoutMines++;
		}
		else {

			std::cout << matrix[y][x];
			charMatrix[y][x] = ' ';
			countPositionsWithoutMines++;
		}
		
		if (matrix[y][x] == 0) {
			Open(matrix, y - 1, x - 1, size, countPositionsWithoutMines, charMatrix);
			Open(matrix, y - 1, x, size, countPositionsWithoutMines, charMatrix);
			Open(matrix, y - 1, x + 1, size, countPositionsWithoutMines, charMatrix);
			Open(matrix, y, x - 1, size, countPositionsWithoutMines, charMatrix);
			Open(matrix, y, x + 1, size, countPositionsWithoutMines, charMatrix);
			Open(matrix, y + 1, x - 1, size, countPositionsWithoutMines, charMatrix);
			Open(matrix, y + 1, x, size, countPositionsWithoutMines, charMatrix);
			Open(matrix, y + 1, x + 1, size, countPositionsWithoutMines, charMatrix);
		}
	}
	else if (IsCoordinatesValid(y, x, size) && charMatrix[y][x] == 'X') {
		return false;
	}

	return true;
}

void Play(int** matrix, int size, int mines, char** charMatrix) {

	std::cout << "\nCommand list:\n"
		<< "open y x\n"
		<< "mark y x\n"
		<< "unmark y x\n\n";

	int mines_counter = mines;
	int countPositionsWithoutMines = 0;

	//Console coordinates
	int xCoordinate = 4; //cols
	int yCoordinate = 3; //rows

	char buff[MAX_SIZE];
	int y, x;

	int _y, _x; //to save current console coordinates

	while (true) {

		std::cout << "command> ";
		std::cin >> std::setw(MAX_SIZE) >> buff;
		std::cin >> y >> x;
		y -= 1;  //to start from 1 in console
		x -= 1;

		while (!std::cin || !IsCoordinatesValid(x,y,size)) {
			std::cout << "Wrong index! Try again.\n\n";
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "command> ";
			std::cin >> std::setw(MAX_SIZE) >> buff;
			std::cin >> y >> x;
			y -= 1;
			x -= 1;
		}

		if (!strcmp(buff, "mark")) {

			_x = wherex();
			_y = wherey();

			if (mines_counter < 0) {

				std::cout << "You have marked all mines.\n";
				_x = wherex();
				_y = wherey();
			}
			else if (charMatrix[y][x] == ' ') {
				std::cout << "Can't mark this.\n";
				_x = wherex();
				_y = wherey();
			}
			else {

				GotoXY(xCoordinate + (2*x), yCoordinate + y);
				std::cout << 'X';
				charMatrix[y][x] = 'X';

				mines_counter--;
				GotoXY(7, 0);
				std::cout << mines_counter;
			}

			GotoXY(_x, _y);
		}
		else if (!strcmp(buff, "open")) {

			_x = wherex();
			_y = wherey();

			if (charMatrix[y][x] == ' ') {
				GotoXY(_x, _y);
				std::cout << "Already opened.\n";
				_x = wherex();
				_y = wherey();
			}
			else if (!Open(matrix, y, x, size, countPositionsWithoutMines, charMatrix)) {

				GotoXY(_x, _y);
				std::cout << "Can't open this.It is marked as a mine.\n";
				_x = wherex();
				_y = wherey();
			}
			else if (matrix[y][x] == 9) {

				GotoXY(xCoordinate + (2 * x), yCoordinate + y);
				std::cout << '@';
				GotoXY(_x, _y);
				std::cout << "Game Over\n";
				break;
			}
			
			GotoXY(_x, _y);
		}
		else if (!strcmp(buff, "unmark")) {

			_x = wherex();
			_y = wherey();

			if (charMatrix[y][x] == 'X') {

				GotoXY(xCoordinate + (2 * x), yCoordinate + y);
				std::cout << '#';
				charMatrix[y][x] = '#';

				mines_counter++;
				GotoXY(7, 0);
				std::cout << mines_counter;
			}
			else {
				std::cout << "It is not marked. Can't unmark it.\n";
				_x = wherex();
				_y = wherey();
			}

			GotoXY(_x, _y);
		}
		else {
			std::cout << "Wrong command!Try again.\n\n";
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}

		if (mines_counter == 0 && countPositionsWithoutMines == size * size - mines) {

			std::cout << "You win!\n";
			break;
		}
	}
}

void PrintMatrix(char** charMatrix, const int size, const int mine_counter) {

	std::cout << "MINES: ";
	GotoXY(7, 0);
	std::cout << mine_counter << "\n\n";

	std::cout << "    ";

	for (int i = 0; i < size; i++) {
		std::cout << i + 1 << ' ';
	}

	std::cout << std::endl;

	for (int i = 0; i < size; i++) {

		if (i > 8) 
			std::cout << i + 1 << "  ";
		else 
			std::cout << ' ' << i + 1 << "  ";

		for (int j = 0; j < size; j++) {
			if (j > 8)
				std::cout << charMatrix[i][j] << "  ";
			else
				std::cout << charMatrix[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

void PrintMenu() {

	std::cout << "  *Minesweeper Game*  \n";
	std::cout << "0 - Menu\n";
	std::cout << "1 - Beginner\n";
	std::cout << "2 - Intermediate\n";
	std::cout << "3 - Expert\n";
	std::cout << "4 - Rules\n";
	std::cout << "5 - Exit\n";
}

int** CreateMatrix(int n, int m) {

	int** matrix = new(std::nothrow) int*[n];
	if (!matrix)
		return NULL;

	for (int i = 0; i < n; i++) {
		matrix[i] = new(std::nothrow) int[m];
		if (!matrix[i]) {
			clean(matrix, i);
			return NULL;
		}
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			matrix[i][j] = 0;
		}
	}

	return matrix;
}

char** CreateCharMatrix(int n, int m) {

	char** matrix = new (std::nothrow) char*[n];
	if (!matrix) 
		return NULL;

	for (int i = 0; i < n; i++) {
		matrix[i] = new(std::nothrow) char[m + 1];
		if (!matrix[i]) {
			cleanCharMatrix(matrix, i);
			return NULL;
		}
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			matrix[i][j] = '#';
		}
		matrix[i][m] = '\0';
	}

	return matrix;
}

void clean(int** matrix, int n) {

	for (int i = n - 1; i >= 0; i--) {
		delete[] matrix[i];
	}
	delete[] matrix;
}

void cleanCharMatrix(char** matrix, int n) {

	for (int i = n - 1; i >= 0; i--) {
		delete[] matrix[i];
	}
	delete[] matrix;
}

void ClearScreen() {

	HANDLE                     hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD                      count;
	DWORD                      cellCount;
	COORD                      homeCoords = { 0, 0 };

	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) return;

	/* Get the number of cells in the current buffer */
	if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
	cellCount = csbi.dwSize.X *csbi.dwSize.Y;

	/* Fill the entire buffer with spaces */
	if (!FillConsoleOutputCharacter(
		hStdOut,
		(TCHAR) ' ',
		cellCount,
		homeCoords,
		&count
	)) return;

	/* Fill the entire buffer with the current colors and attributes */
	if (!FillConsoleOutputAttribute(
		hStdOut,
		csbi.wAttributes,
		cellCount,
		homeCoords,
		&count
	)) return;

	/* Move the cursor home */
	SetConsoleCursorPosition(hStdOut, homeCoords);
}

bool GotoXY(unsigned x, unsigned y) {
	COORD position = { x, y };
	return SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
}

int wherex() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (!GetConsoleScreenBufferInfo(
		GetStdHandle(STD_OUTPUT_HANDLE),
		&csbi
	))
		return -1;
	return csbi.dwCursorPosition.X;
}

int wherey() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (!GetConsoleScreenBufferInfo(
		GetStdHandle(STD_OUTPUT_HANDLE),
		&csbi
	))
		return -1;
	return csbi.dwCursorPosition.Y;
}