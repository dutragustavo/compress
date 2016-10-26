#include "file_writter.h"
#include "buffer.h"
#include "util.h"
#include <assert.h>
#include <pthread.h>

struct file_writter
{
    buffer_t buffer;
    pthread_t buffer_worker;
    FILE* file;
};

static void * buffer_worker( void * );

file_writter_t file_writter_create( FILE *file )
{
    assert( file != NULL);
    file_writter_t writter = smalloc( sizeof( struct file_writter ) );
    writter->buffer = buffer_create( FILE_WRITTER__BUFFER_SIZE );
    writter->file = file;
	pthread_create( &writter->buffer_worker , NULL, buffer_worker, writter );
    return writter;
}

void file_writter_write( file_writter_t writter, unsigned data_to_write )
{
    assert( data_to_write != EOF );
    buffer_put( writter->buffer, data_to_write );
}

void file_writter_destroy(  file_writter_t writter )
{
    file_writter_write( writter, EOF );
    pthread_join( writter->buffer_worker, NULL );
    buffer_destroy( writter->buffer );
    free( writter );
}

static void * buffer_worker( void * arg )
{
	unsigned ch;
	file_writter_t writter = ( file_writter_t ) arg;

	/* Read data from buffer. */
	while( 1 )
    {
		ch = buffer_get( writter->buffer );
        if( ch == EOF )
        {
            break;
        }

		fputc( ch, writter->file );
    }

    return NULL;
}
