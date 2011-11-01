#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "lex.h"
#include "parse.h"
#include "memdbg.h"

int warnings = 0;

#if defined(WIN32)
	#include <io.h>

	#define FINDDATA _finddata_t
	#define FINDFIRST(x, y) _findfirst(x, y)
	#define FINDNEXT(x, y) _findnext(x, y)
	#define FINDCLOSE(x, y) _findclose(x)
	#define FINDHANDLE long
//	#define BAD_HANDLE -1
	
	#if defined(_MSC_VER)
		#define FIND_SUCCESS(x)	((x) != -1)
	#else
		#define FIND_SUCCESS(x)	((x) == 0)
	#endif
#else
	#include <dos.h>

	#define FINDDATA _find_t
	#define FINDFIRST(x, y) _dos_findfirst(x, _A_NORMAL, y)
	#define FINDNEXT(x, y) _dos_findnext(y)

	#if defined(DJGPP)
		#define FINDCLOSE(x, y) //_dos_findclose(y)
	#else
		#define FINDCLOSE(x, y) _dos_findclose(y)
	#endif

	#define FINDHANDLE int
//	#define BAD_HANDLE 0
	#define FIND_SUCCESS(x)	((x) != -1)
#endif

int main(int argc, char **argv)
{
	InputStream foo;
	char name[80], *c, *file;
	struct FINDDATA buf;
	FINDHANDLE handle;

#ifndef FALLOUT_VERSION
	printf("Startreck scripting language compiler (Fallout 2 edition)\n\n");
#else
	#if (FALLOUT_VERSION != 2)
		printf("Startreck scripting language compiler (Fallout 1 edition)\n\n");
	#endif
#endif

	if (argc < 2) {
		printf("Usage: compile {switches} filename [filename [..]]\n");
		exit(1);
	}

	foo.type = INPUT_FILE;

	while((argv[1] != NULL) && (argv[1][0] == '-')) {
		switch(argv[1][1]) {
			case 'w':
				warnings = 1;
				break;

			default:
				printf("Unknown option %c\n", argv[1][1]);
		}

		argv++;
		argc--;
	}

	compilerErrorTotal = 0;

	while(argv[1]) {
		file = argv[1];
		argv++;

		if (FIND_SUCCESS(handle = FINDFIRST(file, &buf))) {
			do {
				strcpy(foo.name, buf.name);

				if ((foo.file = fopen(buf.name, "r")) == 0) {
					printf("Couldn't find file %s\n", argv[0]);
					exit(1);
				}

				fprintf(stderr, "Compiling %s\n", buf.name);
				strcpy(name, buf.name);
				c = strrchr(name, '.');

				if (c) {
					*c = 0;
				}

				strcat(name, ".int");

				if (strcmp(name, buf.name) == 0) {
					c = strrchr(name, '.');
					*c = 0;
					*--c = 0;
					strcat(name, "1.int");
				}

				parse(&foo, name);
				freeCurrentProgram();
				fclose(foo.file);
			} while (!FINDNEXT(handle, &buf));

			FINDCLOSE(handle, &buf);

			if (compilerErrorTotal) {
				printf( "\n*** THERE WERE ERRORS (%u of them)\n", compilerErrorTotal);
				return EXIT_FAILURE;
			}
		}
		else  {
			fprintf(stderr, "Warning: %s not found\n", file);
		}
	}

	return EXIT_SUCCESS;
}
