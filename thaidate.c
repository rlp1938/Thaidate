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
	int oporder;	// 0 = default, dd mmm yyyy else yyyy mmm dd
} progdata;

#include "str.h"
#include "files.h"
#include "gopt.h"

static progdata
*validate_input(options_t *opts, char **argv);
static char
*generateop(progdata *pd);
static char
*genthaidigits(int num);


int main(int argc, char **argv)
{
	options_t opts = process_options(argc, argv);
	progdata *pd = validate_input(&opts, argv);
	char *opstr = generateop(pd);
	fprintf(stdout, "%s\n", opstr);
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
	leap = (leap && !(y % 100 == 0));
	leap = (leap || (y % 400 == 0));
	// validate day in month.
	int dim[13] = {-1, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	if (leap) dim[2] = 29;
	if (!(d > 0 && d <= dim[m])) {
		fprintf(stderr, "Invalid days: %d for month: %d\n", d, m);
		exit(EXIT_FAILURE);
	}
	progdata *res = xmalloc(sizeof(progdata));
	res->dayin = d;
	res->monin = m;
	res->yerin = y;
	res->yerout = y + 543;
	res->wantreal = opts->o_r;
	res->oporder = opts->o_Y;	// 1 is yyyy mmm dd, 0 dd mmm yyyy
	return res;
} // validate_input()

static char
*generateop(progdata *pd)
{/* build the output string */
	char *monamesth[] = {"none", "มกราคม", "กุมภาพันธ์", "มีนาคม", "เมษายน",
						"พฤษภาคม", "มิถุนายน", "กรกฎาคม", "สิงหาคม",
						"กันยายน", "ตุลาคม", "พฤศจิกายน", "ธันวาคม"};
	// has max length 27 (november). 3 chars per visible char.
	char thmoname[32];
	strcpy(thmoname, monamesth[pd->monin]);
	char daysth[8];		// 2 days * 3 + '\0' is 7 needed.
	char yearth[16];	// 4 digits * 3 + '\0' is 13 needed.
	if (pd->wantreal) {	// buffers have to be bigger
		char *cp = genthaidigits(pd->dayin);
		strcpy(daysth, cp);
		cp = genthaidigits(pd->yerout);
		strcpy(yearth, cp);
	} else {
		sprintf(daysth, "%d", pd->dayin);
		sprintf(yearth, "%d", pd->yerout);
	}
	static char outbuf[PATH_MAX];
	// outut order ymd or dmy
	if (pd->oporder) {	// ymd
		sprintf(outbuf, "%s %s %s", yearth, thmoname, daysth);
	} else {	// dmy
		sprintf(outbuf, "%s %s %s", daysth, thmoname, yearth);
	}
	return outbuf;
}

char
*genthaidigits(int num)
{/* given an abitrary integer, return a Thai digit string */
	char *digitnamesth[] = {"๐", "๑", "๒", "๓", "๔", "๕", "๖", "๗",
						   /*0    1    2    3    4    5    6    7*/
							"๘", "๙"};
						   /*8    9 */
	static char buffer[NAME_MAX];
	buffer[0] = 0;
	sprintf(buffer, "%d", num);
	int l = strlen(buffer);
	// sanity check
	if (3 * l + 1 > NAME_MAX) {
		fprintf(stderr, "Your number %d, is too large.\n", num);
		exit(EXIT_FAILURE);
	}
	int i;
	int exp = 1;
	for (i = 0; i < l; i++) {
		exp *= 10;
	}
	exp /= 10;	// a digit too long
	int divby = exp;
	buffer[0] = 0;
	for (i = 0; i < l; i++) {
		int dd = num / divby;
		strcat(buffer, digitnamesth[dd]);
		num %= divby;
		divby /= 10;
	}
	return buffer;
}
