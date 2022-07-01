glWindow EditorWindow;
glFramebuffer EditorFramebuffer;

void MapEditorInit()
{
	glCreateWindow("Editor", 1920, 30, Width, Height, &EditorWindow, GL_WINDOW_DYNAMIC_ASPECT);
	glCreateFramebuffer(Width, Height, &EditorFramebuffer);
}

void MapEditorDestroy()
{
	glDestroyFramebuffer(&EditorFramebuffer);
	glDestroy(&EditorWindow);
}

int SelectedTile = 81;

void DrawMap()
{
	float MapW = EditorFramebuffer.Width * 0.9;
	float MapH = EditorFramebuffer.Height * 0.9;

	float ScaleY = (float)MapH / MapY;
	float ScaleX = (float)MapW / ((float)MapW / (float)ScaleY);

	int MouseX;
	int MouseY;
	Uint32 Mouse = SDL_GetMouseState(&MouseX, &MouseY);

	for (int x = 0; x < MapX; x++)
	{
		for (int y = 0; y < MapY; y++)
		{
			int Index = x + y * MapX;
			unsigned char Tile = Map[Index];

			float MinX = (x * ScaleX);
			float MinY = (y * ScaleY);

			float MaxX = ScaleX;
			float MaxY = ScaleY;


			if (MouseX <= MinX + MaxX && MouseX >= MinX &&
				MouseY <= MinY + MaxY && MouseY >= MinY)
			{
				
				if (Mouse & SDL_BUTTON_LMASK)
					Map[Index] = SelectedTile;
				if (Mouse & SDL_BUTTON_RMASK)
					Map[Index] = 255;
				if (Mouse & SDL_BUTTON_MMASK)
					SelectedTile = Map[Index];
			}

			if (Tile == 255)
				glDrawQuad(MinX + 1, MinY + 1, MaxX - 1, MaxY - 1, 255, 255, 255, &EditorFramebuffer);
			else
			{
				if (Tile < TILE_COUNT)
					glDrawTexture(MinX, MinY, MaxX, MaxY, &Tiles[Tile], 1.0, false, false, &EditorFramebuffer);
				else
				{
				//	printf("%d\n", Tile - TILE_COUNT);
					
				 	float ScaleX = MaxX / (Sprites[Tile - TILE_COUNT].Image.Height);
					float ScaleY = MaxY / (Sprites[Tile - TILE_COUNT].Image.Height);
				
					SpriteDraw(MinX, MinY, ScaleX, ScaleX, false, false, &Sprites[Tile - TILE_COUNT], &EditorFramebuffer);
				}
			}
			//	glDrawTexture(MinX, MinY, MaxX, MaxY, &Tiles[Tile], 1.0, false, false, &EditorFramebuffer);
		}
	}
}

void DrawStaticTiles()
{
	float StartX = EditorFramebuffer.Width * 0.9;
	float StartY = 0.0;

	float ScaleX = EditorFramebuffer.Width - StartX;
	float ScaleY = EditorFramebuffer.Height * 0.9;
	float Size = ScaleX * ScaleY;

	float SizePerTile = Size / TILE_COUNT;
	float ScalePerTile = sqrtf(SizePerTile);

//	float TilePerX = ScaleX / ScalePerTile;
//	float TilePerY = ScaleY / ScalePerTile;

	int MouseX;
	int MouseY;
	Uint32 Mouse = SDL_GetMouseState(&MouseX, &MouseY);

	int Index = 0;
	for (float x = StartX; x < EditorFramebuffer.Width; x += ScalePerTile)
	{
		for (float y = StartY; y < ScaleY; y += ScalePerTile)
		{
			if (Index >= TILE_COUNT)
				break;

		//	unsigned char Tile = Map[Index];

			float MinX = (x * ScalePerTile);
			float MinY = (y * ScalePerTile);

			float MaxX = ScalePerTile;
			float MaxY = ScalePerTile;

			if (Mouse & SDL_BUTTON_RIGHT &&
				MouseX <= x + MaxX && MouseX >= x &&
				MouseY <= y + MaxY && MouseY >= y)
			{
				SelectedTile = Index;
			}

			float Brightness = 1.0;
			if (SelectedTile == Index)
				Brightness = 0.65;

			glDrawTexture(x, y, ScalePerTile, ScalePerTile, &Tiles[Index], Brightness, false, false, &EditorFramebuffer);
			Index++;
		}
	}
}

void DrawAnimationTiles()
{
	float StartX = 0.0;
	float StartY = EditorFramebuffer.Height * 0.9;

	float ScaleX = EditorFramebuffer.Width;
	float ScaleY = EditorFramebuffer.Height - StartY;
	float Size = ScaleX * ScaleY;

	float SizePerTile = Size / SPRITE_COUNT;
	float ScalePerTile = sqrtf(SizePerTile);

	int MouseX;
	int MouseY;
	Uint32 Mouse = SDL_GetMouseState(&MouseX, &MouseY);

	int Index = 0;
	for (float x = StartX; x < EditorFramebuffer.Width; x += ScalePerTile)
	{
		for (float y = StartY; y < EditorFramebuffer.Height; y += ScalePerTile)
		{
			if (Index >= SPRITE_COUNT)
				break;

		//	unsigned char Tile = Map[Index];

			float MaxX = ScalePerTile * 2;
			float MaxY = ScalePerTile * 2;

			float MinX = (x * MaxX);
			float MinY = (y * MaxY);

			

			if (Mouse & SDL_BUTTON_RIGHT &&
				MouseX <= x + MaxX && MouseX >= x &&
				MouseY <= y + MaxY && MouseY >= y)
			{
				SelectedTile = Index + TILE_COUNT;
			}

			float Brightness = 1.0;
			if (SelectedTile - TILE_COUNT == Index)
				glDrawQuad(x, y, ScalePerTile, ScalePerTile, 200, 200, 200, &EditorFramebuffer);

			ScaleX = ScalePerTile / Sprites[Index].Image.Width * 2;
		 	ScaleY = ScalePerTile / Sprites[Index].Image.Height * 2;
		//	glDrawQuad(x, y, Scale, Scale, 255, 0, 255, &EditorFramebuffer);
		//	glDrawQuad(x, y, ScalePerTile * 2, ScalePerTile * 2, x * 0.1, y * 0.11, y * 0.1, &EditorFramebuffer);
			SpriteDraw(x, y, ScaleX, ScaleX, false, false, &Sprites[Index], &EditorFramebuffer);
		//	SpriteDraw(0, 0, 2, 2, 12.5, false, false, &Sprites[0]);
		//	glDrawTexture(x, y, ScalePerTile, ScalePerTile, &Tiles[Index], Brightness, false, false, &EditorFramebuffer);
			Index++;
		}
	}
}

void MapEditorUpdate()
{
	int Event = glPollEvent(&EditorWindow);
	switch (Event)
	{
	case GL_EVENT_RESIZE:
		glResizeFramebuffer(EditorWindow.Width, EditorWindow.Height, &Framebuffer);
		break;

	default:
		break;
	}

	glClearFramebuffer(35, &EditorFramebuffer);
	DrawMap();
	DrawStaticTiles();
	DrawAnimationTiles();

	glDrawFramebuffer(&EditorWindow, &EditorFramebuffer);
}