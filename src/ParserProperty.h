#ifndef __PARSERPROPERTY_H__
#define __PARSERPROPERTY_H__

#include <iostream>
#include <algorithm>
#include <vector>
#include <map>

#include "Vector3.h"

// ParserProperty
class IParserProperty
{
public:
    virtual ~IParserProperty() {}
    
    virtual void parse(std::istream& in) = 0;
    virtual void print(std::ostream& out) const = 0;
};

typedef std::shared_ptr<IParserProperty> ParserPropertyPtr;
typedef std::map<std::string, ParserPropertyPtr> ParserPropertyMap;

inline std::ostream& operator<<(std::ostream& out, const IParserProperty* p)
{
    p->print(out);
    
    return out;
}

inline std::istream& operator>>(std::istream& in, IParserProperty* p)
{
    p->parse(in);
    
    return in;
}

// Float vector
class FloatVector : public IParserProperty
{
protected:
    std::vector<float> m_vector;
    
public:
    FloatVector(float x, float y) {m_vector.push_back(x); m_vector.push_back(y);}
    FloatVector(float x, float y, float z) {m_vector.push_back(x); m_vector.push_back(y); m_vector.push_back(z);}
    
    virtual void parse(std::istream& in) override;
    virtual void print(std::ostream& out) const override;
    
    float getFloat(int index) {if (index < m_vector.size()) return m_vector[index]; return 0;}
    Vector3 getVector3() {if (m_vector.size() >= 3) return Vector3(m_vector[0], m_vector[1], m_vector[2]); return Vector3();}
};

#endif