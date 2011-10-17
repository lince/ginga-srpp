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

#include "../../include/layout/LayoutRegion.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace ncl {
namespace layout {
	LayoutRegion::LayoutRegion(string id) : Entity(id) {
		title = "";
		outputMapRegionId = "";

		regions = new map<string, LayoutRegion*>;
		sorted = new vector<LayoutRegion*>;

		top = NaN();
		topPercentual = false;

		left = NaN();
		leftPercentual = false;

		bottom = NaN();
		bottomPercentual = false;

		right = NaN();
		rightPercentual = false;

		width = NaN();
		widthPercentual = false;

		height = NaN();
		heightPercentual = false;

		zIndex = NULL;

		movable = false;
		resizable = false;
		decorated = false;

		devClass = -1;

		parent = NULL;
		pthread_mutex_init(&mutex, NULL);
	}

	LayoutRegion::~LayoutRegion() {
		lock();
		if (parent != NULL) {
			parent->removeRegion(this);
			parent = NULL;
		}

		LayoutRegion* region;
		map<string, LayoutRegion*>::iterator i;

		if (regions != NULL) {
			for (i=regions->begin(); i!=regions->end(); ++i) {
				region = i->second;
				region->setParent(NULL);
			}
			regions->clear();
			delete regions;
			regions = NULL;
		}

		if (sorted != NULL) {
			delete sorted;
			sorted = NULL;
		}

		if (zIndex != NULL) {
			delete zIndex;
			zIndex = NULL;
		}

		unlock();
		pthread_mutex_destroy(&mutex);
	}

	void LayoutRegion::addRegion(LayoutRegion* region) {
		int zIndexVal;
		vector<LayoutRegion*>::iterator i;
		string regId;

		regId = region->getId();

		lock();
		if (regions->count(regId) != 0) {
			unlock();
			return;
		}

		(*regions)[regId] = region;
		region->setParent(this);

		zIndexVal = region->getZIndexValue();
		i = sorted->begin();
		while (i != sorted->end()) {
			if (zIndexVal <= (*i)->getZIndexValue()) {
				break;
			}
			++i;
		}

		sorted->insert(i, region);
		unlock();
	}

	LayoutRegion* LayoutRegion::cloneRegion() {
		LayoutRegion *cloneRegion;
		vector<LayoutRegion*>* childRegions;
		LayoutRegion *childRegion;

		cloneRegion = new LayoutRegion(getId());

		if (getParent() != NULL) {
			cloneRegion->setParent(getParent());
		}

		cloneRegion->setTitle(getTitle());

		if (!isNaN(getBottom())) {
			cloneRegion->setBottom(getBottom(), isBottomPercentual());
		}

		if (!isNaN(getLeft())) {
			cloneRegion->setLeft(getLeft(), isLeftPercentual());
		}

		if (!isNaN(getTop())) {
			cloneRegion->setTop(getTop(), isTopPercentual());
		}

		if (!isNaN(getRight())) {
			cloneRegion->setRight(getRight(), isRightPercentual());
		}

		if (!isNaN(getWidth())) {
			cloneRegion->setWidth(getWidth(), isWidthPercentual());
		}

		if (!isNaN(getHeight())) {
			cloneRegion->setHeight(getHeight(), isHeightPercentual());
		}

		cloneRegion->setZIndex(getZIndex());

		cloneRegion->setDecorated(isDecorated());
		cloneRegion->setMovable(isMovable());
		cloneRegion->setResizable(isResizable());

		childRegions = getRegions();
		if (childRegions == NULL) {
			delete cloneRegion;
			return NULL;
		}

		lock();
		vector<LayoutRegion*>::iterator it;
		for (it = childRegions->begin(); it != childRegions->end(); ++it) {
			childRegion = (*it)->cloneRegion();
			cloneRegion->addRegion(childRegion);
		}
		unlock();
		delete childRegions;
		childRegions = NULL;

		return cloneRegion;
	}

	LayoutRegion* LayoutRegion::copyRegion() {
		LayoutRegion *cloneRegion;

		cloneRegion = new LayoutRegion(getId());

		cloneRegion->setTitle(getTitle());

		if (!isNaN(getBottom())) {
			cloneRegion->setBottom(getBottomInPixels(), false);
		}

		if (!isNaN(getLeft())) {
			cloneRegion->setLeft(getAbsoluteLeft(), false);
		}

		if (!isNaN(getTop())) {
			cloneRegion->setTop(getAbsoluteTop(), false);
		}

		if (!isNaN(getRight())) {
			cloneRegion->setRight(getRightInPixels(), false);
		}

		if (!isNaN(getWidth())) {
			cloneRegion->setWidth(getWidthInPixels(), false);
		}

		if (!isNaN(getHeight())) {
			cloneRegion->setHeight(getHeightInPixels(), false);
		}

		cloneRegion->setZIndex(getZIndex());

		cloneRegion->setDecorated(isDecorated());
		cloneRegion->setMovable(isMovable());
		cloneRegion->setResizable(isResizable());

		return cloneRegion;
	}

	int LayoutRegion::compareWidthSize(string w) {
		int oldW;
		int newW;

		oldW = getWidthInPixels();
		if (isPercentualValue(w)) {
			newW = (int)(getParent()->getWidthInPixels() *
				    (getPercentualValue(w) / 100));
		} else {
			newW = atoi(w.c_str());
		}

		if (newW == oldW) {
			return 0;

		} else if (newW > oldW) {
			return 1;

		} else {
			return -1;
		}
	}

	int LayoutRegion::compareHeightSize(string h) {
		int oldH;
		int newH;

		oldH = getHeightInPixels();
		if (isPercentualValue(h)) {
			newH = (int)(getParent()->getHeightInPixels() *
				    (getPercentualValue(h) / 100));
		}
		else {
			newH = atoi(h.c_str());
		}
		if (newH == oldH) {
			return 0;
		}
		else if (newH > oldH) {
			return 1;
		}
		else {
			return -1;
		}
	}

	double LayoutRegion::getBottom() {
		return bottom;
	}

	double LayoutRegion::getHeight() {
		return height;
	}

	double LayoutRegion::getLeft() {
		return left;
	}

	double LayoutRegion::getRight() {
		return right;
	}

	LayoutRegion* LayoutRegion::getRegion(string id) {
		lock();
		if (regions->count(id) == 0) {
			unlock();
			return NULL;
		}

		LayoutRegion* someRegion;
		someRegion = (*regions)[id];
		unlock();
		return someRegion;
	}

	LayoutRegion* LayoutRegion::getRegionRecursively(string id) {
		map<string, LayoutRegion*>::iterator childRegions;
		LayoutRegion* region;
		LayoutRegion* auxRegion;

		if (regions->count(id) != 0) {
			return (*regions)[id];
		}

		childRegions = regions->begin();
		while (childRegions != regions->end()) {
			region = childRegions->second;
			if (region->getId() == id) {
				return region;
			}

			auxRegion = region->getRegionRecursively(id);
			if (auxRegion != NULL) {
				return auxRegion;
			}

			++childRegions;
		}
		return NULL;
	}

	vector<LayoutRegion*>* LayoutRegion::getRegions() {
		map<string, LayoutRegion*>::iterator i;
		vector<LayoutRegion*>* childRegions;
		lock();
		childRegions = new vector<LayoutRegion*>;
		for (i=regions->begin(); i!=regions->end(); ++i) {
			childRegions->push_back(i->second);
		}
		unlock();
		return childRegions;
	}

	string LayoutRegion::getTitle() {
		return title;
	}

	double LayoutRegion::getTop() {
		return top;
	}

	double LayoutRegion::getWidth() {
		return width;
	}

	int LayoutRegion::getZIndex() {
		if (zIndex != NULL) {
			return (int)*zIndex;
		} else {
			return 0;
		}
	}

	int LayoutRegion::getZIndexValue() {
		if (zIndex != NULL) {
			return (int)*zIndex;
		} else {
			return 0;
		}
	}

	bool LayoutRegion::isBottomPercentual() {
		return bottomPercentual;
	}

	bool LayoutRegion::isHeightPercentual() {
		return heightPercentual;
	}

	bool LayoutRegion::isLeftPercentual() {
		return leftPercentual;
	}

	bool LayoutRegion::isRightPercentual() {
		return rightPercentual;
	}

	bool LayoutRegion::isTopPercentual() {
		return topPercentual;
	}

	bool LayoutRegion::isWidthPercentual() {
		return widthPercentual;
	}

	string LayoutRegion::toString() {
		string str;
		map<string, LayoutRegion*>::iterator i;
		LayoutRegion* region;

		str = "id: " + getId() + " title: " + title + '\n';
		wclog << "LayoutRegion::toString(" << this << ")" << endl;
		lock();
		i = regions->begin();
		while (i != regions->end()) {
			region = i->second;
			str = str + region->toString();
			++i;
		}
		unlock();
		return str + '\n';
	}

	bool LayoutRegion::removeRegion(LayoutRegion *region) {
		map<string, LayoutRegion*>::iterator i;
		LayoutRegion* childRegion;

		wclog << "LayoutRegion::removeRegion(" << this << ")" << endl;
		lock();
		if (regions->count(region->getId()) != 0) {
			i = regions->find(region->getId());
			childRegion = i->second;
			childRegion->setParent(NULL);
			region->setParent(NULL);
			regions->erase(i);
			unlock();
			return true;
		}
		unlock();
		return false;
	}

	void LayoutRegion::removeRegions() {
		LayoutRegion* region;
		map<string, LayoutRegion*>::iterator i;

		lock();
		if (regions != NULL) {
			for (i=regions->begin(); i!=regions->end(); ++i) {
				region = i->second;
				region->setParent(NULL);
			}
			regions->clear();
		}
		unlock();
	}

	bool LayoutRegion::setBottom(double newBottom, bool isPercentual) {
		if (newBottom < 0 || isNaN(newBottom)) {
			wclog << "LayoutRegion::setBottom Warning! Trying ";
			wclog << "to set an invalid bottom value: " << newBottom << endl;
			return false;
		}

		bottom = newBottom;
		bottomPercentual = isPercentual;
		if (parent != NULL) {
			if ((getTopInPixels() + getHeightInPixels()) >
				    parent->getHeightInPixels()) {

				// since the region will stay outside the parent edges, the
				// top is set to the maximum value allowed
				if (isPercentual) {
					bottom = 1.0;
				}
				else {
					bottom = parent->getHeightInPixels();
				}
				return false;
			}
		}

		return true;
	}

	bool LayoutRegion::setTargetBottom(double newBottom, bool isPercentual) {
		double tBottom;

		if (newBottom < 0) {
			wclog << "LayoutRegion::setTargetBottom Warning! Trying ";
			wclog << "to set an invalid bottom value: " << newBottom << endl;
			return false;
		}

		if (isPercentual) {
			tBottom = (newBottom / 100) * getHeightInPixels();

		} else {
			tBottom = newBottom;
		}

		bottom = tBottom;
		bottomPercentual = false;

		return true;
	}

	bool LayoutRegion::setHeight(double newHeight, bool isPercentual) {
		if (newHeight < 0 || isNaN(newHeight)) {
			wclog << "LayoutRegion::setHeight Warning! Trying ";
			wclog << "to set an invalid height value: " << newHeight << endl;
			return false;
		}

		height = newHeight;
		heightPercentual = isPercentual;

		if (parent != NULL) {
			if ((getTopInPixels() + getHeightInPixels()) >
				    parent->getHeightInPixels()) {

				// since the region will stay outside the parent edges, the
				// top is set to the maximum value allowed
				if (isPercentual) {
					height = (parent->getHeightInPixels() -
						    getTopInPixels()) / parent->getHeightInPixels();

				} else {
					height = parent->getHeightInPixels() -
						    getTopInPixels();
				}

				return false;
			}
		}

		return true;
	}

	bool LayoutRegion::setTargetHeight(double newHeight, bool isPercentual) {
		double tHeight;

		if (newHeight < 0) {
			wclog << "LayoutRegion::setTargetHeight Warning! Trying ";
			wclog << "to set an invalid height value: " << newHeight << endl;
			return false;
		}

		if (isPercentual) {
			tHeight = (newHeight / 100) * getHeightInPixels();

		} else {
			tHeight = newHeight;
		}

		height = tHeight;
		heightPercentual = false;

		return true;
	}

	bool LayoutRegion::setLeft(double newLeft, bool isPercentual) {
		if ((newLeft < 0) || (isNaN(newLeft))) {
			wclog << "LayoutRegion::setLeft Warning! Trying ";
			wclog << "to set an invalid left value: " << newLeft << endl;
			return false;
		}

		this->left = newLeft;
		leftPercentual = isPercentual;

		if (parent != NULL) {
			if ((getLeftInPixels() + getWidthInPixels()) >
				    parent->getWidthInPixels()) {

				// since the region will stay outside the parent edges, the
				// left is set to the maximum value allowed
				if (isPercentual) {
					left = (double)((parent->getWidthInPixels() -
						   getWidthInPixels()) /
						   parent->getWidthInPixels());

			    } else {
			    	left = (double)(parent->getWidthInPixels() -
			    		   getWidthInPixels());
				}

				return false;
			}
		}

		return true;
	}

	bool LayoutRegion::setTargetLeft(double newLeft, bool isPercentual) {
		double tLeft;

		if (newLeft < 0) {
			wclog << "LayoutRegion::setTargetLeft Warning! Trying ";
			wclog << "to set an invalid left value: " << newLeft << endl;
			return false;
		}

		if (isPercentual) {
			tLeft = (newLeft / 100) * getWidthInPixels();
		} else {
			tLeft = newLeft;
		}

		left = tLeft;
		leftPercentual = false;

		return true;
	}

	bool LayoutRegion::setRight(double newRight, bool isPercentual) {
		if (newRight < 0 || isNaN(newRight)) {
			wclog << "LayoutRegion::setRight Warning! Trying ";
			wclog << "to set an invalid right value: " << newRight << endl;
			return false;
		}

		right = newRight;
		rightPercentual = isPercentual;

		if (parent != NULL) {
			if ((getLeftInPixels() + getWidthInPixels()) >
				     parent->getWidthInPixels()) {

				// since the region will stay outside the parent edges, the
				// top is set to the maximum value allowed
				if (isPercentual) {
					right = 1.0;
				}
				else {
					right = parent->getWidthInPixels();
				}
				return false;
			}
		}

		return true;
	}

	bool LayoutRegion::setTargetRight(double newRight, bool isPercentual) {
		double tRight;

		if (newRight < 0) {
			wclog << "LayoutRegion::setTargetRight Warning! Trying ";
			wclog << "to set an invalid right value: " << newRight << endl;
			return false;
		}

		if (isPercentual) {
			tRight = (newRight / 100) * getWidthInPixels();

		} else {
			tRight = newRight;
		}

		right = tRight;
		rightPercentual = false;

		return true;
	}

	bool LayoutRegion::setTop(double newTop, bool isPercentual) {
		if (newTop < 0 || isNaN(newTop)) {
			wclog << "LayoutRegion::setTop Warning! Trying ";
			wclog << "to set an invalid top value: " << newTop << endl;
			return false;
		}

		top = newTop;
		topPercentual = isPercentual;

		if (parent != NULL) {
			if (((double)(getTopInPixels()) + (double)(getHeightInPixels()))
				    > (double)(parent->getHeightInPixels())) {

				// since the region will stay outside the parent edges, the
				// top is set to the maximum value allowed
				if (isPercentual) {
					top = (double)((double)(parent->getHeightInPixels() -
						    getHeightInPixels()) /
						    (double)(parent->getHeightInPixels()));
				} else {
					top = (double)((double)parent->getHeightInPixels() -
						    (double)getHeightInPixels());
				}
				return false;
			}
		}
		return true;
	}

	bool LayoutRegion::setTargetTop(double newTop, bool isPercentual) {
		double tTop;

		if (newTop < 0 || isNaN(newTop)) {
			wclog << "LayoutRegion::setTop Warning! Trying ";
			wclog << "to set an invalid top value: " << newTop << endl;
			return false;
		}

		if (isPercentual) {
			tTop = (newTop / 100) * getHeightInPixels();

		} else {
			tTop = newTop;
		}

		top = tTop;
		topPercentual = false;

		return true;
	}

    bool LayoutRegion::setWidth(double newWidth, bool isPercentual) {
    	double oldWidth;

		if (newWidth < 0 || isNaN(newWidth)) {
			wclog << "LayoutRegion::setWidth Warning! Trying ";
			wclog << "to set an invalid width value: " << newWidth;
			wclog << endl;
			return false;
		}

		oldWidth = width;
		width = newWidth;
		widthPercentual = isPercentual;

		if (parent != NULL) {
			if ((getLeftInPixels() + getWidthInPixels()) >
				    parent->getWidthInPixels()) {

				// since the region will stay outside the parent edges, the
				// top is set to the maximum value allowed
				if (isPercentual) {
					width = ((double)(parent->getWidthInPixels()) -
						    (double)getLeftInPixels()) /
						    (double)(parent->getWidthInPixels());

				} else {
					width = parent->getWidthInPixels() - getLeftInPixels();
				}
				return false;
			}
		}
		return true;
    }

    bool LayoutRegion::setTargetWidth(double newWidth, bool isPercentual) {
    	double tWidth;

		if (newWidth < 0) {
			wclog << "LayoutRegion::setTargetWidth Warning! Trying ";
			wclog << "to set an invalid width value: " << newWidth;
			wclog << endl;
			return false;
		}

		if (isPercentual) {
			tWidth = (newWidth / 100) * getWidthInPixels();

		} else {
			tWidth = newWidth;
		}

		width = tWidth;
		widthPercentual = false;

		return true;
    }

    void LayoutRegion::validateTarget() {
    	LayoutRegion* rootParent;

		rootParent = getRootParent();
		if (rootParent != NULL) {
			if (getTopInPixels() < rootParent->getTopInPixels()) {
				top = rootParent->getTopInPixels();
				topPercentual = false;
			}

			if ((getTopInPixels() + getHeightInPixels()) >
				    rootParent->getHeightInPixels()) {

				// since the region will stay outside the device edges, the
				// bottom is set to the minimum value allowed
				height = (rootParent->getHeightInPixels() -
						getTopInPixels());

				heightPercentual = false;
			}

			if (getLeftInPixels() < rootParent->getLeftInPixels()) {
				left = rootParent->getLeftInPixels();
				leftPercentual = false;
			}

			if ((getLeftInPixels() + getWidthInPixels()) >
				    rootParent->getWidthInPixels()) {

				// since the region will stay outside the parent edges, the
				// left is set to the maximum value allowed
				width = (rootParent->getWidthInPixels() -
						rootParent->getLeftInPixels());

				widthPercentual = false;
			}
		}
    }

	void LayoutRegion::setTitle(string newTitle) {
		title = newTitle;
	}

    void LayoutRegion::setZIndex(int newZIndex) {
    	if (zIndex == NULL) {
    		zIndex = new double;
    	}
    	*zIndex = newZIndex;
    }

    vector<LayoutRegion*>* LayoutRegion::getRegionsSortedByZIndex() {
    	vector<LayoutRegion*>* sortedRegions;
    	lock();
    	sortedRegions = new vector<LayoutRegion*>(*sorted);
    	unlock();
		return sortedRegions;
    }

    vector<LayoutRegion*>* LayoutRegion::getRegionsOverRegion(
    	    LayoutRegion* region) {

    	vector<LayoutRegion*>* allRegions;
    	vector<LayoutRegion*>::iterator i;
		vector<LayoutRegion*>* frontRegions;
		LayoutRegion* childRegion;

		frontRegions = new vector<LayoutRegion*>;
		allRegions = getRegionsSortedByZIndex();

		wclog << "LayoutRegion::getRegionsOverRegion(" << this << ")" << endl;

		i = allRegions->begin();
		while (i != allRegions->end()) {
			childRegion = *i;
			if (childRegion->getZIndexValue() > region->getZIndexValue()) {
				frontRegions->insert(frontRegions->begin(), childRegion);
			}
			++i;
		}
		delete allRegions;
		allRegions = NULL;

		return frontRegions;
    }

    LayoutRegion* LayoutRegion::getRootParent() {
    	LayoutRegion* rootParent  = NULL;

    	if (parent == NULL) {
    		return NULL;
    	}

    	rootParent = parent->getParent();
    	if (rootParent == NULL) {
    		return parent;
    	}

    	while (rootParent->getParent() != NULL) {
    		rootParent = rootParent->getParent();
    	}

    	return rootParent;
    }

    LayoutRegion* LayoutRegion::getParent() {
    	return parent;
    }

	void LayoutRegion::setDeviceClass(int deviceClass, string mapId) {
		bool changed = false;

		if (deviceClass != this->devClass && deviceClass >=0) {
			this->devClass = deviceClass;
			changed = true;
		}

		if (outputMapRegionId == "") {
			this->outputMapRegionId = mapId;
			changed = true;
		}

		if (changed) {
			refreshDeviceClassRegions();
		}
	}

	int LayoutRegion::getDeviceClass() {
		return devClass;
	}

	string LayoutRegion::getOutputMapRegionId() {
		return outputMapRegionId;
	}

    void LayoutRegion::setParent(LayoutRegion* parent) {
    	int dClass;
    	string mapId;

    	this->parent = parent;

    	if (parent != NULL) {
    		dClass = parent->getDeviceClass();
    		mapId  = parent->getOutputMapRegionId();
    		if (dClass >= 0) {
    			setDeviceClass(dClass, mapId);
    		}
    	}
    }

    void LayoutRegion::refreshDeviceClassRegions() {
    	map<string, LayoutRegion*>::iterator i;

    	i = regions->begin();
    	while (i != regions->end()) {
    		if (i->second != this) {
    			i->second->setDeviceClass(devClass, outputMapRegionId);
    		}
    		++i;
    	}
    }

    int LayoutRegion::getTopInPixels() {
    	double b, h;

		if (!isNaN(top)) {
			// top was defined
			if (isTopPercentual() && parent != NULL) {
				return (int)((top / 100.0) *
					    parent->getHeightInPixels());

			} else {
				return (int)top;
			}

		} else if ((!isNaN(height)) &&
			    (!isNaN(bottom))) {

			// top is based on height and bottom
			if (isHeightPercentual() && parent != NULL) {
				h = (int)((height / 100.0) *
					    parent->getHeightInPixels());

			} else {
				h = height;
			}

			if (parent != NULL) {
				if (isBottomPercentual()) {
					b = (int)(((100 - bottom) / 100.0) *
						    parent->getHeightInPixels());
				} else {
					b = parent->getHeightInPixels() - bottom;
				}

			} else {
				b = h;
			}

			return (int)(b - h);

		} else {
			// default value
			return 0;
		}
    }

    int LayoutRegion::getBottomInPixels() {
    	double bottomInPixels;
    	double t, h;

		bottomInPixels = NaN();

		if (!isNaN(height) &&
			    !isNaN(top)) {

			// bottom is based on height and top, independent
			// of bottom definition
			if (isTopPercentual() && parent != NULL) {
				t = (int)(((double)top / 100.0) *
					    parent->getHeightInPixels());

			} else {
				t = top;
			}

			if (isHeightPercentual() && parent != NULL) {
				h = (int)(((double)height / 100.0) *
					    parent->getHeightInPixels());

			} else {
				h = height;
			}
			bottomInPixels = (int)(t + h);

		} else if (!isNaN(bottom)) {
			// bottom is based on its own value
			if (isBottomPercentual() && parent != NULL) {
				bottomInPixels = (int)(((100 - bottom) / 100) *
					    parent->getHeightInPixels());

			} else {
				bottomInPixels = (int)(parent->getHeightInPixels()-bottom);
			}

		} else {
			bottomInPixels = (int)(getTopInPixels() + getHeightInPixels());
		}

		return (int)bottomInPixels;
    }

    int LayoutRegion::getRightInPixels() {
    	double l, w;

		if (!isNaN(width) &&
			    !isNaN(left)) {

			// right is based on width and left, independent of right
			// definition
			if (isLeftPercentual() && parent != NULL) {
				l = (int)(((double)left / 100.0) *
					    parent->getWidthInPixels());
			} else {
				l = left;
			}

			if (isWidthPercentual() && parent != NULL) {
				w = (int)(((double)width / 100.0) *
					    parent->getWidthInPixels());
			} else {
				w = width;
			}
			return (int)(l + w);

		} else if (!isNaN(right)) {
			// right is based on its own value
			if (isRightPercentual() && parent != NULL) {
				return (int)(((100 - right) / 100) *
					    parent->getWidthInPixels());
			} else {
				return (int) ( parent->getWidthInPixels() - right );
			}

		} else {
			return (int)(getLeftInPixels() + getWidthInPixels());
		}
		return 0;
    }

    int LayoutRegion::getLeftInPixels() {
    	double r, w;

		if (!isNaN(left)) {
			// left was defined
			if (isLeftPercentual() && parent != NULL) {
				return (int)((left / 100) * parent->getWidthInPixels());
			} else {
				return (int)left;
			}

		} else if (!isNaN(width) && !isNaN(right)) {
			// left is based on width and right
			if (isWidthPercentual() && parent != NULL) {
				w = (int)((width / 100) *
					    parent->getWidthInPixels());
			} else {
				w = (int)width;
			}

			if (parent != NULL) {
				if (isRightPercentual()) {
					r = (int)(((100 - right) / 100) *
						    parent->getWidthInPixels());

				} else {
					r = parent->getWidthInPixels() - (int)right;
				}

			} else {
				r = w;
			}

			return (int)(r - w);

		} else {
			// default value
			return 0;
		}
    }

    int LayoutRegion::getHeightInPixels() {
    	double t, b;

		if (!isNaN(getHeight())) {
			if (isHeightPercentual() && parent != NULL) {
				return (int)((getHeight() / 100) *
					    parent->getHeightInPixels());
			} else {
				return (int)getHeight();
			}
		}
		else {
			t = 0;
			b = parent->getHeightInPixels();

			if (!isNaN(getTop())) {
				// height is based on top and bottom, independent of
				// height definition
				if (isTopPercentual() && parent != NULL) {
					t = (int)((getTop() / 100) *
						    parent->getHeightInPixels());
				}
				else {
					t = (int)getTop();
				}
			}

			if (!isNaN(getBottom())) {
				if (isBottomPercentual() && parent != NULL) {
					b = (int)(((100 - getBottom()) / 100) *
						    parent->getHeightInPixels());
				}
				else {
					b = parent->getHeightInPixels() - (int)getBottom();
				}
			}

			if (b >= t) {
				return (int)(b - t);
			}
			else {
				return 0;
			}
		}
    }

    int LayoutRegion::getWidthInPixels() {
    	int l, r;

		if (!(isNaN(getWidth()))) {
			if (isWidthPercentual() && parent != NULL) {
				return (int)((width / 100.0) * parent->getWidthInPixels());

			} else {
				return (int)getWidth();
			}

		} else {
			l = 0;
			r = parent->getWidthInPixels();

			if (!(isNaN(getLeft()))) {
				if (isLeftPercentual() && parent != NULL) {
					l = (int)((getLeft() / 100) *
						    parent->getWidthInPixels());
				} else {
					l = (int)getLeft();
				}
			}

			if (!(isNaN(getRight()))) {
				if (isRightPercentual() && parent != NULL) {
					r = (int)((getRight() / 100) *
						    parent->getWidthInPixels());
				} else {
					r = (int)getRight();
				}
			}

			if (r >= l) {
				return (int)(r - l);

			} else {
				return 0;
			}

		}
		return 0;
    }

	bool LayoutRegion::isMovable() {
    	return movable;
    }

	bool LayoutRegion::isResizable() {
    	return resizable;
    }

	bool LayoutRegion::isDecorated() {
    	return decorated;
    }

	void LayoutRegion::setMovable(bool movable) {
    	this->movable = movable;
    }

	void LayoutRegion::setResizable(bool resizable) {
    	this->resizable = resizable;
    }

	void LayoutRegion::setDecorated(bool decorated) {
    	this->decorated = decorated;
    }

	void LayoutRegion::resetTop() {
    	top = NaN();
    }

	void LayoutRegion::resetBottom() {
    	bottom = NaN();
    }

	void LayoutRegion::resetLeft() {
    	left = NaN();
    }

	void LayoutRegion::resetHeight() {
    	height = NaN();
    }

	void LayoutRegion::resetWidth() {
    	width = NaN();
    }

	void LayoutRegion::resetZIndex() {
		if (zIndex != NULL) {
			delete zIndex;
			zIndex = NULL;
		}
    }

	void LayoutRegion::resetDecorated() {
    	decorated = false;
    }

	void LayoutRegion::resetMovable() {
    	movable = false;
    }

	void LayoutRegion::resetResizable() {
    	resizable = false;
    }

	int LayoutRegion::getAbsoluteLeft() {
    	if (parent != NULL) {
			return getLeftInPixels() + parent->getAbsoluteLeft();

		} else {
			return getLeftInPixels();
		}
    }

	int LayoutRegion::getAbsoluteTop() {
    	if (parent != NULL) {
			return getTopInPixels() + parent->getAbsoluteTop();

		} else {
			return getTopInPixels();
		}
    }

	double LayoutRegion::getPercentualValue(string value) {
		string actualValue;
		double floatValue;

		//retirar o caracter percentual da string
		actualValue = value.substr(0, value.length() - 1);
		//converter para float
		floatValue = stof( actualValue );

		//se menor que zero, retornar zero
		if (floatValue < 0)
			floatValue = 0;
		//else if (floatValue > 100)
			//se maior que 100, retornar 100
			//floatValue = 100;

		//retornar valor percentual
		return floatValue;
	}

	bool LayoutRegion::isPercentualValue(string value) {
		if (value.substr( value.length()-1, 1 ) == "%")
			return true;
		else
			return false;
	}

	bool LayoutRegion::intersects(LayoutRegion* r) {
		return !( r->left > left + width ||
				r->left + r->width < left ||
				r->top > top + height ||
				r->top + r->height < top);
	}

	bool LayoutRegion::intersects(int x, int y) {
		return !(x > getAbsoluteLeft() + getWidthInPixels() ||
				x < getAbsoluteLeft() ||
				y > getAbsoluteTop() + getHeightInPixels() ||
				y < getAbsoluteTop());
	}

	void LayoutRegion::lock() {
		pthread_mutex_lock(&mutex);
	}

	void LayoutRegion::unlock() {
		pthread_mutex_unlock(&mutex);
	}
}
}
}
}
}
