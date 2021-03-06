#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GL_COLOR_32(R,G,B,A) (((unsigned int)(A)<<24) | ((unsigned int)(B)<<16) | ((unsigned int)(G)<<8) | ((unsigned int)(R)<<0))
#define GL_COLOR_24(R,G,B,A) (((unsigned int)(255)<<24) | ((unsigned int)(B)<<16) | ((unsigned int)(G)<<8) | ((unsigned int)(R)<<0))
#define GL_BYTE_FLOAT (1.0 / 255.0)

typedef struct
{
	int Width;
	int Height;
	float Aspect;

	SDL_Window* Window;
	SDL_Surface* Surface;
	SDL_Renderer* Renderer;
	SDL_Texture* Swapchain;
	SDL_Event Event;
} glWindow;

typedef struct
{
	int Width;
	int Height;
	int Size;
	int ImageType;
	unsigned char* Data;
} glImage;

typedef enum
{
	GL_IMAGE_RGB = 0x3,
	GL_IMAGE_RGBA = 0x4,
} glImageType;

typedef enum
{
	GL_WINDOW_DYNAMIC_ASPECT = 0x0,
	GL_WINDOW_ASPECT_16_9 = 0x1,
	GL_WINDOW_ASPECT_4_3 = 0x2,
} glWindowFlags;

void glCreateWindow(const char* Title, int x, int y, int Width, int Height, int WindowFlags, glWindow* Window)
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

	Window->Width = Width;
	Window->Height = Height;

	switch (WindowFlags)
	{
	case GL_WINDOW_DYNAMIC_ASPECT:
		Window->Aspect = 0.0;
		break;

	case GL_WINDOW_ASPECT_16_9:
		Window->Aspect = 1.777777777777778;
		break;

	case GL_WINDOW_ASPECT_4_3:
		Window->Aspect = 1.333333333333333;
		break;

	default:
		break;
	}

	Window->Window = SDL_CreateWindow(Title, x, y, Width, Height, SDL_WINDOW_RESIZABLE);
	Window->Surface = SDL_GetWindowSurface(Window->Window);
	Window->Renderer = SDL_CreateRenderer(Window->Window, -1, SDL_RENDERER_SOFTWARE);
	Window->Swapchain = SDL_CreateTexture(Window->Renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, Width, Height);
}

void glDestroy(glWindow* Window)
{
	SDL_DestroyTexture(Window->Swapchain);
	SDL_DestroyRenderer(Window->Renderer);
	SDL_DestroyWindow(Window->Window);
	SDL_Quit();
}

typedef enum
{
	GL_EVENT_QUIT = 0x1,
	GL_EVENT_RESIZE = 0x2
} glEventType;

int glPollEvent(glWindow* Window)
{
	while (SDL_PollEvent(&Window->Event))
	{
		if (Window->Event.type == SDL_QUIT ||
			Window->Event.type == SDL_WINDOWEVENT && Window->Event.window.event == SDL_WINDOWEVENT_CLOSE)
			return GL_EVENT_QUIT;

		if (Window->Event.window.event == SDL_WINDOWEVENT_RESIZED)
		{
			SDL_GetWindowSize(Window->Window, &Window->Width, &Window->Height);

			if (Window->Aspect != 0.0)
				Window->Height = Window->Width / Window->Aspect;

			SDL_SetWindowSize(Window->Window, Window->Width, Window->Height);

			SDL_DestroyRenderer(Window->Renderer);
			Window->Renderer = SDL_CreateRenderer(Window->Window, -1, SDL_RENDERER_SOFTWARE);
			SDL_RenderSetLogicalSize(Window->Renderer, Window->Width, Window->Height);
			SDL_Rect Rect;
			Rect.x = 0;
			Rect.y = 0;
			Rect.w = Window->Width;
			Rect.h = Window->Height;
			SDL_RenderSetViewport(Window->Renderer, &Rect);
			SDL_DestroyTexture(&Window->Swapchain);
			Window->Swapchain = SDL_CreateTexture(Window->Renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, Window->Width, Window->Height);
			return GL_EVENT_RESIZE;
		}
	}

	return 0;
}

extern inline void glSetWindowTitle(glWindow* Window, const char* Title)
{
	SDL_SetWindowTitle(Window->Window, Title);
}

extern inline void glGetWindowSize(glWindow* Window, int* Width, int* Height)
{
	SDL_GetWindowSize(Window->Window, Width, Height);
}

extern inline void glSetWindowSize(glWindow* Window, int Width, int Height)
{
	SDL_SetWindowSize(Window->Window, Width, Height);
}

extern inline int glGetWindowFocus(glWindow* Window)
{
	return (SDL_GetWindowFlags(Window->Window) & SDL_WINDOW_INPUT_FOCUS);
}

void glFreeImage(glImage* Image)
{
	Image->ImageType = 0;
	Image->Width = 0;
	Image->Height = 0;
	free(Image->Data);
	Image->Data = NULL;
}

void glResizeImage(glImage* Image, int NewWidth, int NewHeight)
{
	glImage NewImage;
	NewImage.ImageType = Image->ImageType;
	NewImage.Width = NewWidth;
	NewImage.Height = NewHeight;
	NewImage.Size = NewWidth * NewHeight * Image->ImageType;
	NewImage.Data = malloc(NewImage.Size);

	float ImageX = 0;
	float ImageY = 0;

	float ImageW = (float)Image->Width / (float)NewWidth;
	float ImageH = (float)Image->Height / (float)NewHeight;

	for (int x = 0; x < NewWidth; x++)
	{
		for (int y = 0; y < NewHeight; y++)
		{
			int Index0 = ((int)ImageX + (int)ImageY * Image->Width) * Image->ImageType;
			int Index1 = (x + y * NewWidth) * Image->ImageType;

			NewImage.Data[Index1] = Image->Data[Index0];
			NewImage.Data[Index1 + 1] = Image->Data[Index0 + 1];
			NewImage.Data[Index1 + 2] = Image->Data[Index0 + 2];
			if (Image->ImageType == GL_IMAGE_RGBA)
				NewImage.Data[Index1 + 3] = Image->Data[Index0 + 3];

			ImageY += ImageH;
		}

		ImageY = 0;
		ImageX += ImageW;
	}

	free(Image->Data);
	*Image = NewImage;
}

void glImageRGB(glImage* Image)
{
	if (Image->ImageType == GL_IMAGE_RGB)
		return;

	glImage NewImage = *Image;
	NewImage.ImageType = GL_IMAGE_RGB;
	NewImage.Size = NewImage.Width * NewImage.Width * NewImage.ImageType;
	NewImage.Data = malloc(NewImage.Size);

	int j = 0;
	for (int i = 0; i < Image->Size; i += 4)
	{
		NewImage.Data[j] = Image->Data[i];
		NewImage.Data[j + 1] = Image->Data[i + 1];
		NewImage.Data[j + 2] = Image->Data[i + 2];
		j += 3;
	}

	free(Image->Data);
	*Image = NewImage;
}

void glImageRGBA(glImage* Image)
{
	if (Image->ImageType == GL_IMAGE_RGBA)
		return;

	glImage NewImage = *Image;
	NewImage.ImageType = GL_IMAGE_RGBA;
	NewImage.Size = NewImage.Width * NewImage.Width * NewImage.ImageType;
	NewImage.Data = malloc(NewImage.Size);

	int j = 0;
	for (int i = 0; i < Image->Size; i += 3)
	{
		NewImage.Data[j] = Image->Data[i];
		NewImage.Data[j + 1] = Image->Data[i + 1];
		NewImage.Data[j + 2] = Image->Data[i + 2];
		NewImage.Data[j + 3] = 255;
		j += 4;
	}

	free(Image->Data);
	*Image = NewImage;
}

extern inline void glImageBrightness(float Brightness, glImage* Image)
{
	for (int i = 0; i < Image->Size; i++)
		Image->Data[i] = ((float)Image->Data[i] * Brightness);
}

void glCreateFramebuffer(int Width, int Height, glImage* Framebuffer)
{
	Framebuffer->Width = Width;
	Framebuffer->Height = Height;
	Framebuffer->ImageType = GL_IMAGE_RGBA;
	Framebuffer->Size = Width * Height * Framebuffer->ImageType;
	Framebuffer->Data = (unsigned char*)malloc(Framebuffer->Size);
	memset(Framebuffer->Data, 255, Framebuffer->Size);
}

void glResizeFramebuffer(int Width, int Height, glImage* Framebuffer)
{
	Framebuffer->Width = Width;
	Framebuffer->Height = Height;
	Framebuffer->Size = Width * Height * Framebuffer->ImageType;
	Framebuffer->Data = (unsigned char*)realloc(Framebuffer->Data, Framebuffer->Size);
	memset(Framebuffer->Data, 255, Framebuffer->Size);
}

void glDrawFramebuffer(glWindow* Window, glImage* Framebuffer)
{
	void* Pixels;
	int Pitch;

	SDL_LockTexture(Window->Swapchain, NULL, &Pixels, &Pitch);
	memcpy(Pixels, Framebuffer->Data, Pitch * Framebuffer->Height);
	SDL_UnlockTexture(Window->Swapchain);

	SDL_RenderClear(Window->Renderer);
	SDL_RenderCopy(Window->Renderer, Window->Swapchain, NULL, NULL);
	SDL_RenderPresent(Window->Renderer);
}

void glClearFramebuffer(int Shade, glImage* Framebuffer)
{
	memset(Framebuffer->Data, Shade, Framebuffer->Size);
}

//must be equal to framebuffer
void glDrawBackground(glImage* Image, glImage* Framebuffer)
{
	memcpy(Framebuffer->Data, Image->Data, Framebuffer->Size);
}

void glDrawLayer(glImage* Image, glImage* Framebuffer)
{
	for (int i = 0; i < Framebuffer->Size; i += 4)
	{
		if (Image->Data[i + 3] != 0)
		{
			Framebuffer->Data[i] = Image->Data[i];
			Framebuffer->Data[i + 1] = Image->Data[i + 1];
			Framebuffer->Data[i + 2] = Image->Data[i + 2];
			Framebuffer->Data[i + 3] = Image->Data[i + 3];
		}
	}
}

extern inline void glDrawPixel(int x, int y, unsigned int Color, glImage* Framebuffer)
{
	int Index = (x + y * Framebuffer->Width) * Framebuffer->ImageType;
	if (Index >= 0 && Index < Framebuffer->Size &&
		x >= 0 && x < Framebuffer->Width &&
		y >= 0 && y < Framebuffer->Height)
	{
		if (((Color >> 24) & 0xFF) != 0)
		{
			Framebuffer->Data[Index] = ((Color >> 0) & 0xFF);
			Framebuffer->Data[Index + 1] = ((Color >> 8) & 0xFF);
			Framebuffer->Data[Index + 2] = ((Color >> 16) & 0xFF);
			Framebuffer->Data[Index + 3] = ((Color >> 24) & 0xFF);
		}		
	}
}

extern inline void glDrawRect(int MinX, int MinY, int MaxX, int MaxY, unsigned int Color, glImage* Framebuffer)
{
	for (int x = MinX; x < MaxX; x++)
	{
		for (int y = MinY; y < MaxY; y++)
		{
			glDrawPixel(x, y, Color, Framebuffer);
		}
	}
}

extern inline void glDrawQuad(int PosX, int PosY, int ScaleX, int ScaleY, unsigned int Color, glImage* Framebuffer)
{
	glDrawRect(PosX, PosY, PosX + ScaleX, PosY + ScaleY, Color, Framebuffer);
}

extern inline void glDrawPoint(int Scale, int PosX, int PosY, unsigned int Color, glImage* Framebuffer)
{
	float Half = Scale * 0.5;

	glDrawRect(PosX - Half, PosY - Half, PosX + Half, PosY + Half, Color, Framebuffer);
}

void glDrawSubTexture(int PosX, int PosY, int ScaleX, int ScaleY, int StartX, int StartY, int EndX, int EndY, glImage* Image, int FlipVert, int FlipHorz, glImage* Framebuffer)
{
	float ImageX;
	float ImageY;

	float ImageW;
	float ImageH;

	float ImageStartY;

	if (FlipHorz)
	{
		ImageX = StartX + EndX - (1.0 / (float)ScaleX);
		ImageW = -(float)EndX / (float)ScaleX;
	}
	else
	{
		ImageX = StartX;
		ImageW = (float)EndX / (float)ScaleX;
	}

	if (FlipVert)
	{
		ImageStartY = StartY + EndY - (1.0 / (float)ScaleY);
		ImageY = ImageStartY;
		ImageH = -(float)EndY / (float)ScaleY;
	}
	else
	{
		ImageStartY = 0;
		ImageY = StartY;
		ImageH = (float)EndY / (float)ScaleY;
	}

	for (int x = PosX; x < PosX + ScaleX; x++)
	{
		for (int y = PosY; y < PosY + ScaleY; y++)
		{
			int Index = ((int)ImageX + (int)ImageY * Image->Width) * Image->ImageType;

			ImageY += ImageH;

		//	if ((Image->ChannelCount == 4 && Image->Data[Index + 3] > 0.5) || (Image->ChannelCount != 4))
				
			if (Image->ImageType == GL_IMAGE_RGB)
				glDrawPixel(x, y, GL_COLOR_24(Image->Data[Index], Image->Data[Index + 1], Image->Data[Index + 2]), Framebuffer);
			else
				glDrawPixel(x, y, GL_COLOR_32(Image->Data[Index], Image->Data[Index + 1], Image->Data[Index + 2], Image->Data[Index + 3]), Framebuffer);
		}

		ImageY = ImageStartY;
		ImageX += ImageW;
	}
}


extern inline void glDrawTexture(int PosX, int PosY, int ScaleX, int ScaleY, glImage* Image, int FlipVert, int FlipHorz, glImage* Framebuffer)
{
	glDrawSubTexture(PosX, PosY, ScaleX, ScaleY, 0, 0, Image->Width, Image->Height, Image, FlipVert, FlipHorz, Framebuffer);
}

void glDrawLine(int x1, int y1, int x2, int y2, unsigned int Color, glImage* Framebuffer)
{
	int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
	dx = x2 - x1;
	dy = y2 - y1;
	dx1 = fabs(dx);
	dy1 = fabs(dy);
	px = 2 * dy1 - dx1;
	py = 2 * dx1 - dy1;
	if (dy1 <= dx1)
	{
		if (dx >= 0)
		{
			x = x1;
			y = y1;
			xe = x2;
		}
		else
		{
			x = x2;
			y = y2;
			xe = x1;
		}
		glDrawPixel(x, y, Color, Framebuffer);
		for (i = 0; x < xe; i++)
		{
			x = x + 1;
			if (px < 0)
			{
				px = px + 2 * dy1;
			}
			else
			{
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
				{
					y = y + 1;
				}
				else
				{
					y = y - 1;
				}
				px = px + 2 * (dy1 - dx1);
			}
			glDrawPixel(x, y, Color, Framebuffer);
		}
	}
	else
	{
		if (dy >= 0)
		{
			x = x1;
			y = y1;
			ye = y2;
		}
		else
		{
			x = x2;
			y = y2;
			ye = y1;
		}
		glDrawPixel(x, y, Color, Framebuffer);
		for (i = 0; y < ye; i++)
		{
			y = y + 1;
			if (py <= 0)
			{
				py = py + 2 * dx1;
			}
			else
			{
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
				{
					x = x + 1;
				}
				else
				{
					x = x - 1;
				}
				py = py + 2 * (dx1 - dy1);
			}
			glDrawPixel(x, y, Color, Framebuffer);
		}
	}
}

//Image
#pragma pack(push, 1)

typedef struct
{
	uint16_t Type;
	uint32_t Size;
	uint16_t Resv1;
	uint16_t Resv2;
	uint32_t OffBits;
} BitmapFileHeader;

typedef struct
{
	uint32_t Size;
	int32_t Width;
	int32_t Height;
	uint16_t Planes;
	uint16_t Bits;
	uint32_t Compression;
	uint32_t SizeImage;
	int32_t XPelsPerMeter;
	int32_t YPelsPerMeter;
	uint32_t ClrUsed;
	uint32_t ClrImportant;
} BitmapInfoHeader;

typedef struct
{
	uint8_t Red;
	uint8_t Green;
	uint8_t Blue;
	uint8_t Alpha;
} BitmapColorHeader;

#pragma pack(pop)

glImage glLoadImage(const char* Filename, int FlipVertical)
{
	BitmapFileHeader ImageHeader;
	BitmapInfoHeader ImageInfo;
	uint8_t* ImageData;

	FILE* File = fopen(Filename, "rb");
	if (!File)
		printf("Not Able to Load: %s\n", Filename);

	fread(&ImageHeader, sizeof(BitmapFileHeader), 1, File);
	fread(&ImageInfo, sizeof(BitmapInfoHeader), 1, File);

	fseek(File, ImageHeader.OffBits, SEEK_SET);

	uint32_t ImageSize = ImageInfo.Width * ImageInfo.Height * (ImageInfo.Bits == 32 ? 4 : 3);
	ImageInfo.SizeImage = ImageSize;

	ImageData = (uint8_t*)malloc(ImageSize);
	fread(ImageData, ImageSize, 1, File);

	fclose(File);

	/*
	printf("Width: %d\n", ImageInfo.Width);
	printf("Height: %d\n", ImageInfo.Height);
	printf("Size: %d\n", ImageInfo.SizeImage);
	printf("Bits: %d\n", ImageInfo.Bits);
	*/

	for (int32_t i = 0; i < ImageInfo.Width * ImageInfo.Height; i++)
	{
		int32_t Index = i * (ImageInfo.Bits == 32 ? 4 : 3);

		uint8_t B = ImageData[Index];
		uint8_t R = ImageData[Index + 2];

		ImageData[Index] = R;
		ImageData[Index + 2] = B;
	}

	if (FlipVertical)
	{
		const size_t Stride = ImageInfo.Width * (ImageInfo.Bits == 32 ? 4 : 3);
		unsigned char* Row = malloc(Stride);
		unsigned char* Low = ImageData;
		unsigned char* High = &ImageData[(ImageInfo.Height - 1) * Stride];

		for (; Low < High; Low += Stride, High -= Stride)
		{
			memcpy(Row, Low, Stride);
			memcpy(Low, High, Stride);
			memcpy(High, Row, Stride);
		}
		free(Row);
	}

	glImage Image;
	Image.Width = ImageInfo.Width;
	Image.Height = ImageInfo.Height;
	Image.ImageType = (ImageInfo.Bits == 32 ? 4 : 3);
	Image.Size = ImageSize;
	Image.Data = ImageData;

	return Image;
}