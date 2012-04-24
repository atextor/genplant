/* tools.h
 * Miscellaneous functions
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

#ifndef _TOOLS_H_
#define _TOOLS_H_

#include <stdlib.h>
#include <stdio.h>

/**
 * Macros to allocate memory and auto-panic on failure.
 */
static void __attribute__((__unused__)) *_new_ptr = NULL;
/* new(): allocate memory and return as pointer */
#define new(x) (x *)(_new_ptr = malloc(sizeof(x))); \
	if (NULL==_new_ptr) { fprintf(stderr, "malloc() failed"); \
	exit(EXIT_FAILURE); }
/* newn(): allocate n units of memory and return as pointer */
#define newn(x,n) (x *)(_new_ptr = malloc(sizeof(x)*n)); \
	if (NULL==_new_ptr) { fprintf(stderr, "malloc() failed"); \
	exit(EXIT_FAILURE); }

double normalizeAngle(double ang);
double deg2rad(double ang);
int hex2dec(char *a);
unsigned int getbit(unsigned int x, int k);
void swap(int *a, int *b);
int randi(int a, int e);
double randd();

#endif

