#pragma once


namespace EditorPlugin
{
	enum PropertyType
	{
		PropertyFloat,
		PropertyFloat2,
		PropertyFloat3,
		PropertyFloat4,
		PropertyInt,
		PropertyBool
	};

	/**Klasa bazowa dla w³asciwoœci oraz grup w³aœciwoœci.
	
	Parametry aktorów maj¹ strukturê hierarchiczn¹. Moga byæ typami prostymi:
	- int
	- float
	- bool
	- XMFloat2
	- XMFloat3
	- XMFloat4
		
	Parametry mog¹ byæ równie¿ strukturami/klasami, je¿eli dziedzicz¹ po IEnableProperty.
		
	Dodatkowo w³aœciwoœci mog¹ byæ grupowane w kategorie przy pomocy metadanych do³¹czanych
	do informacji i klasie.*/
	public ref class IProperty
	{
		virtual		System::String		GetName() = 0;
	};

}