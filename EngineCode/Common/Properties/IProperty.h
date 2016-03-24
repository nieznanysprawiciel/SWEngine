#pragma once



class IProperty
{
private:
	
	const char*		m_name;		///< Nazwa w�a�ciwo�ci, po kt�rej mo�na si� do niej odwo�ywa�.
	const char*		m_type;		///< Typ w�a�ciwo�ci identyfikowany wska�nikiem na struktur� typeinfo. (mo�e si� zmieni� w przysz�o�ci)

	bool			m_serializable;
	bool			m_showInEditor;

public:
	const char*		GetPropertyName()		{ return m_name; }
	const char*		GetPropertyType()		{ return m_type; }
};
