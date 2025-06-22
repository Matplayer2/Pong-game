#ifndef PONGLIB
	#include <string.h>
	#include <iostream>
	#include <time.h>
	#include <stdlib.h>
	#include <raylib.h>
	using namespace std;

	#define PONGLIB
	//CONSTS
	#define widthMap 1024
	#define heightMap 512
	#define borderWidth 16

	#define maxSpeed 10
	#define minSpeed -10

	#define xPlayerA borderWidth
	#define xPlayerB (widthMap - borderWidth - 15)
	#define ballDiameter 20
	#define borderMenu 6
	#define ySetter -85

	#define startPosButtomMenuX  (widthMap-512)/2+borderMenu + 130
	#define lenghtButtomMenu  (512-borderMenu)-260
	#define yButMenu1  (heightMap)/2+borderMenu + 15 + ySetter
	#define yButMenu2  (heightMap)/2+borderMenu + 60 + ySetter
	#define yButMenu3 (heightMap)/2+borderMenu + 105 + ySetter
	#define heightButMenu 40

	#define ray (float)(ballDiameter)/2
	#define FPS 60

	//map command key
	#define exitKey 81
	#define enterKey 257
	#define arrowUpKey 265
	#define arrowDownKey 264
	#define arrowLeftKey 263
	#define arrowRightKey 262
	#define escKey 256
	//COLOR
	#define GRAY_SOFT       (Color){ 70, 70, 70, 100 }
	#define REDLIGHT        (Color){ 255, 0, 0, 255 }     // Red
	#define WHITESOFT		(Color){180,240,210,255}
														  
	char *itoa (int value, char *result, int base);
	void printMap(bool victory);

	bool pressedAValidKey(int mapKey[], int n);
	int generateAngle(int minModule, int maxModule);

	void verifyCollision(int *xBall, int *yBall, int yPlayerA, int yPlayerB, bool *collisioneMuro, bool *collisionePlayer);

	void collisionedTheWall(int *feed_xBall, int *feed_yBall, Sound bounce);

	void collisionedPlayer(int *feed_xBall, int *feed_yBall, Sound bounce);
	
	void invertAngulation(int *xBall, int *yBall, int *feed_xBall, int *feed_yBall, 
							 int yPlayerA, int yPlayerB, Sound bounce);


	void printEnding(bool a, bool b, int scoreA, int scoreB);
	void isEnd(int scoreA, int scoreB, int *keyPressed, bool *aWin, bool *bWin);
	
	void thereIsGoal(int *xBall, int *yBall, int *scoreA, int *scoreB,
				int *speed_coeficientBallX, int *speed_coeficientBallY, int *yPlayerA, int *yPlayerB,
				int *feed_yBall, int *feed_xBall);
	void feedsMovement(int *xBall, int *yBall, int *yPlayerA, int *yPlayerB, 
			int feed_speedA, int feed_speedB, int feed_xBall, int feed_yBall);
void verifyKeyPressing(int keyMap[], int nKeyRegistered, int *feed_speedA, int *feed_speedB,
					int *keyA, int *keyB, int *keyPressed, int speed_coeficient, bool pause);

#endif
