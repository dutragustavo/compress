#ifndef FILE_WRITTER_H_
#define FILE_WRITTER_H_

#include <stdio.h>

    typedef struct file_writter* file_writter_t;

	extern file_writter_t file_writter_create( FILE *file );
	extern void file_writter_write( file_writter_t writter, unsigned data_to_write );
	extern void file_writter_destroy(  file_writter_t writter );

#endif
