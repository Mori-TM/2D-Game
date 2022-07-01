typedef struct
{
	float Index;
	float AnimationLen;
	int LastTicks;
	glImage Image;
} Sprite;

Sprite SpriteLoad(const char* FileName, int AnimationLen, bool FlipVert)
{
	Sprite Sprite;
	Sprite.Index = 0.0;
	Sprite.AnimationLen = AnimationLen;
	Sprite.LastTicks = 0;
	Sprite.Image = glLoadImage(FileName, FlipVert);

	return Sprite;
}

void SpriteUpdate(float AnimationSpeed, Sprite* Sprite)
{
	if (Sprite->LastTicks != SDL_GetTicks())
		Sprite->Index += DeltaTime * AnimationSpeed;

	if ((int)Sprite->Index >= Sprite->AnimationLen || (int)Sprite->Index < 0)
		Sprite->Index = 0;
}

void SpriteDraw(int x, int y, float ScaleY, float ScaleX, bool FlipVert, bool FlipHorz, Sprite* Sprite, glFramebuffer* Framebuffer)
{
	int Offset = Sprite->Image.Width / Sprite->AnimationLen;

	int AddX;
	if (FlipHorz) AddX = x - (Offset * 0.5);
	else		  AddX = x + (Offset * 0.5);
	int AddY;
	if (FlipVert) AddY = y - (Offset * 0.5);
	else		  AddY = y + (Offset * 0.5);

	glDrawSubTexture(AddX, AddY, Offset * ScaleX, Sprite->Image.Height * ScaleY, (int)Sprite->Index * Offset, 0, Offset, Sprite->Image.Height, &Sprite->Image, 1.0, FlipVert, FlipHorz, Framebuffer);
}