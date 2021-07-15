#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <time.h>

// Makes a 2d array with random values of 0's and 1's
int **make2dArray(int rows, int cols)
{
	int **arr = new int *[rows];
	for (int i = 0; i < rows; i++)
		arr[i] = new int[cols];

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			int col = rand() % 2;
			arr[i][j] = col;
		}
	}
	return arr;
}

void delete2dArray(int **arr, int rows, int cols)
{
	for (int i = 0; i < rows; i++)
		delete[] arr[i];
	delete arr;
}

// Function to count the value of eight adjacent neighbors to the current cell
int countNeighbors(int **grid, int x, int y, int rows, int cols)
{
	int sum = 0;
	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			/*
			let x = 0, i = -1, rows = 10:
			(0 - 1 + 10) % 10 = 9 	// The -1 gets wrapped around to 9
			Similarly:
			(9 + 1 + 10) % 10 = 0	// The 10 gets wrapped around to 0
			But:
			(3 + 1 + 10) % 10 = 4	// No effect on other than edges
			*/
			int row = (x + i + rows) % rows;
			int col = (y + j + cols) % cols;
			sum += grid[row][col];
		}
	}
	sum -= grid[x][y];
	return sum;
}

int main()
{
	/* Uncomment to get best possible resolution
	auto videoMode = sf::VideoMode::getFullscreenModes();
	int winWidth = videoMode[0].width, winHeight = videoMode[0].height; */
	int winWidth = 800, winHeight = 400;
	int squareSide = 10;
	int rows = winWidth / squareSide, cols = winHeight / squareSide;
	srand(time(0));

	// Create the main window
	sf::RenderWindow window(sf::VideoMode(winWidth, winHeight, 32), "SFML Window", sf::Style::Default);
	window.setFramerateLimit(10);
	int **grid = make2dArray(rows, cols);
	int **next = make2dArray(rows, cols);

	// Start game loop
	while (window.isOpen())
	{
		// Process Events
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Close Window: exit
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// Clear screen
		window.clear();

		// Print the old buffer (grid)
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				sf::RectangleShape square(sf::Vector2f(squareSide, squareSide));
				square.setPosition(sf::Vector2f(i * squareSide, j * squareSide));
				square.setOutlineThickness(1.0f);
				square.setOutlineColor(sf::Color(125, 125, 125, 25));

				if (grid[i][j] == 0)
					square.setFillColor(sf::Color::White);
				else
					square.setFillColor(sf::Color::Black);

				// Draw the shape
				window.draw(square);
			}
		}

		// Make changes in the next buffer
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				int state = grid[i][j];
				int neighbors = countNeighbors(grid, i, j, rows, cols);

				if (state == 0 && neighbors == 3)
					next[i][j] = 1;
				else if (state == 1 && (neighbors < 2 || neighbors > 3))
					next[i][j] = 0;
				else
					next[i][j] = state;
			}
		}

		// Swap the old buffer with the next buffer
		int **temp = grid;
		grid = next;
		next = temp;

		// Update the window
		window.display();
	}

	delete2dArray(grid, rows, cols);
	return EXIT_SUCCESS;
}