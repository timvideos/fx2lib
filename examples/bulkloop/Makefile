FX2LIBDIR=../../
BASENAME = bulkloop
SOURCES=bulkloop.c dscr_test.c dscr_str.c
A51_SOURCES=dscr.a51
PID=0x1004

SDCCFLAGS=--std-c99

include $(FX2LIBDIR)lib/fx2.mk

dscr_str.c: dscr_str.in string_table.py
	cat dscr_str.in | python string_table.py > dscr_str.c

test: test.cpp
	g++ -o test test.cpp -lusb-1.0

