/* tools.c
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

#include <math.h>
#include "tools.h"

/**
 * Make sure 0 < angle < 2*PI
 */
double normalizeAngle(double ang) {
	double result = ang;

	if (ang > 2*M_PI) {
		ang = (ang - 2*M_PI);
	} else if (ang < 0.0) {
		ang = (2*M_PI - ang);
	}

	return result;
}

/**
 * Convert degrees to radians
 */
double deg2rad(double ang) {
	return M_PI * ang / 180.0;
}

/**
 * Converts a hexadecimal number to decimal
 */
int hex2dec(char *a) {
	int result = 0;
	int i;
	for(i = 0; (a[i]!='\0') && (a[i]!='\n'); i++)
		if(a[i] >= 'A' && a[i] <= 'F')
			result = (16*result)+(a[i]-'A'+10);
		else
			result = (16*result)+(a[i]-'0');
	return result;
}

/**
 * Get the n-th bit of a number
 */
unsigned int getbit(unsigned int x, int i) {
	return (x>>i) & 1;
}

/**
 * Swap two variables.
 */
void swap(int *a, int *b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}

/**
 * Returns an integer pseudo random number between a end e
 */
int randi(int a, int e) {
	double r = e - a + 1;
	return a + (int)(r * rand()/(RAND_MAX+1.0));
}

/**
 * Returns a double pseudo random number between 0 and 1
 */
double randd() {
	return ((double)randi(1, 1000)) / 1000;
}


