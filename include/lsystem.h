/* lsystem.h
 * Include file for L-System iterator
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

#ifndef _LSYSTEM_H
#define _LSYSTEM_H
#include "list.h"

typedef struct {
	double x;
	double y;
	double ang;
#ifdef VRML
	int rotation;
	int path;
#endif
} State;

Object stateToObject(State n);
Object newState(double x, double y, double ang);
void printList(List xs);
char *lSystem(char *lsys, char *fTo);
char *lSysIterate(char *start, char *fTo, int iterations);

#endif

