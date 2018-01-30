#ifndef __JOINTDOF_H__
#define __JOINTDOF_H__

#include <iostream>
#include <string>
#include <algorithm>

class JointDOF
{
    friend std::ostream& operator<<(std::ostream&, const JointDOF&);
    
protected:
    std::string m_name;
    float m_val, m_min, m_max;
    
public:
    void setName(std::string name) {m_name = name;}
    void setVal(float val) {m_val = std::max(m_min, std::min(val, m_max));}
    void setMin(float min) {m_min = min;}
    void setMax(float max) {m_max = max;}
    
    std::string getName() const {return m_name;}
    float getVal() const {return m_val;}
    float getMin() const {return m_min;}
    float getMax() const {return m_max;}
};

typedef std::shared_ptr<JointDOF> JointDOFPtr;

inline std::ostream& operator<<(std::ostream& out, const JointDOF& dof)
{
    return out << dof.getName() << ": " << dof.getVal() << " (" << dof.getMin() << ", " << dof.getMax() << ")";
}

#endif