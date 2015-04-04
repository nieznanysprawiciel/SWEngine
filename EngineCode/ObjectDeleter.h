#pragma once
/**@file ObjectDeleter.h
@brief Klasy s³u¿¹ce do zapewnienia ograniczonej mo¿liwoœci kasowania niektórych obiektów silnika,
które maj¹ prywatne destruktory.
*/

template<typename class_type> class ObjectDeleterKey
{
	friend class ModelsManager;
	friend class ResourceContainer<class_type*>;
private:
	ObjectDeleterKey() = default;								///<Tylko klasa zaprzyjaŸniona mo¿e stworzyæ obiekt.
	ObjectDeleterKey( const ObjectDeleterKey& ) = default;		///<Tylko klasa zaprzyjaŸniona mo¿e stworzyæ obiekt.
	ObjectDeleterKey( const ObjectDeleterKey&& ) = default;		///<Tylko klasa zaprzyjaŸniona mo¿e stworzyæ obiekt.
public:
};

/**@brief Klasa s³u¿y do kasowania obiektów, których destruktor jest prywatny.

Taka sytuacja zachodzi w momencie, gdy nie chcemy, aby ka¿da klasa mog³a skasowaæ
jakiœ obiekt, ale chcemy daæ takie uprawnienia jedynie wybrañcom. Wtedy mo¿emy
sprawiæ, aby ta klasa by³a zaprzyjaŸniona
*/
template<typename class_type> class ObjectDeleter
{
private:
	ObjectDeleter() = default;							///<Nie mo¿na stworzyæ obiektu nie maj¹c klucza.
	ObjectDeleter( const ObjectDeleter& ) = default;	///<Nie mo¿na stworzyæ obiektu nie maj¹c klucza.
public:
	ObjectDeleter( const ObjectDeleterKey<class_type>& deleter_key ) = default;

	delete_object( class_type* object )
	{
		delete object;
	}
};
