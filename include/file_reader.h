#ifndef FILE_READER_H_
#define FILE_READER_H_

#include <stdio.h>

    typedef struct file_reader* file_reader_t;

	extern file_reader_t file_reader_create( FILE *file );
    /**
     *This function read a file and put the value in parameter data_read, the value of
     * and of file is not read.
     *
     *@return return 0 if end of file reached and 1 if not
     * */
	extern unsigned file_reader_read( file_reader_t reader, unsigned *data_read );
	extern void file_reader_destroy( file_reader_t reader );

#endif
