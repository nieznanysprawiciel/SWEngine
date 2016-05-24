#include "EngineCore/stdafx.h"
#include "WPFInputProxy.h"


/**@brief */
WPFInputProxy::WPFInputProxy()
{ }

/**@brief */
WPFInputProxy::~WPFInputProxy()
{ }

bool WPFInputProxy::Init( const InputInitInfo & initInfo )
{
	m_keyboards.push_back( new KeyboardState() );
	m_mouses.push_back( new MouseState() );
	m_joysticks.push_back( new JoystickState() );

	return true;
}


/**@copydoc IInput::GetKeyboardStates*/
const std::vector< KeyboardState* >&	WPFInputProxy::GetKeyboardStates	()
{
	return m_keyboards;
}

/**@copydoc IInput::GetMouseStates*/
const std::vector< MouseState* >&		WPFInputProxy::GetMouseStates		()
{
	return m_mouses;
}

/**@copydoc IInput::GetJoystickStates*/
const std::vector< JoystickState* >&	WPFInputProxy::GetJoystickStates	()
{
	return m_joysticks;
}

/**@brief */
std::vector< const InputDeviceInfo* >	WPFInputProxy::GetDevicesInfo		()
{
	std::vector< const InputDeviceInfo* > infos;
	for( auto& device : m_keyboards )
		infos.push_back( &device->GetInfo() );
	for( auto& device : m_joysticks )
		infos.push_back( &device->GetInfo() );
	for( auto& device : m_mouses )
		infos.push_back( &device->GetInfo() );

	return infos;
}

/**@brief */
void WPFInputProxy::Update( float timeInterval )
{ }

/**@brief */
bool WPFInputProxy::UpdateDevices()
{
	return false;
}
