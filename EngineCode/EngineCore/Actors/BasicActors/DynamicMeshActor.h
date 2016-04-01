#pragma once
#include "PhysicalActor.h"

#include "GraphicAPI/MeshResources.h"



/**@brief Klasa bazowa dla obiektów, które bêd¹ renderowane.
*/
class DynamicMeshActor : public PhysicalActor
{
	friend class DisplayEngine;

	RTTR_ENABLE( PhysicalActor )
	
#ifdef _SCALEABLE_OBJECTS
private:
	float							scale;		///<Skalowanie wzglêdem wszystkich osi.
public:
	void set_scale( float sc ) { scale = sc; };
#endif
protected:
	Model3DFromFile*				model_reference;					//zapisujemy odwo³anie, ¿ebyœmy wiedzieli, ¿e nie mo¿emy kasowaæ tego obiektu
	BufferObject*					vertex_buffer;						//ca³y bufor przechowujemy w jednym obiekcie
	BufferObject*					index_buffer;						//tak samo bufor indeksów
	std::vector<ModelPart>			model_parts;

	bool							model_changed;
public:
	DynamicMeshActor();
	DynamicMeshActor( BufferObject* vertexBuffer, BufferObject* indexBuffer );
	virtual ~DynamicMeshActor();

	int							SetModel		( Model3DFromFile* model );

	/// @todo Przemyœleæ czy te funkcje s¹ konieczne.
	BufferObject*				GetVertexBuffer	()	{ return vertex_buffer; }
	BufferObject*				GetIndexBuffer	()	{ return index_buffer; }
	std::vector<ModelPart>&		GetModelParts	()	{ return model_parts; }

	void						AddModelPart	( ModelPart& modelPart );
	///

	static Object*				Create()	{ return new DynamicMeshActor; }

private:
	void AddReferences( const ModelPart* part );
	void DeleteAllReferences();

	/**@brief Funkcja wywo³ywana dla obiektów samo-renderuj¹cych. Ma za zadanie narysowaæ
	obiekt, dla którego zosta³a wywo³ana.

	Je¿eli obiekt ma siê renderowaæ w sposób standardowy, nie ma potrzeby implementowania tej funkcji.
	Zasadniczo nie powinno siê u¿ywaæ tego sposoby renderowania, je¿eli nie ma ku temu wa¿nych powodów,
	poniewa¿ uniemo¿liwia on skuteczn¹ optymalizacjê renderowania.

	@see @ref selfDrawingObjects

	@param[in] device_context WskaŸnik do obiektu directXa s³u¿¹cego do renderowania sceny.
	@param[in] time_interval Czas od ostatniej klatki.
	@param[in] time_lag Czas wzglêdem ostatniego przeliczenia po³o¿eñ.
	*/
	virtual void Draw( IRenderer* renderer, float timeInterval, float timeLag ) {}
};

