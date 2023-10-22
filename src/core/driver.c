#include "driver.h"
#include <SDL2/SDL_syswm.h>
/*
#ifndef _WIN32
#error "This code only works on Windows"
#endif

#include <windows.h>*/

DRIVER *driver = NULL;

/* ----- Driver ----- */

SDL_Color hexColorToRGBA(Uint32 hex)
{
	SDL_Color color;
	color.r = (hex >> 24) & 0xFF;
	color.g = (hex >> 16) & 0xFF;
	color.b = (hex >> 8)  & 0xFF;
	color.a = hex & 0xFF;

	return color;
}

bool Driver_init(char *title, Uint32 width, Uint32 height, Uint32 bgColor, Uint32 windowflags)
{
	bool success = false;
	if (driver == NULL) {
		ARRAY *ressources		= NULL;
		SDL_Window	*window		= NULL;
		SDL_Renderer *renderer	= NULL;
		
		ressources = (ARRAY *) Array_new(NO_LIMITED);
		if (ressources == NULL) { LOG_ERROR("Echec de l'allocation de la structure ARRAY"); goto EndGraphics; }

		window = (SDL_Window *) SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,  SDL_WINDOW_SHOWN|windowflags);
		if (window == NULL) {
			LOG_ERROR("Could not create SDL window: %s", SDL_GetError());
			goto EndGraphics;
		}
		
		//SDL_SetSurfaceBlendMode(SDL_GetWindowSurface(window), SDL_BLENDMODE_BLEND);
		
		//Drvier_windowTransparent();

		renderer = (SDL_Renderer *) SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
		if (renderer == NULL) {
			LOG_ERROR("Could not create SDL renderer: %s\n", SDL_GetError());
			goto EndGraphics;
		}

		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

		driver = (DRIVER *) calloc(1, sizeof(DRIVER));
		if (driver == NULL) { LOG_ERROR("Echec de l'allocation de la structure Driver"); goto EndGraphics; }

		Driver_openAudio(driver);

		success = true;
	EndGraphics:
		if (success == false) {
			if (ressources != NULL)	Array_free(ressources, true);
			if (renderer != NULL)	SDL_DestroyRenderer(renderer);
			if (window != NULL)		SDL_DestroyWindow(window);
			driver = false;
		} else {
			driver->width		= width;
			driver->height		= height;
			driver->bgColor		= bgColor;
			driver->ressources	= ressources;
			driver->window		= window;
			driver->renderer	= renderer;
		}
	} else {
		success = true;
	}
	return success;
}

void Driver_free()
{
	if (driver != NULL) {
		Driver_closeAudio(driver);
		if (driver->iconSurface != NULL) SDL_FreeSurface(driver->iconSurface);
		if (driver->ressources != NULL) Array_free(driver->ressources, true);
		if (driver->renderer != NULL)   SDL_DestroyRenderer(driver->renderer);
		if (driver->window != NULL)		SDL_DestroyWindow(driver->window);
		free(driver);
		driver = false;
	}
}

DRIVER *Driver_get()
{
	return driver;
}

bool Drvier_makeWindowTransparent()
{
	if (driver) {
		 // Set background color to magenta and clear screen
		SDL_SetRenderDrawColor(driver->renderer, 255, 0, 255, 255);
		SDL_RenderClear(driver->renderer);
		
		// Add window transparency (Magenta will be see-through)
		// Get window handle
		SDL_SysWMinfo wmInfo;
		SDL_VERSION(&wmInfo.version);  // Initialize wmInfo
		SDL_GetWindowWMInfo(driver->window, &wmInfo);
		HWND hWnd = wmInfo.info.win.window;
		// Change window type to layered
		SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
		// Set transparency color
		return SetLayeredWindowAttributes(hWnd, RGB(255, 0, 255), 0, LWA_COLORKEY);
	}
	return false;
}

void Driver_minimizeWindow()
{
	if (driver) SDL_MinimizeWindow(driver->window);
}

char *Driver_getTitle()
{
	return (char *) SDL_GetWindowTitle(driver->window);
}

void Driver_setTitle(char *title)
{
	if (driver != NULL) {
		SDL_SetWindowTitle(driver->window, title);
	}
}

Sint32 Driver_getWidth()
{
	if (driver != NULL) {
		return driver->width;
	} else {
		return 0;
	}
}

Sint32 Driver_getHeight()
{
	if (driver != NULL) {
		return driver->height;
	} else {
		return 0;
	}
}

void Driver_setBgColor(Uint32 bgColor)
{
	if (driver != NULL) driver->bgColor = bgColor;
}

Uint32 Driver_getBgColor()
{
	if (driver != NULL) return driver->bgColor;
	return 0;
}

bool Driver_setIcon(char *iconName)
{
	if (driver != NULL) {
		char imagePath[100];

		if (driver->iconSurface != NULL) SDL_FreeSurface(driver->iconSurface);

		sprintf(imagePath, "./res/images/%s", iconName);
		driver->iconSurface = IMG_Load(imagePath);
		if (driver->iconSurface == NULL) {
			sprintf(imagePath, "../res/images/%s", iconName);
			driver->iconSurface = IMG_Load(imagePath);
		}
		if (driver->iconSurface == NULL) {
			LOG_ERROR("Image \"%s\" introuvable", imagePath);
			return false;
		}
		SDL_SetWindowIcon(driver->window, driver->iconSurface);
	}
	return false;
}

IMAGE *Driver_loadImage(char *imageName)
{
	IMAGE *image = NULL;
	if (driver != NULL) {
		/* On importe l'image si cela n'a pas dejà été effectué, sinon on la réccupère de la liste */
		DATA *data = Array_getFromKey(driver->ressources, imageName);
		if (data == NULL) {
			char imagePath[100];
			SDL_Surface* surface;

			sprintf(imagePath, "./res/images/%s", imageName);
			surface = IMG_Load(imagePath);
			if (surface == NULL) {
				sprintf(imagePath, "../res/images/%s", imageName);
				surface = IMG_Load(imagePath);
			}
			if (surface == NULL) {
				LOG_ERROR("Image \"%s\" introuvable", imagePath);
				return NULL;
			}

			SDL_Texture *texture = Driver_createTextureFromSurface(surface);
			image = Image_new(texture, surface->w, surface->h);
			Array_append(driver->ressources, Data_new(ID_DEFAULT, imageName, IMAGE_TYPE, image, freeImage));

			SDL_FreeSurface(surface);

			SUCCESS("Importation de %s", imagePath);
		} else {
			if (data->type == IMAGE_TYPE) {
				image = (IMAGE *) data->buffer;
			}
		}
	}
	return image;
}

void freeImage(void *buffer, TypeData type)
{
	if (buffer != NULL && type == IMAGE_TYPE) {
		Image_free(buffer);
	}
}

TTF_Font *Driver_loadFont(char *fontName, Uint16 fontSize)
{
	TTF_Font* font = NULL;
	if (driver != NULL) {
		char sizeNameFont[100];
		sprintf(sizeNameFont, "%s:%d", fontName, fontSize);
		/* On importe la police si cela n'a pas dejà été effectué, sinon on la réccupère de la liste */
		DATA *data = Array_getFromKey(driver->ressources, sizeNameFont);
		if (data == NULL) {
			char fontPath[100];
			sprintf(fontPath, "./res/fonts/%s", fontName);
			font = TTF_OpenFont(fontPath, fontSize);
			if (font == NULL) {
				sprintf(fontPath, "../res/fonts/%s", fontName);
				font = TTF_OpenFont(fontPath, fontSize);
			}
			if (font == NULL) {
				LOG_ERROR("Police \"%s\" introuvable", fontPath);
				return NULL;
			}
			Array_append(driver->ressources, Data_new(ID_DEFAULT, sizeNameFont, FONT_TYPE, font, freeFont));
			SUCCESS("Importation de %s", fontPath);
		} else {
			if (data->type == FONT_TYPE) {
				font = (TTF_Font *) data->buffer;
			}
		}
	}
	return font;
}

void freeFont(void *buffer, TypeData type)
{
	if (buffer != NULL && type == FONT_TYPE) {
		TTF_CloseFont(buffer);
	}
}

void Driver_clear()
{
	if (driver != NULL) {
		//SDL_SetRenderDrawBlendMode(driver->renderer, SDL_BLENDMODE_BLEND);
		SDL_Color c = hexColorToRGBA(driver->bgColor);
		SDL_SetRenderDrawColor(driver->renderer, c.r, c.g, c.b, c.a); // Set the renderer's draw color
		SDL_RenderClear(driver->renderer);

		if (driver->bgColor == SDL_ALPHA_TRANSPARENT)
			Drvier_makeWindowTransparent();
	}
}

SDL_Renderer *Driver_getRenderer()
{
	if (driver != NULL) {
		return driver->renderer;
	}
	return NULL;
}

void Driver_drawColor(Uint32 color)
{
	if (driver != NULL) {
		Uint8* c = (Uint8 *) &color;
		//SDL_SetRenderDrawBlendMode(driver->renderer, SDL_BLENDMODE_ADD);
		SDL_SetRenderDrawColor(driver->renderer, c[3], c[2], c[1], c[0]);
	}
}

void Driver_drawRect(SDL_Rect *rect, bool fill)
{
	if (driver != NULL) {
		if (fill) SDL_RenderFillRect(driver->renderer, rect);
		else SDL_RenderDrawRect(driver->renderer, rect);
	}
}

void Driver_present()
{
	if (driver != NULL) {
		SDL_RenderPresent(driver->renderer);
	}
}

void Driver_copy(SDL_Texture *texture, SDL_Rect *src, SDL_Rect *dest)
{
	if (driver != NULL && texture != NULL) {
		if (SDL_RenderCopy(driver->renderer, texture, src, dest) != 0) // Copy the final texture to the buffer
			LOG_ERROR("Texture SDL_RenderCopy : %s", SDL_GetError());
	}
}

SDL_Texture *Driver_createTextureFromSurface(SDL_Surface *surface)
{
	SDL_Texture *newTexture = NULL;
	if (driver != NULL) {		
		surface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);
		SDL_Texture* staticTexture = SDL_CreateTextureFromSurface(driver->renderer, surface);
		newTexture = SDL_CreateTexture(driver->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, surface->w, surface->h);
		if (staticTexture != NULL && newTexture != NULL) {
			SDL_SetTextureBlendMode(newTexture, SDL_BLENDMODE_NONE);
			SDL_SetRenderTarget(driver->renderer, newTexture);
			SDL_RenderCopy(driver->renderer, staticTexture, NULL, NULL);
			SDL_SetRenderTarget(driver->renderer, NULL);
			SDL_DestroyTexture(staticTexture);
		} else
			LOG_ERROR("SDL_CreateTexture : Texture not create");
	}
	return newTexture;
}

SDL_Texture *Driver_createTexture(Sint32 w, Sint32 h)
{
	SDL_Texture *newTexture = NULL;
	if (driver != NULL && w > 0 && h > 0) {
		newTexture = SDL_CreateTexture(driver->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
		if (newTexture != NULL) {
			SDL_SetTextureBlendMode(newTexture, SDL_BLENDMODE_BLEND);
		} else
			LOG_ERROR("SDL_CreateTexture : Texture not create");
	}
	return newTexture;
}

void Driver_setTarget(SDL_Texture *texture)
{
	if (driver != NULL) {
		if (SDL_SetRenderTarget(driver->renderer, texture) != 0)
			LOG_ERROR("Texture SDL_SetRenderTarget : %s", SDL_GetError());
	}
}

void Driver_copyText(char* text, TTF_Font* font, Uint32 textColor, SDL_Rect *rect, bool centerX, bool centerY)
{
	if (driver != NULL) {
		SDL_Color c = hexColorToRGBA(textColor);
		SDL_Surface* surfaceMessage = TTF_RenderText_Blended(font, text, c);
		SDL_Texture* message = SDL_CreateTextureFromSurface(driver->renderer, surfaceMessage);

		SDL_Rect src;
		SDL_Rect dest;

		src.x = 0;
		src.y = 0;
		dest.x = rect->x;
		dest.y = rect->y;

		if (rect->w != 0 && surfaceMessage->w > rect->w) {
			src.w = rect->w;
			if (centerX == true) src.x += surfaceMessage->w/2 - src.w/2;
		} else {
			src.w = surfaceMessage->w;
			if (rect->w != 0 && centerX == true) dest.x += rect->w/2 - src.w/2;
		}
			
		if (rect->h != 0 && surfaceMessage->h > rect->h) {
			src.h = rect->h;
			if (centerY == true) src.y += surfaceMessage->h/2 - src.h/2;
		} else {
			src.h = surfaceMessage->h;
			if (rect->h != 0 && centerY == true) dest.y += rect->h/2 - src.h/2;
		}

		dest.w = src.w;
		dest.h = src.h;

		SDL_RenderCopy(driver->renderer, message, &src, &dest);
		SDL_FreeSurface(surfaceMessage);
		SDL_DestroyTexture(message);
	}
}

void Driver_printf(Sint32 x, Sint32 y, TTF_Font *font, Uint32 textColor, char *text, ... )
{
	if (driver != NULL) {
		static char buffer[1000];
		SDL_Rect dest;
		
		/* on formate la chaîne à afficher avec les arguments */
		va_list args;
		va_start(args, text);
		vsprintf(buffer, text, args);
		va_end(args);
		
		SDL_Color c = hexColorToRGBA(textColor);
		SDL_Surface* surfaceMessage = TTF_RenderText_Blended(font, buffer, c);
		SDL_Texture* message = SDL_CreateTextureFromSurface(driver->renderer, surfaceMessage);

		dest.x = x;
		dest.y = y;
		dest.w = surfaceMessage->w;
		dest.h = surfaceMessage->h;
		
		SDL_RenderCopy(driver->renderer, message, NULL, &dest);
		SDL_FreeSurface(surfaceMessage);
		SDL_DestroyTexture(message);
	}
}


/* ----- AUDIO ----- */

bool Driver_openAudio()
{
	bool success = false;
	if (driver != NULL && driver->audioInited == false) {
		// definition des propriétés audio
		int frequency	= 22050;
		Uint16 format	= AUDIO_S16;
		int channels	= 2;
		int chunksize	= 4096;

		if (Mix_OpenAudio(frequency, format, channels, chunksize) < 0) {
			LOG_ERROR("Erreur d'ouverture audio %s", SDL_GetError());
		} else {
			success = true;
			driver->audioInited = true;
			driver->volume = MIX_MAX_VOLUME;
		}
	}
	return success;
}

void Driver_closeAudio()
{
	if (driver != NULL && driver->audioInited == true) {
		Mix_CloseAudio();
		driver->audioInited = false;
	}
}

void Driver_setVolume(Sint32 volume)
{
	if (driver != NULL) {
		driver->volume = (volume > MIX_MAX_VOLUME) ? MIX_MAX_VOLUME : volume;
	}
}

Sint32 Driver_getVolume()
{
	if (driver != NULL) {
		return driver->volume;
	}
	return -1;
}

Mix_Chunk* Driver_loadSound(char *soundName)
{
	Mix_Chunk* sound = NULL;
    if (driver != NULL && soundName != NULL && driver->audioInited == true) {
        /* On importe la musique si cela n'a pas dejà été effectué, sinon on la réccupère de la liste */
		DATA *data = Array_getFromKey(driver->ressources, soundName);
		if (data == NULL) {
			char soundPath[100];

			sprintf(soundPath, "./res/sounds/%s", soundName);
			sound = Mix_LoadWAV(soundPath);
			if (sound == NULL) {
				sprintf(soundPath, "../res/sounds/%s", soundName);
				sound = Mix_LoadWAV(soundPath);
			}
			if (sound == NULL) {
				LOG_ERROR("Sound \"%s\" introuvable", soundPath);
				return NULL;
			}
		
			Array_append(driver->ressources, Data_new(ID_DEFAULT, soundName, SOUND_TYPE, sound, freeSound));

			SUCCESS("Importation de %s", soundPath);
		} else {
			if (data->type == SOUND_TYPE) {
				sound = (Mix_Chunk *) data->buffer;
			}
		}
    }
	return sound;
}

void freeSound(void *buffer, TypeData type)
{
	if (buffer != NULL && type == SOUND_TYPE) {
		Mix_FreeChunk(buffer);
	}
}

void Driver_playSound(char *soundName, int loops)
{
	if (driver != NULL && soundName != NULL &&driver->audioInited == true) {
		DATA *data = Array_getFromKey(driver->ressources, soundName);
		Mix_Chunk *sound = NULL;
		if (data != NULL && data->type == MUSIC_TYPE) {
			sound = (Mix_Chunk *) data->buffer;
		} else {
			sound = Driver_loadSound(soundName);
		}

		if (sound != NULL) {
			data->id = Mix_PlayChannel(-1, sound, loops);
			Mix_Volume(data->id, driver->volume);
		}
	}
}

void Driver_pauseSound(char *soundName)
{
	if (driver != NULL && soundName != NULL && driver->audioInited == true) {
		DATA *data = Array_getFromKey(driver->ressources, soundName);
		if (data != NULL && data->type == MUSIC_TYPE && data->buffer != NULL) {
			Mix_Pause(data->id);
		}
	}
}

void Driver_resumeSound(char *soundName)
{
	if (driver != NULL && soundName != NULL && driver->audioInited == true) {
		DATA *data = Array_getFromKey(driver->ressources, soundName);
		if (data != NULL && data->type == MUSIC_TYPE && data->buffer != NULL) {
			Mix_Resume(data->id);
		}
	}
}

Mix_Music* Driver_loadMusic(char *musicName)
{
	Mix_Music* music = NULL;
    if (driver != NULL && musicName != NULL && driver->audioInited == true) {
        /* On importe la musique si cela n'a pas dejà été effectué, sinon on la réccupère de la liste */
		DATA *data = Array_getFromKey(driver->ressources, musicName);
		if (data == NULL) {
			char musicPath[100];

			sprintf(musicPath, "./res/sounds/%s", musicName);
			music = Mix_LoadMUS(musicPath);
			if (music == NULL) {
				sprintf(musicPath, "../res/sounds/%s", musicName);
				music = Mix_LoadMUS(musicPath);
			}
			if (music == NULL) {
				LOG_ERROR("Music \"%s\" introuvable", musicPath);
				return NULL;
			}
		
			Array_append(driver->ressources, Data_new(ID_DEFAULT, musicName, MUSIC_TYPE, music, freeMusic));

			SUCCESS("Importation de %s", musicPath);
		} else {
			if (data->type == MUSIC_TYPE) {
				music = (Mix_Music *) data->buffer;
			}
		}
    }
	return music;
}

void freeMusic(void *buffer, TypeData type)
{
	if (buffer != NULL && type == MUSIC_TYPE) {
		Mix_FreeMusic(buffer);
	}
}

void Driver_playMusic(char *musicName, int loops)
{
    if (driver != NULL && musicName != NULL && driver->audioInited == true) {
		Driver_stopMusic();
		DATA *data = Array_getFromKey(driver->ressources, musicName);
		Mix_Music *music = NULL;
		if (data != NULL && data->type == MUSIC_TYPE) {
			music = (Mix_Music  *) data->buffer;
		} else {
			music = Driver_loadMusic(musicName);
		}

		if (music != NULL) {
			driver->currentMusic = music;
			driver->loopMusic = loops;
			driver->pauseMusic = false;
			Mix_PlayMusic(music, loops);
		}
	}
}

void Driver_stopMusic()
{
    if (driver != NULL && driver->audioInited == true && driver->currentMusic != NULL) {
		driver->currentMusic = NULL;
		Mix_HaltMusic(); //Stop the music
	}
}

void Driver_pauseMusic()
{
    if (driver != NULL && driver->audioInited == true
		&& driver->currentMusic != NULL && driver->pauseMusic == false) {
		driver->pauseMusic = true;
		Mix_PauseMusic(); //Pause the music
	}
}

void Driver_resumeMusic()
{
    if (driver != NULL && driver->audioInited == true
		&& driver->currentMusic != NULL && driver->pauseMusic == true) {
		driver->pauseMusic = false;
		Mix_ResumeMusic(); //Resume the music
	}
}
