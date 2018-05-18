/*    thaidate.c
 *
 * Copyright 2017 Robert L (Bob) Parker rlp1938@gmail.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdarg.h>
#include <getopt.h>
#include <ctype.h>
#include <limits.h>
#include <linux/limits.h>
#include <libgen.h>
#include <errno.h>
// typdefs/structs here.
typedef struct progdata {
	int dayin;
	int monin;
	int yerin;
	int yerout;
	int wantreal;
} progdata;

#include "str.h"
#include "files.h"
#include "gopt.h"

static progdata
*validate_input(options_t *opts, char **argv);


int main(int argc, char **argv)
{
	options_t opts = process_options(argc, argv);
	progdata *pd = validate_input(&opts, argv);

	free(pd);	//nothing more to do, no pointers inside the struct.
	return 0;
}//main()

progdata
*validate_input(options_t *opts, char ** argv)
{/* All being well return valid progdata* */
	// First up make sure all args are there.
	if (!argv[optind] || !argv[optind+1] || !argv[optind+2]) {
		fputs("Must have day month and year as separate fields.\n",
		stderr);
		exit(EXIT_FAILURE);
	}
	int one, two, three, y, m, d;
	one = strtol(argv[optind], NULL, 10);
	optind++;
	two = strtol(argv[optind], NULL, 10);
	optind++;
	three = strtol(argv[optind], NULL, 10);
	if (opts->o_y) {	// ISO input
		y = one;
		m = two;
		d = three;
	} else if (opts->o_d) {	// dmy input;
		d = one;
		m = two;
		y = three;
	} else {	// mdy input
		m = one;
		d = two;
		y = three;
	}
	// test month first.
	if (m < 1 || m > 12) {
		fprintf(stderr, "Invalid month input: %d\n", m);
		exit(EXIT_FAILURE);
	}
	// validate year.
	int leap = (y % 4 == 0);
	printf("%d\n", leap);

	progdata *res = xmalloc(sizeof(progdata));
	return res;
} // validate_input()
