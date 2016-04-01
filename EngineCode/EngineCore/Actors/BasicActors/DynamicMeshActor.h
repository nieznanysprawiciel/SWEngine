#pragma once
#include "PhysicalActor.h"

#include "GraphicAPI/MeshResources.h"



/**@brief Klasa bazowa dla obiekt�w, kt�re b�d� renderowane.
*/
class DynamicMeshActor : public PhysicalActor
{
	friend class DisplayEngine;

	RTTR_ENABLE( PhysicalActor )
	
#ifdef _SCALEABLE_OBJECTS
private:
	float							scale;		///<Skalowanie wzgl�dem wszystkich osi.
public:
	void set_scale( float sc ) { scale = sc; };
#endif
protected:
	Model3DFromFile*				model_reference;					//zapisujemy odwo�anie, �eby�my wiedzieli, �e nie mo�emy kasowa� tego obiektu
	BufferObject*					vertex_buffer;						//ca�y bufor przechowujemy w jednym obiekcie
	BufferObject*					index_buffer;						//tak samo bufor indeks�w
	std::vector<ModelPart>			model_parts;

	bool							model_changed;
public:
	DynamicMeshActor();
	DynamicMeshActor( BufferObject* vertexBuffer, BufferObject* indexBuffer );
	virtual ~DynamicMeshActor();

	int							SetModel		( Model3DFromFile* model );

	/// @todo Przemy�le� czy te funkcje s� konieczne.
	BufferObject*				GetVertexBuffer	()	{ return vertex_buffer; }
	BufferObject*				GetIndexBuffer	()	{ return index_buffer; }
	std::vector<ModelPart>&		GetModelParts	()	{ return model_parts; }

	void						AddModelPart	( ModelPart& modelPart );
	///

	static Object*				Create()	{ return new DynamicMeshActor; }

private:
	void AddReferences( const ModelPart* part );
	void DeleteAllReferences();

	/**@brief Funkcja wywo�ywana dla obiekt�w samo-renderuj�cych. Ma za zadanie narysowa�
	obiekt, dla kt�rego zosta�a wywo�ana.

	Je�eli obiekt ma si� renderowa� w spos�b standardowy, nie ma potrzeby implementowania tej funkcji.
	Zasadniczo nie powinno si� u�ywa� tego sposoby renderowania, je�eli nie ma ku temu wa�nych powod�w,
	poniewa� uniemo�liwia on skuteczn� optymalizacj� renderowania.

	@see @ref selfDrawingObjects

	@param[in] device_context Wska�nik do obiektu directXa s�u��cego do renderowania sceny.
	@param[in] time_interval Czas od ostatniej klatki.
	@param[in] time_lag Czas wzgl�dem ostatniego przeliczenia po�o�e�.
	*/
	virtual void Draw( IRenderer* renderer, float timeInterval, float timeLag ) {}
};

