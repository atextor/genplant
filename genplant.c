/* genplant.c
 * Main module: Read argv, enter main loop etc.
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
#include <math.h>
#include <time.h>
#include "list.h"
#include "lsystem.h"
#include "renderlsystem.h"
#include "simplegfx.h"
#include "input.h"
#include "tools.h"

#define TITLE "genplant v0.3"

/**
 * Prints the command line options to stdout
 */
void outputHelp() {
	printf("%s - Andreas Textor\n", TITLE);
	printf(
		"Usage: genplant [-h|--help][-f][-s L-System][-t][-a angle]\n"
		"       [-l length][-x factor]\n\n"
		"Options:\n"
		"-h  --help  - Show this help screen\n"
		"-f          - Enable fullscreen\n"
		"-t          - Print the state of the L-System (default: off)\n"
		"-a <angle>  - Set angle in degrees (default: 20)\n"
		"-l <length> - Initial line length in pixels (default: 40)\n"
		"-x <factor> - Contraction-factor, e.g. -x 7 will multiply the "
		"line-length\n              with 0.7 at each iteration "
		"(default: 7)\n");
	printf(
		"-s L-System - set L-System\n"
		"-ax axiom   - set axiom (default: F)\n\n"
		"Sample:       genplant -s \"F[+F]F[-F][F]\"\n"
		"              genplant -s \"F[+F]F[-F]F\"\n"
		"              genplant -s \"FF-[-F+F+F]+[+F-F-F]\"\n"
		"\n");
}

/**
 * Displays instructions for interactive commandos on the screen
 */
void drawHelp(bool complete) {
	Uint32 white = sgCreateColor(255, 255, 255);
	if (complete) {
		sgDrawText(
			"[H] - Hide Help\n"
			"[Q] - Increase line length\n"
			"[W] - Decrease line length\n"
			"[A] - Increase angle\n"
			"[S] - Decrease angle\n"
			"[R] - Increase angle randomization\n"
			"[T] - Reset angle randomization\n"
			"[O] - Reset movement\n"
#ifdef VRML
			"[M] - Export VRML\n"
#endif
			"[Arrow Keys] - Move around\n"
			"[SPACE] - Next iteration", 10, 20, white);
	} else {
		sgDrawText("[H] - Display Help", 10, 20, white);
	}
}

/**
 * Main function: checks commandline, initializes graphics output
 * and goes into main loop
 */
int main(int argc, char *argv[]) {
	bool drawText = false;
	bool running = true;
	bool fullscreen = false;
	bool help = true;
#ifdef VRML
	bool exportVRML = false;
#endif
	Uint32 color;
	char *lpreset = "F[+F]F[-F][F]";
	char *fto = lpreset;
	char *lsys = NULL;
	char *output;
	int input;
	int i;
	int iteration = 1;
	int factor = 7;
	int offsetX = XRES / 2;
	int offsetY = YRES;
	double linelength = 40.;
	double angle = 20.;
	double randomizeAngle = 0.;

	/* check commandline */
	for (i = 0; i < argc; i++) {
		if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
			outputHelp();
			return 0;
		}
		if (strcmp(argv[i], "-s") == 0) {
			if(argc-1 > i) {
				fto = newn(char, strlen(argv[i+1]));
				strncpy(fto, argv[i+1], strlen(argv[i+1]));
			}
		}
		if (strcmp(argv[i], "-ax") == 0) {
			if(argc-1 > i) {
				lsys = newn(char, strlen(argv[i+1]));
				strncpy(lsys, argv[i+1], strlen(argv[i+1]));
			}
		}
		if (strcmp(argv[i], "-x") == 0) {
			if(argc-1 > i) {
				factor = atoi(argv[i+1]);
			}
		}
		if (strcmp(argv[i], "-l") == 0) {
			if(argc-1 > i) {
				linelength = (double)atoi(argv[i+1]);
			}
		}
		if (strcmp(argv[i], "-a") == 0) {
			if(argc-1 > i) {
				angle = (double)atoi(argv[i+1]);
			}
		}
		if (strcmp(argv[i], "-t") == 0) {
			drawText = true;
		}
		if (strcmp(argv[i], "-f") == 0) {
			fullscreen = true;
		}
	}
	
	/* create screen */
	if (sgInit(fullscreen, TITLE) != 0) {
		exit(EXIT_FAILURE);
	}
	color = sgCreateColor(255, 255, 255);

	/* create L-system */
	if (lsys == NULL) {
		lsys = newn(char, 2);
		lsys = "F";
	}

	/* allocate memory for output string */
	output = newn(char, 15);

	srand((unsigned)time(NULL));

	/* display L-system */
	while (running) {
		sgClearScreen();
		snprintf(output, 15, "Iteration: %d", iteration);
		sgDrawText(output, 670, 580, color);
#ifdef VRML
		renderLSystem(lsys, (int)linelength, angle, randomizeAngle,
			offsetX, offsetY, exportVRML);
		exportVRML = false;
#else
		renderLSystem(lsys, (int)linelength, angle, randomizeAngle,
			offsetX, offsetY);
#endif
		if (drawText) {
			sgDrawText(lsys, 10, 10, color);
		}
		drawHelp(help);
		sgUpdateScreen();
		do {
			input = INP_NULL;
			input = checkInput();
			switch (input) {
				case INP_QUIT:
					running = false;
					break;
				case INP_CONT:
					iteration++;
					lsys = lSystem(lsys, fto);
					linelength *= ((double)factor/10.0);
					break;
				case INP_TOGGLEHELP:
					help = !help;
					break;
				case INP_INCANGLE:
					angle += 5.;
					break;
				case INP_DECANGLE:
					angle -= 5.;
					break;
				case INP_INCLINELEN:
					linelength += 3;
					break;
				case INP_DECLINELEN:
					linelength -= 3;
					break;
				case INP_INCRANDANGLE:
					randomizeAngle += 1.;
					break;
				case INP_RESETRANDANGLE:
					randomizeAngle = 0.;
					break;
				case INP_DECYOFFSET:
					offsetY -= (int)linelength;
					break;
				case INP_INCYOFFSET:
					offsetY += (int)linelength;
					break;
				case INP_DECXOFFSET:
					offsetX -= (int)linelength;
					break;
				case INP_INCXOFFSET:
					offsetX += (int)linelength;
					break;
				case INP_RESETOFFSET:
					offsetX = XRES / 2;
					offsetY = YRES;
					break;
#ifdef VRML
				case INP_EXPORTVRML:
					exportVRML = true;
					break;
#endif
				default:
					break;
			}
		} while (input == INP_NULL);
	}
	SDL_Quit();

	return 0;
}

