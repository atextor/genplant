/* simplegfx.c
 * Simple graphics library, a wrapper for SDL with some simplifications
 * and functions SDL is lacking like drawing a line and a text routine
 * (using a home-brewn UTF-8 decoder and the font found at
 * http://czyborra.com/unifont/HEADER.html).
 *
 * genplant - L-System plant renderer
 * by Andreas Textor <textor.andreas@googlemail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include "simplegfx.h"
#include "unifont.h"

/** The screen surface */
static SDL_Surface *screen;
/** The surface that acts as a color source */
static SDL_Surface *colorSource;

/**
 * Create a new window and screen
 */
int sgInit(bool fullscreen, char *title) {
	Uint32 videoflags;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
		return 1;
	}

	videoflags = SDL_SWSURFACE;
	if (fullscreen)
		videoflags |= SDL_FULLSCREEN;

	screen = SDL_SetVideoMode(XRES, YRES, 0, videoflags);
	if (screen == NULL) {
		fprintf(stderr, "Couldn't set video mode: %s\n",
			SDL_GetError());
		return 1;
	}
	SDL_WM_SetCaption(title, NULL);

	colorSource = sgCreateSurface(1, 1);
	return 0;
}

/**
 * Creates a new surface with given dimensions
 */
SDL_Surface *sgCreateSurface(int width, int height) {
	SDL_Surface *surface;
	Uint32 rmask;
	Uint32 gmask;
	Uint32 bmask;
	Uint32 amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif

	surface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32,
		rmask, gmask, bmask, amask);

	if(surface == NULL) {
		fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
		exit(1);
	}
	
	return surface;
}

/**
 * Create a new Uint32 color for SDL using RGB components
 */
Uint32 sgCreateColor(Uint8 r, Uint8 g, Uint8 b) {
	Uint32 result;
	if (colorSource != NULL) {
		result = SDL_MapRGB(colorSource->format, r, g, b);
	} else {
		result = 0;
	}

	return result;
}

/**
 * Put a pixel onto a surface
 */
void sgPutPixel(SDL_Surface *surface, int x, int y, Uint32 color) {
	int bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to set */
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch(bpp) {
		case 1:
			*p = color;
			break;
		case 2:
			*(Uint16 *)p = color;
			break;
		case 3:
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
				p[0] = (color >> 16) & 0xff;
				p[1] = (color >> 8) & 0xff;
				p[2] = color & 0xff;
#else
				p[0] = color & 0xff;
				p[1] = (color >> 8) & 0xff;
				p[2] = (color >> 16) & 0xff;
#endif
			break;
		case 4:
			*(Uint32 *)p = color;
			break;
	}
}

/**
 * Draw a single character onto a surface using the unifont.
 * The character pointer should be a pointer to the unifont character array.
 * To actually draw a text use the sgDrawUTF8Text function below.
 */
static void sgDrawCharacter(SDL_Surface *surface, int x, int y, Uint32 color,
	char *ch) {

	char line[5];
	int i, j, l;
	int len = strlen(ch);

	if(len != 32 && len != 64) {
		fprintf(stderr, "Error: drawcharacter: Invalid character length\n");
		return;
	}

	/* Warning: ugly code ahead. */
	for(i = 0; i < len; i += (len/16)) {
		sprintf(line, "%c%c%c%c", ch[i], ch[i+1],
			(len==64?ch[i+2]:'\0'), (len==64?ch[i+3]:'\0'));
		l = hex2dec(line);
		for(j = (len/4)-1; j >= 0; j--)
			if(getbit(l, j))
				sgPutPixel(surface, x+((len/4)-j), y+(i/(len/16)), color);
	}
}

/**
 * Draw a complete UTF-8 string onto a given surface. This also works
 * with static strings (strings that are contained in the sourcecode) as
 * long as the corresponding source file is UTF-8 encoded.
 * Example: sgDrawUTF8Text(surface, 0, 0, color, "░░▒▒▓▓██");
 */
static void sgDrawUTF8Text(SDL_Surface *surface, int x, int y,
	Uint32 color, char *ch) {

	int chlen = strlen(ch);
	int i = 0, k;
	int c;					/* unicode codepoint */
	int followbytes = 0;	/* number of following bytes */
	int drawx = x;
	int line = 0;
	unsigned char byte;
	unsigned char chati;

	/* UTF-8-decoder */
	while(i < chlen) {
		c = 0;
		chati = (unsigned char)ch[i];
		/* byte is 0xxxxxxx - no following bytes */
		if(chati>>7 == 0)
			c = chati;
		else {
			/* check for overlong UTF-8 sequences */
			if((chati>>1 == 0x60) ||
				((chati>>2 == 0xE0) && ((i+1 < chlen) &&
					(unsigned char)ch[i+1]>>5 == 0x04)) ||
				((chati>>3 == 0xF0) && ((i+1 < chlen) &&
					(unsigned char)ch[i+1]>>4 == 0x80)) ||
				((chati>>4 == 0xF8) && ((i+1 < chlen) &&
					(unsigned char)ch[i+1]>>3 == 0x10)) ||
				((chati>>5 == 0xFC) && ((i+1 < chlen) &&
					(unsigned char)ch[i+1]>>2 == 0x20))) {
				fprintf(stderr, "Error: Text is no valid UTF-8 "
					"(overlong UTF-8 sequence at byte 0x%02X)\n", i);
				c = 0x003F;
				i++;
				continue;
			}

			/* get number of following bytes */
			if((chati>>1) == 126) { followbytes = 5; c = (chati & 0x01); }
			else if((chati>>2) == 62) { followbytes = 4; c = (chati & 0x03); }
			else if((chati>>3) == 30) { followbytes = 3; c = (chati & 0x07); }
			else if((chati>>4) == 14) { followbytes = 2; c = (chati & 0x0F); }
			else if((chati>>5) == 6) { followbytes = 1; c = (chati & 0x1F); }
			else if((chati>>6) == 2) {
				followbytes = 0;
				fprintf(stderr, "Error: Text is no valid UTF-8 (lone following"
					" byte at 0x%02X)\n", i);
			}

			/* process following bytes */
			for(k = 1; k <= followbytes; k++) {
				if(k > strlen(ch)) {
					fprintf(stderr, "Error: Text is no valid UTF-8 (EOL "
						"reached, follow byte expected)\n");
				} else {
					byte = (unsigned char)ch[k+i];
					/* byte is really a following byte? */
					if((byte>>6) == 2) {
						c = c << 6;
						c += (byte & 0x3F);
					} else {
						c = 0x003F;
						fprintf(stderr, "Error: Text is no valid UTF-8 "
							"(byte 0x%02X is no valid following byte)", byte);
					}
				}
			}
			i += followbytes;
		}
		/* respect newline */
		if (c == 10) {
			line++;
			drawx = x;
		} else {
			sgDrawCharacter(surface, drawx, y + (line * 15), color, unifont[c]);
			drawx += strlen(unifont[c]) / 4;
		}
		i++;
	}
} 

/**
 * Function to draw text to the screen.
 * See descriptions of functions sgDrawCharacter and sgDrawUTF8Text above.
 */
void sgDrawText(char *text, int x, int y, Uint32 color) {
	sgDrawUTF8Text(screen, x, y, color, text);
}

/**
 * Draw an image to the screen. To load an image, use something like:
 * SDL_Surface *bg = SDL_LoadBMP("picture.bmp");
 * (For other formats than BMP see the SDL_image library)
 */
void sgDrawImage(SDL_Surface *img, int x, int y) {
	SDL_Rect dest;

	dest.x = x;
	dest.y = y;
	dest.w = img->w;
	dest.h = img->h;
	SDL_BlitSurface(img, NULL, screen, &dest);
}

/**
 * Clears the screen for correct repainting
 */
void sgClearScreen() {
	SDL_FillRect(screen, 0, 0);
}

/**
 * Updates the screen
 */
void sgUpdateScreen() {
	SDL_UpdateRect(screen, 0, 0, 0, 0);
}

/**
 * Draws a line using Bresenham's line algorithm
 */
#define in(x,b1,b2) ((x>=b1) && (x<b2))
void sgLine(int x1, int y1, int x2, int y2, Uint32 color) {
	bool steep = (abs(y2 - y1) > abs(x2 - x1));
	int x, y, v, dx, dy, da, ystep;

	if(steep) {
		swap(&x1, &y1);
		swap(&x2, &y2);
	}
	if(x1 > x2) { 
		swap(&x1, &x2);
		swap(&y1, &y2);
	}

	dx = x2 - x1;
	dy = abs(y2 - y1);
	v = 0;
	da = dy;
	y = y1;
		
	ystep = (y1 < y2) ? 1 : -1;

	for(x = x1; x <= x2; x++) {
		if(steep) {
			if (in(y, 0, XRES-1) && in(x, 0, YRES-1))
				sgPutPixel(screen, y, x, color);
		} else {
			if (in(x, 0, XRES-1) && in(y, 0, YRES-1))
				sgPutPixel(screen, x, y, color);
		}

		v += da;
		if((2*v) >= dx) {
			y += ystep;
			v -= dx;
		}
	}
}

