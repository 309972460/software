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
#include <sys/syscall.h>
#include <libcryptsetup.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mount.h>
#include <sys/types.h>
#include <sys/stat.h>

/*
 * this header is created at config time
 */
#include "truecrypt_support.h"

/*
 * TRIM support for truecrypt volume is off due to recommendation from the following wiki page:
 * https://wiki.archlinux.org/index.php/dm-crypt_with_LUKS#Discard.2FTRIM_support_for_solid_state_disks_.28SSD.29
 */

string_t zuluCryptCreateKeyFile( const char * key,size_t key_len,const char * fileName )
{
	string_t st = StringVoid ;
	int fd ;
	const char * file ;

	struct stat statstr ;

	if( key == NULL || key_len == 0 || fileName == NULL ){
		return StringVoid ;
	}

	#define path_does_not_exist( x ) stat( x,&statstr ) != 0

	if( path_does_not_exist( "/run" ) ){
		mkdir( "/run",S_IRWXU | S_IRGRP | S_IXGRP | S_IXOTH | S_IROTH ) ;
		chown( "/run",0,0 ) ;
	}
	if( path_does_not_exist( "/run/zuluCrypt" ) ){
		mkdir( "/run/zuluCrypt",S_IRWXU ) ;
		chown( "/run/zuluCrypt",0,0 ) ;
	}

	st = String_1( "/run/zuluCrypt/",fileName,NULL ) ;
	file = StringAppendInt( st,syscall( SYS_gettid ) ) ;
	fd = open( file,O_WRONLY | O_CREAT,S_IRUSR | S_IWUSR | S_IRGRP |S_IROTH ) ;

	if( fd == -1 ){
		StringDelete( &st ) ;
	}else{
		write( fd,key,key_len ) ;
		close( fd ) ;
		chown( file,0,0 ) ;
		chmod( file,S_IRWXU ) ;
	}

	return st ;
}

string_t zuluCryptCreateKeyFile_1( string_t st,const char * fileName )
{
	return zuluCryptCreateKeyFile( StringContent( st ),StringLength( st ),fileName ) ;
}

#if TRUECRYPT_CRYPTSETUP

static inline int zuluExit( int st,struct crypt_device * cd )
{
	crypt_free( cd ) ;
	return st ;
}

/*
 * 1 is returned if a volume is a truecrypt volume.
 * 0 is returned if a volume is not a truecrypt volume or functionality is not supported
 */
int zuluCryptVolumeIsTcrypt( const char * device,const char * key,int key_source )
{
	struct crypt_device * cd = NULL;
	struct crypt_params_tcrypt params ;

	memset( &params,'\0',sizeof( struct crypt_params_tcrypt ) ) ;

	if( key_source ){;}

	if( crypt_init( &cd,device ) < 0 ){
		return 0 ;
	}else{
		params.passphrase      = key ;
		params.passphrase_size = StringSize( key );
		params.flags           = CRYPT_TCRYPT_LEGACY_MODES ;

		if( crypt_load( cd,CRYPT_TCRYPT,&params ) == 0 ){
			return zuluExit( 1,cd ) ;
		}else{
			return zuluExit( 0,cd ) ;
		}
	}
}

static int _open_tcrypt_volume( const char * device,const open_struct_t * opts )
{
	uint32_t flags ;

	int r ;

	struct crypt_device * cd = NULL;
	struct crypt_params_tcrypt params ;

	if( crypt_init( &cd,device ) < 0 ){
		return 1 ;
	}else{
		memset( &params,'\0',sizeof( struct crypt_params_tcrypt ) ) ;

		params.keyfiles_count   = opts->tcrypt_keyfiles_count ;
		params.keyfiles         = opts->tcrypt_keyfiles ;

		params.passphrase       = opts->key ;
		params.passphrase_size  = opts->key_len ;

		if( params.passphrase_size > 64 ){
			/*
			 * truecrypt passphrase is limited to 64 characters
			 */
			params.passphrase_size = 64 ;
		}
		if( opts->volume_type == TCRYPT_HIDDEN ){
			params.flags = CRYPT_TCRYPT_LEGACY_MODES | CRYPT_TCRYPT_HIDDEN_HEADER ;
		}else{
			params.flags = CRYPT_TCRYPT_LEGACY_MODES ;
		}
		if( crypt_load( cd,CRYPT_TCRYPT,&params ) != 0 ){

			params.flags |= CRYPT_TCRYPT_SYSTEM_HEADER ;

			if( crypt_load( cd,CRYPT_TCRYPT,&params ) != 0 ){
				return zuluExit( 1,cd ) ;
			}
		}
		if( StringHasComponent( opts->m_opts,"ro" ) ){
			flags = CRYPT_ACTIVATE_READONLY ;
		}else{
			flags = 0 ;
		}

		r = crypt_activate_by_volume_key( cd,opts->mapper_name,NULL,0,flags ) ;

		if( r == 0 ){
			return zuluExit( 0,cd ) ;
		}else{
			return zuluExit( 1,cd ) ;
		}
	}
}

static int _open_tcrypt_0( const open_struct_t * opt )
{
	int mode ;
	string_t st ;
	int fd ;
	int r ;

	if( StringPrefixEqual( opt->device,"/dev/" ) ){
		return _open_tcrypt_volume( opt->device,opt ) ;
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
			r = _open_tcrypt_volume( StringContent( st ),opt ) ;
			StringDelete( &st ) ;
			close( fd ) ;
			return r ;
		}else{
			return 1 ;
		}
	}
}

int zuluCryptOpenTcrypt( const char * device,const char * mapper,const char * key,size_t key_len,
			 int key_source,int volume_type,const char * m_point,
			 uid_t uid,unsigned long m_flags,const char * fs_opts )
{
	open_struct_t opts ;
	string_t st ;
	int r ;
	const char * keyfile ;

	memset( &opts,'\0',sizeof( open_struct_t ) ) ;

	opts.device      = device ;
	opts.mapper_name = mapper ;
	opts.volume_type = volume_type ;
	opts.m_point     = m_point ;
	opts.uid         = uid ;
	opts.m_flags     = m_flags ;
	opts.fs_opts     = fs_opts ;

	if( m_flags & MS_RDONLY ){
		opts.m_opts = "ro" ;
	}else{
		opts.m_opts = "rw" ;
	}

	if( key_source == TCRYPT_KEYFILE ){
		st = zuluCryptCreateKeyFile( key,key_len,"open_tcrypt-" ) ;
		if( st != StringVoid ){

			keyfile = StringContent( st ) ;

			opts.tcrypt_keyfiles_count = 1 ;
			opts.tcrypt_keyfiles       = &keyfile ;

			r = zuluCryptOpenTcrypt_1( &opts ) ;
			/*
			 * zuluCryptDeleteFile() is defined in open_path_security.c
			 */
			zuluCryptDeleteFile( keyfile ) ;
			StringDelete( &st ) ;
		}else{
			r = 1 ;
		}
	}else if( key_source == TCRYPT_KEYFILE_FILE ){

		opts.tcrypt_keyfiles_count = 1 ;
		opts.tcrypt_keyfiles       = &key ;

		r = zuluCryptOpenTcrypt_1( &opts ) ;
	}else{
		opts.key_len = key_len ;
		opts.key     = key ;
		r = zuluCryptOpenTcrypt_1( &opts ) ;
	}

	return r ;
}

int zuluCryptOpenTcrypt_1( const open_struct_t * opts )
{
	/*
	 * zuluCryptOpenVolume_0() is defined in open_volume.c
	 */
	return zuluCryptOpenVolume_0( _open_tcrypt_0,opts ) ;
}

#else

/*
 * We wil get here if truecrypt support is not enabled through cryptsetup or tcplay
 */

/*
 * 1 is returned if a volume is a truecrypt volume.
 * 0 is returned if a volume is not a truecrypt volume or functionality is not supported
 */
int zuluCryptVolumeIsTcrypt( const char * device,const char * key,int key_source )
{
	if( 0 && device && key && key_source ){;}
	return 0 ;
}

/*
 * 0 is returned if a volume was successfully opened.
 * 1 is returned if a volume was not successfully opened or functionality is not supported
 */
int zuluCryptOpenTcrypt( const char * device,const char * mapper,const char * key,size_t key_len,
			 int key_source,int volume_type,const char * m_point,
			 uid_t id,unsigned long m_opts,const char * fs_opts )
{
	if( 0 && device && mapper && key && key_source && volume_type
		&& m_point && id && m_opts && fs_opts && key_len ){;}
	return 1 ;
}

int zuluCryptOpenTcrypt_1( const open_struct_t * opts )
{
	if( 0 && opts ){;}
	return 1 ;
}

#endif
