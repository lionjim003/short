
# short

Usage: `short STRING...`  
Print scripts that contain STRING... as content.

Expand to absolute path when STRING is a relative path and replaces the drive
letters when executed. short also appends argument list to the end of content.  
```
short  STRING!...
short  STRING## STRING...
```
Append `!` to end of STRING to disable path expansion and drive letter replace.    
Append `##` to end of STRING to concatenate the next STRING.
