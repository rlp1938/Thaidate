# thaidate

## NAME
**thaidate** - accepts 3 numbers, year, month and day, and outputs
a date string in Thai language with the month using the Thai name.


## SYNOPSIS

**thaidate** [option] **y m d**.

## DESCRIPTION

**thaidate** accepts as input, 3 numbers which by default must be in
order of 4 digit year, month number [1..12], and day [1..*maxday*],
where *maxday* is calculated by month and year. You may alter the
default order of input fields using options.

By default the **output** order is **d m y** and also by default Latin
digits are used for the numbers, **d** and **y**.

The Thai calendar year is 543 years ahead of the normally accepted
Western calendar. This increment is added to the output year.

## OPTIONS

+ **-h**, **--help**
print help information and exit.

:  **-m**, **--mdy**
order the input date fields as month, day and year.

:  **-d**, **--dmy**
order the input date fields as day, month and year.

:  **-y**, **--ymd**
order the input date fields as year, month and day. This is actually the
default.

:  **-Y**, **--YMD**
orders the **output** field as year, month, day. This the officially
recognised Thai date format. But in spite of this, the //default//
output ordering is more usual.

:  **-D**, **--DMY**
orders the **output** field as day, month, year. This the most common
Thai date format, and it is the default.

:  **-r**, **--real-thai**
Latin digits are used very commonly in Thailand in many situations,
no doubt thanks to the use the ubiquitous electronic calculator. So,
by default the output of this program uses Latin digits for the day and
year fields. Choosing this option will output Thai digits for numbers
instead.

Example: **thaidate** --real-thai 2018 1 31

**๓๑ มกราคม ๒๕๖๑**


= BUGS/FEATURES =
Though within Thai writing spaces are not as commonly used as in
English, the writing is by no means space free. Because I //seem// to
recall having seen spaces in Thai dates, I have chosen to follow my
belief. I shall certainly change this on viewing evidence to the
contrary.

= AUTHOR =
Robert L Parker Bob.Parker@programmer.net

= SEE ALSO =
**date** (1)

