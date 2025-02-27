/*
 *
 *  Copyright (c) 2013
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

#include <sys/stat.h>
#include <fcntl.h>
#include "../constants.h"
#include "../lib/includes.h"
#include "includes.h"
#include <errno.h>
#include <unistd.h>

static int has_device_access( const char * path,int c )
{
	int f ;

	if( c == ZULUCRYPTread ){
		f = open( path,O_RDONLY ) ;
	}else{
		f = open( path,O_WRONLY ) ;
	}

	if( f == -1 ){
		switch( errno ){
			case EACCES : return 1 ; /* permission denied */
			case ENOENT : return 2 ; /* invalid path*/
			default     : return 4 ; /* common error */
		}
	}else{
		close( f ) ;
		return 0 ;
	}
}

/*
 * 1-permissions denied
 * 2-invalid path
 * 3-shenanigans
 * 4-common error
 */
static int path_is_accessible( const char * path,uid_t uid,int action )
{
	int st ;
	char * xt ;

	if( uid ){;}

	if( StringPrefixEqual( path,"/dev/shm/" ) ){
		return 4 ;
	}
	if( StringPrefixEqual( path,"/dev/" ) ){
		if( StringPrefixEqual( path,"/dev/loop" ) ){
			/*
			 * zuluCryptLoopDeviceAddress_1() is defined in ../zuluCrypt-cli/create_loop_device.c
			 */
			xt = zuluCryptLoopDeviceAddress_1( path ) ;
			if( xt != NULL ){
				st = has_device_access( xt,action ) ;
				free( xt ) ;
			}else{
				return 4 ;
			}
		}else{
			zuluCryptSecurityGainElevatedPrivileges() ;
			st = has_device_access( path,action ) ;
			zuluCryptSecurityDropElevatedPrivileges() ;
		}
		return st ;
	}else{
		zuluCryptSecurityDropElevatedPrivileges() ;
		return has_device_access( path,action ) ;
	}
}

int zuluCryptCanOpenPathForReading( const char * path,uid_t uid )
{
	return path_is_accessible( path,uid,ZULUCRYPTread ) ;
}

int zuluCryptCanOpenPathForWriting( const char * path,uid_t uid )
{
	return path_is_accessible( path,uid,ZULUCRYPTwrite ) ;
}

/*
 *  return values:
 *  5 - couldnt get key from the socket
 *  4 -permission denied
 *  1  invalid path
 *  2  insufficient memory to open file
 *  0  success
 */
int zuluCryptGetPassFromFile( const char * path,uid_t uid,string_t * st )
{
	/*
	 * zuluCryptGetUserHomePath() is defined in ../lib/user_home_path.c
	 */
	string_t p     = zuluCryptGetUserHomePath( uid ) ;
	const char * z = StringAppend( p,".zuluCrypt-socket" ) ;
	size_t s       = StringLength( p ) ;
	int m          = StringPrefixMatch( path,z,s ) ;

	StringDelete( &p ) ;

	zuluCryptSecurityDropElevatedPrivileges() ;

	if( m ){
		/*
		 * path that starts with $HOME/.zuluCrypt-socket is treated not as a path to key file but as path
		 * to a local socket to get a passphrase
		 */
		/*
		 * zuluCryptGetKeyFromSocket() is defined in ../pluginManager/zuluCryptPluginManager.c
		 */
		return zuluCryptGetKeyFromSocket( path,st,uid ) ;
	}else{
		/*
		 * 8192000 bytes is the default cryptsetup maximum keyfile size
		 */
		m = StringGetFromFileMemoryLocked( st,path,0,8192000 ) ;

		switch( m ){
			case 0 : return 0 ;
			case 1 : return 4 ;
			case 2 : return 2 ;
		}
		/*
		 * not supposed to get here
		 */
		return -1 ;
	}
}

char * zuluCryptEvaluateDeviceTags( const char * tag,const char * path )
{
	char * r ;

	zuluCryptSecurityGainElevatedPrivileges() ;

	/*
	 * zuluCryptDeviceFromUUID()  is defined in ../lib/blkid_evaluate_tag.c
	 * zuluCryptDeviceFromLabel() is defined in ../lib/blkid_evaluate_tag.c
	 */
	if( StringsAreEqual( tag,"UUID" ) ){
		r = zuluCryptDeviceFromUUID( path ) ;
	}else{
		r = zuluCryptDeviceFromLabel( path ) ;
	}

	zuluCryptSecurityDropElevatedPrivileges() ;

	return r ;
}

char * zuluCryptUUIDFromPath( const char * device )
{
	char * c ;

	zuluCryptSecurityGainElevatedPrivileges() ;

	/*
	 * zuluCryptUUIDFromPath_1() is defined in ../lib/blkid_evaluate_tag.c
	 */
	c = zuluCryptUUIDFromPath_1( device ) ;

	zuluCryptSecurityDropElevatedPrivileges() ;
	return c ;
}
