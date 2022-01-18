#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include <Shlwapi.h>

#define PROGRAM_NAME "short"

#define PROGRAM_VERSION "3.10"

#define DISABLE "!"

#define CONNECT "##"

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#define USAGE() \
	fputs("\
Usage: "PROGRAM_NAME" STRING...\n\
Print scripts that contain STRING... as content.\n\
\n\
Expand to absolute path when STRING is a relative path and replaces the drive\n\
letters when executed. "PROGRAM_NAME" also appends argument list to the end of content.\n\
\n\
    "PROGRAM_NAME"  STRING"DISABLE"...\n\
    "PROGRAM_NAME"  STRING"CONNECT" STRING...\n\
\n\
Append '"DISABLE"' to end of STRING to disable path expansion and drive letter replace.\n\
Append '"CONNECT"' to end of STRING to concatenate the next STRING.\n\
", stdout),0

#define VERSION() \
	puts(PROGRAM_NAME" (Homemade tool) "PROGRAM_VERSION),0

#define STREQ(str1,str2) \
	strcmp(str1,str2)==0

#define LAST(n,ptr) \
	(ptr+strlen(ptr)-n)

int main (int argc, char **argv) {
	
	char i = 1,
		 append = true,
		 connect = true,
		 disable = false,
		 buffer[MAX_PATH + 1],
		 *ptr;
		
	if (argc <= i ||
		STREQ (argv[i], "--help"))
		return USAGE ();
		
	if (STREQ (argv[i], "--version"))
		return VERSION ();
	
	if (argv[i][0] != '@')
		fputc ('@', stdout);
	
	do {
		if (!connect)
			fputc (' ', stdout);
		
		connect = STREQ (CONNECT, LAST (2, argv[i]));
		
		disable = STREQ (DISABLE, LAST (1, argv[i]));
		
		append *= (ptr = strstr (argv[i], "%*")) ?
					*(--ptr) == '%' : 1;
		
		strcpy (buffer, argv[i]);
		
		if (connect)
			*LAST (2, buffer) = '\0';
		
		if (disable)
			fputs (buffer, stdout);
		
		else {
			if (PathFileExistsA (buffer)) {
				GetFullPathNameA (buffer, MAX_PATH, buffer, NULL);
				printf ("%%~d0%s", buffer + 2);
				
			} else {
				char found = false,
					 *last = LAST (0, buffer);
					 
				GetEnvironmentVariable ("PATHEXT", last, MAX_PATH);
				strcat (buffer, ";");
				
				WIN32_FIND_DATAA file;
				while ((ptr = strchr (last, ';')) != NULL) {
					*ptr = '\0';
					
					if (FindFirstFileA (buffer, &file) != INVALID_HANDLE_VALUE) {
						ptr = strrchr (buffer, '\\');
						strcpy (ptr == NULL ? buffer : ptr + 1, file.cFileName);
						GetFullPathNameA (buffer, MAX_PATH, buffer, NULL);
						printf ("%%~d0%s", buffer + 2);
						FindClose (&file);
						found = true;
						break;
					}
					strcpy (last, ptr + 1);
				}
				if (!found) {
					*last = '\0';
					fputs (buffer, stdout);
				}
			}
		}
		
	} while (++i < argc);
	
	if (append)
		fputs (" %*", stdout);
	
	return 0;
}
