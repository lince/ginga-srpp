#include "../include/createXML.h"

string getValue(string varAmbiente) {
	IComponentManager* cm = IComponentManager::getCMInstance();
	IContextManager* cmng = ((ContextManagerCreator*)(cm->getObject("ContextManager")))();
	map<string, string>* mapa = cmng->getUserProfile(cmng->getCurrentUserId());

	map<string, string>::iterator i;
	if (mapa != NULL) {
		i = mapa->find(varAmbiente);
		if (i != mapa->end()) {
			return i->second;
		}
	}
	return "";
}

int exportXML(string path)
{
	int errorCode = 0;

	// ---------------------------------------------------------------------------
	//  Inicializacao do Xerces
	// ---------------------------------------------------------------------------
    try
    {
        XMLPlatformUtils::Initialize();
    }

    catch(const XMLException& toCatch)
    {
         char *pMsg = XMLString::transcode(toCatch.getMessage());
         XERCES_STD_QUALIFIER cerr << "Error during Xerces-c Initialization.\n"
              << "  Exception message:"
              << pMsg;
         XMLString::release(&pMsg);
         return 1; 
     }

    // ---------------------------------------------------------------------------
    //  Inicializacao da geracao da arvore DOM
    // ---------------------------------------------------------------------------
    DOMImplementation* impl =  DOMImplementationRegistry::getDOMImplementation(X("Core"));

    if (impl != NULL)
            {
                try
                {
                    DOMDocument* doc = impl->createDocument(
                                0,                    // root element namespace URI.
                                X("watchTV"),         // root element name
                                0);                   // document type object (DTD).
                                
                    Database* db = new Database("", "", "/usr/local/etc/ginga/files/recommender/iteraction.db");
					
		    if(db->createDatabase()) {
	
				db->query("SELECT u.age, u.genre, u.idUser FROM users AS u;", &head3, &data3);
				cout << "!!!!!!!!!!!!!!!!Antes do iffffffffffffffffffff" << endl;
				if(!data3.empty()) {
					cols3 = (int)head3.size();						
					cout << "!!!!!!!!!!!!!!!!Entrei hei hei hei 1" << endl;
					rows3 = (int)data3.size()/cols3;
					cout << "!!!!!!!!!!!!!!!!Entrei hei hei hei 2" << endl;
				}			
				else {
					cerr << "Database is empty!" << endl;
				}
			}
		    else {
			cerr << "Database doesn't exist" << endl;
		    }


		    //Gerar o startDate =/

                    // <watchTV country="Brazil" startDate="2010-06-16T12:00:00.0" endDate="2010-06-16T17:30:11.1">
                    DOMElement* rootElem = doc->getDocumentElement();
                    rootElem->setAttribute(X("country"), X(getValue("system.country").c_str()));
        			rootElem->setAttribute(X("startDate"), X(getValue("default.startSession").c_str()));
                    //rootElem->setAttribute(X("startDate"), X(data[1].c_str()));
                    Utilitario* util = Utilitario::getInstance();
                    string time = util->getAno() + "/" + util->getMes() + "/" + util->getDia() + "T" + util->getHora() + ":" + util->getMinuto() + ":" + util->getSegundo();
                    rootElem->setAttribute(X("endDate"), X(time.c_str()));
                    // <head>
                    DOMElement*  headElem = doc->createElement(X("head"));
                    rootElem->appendChild(headElem);

                    // <location zip="36035330" lat="-21.455100" long="-43.210100"/>
                    DOMElement*  locationElem = doc->createElement(X("location"));
                    locationElem->setAttribute(X("zip"), X(getValue("system.location").c_str()));
                    locationElem->setAttribute(X("lat"), X(getValue("system.latitude").c_str()));
                    locationElem->setAttribute(X("long"), X(getValue("system.longitude").c_str()));
                    headElem->appendChild(locationElem);   //------------------> ISSO TAH DENTRO DO HEAD

                    //<device serialNumber="20020293xs" type="set-top-box" profile="fullseg">
                    DOMElement* deviceElem = doc->createElement(X("device"));
                    deviceElem->setAttribute(X("serialNumber"), X(getValue("system.serial").c_str()));
                    deviceElem->setAttribute(X("type"), X(getValue("system.type").c_str()));
                    deviceElem->setAttribute(X("profile"), X(getValue("system.profile").c_str()));
                    headElem->appendChild(deviceElem);

                    //<resources>
                    DOMElement* resourcesElem = doc->createElement(X("resources"));
                    deviceElem->appendChild(resourcesElem);

                    //<resource type="network">
                    DOMElement* resourceElem = doc->createElement(X("resource"));
                    resourceElem->setAttribute(X("type"), X("network"));
                    resourcesElem->appendChild(resourceElem);

                    //<information name="velox" value="512Mbps"/>
					//<information name="model" value="Realtek RTL8139/810x Family FastEthernet"/>
                    DOMElement* infoElem = doc->createElement(X("information"));
                    infoElem->setAttribute(X("name"), X(getValue("system.net.name").c_str()));
                    infoElem->setAttribute(X("value"), X(getValue("system.net.velocidade").c_str()));
                    resourceElem->appendChild(infoElem);
                    
		    infoElem = doc->createElement(X("information"));
                    infoElem->setAttribute(X("name"), X("model"));
                    infoElem->setAttribute(X("value"), X(getValue("system.net.model").c_str()));
		    resourceElem->appendChild(infoElem);
					
					//<resource type="storage">
                    resourceElem = doc->createElement(X("resource"));
                    resourceElem->setAttribute(X("type"), X("storage"));
                    resourcesElem->appendChild(resourceElem);

                    //<information name="total" value="200GB"/>
		    //<information name="free" value="100GB"/>
                    infoElem = doc->createElement(X("information"));
                    infoElem->setAttribute(X("name"), X("total"));
                    infoElem->setAttribute(X("value"), X(getValue("system.sto.total").c_str()));
                    resourceElem->appendChild(infoElem);
                    infoElem = doc->createElement(X("information"));
                    infoElem->setAttribute(X("name"), X("free"));
                    infoElem->setAttribute(X("value"), X(getValue("system.sto.free").c_str()));
		    resourceElem->appendChild(infoElem);
					
		    //<resource type="cpu">
                    resourceElem = doc->createElement(X("resource"));
                    resourceElem->setAttribute(X("type"), X("cpu"));
                    resourcesElem->appendChild(resourceElem);

                    //<information name="model" value="Intel Core 4 Duo"/>
					//<information name="clock" value="2.2GHz"/>
					//<information name="cores" value="4"/>
					//<information name="arch" value="64bits"/>
                    infoElem = doc->createElement(X("information"));
                    infoElem->setAttribute(X("name"), X("model"));
                    infoElem->setAttribute(X("value"), X(getValue("system.cpu.model").c_str()));
                    resourceElem->appendChild(infoElem);
                    infoElem = doc->createElement(X("information"));
                    infoElem->setAttribute(X("name"), X("clock"));
                    infoElem->setAttribute(X("value"), X(getValue("system.cpu.clock").c_str()));
		    resourceElem->appendChild(infoElem);
		    infoElem = doc->createElement(X("information"));
                    infoElem->setAttribute(X("name"), X("cores"));
                    infoElem->setAttribute(X("value"), X(getValue("system.cpu.cores").c_str()));
                    resourceElem->appendChild(infoElem);
                    infoElem = doc->createElement(X("information"));
                    infoElem->setAttribute(X("name"), X("arch"));
                    infoElem->setAttribute(X("value"), X(getValue("system.cpu.arch").c_str()));
		    resourceElem->appendChild(infoElem);
					
					//<resource type="hid">
                    resourceElem = doc->createElement(X("resource"));
                    resourceElem->setAttribute(X("type"), X("hid"));
                    resourcesElem->appendChild(resourceElem);

                    //<information name="model" value="Hp Touch"/>
		    //<information name="type" value="tablet"/>
                    infoElem = doc->createElement(X("information"));
                    infoElem->setAttribute(X("name"), X("model"));
                    infoElem->setAttribute(X("value"), X(getValue("system.hid.model").c_str()));
                    resourceElem->appendChild(infoElem);
                    infoElem = doc->createElement(X("information"));
                    infoElem->setAttribute(X("name"), X("type"));
                    infoElem->setAttribute(X("value"), X(getValue("system.hid.type").c_str()));
		    resourceElem->appendChild(infoElem);
					
		    //<attachedDevices>
                    DOMElement* attachedElem = doc->createElement(X("attachedDevices"));
                    deviceElem->appendChild(attachedElem);
                    
                    DOMElement* adeviceElem = doc->createElement(X("device"));
                    adeviceElem->setAttribute(X("type"), X(getValue("system.att.interaction").c_str()));
                    attachedElem->appendChild(adeviceElem);
                    adeviceElem = doc->createElement(X("device"));
                    adeviceElem->setAttribute(X("type"), X(getValue("system.att.storage").c_str()));
                    attachedElem->appendChild(adeviceElem);

                    //<!-- Fim Head -->
        			cout << "!!!!!!!!!! 9" << endl;

		    for (int k=0;k<rows3;k++){
			  int l=0;
				
			  //<user birth="01-02-1972" genre="male">
			  DOMElement* userElem = doc->createElement(X("user"));
      //                    userElem->setAttribute(X("birth"), X(getValue("user.birth").c_str()));
      //                    userElem->setAttribute(X("genre"), X(getValue("user.genre").c_str()));
			  userElem->setAttribute(X("birth"), X(data3[k*cols3 + l++].c_str()));
			  userElem->setAttribute(X("genre"), X(data3[k*cols3 + l++].c_str()));
 			  rootElem->appendChild(userElem); //---------------------------> USUARIO AGORA VAI FAZER PARTE DO PRINCIPAL

			  //<socialNetwork name="orkut" identification="orkuteduardo"/>
					      //<socialNetwork name="facebook" identification="facebookeduardo"/>
			  DOMElement* socialElem = doc->createElement(X("socialNetwork"));
			  socialElem->setAttribute(X("name"), X("orkut"));
			  socialElem->setAttribute(X("identification"), X(getValue("user.sn.orkut").c_str()));
			  userElem->appendChild(socialElem);
			  socialElem = doc->createElement(X("socialNetwork"));
			  socialElem->setAttribute(X("name"), X("facebook"));
			  socialElem->setAttribute(X("identification"), X(getValue("user.sn.facebook").c_str()));
			  userElem->appendChild(socialElem);

			  //--------------------------> AKI COMEÇA A PESQUISA DOS ITERACTIONS

			    string qrySql = "SELECT i.type, i.time, k.code, k.action, c.code, c.name, p.code, p.name, p.genre, p.age, v.level, "
			    "v.mute, i.idIteraction FROM iteraction AS i, key AS k, channel AS c, program AS p, "
			    "volume AS v WHERE i.idProgram = p.idProgram AND i.idVolume = v.idVolume AND i.idKey = k.idKey "
			    "AND p.idChannel = c.idChannel AND i.idUi = "; //tirei essa parte do final
			  qrySql += data3[k*cols3 + l++] + " ;";
			  db->query(qrySql.c_str(), &head, &data);

			  if(!data.empty()) {
				  cols = (int)head.size();						
				  rows = (int)data.size()/cols;
			  }

			  DOMElement* interactionElem;
			  DOMElement* keyElem;
			  DOMElement* chElem;
			  DOMElement* progElem;
			  DOMElement* volElem;
			  DOMElement* nclStateElem;
			  DOMElement* docElem;
			  DOMElement* mediaElem;
			  DOMElement* contextElem;
			  for(int i = 0; i < rows; i++) {
			      int j = 0;
			      //<interaction type="channelChange" time="2010-06-16T12:00:02.5">
			      interactionElem = doc->createElement(X("interaction"));
			      interactionElem->setAttribute(X("type"), X(data[i*cols + j++].c_str()));
			      interactionElem->setAttribute(X("time"), X(data[i*cols + j++].c_str()));
			      rootElem->appendChild(interactionElem);
			      // <key code="CH_UP"/>
			      keyElem = doc->createElement(X("key"));
			      keyElem->setAttribute(X("code"), X(data[i*cols + j++].c_str()));
			      keyElem->setAttribute(X("action"), X(data[i*cols + j++].c_str()));
			      interactionElem->appendChild(keyElem);
			      //<channel code="20" name="SporTV">
			      chElem = doc->createElement(X("channel"));
			      chElem->setAttribute(X("code"), X(data[i*cols + j++].c_str()));
			      chElem->setAttribute(X("name"), X(data[i*cols + j++].c_str()));
			      interactionElem->appendChild(chElem);
			      //<program code="452" name="Gols da Rodada" category="Esportes" age="10">
			      progElem = doc->createElement(X("program"));
			      progElem->setAttribute(X("code"), X(data[i*cols + j++].c_str()));
			      progElem->setAttribute(X("name"), X(data[i*cols + j++].c_str()));
			      progElem->setAttribute(X("category"), X(data[i*cols + j++].c_str()));
			      progElem->setAttribute(X("age"), X(data[i*cols + j++].c_str()));
			      chElem->appendChild(progElem);
			      //<volume level="15" mute="no" />
			      volElem = doc->createElement(X("volume"));
			      volElem->setAttribute(X("level"), X(data[i*cols + j++].c_str()));
			      volElem->setAttribute(X("mute"), X(data[i*cols + j++].c_str()));
			      interactionElem->appendChild(volElem);
			      /* Inicio nclStateMachine */
			      if(data[i*cols] == "interaction") {
				    string nclSql = "SELECT d.idDocument, d.id FROM document AS d, nclstatemachine AS n WHERE d.idNclStateMachine = n.idNclStateMachine AND n.idIteraction = " + data[i*cols + j++] + ";";
				    db->query(nclSql.c_str(), &head2, &data2);
				    int document = atoi(data2[0].c_str());
				    if(!data2.empty()) {
					  nclStateElem = doc->createElement(X("nclStateMachine"));
					  interactionElem->appendChild(nclStateElem);
					  docElem = doc->createElement(X("document"));
					  docElem->setAttribute(X("id"), X(data2[1].c_str()));
					  nclStateElem->appendChild(docElem);
					  data2.clear();
					  head2.clear();
					  //algoritmo context/medias
					  string ctxSql = "SELECT c.idContext, c.id, c.idSContext FROM context AS c WHERE c.idDocument = " + util->IntToString(document) + ";";
					  db->query(ctxSql.c_str(), &head2, &data2);
					  if(!data2.empty()) {
					      int context = 0;
					      int cont = 0;
					      int n_context = data2.size()/head2.size();
					      while(cont < n_context) { //loop nos contextos
						      vector<string> headm;
						      vector<string> medias;
						      contextElem = doc->createElement(X("context"));
						      contextElem->setAttribute(X("id"), X(data2[cont*head2.size() + 1].c_str()));
						      docElem->appendChild(contextElem);
						      string mediaSql = "SELECT m.id, m.status, m.time, m.idContext FROM media AS m WHERE m.idDocument = " + util->IntToString(document) + " AND m.idContext = " + data2[cont*head2.size()] + ";";
						      db->query(mediaSql.c_str(), &headm, &medias);
						      if(!medias.empty()) {
							      cols2 = headm.size();
							      rows2 = medias.size()/cols2;
							      for(int n = 0; n < rows2; n++) {
								      int m = 0;
								      mediaElem = doc->createElement(X("media"));
								      mediaElem->setAttribute(X("id"), X(medias[n*cols2 + m++].c_str()));
								      mediaElem->setAttribute(X("status"), X(medias[n*cols2 + m++].c_str()));
								      mediaElem->setAttribute(X("time"), X(medias[n*cols2 + m++].c_str()));
								      m++;
								      contextElem->appendChild(mediaElem);
							      }
						      }
						      headm.clear();
						      medias.clear();
						      cont++;
					      }
					      head2.clear();
					      data2.clear();
					  }
					  else {
					      vector<string> headm;
					      vector<string> medias;
					      string mediaSql = "SELECT m.id, m.status, m.time, m.idContext FROM media AS m WHERE m.idDocument = " + util->IntToString(document) + ";";
					      db->query(mediaSql.c_str(), &headm, &medias);
					      if(!medias.empty()) {
						      cols2 = headm.size();
						      rows2 = medias.size()/cols2;
						      for(int n = 0; n < rows2; n++) {
							    int m = 0;
							    if(atoi(medias[n*cols2 + 3].c_str()) == 0) {
								    mediaElem = doc->createElement(X("media"));
								    mediaElem->setAttribute(X("id"), X(medias[n*cols2 + m++].c_str()));
								    mediaElem->setAttribute(X("status"), X(medias[n*cols2 + m++].c_str()));
								    mediaElem->setAttribute(X("time"), X(medias[n*cols2 + m++].c_str()));
								    docElem->appendChild(mediaElem);
							    }
						      }
					      }
					      headm.clear();
					      medias.clear();
					  }
				      }
			      }
			  }
		    }
		    db->closeDatabase();

                    // Contagem dos nos da arvore. Teste apenas!
                    const XMLSize_t elementCount = doc->getElementsByTagName(X("*"))->getLength();
                        XERCES_STD_QUALIFIER cout << "The tree just created contains: " << elementCount
                             << " elements." << XERCES_STD_QUALIFIER endl;


                    // Salvar o arquivo XML!

                    DOMWriter *theSerializer = ((DOMImplementationLS*)impl)->createDOMWriter();

                    // set user specified output encoding
                   theSerializer->setEncoding(gOutputEncoding);

                   XMLFormatTarget *myFormTarget;
                   if (path != "")
                       myFormTarget = new LocalFileFormatTarget(path.c_str());
                   else
                	   myFormTarget = new StdOutFormatTarget();

                   theSerializer->writeNode(myFormTarget, *doc);

                   delete theSerializer;

                    // Liberacao da arvore - ao fim do processamento
                    doc->release();
                }
                //Exception - falta de memoria
                catch (const OutOfMemoryException&)
                {
                    XERCES_STD_QUALIFIER cerr << "OutOfMemoryException" << XERCES_STD_QUALIFIER endl;
                    errorCode = 5;
                }
                //Exception - DOM
                catch (const DOMException& e)
                {
                    XERCES_STD_QUALIFIER cerr << "DOMException code is:  " << e.code << XERCES_STD_QUALIFIER endl;
                    errorCode = 2;
                }
                //Exception Generica
                catch (...)
                {
                    XERCES_STD_QUALIFIER cerr << "An error occurred creating the document" << XERCES_STD_QUALIFIER endl;
                    errorCode = 3;
                }
            }  // (inpl != NULL)
            else
            {
                XERCES_STD_QUALIFIER cerr << "Requested implementation is not supported" << XERCES_STD_QUALIFIER endl;
                errorCode = 4;
            }

    	XMLPlatformUtils::Terminate();
		return errorCode;

    }
