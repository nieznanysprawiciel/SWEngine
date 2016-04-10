#include "EngineCore/stdafx.h"
#include "InputApi.h"

#include "EngineCore/MainEngine/EngineContext.h"

namespace Api
{

/**@brief Pobiera jedn¹ ze standardowych (wbudowanych) warstw abstrakcji.

@param[in] layer Enum oznaczaj¹cy pobieran¹ warstwê.
@return Zwraca wskaŸnik na pobieran¹ warstwê.
*/
InputAbstractionLayer*		InputApi::GetStandardAbstractionLayer		( STANDARD_ABSTRACTION_LAYER layer )
{	return Context->ui_engine->GetStandardAbstractionLayer( layer );	}

/**@brief Ustawia podan¹ warstwê abstrakcji jako aktualn¹.

@param[in] layer WskaŸnik na warstwê abstrakcji.
*/
void						InputApi::SetAbstractionLayer				( InputAbstractionLayer* layer )
{	Context->ui_engine->ChangeAbstractionLayer( layer );	}

/**@brief Ustawia jedn¹ ze standardowych (wbudowanych) warstw abstrakcji.

@param[in] layer Enum oznaczaj¹cy wybran¹ warstwê.
*/
void						InputApi::SetStandardAbstractionLayer		( STANDARD_ABSTRACTION_LAYER layer )
{	Context->ui_engine->SetStandardAbstractionLayer( layer );	}


}	// Api
