float PlayerSize = 2.0;
float PlayerPosX = 10.0;
float PlayerPosY = 150.0;
float PlayerLastPosX = 10.0;
float PlayerLastPosY = 10.0;
float PlayerSpeed = 500.0;
float PlayerGravity = 900.0;
float PlayerFallSpeed = 0.0;
float PlayerJumpHeight = -340.0;
float PlayerAnimationSpeed = 12.5;

Sprite PlayerRun;
Sprite PlayerIdle;
Sprite PlayerJump;
bool PlayerFlipVertical = false;
Sprite* PlayerAnimation;

void PlayerInit()
{
	PlayerRun = SpriteLoad("Sprites/Cyborg/Cyborg_run.bmp", 6, true);
	PlayerIdle = SpriteLoad("Sprites/Cyborg/Cyborg_idle.bmp", 4, true);
	PlayerJump = SpriteLoad("Sprites/Cyborg/Cyborg_jump.bmp", 4, true);

	bool PlayerFlip = false;
	Sprite* Animation = &PlayerIdle;
}

void PlayerCollision(float PlayerWidth, float PlayerHeight)
{
	float DiffX = PlayerPosX - PlayerLastPosX;
	float DiffY = PlayerLastPosY - PlayerPosY;

	PlayerTile = (int)((PlayerPosX + PlayerWidth* 2) / Width * TileMapX + 1 * DiffX) + (int)((PlayerPosY + DiffY - PlayerHeight) / Height * TileMapY + 2) * TileMapX;
	unsigned char Tile = TileMap[PlayerTile];
	if (Tile != 255)
	{
	//	PlayerPosX = PlayerLastPosX;
	//	PlayerPosY = PlayerLastPosY;
	}
}

void PlayerUpdate()
{
	PlayerLastPosX = PlayerPosX;
	PlayerLastPosY = PlayerPosY;

	PlayerAnimation = &PlayerIdle;

	PlayerPosY += PlayerFallSpeed * DeltaTime;
	if (Key[SDL_SCANCODE_A])
	{
		PlayerPosX -= PlayerSpeed * DeltaTime;
		PlayerFlipVertical = true;
	}
	if (Key[SDL_SCANCODE_D])
	{
		PlayerPosX += PlayerSpeed * DeltaTime;
		PlayerFlipVertical = false;
	}

	PlayerFallSpeed += PlayerGravity * DeltaTime;

	int Ground = Height - 200;

	if (PlayerPosY >= Ground)
	{
		PlayerFallSpeed = 0.0;

		if (Key[SDL_SCANCODE_SPACE])
			PlayerFallSpeed = PlayerJumpHeight;

		if (Key[SDL_SCANCODE_D] || Key[SDL_SCANCODE_A])
		{
			PlayerAnimation = &PlayerRun;
		}
	}
	else
	{
		PlayerAnimation = &PlayerJump;
	}

	float Size = ((float)RenderHeight * 0.001) * PlayerSize;
	PlayerCollision(Size, Size);
	SpriteUpdate(PlayerAnimationSpeed, PlayerAnimation);
	SpriteDraw(PlayerPosX, PlayerPosY, Size, Size, false, PlayerFlipVertical, PlayerAnimation, &Framebuffer);
}

void PlayerDeInit()
{
	glFreeImage(&PlayerRun.Image);
	glFreeImage(&PlayerIdle.Image);
	glFreeImage(&PlayerJump.Image);
}