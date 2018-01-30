#include "ParserProperty.h"
#include "Vector3.h"

void FloatVector::parse(std::istream& in)
{
    for (int i = 0; i < m_vector.size(); ++i)
        in >> m_vector[i];
}

void FloatVector::print(std::ostream& out) const
{
    for (int i = 0; i < m_vector.size(); ++i)
        out << m_vector[i] << " ";
}