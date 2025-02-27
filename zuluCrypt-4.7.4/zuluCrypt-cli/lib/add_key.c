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
#include <libcryptsetup.h>
#include <fcntl.h>
#include <unistd.h>

static inline int zuluExit( int st,struct crypt_device * cd )
{
	crypt_free( cd ) ;
	return st ;
}

static int _add_key( const char * device,const char * existingkey,size_t existingkey_size,const char * newkey,size_t newkey_size )
{
	struct crypt_device * cd ;

	if( zuluCryptVolumeIsNotLuks( device ) ){
		return 3 ;
	}
	if( crypt_init( &cd,device ) != 0 ){
		return 2 ;
	}
	if( crypt_load( cd,NULL,NULL ) != 0 ){
		return zuluExit( 2,cd ) ;
	}
	if( crypt_keyslot_add_by_passphrase( cd,CRYPT_ANY_SLOT,existingkey,existingkey_size,newkey,newkey_size ) < 0 ){
		return zuluExit( 1,cd ) ;
	}else{
		return zuluExit( 0,cd ) ;
	}
}

int zuluCryptAddKey( const char * device,const char * existingkey,size_t existingkey_size,const char * newkey,size_t newkey_size )
{
	string_t st ;
	int fd ;
	int r ;
	if( StringPrefixEqual( device,"/dev/" ) ){
		return _add_key( device,existingkey,existingkey_size,newkey,newkey_size ) ;
	}else{
		/*
		 * zuluCryptAttachLoopDeviceToFile() is defined in ./create_loop.c
		 */
		if( zuluCryptAttachLoopDeviceToFile( device,O_RDWR,&fd,&st ) ){
			r = _add_key( StringContent( st ),existingkey,existingkey_size,newkey,newkey_size ) ;
			StringDelete( &st ) ;
			close( fd ) ;
			return r ;
		}else{
			return 1 ;
		}
	}
}
