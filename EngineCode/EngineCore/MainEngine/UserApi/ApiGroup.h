#pragma once

#include "Common/TypesDefinitions.h"

class Engine;
struct EngineContext;
class EngineInterface;

namespace Api
{

	/**@brief Klasa bazowa dla klas w EngineInterface s³u¿¹cych do grupowania
	funkcji interfejsu dla u¿ytkownika (klasy GamePlay i obiekty aktorów).

	Klasa posiada statyczny wskaŸnik na Engine i EngineContext. Dziêki temu nie trzeba go przekazywaæ
	do ka¿dej klasy z osobna. Dodatkowo mo¿na w tej klasie umieszczaæ wskaŸniki na inne
	obiekty, które wymagaj¹ bezpoœredniego dostêpu.*/
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
