/******************************************************************************
Este arquivo eh parte da implementacao do ambiente declarativo do middleware
Ginga (Ginga-NCL).

Direitos Autorais Reservados (c) 1989-2007 PUC-Rio/Laboratorio TeleMidia

Este programa eh software livre; voce pode redistribui-lo e/ou modificah-lo sob
os termos da Licenca Publica Geral GNU versao 2 conforme publicada pela Free
Software Foundation.

Este programa eh distribuido na expectativa de que seja util, porem, SEM
NENHUMA GARANTIA; nem mesmo a garantia implicita de COMERCIABILIDADE OU
ADEQUACAO A UMA FINALIDADE ESPECIFICA. Consulte a Licenca Publica Geral do
GNU versao 2 para mais detalhes.

Voce deve ter recebido uma copia da Licenca Publica Geral do GNU versao 2 junto
com este programa; se nao, escreva para a Free Software Foundation, Inc., no
endereco 59 Temple Street, Suite 330, Boston, MA 02111-1307 USA.

Para maiores informacoes:
ncl @ telemidia.puc-rio.br
http://www.ncl.org.br
http://www.ginga.org.br
http://www.telemidia.puc-rio.br
******************************************************************************
This file is part of the declarative environment of middleware Ginga (Ginga-NCL)

Copyright: 1989-2007 PUC-RIO/LABORATORIO TELEMIDIA, All Rights Reserved.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License version 2 as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License version 2 for more
details.

You should have received a copy of the GNU General Public License version 2
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA

For further information contact:
ncl @ telemidia.puc-rio.br
http://www.ncl.org.br
http://www.ginga.org.br
http://www.telemidia.puc-rio.br
*******************************************************************************/

#include "util/Color.h"

#include "../../../include/PlainTxtPlayer.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace ginga {
namespace core {
namespace player {
	PlainTxtPlayer::PlainTxtPlayer(string mrl) : TextPlayer() {
		pthread_mutex_init(&mutex, NULL);
		content = "";
	}

	PlainTxtPlayer::~PlainTxtPlayer() {
		pthread_mutex_lock(&mutex);
		if (surface != NULL) {
			/*
			 * the surface could never be a child of window
			 * (it gets the widget surface)
			 */
			surface->setParent(NULL);
		}
		pthread_mutex_unlock(&mutex);
		pthread_mutex_destroy(&mutex);
   	}

	void PlainTxtPlayer::setFile(string mrl) {
		if (mrl == "" || !fileExists(mrl)) {
			cout << "PlainTxtPlayer::setFile Warning! File not found: '";
			cout << mrl << "'" << endl;
			return;
		}

		if (mrl.length() > 4) {
			string fileType;

			this->mrl = mrl;
			fileType = this->mrl.substr(this->mrl.length() - 4, 4);
			if (fileType != ".txt") {
				cout << "PlainTxtPlayer::loadFile Warning! Unknown file ";
				cout << "type for: '" << this->mrl << "'" << endl;
			}

		} else {
			cout << "PlainTxtPlayer::loadFile Warning! Unknown extension ";
			cout << "type for: '" << mrl << "'" << endl;
		}
	}

	void PlainTxtPlayer::loadTxt() {
		ifstream fis;
		string line, aux;
		int surfaceW, surfaceH;

		if (isXmlStr(mrl)) {
			mrl = ("/usr/local/lib/ginga/epgfactory/src/" +
					mrl.substr(mrl.find_last_of("/"),
							mrl.length() - mrl.find_last_of("/")));
		}

		pthread_mutex_lock(&mutex);
		fis.open((this->mrl).c_str(), ifstream::in);
		if (!fis.is_open() && (mrl != "" || content == "")) {
			cout << "PlainTxtPlayer::loadFile Warning! can't open input ";
			cout << "file: '" << this->mrl << "'" << endl;
			pthread_mutex_unlock(&mutex);
			return;
		}

		if (fontColor == NULL) {
			fontColor = new Color(255, 255, 255, 255);
		}

		if (surface != NULL && surface->getParent() != NULL) {
			if (bgColor != NULL) {
				//this->surface->setCaps(0);
				surface->clearContent();
				surface->setBgColor(bgColor);

			} else {
				((IWindow*)(surface->getParent()))->clearContent();
			}
		}

		this->currentLine = 0;
		this->currentColumn = 0;

		if (mrl != "" && content == "") {
			surface->getSize(&surfaceW, &surfaceH);
			while (!fis.eof() && fis.good() &&
					(currentLine + fontHeight) < surfaceH) {

				getline(fis, line);
				drawTextLn(line);
			}

		} else if (content != "") {
			drawTextLn(content);
		}

		if (surface != NULL && surface->getParent() != NULL) {
			((IWindow*)(surface->getParent()))->validate();
		}

		fis.close();
		pthread_mutex_unlock(&mutex);
	}

	void PlainTxtPlayer::play() {
		Player::play();
		if (surface != NULL && surface->getContent() != NULL) {
			wclog << "PlainTxtPlayer::play ok" << endl;
			loadTxt();

		} else {
			wclog << "PlainTxtPlayer::play warning" << endl;
		}
	}

	ISurface* PlainTxtPlayer::getSurface() {
		/*if (surface != NULL && surface->getSurface() != NULL) {
			loadTxt();
		}*/
		return surface;
	}

	void PlainTxtPlayer::setContent(string content) {
		pthread_mutex_lock(&mutex);

		if (surface != NULL && surface->getParent() != NULL) {
			if (bgColor != NULL) {
				//this->surface->setCaps(0);
				surface->clearContent();
				surface->setBgColor(bgColor);

			} else {
				((IWindow*)(surface->getParent()))->clearContent();
			}
		}

		this->currentLine = 0;
		this->currentColumn = 0;
		this->content = content;

		if (content != "") {
			drawTextLn(this->content);
			mrl = "";
		}

		if (surface != NULL && surface->getParent() != NULL) {
			((IWindow*)(surface->getParent()))->validate();
		}

		pthread_mutex_unlock(&mutex);
	}

	void PlainTxtPlayer::setPropertyValue(
			string name, string value, double duration, double by) {

		pthread_mutex_lock(&mutex);

		vector<string>* params;
		bool refresh = true;

		/*
		cout << "PlainTxtPlayer::setPropertyValue name = '" << name.c_str();
		cout << "' value = '" << value.c_str() << "'" << endl;
		*/

		if (surface == NULL || surface->getContent() == NULL) {
			refresh = false;
		}

		if (name == "fontColor" && value != "") {
			if (fontColor != NULL) {
				delete fontColor;
				fontColor = NULL;
			}

			fontColor = new Color(value);

		} else if (name == "fontSize" && value != "" &&
				isNumeric((void*)(value.c_str()))) {

			setFontSize((int)(stof(value)));

		} else if (name == "fontUri" && value != "") {
			setFont(value);

		} else if (name == "x-bgColor" && value != "") {
			if (surface != NULL) {
				if (bgColor != NULL) {
					delete bgColor;
					bgColor = NULL;
				}

				if (value == "black") {
					bgColor = new Color(0x10, 0x10, 0x10, 0xFF);

				} else {
					bgColor = new Color(value);
				}

				//this->surface->setCaps(0);
				surface->clearContent();
				surface->setBgColor(bgColor);
			}

		} else if (name == "x-rgbBgColor" && value != "") {
			params = split(value, ",");
			if (params->size() == 3) {
				if (surface != NULL) {
					if (bgColor != NULL) {
						delete bgColor;
						bgColor = NULL;
					}

					bgColor = new Color(
							(int)stof((*params)[0]),
							(int)stof((*params)[1]),
							(int)stof((*params)[2]));

					//this->surface->setCaps(0);
					surface->clearContent();
					surface->setBgColor(bgColor);
				}

			} else {
				refresh = false;
			}

			delete params;
			params = NULL;

		} else if (name == "x-rgbFontColor" && value != "") {
			params = split(value, ",");
			if (params->size() == 3) {
				if (fontColor != NULL) {
					delete fontColor;
					fontColor = NULL;
				}

				fontColor = new Color(
						(int)stof((*params)[0]),
						(int)stof((*params)[1]),
						(int)stof((*params)[2]));

			} else {
				refresh = false;
			}

			delete params;
			params = NULL;

		} else if (name == "x-content") {
			pthread_mutex_unlock(&mutex);
			setContent(value);
			pthread_mutex_lock(&mutex);
			refresh = false;

		} else if (name == "x-setFile" && value != "") {
			setFile(value);
			refresh = false;
		}

		Player::setPropertyValue(name, value, duration, by);
		pthread_mutex_unlock(&mutex);

		// refreshing changes
		if (refresh) {
			loadTxt();
			if (notifyContentUpdate) {
				notifyListeners(
						PL_NOTIFY_UPDATECONTENT, "", TYPE_PASSIVEDEVICE);
			}
		}
	}
}
}
}
}
}
}

extern "C" ::br::pucrio::telemidia::ginga::core::player::IPlayer*
		createPlainTextPlayer(const char* mrl, bool hasVisual) {

	return (new ::br::pucrio::telemidia::ginga::core::player::
			PlainTxtPlayer((string)mrl));
}

extern "C" void destroyPlainTextPlayer(
		::br::pucrio::telemidia::ginga::core::player::IPlayer* p) {

	delete p;
}
