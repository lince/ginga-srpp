#include "../include/User.h"

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
  
		namespace recommender {
		
				    int User::idUser=1;
				    //string User::ID="1";		    
	      
//				    User::User(string userGenre, string locationZip, string locationLatitude, string locationLongitude, 
//								  string userBirth, Device* device, string docId, string filename){
				    User::User(string ID, string userGenre, string locationZip, string locationLatitude, string locationLongitude, 
								  string userBirth, string docId, string filename){
			    	
							this->ID = ID;
						    this->userGenre = userGenre;
						    this->locationZip = locationZip;
						    this->locationLatitude = locationLatitude;
						    this->locationLongitude = locationLongitude;
						    this->userBirth = userBirth;
						   // this->device = device;
						    this->docId = docId;
						    this->filename = filename;
				    }

				    string User::generateSql() {
						Utilitario* util = Utilitario::getInstance();
						return "insert into users (idRefUser,genre, zip, lat, long, age, country,idDevice) values (" +
						  ID + ", \"" +					
						  userGenre + "\", " + 
						  locationZip + ", " + 
						  locationLatitude + ", " + 
						  locationLongitude + ", " + 
						  userBirth + ", 0, \"Brazil\" );";
				    }
				    
				    string User::generateSql2() {
						Utilitario* util = Utilitario::getInstance();
						return "delete from users where idRefUser = " +
						  ID + " ;";
				    }

				    string User::generateSql3() {
						Utilitario* util = Utilitario::getInstance();
						return "select idUser from users where idRefUser = " +
						  ID + " ;";
				    }
				    
				    string User::generateSql4() {
						Utilitario* util = Utilitario::getInstance();
						return "select MAX(idUser) from users;";
				    }
				    
				    string User::generateSql5(int id) {
						Utilitario* util = Utilitario::getInstance();
						return "select idRefUser, genre, zip, age from users where idUser = " + util->IntToString(id) +" ;";
				    }			    
				    
					//User* user = new User (ID,userGenre,locationZip,"0","0",age,"--","--");				    
				    
				    
				    int User::getIdUser(){return idUser;}
				    void User::setIdUser(int id){this->idUser = id;}
				    void User::incIdUser(){this->idUser++;}
				    
				    string User::getID(){return ID;};
				    void User::setID(string ID){this->ID = ID;};	      

				    string User::getUserGenre(){return userGenre;}
				    void User::setUserGenre(string userGenre){this->userGenre = userGenre;}

				    string User::getLocationZip(){return locationZip;}
				    void User::setLocationZip(string locationZip){this->locationZip = locationZip;}

				    string User::getLocationLatitude(){return locationLatitude;}
				    void User::setLocationLatitude(string locationLatitude){this->locationLatitude = locationLatitude;}

				    string User::getLocationLongitude(){return locationLongitude;}
				    void User::setLocationLongitude(string locationLongitude){this->locationLongitude = locationLongitude;}

				    string User::getUserBirth(){return userBirth;}
				    void User::setUserBirth(string userBirth){this->userBirth = userBirth;}

				    Device* User::getDevice(){return device;}
				    void User::setDevice(Device* device) {this->device = device;}

				    string User::getDocId(){return docId;}
				    void User::setDocId(string docId){this->docId = docId;}

				    string User::getFilename(){return filename;}
				    void User::setFilename(string filename){this->filename = filename;}
				    
				    				    

				    };


			}
		}
	}
}				

