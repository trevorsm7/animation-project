#include "Animation.h"

#include <fstream>
#include <cmath>

inline std::string readToken(std::istream& in)
{
    std::string line;
    in >> line;
    if (in.fail())
        throw std::runtime_error("Error: I/O error in animation");
    return line;
}

// String to extrapolation type mapping

template <>
EnumMap<Keyframe::TanType>::EnumMap(Keyframe::TanType none): m_none(none)
{
    m_map["flat"] = Keyframe::FLAT;
    m_map["linear"] = Keyframe::LINEAR;
    m_map["smooth"] = Keyframe::SMOOTH;
}

template <>
EnumMap<Channel::ExtrapType>::EnumMap(Channel::ExtrapType none): m_none(none)
{
    m_map["constant"] = Channel::CONSTANT;
    m_map["linear"] = Channel::LINEAR;
    m_map["cycle"] = Channel::CYCLE;
    m_map["cycle_offset"] = Channel::CYCLE_OFFSET;
    m_map["bounce"] = Channel::BOUNCE;
}

// Keyframe
EnumMap<Keyframe::TanType> Keyframe::k_tanMap(Keyframe::NONE);

void Keyframe::parse(std::istream& in)
{
    std::string line;
    
    in >> m_time >> m_value;
    if (in.fail())
        throw std::runtime_error("Error: failed to read keyframe time and value");
    
    line = readToken(in);
    m_ruleIn = k_tanMap.get(line);
    if (m_ruleIn == NONE)
        m_tanIn = std::stof(line);
    else
        m_tanIn = 0.0f;
    
    line = readToken(in);
    m_ruleOut = k_tanMap.get(line);
    if (m_ruleOut == NONE)
        m_tanOut = std::stof(line);
    else
        m_tanOut = 0.0f;
}

void Keyframe::computeTangent(Keyframe *prev, Keyframe *next)
{
    if (prev == nullptr)
    {
        // pre-set tanIn to 0 in case tanOut has dependency
        if (m_ruleIn == LINEAR || m_ruleIn == SMOOTH)
            m_tanIn = 0.0f;
        
        if (next == nullptr) // only keyframe
        {
            if (m_ruleOut == LINEAR || m_ruleOut == SMOOTH)
                m_tanOut = m_tanIn; // note: tanIn could be manually set
        }
        else // first keyframe
        {
            if (m_ruleOut == LINEAR || m_ruleOut == SMOOTH) // treat smooth same as linear
                m_tanOut = (next->m_value - m_value) / (next->m_time - m_time);
        }
        
        // use tanOut for tanIn if there is a dependency
        if (m_ruleIn == LINEAR || m_ruleIn == SMOOTH)
            m_tanIn = m_tanOut;
    }
    else
    {
        if (m_ruleIn == LINEAR)
            m_tanIn = (m_value - prev->m_value) / (m_time - prev->m_time);
        
        if (next == nullptr) // last keyframe
        {
            if (m_ruleIn == SMOOTH) // same as linear
                m_tanIn = (m_value - prev->m_value) / (m_time - prev->m_time);
            
            if (m_ruleOut == LINEAR || m_ruleOut == SMOOTH)
                m_tanOut = m_tanIn;
        }
        else // neither first nor last keyframe
        {
            // pre-set tanIn to the slope of prev->next
            if (m_ruleIn == SMOOTH)
                m_tanIn = (next->m_value - prev->m_value) / (next->m_time - prev->m_time);
            
            if (m_ruleOut == LINEAR)
                m_tanOut = (next->m_value - m_value) / (next->m_time - m_time);
            else if (m_ruleOut == SMOOTH)
                m_tanOut = m_tanIn; // slope of prev->next only if BOTH are smooth
            
            if (m_ruleIn == SMOOTH)
                m_ruleIn = m_ruleOut; // slope of prev->next only if BOTH are smooth
        }
    }
    
    //std::cout << "  " << m_time << ", " << m_value << ", " << m_tanIn << ", " << m_tanOut << std::endl;
}

void Keyframe::computeCoefficient(Keyframe& next)
{
    float time = next.m_time - m_time;
    float v0 = m_tanOut * time;
    float v1 = next.m_tanIn * time;
    m_a = 2.0f*m_value - 2.0f*next.m_value + v0 + v1;
    m_b = -3.0f*m_value + 3.0f*next.m_value - 2.0f*v0 - v1;
    m_c = v0;
    //m_d = m_value;
    
    //std::cout << "  " << m_a << ", " << m_b << ", " << m_c << ", " << m_value << std::endl;
}

// Channel
EnumMap<Channel::ExtrapType> Channel::k_extrapMap(Channel::NONE);

void Channel::parse(std::istream& in)
{
    std::string line;
    
    line = readToken(in);
    if (line.compare("channel") != 0)
        throw std::runtime_error(std::string("Error: expected \'channel\' but read ") + line);
    
    line = readToken(in);
    if (line.compare("{") != 0)
        throw std::runtime_error(std::string("Error: expected \'{\' but read ") + line);
    
    // Extrapolation types
    line = readToken(in);
    if (line.compare("extrapolate") != 0)
        throw std::runtime_error(std::string("Error: expected \'exrapolate\' but read ") + line);
    
    line = readToken(in);
    m_extrapIn = k_extrapMap.get(line);
    if (m_extrapIn == NONE)
        throw std::runtime_error(std::string("Error: expected extrapolation type: ") + line);
    
    line = readToken(in);
    m_extrapOut = k_extrapMap.get(line);
    if (m_extrapOut == NONE)
        throw std::runtime_error(std::string("Error: expected extrapolation type: ") + line);
    
    // Keyframes
    line = readToken(in);
    if (line.compare("keys") != 0)
        throw std::runtime_error(std::string("Error: expected \'keys\' but read ") + line);
    
    int numkeys;
    in >> numkeys;
    if (in.fail() || numkeys == 0)
        throw std::runtime_error("Error: failed to read numkeys or was zero");
    
    line = readToken(in);
    if (line.compare("{") != 0)
        throw std::runtime_error(std::string("Error: expected \'{\' but read ") + line);
    
    for (int i = 0; i < numkeys; ++i)
    {
        Keyframe key;
        key.parse(in);
        m_keyframes.push_back(key);
    }
    
    // Post-parse keyframes
    if (numkeys > 1)
    {
        m_keyframes[0].computeTangent(nullptr, &m_keyframes[1]);
        for (int i = 1; i < numkeys-1; ++i)
            m_keyframes[i].computeTangent(&m_keyframes[i-1], &m_keyframes[i+1]);
        m_keyframes[numkeys-1].computeTangent(&m_keyframes[numkeys-2], nullptr);
        
        for (int i = 0; i < numkeys-1; ++i)
            m_keyframes[i].computeCoefficient(m_keyframes[i+1]);
    }
    else
        m_keyframes[0].computeTangent(nullptr, nullptr); // just get set to 0 anyway...
    
    line = readToken(in);
    if (line.compare("}") != 0)
        throw std::runtime_error(std::string("Error: expected \'}\' but read ") + line);
    
    line = readToken(in);
    if (line.compare("}") != 0)
        throw std::runtime_error(std::string("Error: expected \'}\' but read ") + line);
}

float Channel::evaluate(float animTime)
{
    Keyframe& lastKey = m_keyframes[m_keyframes.size() - 1];
    
    float start = m_keyframes[0].getTime();
    float end = lastKey.getTime();
    
    float diff = lastKey.getValue()-m_keyframes[0].getValue();
    float offset = 0.0f;
    
    if (animTime < start)
    {
        // Extrapolate in
        switch (m_extrapIn)
        {
            case CYCLE_OFFSET:
                offset = -std::ceil((start-animTime)/(end-start)) * diff;
                // fall through...
            case CYCLE:
                animTime = end - std::fmod(start-animTime, end-start);
                break;
            case BOUNCE:
                animTime = start + std::fmod(start-animTime, 2.0f*(end-start));
                if (animTime > end)
                    animTime = end - (animTime - end);
            case LINEAR:
                return m_keyframes[0].getValue() - (start-animTime)*m_keyframes[0].getTanIn();
            default: // CONSTANT
                return m_keyframes[0].getValue();
        }
    }
    else if (animTime >= end)
    {
        // Extrapolate out
        switch (m_extrapOut)
        {
            case CYCLE_OFFSET:
                offset = std::floor((animTime-start)/(end-start)) * diff;
                // fall through...
            case CYCLE:
                animTime = start + std::fmod(animTime-start, end-start);
                break;
            case BOUNCE:
                animTime = start + std::fmod(animTime-start, 2.0f*(end-start));
                if (animTime > end)
                    animTime = end - (animTime - end);
                break;
            case LINEAR:
                return lastKey.getValue() + (animTime-end)*lastKey.getTanOut();
            default: // CONSTANT
                return lastKey.getValue();
        }
    }
    
    // Find keyframe interval containing time
    int i = 0;
    while (animTime > m_keyframes[i+1].getTime()) ++i;
    
    // Compute relative time and pass to keyframe
    float t0 = m_keyframes[i].getTime();
    float t1 = m_keyframes[i+1].getTime();
    float normTime = (animTime - t0) / (t1 - t0); // can optimize to include /(t1-t0) in coefficients
    return offset + m_keyframes[i].evaluate(normTime);
}

void Animation::load(std::string filename)
{
    m_channels.clear();
    
    std::fstream file;
    file.open(filename, file.in);
    if (!file.is_open())
        throw std::runtime_error(std::string("Error: failed to open file ") + filename);
    
    std::string line;
    
    line = readToken(file);
    if (line.compare("animation") != 0)
        throw std::runtime_error(std::string("Error: expected \'animation\' but read ") + line);
    
    line = readToken(file);
    if (line.compare("{") != 0)
        throw std::runtime_error(std::string("Error: expected \'{\' but read ") + line);
    
    // Start and end times
    line = readToken(file);
    if (line.compare("range") != 0)
        throw std::runtime_error(std::string("Error: expected \'range\' but read ") + line);
    
    file >> m_start >> m_end;
    if (file.fail())
        throw std::runtime_error("Error: failed to read ranges");
    m_start = -4.0f;
    m_end = 8.0f;
    
    // Channel count
    line = readToken(file);
    if (line.compare("numchannels") != 0)
        throw std::runtime_error(std::string("Error: expected \'numchannels\' but read ") + line);
    
    int numchannels;
    file >> numchannels;
    if (file.fail())
        throw std::runtime_error("Error: failed to read numchannels");
    
    for (int i = 0; i < numchannels; ++i)
    {
        //std::cout << "Channel " << i << std::endl;
        ChannelPtr channel(new Channel());
        channel->parse(file);
        m_channels.push_back(channel);
    }
    
    //for (float t = m_start; t <= m_end; t += 0.25f)
    //    std::cout << m_channels[0]->evaluate(t) << std::endl;
    
    line = readToken(file);
    if (line.compare("}") != 0)
        throw std::runtime_error(std::string("Error: expected \'}\' but read ") + line);
    
    file.close();
}

void Animation::animate(std::vector<JointDOFPtr>& dofList, float time)
{
    time = std::fmod(time, m_end-m_start) + m_start;
    
    //if (dofList.size() != m_channels.size())
    //    std::cerr << "Animation channels do not match skeleton DOFs" << std::endl;
    
    int size = std::min(dofList.size(), m_channels.size());
    for (int i = 0; i < size; ++i)
        dofList[i]->setVal(m_channels[i]->evaluate(time));
}