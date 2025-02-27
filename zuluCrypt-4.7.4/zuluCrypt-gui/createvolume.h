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

#ifndef CREATEPARTITION_H
#define CREATEPARTITION_H

#include <QDialog>
#include <QString>

#include "utility.h"

class keystrength ;
class QCloseEvent ;

/*
 * this header is created at config time
 */
#include "truecrypt_support_1.h"

namespace Ui {
    class createvolume;
}

class createvolume : public QDialog
{
	Q_OBJECT
public:
	typedef enum{
		plain = 0,
		luks = 1,
		normal_truecrypt = 2,
		normal_and_hidden_truecrypt = 3
	}createVolumeType ;

	explicit createvolume( QWidget * parent = 0 ) ;
	~createvolume() ;
signals:
	void CreateVolume( QString fileSystem,QString containterType,QString volumePath,QString passphrase,bool passphraseFromFile ) ;
	void HideUISignal( void ) ;
public slots:
	void ShowPartition( QString volume ) ;
	void ShowFile( QString volume ) ;
	void HideUI( void ) ;
private slots:
	void keyChanged( QString ) ;
	void pbCreateClicked( void ) ;
	void pbCancelClicked( void ) ;
	void cbNormalVolume( int ) ;
	void cbHiddenVolume( int ) ;
	void pbOpenKeyFile( void ) ;
	void pbOpenHiddenKeyFile( void ) ;
	void taskFinished( int ) ;
	void volumeType( int ) ;
	void dialogResult( int ) ;
	void setOptions( int ) ;
private:
	void eraseDataPartition( void ) ;
	void findInstalledFs( void ) ;
	void enableAll( void ) ;
	void disableAll( void ) ;
	void closeEvent( QCloseEvent * ) ;
	bool eventFilter( QObject * watched,QEvent * event ) ;
	void ShowUI( const QString&,const QString& ) ;
	bool m_created ;
	Ui::createvolume * m_ui ;
	bool m_isWindowClosable ;
	keystrength * m_keyStrength ;
	QString m_volumeType ;
	bool m_warned ;
};

#endif // CREATEPARTITION_H
