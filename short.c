#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include <Shlwapi.h>

#define PROGRAM_NAME "short"

#define PROGRAM_VERSION "3.00"

#define DISABLE "!"

#define CONNECT "##"

#define USAGE() \
	fputs("\
Usage: "PROGRAM_NAME" STRING...\n\
Print scripts that contain STRING... as content.\n\
\n\
Expands to absolute path when STRING is a relative path and replaces the drive\n\
letter when executed. "PROGRAM_NAME" also appends argument list to the end of content.\n\
\n\
	"PROGRAM_NAME"  "DISABLE" STRING...\n\
	"PROGRAM_NAME"  STRING... "DISABLE"\n\
	"PROGRAM_NAME"  STRING1 "CONNECT" STRING2\n\
\n\
Use '"DISABLE"' to disable path expansion and drive letter replace in the next STRING.\n\
Use '"DISABLE"' at the end of content to disable argument list appending.\n\
Use '"CONNECT"' to concatenate the previous STRING and the next STRING.\n\
", stdout),0

#define VERSION() \
	puts(PROGRAM_NAME" (Homemade tool) "PROGRAM_VERSION),0

#define STREQ(str1,str2) \
	strcmp(str1,str2)==0

int main (int argc, char **argv) {
	
	char i = 1,
		 begin = 1,
		 connect = 1,
		 disable = 0,
		 buffer[MAX_PATH + 1];
		 
	buffer[0] = '\0';
		
	if (argc <= i ||
		STREQ (argv[i], "--help"))
		return USAGE ();
		
	if (STREQ (argv[i], "--version"))
		return VERSION ();
	
	do {
		if (STREQ (argv[i], CONNECT) 
			&& (begin || !connect)) {
			connect = 1;
			disable = 0;
			continue;
		}
		if (STREQ (argv[i], DISABLE)
			&& (begin || !connect)) {
			disable = 1;
			continue;
		}
		if (!connect)
			fputc (' ', stdout);
		
		if (begin && argv[i][0] != '@')
			fputc ('@', stdout);
		
		strcpy (buffer, argv[i]);
		if (disable)
			fputs (buffer, stdout);
		
		else {
			if (PathFileExistsA (buffer)) {
				GetFullPathNameA (buffer, MAX_PATH, buffer, NULL);
				printf ("%%~d0%s", buffer + 2);
				
			} else {
				WIN32_FIND_DATAA file;
				strcat (buffer, ".*");
				if (FindFirstFileA (buffer, &file) != INVALID_HANDLE_VALUE) {
					char *ptr = strrchr (buffer, '\\');
					strcpy (ptr == NULL ? buffer : ptr + 1, file.cFileName);
					GetFullPathNameA (buffer, MAX_PATH, buffer, NULL);
					printf ("%%~d0%s", buffer + 2);
					FindClose (&file);
					
				} else {
					buffer[strlen (buffer) - 2] = '\0';
					fputs (buffer, stdout);
				}
			}
		}
		connect = 0;
		disable = 0;
		begin = 0;
		
	} while (++i < argc);
	
	if (!disable
		&& !STREQ (buffer + strlen (buffer) - 2, "%*"))
		fputs (" %*", stdout);
	
	return 0;
}
