#pragma once

#include "Common/TypesDefinitions.h"

class Engine;
struct EngineContext;
class EngineInterface;

namespace Api
{

	/**@brief Klasa bazowa dla klas w EngineInterface s�u��cych do grupowania
	funkcji interfejsu dla u�ytkownika (klasy GamePlay i obiekty aktor�w).

	Klasa posiada statyczny wska�nik na Engine i EngineContext. Dzi�ki temu nie trzeba go przekazywa�
	do ka�dej klasy z osobna. Dodatkowo mo�na w tej klasie umieszcza� wska�niki na inne
	obiekty, kt�re wymagaj� bezpo�redniego dost�pu.*/
	class ApiGroup
	{
		friend class EngineInterface;
	private:
	protected:
		static Engine*			Engine;
		static EngineContext*	Context;
	public:
		ApiGroup() = default;
		~ApiGroup() = default;
	};

}	// Api
