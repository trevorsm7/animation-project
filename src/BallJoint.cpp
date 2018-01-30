#include "BallJoint.h"
#include "Vector3.h"
#include "Matrix4.h"

void BallJoint::preParse(ParserPropertyMap& properties)
{
    // Define the properties that can be loaded from file and their defaults
    properties["offset"] = ParserPropertyPtr(new FloatVector(0.0f, 0.0f, 0.0f));
    properties["boxmin"] = ParserPropertyPtr(new FloatVector(-0.5f, -0.5f, -0.5f));
    properties["boxmax"] = ParserPropertyPtr(new FloatVector(0.5f, 0.5f, 0.5f));
    properties["pose"] = ParserPropertyPtr(new FloatVector(0.0f, 0.0f, 0.0f));
    properties["rotxlimit"] = ParserPropertyPtr(new FloatVector(-10.0f, 10.0f));
    properties["rotylimit"] = ParserPropertyPtr(new FloatVector(-10.0f, 10.0f));
    properties["rotzlimit"] = ParserPropertyPtr(new FloatVector(-10.0f, 10.0f));
}

void BallJoint::postParse(ParserPropertyMap& properties)
{
    // Get property pointers
    FloatVector* offset = static_cast<FloatVector*>(properties["offset"].get());
    FloatVector* pose = static_cast<FloatVector*>(properties["pose"].get());
    FloatVector* rotxlimit = static_cast<FloatVector*>(properties["rotxlimit"].get());
    FloatVector* rotylimit = static_cast<FloatVector*>(properties["rotylimit"].get());
    FloatVector* rotzlimit = static_cast<FloatVector*>(properties["rotzlimit"].get());
    FloatVector* boxmin = static_cast<FloatVector*>(properties["boxmin"].get());
    FloatVector* boxmax = static_cast<FloatVector*>(properties["boxmax"].get());
    
    // Set joint offset
    m_offset = offset->getVector3();
    
    // Create x-axis rotation DOF
    JointDOF *dof = new JointDOF();
    dof->setName(m_name + ".xrot");
    dof->setMin(rotxlimit->getFloat(0));
    dof->setMax(rotxlimit->getFloat(1));
    dof->setVal(pose->getFloat(0));
    m_dofList.push_back(JointDOFPtr(dof));
    
    // Create y-axis rotation DOF
    dof = new JointDOF();
    dof->setName(m_name + ".yrot");
    dof->setMin(rotylimit->getFloat(0));
    dof->setMax(rotylimit->getFloat(1));
    dof->setVal(pose->getFloat(1));
    m_dofList.push_back(JointDOFPtr(dof));
    
    // Create z-axis rotation DOF
    dof = new JointDOF();
    dof->setName(m_name + ".zrot");
    dof->setMin(rotzlimit->getFloat(0));
    dof->setMax(rotzlimit->getFloat(1));
    dof->setVal(pose->getFloat(2));
    m_dofList.push_back(JointDOFPtr(dof));
    
    // Compute box scale/translate matrix for unit cube (1x1x1 around origin)
    Vector3 boxMin = boxmin->getVector3();
    Vector3 boxMax = boxmax->getVector3();
    m_cubeMatrix = Matrix4().translate((boxMax + boxMin) / 2) * Matrix4().scale(boxMax - boxMin);
}

void BallJoint::updateLocal()
{
    float x = m_dofList[0]->getVal();
    float y = m_dofList[1]->getVal();
    float z = m_dofList[2]->getVal();
    
    // Translate, then rotate by X, Y, then Z
    // NOTE: my rotate classes take degrees instead of radians
    //m_localMatrix = Matrix4().rotateZ(z) * Matrix4().rotateY(y) * Matrix4().rotateX(x) * Matrix4().translate(offset);
    //m_localMatrix = Matrix4().translate(offset) * Matrix4().rotateZ(z) * Matrix4().rotateY(y) * Matrix4().rotateX(x);
    
    Vector3 rotation(x, y, z);
    m_localMatrix.ballJoint(rotation, m_offset); // 3-DOF matrix from lecture slides
}