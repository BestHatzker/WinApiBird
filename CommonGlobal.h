#pragma once

#define PI 3.14159265 

#define MAX_BLOCKS 5

#define FBC_UPDATEMODEL (WM_USER + 1)

#define MAX_LOADSTRING 100

#define BUTTON_START   1001
#define BUTTON_HIGH    1002
#define BUTTON_EXIT    1003

int higthscore = 0;
bool startingGame = true;
bool firstStart = true;
bool playingNow = false;

int fieldSizeX = 0;
int fieldSizeY = 0;
int blockMin = 0;

#define min(a,b)            (((a) < (b)) ? (a) : (b))
#define max(a,b)            (((a) > (b)) ? (a) : (b))

bool rectangleIntersection(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
	int left = max(x1, x3);
	int top = min(y2, y4);
	int right = min(x2, x4);
	int bottom = max(y1, y3);

	return  left < right && top < bottom;
}

bool _rectangleIntersection(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {


	if (x1 >= x2 || y2 >= y1 || x3 >= x4 || y4 >= y1)
		return true;
	else
		return false;
}

bool doOverlap(int l1x, int r1y, int r1x, int l1y, int l2x, int r2y, int r2x, int l2y)
{
	// If one rectangle is on left side of other 
 	if (l1x > r2x || l2x > r1x)
 		return false;

	// If one rectangle is above other 
	if (l1y < r2y || l2y < r1y)
		return false;

	return true;
}