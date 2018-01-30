#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <map>
#include <cstdint>

#include "JointDOF.h"

template <class T>
class EnumMap
{
protected:
    std::map<std::string, T> m_map;
    T m_none;
    
public:
    EnumMap(T none);
    T get(std::string str)
    {
        auto it = m_map.find(str);
        if (it == m_map.end())
            return m_none;
        return it->second;
    }
};

class Keyframe
{
protected:
    enum TanType: uint8_t {NONE, FLAT, LINEAR, SMOOTH};
    friend class EnumMap<TanType>;
    static EnumMap<TanType> k_tanMap;
    
protected:
    float m_time, m_value;
    float m_tanIn, m_tanOut;
    float m_a, m_b, m_c;//, m_d; // m_d == m_value
    TanType m_ruleIn, m_ruleOut;
    
public:
    void parse(std::istream& in);
    void computeTangent(Keyframe *prev, Keyframe *next);
    void computeCoefficient(Keyframe& next);
    
    float getTime() {return m_time;}
    float getValue() {return m_value;}
    float getTanIn() {return m_tanIn;}
    float getTanOut() {return m_tanOut;}
    float evaluate(float normTime) {return m_value + normTime*(m_c + normTime*(m_b + normTime*m_a));}
};

class Channel
{
protected:
    enum ExtrapType: uint8_t {NONE, CONSTANT, LINEAR, CYCLE, CYCLE_OFFSET, BOUNCE};
    friend class EnumMap<ExtrapType>;
    static EnumMap<ExtrapType> k_extrapMap;
    
protected:
    ExtrapType m_extrapIn, m_extrapOut;
    std::vector<Keyframe> m_keyframes;
    
public:
    void parse(std::istream& in);
    float evaluate(float animTime);
};

typedef std::shared_ptr<Channel> ChannelPtr;

class Animation
{
protected:
    std::vector<ChannelPtr> m_channels;
    float m_start, m_end;
    
public:
    void load(std::string filename);
    
    void animate(std::vector<JointDOFPtr>& dofList, float time);
};

#endif