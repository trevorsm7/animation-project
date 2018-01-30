#ifndef __BASEJOINT_H__
#define __BASEJOINT_H__

#include <iostream>
#include <string>
#include <vector>

#include "ParserProperty.h"
#include "JointDOF.h"
#include "Matrix4.h"

class BaseJoint;
typedef std::shared_ptr<BaseJoint> JointPtr;

class BaseJoint
{
protected:
    Matrix4 m_localMatrix;
    Matrix4 m_worldMatrix;
    Matrix4 m_cubeMatrix;
    std::string m_name;
    Vector3 m_offset;
    std::vector<JointPtr> m_children;
    std::vector<JointDOFPtr> m_dofList;
    
public:
    virtual ~BaseJoint() {}
    
    void parse(std::istream& in, std::vector<JointPtr>& list);
    void print(std::ostream& out, int level);
    
    // Common matrix code; calls updateLocal of subclass
    void update(Matrix4& parent);
    Matrix4& getWorldMatrix() {return m_worldMatrix;}
    Matrix4& getCubeMatrix() {return m_cubeMatrix;}
    
    std::vector<JointDOFPtr>& getDOFList() {return m_dofList;}
    
protected:
    virtual void preParse(ParserPropertyMap& properties) = 0;
    virtual void postParse(ParserPropertyMap& properties) = 0;
    virtual void updateLocal() = 0;
};

#endif