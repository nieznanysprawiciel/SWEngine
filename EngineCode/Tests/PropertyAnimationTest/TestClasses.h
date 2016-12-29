#pragma once


#include "Common/ParameterAnimation/Animation/Animation.h"

#include "Common/ParameterAnimation/Parameters/DirectPropertyh.h"
#include "Common/ParameterAnimation/Parameters/PropertyPath.h"
#include "Common/ParameterAnimation/Parameters/StringPropertyPath.h"


#include <string>



struct Nested2
{
	float			SomeValue;
	int				IntValue;
	std::string		Name;
};

struct Nested
{
	float			SomeValue;
	int				IntValue;
	std::string		Name;
	Nested2			Additional;
};


/**@brief */
class TestAnim : public EngineObject
{
	RTTR_ENABLE( EngineObject );
	RTTR_REGISTRATION_FRIEND;
private:
	float			m_energy;
	float			m_mass;
	float			m_power;
	float			m_shit;

	Nested			m_otherData;

protected:
public:
	Ptr< FloatAnimation< StringPropertyPath > >		FloatAnimString;
	Ptr< FloatAnimation< PropertyPath > >			FloatAnimProperty;
	Ptr< FloatAnimation< DirectProperty > >			FloatAnimDirect;

public:
	explicit		TestAnim();
	~TestAnim() { };

};

inline TestAnim::TestAnim()
{
	m_energy = 1.0f;
	m_mass = 2.0f;
	m_power = 3.0f;
	m_shit = 4.0f;
	m_otherData.SomeValue = 5.0f;
	m_otherData.IntValue = 6;
	m_otherData.Name = "Nested other data";
	m_otherData.Additional.SomeValue = 7.0f;
	m_otherData.Additional.Name = "Nested2 additional data";
	m_otherData.Additional.IntValue = 8;
}
