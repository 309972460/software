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

static u_int64_t _offset( const char * offset )
{
	char * e ;
	char f   ;

	u_int64_t r = 0 ;

	size_t s = StringSize( offset ) ;

	if( s == 0 ){
		return 0 ;
	}else{
		f = *( offset + s - 1 ) ;
	}
	if( !( f >= '0' && f <= '9' ) ){
		/*
		 * The argument ends with a non digit number,assume the argument is not in offsets but
		 * in bytes and convert it to offsets
		 */
		e = StringCopy_2( offset ) ;
		*( e + s - 1 ) = '\0' ;
		r = StringConvertToInt( e ) ;
		if( f == 'b' || f == 'B' ) {
			if( r < 512 ){
				r = 0 ;
			}else{
				r = r / 512 ;
			}
		}else if( f == 'k' || f == 'K' ){
			r = 2 * r ;
		}else if( f == 'm' || f == 'M' ){
			r = 2 * 1024 * r ;
		}else if( f == 'g' || f == 'G' ){
			r = 2 * 1024 * 1024 * r ;
		}else if( f == 't' || f == 'T' ){
			r = 1.0 * 2 * 1024 * 1024 * 1024 * r ;
		}else{
			r = 0 ;
		}
		StringFree( e ) ;
	}else{
		r = StringConvertToInt( offset ) ;
	}

	return r ;
}

static int _open_plain( const char * device,const open_struct_t * opt )
{
	uint32_t flags ;

	struct crypt_device * cd ;
	struct crypt_params_plain params ;

	memset( &params,'\0',sizeof( struct crypt_params_plain ) ) ;

	params.hash = "ripemd160" ;

	if( zuluCryptPathIsNotValid( device ) ){
		return 3 ;
	}
	if( crypt_init( &cd,device ) != 0 ){
		return 2 ;
	}

	params.offset = _offset( opt->offset ) ;

	if( StringHasComponent( opt->m_opts,"ro" ) ){
		flags = CRYPT_ACTIVATE_READONLY ;
	}else{
		flags = CRYPT_ACTIVATE_ALLOW_DISCARDS ;
	}
	if( crypt_format( cd,CRYPT_PLAIN,"aes","cbc-essiv:sha256",NULL,NULL,32,&params ) != 0 ){
		return zuluExit( 2,cd ) ;
	}
	if( crypt_activate_by_passphrase( cd,opt->mapper_name,CRYPT_ANY_SLOT,opt->key,opt->key_len,flags ) < 0 ){
		return zuluExit( 2,cd ) ;
	}else{
		return zuluExit( 0,cd ) ;
	}
}

int zuluCryptOpenPlain_1( const open_struct_t * opt )
{
	int mode ;
	string_t st ;
	int fd ;
	int r ;

	if( StringPrefixEqual( opt->device,"/dev/" ) ){
		return _open_plain( opt->device,opt ) ;
	}else{
		if( StringHasComponent( opt->m_opts,"ro" ) ){
			mode = O_RDONLY ;
		}else{
			mode = O_RDWR ;
		}
		/*
		 * zuluCryptAttachLoopDeviceToFile() is defined in ./create_loop.c
		 */
		if( zuluCryptAttachLoopDeviceToFile( opt->device,mode,&fd,&st ) ){
			r = _open_plain( StringContent( st ),opt ) ;
			StringDelete( &st ) ;
			close( fd ) ;
			return r ;
		}else{
			return 2 ;
		}
	}
}

int zuluCryptOpenPlain( const char * device,const char * mapper,const char * mode,const char * key,size_t key_len )
{
	open_struct_t opt ;

	memset( &opt,'\0',sizeof( open_struct_t ) ) ;

	opt.device      = device ;
	opt.mapper_name = mapper ;
	opt.key         = key ;
	opt.key_len     = key_len ;
	opt.m_opts      = mode ;

	return zuluCryptOpenPlain_1( &opt ) ;
}
