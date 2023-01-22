# In *my* code, I had the interpreter code added to the parser code.
# This additional code is protected by #ifdef INTERP / #endif clauses.
# This code is only "seen" by the compiler if you have #defined INTERP
# This #define would be annoying to change inside nuclei.c/.h, so instead it is
# set in the gcc/clang statement using -DINTERP
# In this way ./parse & ./interp can both be built from the same source file.

#By using the instruct "make parse", "make interp" and "make extension" to achieve these function.

CC      := clang
DEBUG   := -g3
OPTIM   := -O3
CFLAGS  := -Wall -Wextra -Wpedantic -Wfloat-equal -Wvla -std=c99 -Werror
RELEASE := $(CFLAGS) $(OPTIM)
SANI    := $(CFLAGS) -fsanitize=undefined -fsanitize=address $(DEBUG)
VALG    := $(CFLAGS)  $(DEBUG)

NCLS    := $(wildcard Test/*.ncl)
PRES := $(NCLS:.ncl=.pres)
IRES := $(NCLS:.ncl=.ires)

NCLS_EXTENSION := $(wildcard Test_extension/*.ncl)
PRES_EXTENSION := $(NCLS_EXTENSION:.ncl=.pres)
IRES_EXTENSION := $(NCLS_EXTENSION:.ncl=.ires)

LIBS    := -lm

parse_r: nuclei.c general.c nuclei.h
	$(CC) nuclei.c general.c $(RELEASE) -o parse $(LIBS)

parse_s: nuclei.c general.c nuclei.h
	$(CC) nuclei.c general.c $(SANI) -o parse_s $(LIBS)

parse_v: nuclei.c general.c nuclei.h
	$(CC) nuclei.c general.c $(VALG) -o parse_v $(LIBS)

all: parse_r parse_s parse_v interp interp_s interp_v

interp_r: nuclei.c linked.c general.c nuclei.h lisp.h general.h
	$(CC) nuclei.c linked.c general.c $(RELEASE) -DINTERP -o interp $(LIBS)

interp_s: nuclei.c linked.c general.c nuclei.h lisp.h general.h
	$(CC) nuclei.c linked.c general.c $(SANI) -DINTERP -o interp_s $(LIBS)

interp_v: nuclei.c linked.c general.c nuclei.h lisp.h general.h
	$(CC) nuclei.c linked.c general.c $(VALG) -DINTERP -o interp_v $(LIBS)

extension_r: nuclei.c linked.c general.c nuclei.h lisp.h general.h
	$(CC) nuclei.c linked.c general.c $(RELEASE) -DINTERP -o extension $(LIBS)
extension_s: nuclei.c linked.c general.c nuclei.h lisp.h general.h
	$(CC) nuclei.c linked.c general.c $(SANI) -DINTERP -o extension_s $(LIBS)
extension_v: nuclei.c linked.c general.c nuclei.h lisp.h general.h
	$(CC) nuclei.c linked.c general.c $(VALG) -DINTERP -o extension_v $(LIBS)

# For all .ncl files, run them and output result to a .pres (prase result) 
# or .ires (interpretted result) file.
runall : ./parse_s ./interp_s $(PRES) $(IRES) $(PRES_EXTENSION) $(IRES_EXTENSION)

run_parse: ./parse_s $(PRES)

run_interp: ./interp_s $(IRES)

run_extension: ./extension_s $(PRES_EXTENSION) $(IRES_EXTENSION)

parse: parse_r parse_s parse_v run_parse

interp: interp_r interp_s interp_v run_interp

extension: parse_s interp_s extension_r extension_s extension_v run_extension

%.pres:
	-./parse_s  $*.ncl > $*.pres 2>&1
%.ires:
	-./interp_s $*.ncl > $*.ires 2>&1

clean:
	rm -f parse parse_s parse_v interp interp_s interp_v extension extension_s extension_v $(PRES) $(IRES)
	rm -f $(PRES_EXTENSION) $(IRES_EXTENSION)