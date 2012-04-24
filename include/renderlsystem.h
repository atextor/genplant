/* renderlsystem.h
 * Include file for L-System renderer
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

#ifndef _RENDERLSYSTEM_H_
#define _RENDERLSYSTEM_H_

#ifdef VRML
#include <stdbool.h>

void renderLSystem(char *lsys, int len, double angle, double randomize,
		int offsetX, int offsetY, bool exportVRML);
#else

void renderLSystem(char *lsys, int len, double angle, double randomize,
		int offsetX, int offsetY);
#endif

#endif

