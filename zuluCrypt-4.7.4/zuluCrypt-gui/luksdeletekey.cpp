/*
 *
 *  Copyright ( c ) 2011
 *  name : Francis Banyikwa
 *  email: mhogomchungu@gmail.com
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  ( at your option ) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "luksdeletekey.h"
#include "zulucrypt.h"
#include "../zuluCrypt-cli/constants.h"
#include "utility.h"

#include <QObject>
#include <Qt>
#include <QObject>
#include <QFileDialog>
#include <QRadioButton>
#include <QPushButton>
#include <QMessageBox>
#include <QCloseEvent>
#include <QIcon>
#include <QKeyEvent>

#include "ui_openvolume.h"
#include "openvolume.h"
#include "ui_luksdeletekey.h"
#include "task.h"
#include "dialogmsg.h"

luksdeletekey::luksdeletekey( QWidget * parent ) :
	QDialog( parent ),
	m_ui( new Ui::luksdeletekey )
{
	m_isWindowClosable = true ;
	m_ui->setupUi( this ) ;

	this->setFixedSize( this->size() ) ;
	this->setFont( parent->font() ) ;

	m_ui->lineEditVolumePath->clear() ;

	connect( m_ui->pushButtonDelete,SIGNAL( clicked() ),this,SLOT( pbDelete() ) ) ;
	connect( m_ui->pushButtonCancel,SIGNAL( clicked() ),this,SLOT( pbCancel() ) ) ;
	connect( m_ui->rbPassphrase,SIGNAL( toggled( bool ) ),this,SLOT( rbPassphrase() ) ) ;
	connect( m_ui->rbPassphraseFromFile,SIGNAL( toggled( bool ) ),this,SLOT( rbPassphraseFromFile() ) ) ;
	connect( m_ui->pushButtonOpenKeyFile,SIGNAL( clicked() ),this,SLOT( pbOpenKeyFile() ) ) ;
	connect( m_ui->pushButtonOpenVolume,SIGNAL( clicked() ),this,SLOT( pbOpenVolume() ) ) ;
	connect( m_ui->pushButtonOpenPartition,SIGNAL( clicked() ),this,SLOT( pbOpenPartition() ) ) ;

	this->installEventFilter( this ) ;
}

bool luksdeletekey::eventFilter( QObject * watched,QEvent * event )
{
	if( utility::eventFilter( this,watched,event ) ){
		this->HideUI() ;
		return true ;
	}else{
		return false ;
	}
}

void luksdeletekey::closeEvent( QCloseEvent * e )
{
	e->ignore() ;
	if(  m_isWindowClosable ){
		pbCancel() ;
	}
}

void luksdeletekey::rbPassphrase()
{
	m_ui->lineEditPassphrase->setToolTip( tr( "enter a key" ) ) ;
	m_ui->labelPassphrase->setText( tr( "key" ) ) ;
	m_ui->lineEditPassphrase->setEchoMode( QLineEdit::Password ) ;
	m_ui->lineEditPassphrase->clear() ;
	m_ui->pushButtonOpenKeyFile->setEnabled( false ) ;
	m_ui->pushButtonOpenKeyFile->setIcon( QIcon( QString( ":/passphrase.png" ) ) ) ;
	m_ui->lineEditPassphrase->setFocus() ;
}

void luksdeletekey::rbPassphraseFromFile()
{
	m_ui->lineEditPassphrase->setToolTip( tr( "enter a path to a keyfile location" ) ) ;
	m_ui->labelPassphrase->setText( tr( "keyfile path" ) ) ;
	m_ui->lineEditPassphrase->setEchoMode( QLineEdit::Normal ) ;
	m_ui->lineEditPassphrase->clear() ;
	m_ui->pushButtonOpenKeyFile->setEnabled( true ) ;
	m_ui->pushButtonOpenKeyFile->setIcon( QIcon( QString( ":/keyfile.png" ) ) ) ;
}

void luksdeletekey::pbOpenKeyFile()
{
	QString Z = QFileDialog::getOpenFileName( this,tr( "key file with a passphrase to delete" ),QDir::homePath(),0 ) ;
	m_ui->lineEditPassphrase->setText( Z ) ;
	if( m_ui->lineEditVolumePath->text().isEmpty() ){
		m_ui->lineEditVolumePath->setFocus() ;
	}else{
		m_ui->pushButtonDelete->setFocus() ;
	}
}

void luksdeletekey::ShowUI()
{
	this->enableAll() ;
	m_ui->rbPassphrase->setEnabled( true ) ;
	m_ui->labelPassphrase->setText( tr( "key" ) ) ;
	m_ui->rbPassphrase->setChecked( true ) ;

	if( m_ui->lineEditVolumePath->text().isEmpty() ){
		m_ui->lineEditVolumePath->setFocus() ;
	}else{
		m_ui->lineEditPassphrase->setFocus() ;
	}
	m_ui->pushButtonOpenPartition->setIcon( QIcon( QString( ":/partition.png" ) ) ) ;
	m_ui->pushButtonOpenVolume->setIcon( QIcon( QString( ":/file.png" ) ) ) ;
	this->show() ;
}

void luksdeletekey::ShowUI( const QString& path )
{
	m_ui->lineEditVolumePath->setText( path ) ;
	this->ShowUI() ;
}

void luksdeletekey::disableAll()
{
	m_ui->groupBox->setEnabled( false ) ;
	m_ui->label->setEnabled( false ) ;
	m_ui->labelPassphrase->setEnabled( false ) ;
	m_ui->lineEditPassphrase->setEnabled( false ) ;
	m_ui->lineEditVolumePath->setEnabled( false ) ;
	m_ui->pushButtonCancel->setEnabled( false ) ;
	m_ui->pushButtonDelete->setEnabled( false ) ;
	m_ui->pushButtonOpenKeyFile->setEnabled( false ) ;
	m_ui->pushButtonOpenPartition->setEnabled( false ) ;
	m_ui->pushButtonOpenVolume->setEnabled( false ) ;
	m_ui->rbPassphrase->setEnabled( false ) ;
	m_ui->rbPassphraseFromFile->setEnabled( false ) ;
}

void luksdeletekey::enableAll()
{
	m_ui->groupBox->setEnabled( true ) ;
	m_ui->label->setEnabled( true ) ;
	m_ui->labelPassphrase->setEnabled( true ) ;
	m_ui->lineEditPassphrase->setEnabled( true ) ;
	m_ui->lineEditVolumePath->setEnabled( true ) ;
	m_ui->pushButtonCancel->setEnabled( true ) ;
	m_ui->pushButtonDelete->setEnabled( true ) ;
	m_ui->pushButtonOpenKeyFile->setEnabled( true ) ;
	m_ui->pushButtonOpenPartition->setEnabled( true ) ;
	m_ui->pushButtonOpenVolume->setEnabled( true ) ;
	m_ui->rbPassphrase->setEnabled( true ) ;
	m_ui->rbPassphraseFromFile->setEnabled( true ) ;
}

void luksdeletekey::pbCancel()
{
	this->HideUI() ;
}

void luksdeletekey::pbOpenPartition()
{
	auto op = new openvolume( this ) ;
	op->showLuksOnly() ;
	connect( op,SIGNAL( clickedPartition( QString ) ),this,SLOT( ShowUI( QString ) ) ) ;
	connect( op,SIGNAL( HideUISignal() ),op,SLOT( deleteLater() ) ) ;
	op->ShowAllPartitions() ;
}

void luksdeletekey::pbDelete()
{
	DialogMsg msg( this ) ;

	m_volumePath = utility::resolvePath( m_ui->lineEditVolumePath->text() ) ;

	if( m_volumePath.isEmpty() ){
		msg.ShowUIOK( tr( "ERROR!" ),tr( "atleast one required field is empty" ) ) ;
	}else{
		this->disableAll() ;

		m_volumePath.replace( "\"","\"\"\"" ) ;

		auto l = Task::await<QStringList>( utility::luksEmptySlots( m_volumePath ) ) ;

		this->deleteKey( l ) ;
	}
}

void luksdeletekey::deleteKey( const QStringList& l )
{
	DialogMsg msg( this ) ;

	if( l.isEmpty() ){
		msg.ShowUIOK( tr( "ERROR!" ),tr( "volume is not a luks volume" ) ) ;
		return this->enableAll() ;
	}else if( l.first().toInt() == 1 ){
		QString s = tr( "There is only one last key in the volume." ) ;
		s = s + tr( "\nDeleting it will make the volume unopenable and lost forever." ) ;
		s = s + tr( "\nAre you sure you want to delete this key?" ) ;

		if( msg.ShowUIYesNoDefaultNo( tr( "WARNING" ),s ) == QMessageBox::No ){
			return this->enableAll() ;
		}
	}else{
		QString s = tr( "are you sure you want to delete a key from this volume?" ) ;
		if( msg.ShowUIYesNoDefaultNo( tr( "WARNING" ),s ) == QMessageBox::No ){
			return this->enableAll() ;
		}
	}

	m_keyNumber = l.first().toInt() - 1 ;
	m_totalKeys = l.at( 1 ) ;

	QString keypath ;

	if( m_ui->rbPassphraseFromFile->isChecked() ){
		keypath = utility::resolvePath( m_ui->lineEditPassphrase->text() ).replace( "\"","\"\"\"" ) ;
	}else{
		keypath = utility::keyPath() ;
		QString key = m_ui->lineEditPassphrase->text() ;

		utility::keySend( keypath,key ) ;
	}

	const char * r = "%1 -k -r -d \"%2\" -f \"%3\"" ;
	QString exe = QString( r ).arg( ZULUCRYPTzuluCrypt,m_volumePath,keypath ) ;

	m_isWindowClosable = false ;

	this->taskFinished( Task::await<int>( utility::exec( exe ) ) ) ;
}

void luksdeletekey::taskFinished( int r )
{
	DialogMsg msg( this ) ;
	m_isWindowClosable = true ;
	QString success;
	switch(  r ){
		case 0 :
			success = tr( "key removed successfully.\n%1 / %2 slots are now in use" ).arg( QString::number( m_keyNumber ) ).arg( m_totalKeys ) ;
			msg.ShowUIOK( tr( "SUCCESS!" ),success ) ;
			return this->HideUI() ;
		case 2 : msg.ShowUIOK( tr( "ERROR!" ),tr( "there is no key in the volume that match the presented key" ) ) ;				break ;
		case 3 : msg.ShowUIOK( tr( "ERROR!" ),tr( "could not open the volume" ) ) ;								break ;
		case 4 : msg.ShowUIOK( tr( "ERROR!" ),tr( "insufficient privilege to open a system device,\
only root user or members of group zulucrypt-system can do that" ) ) ;											break ;
		case 5 : msg.ShowUIOK( tr( "ERROR!" ),tr( "could not open the volume in write mode" ) ) ;						break ;
		case 6 : msg.ShowUIOK( tr( "ERROR!" ),tr( "insufficient memory to hold your response" ) ) ;						break ;
		case 7 : msg.ShowUIOK( tr( "ERROR!" ),tr( "operation terminated per user request" ) ) ;							break ;
		case 8 : msg.ShowUIOK( tr( "ERROR!" ),tr( "can not get passphrase in silent mode" ) ) ;							break ;
		case 9 : msg.ShowUIOK( tr( "ERROR!" ),tr( "insufficient memory to hold passphrase" ) ) ;						break ;
		case 10: msg.ShowUIOK( tr( "ERROR!" ),tr( "one or more required argument(s) for this operation is missing" ) ) ;			break ;
		case 11: msg.ShowUIOK( tr( "ERROR!" ),tr( "keyfile does not exist" ) ) ;								break;
		case 12: msg.ShowUIOK( tr( "ERROR!" ),tr( "could not get enough memory to open the key file") ) ;					break ;
		case 13: msg.ShowUIOK( tr( "ERROR!" ),tr( "insufficient privilege to open key file for reading" ) ) ;					break ;
		case 14: msg.ShowUIOK( tr( "ERROR!" ),tr( "could not get a key from a socket" ) ) ;							break ;
		case 110:msg.ShowUIOK( tr( "ERROR!" ),tr( "can not find a partition that match presented UUID" ) ) ;					break ;
		default :msg.ShowUIOK( tr( "ERROR!" ),tr( "unrecognized ERROR! with status number %1 encountered" ).arg( r ) ) ;
	}

	this->enableAll() ;

	if( r == 2 ){
		m_ui->lineEditPassphrase->clear() ;
		m_ui->lineEditPassphrase->setFocus() ;
	}
}

void luksdeletekey::pbOpenVolume()
{
	QString Z = QFileDialog::getOpenFileName( this,tr( "volume path" ),QDir::homePath(),0 ) ;
	m_ui->lineEditVolumePath->setText( Z ) ;
	if( m_ui->lineEditPassphrase->text().isEmpty() ){
		m_ui->lineEditPassphrase->setFocus() ;
	}else{
		m_ui->pushButtonDelete->setFocus() ;
	}
}

void luksdeletekey::HideUI()
{
	this->hide() ;
	emit HideUISignal() ;
}

luksdeletekey::~luksdeletekey()
{
	delete m_ui ;
}
