/* input.h
 * Include file for keyboard and mouse checker
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

#ifndef _INPUT_H_
#define _INPUT_H_

#include <stdbool.h>
#include "SDL.h"

enum inputResults {INP_NULL = 0, INP_QUIT, INP_CONT, INP_TOGGLEHELP,
	INP_INCLINELEN, INP_DECLINELEN, INP_INCANGLE, INP_DECANGLE,
	INP_INCRANDANGLE, INP_RESETRANDANGLE, INP_INCXOFFSET, INP_DECXOFFSET,
	INP_INCYOFFSET, INP_DECYOFFSET, INP_RESETOFFSET, INP_EXPORTVRML};

int checkInput();

#endif
