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

static inline char * zuluExit( char * c,struct crypt_device * cd )
{
	if( cd != NULL ){
		crypt_free( cd );
	}
	return c ;
}

static char * _empty_slots( const char * device )
{
	struct crypt_device * cd;

	int j ;
	int k ;
	const char * type ;

	string_t p ;

	if( crypt_init( &cd,device ) != 0 ){
		return zuluExit( NULL,NULL ) ;
	}
	if( crypt_load( cd,NULL,NULL ) != 0 ){
		return zuluExit( NULL,cd ) ;
	}

	type = crypt_get_type( cd ) ;

	if( type == NULL ){
		return zuluExit( NULL,cd ) ;
	}

	k = crypt_keyslot_max( type ) ;

	if( k < 0 ){
		return zuluExit( NULL,cd ) ;
	}

	p = StringEmpty() ;

	for( j = 0 ; j < k ; j++ ){
		switch( crypt_keyslot_status( cd,j ) ){
			case CRYPT_SLOT_INACTIVE   : StringAppend( p,"0" ) ; break ;
			case CRYPT_SLOT_ACTIVE     : StringAppend( p,"1" ) ; break ;
			case CRYPT_SLOT_INVALID    : StringAppend( p,"2" ) ; break ;
			case CRYPT_SLOT_ACTIVE_LAST: StringAppend( p,"3" ) ; break ;
			default : ;
		}
	}

	return zuluExit( StringDeleteHandle( &p ),cd ) ;
}

char * zuluCryptEmptySlots( const char * device )
{
	string_t st ;
	int fd ;
	char * r ;
	if( StringPrefixEqual( device,"/dev/" ) ){
		return _empty_slots( device ) ;
	}else{
		/*
		 * zuluCryptAttachLoopDeviceToFile() is defined in ./create_loop.c
		 */
		if( zuluCryptAttachLoopDeviceToFile( device,O_RDONLY,&fd,&st ) ){
			r = _empty_slots( StringContent( st ) ) ;
			StringDelete( &st ) ;
			close( fd ) ;
			return r ;
		}else{
			return NULL ;
		}
	}
}
