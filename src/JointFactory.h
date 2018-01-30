#ifndef __JOINTFACTORY_H__
#define __JOINTFACTORY_H__

#include <string>
#include <map>

#include "BaseJoint.h"

class JointFactory
{
protected:
    typedef JointPtr (*JointCreator)(void);
    std::map<std::string, JointCreator> m_creators;
    static JointFactory factory;
    
public:
    static JointPtr create(std::string type);
    
protected:
    JointFactory();
    static JointPtr ballJointCreator();
};

#endif