/*
 *
 *  Copyright (c) 2011
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

#include "includes.h"

#include <sys/syscall.h>
#include <libcryptsetup.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>

static inline int zuluExit( int st,string_t m )
{
	StringDelete( &m ) ;
	return st ;
}

int zuluCryptCreateFileSystemInAVolume( const char * fs,const char * device_mapper )
{
	int status ;
	char * e = NULL ;
	process_t p ;

	/*
	 * zulucryptFileSystemIsSupported() is defined in mount_fs_options.c
	 */
	if( zulucryptFileSystemIsSupported( fs ) == 0 ){
		return 1 ;
	}

	p = Process( ZULUCRYPTmkfs ) ;

	if( StringAtLeastOneMatch_1( fs,"ext2","ext3","ext4",NULL ) ){

		ProcessSetArgumentList( p,"-t",fs,"-m","1",device_mapper,NULL ) ;

	}else if( StringsAreEqual( fs,"reiserfs" ) ){

		ProcessSetArgumentList( p,"-t",fs,"-f","-f","-q",device_mapper,NULL ) ;

	}else if( StringsAreEqual( fs,"jfs" ) ){

		ProcessSetArgumentList( p,"-t",fs,"-q",device_mapper,NULL ) ;

	}else if( StringsAreEqual( fs,"ntfs" ) ){

		ProcessSetArgumentList( p,"-t",fs,"-f",device_mapper,NULL ) ;

	}else if( StringsAreEqual( fs,"xfs" ) ){

		ProcessSetArgumentList( p,"-t",fs,"-f",device_mapper,NULL ) ;

	}else{
		ProcessSetArgumentList( p,"-t",fs,device_mapper,NULL ) ;

		/*
		 * unhandled fs are processed here.They are given 60 seconds to accomplish their task
		 * and are assumed to be running in interactive more and are blocked waiting for user input
		 * when they fail to return in time and hence are killed since we cant get to them from GUI
		 */

		ProcessSetOptionTimeout( p,60,SIGKILL ) ;
	}

	ProcessStart( p ) ;

	status = ProcessExitStatus( p ) ;

	if( status ){
		ProcessGetOutPut( p,&e,ProcessStdError ) ;
		if( e ){
			puts( e ) ;
			free( e ) ;
		}
	}

	ProcessDelete( &p ) ;
	return status ;
}

static int _create_volume( const char * dev,const char * fs,const char * type,const char * pass,size_t pass_size,const char * rng )
{
	int r ;
	
	const char * mapper ;

	string_t m = String( "/zuluCrypt-create-volume-" ) ;
	mapper = StringAppendInt( m,syscall( SYS_gettid ) ) + 1 ;

	if( StringsAreEqual( type,"luks" ) ){

		if( StringAtLeastOnePrefixMatch( rng,"/dev/random","/dev/urandom",NULL ) ){

			if( zuluCryptCreateLuks( dev,pass,pass_size,rng ) != 0 ){

				return zuluExit( 3,m ) ;
			}
			if( zuluCryptOpenLuks( dev,mapper,"rw",pass,pass_size ) != 0 ){

				return zuluExit( 3,m ) ;
			}
		}else{
			return zuluExit( 2,m ) ;
		}

	}else if( StringsAreEqual( type,"plain") ){

		if( zuluCryptOpenPlain( dev,mapper,"rw",pass,pass_size ) != 0 ){

			return zuluExit( 3,m ) ;
		}
	}else{
		return zuluExit( 2,m ) ;
	}

	mapper = StringPrepend( m,crypt_get_dir() ) ;

	r = zuluCryptCreateFileSystemInAVolume( fs,mapper ) ;
	/*
	 * zuluCryptCloseMapper() is defined in close_mapper.c
	 */
	zuluCryptCloseMapper( mapper ) ;

	if( r == 0 ){
		return zuluExit( 0,m ) ;
	}else{
		return zuluExit( 3,m ) ;
	}
}

int zuluCryptCreateVolume( const char * dev,const char * fs,const char * type,const char * pass,size_t pass_size,const char * rng )
{
	string_t st ;
	int fd ;
	int r ;
	if( StringPrefixEqual( dev,"/dev/" ) ){
		return _create_volume( dev,fs,type,pass,pass_size,rng ) ;
	}else{
		/*
		 * zuluCryptAttachLoopDeviceToFile() is defined in ./create_loop.c
		 */
		if( zuluCryptAttachLoopDeviceToFile( dev,O_RDWR,&fd,&st ) ){
			r = _create_volume( StringContent( st ),fs,type,pass,pass_size,rng ) ;
			StringDelete( &st ) ;
			close( fd ) ;
			return r ;
		}else{
			return 3 ;
		}
	}
}
