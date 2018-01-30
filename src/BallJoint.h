#ifndef __BALLJOINT_H__
#define __BALLJOINT_H__

#include "BaseJoint.h"
#include "ParserProperty.h"

class BallJoint : public BaseJoint
{
protected:
    virtual void preParse(ParserPropertyMap& properties) override;
    virtual void postParse(ParserPropertyMap& properties) override;
    virtual void updateLocal() override;
};

#endif