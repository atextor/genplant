/* simplegfx.h
 * Simple graphics library, a wrapper for SDL with some simplifications
 * and functions SDL is lacking like drawing a point and a text routine
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

#ifndef _SIMPLEGFX_H_
#define _SIMPLEGFX_H_

#include <stdbool.h>
#include "tools.h"
#include "SDL.h"

/* To keep it simple, we use a static resolution */
#define XRES 800
#define YRES 600

int sgInit(bool fullscreen, char *title);
SDL_Surface *sgCreateSurface(int width, int height);
Uint32 sgCreateColor(Uint8 r, Uint8 g, Uint8 b);
void sgPutPixel(SDL_Surface *surface, int x, int y, Uint32 color);
void sgDrawImage(SDL_Surface *img, int x, int y);
void sgUpdateScreen();
void sgClearScreen();
void sgDrawText(char *text, int x, int y, Uint32 color);
void sgLine(int x1, int y1, int x2, int y2, Uint32 color);

#endif

