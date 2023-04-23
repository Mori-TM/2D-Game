glWindow SpriteWindow;
glImage SpriteFramebuffer;

void SpriteEditorInit()
{
	glCreateWindow("Sprite Editor", Width, Height + 30, Width, Height, GL_WINDOW_ASPECT_DYNAMIC, &SpriteWindow);
	glCreateImage(Width, Height, GL_IMAGE_FRAME_BUFFER, &SpriteFramebuffer);
}

void SpriteEditorDestroy()
{
	glFreeImage(&SpriteFramebuffer);
	glDestroy(&SpriteWindow);
}

int SelectedSprite = 0;

void DrawAnimationSprites()
{
	float StartX = 0.0;
	float StartY = SpriteFramebuffer.Height * 0.9;

	float ScaleX = SpriteFramebuffer.Width;
	float ScaleY = SpriteFramebuffer.Height - StartY;
	float Size = ScaleX * ScaleY;

	float SizePerSprite = Size / SPRITE_COUNT;
	float ScalePerSprite = sqrtf(SizePerSprite);

	int Index = 0;
	for (float x = StartX; x < SpriteFramebuffer.Width; x += ScalePerSprite)
	{
		for (float y = StartY; y < SpriteFramebuffer.Height; y += ScalePerSprite)
		{
			if (Index >= SPRITE_COUNT)
				break;

			//	unsigned char Sprite = TileMap[Index];

			float MaxX = ScalePerSprite * 2;
			float MaxY = ScalePerSprite * 2;

			float MinX = (x * MaxX);
			float MinY = (y * MaxY);



			if (Mouse & SDL_BUTTON_LMASK &&
				MouseX <= x + MaxX && MouseX >= x &&
				MouseY <= y + MaxY && MouseY >= y)
			{
				SelectedSprite = Index;
			}

			float Brightness = 1.0;
			if (SelectedSprite == Index)
				glDrawQuad(x, y, ScalePerSprite, ScalePerSprite, GL_COLOR_24(200, 200, 200), &SpriteFramebuffer);

			ScaleX = ScalePerSprite / Sprites[Index].Image.Width * 2;
			ScaleY = ScalePerSprite / Sprites[Index].Image.Height * 2;
			//	glDrawQuad(x, y, Scale, Scale, 255, 0, 255, &SpriteFramebuffer);
			//	glDrawQuad(x, y, ScalePerSprite * 2, ScalePerSprite * 2, x * 0.1, y * 0.11, y * 0.1, &SpriteFramebuffer);
			SpriteDraw(x, y, ScaleX, ScaleX, false, false, &Sprites[Index], &SpriteFramebuffer);
			//	SpriteDraw(0, 0, 2, 2, 12.5, false, false, &Sprites[0]);
			//	glDrawTexture(x, y, ScalePerSprite, ScalePerSprite, &Sprites[Index], Brightness, false, false, &SpriteFramebuffer);
			Index++;
		}
	}
}

bool WasPlaced = false;
float DeltaRX = 0.0;
float DeltaRY = 0.0;

float DeltaLX = 0.0;
float DeltaLY = 0.0;

float DeltaSX = 0.0;
float DeltaSY = 0.0;

void PlaceSprite()
{
	if (WasPlaced && (Mouse & SDL_BUTTON_LMASK) &&
		MouseX < SpriteWindow.Width * 0.9 &&
		MouseY < SpriteWindow.Height * 0.9 &&
		SpriteMapCount < 128)
	{
		WasPlaced = false;
		SpriteMap[SpriteMapCount].x = MouseX;
		SpriteMap[SpriteMapCount].y = MouseY;
		SpriteMap[SpriteMapCount].ScaleX = 1.0;
		SpriteMap[SpriteMapCount].ScaleY = 1.0;
		SpriteMap[SpriteMapCount].Sprite = SelectedSprite;
		SpriteMapCount++;

		DeltaRX = MouseX;
		DeltaRY = MouseY;
	}
	
	if (Mouse & SDL_BUTTON_LMASK)
	{
		if (SpriteMapCount > 0)
		{
			SpriteMap[SpriteMapCount - 1].ScaleX = (int)(fabsf((DeltaRX - MouseX) / (SpriteWindow.Width * 0.025)));
			SpriteMap[SpriteMapCount - 1].ScaleY = SpriteMap[SpriteMapCount - 1].ScaleX;
		//	SpriteMap[SpriteMapCount - 1].ScaleY = fabsf((DeltaRX - MouseX) / (SpriteWindow.Height * 0.25));

			printf("s: %f %f\n", SpriteMap[SpriteMapCount - 1].ScaleX, SpriteMap[SpriteMapCount - 1].ScaleY);
		}
	}
	else
	{
		
		WasPlaced = true;
	}

	if (SpriteMapCount > 0)
	{
		if (Mouse & SDL_BUTTON_RMASK)
		{

			SpriteMap[SpriteMapCount - 1].x = DeltaSX + ((MouseX - DeltaLX));
			SpriteMap[SpriteMapCount - 1].y = DeltaSY + ((MouseY - DeltaLY));
			printf("p: %f %f\n", SpriteMap[SpriteMapCount - 1].x, SpriteMap[SpriteMapCount - 1].y);
		}
		else
		{
			DeltaSX = SpriteMap[SpriteMapCount - 1].x;
			DeltaSY = SpriteMap[SpriteMapCount - 1].y;

			DeltaLX = MouseX;
			DeltaLY = MouseY;
		}
	}
	
}

void SpriteEditorUpdate()
{
	int Event = glPollEvent(&SpriteWindow);
	switch (Event)
	{
	case GL_EVENT_RESIZE:
		glResizeImage(SpriteWindow.Width, SpriteWindow.Height, &Framebuffer);
		break;

	default:
		break;
	}

	if (glGetWindowFocus(&SpriteWindow) || ForceUpdate)
	{
		glClearImage(35, &SpriteFramebuffer);
	//	glDrawBackground(&MapFramebuffer, &SpriteFramebuffer);

		float ScaleX = (float)SpriteWindow.Width * 0.9;
		float ScaleY = (float)SpriteWindow.Height * 0.9;
		glDrawTexture(0, 0, ScaleX, ScaleY, &MapFramebuffer, false, false, &SpriteFramebuffer);
	//	MapDraw(true, ScaleX, ScaleY, &SpriteFramebuffer);
		DrawAnimationSprites();
		PlaceSprite();

		glDrawFramebuffer(&SpriteWindow, &SpriteFramebuffer);
	}
}