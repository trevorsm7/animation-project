#include "BaseJoint.h"
#include "JointFactory.h"

void BaseJoint::parse(std::istream& in, std::vector<JointPtr>& list)
{
    std::string line;
    
    in >> line;
    m_name = line;
    
    in >> line;
    if (line.compare("{") != 0)
        throw std::runtime_error(std::string("Error: expected { but got ") + line);
    
    // Map of strings for each property that the file can define
    ParserPropertyMap properties;
    preParse(properties);
    
    while (true)//!in.eof())
    {
        in >> line;
        if (in.fail())
            throw std::runtime_error("Error: joint parser reached end of file before finishing");
        
        // Check line against joint types first
        JointPtr child = JointFactory::create(line);
        if (child)
        {
            list.push_back(child);
            m_children.push_back(child);
            child->parse(in, list);
            continue;
        }
        
        // Check line against property types second
        auto it = properties.find(line);
        if (it != properties.end())
        {
            ParserPropertyPtr property = it->second;
            property->parse(in);
            continue;
        }
        
        // Handle end of joint
        if (line.compare("}") == 0)
            break;
        
        throw std::runtime_error(std::string("Error: failed to parse joint property ") + line);
    }
    
    postParse(properties);
}

void BaseJoint::print(std::ostream& out, int level)
{
    std::string indent;
    for (int i = 0; i < level; ++i)
        indent += "  ";
    
    out << indent << m_name << "\n";
    out << indent << "-offset: " << m_offset << "\n";
    for (JointDOFPtr dof : m_dofList)
        out << indent << "-" << *dof << "\n";
    //out << m_localMatrix << std::endl;
    //out << m_worldMatrix << std::endl;
    
    for (JointPtr child : m_children)
        child->print(out, level + 1);
}

void BaseJoint::update(Matrix4& parent)
{
    updateLocal();
    
    m_worldMatrix = parent * m_localMatrix;
    
    for (JointPtr child : m_children)
        child->update(m_worldMatrix);
}