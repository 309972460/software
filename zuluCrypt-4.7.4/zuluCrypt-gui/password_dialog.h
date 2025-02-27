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

#ifndef PASSWORD_DIALOG_H
#define PASSWORD_DIALOG_H

#include <QDialog>
#include <QString>

#include "utility.h"

namespace Ui {
class PasswordDialog ;
}

class QWidget ;
class QAction ;
class QCloseEvent ;
class QTableWidget ;
class QMenu ;

struct taskResult ;

class passwordDialog :  public QDialog
{
	Q_OBJECT
public:
	passwordDialog( QTableWidget * table,QWidget * parent = 0 ) ;
	virtual ~passwordDialog() ;
signals :
	void addItemToTable( QString,QString,QString ) ;
	void HideUISignal( void ) ;
	void openFolder( QString ) ;
public slots:
	void HideUI( void ) ;
	void ShowUI( void ) ;
	void ShowUI( const QString& volumePath,const QString& mount_point ) ;
	void clickedPartitionOption( QString ) ;
private slots :
	void cbActicated( int ) ;
	void pbKeyOption( void ) ;
	void pbPluginEntryClicked( QAction * ) ;
	void pbPlugin( void ) ;
	void buttonOpenClicked( void ) ;
	void mount_point( void ) ;
	void clickedPassPhraseFromFileButton( void ) ;
	void passphraseFromFileOption( void ) ;
	void passphraseOption( void ) ;
	void pluginOption( void ) ;
	void file_path( void  ) ;
	void mountPointPath( QString ) ;
	void cbStateChanged( int ) ;
	void taskComplete( const taskResult& ) ;
	void keys( QString key,QString keyFiles ) ;
	void tcryptCancelled( void ) ;
private :
	void tcryptGui( void ) ;
	void openVolume( void ) ;
	QString getUUIDFromPath( const QString& ) ;
	void sendKey( const QString& sockpath ) ;
	void setDefaultOpenMode( void ) ;
	void disableAll( void ) ;
	void enableAll( void ) ;
	void closeEvent( QCloseEvent * ) ;
	bool eventFilter( QObject * watched,QEvent * event ) ;
	void success( const taskResult& ) ;
	void failed( void ) ;
	Ui::PasswordDialog * m_ui ;
	bool m_isWindowClosable ;
	QTableWidget * m_table ;
	bool m_open_with_path ;
	QMenu * m_pluginMenu ;
	QWidget * m_parent ;
	QString m_key ;
	QString m_keyFiles ;
	QString m_device ;
	QString m_point ;

	enum{ key = 0,keyfile = 1,plugin = 2,tcryptKeys = 3 } ;
};

#endif // PASSWORD_DIALOG_H

