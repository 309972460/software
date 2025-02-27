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
#include "../lib/includes.h"
#include <libintl.h>
#include <locale.h>
#include <stdio.h>
#include <sys/stat.h>

static int zuluExit( int st,stringList_t stl )
{
	/*
	 * this function is defined in ../string/StringList.c
	 */
	StringListClearDelete( &stl ) ;

	switch ( st ){
		case 0 : printf( gettext( "SUCCESS: volume created successfully\n" ) ) ;						break  ;
		case 1 : printf( gettext( "ERROR: presented file system is not supported,see documentation for more information\n" ) ) ;break ;
		case 2 : printf( gettext( "ERROR: insufficient privilege to open a system device in read/write mode,\n\
only root user or members of group zulucrypt-system can do that\n" ) ) ;						break  ;
		case 3 : printf( gettext( "ERROR: could not create an encrypted volume\n" ) ) ;				break  ;
		case 4 : printf( gettext( "ERROR: could not open volume for writing\n" ) ) ;				break  ;
		case 5 : printf( gettext( "ERROR: there seem to be an opened mapper associated with the device\n" ) );	break  ;
		case 6 : printf( gettext( "ERROR: can not create a volume on a mounted device\n" ) ) ;			break  ;
		case 7 : printf( gettext( "ERROR: container file must be bigger than 3MB\n" ) ) ;			break  ;
		case 8 : printf( "ERROR: %s not found \n",ZULUCRYPTmkfs ) ;						break  ;
		case 9 : printf( gettext( "ERROR: insufficient memory to hold your response\n" ) ) ;			break  ;
		case 10: printf( gettext( "INFO: operation terminated per user request\n" ) ) ;				break  ;
		case 11: printf( gettext( "ERROR: could not get passphrase in silent mode\n" ) ) ;			break  ;
		case 12: printf( gettext( "ERROR: insufficient memory to hold the passphrase\n" ) ) ;			break  ;
		case 13: printf( gettext( "ERROR: passphrases do not match\n" ) ) ;					break  ;
		case 14: printf( gettext( "ERROR: invalid path to key file\n" ) ) ;					break  ;
		case 15: printf( gettext( "ERROR: could not get a key from a key file\n" ) ) ;				break  ;
		case 16: printf( gettext( "ERROR: couldnt get enought memory to hold the key file\n" ) ) ;		break  ;
		case 17: printf( gettext( "ERROR: could not get a key from a socket\n" ) ) ;				break  ;
		case 18: printf( gettext( "ERROR: one or more required argument(s) for this operation is missing\n" ));	break  ;
		case 19: printf( gettext( "ERROR: can not get passphrase in silent mode\n" ) ) ;			break  ;
		case 20: printf( gettext( "ERROR: insufficient memory to hold passphrase\n" ) ) ;			break  ;
		case 21: printf( gettext( "ERROR: passphrases do not match\n" ) ) ;					break  ;
		case 22: printf( gettext( "ERROR: failed to create a volume\n" ) ) ;					break  ;
		case 23: printf( gettext( "ERROR: wrong argument detected for tcrypt volume" ) ) ;			break  ;
		default: printf( gettext( "ERROR: unrecognized error with status number %d encountered\n" ),st ) ;
	}
	return st ;
}

static int zuluExit_1( const char * type,stringList_t stl )
{
	StringListClearDelete( &stl ) ;

	printf( gettext( "SUCCESS: volume created successfully\n" ) ) ;

	if( StringAtLeastOneMatch_1( type,"luks","tcrypt","truecrypt",NULL ) ){
		printf( gettext( "\ncreating a backup of the \"%s\" volume header is strongly adviced.\n" ),type ) ;
		printf( gettext( "Please read documentation on why this is important\n\n" ) ) ;
	}
	return 0 ;
}

int zuluCryptEXECreateVolume( const struct_opts * opts,const char * mapping_name,uid_t uid )
{
	int ask_confirmation = opts->ask_confirmation ;
	const char * device  = opts->device ;
	const char * fs      = opts->fs ;
	const char * type    = opts->type ;
	const char * key_source = opts->key_source ;
	const char * pass    = opts->key ;
	const char * rng     = opts->rng ;

	/*
	 * Below is a form of memory management.All strings are collected in a stringlist object to easily delete them
	 * when the function returns.This allows for the function to have multiple exit points without risks of leaking
	 * memory from manually examining each exit point to make sure all strings are deleted or go with multiple goto
	 * code deleting blocks to take into account different exit points.
	 */
	stringList_t stl  ;
	string_t * stringArray  = StringListArray( &stl,6 ) ;
	string_t * pass_1  = &stringArray[ 0 ] ;
	string_t * pass_2  = &stringArray[ 1 ] ;
	string_t * confirm = &stringArray[ 2 ] ;
	string_t * mapper  = &stringArray[ 3 ] ;
	string_t * pass_3  = &stringArray[ 4 ] ;
	string_t * pass_4  = &stringArray[ 5 ] ;

	string_t p = StringVoid ;
	string_t q = StringVoid ;

	const char * volkey ;
	size_t       volkeysize ;
	const char * volkey_h = NULL ;
	size_t       volkeysize_h = 0 ;

	int tcrypt_source   = TCRYPT_PASSPHRASE ;
	int tcrypt_source_h = TCRYPT_PASSPHRASE ;

	int st  ;

	int j ;
	int k ;

	int truecrypt_volume = 0 ;
	u_int64_t hidden_volume_size = 0 ;

	u_int64_t size ;

	create_tcrypt_t tcrypt ;

	const char * keyFile ;
	const char * keyFile_1 ;

	const char * tcrypt_hidden_volume_size     = opts->tcrypt_hidden_volume_size ;
	const char * tcrypt_hidden_volume_key_file = opts->tcrypt_hidden_volume_key_file ;
	const char * tcrypt_hidden_volume_key      = opts->tcrypt_hidden_volume_key ;

	/*
	 * zulucryptFileSystemIsNotSupported() is defined in ../lib/mount_fs_options.c
	 */
	if( !zulucryptFileSystemIsSupported( fs ) ){
		return zuluExit( 1,stl ) ;
	}
	/*
	 * zuluCryptPartitionIsSystemPartition() is defined in ./volumess.c
	 */
	if( zuluCryptPartitionIsSystemPartition( device,uid ) ){
		if( !zuluCryptUserIsAMemberOfAGroup( uid,"zulucrypt" ) ){
			return zuluExit( 2,stl ) ;
		}
	}

	/*
	 * zuluCryptSecurityDeviceIsWritable() is defined in path_access.c
	 */
	st = zuluCryptCanOpenPathForWriting( device,uid ) ;
	/*
	 * 1-permissions denied
	 * 2-invalid path
	 * 3-shenanigans
	 * 4-common error
	 */
	switch( st ){
		case 0 : break ;
		case 1 : return zuluExit( 4,stl ) ;
		case 2 : return zuluExit( 4,stl ) ;
		case 3 : return zuluExit( 4,stl ) ;
		case 4 : return zuluExit( 4,stl ) ;
		default: return zuluExit( 4,stl ) ;
	}

	/*
	 * ZULUCRYPTlongMapperPath is set in ../constants.h
	 * zuluCryptCreateMapperName() is defined at ../lib/create_mapper_name.c
	 */
	*mapper = zuluCryptCreateMapperName( device,mapping_name,uid,ZULUCRYPTlongMapperPath ) ;

	j = zuluCryptCheckOpenedMapper( StringContent( *mapper ) ) ;

	/*
	 * defined in ../lib/process_mountinfo.c
	 */
	k = zuluCryptPartitionIsMounted( device ) ;

	if( j == 1 ){
		return zuluExit( 5,stl ) ;
	}
	if( k == 1 ){
		return zuluExit( 6,stl ) ;
	}

	if( StringPrefixEqual( device,"/dev/loop" ) ){
		zuluCryptSecurityGainElevatedPrivileges() ;
		/*
		 * zuluCryptGetVolumeSize() is defined in volumes.c
		 */
		size = zuluCryptGetVolumeSize( device ) ;
		zuluCryptSecurityDropElevatedPrivileges() ;
		if( size < 3145728 ){
			return zuluExit( 7,stl ) ;
		}
	}

	/*
	 * ZULUCRYPTmkfs is defined at "../constants.h"
	 * File systems are created not through file systems APIs but through mkfs.xxx executables started using exec call.
	 */
	if( zuluCryptPathIsNotValid( ZULUCRYPTmkfs ) ){
		return zuluExit( 8,stl ) ;
	}
	if( ask_confirmation ){
		printf( gettext( "\nThis operation will destroy all data in a device at: \"%s\"\n" ),device ) ;
		printf( gettext( "Are you sure you want to proceed?\n" ) ) ;
		printf( gettext( "Type \"YES\" and press enter if you want to process: " ) ) ;

		*confirm = StringGetFromTerminal_1( 3 ) ;
		if( *confirm == StringVoid ){
			return zuluExit( 9,stl ) ;
		}else{
			if( !StringEqual( *confirm,gettext( "YES" ) ) ){
				return zuluExit( 10,stl ) ;
			}
		}
	}

	truecrypt_volume = StringAtLeastOneMatch_1( type,"tcrypt","truecrypt",NULL ) ;

	if( key_source == NULL ){
		printf( gettext( "Enter passphrase: " ) ) ;
		/*
		 * ZULUCRYPT_KEY_MAX_SIZE is set in ../constants.h
		 */
		switch( StringSilentlyGetFromTerminal_1( pass_1,ZULUCRYPT_KEY_MAX_SIZE ) ){
			case 1 : return zuluExit( 11,stl ) ;
			case 2 : return zuluExit( 12,stl ) ;
		}

		printf( gettext( "\nRe enter passphrase: " ) ) ;
		switch( StringSilentlyGetFromTerminal_1( pass_2,ZULUCRYPT_KEY_MAX_SIZE ) ){
			case 1 : return zuluExit( 11,stl ) ;
			case 2 : return zuluExit( 12,stl ) ;
		}

		printf( "\n" ) ;

		if( !StringEqualString( *pass_1,*pass_2 ) ){
			return zuluExit( 13,stl ) ;
		}

		tcrypt_source = TCRYPT_PASSPHRASE ;
	}else{
		if( StringsAreEqual( key_source,"-p" ) ){
			*pass_1 = String( pass ) ;
			tcrypt_source = TCRYPT_PASSPHRASE ;
		}else if( StringsAreEqual( key_source,"-f" ) ){
			/*
			 * function is defined at "path_access.c"
			 */
			switch( zuluCryptGetPassFromFile( pass,uid,pass_1 ) ){
				case 1 : return zuluExit( 14,stl ) ;
				case 4 : return zuluExit( 15,stl ) ;
				case 2 : return zuluExit( 16,stl ) ;
				case 5 : return zuluExit( 17,stl ) ;
			}

			if( StringHasComponent( pass,"/.zuluCrypt-socket" ) ){
				tcrypt_source = TCRYPT_PASSPHRASE ;
			}else{
				tcrypt_source = TCRYPT_KEYFILE ;
			}
		}else{
			return zuluExit( 18,stl ) ;
		}
	}

	if( tcrypt_hidden_volume_size != NULL ){

		if( !truecrypt_volume ){
			return zuluExit( 23,stl ) ;
		}else{
			hidden_volume_size = StringConvertToInt( tcrypt_hidden_volume_size ) ;

			if( tcrypt_hidden_volume_key_file != NULL ){
				/*
				 * function is defined in "path_access.c"
				 */
				switch( zuluCryptGetPassFromFile( tcrypt_hidden_volume_key_file,uid,pass_3 ) ){
					case 1 : return zuluExit( 14,stl ) ;
					case 4 : return zuluExit( 15,stl ) ;
					case 2 : return zuluExit( 16,stl ) ;
					case 5 : return zuluExit( 17,stl ) ;
				}
				if( StringHasComponent( tcrypt_hidden_volume_key_file,"/.zuluCrypt-socket" ) ){
					tcrypt_source_h = TCRYPT_PASSPHRASE ;
				}else{
					tcrypt_source_h = TCRYPT_KEYFILE ;
				}
			}else if( tcrypt_hidden_volume_key == NULL ){
				printf( gettext( "Enter tcrypt hidden passphrase: " ) ) ;
				/*
				 * ZULUCRYPT_KEY_MAX_SIZE is set in ../constants.h
				 */
				switch( StringSilentlyGetFromTerminal_1( pass_3,ZULUCRYPT_KEY_MAX_SIZE ) ){
					case 1 : return zuluExit( 19,stl ) ;
					case 2 : return zuluExit( 20,stl ) ;
				}

				printf( gettext( "\nRe enter tcrypt hidden passphrase: " ) ) ;
				switch( StringSilentlyGetFromTerminal_1( pass_4,ZULUCRYPT_KEY_MAX_SIZE ) ){
					case 1 : return zuluExit( 19,stl ) ;
					case 2 : return zuluExit( 20,stl ) ;
				}

				printf( "\n" ) ;

				if( !StringEqualString( *pass_3,*pass_4 ) ){
					return zuluExit( 21,stl ) ;
				}

				tcrypt_source_h = TCRYPT_PASSPHRASE ;
			}else if( tcrypt_hidden_volume_key != NULL ){
				*pass_3 = String( tcrypt_hidden_volume_key ) ;
				tcrypt_source_h = TCRYPT_PASSPHRASE ;
			}else{
				return zuluExit( 18,stl ) ;
			}

			volkey_h = StringContent( *pass_3 ) ;
			volkeysize_h = StringLength( *pass_3 ) ;
		}
	}

	volkey     = StringContent( *pass_1 ) ;
	volkeysize = StringLength( *pass_1 ) ;

	zuluCryptSecurityGainElevatedPrivileges() ;

	if( truecrypt_volume ){

		memset( &tcrypt,'\0',sizeof( create_tcrypt_t ) ) ;

		tcrypt.device             = device ;
		tcrypt.fs                 = fs ;
		tcrypt.fs_h               = fs ;
		tcrypt.encryption_options = rng ;
		tcrypt.hidden_volume_size = hidden_volume_size ;

		if( tcrypt_source == TCRYPT_KEYFILE ){
			/*
			 * zuluCryptCreateKeyFile() is defined in ../lib/open_tcrypt.c
			 */
			p = zuluCryptCreateKeyFile( volkey,volkeysize,"create-key-" ) ;

			keyFile = StringContent( p ) ;
			tcrypt.keyfiles = &keyFile ;

			tcrypt.keyfiles_number = 1 ;

			tcrypt.passphrase       = "" ;
			tcrypt.passphrase_size  = 0 ;
		}else{
			tcrypt.passphrase       = volkey ;
			tcrypt.passphrase_size  = volkeysize ;
		}

		if( tcrypt.hidden_volume_size > 0 ){

			if( tcrypt_source_h == TCRYPT_KEYFILE ){

				q = zuluCryptCreateKeyFile( volkey_h,volkeysize_h,"create-key_h-" ) ;
				keyFile_1 = StringContent( q ) ;

				tcrypt.keyfiles_h = &keyFile_1 ;
				tcrypt.keyfiles_h_number = 1 ;

				tcrypt.passphrase_h      = "" ;
				tcrypt.passphrase_h_size = 0 ;
			}else{
				tcrypt.passphrase_h      = volkey_h ;
				tcrypt.passphrase_h_size = volkeysize_h ;
			}
		}

		/*
		 * zuluCryptCreateTCryptVolume() is defined in ../lib/create_tcrypt.c
		 */
		st = zuluCryptCreateTCryptVolume( &tcrypt ) ;

		/*
		 * zuluCryptDeleteFile_1() is defined in ../lib/file_path_security.c
		 */
		zuluCryptDeleteFile_1( p ) ;
		zuluCryptDeleteFile_1( q ) ;

		StringDelete( &p ) ;
		StringDelete( &q ) ;
	}else{
		/*
		 * zuluCryptCreateVolume() is defined in ../lib/create_volume.c
		 */
		st = zuluCryptCreateVolume( device,fs,type,volkey,volkeysize,rng ) ;
	}

	zuluCryptSecurityDropElevatedPrivileges() ;

	if( st == 0 ){
		return zuluExit_1( type,stl ) ;
	}else{
		return zuluExit( 22,stl ) ;
	}
}
