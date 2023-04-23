#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <float.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>

#define GL_PIXEL_CLIPPING
#define GL_REMOVE_TRANSPARENT_PIXEL
#include "glWindow.h"

float Aspect = 1.777777777777778;
int Width = 1600 / 2;
int Height = 900 / 2;

int RenderWidth = 1600;
int RenderHeight = 900;

glWindow Window;
glImage Framebuffer;

int PlayerTile = 0;

#include "Helper.h"
#include "Audio.h"
#include "Sprite.h"
#include "Map.h"
#include "Player.h"
#include "TileEditor.h"
//#include "SpriteEditor.h"



/*
void DrawSpriteAt(int x, int y, int Index, int Max, int ScaleX, int ScaleY, glImage* Texture, bool Flip)
{
	int Offset = Texture->Width / Max;
	
	int Add;
	if (Flip) Add = x - (Offset * 0.5);
	else	  Add = x + (Offset * 0.5);
	glDrawSubTexture(Add, y, Offset * ScaleX, Texture->Height * ScaleY, Index * Offset, 0, Offset, Texture->Height, Texture, 1.0, Flip, &Framebuffer);
}

void DrawSprite(int x, int y, int Max, int ScaleX, int ScaleY, glImage* Texture, bool Flip)
{
	static float Index = 0.0;
	Index += DeltaTime * 12.5;

	if ((int)Index >= Max || (int)Index < 0)
		Index = 0;

	int Offset = Texture->Width / Max;

	int Add;
	if (Flip) Add = x - (Offset * 0.5);
	else	  Add = x + (Offset * 0.5);
	glDrawSubTexture(Add, y, Offset * ScaleX, Texture->Height * ScaleY, (int)Index * Offset, 0, Offset, Texture->Height, Texture, 1.0, Flip, &Framebuffer);
}
*/



int main(int argc, char** argv)
{
	glCreateWindow("GL Software", 0, 30, Width, Height, GL_WINDOW_ASPECT_DYNAMIC, &Window);
	GetRenderArea(Window.Width, Window.Height, &RenderWidth, &RenderHeight);
	glCreateImage(Width, Height, GL_IMAGE_FRAME_BUFFER, &Framebuffer);

	float x = 0.2 + 0.1;
	printf("%.10f\n", x);

	MapInit();
	TileEditorInit();
//	SpriteEditorInit();
	
	glImage BackgroundOG = glLoadImage("Sprites/Backgrounds/Background.bmp", true);
	glImageRGBA(GL_COLOR_24(255, 255, 255), &BackgroundOG);

	glImage Background; 
	glCreateImage(Width, Height, GL_IMAGE_RGBA, &Background);

	glCopyImage(&Background, &BackgroundOG);
	glRescaleImage(Width, Height, &Background);
	
	Sprite Money = SpriteLoad("Sprites/AnimatedObjects/Money.bmp", 6, true);
	Sprite Transporter = SpriteLoad("Sprites/AnimatedObjects/Transporter.bmp", 4, true);
	Sprite Entry = SpriteLoad("Sprites/AnimatedObjects/Entry.bmp", 8, true);
	Sprite Chest = SpriteLoad("Sprites/AnimatedObjects/Chest.bmp", 8, true);
	Sprite Skull = SpriteLoad("Sprites/AnimatedObjects/Skull.bmp", 10, true);
	
	AudioBuffer Audio = AudioLoad("Audio/MoonBase.wav");
	AudioPlay(&Audio, false);

	PlayerInit();

//	float PlayerSpeed = 500.0;
//	float PX = 0;
//	float PY = Height - 300;
//	float PS = 3.0;
//	float PlayerFallSpeed = 1.0;

	int a = 0;
	int b = 0;
	int Delta = 0;

	bool Run = true;
	while (Run)
	{
		a = SDL_GetTicks();
		Delta = a - b;
		int Event = glPollEvent(&Window);
		PollEvents();

		switch (Event)
		{
		case GL_EVENT_QUIT:
			Run = false;
			break;

		case GL_EVENT_RESIZE:
			glGetWindowSize(&Width, &Height, &Window);
			
			GetRenderArea(Window.Width, Window.Height, &RenderWidth, &RenderHeight);

			glCopyImage(&Background, &BackgroundOG);
			glRescaleImage(RenderWidth, RenderHeight, &Background);

			glResizeImage(Width, Height, &Framebuffer);
			glResizeImage(Width, Height, &MapFramebuffer);
			glResizeImage(Width, Height, &MapFramebufferPrev);
			ForceUpdate = true;
			break;

		default:
			break;
		}

		

		//	float ScaleX = (float)Width / (float)TileMapX;
		//	float ScaleY = (float)Height / (float)TileMapY;

	//	if (glGetWindowFocus(&SpriteWindow) || ForceUpdate || glGetWindowFocus(&TileWindow))
	//		MapUpdateTiles(ScaleX, ScaleY);
	//	MapUpdateSprites(ScaleX, ScaleY);
		
		if (glGetWindowFocus(&TileWindow) || ForceUpdate)
		{
			TileEditorUpdate();
			float ScaleY = (float)RenderHeight / TileMapY;
			float ScaleX = (float)RenderWidth / ((float)RenderWidth / (float)ScaleY);
			MapUpdateTiles(ScaleX, ScaleY);
		}
	//	SpriteEditorUpdate();

		
		/*
		AnimationLen = 4;
		Animation = &CyborgIdle;

		PY += PlayerFallSpeed * DeltaTime;
		if (Key[SDL_SCANCODE_A])
		{
			PX -= PlayerSpeed * DeltaTime;
			PlayerFlip = true;
		}
		if (Key[SDL_SCANCODE_D])
		{
			PX += PlayerSpeed * DeltaTime;
			PlayerFlip = false;
		}
		
		PlayerFallSpeed += 900.0 * DeltaTime;

		int Ground = Height - 200;

		if (PY >= Ground)
		{
			PlayerFallSpeed = 0.0;

			if (Key[SDL_SCANCODE_SPACE])
				PlayerFallSpeed = -340.0;

			if (Key[SDL_SCANCODE_D] || Key[SDL_SCANCODE_A])
			{
				AnimationLen = 6;
				Animation = &CyborgRun;
			}
		}
		else
		{
			AnimationLen = 4;
			Animation = &CyborgJump;
		}
		*/
		glClearImage(66, &Framebuffer);
	//	glDrawTexture(0, 0, Window.Width, Window.Height, &Background, 0, 0, &Framebuffer);
	//	glDrawBackground(&Background, &Framebuffer);
		if (Background.Width != Framebuffer.Width ||
			Background.Height != Framebuffer.Height)
			glDrawBackgroundLazy(&Background, &Framebuffer);
		else
			glDrawBackground(&Background, &Framebuffer);

		glDrawLayer(&MapFramebuffer, &Framebuffer);
	
		PlayerUpdate();

	//	float Size = ((float)RenderHeight  * 0.001) * PS;
	//	SpriteUpdate(12.5, Animation);
	//	SpriteDraw(PX, PY, Size, Size, false, PlayerFlip, Animation, &Framebuffer);
	//	SpriteDraw(400, 400, 1, 1, 12.5, false, false, &Money);
	//	SpriteDraw(600, 400, 4, 4, 12.5, false, false, &Transporter);
	//	SpriteDraw(1000, 400, 4, 4, 10.0, false, false, &Entry);
	//	SpriteDraw(800, Ground + 32 * 2, 3, 3, 10.0, false, true, &Chest);
	//	SpriteDraw(500, Ground + 32 * 2, 3, 3, 15.0, false, true, &Skull);

	//	if (Delta > 1000.0 / 24.0)
		{
			glDrawFramebuffer(&Window, &Framebuffer);
			b = a;
		}
		

		CalcFps("2D Game");
		GetDeltaTime();

		ForceUpdate = false;
	}

	PlayerDeInit();

	AudioFree(&Audio);

	glFreeImage(&Background);
	glFreeImage(&Money.Image);
	glFreeImage(&Transporter.Image);
	glFreeImage(&Entry.Image);
	glFreeImage(&Chest.Image);
	glFreeImage(&Skull.Image);
	
	MapDestroy();

	glFreeImage(&Framebuffer);
	TileEditorDestroy();
//	SpriteEditorDestroy();
	glDestroy(&Window);

	return 0;
}