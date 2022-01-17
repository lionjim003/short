#short

Usage: short STRING...
Print scripts that contain STRING... as content.

Expands to absolute path when STRING is a relative path and replaces the drive
letter when executed. short also appends argument list to the end of content.

	short  ! STRING...
	short  STRING... !
	short  STRING1 ## STRING2

Use '!' to disable path expansion and drive letter replace in the next STRING.
Use '!' at the end of content to disable argument list appending.
Use '##' to concatenate the previous STRING and the next STRING.
