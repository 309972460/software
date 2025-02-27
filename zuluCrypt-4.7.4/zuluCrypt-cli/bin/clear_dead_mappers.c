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

#include "includes.h"
#include "../lib/includes.h"
#include <sys/types.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <libintl.h>
#include <libcryptsetup.h>

static void _remove_mapper( const char * path,stringList_t stl,uid_t uid )
{
	char * m_point = NULL ;
	/*
	 * zuluCryptBindUnmountVolume() is defined in ./bind.c
	 */
	int r = zuluCryptBindUnmountVolume( stl,path,uid )  ;

	if( r == 3 || r == 4 ){
		/*
		 * shared mount is busy or belong to another user
		 */
		return ;
	}

	/*
	 * zuluCryptCloseVolume() is defined in ../lib/close_volume.c
	 */
	r = zuluCryptCloseVolume( path,&m_point ) ;

	if( r == 0 ){
		if( m_point != NULL ){
			remove( m_point ) ;
			free( m_point ) ;
		}
	}
}

void zuluCryptClearDeadMappers( uid_t uid )
{
	struct crypt_device * cd ;
	const char * dir_path = crypt_get_dir() ;
	DIR * dir = opendir( dir_path ) ;
	struct dirent * entry ;
	const char * m ;
	const char * e ;
	size_t len ;
	size_t len1 ;

	stringList_t stl ;
	string_t p ;
	string_t z ;
#if TRUECRYPT_TCPLAY
	/*
	 * return pematurely because crypt_get_device_name() does not work
	 * as expected with truecrypt volumes opened with tcplay
	 */
	return ;
#endif
	if( dir == NULL ){
		return ;
	}

	/*
	 * zuluCryptGetMoutedListFromMountInfo_1()  is defined in ../lib/process_mountinfo.c
	 */
	stl = zuluCryptGetMoutedListFromMountInfo_1() ;
	z = String( dir_path ) ;

	StringAppend( z,"/" ) ;
	len1 = StringLength( z ) ;

	p = String( "zuluCrypt-" );
	m = StringAppendInt( p,uid ) ;
	len = StringLength( p ) ;

	/*
	 * zuluCryptSecurityGainElevatedPrivileges() is defined in security.c
	 */
	zuluCryptSecurityGainElevatedPrivileges() ;

	while( ( entry = readdir( dir ) ) != NULL ){
		if( StringPrefixMatch( entry->d_name,m,len ) ){
			e = StringAppendAt( z,len1,entry->d_name ) ;
			if( crypt_init_by_name( &cd,e ) == 0 ){
				if( crypt_get_device_name( cd ) == NULL ){
					/*
					 * we will get here if PLAIN or TRUECRYPT mapper is active but the underlying device is gone
					 */
					_remove_mapper( e,stl,uid ) ;
				}
				crypt_free( cd ) ;
			}else{
				/*
				 * we will get here if the LUKS mapper is active but the underlying device is gone
				 */
				_remove_mapper( e,stl,uid ) ;
			}
		}
	}

	/*
	 * zuluCryptSecurityDropElevatedPrivileges() is defined in security.c
	 */
	zuluCryptSecurityDropElevatedPrivileges() ;

	StringListDelete( &stl ) ;
	StringMultipleDelete( &p,&z,NULL ) ;
	closedir( dir ) ;
}
