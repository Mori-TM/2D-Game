#define TILE_SIZE 32
#define TILE_COUNT 81
glImage Tiles[TILE_COUNT];
glImage TilesDark[TILE_COUNT];

int TileMapX = 32;
int TileMapY = 18;
int TileMapS = 576;

unsigned char TileMap[] =
{
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 003, 004, 004, 005, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 014, 013, 013, 016, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 014, 013, 013, 016, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 014, 013, 013, 016, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	004, 004, 004, 004, 004, 004, 004, 004, 004, 004, 004, 004, 004, 004, 004, 004, 004, 004, 004, 004, 004, 004, 004, 004, 004, 004, 004, 004, 004, 004, 004, 004,
};

typedef struct
{
	float x;
	float y;
	float ScaleX;
	float ScaleY;

	int Sprite;
} SpriteTile;

#define SPRITE_MAP_MAX_SIZE 128
int SpriteMapCount = 0;
SpriteTile SpriteMap[SPRITE_MAP_MAX_SIZE];

#define SPRITE_COUNT 13
Sprite Sprites[SPRITE_COUNT];

glImage MapFramebuffer;
glImage MapFramebufferPrev;

void MapInit()
{
	glCreateImage(RenderWidth, RenderHeight, GL_IMAGE_FRAME_BUFFER, &MapFramebuffer);
	glCreateImage(RenderWidth, RenderHeight, GL_IMAGE_FRAME_BUFFER, &MapFramebufferPrev);

	memset(SpriteMap, 0, sizeof(SpriteTile) * SPRITE_MAP_MAX_SIZE);

//	FILE* File = fopen("TileMap.txt", "wb");
//	fwrite(TileMap, 1, TileMapS, File);
//	fwrite(&SpriteMapCount, sizeof(int), 1, File);
//	fwrite(SpriteMap, sizeof(SpriteTile), SPRITE_MAP_MAX_SIZE, File);
//	fclose(File);

	FILE* File = fopen("Map.txt", "rb");
	if (File)
	{
		fread(TileMap, 1, TileMapS, File);
		fread(&SpriteMapCount, sizeof(int), 1, File);
		fread(SpriteMap, sizeof(SpriteTile), SPRITE_MAP_MAX_SIZE, File);
		fclose(File);
	}	

	Sprites[0] = SpriteLoad("Sprites/AnimatedObjects/Card.bmp", 8, true);
	Sprites[1] = SpriteLoad("Sprites/AnimatedObjects/Chest.bmp", 8, true);
	Sprites[2] = SpriteLoad("Sprites/AnimatedObjects/Entry.bmp", 8, true);
	Sprites[3] = SpriteLoad("Sprites/AnimatedObjects/Hammer.bmp", 8, true);
	Sprites[4] = SpriteLoad("Sprites/AnimatedObjects/Money.bmp", 6, true);
	Sprites[5] = SpriteLoad("Sprites/AnimatedObjects/Platform.bmp", 8, true);
	Sprites[6] = SpriteLoad("Sprites/AnimatedObjects/Screen1.bmp", 4, true);
	Sprites[7] = SpriteLoad("Sprites/AnimatedObjects/Screen2.bmp", 4, true);
	Sprites[8] = SpriteLoad("Sprites/AnimatedObjects/Skull.bmp", 10, true);
	Sprites[9] = SpriteLoad("Sprites/AnimatedObjects/Transporter.bmp", 4, true);
	Sprites[10] = SpriteLoad("Sprites/AnimatedObjects/Transporter1.bmp", 4, true);
	Sprites[11] = SpriteLoad("Sprites/AnimatedObjects/Transporter2.bmp", 4, true);
	Sprites[12] = SpriteLoad("Sprites/AnimatedObjects/Transporter3.bmp", 4, true);

	for (int i = 0; i < TILE_COUNT; i++)
	{
		char Name[2048];
		sprintf(Name, "Sprites/Tiles/IndustrialTile_%d.bmp", i + 1);
		Tiles[i] = glLoadImage(Name, true);
		glCreateImage(Tiles[i].Width, Tiles[i].Height, Tiles[i].Type, &TilesDark[i]);
		glCopyImage(&TilesDark[i], &Tiles[i]);
		glImageBrightness(0.5, &TilesDark[i]);
	}
}

void MapUpdateTiles(float ScaleX, float ScaleY)
{
	glClearImage(0, &MapFramebuffer);

	for (int x = 0; x < TileMapX; x++)
	{
		for (int y = 0; y < TileMapY; y++)
		{
			int TileIndex = x + y * TileMapX;
			
			unsigned char Tile = TileMap[TileIndex];

			if (TileIndex == PlayerTile)
			{
				glDrawQuad(floorf(x * ScaleX), floorf(y * ScaleY), ceilf(ScaleX), ceilf(ScaleY), GL_COLOR_24(0, 0, 0), &MapFramebuffer);
			}
			else
			{
				if (Tile < TILE_COUNT)
					glDrawTexture(floorf(x * ScaleX), floorf(y * ScaleY), ceilf(ScaleX), ceilf(ScaleY), &Tiles[Tile], false, false, &MapFramebuffer);
			}

			
		}
	}

	memcpy(MapFramebufferPrev.Data, MapFramebuffer.Data, MapFramebuffer.Size);
}

void MapUpdateSprites(float ScaleX, float ScaleY)
{
	glDrawBackground(&MapFramebufferPrev, &MapFramebuffer);

	for (int i = 0; i < SPRITE_COUNT; i++)
		SpriteUpdate(10.0, &Sprites[i]);

	for (int i = 0; i < SpriteMapCount; i++)
	{
		float Scale = Sprites[SpriteMap[i].Sprite].Image.Height * ScaleY;

		SpriteDraw(SpriteMap[i].x, SpriteMap[i].y, SpriteMap[i].ScaleX, SpriteMap[i].ScaleY, false, false, &Sprites[SpriteMap[i].Sprite], &MapFramebuffer);
	}
		
}

void MapDestroy()
{
//	memset(SpriteMap, 0, sizeof(SpriteTile) * SPRITE_MAP_MAX_SIZE);

	glFreeImage(&MapFramebuffer);
	glFreeImage(&MapFramebufferPrev);

//	FILE* File = fopen("TileMap.txt", "wb");
//	fwrite(TileMap, 1, TileMapS, File);
//	fwrite(&SpriteMapCount, sizeof(int), 1, File);
//	fwrite(SpriteMap, sizeof(SpriteTile), SPRITE_MAP_MAX_SIZE, File);
//	fclose(File);

	for (int i = 0; i < 81; i++)
		glFreeImage(&Tiles[i]);
}