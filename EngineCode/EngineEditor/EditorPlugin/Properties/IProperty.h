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

	/**Klasa bazowa dla w�asciwo�ci oraz grup w�a�ciwo�ci.
	
	Parametry aktor�w maj� struktur� hierarchiczn�. Moga by� typami prostymi:
	- int
	- float
	- bool
	- XMFloat2
	- XMFloat3
	- XMFloat4
		
	Parametry mog� by� r�wnie� strukturami/klasami, je�eli dziedzicz� po IEnableProperty.
		
	Dodatkowo w�a�ciwo�ci mog� by� grupowane w kategorie przy pomocy metadanych do��czanych
	do informacji i klasie.*/
	public ref class IProperty
	{
		virtual		System::String		GetName() = 0;
	};

}