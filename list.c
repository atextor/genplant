/* list.c
 * Linked list that actually acts as a stack
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

#include "list.h"
#include "tools.h"
#include <stdlib.h>
#include <stdio.h>

/** 
 * Sample function to convert a double into an Object as the linked
 * list only holds Objects
 */
Object newDouble(double d) {
	double *result = new(double);
	*result = d;
	return (Object)result;
}

/**
 * Sample function to convert an int into an Object  as the linked
 * list only holds Objects
 */
Object newInt(int i) {
	int *result = new(int);
	*result = i;
	return (Object)result;
}

/**
 * Creates a new one-element list
 */
List newList() {
	List list = new(struct Liste);
	return list;
}

/**
 * Creates a new one-element list with a NULL-link
 */
List nil() {
	List result = newList();
	result->tail = NULL;
	return result;
}

/**
 * Constructor for list (can be used basically
 * as a "push_front"
 */
List cons(Object x, List xs) {
	List result = newList();
	result->head = x;
	result->tail = xs;
	return result;
}

/**
 * Checks if the list element is empty
 */
bool isEmpty(List xs) {
	return (xs->tail == NULL);
}

/**
 * Remove a list and all following elements
 */
void delete(List xs) {
	if (!isEmpty(xs)) {
		delete(xs->tail);
	}
	free(xs->head);
	free(xs);
}

/**
 * Append a new element to the list
 */
void pushBack(List xs, Object x) {
	if (isEmpty(xs)) {
		xs->tail = cons(x, NULL);
		return;
	} else {
		pushBack(xs->tail, x);
	}
}

/**
 * Remove the last element from the list and return it
 */
Object popBack(List xs) {
	if (isEmpty(xs)) {
		return NULL;
	}
	if (isEmpty(xs->tail)) {
		Object x = xs->tail->head;
		delete(xs->tail);
		xs->tail = NULL;
		return x;
	} else {
		return popBack(xs->tail);
	}
}

/**
 * Returns the lenght of the list
 */
int length(List xs) {
	if (isEmpty(xs)) {
		return 1;
	}
	return length(xs->tail) + 1;
}

