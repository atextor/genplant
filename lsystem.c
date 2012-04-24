/* lsystem.c
 * Iterate a given L-System
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lsystem.h"
#include "list.h"
#include "tools.h"

/**
 * For debugging purposes: output the whole list
 */
void printList(List xs) {
	if (xs == NULL) {
		return;
	}
	fprintf(stderr, "{ %f %f %f }\n", ((State*)xs->head)->x,
		((State*)xs->head)->y, ((State*)xs->head)->ang);
	printList(xs->tail);
}

/**
 * Generate a new state object
 */
Object newState(double x, double y, double ang) {
	State *result = new(State);
	result->x = x;
	result->y = y;
	result->ang = ang;
	return (Object)result;
}

/**
 * Convert an exisiting state to an object
 */
Object stateToObject(State n) {
	State *result = new(State);
	*result = n;
	return (Object)result;
}

/**
 * Iterate a L-System and return the result
 * In "F->F+F", lsys would be "F" and fTo would be "F+F".
 */
char *lSystem(char *lsys, char *fTo) {
	int numF = 0;
	int i;
	int j;
	int newlen;
	char *result;

	/* 1. check number of Fs in fTo */
	for (i = 0; i < strlen(lsys); i++) {
		if (lsys[i] == 'F') {
			numF++;
		}
	}
	if (numF == 0) {
		return lsys;
	}

	/* 2. allocate memory for result string */
	newlen = (numF * strlen(fTo)) + (strlen(lsys) - numF);
	result = (char*)calloc(1, (newlen + 1) * sizeof(char));

	/* 3. generate result string */
	for (i = 0, j = 0; lsys[i] != 0; i++) {
		if (lsys[i] != 'F') {
			result[j] = lsys[i];
		} else {
			strncpy(result + j, fTo, strlen(fTo));
			j = j + strlen(fTo) - 1;
		}
		j++;
	}
	
	return result;
}

/**
 * Iterate a L-System n times and return the result
 * In "F->F+F", start would be "F" and fTo would be "F+F".
 */
char *lSysIterate(char *start, char *fTo, int iterations) {
	int i;
	char *it = start;
	char *old;
	for (i = 0; i < iterations; i++) {
		old = it;
		it = lSystem(it, fTo);
		if (old != start) {
			free(old);
		}
	}
	return it;
}



