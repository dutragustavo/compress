#include "file_reader.h"
#include "buffer.h"
#include "util.h"
#include <pthread.h>
#include <assert.h>

#define FILE_READER_BUFFER_SIZE 1024

struct file_reader
{
    buffer_t buffer;
    pthread_t buffer_worker;
    FILE* file;
};

static void * buffer_worker( void * arg );

file_reader_t file_reader_create( FILE *file )
{
    assert( file != NULL);
    file_reader_t reader = smalloc( sizeof( struct file_reader ) );
    reader->buffer = buffer_create( FILE_READER_BUFFER_SIZE );
    reader->file = file;
	pthread_create( &reader->buffer_worker , NULL, buffer_worker, reader );
    pthread_detach( reader->buffer_worker );
    return reader;
}


unsigned file_reader_read( file_reader_t reader, unsigned *data_read )
{
    (*data_read) = buffer_get( reader->buffer );
    return (*data_read) != EOF;
}

void file_reader_destroy( file_reader_t reader )
{
    assert( reader != NULL);
    buffer_destroy( reader->buffer );
    free( reader );
}

static void * buffer_worker( void * arg )
{
	int ch;
	file_reader_t reader = ( file_reader_t ) arg;

	/* Read data from file to the buffer. */
	while( ( ch = fgetc( reader->file ) ) != EOF )
    {
		buffer_put( reader->buffer, ch );
    }

	buffer_put( reader->buffer, EOF );
    return NULL;
}
