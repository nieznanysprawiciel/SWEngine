#pragma once

/**@file SkyDome.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief Plik zawiera deklracj� interfejsu SkyDome, po kt�rym dziedzicz� klasy obs�uguj�ce 
obiekty typu skyboxy, skydome'y i wszystkie inne twory, wy�wietlaj�ce t�o.*/


#include "GraphicAPI/MeshResources.h"
#include "EngineCore/ModelsManager/ModelsManager.h"
#include "GraphicAPI/IRenderer.h"


/**@brief Klasa jest interfejsem dla wszystkich klas, kt�re maj� wy�wietla� jakie� t�o.

Interfejs zawiera jedynie metody do komunikacji z klasa DisplayEngine. Poza tym nie ma konieczno�ci
tworzenia jednolitego interfejsu. Nawet nie jest to do ko�ca mo�liwe, bo u�ytkownik musi poda� do inicjacji
dane specyficzne dla konkretnego modelu nieba.

Implementuj�c w�asn� klas�, trzeba pami�ta�, �e po dodaniu do DisplayEngine, obiekt musi by� w ka�dej chwili gotowy
do wy�wietlania. Trzeba tak�e uwa�a� na wielowatkowo��, aby nie wyst�powa�y niesp�jno�ci w danych. Klasa DisplayEngine
nie b�dzie u�ywa�a �adnych narz�dzi do synchronizacji, wi�c jedyne istniej�ce, to te wbudowane w bufory.

W strukturze ModelPart pole mesh jest zainicjowane zaalokowan� struktur� MeshPartObject i jest r�wnie� zwalaniane
w destruktorze klasy SkyDome.

Polecam zapozna� si� z opisem funkcji DisplayEngine::display_sky_box w celu lepszego zorientowania si�
co si� dzieje podczas wy�wietlania kopu�y.

@note U�ywaj�c funkcji przeznaczonych do wype�niania poszczeg�lnych p�l klasy, zwalniamy z siebie obowi�zek
dodawania odwo�a�. Funkcje pilnuj� tak�e, �eby kasowa� odwo�ania w przypadku, gdy jakie� pole zosta�o nadpisane.
Wszystkie odwo�ania do obiekt�w s� kasowane w destruktorze, wi�c nie trzeba si� wog�le przejmowa� �adnymi odwo�aniami.
*/
class SkyDome
{
public:
	bool update_vertex_buffer;			///<Ustawiane w momencie zako�czenia generowania nieba, w celu zaktualizowania bufora
private:
	ModelsManager* models_manager;		///<Wska�nik na ModelsManager. Bufory s� zarz�dzane przez niego.

protected:
	BufferObject*				vertex_buffer;			///<Bufor wierzcho�k�w
	BufferObject*				index_buffer;			///<Bufor indeks�w
	BufferObject*				constant_buffer;		///<Bufor sta�ych dla shader�w
	ShaderInputLayout*	layout;					///<Layout wierzcho�k�w u�ywanych przez klas�
	ModelPart					display_data;			///<Shadery, tekstury itp.

public:
	SkyDome( ModelsManager* man )
		: models_manager( man ),
		vertex_buffer( nullptr ),
		index_buffer( nullptr ),
		constant_buffer( nullptr ),
		layout( nullptr )
	{
		display_data.mesh = new MeshPartObject;
		update_vertex_buffer = false;
	}

	virtual ~SkyDome()
	{
		if ( vertex_buffer )
			vertex_buffer->DeleteObjectReference(), vertex_buffer = nullptr;
		if ( index_buffer )
			index_buffer->DeleteObjectReference(), index_buffer = nullptr;
		if ( layout )
			layout->DeleteObjectReference(), layout = nullptr;
		if ( constant_buffer )
			constant_buffer->DeleteObjectReference(), constant_buffer = nullptr;

		if ( display_data.mesh )
			delete display_data.mesh, display_data.mesh = nullptr;
		if ( display_data.pixel_shader )
			display_data.pixel_shader->DeleteObjectReference(), display_data.pixel_shader = nullptr;
		if ( display_data.vertex_shader )
			display_data.vertex_shader->DeleteObjectReference(), display_data.vertex_shader = nullptr;
		if ( display_data.material )
			display_data.material->DeleteObjectReference(), display_data.material = nullptr;
		for ( int i = 0; i < ENGINE_MAX_TEXTURES; ++i )
			if ( display_data.texture[i] )
				display_data.texture[i]->DeleteObjectReference(), display_data.texture[i] = nullptr;
	}

	inline BufferObject*			get_vertex_buffer()		{ return vertex_buffer; }				///<Zwraca bufor wierzcho�k�w
	inline BufferObject*			get_index_buffer()		{ return index_buffer; }				///<Zwraca bufor indeks�w
	inline BufferObject*			get_constant_buffer()	{ return constant_buffer; }				///<Zwraca bufor sta�ych dla shader�w
	inline ShaderInputLayout*	get_vertex_layout()		{ return layout; }						///<Zwraca layout wierzcho�ka
	inline ModelPart*				get_model_part()		{ return &display_data; }				///<Zwraca dane potrzebne do wy�wietlania

	/**@brief Funkcja jest wywo�ywana w momencie kiedy zmienna update_vertex_buffer zawiera warto�� true.
	
	Zadaniem funkcji jest wype�nienie bufora indeks�w lub wierzcho�k�w nowymi danymi, kt�re zosta�y przygotowane
	gdzie� w buforach tylnich. Dane powinny by� generowane inn� funkcj�, kt�rej nie ma w tym interfejsie, przez
	jak�� funkcj� spoza klasy DisplayEngine i najlepiej w innym w�tku. Zadaniem tej funkcji jest jedynie
	wywo�anie funkcji UpdateSubresource na tych buforach, kt�re wymagaj� aktualizacji.
	
	@attention Nie nale�y generowa� danych w tej funkcji chyba, �e jest ich naprawd� niewiele. Ta funkcja
	b�dzie wywo�ywana w p�tli g��wnej i mo�e zaburzy� renderowanie.
	
	Pole update_vertex_buffer powinno zosta� ustawione na false po wykonaniu tej funkcji.*/
	virtual void update_buffers( IRenderer* renderer ) = 0;
protected:
	// Funkcje do ustawiania

	/**@brief Funkcja ustawia w zmiennej display_data tekstur�.
	Tekstura jest pobierana z ModelsManagera, je�eli nie istnieje jest dodawana.
	
	@param[in] name Nazwa pliku z tekstur�.
	@param[in] type Pozycja w tablicy, do kt�rej zostaniej przypisana tekstura.*/
	inline void set_texture( const std::wstring& name, TextureUse type )
	{
		if ( type > ENGINE_MAX_TEXTURES )
			return;

		auto tex = models_manager->LoadTexture( name );
		if ( !tex )
			return;	// Zabezpieczenie przed nullptrem

		if ( display_data.texture[type] )	// Je�eli co� wcze�niej by�o to kasujemy odwo�anie
			display_data.texture[type]->DeleteObjectReference();

		tex->AddObjectReference();
		display_data.texture[type] = tex;
	}
	/**@brief Funkcja ustawia w zmiennej display_data vertex shader.
	Shader jest pobierany z ModelsManagera, je�eli nie istnieje jest dodawany.

	@param[in] file_name Nazwa pliku z shaderem.
	@param[in] shader_entry Nazwa funkcji shadera.
	@param[in] layout_desc Deskryptor formatu wierzcho�ka.
	@param[in] array_size Liczba element�w tablicy layout_desc.*/
	inline void set_vertex_shader( const std::wstring& file_name,
								   const std::string& shader_entry,
								   InputLayoutDescriptor* layout_desc )
	{
		if ( layout )	// Zwalniamy layout, je�eli jaki� by�
			layout->DeleteObjectReference();

		auto shader = models_manager->LoadVertexShader( file_name, shader_entry, &layout, layout_desc );
		if ( !shader )
			return;	// Zabezpieczenie przed nullptrem
		if ( !layout )
			return;	// Je�eli layout si� nie chcia� stworzy� to lepiej nie dodawa� te� vertex shadera

		if ( display_data.vertex_shader )	// Je�eli co� wcze�niej by�o to kasujemy odwo�anie
			display_data.vertex_shader->DeleteObjectReference( );

		shader->AddObjectReference( );
		display_data.vertex_shader = shader;
	}
	/**@brief Funkcja ustawia w zmiennej display_data pixel shader.
	Shader jest pobierany z ModelsManagera, je�eli nie istnieje jest dodawany.

	@param[in] file_name Nazwa pliku z shaderem.
	@param[in] shader_entry Nazwa funkcji shadera.*/
	inline void set_pixel_shader( const std::wstring& file_name, const std::string& shader_entry )
	{
		auto shader = models_manager->LoadPixelShader( file_name, shader_entry );
		if ( !shader )
			return;	// Zabezpieczenie przed nullptrem

		if ( display_data.pixel_shader )	// Je�eli co� wcze�niej by�o to kasujemy odwo�anie
			display_data.pixel_shader->DeleteObjectReference( );

		shader->AddObjectReference( );
		display_data.pixel_shader = shader;
	}
	/**@brief Funkcja ustawia w zmiennej display_data materia�.
	Materia� jest pobierany z ModelsManagera, je�eli nie istnieje jest dodawany.

	@param[in] material Materia�, kt�ry ma zosta� dodany
	@param[in] material_name Nazwa materia�u. Do materia�u b�dzie mo�na si� odwo�a� podaj�c ci�g znak�w*/
	inline void set_material( MaterialObject* mat, const std::wstring& name)
	{
		auto material = models_manager->AddMaterial( mat, name );
		if ( !material )
			return;	// Zabezpieczenie przed nullptrem

		if ( display_data.material )	// Je�eli co� wcze�niej by�o to kasujemy odwo�anie
			display_data.material->DeleteObjectReference( );

		material->AddObjectReference( );
		display_data.material = material;
	}
	/**@brief Funkcja tworzy, a potem ustawia w zmiennej vertex_buffer bufor wierzcho�k�w.
	Bufor jest pobierany z ModelsManagera, je�eli nie istnieje jest dodawany.

	@param[in] name Nazwa bufora, po kt�rej mo�na si� b�dzie odwo�a�.
	@param[in] buffer Wska�nik na bufor z danym, kt�re maj� by� przeniesione do bufora DirectXowego.
	@param[in] element_size Rozmiar pojedynczego elementu w buforze.
	@param[in] vert_count Liczba wierzcho�k�w/indeks�w w buforze.*/
	inline void set_vertex_buffer( const std::wstring& name, const void* buffer, unsigned int element_size, unsigned int vert_count )
	{
		VertexBufferInitData initData;
		initData.Data = (const uint8*)buffer;
		initData.ElementSize = element_size;
		initData.NumElements = vert_count;

		auto buff = models_manager->CreateVertexBuffer( name, initData );
		if ( !buff )
			return;	// Zabezpieczenie przed nullptrem

		if ( vertex_buffer )	// Je�eli co� wcze�niej by�o to kasujemy odwo�anie
			vertex_buffer->DeleteObjectReference( );

		buff->AddObjectReference( );
		vertex_buffer = buff.Ptr();
	}
	/**@brief Funkcja tworzy, a potem ustawia w zmiennej index_buffer bufor indeks�w.
	Bufor jest pobierany z ModelsManagera, je�eli nie istnieje jest dodawany.

	@param[in] name Nazwa bufora, po kt�rej mo�na si� b�dzie odwo�a�.
	@param[in] buffer Wska�nik na bufor z danym, kt�re maj� by� przeniesione do bufora DirectXowego.
	@param[in] element_size Rozmiar pojedynczego elementu w buforze.
	@param[in] vert_count Liczba wierzcho�k�w/indeks�w w buforze.*/
	inline void set_index_buffer( const std::wstring& name, const void* buffer, unsigned int element_size, unsigned int vert_count )
	{
		IndexBufferInitData initData;
		initData.Data = (const uint8*)buffer;
		initData.ElementSize = element_size;
		initData.NumElements = vert_count;

		auto buff = models_manager->CreateIndexBuffer( name, initData );
		if ( !buff )
			return;	// Zabezpieczenie przed nullptrem

		if ( index_buffer )	// Je�eli co� wcze�niej by�o to kasujemy odwo�anie
			index_buffer->DeleteObjectReference( );

		buff->AddObjectReference( );
		index_buffer = buff.Ptr();
	}

	/**@brief Funkcja tworzy, a potem ustawia w zmiennej constant_buffer bufor sta�ych.
	Bufor jest pobierany z ModelsManagera, je�eli nie istnieje jest dodawany.

	@param[in] name Nazwa bufora, po kt�rej mo�na si� b�dzie odwo�a�.
	@param[in] buffer Wska�nik na bufor z danym, kt�re maj� by� przeniesione do bufora DirectXowego.
	@param[in] element_size Rozmiar pojedynczego elementu w buforze..*/
	inline void set_constants_buffer( const std::wstring& name, const void* buffer, unsigned int element_size )
	{
		ConstantBufferInitData initData;
		initData.Data = (const uint8*)buffer;
		initData.ElementSize = element_size;
		initData.NumElements = 1;

		auto buff = models_manager->CreateConstantsBuffer( name, initData );
		if ( !buff )
			return;	// Zabezpieczenie przed nullptrem

		if ( index_buffer )	// Je�eli co� wcze�niej by�o to kasujemy odwo�anie
			index_buffer->DeleteObjectReference( );

		buff->AddObjectReference( );
		index_buffer = buff.Ptr();
	}
};
