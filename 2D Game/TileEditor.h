glWindow TileWindow;
glImage TileFramebuffer;

int TileRenderWidth = 1600;
int TileRenderHeight = 900;

void TileEditorInit()
{
	glCreateWindow("Tile Editor", Width, 30, Width, Height, GL_WINDOW_ASPECT_DYNAMIC, &TileWindow);
	GetRenderArea(Width, Height, &TileRenderWidth, &TileRenderHeight);
	glCreateImage(Width, Height, GL_IMAGE_FRAME_BUFFER, &TileFramebuffer);
}

void TileEditorDestroy()
{
	glFreeImage(&TileFramebuffer);
	glDestroy(&TileWindow);
}

int SelectedTile = 0;

void TileEditorDrawMap()
{
	float MapW = TileRenderWidth * 0.9;
	float MapH = TileRenderHeight * 0.9;

	float ScaleY = (float)MapH / TileMapY;
	float ScaleX = (float)MapW / ((float)MapW / (float)ScaleY);

	for (int x = 0; x < TileMapX; x++)
	{
		for (int y = 0; y < TileMapY; y++)
		{
			int Index = x + y * TileMapX;
			unsigned char Tile = TileMap[Index];

			float MinX = (x * ScaleX);
			float MinY = (y * ScaleY);

			float MaxX = ScaleX;
			float MaxY = ScaleY;


			if (MouseX <= MinX + MaxX && MouseX >= MinX &&
				MouseY <= MinY + MaxY && MouseY >= MinY)
			{
				
				if (Mouse & SDL_BUTTON_LMASK)
					TileMap[Index] = SelectedTile;
				if (Mouse & SDL_BUTTON_RMASK)
					TileMap[Index] = 255;
				if (Mouse & SDL_BUTTON_MMASK)
					SelectedTile = TileMap[Index];
			}

		//	if (Tile == 255)
				glDrawQuad(MinX + 2, MinY + 2, MaxX - 2, MaxY - 2, GL_COLOR_24(240, 240, 240), &TileFramebuffer);
		//	else
			{
				if (Tile < TILE_COUNT)
					glDrawTexture(MinX, MinY, MaxX, MaxY, &Tiles[Tile], false, false, &TileFramebuffer);
			}
		}
	}
}

void TileEditorDrawTiles()
{
	float StartX = TileRenderWidth * 0.9;
	float StartY = 0.0;

	float ScaleX = TileRenderWidth - StartX;
	float ScaleY = TileRenderHeight * 0.9;
	float Size = ScaleX * ScaleY;

	float SizePerTile = Size / TILE_COUNT;
	float ScalePerTile = sqrtf(SizePerTile);

//	float TilePerX = ScaleX / ScalePerTile;
//	float TilePerY = ScaleY / ScalePerTile;

	int Index = 0;
	for (float x = StartX; x < TileFramebuffer.Width; x += ScalePerTile)
	{
		for (float y = StartY; y < ScaleY; y += ScalePerTile)
		{
			if (Index >= TILE_COUNT)
				break;

		//	unsigned char Tile = TileMap[Index];

			float MinX = floorf(x * ScalePerTile);
			float MinY = floorf(y * ScalePerTile);

			float MaxX = ceilf(ScalePerTile);
			float MaxY = ceilf(ScalePerTile);
			
			if (Mouse & SDL_BUTTON_LMASK &&
				MouseX <= x + MaxX && MouseX >= x &&
				MouseY <= y + MaxY && MouseY >= y)
			{
				SelectedTile = Index;
			}

			

			float Brightness = 1.0;
			if (SelectedTile == Index)
			{
				glDrawQuad(x, y, ScalePerTile, ScalePerTile, GL_COLOR_24(120, 120, 120), &TileFramebuffer);
				glDrawTexture(x, y, ScalePerTile, ScalePerTile, &TilesDark[Index], false, false, &TileFramebuffer);
			}
			else
			{
				glDrawQuad(x, y, ScalePerTile, ScalePerTile, GL_COLOR_24(240, 240, 240), &TileFramebuffer);
				glDrawTexture(x, y, ScalePerTile, ScalePerTile, &Tiles[Index], false, false, &TileFramebuffer);
			}
				
			Index++;
		}
	}
}

void TileEditorUpdate()
{
	int Event = glPollEvent(&TileWindow);
	switch (Event)
	{
	case GL_EVENT_RESIZE:
		glResizeImage(TileWindow.Width, TileWindow.Height, &TileFramebuffer);
		GetRenderArea(TileWindow.Width, TileWindow.Height, &TileRenderWidth, &TileRenderHeight);
		break;

	default:
		break;
	}

//	if (glGetWindowFocus(&TileWindow) || ForceUpdate)
	{
		glClearImage(100, &TileFramebuffer);
	//	float ScaleY = (float)TileWindow.Height / TileMapY;
	//	float ScaleX = (float)TileWindow.Width / ((float)TileWindow.Width / (float)ScaleY);
	//	MapDraw(false, ScaleX, ScaleY, &TileFramebuffer);
		TileEditorDrawMap();
		TileEditorDrawTiles();

		glDrawFramebuffer(&TileWindow, &TileFramebuffer);
	}	
}