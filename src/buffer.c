/*
 * Copyright(C) 2014-2016 Pedro H. Penna <pedrohenriquepenna@gmail.com>
 * 
 * This file is part of compress.
 * 
 * compress is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * compress is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with compress. If not, see <http://www.gnu.org/licenses/>.
 */

#include <assert.h>
#include <buffer.h>
#include <pthread.h>
#include <semaphore.h>
#include <util.h>

/*
 * Buffer.
 */
struct buffer
{
	unsigned *data; /* Data.                        */
	unsigned size;  /* Max size (in elements).      */
	unsigned first; /* First element in the buffer. */
	unsigned last;  /* Last element in the buffer.  */
	pthread_mutex_t mutex_read;
	pthread_mutex_t mutex_write;
	sem_t sem_write;
	sem_t sem_read;
};

/*
 * Creates a buffer.
 */
struct buffer *buffer_create(unsigned size)
{
	struct buffer *buf;
	
	/* Sanity check. */
	assert(size > 0);

	buf = smalloc( sizeof( struct buffer ) );
	
	/* Initialize buffer. */
	buf->size = size;
	buf->data = smalloc( size * sizeof ( unsigned ) );
	buf->first = 0;
	buf->last = 0;

	pthread_mutex_init(&buf->mutex_read, NULL);
	pthread_mutex_init(&buf->mutex_write, NULL);

	sem_init(&buf->sem_read, 0, 0);
	sem_init(&buf->sem_write, 0, size);

	return (buf);
}

/*
 * Destroys a buffer.
 */
void buffer_destroy(struct buffer *buf)
{
	/* Sanity check. */
	assert(buf != NULL);
	
	/* House keeping. */
	free(buf->data);

	pthread_mutex_destroy(&buf->mutex_read);
	pthread_mutex_destroy(&buf->mutex_write);

	sem_destroy(&buf->sem_write);
	sem_destroy(&buf->sem_read);

	free(buf);
}

/*
 * Puts an item in a buffer.
 */
void buffer_put(struct buffer *buf, unsigned item)
{
	/* Sanity check. */
	assert(buf != NULL);
	assert( buf->last < buf->size );

	/*Init synchronize region */
	sem_wait(&buf->sem_write);

	/*Init critic region */
	pthread_mutex_lock(&buf->mutex_write);

    buf->data[buf->last++] = item;
    buf->last %= buf->size;
	sem_post(&buf->sem_read);

	/*End critic region */
	pthread_mutex_unlock(&buf->mutex_write);

	/*End synchronize region */
}

/*
 * Gets an item from a buffer.
 */
unsigned buffer_get(struct buffer *buf)
{
	unsigned item;

	/* Sanity check. */
	assert(buf != NULL);
	assert( buf->first < buf->size );

	/*Init synchronize region */
	sem_wait(&buf->sem_read);

	/*Init critic region */
	pthread_mutex_lock(&buf->mutex_read);

    item = buf->data[buf->first++];
    buf->first %= buf->size;
	sem_post(&buf->sem_write);

	/*End critic region */
	pthread_mutex_unlock(&buf->mutex_read);

	return (item);
    /*End synchronize region */
}
