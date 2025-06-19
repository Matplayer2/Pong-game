#include <cstdio>
#include <string.h>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <raylib.h>
using namespace std;

//CONSTS
#define widthMap 1024
#define heightMap 512
#define borderWidth 16

#define maxSpeed 10
#define minSpeed -10

#define xPlayerA borderWidth
#define xPlayerB (widthMap - borderWidth - 15)
#define ballDiameter 20
#define ray (float)(ballDiameter)/2
#define FPS 60

//map command key
#define exitKey 81

//COLOR
#define GRAY_SOFT       (Color){ 130, 130, 130, 100 }
#define REDLIGHT        (Color){ 255, 0, 0, 255 }     // Red
#define WHITESOFT		(Color){180,240,210,255}
													  
char *itoa (int value, char *result, int base)
{
    // check that the base if valid
    if (base < 2 || base > 36) { *result = '\0'; return result; }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}

void stampaMappa(bool victory)
{
	//BORDI ORRIZONTALI
	DrawRectangle(0,0, widthMap, borderWidth, WHITE);
	DrawRectangle(0,heightMap-borderWidth, widthMap, borderWidth, WHITE);

	//CONFINE DEL CAMPO
	for (int i = borderWidth; i < heightMap-borderWidth; i += 30) {
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

int generaAngolazione(int minModule, int maxModule)
{
	int modulo,
		angolazione,
		seg;
	seg = rand() % 2;
	modulo = rand() % (maxModule - minModule) + minModule;
	if (seg)
		modulo *= -1;
	return (modulo);
}

void invertAngulation(int *xBall, int *yBall, int *feed_xBall, int *feed_yBall, 
						 int yPlayerA, int yPlayerB, Sound bounce)
{
		int accelerazione,
			deaccelerazione,
			slowdownPossibiliti,
			angle;
		bool collisionePlayer,
			 collisioneMuro;

		collisionePlayer = false;
		collisioneMuro = false;

		if (*yBall-ray <= borderWidth){
			*yBall  = borderWidth+1+ray;
			collisioneMuro = true;
		}
		else if (*yBall+ray >= heightMap-borderWidth-1) {
			*yBall = heightMap-borderWidth-1-1-ray;
			collisioneMuro = true;
		}
		else if (*xBall-ray <= xPlayerA + 15 && ((*yBall-ray > yPlayerA && *yBall-ray < yPlayerA + 100) || (*yBall-ray + ballDiameter > yPlayerA && *yBall-ray + ballDiameter < yPlayerA + 100) )) {
			*xBall = xPlayerA + 15 + ray;
			collisionePlayer = true;
		}
		else if (*xBall-ray >= widthMap - borderWidth - 15 - ballDiameter && (*yBall-ray > yPlayerB && *yBall-ray < yPlayerB + 100 || *yBall-ray + ballDiameter > yPlayerB && *yBall-ray + ballDiameter < yPlayerB + 100 )) {
			*xBall = widthMap - borderWidth - 15 - 1 - ray;
			collisionePlayer = true;
		}
		
		//se fa collisione con la racchetta del player, aumenta di velocità la pallina
		if (collisionePlayer) {
			PlaySound(bounce);
			*feed_xBall *= -1; 
			
			angle = generaAngolazione(0,3);
			if (*feed_yBall + angle >= minSpeed && *feed_yBall + angle <= maxSpeed)
				*feed_yBall += angle; 
			angle = generaAngolazione(0, 2);
			if (*feed_xBall + angle >= minSpeed && *feed_xBall + angle <= maxSpeed)
				*feed_xBall += angle;

 			accelerazione = rand() % 3 + 1;
	
			
			if (abs(*feed_xBall) + accelerazione <= maxSpeed && abs(*feed_yBall) + accelerazione <= maxSpeed)
			{
				if (*feed_xBall <= 0)
					*feed_xBall -= accelerazione;
				else
					*feed_xBall += accelerazione;

				if (*feed_yBall <= 0)
					*feed_yBall -= accelerazione;
				else
					*feed_yBall += accelerazione;
			}
		}

	if (collisioneMuro)	 {
		PlaySound(bounce);
		*feed_yBall *= -1;
		deaccelerazione = 0;

		slowdownPossibiliti = rand() % 3;	

		if (slowdownPossibiliti == 2)	deaccelerazione = 1;
		
		if (abs(*feed_xBall) - deaccelerazione >= minSpeed && abs(*feed_yBall) - deaccelerazione >= minSpeed) {
			if (*feed_xBall <= 0)		
				*feed_xBall += deaccelerazione;
			else
				*feed_xBall -= deaccelerazione;
			
			if (*feed_yBall <= 0)
				*feed_yBall += deaccelerazione;
			else
				*feed_yBall -= deaccelerazione;
		}
	}
}
void printEnding(bool a, bool b, int scoreA, int scoreB)
{
	char *buf;

	buf = (char*)malloc(sizeof(char)*2);
	if (a || b)
	{
		BeginDrawing();	
			ClearBackground(BLACK);
			stampaMappa(true);
			if (a) {
				DrawText("VITTORIA", widthMap/2-320, heightMap/2-50, 50, GREEN);
				DrawText(itoa(scoreA, buf, 10), widthMap/2-320, heightMap/2+20, 40, GREEN);

				DrawText("SCONFITTA", widthMap/2+60, heightMap/2-50, 50, RED);
				DrawText(itoa(scoreB, buf, 10), widthMap/2+60, heightMap/2+20, 40, RED);
			}
			else {
				DrawText(itoa(scoreA, buf, 10), widthMap/2-320, heightMap/2+20, 40, GREEN);
				DrawText("SCONFITTA", widthMap/2-320, heightMap/2-50, 50, GREEN);

				DrawText("VITTORIA", widthMap/2+60, heightMap/2-50, 50, RED);
				DrawText(itoa(scoreB, buf, 10), widthMap/2+60, heightMap/2+20, 40, RED);
			}

		EndDrawing();
		WaitTime(3);

	}

}
void isEnd(int scoreA, int scoreB, int *keyPressed, bool *aWin, bool *bWin)
{
	if (scoreA == 5)		*aWin = true;
	else if (scoreB == 5)	*bWin = true;
	
	if (*aWin || *bWin)
		*keyPressed = exitKey;

}
void thereIsGoal(int *xBall, int *yBall, int *scoreA, int *scoreB,
			int *speed_coeficientBallX, int *speed_coeficientBallY, int *yPlayerA, int *yPlayerB,
			int *feed_yBall, int *feed_xBall)
{
	if ( *xBall-ray + ballDiameter > xPlayerB || *xBall-ray < borderWidth - 15 ) {

		if (*xBall-ray + ballDiameter > xPlayerB) {
			*scoreA += 1;
			*speed_coeficientBallX = 5;
		}
		else {
			*scoreB += 1;
			*speed_coeficientBallX = -5;
		}


		*speed_coeficientBallY = 0;

		*yPlayerA = heightMap/2 - 50;
		*yPlayerB = heightMap/2 - 50;

		*feed_xBall = *speed_coeficientBallX;
		*feed_yBall = *speed_coeficientBallY;
		*xBall = widthMap / 2 - ballDiameter/2;
		*yBall = heightMap / 2 - ballDiameter/2;
	}

}
int main()
{
	
	//VARIABLE DECLARATION
	int keyPressed,
		keyA,
		keyB,
		keyMap[10] = {80, 81, 87, 83, 264, 265},
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
	SetTargetFPS(FPS);
	SetMasterVolume(50);

	HideCursor();
	DisableCursor();

	buf = (char*)malloc(sizeof(char)*2);

	pause = false;
	aWin = false;
	bWin = false;
	
	bounce = LoadSound("AUDIO/23338__altemark__pong.wav");

	nKeyRegistered = 6;
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
				case 80:
					if (!pause)
						pause = true;
					else
						pause = false;		
					break;
			}

		}	

		BeginDrawing();	
			stampaMappa(false);
			
			DrawCircle(0, heightMap/2, heightMap*1.0/2-borderWidth, GRAY_SOFT);
			DrawCircle(widthMap, heightMap/2, heightMap*1.0/2-borderWidth, GRAY_SOFT);

			DrawRectangle(xPlayerA, yPlayerA, 15, 100, REDLIGHT);
			DrawRectangle(xPlayerB, yPlayerB, 15, 100, REDLIGHT);
			
			DrawText("SCORE A: ", widthMap/2-200, borderWidth+15, 20, SKYBLUE);
			DrawText(itoa(scoreA, buf, 10), widthMap/2-100, borderWidth+15, 20, SKYBLUE);
			DrawText("SCORE B: ", widthMap/2+100, borderWidth+15, 20, SKYBLUE);
			DrawText(itoa(scoreB, buf, 10), widthMap/2+200, borderWidth+15, 20, SKYBLUE);

			DrawCircle(xBall, yBall, ray, GREEN);

			ClearBackground(BLACK);
		EndDrawing();

		
		if (IsKeyUp(keyA))
			feed_speedA = 0;
		if (IsKeyUp(keyB))
			feed_speedB = 0;
		if (!pause)
		{
			if (yPlayerA + feed_speedA >= borderWidth && 
			yPlayerA + feed_speedA <= heightMap - borderWidth - 100)
			{
				yPlayerA += feed_speedA;
			}
			if (yPlayerB + feed_speedB >= borderWidth &&
				yPlayerB + feed_speedB <= heightMap - borderWidth - 100)
			{
				yPlayerB += feed_speedB;
			}	
			xBall += feed_xBall;
			yBall += feed_yBall;
		}


		invertAngulation(&xBall, &yBall, &feed_xBall, &feed_yBall, yPlayerA, yPlayerB, bounce);

		thereIsGoal(&xBall, &yBall, &scoreA, &scoreB, &speed_coeficientBallX, &speed_coeficientBallY, 
					&yPlayerA, &yPlayerB, &feed_yBall, &feed_xBall);

		isEnd(scoreA, scoreB, &keyPressed, &aWin, &bWin);
	}

	printEnding(aWin, bWin, scoreA, scoreB);
	CloseWindow();
}
