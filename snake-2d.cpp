#include <GL\glut.h>
#include <iostream>
#include <list>

using namespace std;

enum { MAP_WIDTH = 25, MAP_HIGH = 25, BLOCK_WIDTH = 20, BLOCK_HIGH = 20 };
enum Type { EMPTY, SNAKE_BLOCK, FRUIT };
enum Direction { LEFT, UP, RIGHT, DOWN };

Direction lastMove = LEFT;

Type map[MAP_WIDTH][MAP_HIGH];

list<pair<int, int> > snake_blocks;

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	/*for(int i = 0; i < MAP_WIDTH; i++){
			glColor3f(1,1,1);
			glBegin(GL_LINES);
			glVertex2f(i * BLOCK_WIDTH, 0);
			glVertex2f(i * BLOCK_WIDTH, MAP_HIGH * BLOCK_HIGH);
			glEnd();
	}
	for(int i = 0; i < MAP_WIDTH; i++){
			glColor3f(1,1,1);
			glBegin(GL_LINES);
			glVertex2f(0, i * BLOCK_HIGH);
			glVertex2f(MAP_WIDTH * BLOCK_WIDTH, i * BLOCK_HIGH);
			glEnd();
	}*/

	for (int x = 0; x < MAP_WIDTH; x++) {
		for (int y = 0; y < MAP_HIGH; y++) {
			int r = rand() % 2;
			int g = rand() % 2;
			int b = rand() % 2;
			if (r == 0 && g == 0 && b == 0) {
				r = 1;
				g = 1;
			}
			switch (map[x][y]) {
			case EMPTY:
				break;
			case SNAKE_BLOCK:
				glColor3f(r, g, b);
				glBegin(GL_POLYGON);
				glVertex2f(x * BLOCK_WIDTH, y * BLOCK_HIGH);
				glVertex2f(x * BLOCK_WIDTH + BLOCK_WIDTH, y * BLOCK_HIGH);
				glVertex2f(x * BLOCK_WIDTH + BLOCK_WIDTH, y * BLOCK_HIGH + BLOCK_HIGH);
				glVertex2f(x * BLOCK_WIDTH, y * BLOCK_HIGH + BLOCK_HIGH);
				glEnd();
				break;
			case FRUIT:
				glColor3f(1, 1, 0);
				glBegin(GL_POLYGON);
				glVertex2f(x * BLOCK_WIDTH, y * BLOCK_HIGH);
				glVertex2f(x * BLOCK_WIDTH + BLOCK_WIDTH, y * BLOCK_HIGH);
				glVertex2f(x * BLOCK_WIDTH + BLOCK_WIDTH, y * BLOCK_HIGH + BLOCK_HIGH);
				glVertex2f(x * BLOCK_WIDTH, y * BLOCK_HIGH + BLOCK_HIGH);
				glEnd();
				break;
			}
		}
	}

	glutSwapBuffers();
}

void spawnFruit()
{
again:
	int x = rand() % MAP_WIDTH + 1;
	int y = rand() % MAP_HIGH + 1;
	if (map[x][y] == EMPTY) {
		map[x][y] = FRUIT;
	}
	else {
		goto again;
	}
}

void timer(int = 0)
{
	display();

	map[snake_blocks.back().first][snake_blocks.back().second] = EMPTY;

	for (auto first(snake_blocks.rbegin()), second(first); ++second != snake_blocks.rend(); ++first) {
		first->first = second->first;
		first->second = second->second;
	}

	switch (lastMove)
	{
	case LEFT: snake_blocks.front().first--;break;
	case UP: snake_blocks.front().second--;break;
	case RIGHT: snake_blocks.front().first++;break;
	case DOWN: snake_blocks.front().second++;break;
	}

	if (snake_blocks.front().first < -1) {
		snake_blocks.front().first = 24;
	}
	else if (snake_blocks.front().first > 25) {
		snake_blocks.front().first = 0;
	}

	if (map[snake_blocks.front().first][snake_blocks.front().second] == FRUIT) {
		snake_blocks.push_back(pair<int, int>(snake_blocks.back().first, snake_blocks.back().second));
		spawnFruit();
	}

	for (auto iter = snake_blocks.begin(); iter != snake_blocks.end(); ++iter)
	{
		map[iter->first][iter->second] = SNAKE_BLOCK;
	}

	glutTimerFunc(150, timer, 0);
}

void keyEvent(int key, int, int)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		lastMove = LEFT;break;
	case GLUT_KEY_UP:
		lastMove = UP;break;
	case GLUT_KEY_RIGHT:
		lastMove = RIGHT;break;
	case GLUT_KEY_DOWN:
		lastMove = DOWN;break;
	}
}

void gameInit()
{
	//init map
	for (int x = 0; x < MAP_WIDTH; x++) {
		for (int y = 0; y < MAP_HIGH; y++) {
			map[x][y] = EMPTY;
		}
	}

	//init snake
	map[12][12] = SNAKE_BLOCK;
	snake_blocks.push_back(pair<int, int>(12, 12));

	//init fruit
	spawnFruit();

}

int main(int argc, char** argv)
{
	gameInit();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(MAP_WIDTH * BLOCK_WIDTH, MAP_HIGH * BLOCK_HIGH);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Snake2D");
	glClearColor(0, 0, 0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, MAP_WIDTH * BLOCK_WIDTH, MAP_HIGH * BLOCK_HIGH, 0, -1.0, 1.0);
	glutDisplayFunc(display);
	glutSpecialFunc(keyEvent);
	timer();

	glutMainLoop();
}