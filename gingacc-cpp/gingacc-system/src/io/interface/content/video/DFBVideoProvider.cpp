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

#include "../../../../../include/io/LocalDeviceManager.h"
#include "../../../../../include/io/interface/content/video/DFBVideoProvider.h"
#include "../../../../../include/io/interface/output/dfb/DFBSurface.h"

/* macro for a safe call to DirectFB functions */
#ifndef DFBCHECK
#define DFBCHECK(x...)                                            \
{                                                                 \
	DFBResult err = x;                                            \
	if (err != DFB_OK) {                                          \
		fprintf( stderr, "%s <%d>:\n\t", __FILE__, __LINE__ );    \
	}                                                             \
}
#endif /*DFBCHECK*/

namespace br {
namespace pucrio {
namespace telemidia {
namespace ginga {
namespace core {
namespace system {
namespace io {
	DFBVideoProvider::DFBVideoProvider(const char* mrl) {
		IDirectFB* dfb = NULL;

		rContainer = new DFBRendererContainer;
		rContainer->dec = NULL;
		rContainer->isValid = false;

		if (mrl != NULL) {
			dfb = (IDirectFB*)(LocalDeviceManager::getInstance()->getGfxRoot());
			DFBCHECK(dfb->CreateVideoProvider(dfb, mrl, &rContainer->dec));

			/* enable gapless looping playback */
			//rContainer->dec->SetPlaybackFlags(rContainer->dec, DVPLAY_LOOPING);
		}
	}

	DFBVideoProvider::DFBVideoProvider(IDirectFBVideoProvider* dec) {
		rContainer = new DFBRendererContainer;
		rContainer->dec = dec;
		rContainer->isValid = false;
	}

	DFBVideoProvider::~DFBVideoProvider() {
		set<IDirectFBVideoProvider*>::iterator i;

		if (rContainer->dec != NULL) {
			rContainer->dec->Release(rContainer->dec);
			rContainer->dec      = NULL;
			rContainer->listener = NULL;
			rContainer->surface  = NULL;
			delete rContainer;
			rContainer = NULL;
		}
	}

	void* DFBVideoProvider::getContent() {
		return (void*)(rContainer->dec);
	}

	void DFBVideoProvider::feedBuffers() {
		DFBVideoProviderStatus status;

		do {
			::usleep(150000);
			rContainer->dec->GetStatus(rContainer->dec, &status);
		} while (status == DVSTATE_BUFFERING);
	}

	void DFBVideoProvider::getVideoSurfaceDescription(
			DFBSurfaceDescription* dsc) {

		DFBCHECK(rContainer->dec->GetSurfaceDescription(
				rContainer->dec, dsc));
	}

	ISurface* DFBVideoProvider::getPerfectSurface() {
		DFBSurfaceDescription dsc;

		getVideoSurfaceDescription(&dsc);
		dsc.flags = (DFBSurfaceDescriptionFlags)(DSDESC_WIDTH | DSDESC_HEIGHT);

		return new DFBSurface(
				LocalDeviceManager::getInstance()->createSurface(&dsc));
	}

	bool DFBVideoProvider::checkVideoResizeEvent(ISurface* frame) {
		IDirectFBSurface* s = NULL;
		DFBSurfaceDescription dsc;
		int w, h;

		getVideoSurfaceDescription(&dsc);

		frame->getSize(&w, &h);
		if (dsc.width != w || dsc.height != h) {
			cout << "DFBVideoProvider::checkVideoResizeEvent ";
			cout << "width = '" << w << "' height = '" << h << "'" << endl;

			s = (IDirectFBSurface*)(
					LocalDeviceManager::getInstance()->createSurface(&dsc));

			frame->setContent(s);
			return true;
		}

		return false;
	}

	void DFBVideoProvider::dynamicRenderCallBack(void* rendererContainer) {
		IDirectFBWindow* w          = NULL;
		IDirectFBSurface* s         = NULL;
		IDirectFBSurface* frame     = NULL;
		DFBRendererContainer* cont  = NULL;
		IWindow* someWindow         = NULL;
		ISurface* someSurface       = NULL;

		bool fitTo;
		bool stretchTo;
		int winWidth, winHeight;
		int surWidth, surHeight;

		cont = (DFBRendererContainer*)rendererContainer;
		if (cont == NULL || !cont->isValid) {
			return;
		}

		someSurface = cont->surface;
		if (someSurface == NULL) {
			return;
		}

		frame = (IDirectFBSurface*)(someSurface->getContent());
		if (frame == NULL) {
			return;
		}

		someWindow = (IWindow*)(someSurface->getParent());
		if (someWindow != NULL && cont->isValid) {
			DFBCHECK(frame->GetSize(frame, &surWidth, &surHeight));

			someWindow->lock();
			fitTo = someWindow->getFit();
			stretchTo = someWindow->getStretch();
			w = (IDirectFBWindow*)(someWindow->getContent());

			if (w == NULL || !cont->isValid) {
				someWindow->unlock();
				return;
			}

			//DFBCHECK(w->GetSurface(w, &s));
			if (w->GetSurface(w, &s) != DFB_OK) {
				return;
			}

			s->SetBlittingFlags(s, DSBLIT_NOFX);
			s->GetSize(s, &winWidth, &winHeight);

			if ((winWidth != surWidth || winHeight != surHeight)/* && fitTo*/) {
				//if (stretchTo) {
					DFBCHECK(s->StretchBlit(
						    s,
						    frame,
						    NULL,
						    NULL));

				/*} else {
					ISurface* sur;
					IDirectFBSurface* s2;

					sur = new DFBSurface(
							someWindow->getW(), someWindow->getH());

					s2 = (IDirectFBSurface*)(sur->getContent());

					DFBCHECK(s2->StretchBlit(
						    s2,
						    s,
						    NULL,
						    NULL));

					DFBCHECK(s2->Flip(
							s2, NULL, (DFBSurfaceFlipFlags)DSFLIP_BLIT));

					DFBCHECK(s->Blit(s, s2, NULL, 0, 0));
					DFBCHECK(s->Flip(
							s, NULL, (DFBSurfaceFlipFlags) DSFLIP_BLIT));

					delete sur;
				}*/

			} else {
				DFBCHECK(s->Blit(s, frame, NULL, 0, 0));
			}

			DFBCHECK(s->Flip(s, NULL, (DFBSurfaceFlipFlags)DSFLIP_NONE));
			someWindow->unlock();

		} else if (frame != NULL) {
			DFBCHECK(frame->Blit(frame, frame, NULL, 0, 0));
		}

		/*
		listener = cont->listener;
		dec = cont->dec;

		if (listener != NULL && dec != NULL) {
			endTime = listener->getEndTime();
			if (endTime <= 0) {
				dec->GetStatus(dec, &st);
				if (st == DVSTATE_FINISHED) {
					listener->finished();
				}
			}
		}*/
	}

	void DFBVideoProvider::getOriginalResolution(int* height, int* width) {
		DFBSurfaceDescription dsc;

		if (rContainer->dec != NULL) {
			DFBCHECK(rContainer->dec->GetSurfaceDescription(
					rContainer->dec, &dsc));

			*width = dsc.width;
			*height = dsc.height;
		}
	}

	double DFBVideoProvider::getTotalMediaTime() {
		DFBResult res;
		double size;

		if (rContainer->dec != NULL) {
			res = rContainer->dec->GetLength(rContainer->dec, &size);
			if (res == DFB_OK) {
				return size;
			}
		}
		return 0;
	}

	double DFBVideoProvider::getMediaTime() {
		DFBResult res;
		double pos;

		if (rContainer->dec!= NULL) {
			res = rContainer->dec->GetPos(rContainer->dec, &pos);
			if (res != DFB_EOF) {
				return pos;
			}
		}
		return -1;
	}

	void DFBVideoProvider::setMediaTime(double pos) {
		if (rContainer->dec != NULL) {
			DFBCHECK(rContainer->dec->SeekTo(rContainer->dec, pos));
		}
	}

	void DFBVideoProvider::playOver(
			ISurface* surface, bool hasVisual, IProviderListener* listener) {

		IDirectFBSurface* s;

		s = (IDirectFBSurface*)(surface->getContent());

		rContainer->listener = listener;
		rContainer->surface = surface;
		rContainer->isValid = true;

		DFBCHECK(rContainer->dec->PlayTo(
				rContainer->dec,
				s, NULL, dynamicRenderCallBack, (void*)rContainer));
	}

	void DFBVideoProvider::resume(ISurface* surface, bool hasVisual) {
		playOver(surface, hasVisual, rContainer->listener);
	}

	void DFBVideoProvider::pause() {
		stop();
	}

	void DFBVideoProvider::stop() {
		rContainer->isValid = false;
		DFBCHECK(rContainer->dec->Stop(rContainer->dec));
	}

	void DFBVideoProvider::setSoundLevel(float level) {
		DFBCHECK(rContainer->dec->SetVolume(rContainer->dec, level));
	}

	bool DFBVideoProvider::releaseAll() {
		//TODO: release all structures
		return false;
	}
}
}
}
}
}
}
}

extern "C" ::br::pucrio::telemidia::ginga::core::system::io::
		IContinuousMediaProvider* createDFBVideoProvider(const char* mrl) {

	return (new ::br::pucrio::telemidia::ginga::core::system::io::
			DFBVideoProvider(mrl));
}

extern "C" void destroyDFBVideoProvider(
		::br::pucrio::telemidia::ginga::core::system::io::
		IContinuousMediaProvider* cmp) {

	delete cmp;
}
