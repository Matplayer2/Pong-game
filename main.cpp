#include <cstdio>
#include <string.h>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <raylib.h>
using namespace std;

#include "ponglib.h"

int main()
{
	
	//VARIABLE DECLARATION
	int keyPressed,
		keyA,
		keyB,
		keyMap[10] = {80, 81, 87, 83, 264, 265, 256},
		nKeyRegistered,
		yPlayerA,
		yPlayerB,
		feed_speedA,
		feed_speedB,
		speed_coeficient,
		xBall,
		yBall,
		feed_xBall,
		feed_yBall,
		speed_coeficientBall,
		speed_coeficientBallX,
		speed_coeficientBallY,
		scoreA,
		scoreB;
	bool pause,
		 aWin, bWin;
	char *buf;
	Sound bounce;

	cout << "AVVIO PONG-GAME" << endl;	
	srand(time(NULL));
	Image iconGame = LoadImage("IMG/icon.png");	

	InitWindow(widthMap, heightMap, "PONG GAME");
	InitAudioDevice();
	if (IsAudioDeviceReady())	
		cout << "LOG: AUDIO: successfully" << endl;
	else
		cout << "SYSTEM ERROR: AUDIO: NOT STARTED!" << endl;

	SetWindowIcon(iconGame);
	SetExitKey(exitKey);
	SetTargetFPS(FPS);
	SetMasterVolume(50);

	HideCursor();
	DisableCursor();

	buf = (char*)malloc(sizeof(char)*2);

	pause = false;
	aWin = false;
	bWin = false;
	
	bounce = LoadSound("AUDIO/23338__altemark__pong.wav");

	nKeyRegistered = 7;
	yPlayerA = heightMap/2 - 50;
	yPlayerB = heightMap/2 - 50;
	keyA = 0;
	keyB = 0;
	feed_speedA = 0;
	feed_speedB = 0;
	speed_coeficient = 4;
	xBall = widthMap / 2 - ballDiameter/2;
	yBall = heightMap / 2 - ballDiameter/2;
	speed_coeficientBall = 2;
	speed_coeficientBallX = 5;
	speed_coeficientBallY = 0;
	feed_xBall = -speed_coeficientBallX;
	feed_yBall = -speed_coeficientBallY; 
	scoreA = 0;
	scoreB = 0;


	
	while (!WindowShouldClose() && keyPressed != exitKey) {

		verifyKeyPressing(keyMap, nKeyRegistered, &feed_speedA, &feed_speedB,
					&keyA, &keyB, &keyPressed, speed_coeficient, pause);
		BeginDrawing();	
			printMap(false);
			
			DrawCircle(0, heightMap/2, heightMap*1.0/2-borderWidth, GRAY_SOFT);
			DrawCircle(widthMap, heightMap/2, heightMap*1.0/2-borderWidth, GRAY_SOFT);

			DrawRectangle(xPlayerA, yPlayerA, 15, 100, RED);
			DrawRectangle(xPlayerB, yPlayerB, 15, 100, REDLIGHT);
			
			DrawText("SCORE A: ", widthMap/2-200, borderWidth+15, 20, SKYBLUE);
			DrawText(itoa(scoreA, buf, 10), widthMap/2-100, borderWidth+15, 20, SKYBLUE);
			DrawText("SCORE B: ", widthMap/2+100, borderWidth+15, 20, SKYBLUE);
			DrawText(itoa(scoreB, buf, 10), widthMap/2+200, borderWidth+15, 20, SKYBLUE);

			DrawCircle(xBall, yBall, ray, GREEN);

			ClearBackground(BLACK);
		EndDrawing();

		
		if (IsKeyUp(keyA))	feed_speedA = 0;
		if (IsKeyUp(keyB))	feed_speedB = 0;

		if (!pause)	{
			feedsMovement(&xBall, &yBall, &yPlayerA, &yPlayerB, feed_speedA, feed_speedB, feed_xBall, feed_yBall);
		}


		invertAngulation(&xBall, &yBall, &feed_xBall, &feed_yBall, yPlayerA, yPlayerB, bounce);

		thereIsGoal(&xBall, &yBall, &scoreA, &scoreB, &speed_coeficientBallX, &speed_coeficientBallY, 
					&yPlayerA, &yPlayerB, &feed_yBall, &feed_xBall);

		isEnd(scoreA, scoreB, &keyPressed, &aWin, &bWin);
	}

	printEnding(aWin, bWin, scoreA, scoreB);
	CloseWindow();
}
