/* list.h
 * Data structure and prototypes for list/stack implementation
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

#ifndef _LIST_H
#define _LIST_H
#include <stdbool.h>

typedef void* Object;
struct Liste {
	Object head;
	struct Liste* tail;
};

typedef struct Liste* List;

Object popBack(List xs);
void pushBack(List xs, Object x);
Object newDouble(double d);
Object newInt(int i);
List nil();
List cons(Object x, List xs);
bool isEmpty(List xs);
void delete(List xs);
int length(List xs);

#endif

