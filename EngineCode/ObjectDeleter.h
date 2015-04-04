#pragma once
/**@file ObjectDeleter.h
@brief Klasy s�u��ce do zapewnienia ograniczonej mo�liwo�ci kasowania niekt�rych obiekt�w silnika,
kt�re maj� prywatne destruktory.
*/

template<typename class_type> class ObjectDeleterKey
{
	friend class ModelsManager;
	friend class ResourceContainer<class_type*>;
private:
	ObjectDeleterKey() = default;								///<Tylko klasa zaprzyja�niona mo�e stworzy� obiekt.
	ObjectDeleterKey( const ObjectDeleterKey& ) = default;		///<Tylko klasa zaprzyja�niona mo�e stworzy� obiekt.
	ObjectDeleterKey( const ObjectDeleterKey&& ) = default;		///<Tylko klasa zaprzyja�niona mo�e stworzy� obiekt.
public:
};

/**@brief Klasa s�u�y do kasowania obiekt�w, kt�rych destruktor jest prywatny.

Taka sytuacja zachodzi w momencie, gdy nie chcemy, aby ka�da klasa mog�a skasowa�
jaki� obiekt, ale chcemy da� takie uprawnienia jedynie wybra�com. Wtedy mo�emy
sprawi�, aby ta klasa by�a zaprzyja�niona
*/
template<typename class_type> class ObjectDeleter
{
private:
	ObjectDeleter() = default;							///<Nie mo�na stworzy� obiektu nie maj�c klucza.
	ObjectDeleter( const ObjectDeleter& ) = default;	///<Nie mo�na stworzy� obiektu nie maj�c klucza.
public:
	ObjectDeleter( const ObjectDeleterKey<class_type>& deleter_key ) = default;

	delete_object( class_type* object )
	{
		delete object;
	}
};
