#include "EngineCore/stdafx.h"
#include "InputApi.h"

#include "EngineCore/MainEngine/EngineContext.h"

namespace Api
{

/**@brief Pobiera jedn� ze standardowych (wbudowanych) warstw abstrakcji.

@param[in] layer Enum oznaczaj�cy pobieran� warstw�.
@return Zwraca wska�nik na pobieran� warstw�.
*/
InputAbstractionLayer*		InputApi::GetStandardAbstractionLayer		( STANDARD_ABSTRACTION_LAYER layer )
{	return Context->ui_engine->GetStandardAbstractionLayer( layer );	}

/**@brief Ustawia podan� warstw� abstrakcji jako aktualn�.

@param[in] layer Wska�nik na warstw� abstrakcji.
*/
void						InputApi::SetAbstractionLayer				( InputAbstractionLayer* layer )
{	Context->ui_engine->ChangeAbstractionLayer( layer );	}

/**@brief Ustawia jedn� ze standardowych (wbudowanych) warstw abstrakcji.

@param[in] layer Enum oznaczaj�cy wybran� warstw�.
*/
void						InputApi::SetStandardAbstractionLayer		( STANDARD_ABSTRACTION_LAYER layer )
{	Context->ui_engine->SetStandardAbstractionLayer( layer );	}


}	// Api
