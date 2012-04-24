/* renderlsystem.c
 * L-System renderer and VRML export
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
#include <unistd.h>
#include "renderlsystem.h"
#include "tools.h"
#include "simplegfx.h"
#include "lsystem.h"

#ifdef VRML
/**
 * Print out the header for the VRML export
 */
static void printVRMLHeader() {
	int i;
	static char *header[] = {
		"#VRML V2.0 utf8",
		"",
		"PROTO Plant [",
		"	exposedField SFNode plantmat Material {",
		"		diffuseColor 0 0.5 0",
		"	}",
		"] {",
		"	Transform {",
		"		children [",
		"			DEF branch Transform {",
		"				children Shape {",
		"					geometry Box {",
		"						size 2 20 2",
		"					}",
		"					appearance Appearance {",
		"						material IS plantmat",
		"					}",
		"				}",
		"			}",
		"			DEF TS1 TimeSensor {",
		"				enabled TRUE",
		"				loop TRUE",
		"				cycleInterval 1.35",
		"			}",
		"			DEF OI1 OrientationInterpolator {",
		"				key [ 0, 0.5, 1.0 ]",
		"				keyValue [",
		"					0 1 0 -0.5",
		"					0 1 0 -1.0",
		"					0 1 0 -0.6",
		"				]",
		"			}"};
	for (i = 0; i < 32; i++) {
		printf("%s\n", header[i]);
	}
}
#endif

/**
 * Renders a L-System, starting at offsetX, offsetY.
 * The L-System may contain the following characters:
 *  F - go one step to the current direction and draw line
 *  + - turn left
 *  - - turn right
 *  [ - push the current position and angle to the stack
 *  ] - pop the top stack element
 */
#define rnd ((double)randi(0, (int)(randomize * 100)/100.)/50.)
#ifdef VRML
void renderLSystem(char *lsys, int len, double angle, double randomize,
		int offsetX, int offsetY, bool exportVRML) {
#else
void renderLSystem(char *lsys, int len, double angle, double randomize,
		int offsetX, int offsetY) {
#endif
	Uint32 green = sgCreateColor(0, 255, 0);
	Uint32 white = sgCreateColor(255, 255, 255);
	char *output;
	int i;
	int j = 0;
#ifdef VRML
	int openTransforms = 0;
	int oldpath;
	double rotation = 1.;
	State state = {0.0, 0.0, -M_PI/2, 0, 0};
#else
	State state = {0.0, 0.0, -M_PI/2};
#endif
	int maxStackSize = 0;
	double ang = deg2rad(angle);
	State *tmp;

	List list = cons(stateToObject(state), NULL);
#ifdef VRML
	if (exportVRML) {
		printVRMLHeader();
	}
#endif

	output = newn(char, 70);
	tmp = new(State);

	for (i = 0; i < strlen(lsys); i++) {
		switch (lsys[i]) {
			case 'F': /* go one step and draw */
				tmp->x = state.x;
				tmp->y = state.y;
				tmp->ang = state.ang;
#ifdef VRML
				tmp->rotation = state.rotation;
#endif
				state.x += cos(state.ang) * len;
				state.y += sin(state.ang) * len;
				sgLine((int)tmp->x + offsetX, (int)tmp->y + offsetY,
					(int)state.x + offsetX, (int)state.y + offsetY, white);
#ifdef VRML
				if (exportVRML) {
					rotation = state.rotation * ang;
					printf(
						"	Transform {\n"
						"		translation 0 18 0\n"
						"		rotation 0 0 1 %f\n"
						"		center 0 -9 0\n"
						"		children [\n"
						"			USE branch\n", rotation);
					openTransforms++;
					state.path++;
					state.rotation = 0;
				}
#endif
				break;
			case '+': /* turn left */
				state.ang -= ang + rnd;
				state.ang = normalizeAngle(state.ang);
#ifdef VRML
				state.rotation = 1;
#endif
				break;
			case '-': /* turn right */
				state.ang += ang + rnd;
				state.ang = normalizeAngle(state.ang);
#ifdef VRML
				state.rotation = -1;
#endif
				break;
			case '[': /* push state */
				pushBack(list, stateToObject(state));
				j = length(list);
				if (j > maxStackSize) {
					maxStackSize = j;
				}
				break;
			case ']': /* pop state */
#ifdef VRML
				if (exportVRML) {
					oldpath = state.path;
				}
#endif
				state = *(State*)popBack(list);
#ifdef VRML
				if (exportVRML) {
					for (j = oldpath - state.path; j > 0
							&& openTransforms > 0; j--) {
						openTransforms--;
						printf("]}\n");
					}
				}
#endif
				break;
			default:
#ifdef DEBUG
				fprintf(stderr, "Error: Illegal character in "
					"L-System: %c (%d)\n", lsys[i], (int)lsys[i]);
#endif
				break;
		}
	}

#ifdef VRML
	if (exportVRML) {
		for (i = openTransforms + 1; i > 0; i--) {
			printf("]} ");
		}
		printf("	ROUTE TS1.fraction_changed TO OI1.set_fraction\n");
		printf("	ROUTE OI1.value_changed TO branch.rotation\n");
		printf("}\n");
		fflush(stdout);
	}
#endif

	snprintf(output, 70, "Stack max: %d  Randomization: %1.2f  "
		"Line length: %d  Angle: %3.2f", maxStackSize, randomize, len, angle);
	sgDrawText(output, 10, 580, green);
	free(output);
}


