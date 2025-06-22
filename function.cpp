#include "ponglib.h"
#include <raylib.h>

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

void printMap(bool victory)
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

int generateAngle(int minModule, int maxModule)
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

void verifyCollision(int *xBall, int *yBall, int yPlayerA, int yPlayerB, bool *collisioneMuro, bool *collisionePlayer)
{
	if (*yBall-ray <= borderWidth){
		*yBall  = borderWidth+1+ray;
		*collisioneMuro = true;
	}
	else if (*yBall+ray >= heightMap-borderWidth-1) {
		*yBall = heightMap-borderWidth-1-1-ray;
		*collisioneMuro = true;
	}
	else if (*xBall-ray <= xPlayerA + 15 && ((*yBall-ray > yPlayerA && *yBall-ray < yPlayerA + 100) || (*yBall-ray + ballDiameter > yPlayerA && *yBall-ray + ballDiameter < yPlayerA + 100) )) {
		*xBall = xPlayerA + 15 + ray;
		*collisionePlayer = true;
	}
	else if (*xBall-ray >= widthMap - borderWidth - 15 - ballDiameter && (*yBall-ray > yPlayerB && *yBall-ray < yPlayerB + 100 || *yBall-ray + ballDiameter > yPlayerB && *yBall-ray + ballDiameter < yPlayerB + 100 )) {
		*xBall = widthMap - borderWidth - 15 - 1 - ray;
		*collisionePlayer = true;
	}
}
void collisionedTheWall(int *feed_xBall, int *feed_yBall, Sound bounce) {
	int deaccelerazione,
		slowdownPossibiliti;

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
void collisionedPlayer(int *feed_xBall, int *feed_yBall, Sound bounce)
{
	int angle,
		accelerazione;

	PlaySound(bounce);
	*feed_xBall *= -1; 
	
	angle = generateAngle(0,3);
	if (*feed_yBall + angle >= minSpeed && *feed_yBall + angle <= maxSpeed)
		*feed_yBall += angle; 

	angle = generateAngle(0, 2);
	if (*feed_xBall + angle >= minSpeed && *feed_xBall + angle <= maxSpeed)
		*feed_xBall += angle;

	accelerazione = rand() % 3 + 1;
	if (abs(*feed_xBall) + accelerazione <= maxSpeed && abs(*feed_yBall) + accelerazione <= maxSpeed) {
		if (*feed_xBall <= 0)	*feed_xBall -= accelerazione;
		else	
			*feed_xBall += accelerazione;

		if (*feed_yBall <= 0)	*feed_yBall -= accelerazione;
		else
			*feed_yBall += accelerazione;
	}
}
void invertAngulation(int *xBall, int *yBall, int *feed_xBall, int *feed_yBall, 
						 int yPlayerA, int yPlayerB, Sound bounce)
{
	bool collisionePlayer,
		 collisioneMuro;

	collisionePlayer = false;
	collisioneMuro = false;

	verifyCollision(xBall, yBall, yPlayerA, yPlayerB, &collisioneMuro, &collisionePlayer);	

	//se fa collisione con la racchetta del player, aumenta di velocità la pallina
	if (collisionePlayer)
		collisionedPlayer(feed_xBall, feed_yBall, bounce);

	if (collisioneMuro)
		collisionedTheWall(feed_xBall, feed_yBall, bounce);
}

void printEnding(bool a, bool b, int scoreA, int scoreB)
{
	char *buf;

	buf = (char*)malloc(sizeof(char)*2);
	if (a || b)
	{
		BeginDrawing();	
			ClearBackground(BLACK);
			printMap(true);
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
void feedsMovement(int *xBall, int *yBall, int *yPlayerA, int *yPlayerB, 
		int feed_speedA, int feed_speedB, int feed_xBall, int feed_yBall)
{
	if (*yPlayerA + feed_speedA >= borderWidth && 
	*yPlayerA + feed_speedA <= heightMap - borderWidth - 100)
	{
		*yPlayerA += feed_speedA;
	}
	if (*yPlayerB + feed_speedB >= borderWidth &&
		*yPlayerB + feed_speedB <= heightMap - borderWidth - 100)
	{
		*yPlayerB += feed_speedB;
	}	
	*xBall += feed_xBall;
	*yBall += feed_yBall;
}
void menuModeManage(int *keyPressed)
{
	int	state,
		key,
		Mx, My,
		audioLevel;
	double volume;
	bool mousePressed;

	char *buf,
		 *tmp;

	buf = (char*)malloc(sizeof(char)*10);
	tmp = (char*)malloc(sizeof(char)*10);
	state = 0;
	key = 0;
	EnableCursor();
	SetMousePosition(100, 100);

	while (key != 256)
	{
		key = GetKeyPressed();	

		//arrow map 
		switch (key) {
			case escKey:	
				key = 256;
				break;
			case arrowUpKey: // arrow up
					if (state == 0)	state = 2;
					else
						state--;
				break;
			case arrowDownKey: // arrow down
					if (state == 2)	state = 0;
					else
						state++;
				break;
		}
		mousePressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

		Mx = GetMouseX();
		My = GetMouseY();
		if (Mx >= (widthMap-512)/2+borderWidth+130 && Mx < (widthMap-512)/2+borderMenu + 130 + (512-borderMenu)-260)
		{
			if (My >= yButMenu1  && My <= yButMenu1 + heightButMenu)
				state = 0;
			else if (My >= yButMenu2  && My <= yButMenu2 + heightButMenu)
				state = 1;
			else if (My >= yButMenu3  && My <= yButMenu3 + heightButMenu)
				state = 2;
		}

		switch (state) {
			case 0:
				if (key == enterKey || mousePressed)
					key = 256;
				break;
			case 1:
				volume = GetMasterVolume();

				if (key == arrowLeftKey && volume > 0) {
					SetMasterVolume(volume - 0.05);
				} else if (key == arrowRightKey && volume < 1) {
					SetMasterVolume(volume + 0.05);
				}
				break;
			case 2:
				if (key == enterKey || mousePressed) {
					*keyPressed = exitKey;
					key = 256;
				}
				break;
		}

		
		
		BeginDrawing();
			DrawRectangle((widthMap-512)/2, (heightMap-256)/2, 512+borderMenu, 256+borderMenu, WHITE);
			DrawRectangle((widthMap-512)/2+borderMenu, (heightMap-256)/2+borderMenu, 512-borderMenu, 256-borderMenu, (Color){30,30,30,255});

			if (state == 0)
				DrawRectangle(startPosButtomMenuX - 1, yButMenu1-1 , lenghtButtomMenu+2, 42, RED);
			
			DrawRectangle(startPosButtomMenuX, yButMenu1 , lenghtButtomMenu, heightButMenu, BLACK);
			DrawText("RESUME", widthMap/2-65, yButMenu1 + 5 , heightButMenu-5, RED);

			if (state == 1)
				DrawRectangle(startPosButtomMenuX-1, yButMenu2-1 , lenghtButtomMenu+2, heightButMenu+2, RED);
			DrawRectangle(startPosButtomMenuX, yButMenu2 , lenghtButtomMenu, heightButMenu, BLACK);
	
			memset((void*)buf, 0, 10);	
			strcat(buf, "AUDIO: ");
			if ((int)(GetMasterVolume()*100)%5 != 0)
				audioLevel = GetMasterVolume() * 100 + (5 - (int)(GetMasterVolume()*100)%5);
			else
				audioLevel = GetMasterVolume()*100;

			strcat(buf,itoa(audioLevel, tmp, 10));
			strcat(buf, "%");
			DrawText(buf, widthMap/2-65-10, yButMenu2 + 5 , heightButMenu-5, RED);

			if (state == 2)
				DrawRectangle( startPosButtomMenuX-1, yButMenu3-1 , lenghtButtomMenu+2, 42, RED);
			DrawRectangle(startPosButtomMenuX, yButMenu3 , lenghtButtomMenu, heightButMenu, BLACK);
			DrawText("EXIT GAME", widthMap/2-95, yButMenu3 + 5 , heightButMenu-5, RED);
			
		EndDrawing();
	}
}
void verifyKeyPressing(int keyMap[], int nKeyRegistered, int *feed_speedA, int *feed_speedB,
					int *keyA, int *keyB, int *keyPressed, int speed_coeficient, bool pause)
{
	if (pressedAValidKey(keyMap, nKeyRegistered)) {
		*keyPressed = GetKeyPressed();		

		switch(*keyPressed) {
			case 87: 
					*feed_speedA = -speed_coeficient;	
					*keyA = *keyPressed;
					break;
			case 83: 
					*feed_speedA = +speed_coeficient;
					*keyA = *keyPressed;
					break;
			case 265:
					*feed_speedB = -speed_coeficient;
					*keyB = *keyPressed;
					break;
			case 264:
					*feed_speedB = +speed_coeficient;
					*keyB = *keyPressed;
					break;
			case 80:
				if (!pause)	pause = true;
				else
					pause = false;		
				break;
			case 256:
				menuModeManage(keyPressed);
				HideCursor();
				break;
		}
	}
}
