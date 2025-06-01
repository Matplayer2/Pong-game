#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <raylib.h>
using namespace std;

//CONSTS
#define widthMap 1024
#define heightMap 512
#define borderWidth 16

#define xPlayerA 25
#define xPlayerB widthMap - 25 - 15
#define ballDiameter 20
#define FPS 60

//map command key
#define exitKey 81

void stampaMappa()
{
	//BORDI ORRIZONTALI
	DrawRectangle(0,0, widthMap, borderWidth, WHITE);
	DrawRectangle(0,heightMap-borderWidth, widthMap, borderWidth, WHITE);

	//CONFINE DEL CAMPO
	for (int i = borderWidth; i < heightMap-borderWidth; i += 30)
	{
		DrawRectangle(widthMap/2 - 8, i + 7, 16, 15, WHITE);	
	}
}
bool pressedAValidKey(int mapKey[], int n)
{
	bool ans;

	ans = false;
	for (int i = 0; i < n; i++)
	{
		if (IsKeyPressed(mapKey[i]))	
			ans = true;
	}
	return (ans);
}

int generaAngolazione()
{
	int seg,
		modulo,
		angolazione;

	seg = rand() % (2);
	modulo = rand() % (7 - 1) + 1;
	if (seg)
		angolazione = modulo * -1;	
	else
		angolazione = modulo;
	return (angolazione);
}

void translateAngulation(int xBall, int yBall, int feed_xBall, int feed_yBall, 
						 int speed_coeficientBallX, int speed_coeficientBallY,
						 int yPlayerA, int yPlayerB)
{

}

int main()
{
	//VARIABLE DECLARATION
	int keyPressed,
		keyA,
		keyB,
		keyMap[10] = {81,87,83,264,265},
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
	
	srand(time(NULL));
	Image iconGame = LoadImage("IMG/icon.png");	

	InitWindow(widthMap, heightMap, "PONG GAME");
	SetTargetFPS(FPS);
	SetWindowIcon(iconGame);
	HideCursor();
	DisableCursor();

	nKeyRegistered = 5;
	yPlayerA = heightMap/2 - 50;
	yPlayerB = heightMap/2 - 50;
	keyA = 0;
	keyB = 0;
	feed_speedA = 0;
	feed_speedB = 0;
	speed_coeficient = 3;
	xBall = widthMap / 2 - ballDiameter/2;
	yBall = heightMap / 2 - ballDiameter/2;
	speed_coeficientBall = 2;
	speed_coeficientBallX = generaAngolazione();
	speed_coeficientBallY = generaAngolazione();
	feed_xBall = -speed_coeficientBallX;
	feed_yBall = -speed_coeficientBallY; 
	scoreA = 0;
	scoreB = 0;

	while (!WindowShouldClose() && keyPressed != exitKey)
	{
		//COMMANDS MANAGEMENT
		if (pressedAValidKey(keyMap, nKeyRegistered))	
		{
			keyPressed = GetKeyPressed();		
			switch(keyPressed)
			{
				case 87: 
						feed_speedA = -speed_coeficient;	
						keyA = keyPressed;
						break;
				case 83: 
						feed_speedA = +speed_coeficient;
						keyA = keyPressed;
						break;
				case 265:
						feed_speedB = -speed_coeficient;
						keyB = keyPressed;
						break;
				case 264:
						feed_speedB = +speed_coeficient;
						keyB = keyPressed;
						break;
			}

		}	
		if (IsKeyUp(keyA))
			feed_speedA = 0;
		if (IsKeyUp(keyB))
			feed_speedB = 0;
		BeginDrawing();	
			stampaMappa();

			DrawRectangle(xPlayerA, yPlayerA, 15, 100, RED);
			DrawRectangle(xPlayerB, yPlayerB, 15, 100, RED);

			DrawRectangle(xBall, yBall, ballDiameter, ballDiameter, GREEN);
			ClearBackground(BLACK);
		EndDrawing();

		if (yPlayerA + feed_speedA >= borderWidth && yPlayerA + feed_speedA <= heightMap - borderWidth - 100)
			yPlayerA += feed_speedA;
		if (yPlayerB + feed_speedB >= borderWidth && yPlayerB + feed_speedB <= heightMap - borderWidth - 100)
			yPlayerB += feed_speedB;
		
		//COLLISIONI PALLINA

		xBall += feed_xBall;
		yBall += feed_yBall;

		if (yBall <= borderWidth && feed_xBall >= speed_coeficientBallX && feed_yBall <= speed_coeficientBallY ||
			yBall <= borderWidth && feed_xBall <= speed_coeficientBallX && feed_yBall <= speed_coeficientBallY)
			feed_yBall *= -1;
		else if (yBall >= heightMap-borderWidth-ballDiameter && feed_xBall >= speed_coeficientBallX && feed_yBall >= speed_coeficientBallY ||
				 yBall >= heightMap-borderWidth-ballDiameter && feed_xBall <= speed_coeficientBallX && feed_yBall >= speed_coeficientBallY)
			feed_yBall *= -1;
		else if (xBall <= xPlayerA + 15 && yBall > yPlayerA && yBall < yPlayerA + 100 && feed_xBall <= speed_coeficientBallX && feed_yBall <= speed_coeficientBallY || 
				 xBall <= xPlayerA + 15 && yBall > yPlayerA && yBall < yPlayerA + 100 && feed_xBall <= speed_coeficientBallX && feed_yBall >= speed_coeficientBallY)
			feed_xBall *= -1; 
		else if (xBall >= widthMap - 20 - 15 - ballDiameter && yBall > yPlayerB && yBall < yPlayerB + 100 && feed_xBall >= speed_coeficientBallX && feed_yBall >= speed_coeficientBallY || 
				 xBall >= widthMap - 20 - 15 - ballDiameter && yBall > yPlayerB && yBall < yPlayerB + 100 && feed_xBall >= speed_coeficientBallX && feed_yBall <= speed_coeficientBallY)
			feed_xBall *= -1;

		if (xBall > widthMap - 5 - ballDiameter) {
			scoreA += 5;
			speed_coeficientBallX = generaAngolazione();
			speed_coeficientBallY = generaAngolazione();
			feed_xBall = speed_coeficientBallX;
			feed_yBall = speed_coeficientBallY;
			xBall = widthMap / 2 - ballDiameter/2;
			yBall = heightMap / 2 - ballDiameter/2;
	
		}

		if (xBall < 5) {
			speed_coeficientBallX = generaAngolazione();
			speed_coeficientBallY = generaAngolazione();
			feed_xBall = speed_coeficientBallX;
			feed_yBall = speed_coeficientBallY;
			scoreB += 5;
			xBall = widthMap / 2 - ballDiameter/2;
			yBall = heightMap / 2 - ballDiameter/2;
		}
	}
	CloseWindow();
}
