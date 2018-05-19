/*      gopt.c
 *
 *  Copyright 2017 Robert L (Bob) Parker rlp1938@gmail.com
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA 02110-1301, USA.
*/

#include "str.h"
#include "files.h"
#include "gopt.h"


options_t process_options(int argc, char **argv)
{
	optstring = ":hmdyYDr";

	/* declare and set defaults for local variables. */

	/* set up defaults for opt vars. */
	options_t opts = {0};	// assumes defaults all 0/NULL
	opts.o_y = 1;	// ymd is default input format.
	opts.o_D = 1;	// DMY is default output format.

	int c;

	while(1) {
		int this_option_optind = optind ? optind : 1;
		int option_index = 0;
		static struct option long_options[] = {
		{"help",		0,	0,	'h'},
		{"mdy",			0,	0,	'm'},
		{"dmy",			0,	0,	'd'},
		{"ymd",			0,	0,	'y'},
		{"YMD",			0,	0,	'Y'},
		{"DMY",			0,	0,	'D'},
		{"real-thai",	0,	0,	'r'},
		{0,	0,	0,	0 }
		};

		c = getopt_long(argc, argv, optstring,
                        long_options, &option_index);

		if (c == -1)
			break;

		switch (c) {
		case 0:
			switch (option_index) {
			} // switch()
		break;
		case 'h':
		dohelp(0);
		break;
		/* input options */
		case 'm':	// stupid USA format input.
			opts.o_m = 1;
			opts.o_y = 0;
			opts.o_d = 0;
			break;
		case 'd':	// English format input.
			opts.o_d = 1;
			opts.o_m = 0;
			opts.o_y = 0;
			break;
		case 'y':	// ISO format input.
			opts.o_y = 1;
			opts.o_m = 0;
			opts.o_d = 0;
			break;
		/* output options */
		case 'Y':	// to be ordered ymd
			opts.o_Y = 1;
			opts.o_D = 0;
			break;
		case 'D':	// to be ordered dmy
			opts.o_D = 1;
			opts.o_Y = 0;
			break;
		case 'r':	// real Thai numbers, not ASCII digits.
			opts.o_r = 1;
			break;
		case ':':
			fprintf(stderr, "Option %s requires an argument\n",
					argv[this_option_optind]);
			dohelp(1);
		break;
		case '?':
			fprintf(stderr, "Unknown option: %s\n",
					 argv[this_option_optind]);
			dohelp(1);
		break;
		} // switch()
	} // while()
	return opts;
} // process_options()

void dohelp(int forced)
{/* invoke manpage instead of trying to keep 2 sets of text synced. */
	char command[NAME_MAX];
	// so where is the manpage?
	if (exists_file("./thaidate.1")) {	// development time.
		sprintf(command, "man ./thaidate.1");
	} else {
		sprintf(command, "man thaidate.1");
	}
	xsystem(command, 1);
	exit(forced);
} // dohelp()
