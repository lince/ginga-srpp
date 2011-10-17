#ifndef ICONTEXTPROVIDER_H
#define ICONTEXTPROVIDER_H

namespace br{
namespace ufscar{
namespace lince{
namespace ginga{
namespace wac{
namespace state{

class IContextProvider {

public:
	virtual ~IContextProvider() {};
	virtual vector<string>* getPropertyNames()=0;
	virtual string getPropertyValue(string attributeId)=0;

};

}
}
}
}
}
}


#endif //ICONTEXTPROVIDER_H


