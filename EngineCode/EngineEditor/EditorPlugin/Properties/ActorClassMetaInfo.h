#pragma once

#include "Common/RTTR.h"

#include "PropertyHelper.h"


class EngineObject;

namespace EditorPlugin
{
using namespace System::Collections::Generic;

/**@brief Klasa przechowuje metadane na temat typów actorów silnikowych.

Klasa mo¿e byc u¿yta nie tylko do wydobywania metainformacji o typie,
ale równie¿ mo¿e pos³u¿yæ do pobrania wartoœci dla konkretnego obiektu.
Przechowywany wewnêtrznie wskaŸnik m_actorPtr mo¿e zostaæ ustawiony przy
pomocy funkcji reset. Skutkuje to zaktualizowaniem wskaŸnika w ca³ej hierarchii.

WskaŸnik mo¿e byæ nullptrem, wtedy zwracane s¹ jakieœ domyœlne wartoœci dla parametrów.

@todo Bardziej eleganckie by by³o, gdyby metainformacje by³y przechowywane osobno od konkretnych danych
obiektów jak ten wskaŸnik. Ale wtedy oznacza³oby to powielenie tych klas, aby mog³y pe³niæ tê drug¹ funkcjê
i wymusi³oby to ka¿dorazowe alokowanie ca³ej hierarchii przy klikniêciu na jakiœ obiekt.
Obecne rozwi¹zanie pozwala na prost¹ podmianê wskaŸnika.
*/
public ref class ActorClassMetaInfo
{
private:

	// Metadata
	System::String^						m_actorClassName;
	rttr::type::type_id					m_type;

	List< PropertyWrapper^ >^			m_properties;

	// Object data
	EngineObject*						m_actorPtr;		///< WskaŸnik u¿ywany do pobierania parametrów obiektu klasy.

public:

	explicit ActorClassMetaInfo	( rttr::type classType );


	void		ResetActor		( System::IntPtr objectPtr );

public:

	property int	Type
	{
		int get()	{ return (int)m_type; }
	}

	property System::String^			TypeName
	{
		System::String^					get()	{ return m_actorClassName; }
	}

	property List< PropertyWrapper^ >^	Properties
	{
		List< PropertyWrapper^ >^		get()	{ return m_properties; }
	}

private:

	void				BuildHierarchy	( rttr::type classType );

};

}	//	EditorPlugin
