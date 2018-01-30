#include "JointFactory.h"
#include "BallJoint.h"

JointFactory JointFactory::factory;

JointFactory::JointFactory()
{
    m_creators["balljoint"] = ballJointCreator;
}

JointPtr JointFactory::create(std::string type)
{
    auto it = factory.m_creators.find(type);
    if (it != factory.m_creators.end())
    {
        JointCreator creator = it->second;
        return creator();
    }
    return JointPtr();
}

JointPtr JointFactory::ballJointCreator()
{
    return JointPtr(new BallJoint());
}