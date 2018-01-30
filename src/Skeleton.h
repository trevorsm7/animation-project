#ifndef __SKELETON_H__
#define __SKELETON_H__

#include <string>
#include <vector>

#include "BaseJoint.h"
#include "Matrix4.h"
#include "JointDOF.h"

class Skeleton
{
protected:
    std::vector<JointPtr> m_joints;
    std::vector<JointDOFPtr> m_position;
    
public:
    void load(std::string filename);
    
    // consider retrieving DOFs by index and storing list locally
    std::vector<JointDOFPtr> getDOFList();
    int getBoneCount() {return m_joints.size();}
    Matrix4& getWorldMatrix(int i) {return m_joints[i]->getWorldMatrix();}
    Matrix4& getCubeMatrix(int i) {return m_joints[i]->getCubeMatrix();}
    
    void print(std::ostream& out) {m_joints[0]->print(out, 0);}
    void update() {m_joints[0]->update(Matrix4().translate(m_position[0]->getVal(), m_position[1]->getVal(), m_position[2]->getVal()));}
};

#endif