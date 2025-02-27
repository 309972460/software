/*
 *
 *  Copyright (c) 2012
 *  name : Francis Banyikwa
 *  email: mhogomchungu@gmail.com
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include "process.h"

#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <grp.h>

struct ProcessType_t{
	pid_t pid ;
	int fd_0[ 2 ] ; /* this variable is used to write to child process      */
	int fd_1[ 2 ] ; /* this variable is used to read from child's std out   */
	int fd_2[ 2 ] ; /* this variable is used to read from child's std error */
	ProcessStatus state ;
	int std_io ;
	char * exe ;
	char ** args ;
	int signal ;
	int wait_status ;
	pthread_t * thread ;
	ProcessStructure str ;
};

static void ( *_fcn_ )( void )  = NULL ;

void ProcessExitOnMemoryExaustion( void ( *f )( void ) )
{
	_fcn_ = f ;
}

ProcessStructure * ProcessArgumentStructure( process_t p )
{
	if( p == ProcessVoid ){
		return NULL ;
	}else{
		return &p->str ;
	}
}

void ProcessSetEnvironmentalVariable( process_t p,const char * const * env )
{
	if( p != ProcessVoid ){
		p->str.env = env ;
	}
}

static process_t _ProcessError( void )
{
	if( _fcn_ != NULL ){
		( *_fcn_ )() ;
	}
	return ProcessVoid ;
}

void ProcessSetArgumentList( process_t p,... )
{
	char * entry ;
	char ** args  ;
	char ** e ;
	size_t size = sizeof( char * ) ;
	int index = 0 ;
	va_list list ;

	if( p == ProcessVoid ){
		return ;
	}

	args = ( char ** )malloc( size ) ;

	if( args == NULL ){
		_ProcessError() ;
		return ;
	}

	args[ index ] = p->exe ;
	index++ ;

	va_start( list,p ) ;

	while( 1 ){
		entry = va_arg( list,char * ) ;
		e = ( char ** )realloc( args,( 1 + index ) * size ) ;

		if( e == NULL ){
			free( args ) ;
			_ProcessError() ;
			va_end( list ) ;
			return ;
		}else{
			args = e ;
		}

		if( entry == NULL ){
			args[ index ] = NULL ;
			break ;
		}else{
			args[ index ] = entry ;
			index++ ;
		}
	}

	va_end( list ) ;
	p->args = args ;
	p->str.args = ( const char * const * ) args ;
}

static void * __timer( void * x )
{
	process_t  p = ( process_t ) x ;

	sleep( p->str.timeout ) ;

	kill( p->pid,p->str.signal ) ;

	p->state = ProcessCancelled ;

	return ( void * ) 0 ;
}

static void __ProcessStartTimer( process_t p )
{
	p->thread = ( pthread_t * ) malloc( sizeof( pthread_t ) ) ;

	if( p->thread == NULL ){
		_ProcessError()  ;
	}else{
		pthread_create( p->thread,NULL,__timer,( void * ) p );
	}
}

void ProcessSetOptionPriority( process_t p,int priority )
{
	if( p != ProcessVoid ){
		p->str.priority = priority ;
	}
}

pid_t ProcessStart( process_t p )
{
	if( pipe( p->fd_0 ) == -1 ){
		return -1 ;
	}
	if( pipe( p->fd_1 ) == -1 ){
		return -1 ;
	}
	if( pipe( p->fd_2 ) == -1 ){
		return -1 ;
	}

	p->pid = fork() ;

	if( p->pid == -1 ){
		return -1 ;
	}
	if( p->pid == 0 ){
		if( p->str.user_id != ( uid_t )-1 ){
			/*
			 * drop privileges permanently
			 */
			seteuid( 0 ) ;
			setgid( p->str.user_id ) ;
			setgroups( 1,&p->str.user_id ) ;
			setegid( p->str.user_id ) ;
			setuid( p->str.user_id ) ;
		}

		dup2( p->fd_0[ 0 ],0 ) ;
		dup2( p->fd_1[ 1 ],1 ) ;
		dup2( p->fd_2[ 1 ],2 ) ;

		close( p->fd_1[ 0 ] ) ;
		close( p->fd_0[ 1 ] ) ;
		close( p->fd_2[ 0 ] ) ;

		if( p->str.priority != 0 ){
			setpriority( PRIO_PROCESS,0,p->str.priority ) ;
		}

		if( p->str.args == NULL ){
			#define _null ( void * )0
			if( p->str.env != NULL ){
				execle( p->exe,p->exe,_null,p->str.env ) ;
			}else{
				execl( p->exe,p->exe,_null ) ;
			}
		}else{
			#define _cast( x ) ( char * const * )x
			if( p->str.env != NULL ){
				execve( p->str.args[ 0 ],_cast( p->str.args ),_cast( p->str.env ) ) ;
			}else{
				execv( p->str.args[ 0 ],_cast( p->str.args ) ) ;
			}
		}
		/*
		 * execv has failed :-(
		 */
		_Exit( 1 ) ;
		/*
		 * child process block ends here
		 */
	}

	/*
	 * parent process continues from here
	 */
	close( p->fd_0[ 0 ] ) ;
	close( p->fd_1[ 1 ] ) ;
	close( p->fd_2[ 1 ] ) ;

	p->state = ProcessIsStillRunning ;

	if( p->str.timeout != -1 ){
		__ProcessStartTimer( p ) ;
	}

	return p->pid ;
}

#define FACTOR 2
static inline char * __bufferExpandMemory( char * buffer,size_t new_size,size_t * buffer_size )
{
	char * e ;
	if( new_size >= *buffer_size ) {
		*buffer_size = new_size * FACTOR ;
		e = ( char * )realloc( buffer,*buffer_size ) ;
		if( e == NULL ){
			free( buffer )  ;
			_ProcessError() ;
			return NULL     ;
		}else{
			return e ;
		}
	}else{
		return buffer ;
	}
}

size_t ProcessGetOutPut( process_t p,char ** data,ProcessIO std_io )
{
	#define SIZE 64
	#define BUFFER_SIZE 128
	char * buffer ;
	char * e ;
	char buff[ SIZE ] ;
	size_t size = 0 ;
	size_t count ;
	size_t buffer_size = BUFFER_SIZE ;

	int fd ;

	if( p == ProcessVoid ){
		return 0 ;
	}

	buffer = ( char * ) malloc( sizeof( char ) * BUFFER_SIZE ) ;

	if( buffer == NULL ){
		_ProcessError() ;
		return 0 ;
	}

	switch( std_io ){
		case ProcessStdOut   : fd = p->fd_1[ 0 ] ; break ;
		case ProcessStdError : fd = p->fd_2[ 0 ] ; break ;
		default  : return 0 ;
	}

	while( 1 ) {
		count = read( fd,buff,SIZE ) ;
		buffer = __bufferExpandMemory( buffer,size + count,&buffer_size ) ;

		if( buffer == NULL ){
			return 0 ;
		}else{
			memcpy( buffer + size,buff,count ) ;
		}

		size += count ;

		if( count < SIZE ){
			break ;
		}
	}

	if( size > 0 ){
		e = realloc( buffer,size + 1 ) ;
		if( e == NULL ){
			free( buffer ) ;
			_ProcessError() ;
			return 0 ;
		}else{
			e[ size ] = '\0' ;
			*data = e ;
		}
	}

	return size ;
}

ProcessStatus ProcessState( process_t p )
{
	if( p != ProcessVoid ){
		return p->state ;
	}else{
		return ProcessStatusUndefined ;
	}
}

ssize_t ProcessGetOutPut_1( process_t p,char * buffer,int size,ProcessIO std_io )
{
	if( p != ProcessVoid ){
		switch( std_io ){
			case ProcessStdOut   : return read( p->fd_1[ 1 ],buffer,size ) ;
			case ProcessStdError : return read( p->fd_2[ 1 ],buffer,size ) ;
			default              : return -1 ;
		}
	}else{
		return -1 ;
	}
}

size_t ProcessWrite( process_t p,const char * data,size_t len )
{
	if( p != ProcessVoid ){
		return write( p->fd_0[ 1 ],data,len ) ;
	}else{
		return 0 ;
	}
}

void ProcessCloseStdWrite( process_t p )
{
	close( p->fd_0[ 0 ] ) ;
	p->fd_0[ 0 ] = -1 ;
}

process_t Process( const char * path )
{
	process_t p  ;

	size_t len ;

	if( path == NULL ){
		return ProcessVoid ;
	}

	len = strlen( path ) ;

	p = ( process_t ) malloc( sizeof( struct ProcessType_t ) ) ;

	if( p == NULL ){
		return _ProcessError() ;
	}

	if( len == 0 ){
		p->exe = NULL ;
	}else{
		p->exe = ( char * ) malloc( sizeof( char ) * ( len + 1 ) ) ;
		if( p->exe == NULL ){
			free( p ) ;
			return _ProcessError() ;
		}
		memcpy( p->exe,path,len + 1 ) ;
	}

	p->std_io = 0       ;
	p->wait_status = -1 ;
	p->thread = NULL    ;
	p->fd_0[ 0 ] = -1   ;
	p->args     = NULL  ;
	p->str.args = NULL  ;
	p->str.timeout = -1 ;
	p->str.env = NULL   ;
	p->str.user_id = -1 ;
	p->str.priority = 0 ;
	p->str.signal = SIGTERM ;
	p->state = ProcessHasNotStarted ;
	return p ;
}

void ProcessSetOptionTimeout( process_t p,int timeout,int signal )
{
	if( p != ProcessVoid ){
		p->str.signal  = signal ;
		p->str.timeout = timeout ;
	}
}

void ProcessDelete( process_t * p )
{
	process_t px ;

	if( p == NULL || *p == ProcessVoid ){
		return ;
	}

	px = *p ;
	*p = ProcessVoid ;

	if( px->thread != NULL ){
		pthread_cancel( *(px)->thread ) ;
		free( px->thread ) ;
	}

	close( px->fd_2[ 1 ] ) ;
	close( px->fd_1[ 1 ] ) ;

	if( px->fd_0[ 0 ] != -1 ){
		close( px->fd_0[ 0 ] ) ;
	}
	if( px->wait_status == -1 ){
		waitpid( px->pid,0,WNOHANG ) ;
	}
	if( px->args != NULL ){
		free( px->args ) ;
	}
	if( px->exe != NULL ){
		free( px->exe ) ;
	}
	free( px ) ;
}

int ProcessTerminate( process_t p )
{
	int st ;

	if( p == ProcessVoid ){
		return -1 ;
	}else{
		p->state = ProcessCancelled ;
		st = kill( p->pid,SIGTERM ) ;
		waitpid( p->pid,0,WNOHANG ) ;
		p->wait_status = 1 ;
		return st ;
	}
}

void ProcessSetOptionUser( process_t p,uid_t uid )
{
	if( p != ProcessVoid ){
		p->str.user_id = uid ;
	}
}

int ProcessKill( process_t p )
{
	int st ;
	if( p == ProcessVoid ){
		return -1 ;
	}else{
		p->state = ProcessCancelled ;
		st = kill( p->pid,SIGKILL ) ;
		waitpid( p->pid,0,WNOHANG ) ;
		p->wait_status = 1 ;
		return st ;
	}
}

int ProcessExitStatus( process_t p )
{
	int status ;

	if( p == ProcessVoid ){
		return -1 ;
	}else{
		waitpid( p->pid,&status,0 ) ;
		p->state = ProcessCompleted ;
		p->wait_status = 1 ;
		if( WIFEXITED( status ) == 0 ){
			return -1 ;
		}else{
			return WEXITSTATUS( status ) ;
		}
	}
}

void ProcessSetArguments( process_t p,const char * const s[] )
{
	if( p != ProcessVoid ){
		p->str.args = s ;
	}
}
