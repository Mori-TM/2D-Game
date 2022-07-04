#define true 1
#define false 0
typedef int bool;

bool ForceUpdate = true;

//Fps
int Frame = 0;
int Final_Time;
int Init_Time = 0;
int Final_Fps = 0;
char CharFPS[64];

void CalcFps(const char* Title)
{
	Frame++;
	Final_Time = time(NULL);
	if (Final_Time - Init_Time > 0)
	{
		Final_Fps = Frame / (Final_Time - Init_Time);
		Frame = 0;
		Init_Time = Final_Time;

		sprintf(CharFPS, "%s fps: %d ms: %f", Title, Final_Fps, 1000.0 / Final_Fps);
		glSetWindowTitle(&Window, CharFPS);
	}
}

float DeltaTime = 0.0;
float LastTime = 0.0;

float GetDeltaTime()
{
	float CurrentTime = (float)SDL_GetTicks() / 1000.0;
	DeltaTime = CurrentTime - LastTime;
	LastTime = CurrentTime;

	return DeltaTime;
}

int MouseX;
int MouseY;
Uint32 Mouse;
Uint8* Key;

void PollEvents()
{
	Mouse = SDL_GetMouseState(&MouseX, &MouseY);
	Key = SDL_GetKeyboardState(NULL);
}