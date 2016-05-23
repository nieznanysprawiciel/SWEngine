#include "EngineCore/stdafx.h"
#include "DirectInputModule.h"


#define DIRECT_INPUT_OK				DI_OK

/**@brief */
DirectInputModule::DirectInputModule()
{
	m_directInput = nullptr;
	m_mouseInput = nullptr;
	m_keyboardInput = nullptr;
}


/**@brief */
DirectInputModule::~DirectInputModule()
{
	ClearInputStates();
}

/**@brief */
bool									DirectInputModule::Init				( const InputInitInfo& initInfo )
{
	int result = DirectInput8Create( (HINSTANCE)initInfo.AppInstance/* engine->GetInstanceHandler()*/,
									 DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&m_directInput, NULL );
	if( result != DIRECT_INPUT_OK )
		return false;

	//tworzymy obiekt klawiatury
	result = m_directInput->CreateDevice( GUID_SysKeyboard, &m_keyboardInput, nullptr );
	if( result != DIRECT_INPUT_OK )
	{
		m_directInput->Release();
		return false;
	}
	m_keyboardInput->SetDataFormat( &c_dfDIKeyboard );
	m_keyboardInput->SetCooperativeLevel( (HWND)initInfo.WndHandle/*engine->GetWindowHandler()*/, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE );

	//tworzymy obiekt myszy
	result = m_directInput->CreateDevice( GUID_SysMouse, &m_mouseInput, nullptr );
	if( result != DIRECT_INPUT_OK )
	{
		m_keyboardInput->Release();
		m_directInput->Release();
		return false;
	}
	m_mouseInput->SetDataFormat( &c_dfDIMouse2 );
	m_mouseInput->SetCooperativeLevel( (HWND)initInfo.WndHandle/*engine->GetWindowHandler()*/, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE );

	//todo: sprawdziæ czy jest w systemie joystick i go wczytaæ

	m_keyboardInput->Acquire();
	m_mouseInput->Acquire();

	m_keyboards.push_back( new KeyboardState() );
	m_mouses.push_back( new MouseState() );

	return true;
}

/**@copydoc IInput::GetKeyboardStates*/
const std::vector< KeyboardState* >&	DirectInputModule::GetKeyboardStates	()
{
	return m_keyboards;
}

/**@copydoc IInput::GetMouseStates*/
const std::vector< MouseState* >&		DirectInputModule::GetMouseStates		()
{
	return m_mouses;
}

/**@copydoc IInput::GetJoystickStates*/
const std::vector< JoystickState* >&	DirectInputModule::GetJoystickStates	()
{
	return m_joysticks;
}

/**@brief */
std::vector< const InputDeviceInfo* >	DirectInputModule::GetDevicesInfo		()
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
void									DirectInputModule::Update				( float timeInterval )
{

}

/**@brief */
bool									DirectInputModule::UpdateDevices()
{
	assert( !"Zaimplementuj mnie" );
	return false;
}

/**@brief Zwalania zasoby DirectInputa.*/
void DirectInputModule::CleanDirectInput()
{
	if ( m_directInput != nullptr )
	{
		if ( m_keyboardInput != nullptr )
		{
			m_keyboardInput->Unacquire();
			m_keyboardInput->Release();
		}

		if ( m_mouseInput != nullptr)
		{
			m_mouseInput->Unacquire();
			m_mouseInput->Release();
		}
		m_directInput->Release();
	}
}

/**@brief */
void			DirectInputModule::ClearInputStates	()
{
	for( auto state : m_keyboards )
		delete state;
	for( auto state : m_mouses )
		delete state;
	for( auto state : m_joysticks )
		delete state;
}
