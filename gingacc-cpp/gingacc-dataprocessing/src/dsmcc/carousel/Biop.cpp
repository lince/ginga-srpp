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

#include "../../../include/dsmcc/carousel/Biop.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace ginga {
namespace core {
namespace dataprocessing {
namespace carousel {
	Biop::Biop(Module* module, ObjectProcessor* processor) {
		this->module = module;
		currentSize = 0;
		isValidHdr = false;
		hasMoreBiopMessage = false;
		objectKind = "";
		objectInfo = "";
		objects = new map<string, Object*>;
		this->processor = processor;

		moduleFd = open(module->getModuleFileName().c_str(),
				    O_RDWR|O_LARGEFILE);

		if (moduleFd < 0) {
			abortProcess("Cannot open file " + module->getModuleFileName());

		} else {
			this->isValidHdr = processMessageHeader();
		}
	}

	Biop::~Biop() {
//				cout << "Biop destructor" << endl;

		if (moduleFd >= 0) {
//					cout << "closing file" << endl;
			close(moduleFd);
		}

		if (data != NULL && sizeof(data) > 0) {
//					cout << "free data memory" << endl;
			free(data);
			data = NULL;
		}
	}

	string Biop::getObjectKind() {
		return objectKind;
	}

	string Biop::getObjectInfo() {
		return objectInfo;
	}

	void Biop::abortProcess(string warningText) {
		cout << "Warning! " << warningText.c_str() << endl;
		close(moduleFd);
		if (data != NULL && sizeof(data) > 0) {
			free(data);
			data = NULL;
		}
		moduleFd = -1;
	}

	bool Biop::processMessageHeader() {
		int rval;
		unsigned int i;

		data = new char[12];
		memset(data, 0, 12);
		i = 0;

		// BIOP::MessageHeader

		// Check magic Field == BIOP
		rval = read(moduleFd, (void*)&(data[0]), 12);
		if (rval != 12) {
			abortProcess(
					"Biop::processMessageHeader "
					"Can't read 12 bytes on file, " +
				    module->getModuleFileName() +
				    ", rval = " + itos((int)rval));

			return false;
		}

		if ((data[i] & 0xFF) != 0x42 || (data[i+1] & 0xFF) != 0x49 ||
			    (data[i+2] & 0xFF) != 0x4F ||
			    (data[i+3] & 0xFF) != 0x50) {

			abortProcess(
					"Biop::processMessageHeader "
					"magic Field is not 'BIOP' in file: "
				    + module->getModuleFileName());

			return false;
		}

		i = i + 4;

		// Check biop_version field
		if ((data[i] & 0xFF) != 0x01 || (data[i + 1] & 0xFF) != 0x00) {
			abortProcess("Biop::processMessageHeader Wrong biop_version");
			return false;
		}
		i = i + 2;

		// check byte_order field
		if ((data[i] & 0xFF) != 0x00) {
			abortProcess("Biop::processMessageHeader Wrong byte_order");
			return false;
		}
		i++;

		// check message_type field
		if ((data[i] & 0xFF) != 0x00) {
			abortProcess("Biop::processMessageHeader Wrong message_type");
			return false;
		}
		i++;

		// get message_size
		this->messageSize = ((data[i] & 0xFF) << 24) |
				((data[i+1] & 0xFF) << 16) |
				((data[i+2] & 0xFF) << 8) | (data[i+3] & 0xFF);

		cout << "messageSize = '" << hex << messageSize;
		cout << "' currentSize = '" << hex << currentSize << "'";
		cout << " moduleSize = '" << hex << module->getSize() << "'" << endl;

		idx = 0;

		currentSize = currentSize + (messageSize + 12);
		if (currentSize >= module->getSize()) {
			hasMoreBiopMessage = false;

		} else {
			hasMoreBiopMessage = true;
		}

		if (data != NULL && sizeof(data) > 0) {
			delete data;
			data = NULL;
		}
		return true;
	}

	void Biop::processMessageSubHeader() {
		int rval;
		unsigned int len;
		char* field;

		cout << "Biop::processMessageSubHeader" << endl;

		data = (char*)malloc(messageSize + 12);
		memset(data, 0, sizeof(data));

		rval = read(moduleFd, (void*)&(data[0]), messageSize);

		if ((unsigned int)rval != messageSize) {
			abortProcess("Cant read message on file, " +
				    module->getModuleFileName() +
				    ", rval = " + itos((int)rval));

			return;
		}

		cout << rval << " bytes readed from ";
		cout  << module->getModuleFileName() << endl;

		// BIOP::MessageSubHeader

		// objectKey
		len = (data[idx] & 0xFF);
		idx++;

		if (len > 4) {
			cout << "Warning! Size of objectKey > 4, never reaches here!";
			cout << endl;
			idx = idx + len;

		} else if (len > 0) {
			if (len == 4) {
				objectKey = ((data[idx] & 0xFF) << 24) |
						((data[idx+1] & 0xFF) << 16) |
						((data[idx+2] & 0xFF) << 8) |
						(data[idx+3] & 0xFF);

			} else if (len == 3) {
				objectKey = ((data[idx] & 0xFF) << 16) |
			    	    ((data[idx+1] & 0xFF) << 8) |
			    	    (data[idx+2] & 0xFF);

			} else if (len == 2) {
				objectKey = ((data[idx] & 0xFF) << 8) | (data[idx+1] & 0xFF);

			} else {
				objectKey = (data[idx] & 0xFF);
			}

			cout << "biop objectkey = " << objectKey << endl;
			idx = idx + len;
		}

		// objectKind
		len = ((data[idx] & 0xFF) << 24) | ((data[idx+1] & 0xFF) << 16) |
			    ((data[idx+2] & 0xFF) << 8) | (data[idx+3] & 0xFF);

		idx = idx + 4;

		if (len > 0) {
			field = (char*)malloc(len);
			memcpy(field, (void*)&(data[idx]), len);
			this->objectKind = (string)field;
			free(field);
			idx = idx + len;
			cout << "objectKind: " << objectKind << endl;

		} else {
			cout << "Warning! No kind never reaches here!" << endl;
			objectKind = "";
		}

		// objectInfo
		len = ((data[idx] & 0xFF) << 8) | (data[idx+1] & 0xFF);
		idx = idx + 2;

		if (len > 0) {
			field = (char*)malloc(len);
			memcpy(field, (void*)&(data[idx]), len);
			objectInfo = (string)field;
			free(field);
			cout << "objectInfo: " << objectInfo << endl;
			idx = idx + len;

		} else {
			objectInfo = "";
		}

		// skip last SubHeader field, named service_context
		idx++;
	}

	void Biop::skipObject() {
		unsigned int len;

		cout << "Biop::skipObject" << endl;

		// size of messageBody
		len = ((data[idx] & 0xFF) << 24) | ((data[idx+1] & 0xFF) << 16)
			    | ((data[idx+2] & 0xFF) << 8) | (data[idx+3] & 0xFF);

		idx = idx + 4;

		//skip
		idx = idx + len;

		if (hasMoreBiopMessage) {
			isValidHdr = processMessageHeader();
			if (isValidHdr) {
				processMessageSubHeader();
			}
		}
	}

	void Biop::processServiceGateway(unsigned int srgObjectKey) {
		unsigned int i, len;
		Binding* binding;
		Object* carouselObject;

		cout << "Biop::processServiceGateway" << endl;
		if (!isValidHdr) {
			cout << "BIOP process SRG Warning! Invalid HDR" << endl;
			return;

		} else {
			processMessageSubHeader();
		}

		carouselObject = new Object();

		while (objectKey != srgObjectKey) {
			skipObject();
			cout << "currentObject key = " << objectKey << endl;
		}

		carouselObject->setCarouselId(module->getCarouselId());
		carouselObject->setModuleId(module->getId());
		carouselObject->setKey(objectKey);
		carouselObject->setKind(objectKind);

		// size of messageBody
		len = ((data[idx] & 0xFF) << 24) | ((data[idx+1] & 0xFF) << 16)
			    | ((data[idx+2] & 0xFF) << 8) | (data[idx+3] & 0xFF);

		idx = idx + 4;

		//get Number of bindings
		len = (data[idx] & 0xFF) << 8 | (data[idx+1] & 0xFF);
		idx = idx + 2;
		cout << "Number of bindings = " << len << endl;

		for (i = 0; i < len; i++) {
			binding = processBinding();
			carouselObject->addBinding(binding);
		}

		processor->pushObject(carouselObject);
	}

	Binding* Biop::processBinding() {
		Binding* binding;
		binding = new Binding();

		unsigned int numberOfComponents, len;
		char* field;

		cout << "Biop::processingBinding" << endl;

		numberOfComponents = (data[idx] & 0xFF);
		idx++;
		cout << "Number of components = " << numberOfComponents << endl;

		if (numberOfComponents > 1)
			cout << "Warning: numberOfComponents, Never reach here!!! NOC = "
				    << numberOfComponents << endl;

		//binding
		//id_length
		len = (data[idx] & 0xFF);
		idx++;

		//id
		field = (char*)malloc(len);
		memcpy(field, (void*)&(data[idx]), len);
		binding->setId((string)field);
		free(field);
		field = NULL;
		cout << "bindingId = " << binding->getId() << endl;
		idx = idx + len;

		//kind_length
		len = (data[idx] & 0xFF);
		idx++;

		//kind
		field = (char*)malloc(len);
		memcpy(field, (void*)&(data[idx]), len);
		binding->setKind((string)field);
		free(field);
		cout << "bindingKind = " << binding->getKind() << endl;
		idx = idx + len;

		//bindingType
		len = (data[idx] & 0xFF);
		binding->setType(len);
		cout << "bindingType = " << binding->getType() << endl;
		idx++;

		processIor(binding);

		//objectInfo
		len = ((data[idx] & 0xFF ) << 8) | (data[idx+1] & 0xFF);
		idx = idx + 2;

		if (len > 0) {
			field = (char*)malloc(len);
			memcpy(field, (void*)&(data[idx]), len);
			binding->setObjectInfo((string)field);
			free(field);
			cout << "bindingsObjectInfo = " << binding->getObjectInfo() << endl;
			idx = idx + len;
		}

		return binding;
	}

	void Biop::processIor(Binding* binding) {
		Ior* ior;
		unsigned int len, n1;
		char* field;

		ior = new Ior();

		cout << "Biop::processIor " << endl;

		//type_id_length
		n1 = ((data[idx] & 0xFF) << 24) | ((data[idx+1] & 0xFF) << 16) |
			    ((data[idx+2] & 0xFF) << 8)  | (data[idx+3] & 0xFF);

		idx = idx + 4;

		//type_id
		field = (char*)malloc(n1);
		memcpy(field, (void*)&(data[idx]), n1);
		ior->setTypeId((string)field);
		free(field);
		cout << "Ior typeId = " << ior->getTypeId() << endl;
		idx = idx + n1;

		//CDR alignment rule
		if (n1 % 4 != 0) {
			for (unsigned int i = 0; i < (4 - (n1 % 4)); i++) {
				if ((data[idx] & 0xFF) != 0xFF) {
					cout << "CDR alignment Warning! gap must be 0xFF" << endl;
				}
				idx++;
			}
		}

		//check Number of TaggedProfiles (_count)
		len = ((data[idx] & 0xFF) << 24) | ((data[idx+1] & 0xFF) << 16) |
			    ((data[idx+2] & 0xFF) << 8) | (data[idx+3] & 0xFF);

		idx = idx + 4;
		if (len > 1) {
			cout << "Warning: TaggedProfiles, never reach here!!! TP = ";
			cout << len << endl;
		}

		//get ior profile tag and check it
		len = ((data[idx] & 0xFF) << 24) | ((data[idx+1] & 0xFF) << 16) |
			    ((data[idx+2] & 0xFF) << 8) | (data[idx+3] & 0xFF);

		idx = idx + 4;
		if (len != TAG_BIOP) {
			cout << "Warning: TAG_BIOP, never reach here!!!";
			cout << " TAG = " << len << endl;

		} else {
			// BIOP Profile Body
			// skip size of body
			len = ((data[idx] & 0xFF) << 24) | ((data[idx+1] & 0xFF) << 16) |
				    ((data[idx+2] & 0xFF) << 8) | (data[idx+3] & 0xFF);

			idx = idx + 4;
			cout << "Biop profile body size '" << len << "'" << endl;

			// skip byte_order field
			idx++;

			/*
			 * check number of liteComponents
			 * shall have objectLocation and connBinder
			 */
			len = (data[idx] & 0xFF);
			if (len != 2) {
				cout << "Warning: liteComponents, never reach here!!! LC = ";
				cout << len << endl;
			}
			idx++;

			// objectLocation
			// check if tag == "TAG_ObjectLocation"
			len = ((data[idx] & 0xFF) << 24) | ((data[idx+1] & 0xFF) << 16) |
				    ((data[idx+2] & 0xFF) << 8) | (data[idx+3] & 0xFF);

			idx = idx + 4;

			if (len != TAG_BIOP_OBJECT_LOCATION) {
				cout << "Warning: TAG_ObjectLocation, never reach here!!!";
				cout << " TAG = " << len << endl;

			} else {
				// skip size of object data, we don't need it.
				idx++;

				//carousel_id
				len = ((data[idx] & 0xFF) << 24) |
						((data[idx+1] & 0xFF) << 16) |
						((data[idx+2] & 0xFF) << 8) |
						(data[idx+3] & 0xFF);

				ior->setCarouselId(len);
				cout << "Ior carrouselID = " << ior->getCarouselId() << endl;
				idx = idx + 4;

				//module_id
				len = ((data[idx] & 0xFF) << 8) | (data[idx+1] & 0xFF);
				ior->setModuleId(len);
				cout << "Ior moduleId = " << ior->getModuleId() << endl;
				idx = idx + 2;

				//check version: major == 1 && minor == 0
				if (((data[idx] & 0xFF) != 0x01) ||
						((data[idx+1] & 0xFF) != 0x00)) {

					cout << "BIOP Warning: Obj version, never reach here!!!";
					cout << endl;

					cout << "current size '" << hex << currentSize;
					cout << "' current ix '";
					cout << hex << idx << "'" << endl;
				}

				idx = idx + 2;

				//objectKey_len
				len = (data[idx] & 0xFF);
				idx++;

				// if len > 4 then the unsigned int is not larger enough
				if (len > 4) {
					cout << "Warning! Size of";
					cout << " objectKey > 4, never reaches here!";
					cout << endl;

				} else if (len > 0) {
					unsigned int key;
					if (len == 4) {
						key = ((data[idx] & 0xFF) << 24) |
							    ((data[idx+1] & 0xFF) << 16) |
							    ((data[idx+2] & 0xFF) << 8) |
							    (data[idx+3] & 0xFF);

					} else if (len == 3) {
						key = ((data[idx] & 0xFF) << 16) |
					    	    ((data[idx+1] & 0xFF) << 8) |
					    	    (data[idx+2] & 0xFF);

					} else if (len == 2) {
						key = ((data[idx] & 0xFF) << 8) | (data[idx+1] & 0xFF);

					} else {
						key = (data[idx] & 0xFF);
					}
					cout << "Ior objectkey = " << key << endl;
					ior->setObjectKey(key);
				}
				idx = idx + len;
			}

			// connBinder
			// TODO: it will be Usefull?
			//skip connBinder TAG
			idx = idx + 4;

			//get size of connBinder to skip the rest of it
			len = (data[idx] & 0xFF);
			idx++;

			idx = idx + len;
		}
		binding->setIor(ior);
	}

	void Biop::processDirectory() {
		unsigned int i, len;
		Binding* binding;
		Object* carouselObject;

		cout << "Biop::processDirectory = " << endl;

		carouselObject = new Object();
		carouselObject->setCarouselId(module->getCarouselId());
		carouselObject->setModuleId(module->getId());
		carouselObject->setKey(objectKey);
		carouselObject->setKind(objectKind);

		//skip size of messageBody
		idx = idx + 4;

		//get Number of bindings
		len = ((data[idx] & 0xFF) << 8) | (data[idx+1] & 0xFF);
		idx = idx + 2;
		cout << "Number of bindings = " << len << endl;

		for (i = 0; i < len; i++) {
			binding = processBinding();
			carouselObject->addBinding(binding);
		}

		processor->pushObject(carouselObject);
	}

	void Biop::processFile() {
		unsigned int len;
		Object* carouselObject;

		cout << "BIOP processFIL" << endl;

		carouselObject = new Object();
		carouselObject->setCarouselId(module->getCarouselId());
		carouselObject->setModuleId(module->getId());
		carouselObject->setKey(objectKey);
		carouselObject->setKind(objectKind);

		//skip size of messageBody
		idx = idx + 4;

		//get size of file
		len = ((data[idx] & 0xFF) << 24) | ((data[idx+1] & 0xFF) << 16) |
			    ((data[idx+2] & 0xFF) << 8) | (data[idx+3] & 0xFF);

		idx = idx + 4;

		//get file data
		char* fileData;

		fileData = (char*)malloc(len);
		memcpy((void*)&(fileData[0]), (void*)&(data[idx]), len);
		carouselObject->setData(fileData);
		carouselObject->setDataSize(len);

		processor->pushObject(carouselObject);

		//skip file data
		idx = idx + len;
	}

	void Biop::print() {
		cout << "BIOP" << endl;
		cout << "objectKind = " << objectKind.c_str() << endl;
		cout << "objectInfo = " << objectInfo.c_str() << endl;
	}

	void Biop::processObject() {
		if (objectKind == "srg" || objectKind == "DSM::ServiceGateway") {
			skipObject();
			if (objectKind == "fil" || objectKind == "DSM::File") {
				processFile();

			} else if (objectKind == "dir" || objectKind == "DSM::Directory") {
				processDirectory();
			}

		} else if (objectKind == "fil" || objectKind == "DSM::File") {
			processFile();

		} else if (objectKind == "dir" || objectKind == "DSM::Directory") {
			processDirectory();
		}
	}

	void Biop::process() {
		bool processed;

		cout << "Biop::process" << endl;
		if (moduleFd >= 0) {
			processed = false;
			do {
				if (isValidHdr) {
					cout << "Biop::process processing sub hdr" << endl;
					processMessageSubHeader();
					cout << "Biop::process processing obj" << endl;
					processObject();
				}

				if (hasMoreBiopMessage) {
					cout << "Biop::process processing has more msgs" << endl;
					if (data != NULL && sizeof(data) > 0) {
						free(data);
						data = NULL;
					}

					cout << "processing another BIOP message from file ";
					cout << module->getModuleFileName() << endl;

					isValidHdr = processMessageHeader();

				} else {
					cout << "Biop::process processing done" << endl;
					processed = true;
				}

			} while (!processed);

			if (data != NULL && sizeof(data) > 0) {
				free(data);
				data = NULL;
			}

			close(moduleFd);
		}
	}

	map<string, Object*>* Biop::getObjects() {
		return objects;
	}
}
}
}
}
}
}
}
