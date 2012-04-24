/* input.c
 * Keyboard and mouse input
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

#include "input.h"

/**
 * Checks for user input: esc and mouseclick exit the program, space
 * should induce anoter iteration
 */
int checkInput() {
	static SDL_Event event;

	while (SDL_WaitEvent(&event)) {
		/* closed window */
		if (event.type == SDL_QUIT) {
 			return INP_QUIT;
		}

		/* mouse click */
		if (event.type == SDL_MOUSEBUTTONDOWN) {
			return INP_QUIT;
		}

		/* key press */
		if (event.type == SDL_KEYDOWN) {
			switch(event.key.keysym.sym) {
				case SDLK_SPACE:
					return INP_CONT;
					break;
				case SDLK_ESCAPE:
					return INP_QUIT;
				case SDLK_h:
					return INP_TOGGLEHELP;
				case SDLK_a:
					return INP_INCANGLE;
				case SDLK_s:
					return INP_DECANGLE;
				case SDLK_q:
					return INP_INCLINELEN;
				case SDLK_w:
					return INP_DECLINELEN;
				case SDLK_r:
					return INP_INCRANDANGLE;
				case SDLK_t:
					return INP_RESETRANDANGLE;
				case SDLK_UP:
					return INP_DECYOFFSET;
				case SDLK_DOWN:
					return INP_INCYOFFSET;
				case SDLK_LEFT:
					return INP_DECXOFFSET;
				case SDLK_RIGHT:
					return INP_INCXOFFSET;
				case SDLK_o:
					return INP_RESETOFFSET;
#ifdef VRML
				case SDLK_m:
					return INP_EXPORTVRML;
#endif
				default: break;
			}
		}
	}

	return INP_NULL;
}

