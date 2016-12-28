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
	explicit		TestAnim() { };
	~TestAnim() { };

};


