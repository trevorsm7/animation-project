#include "Skeleton.h"

#include <fstream>

#include "JointFactory.h"

void Skeleton::load(std::string filename)
{
    std::fstream file;
    file.open(filename, file.in);
    if (!file.is_open())
        throw std::runtime_error(std::string("Error: failed to open file ") + filename);
    
    std::string line;
    file >> line;
    
    // Create root node with factory
    JointPtr root = JointFactory::create(line);
    if (!root)
        throw std::runtime_error(std::string("Error: expected joint type but read ") + line);
    m_joints.push_back(root);
    root->parse(file, m_joints);
    
    m_position.clear();
    
    JointDOFPtr x(new JointDOF);
    x->setName("position.x");
    x->setVal(0.0f);
    x->setMin(-10.0f);
    x->setMax(10.0f);
    m_position.push_back(x);
    
    JointDOFPtr y(new JointDOF);
    y->setName("position.y");
    y->setVal(0.0f);
    y->setMin(-10.0f);
    y->setMax(10.0f);
    m_position.push_back(y);
    
    JointDOFPtr z(new JointDOF);
    z->setName("position.z");
    z->setVal(0.0f);
    z->setMin(-10.0f);
    z->setMax(10.0f);
    m_position.push_back(z);
}

std::vector<JointDOFPtr> Skeleton::getDOFList()
{
    std::vector<JointDOFPtr> list;
    
    // Use position as first 3 DOFs
    list.insert(list.end(), m_position.begin(), m_position.end());
    
    for (JointPtr joint : m_joints)
    {
        std::vector<JointDOFPtr>& jointDOF = joint->getDOFList();
        list.insert(list.end(), jointDOF.begin(), jointDOF.end());
    }
    
    return list;
}