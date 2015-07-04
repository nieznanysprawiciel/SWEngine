#pragma once

/**@file meshes_textures_materials.h
@brief plik zawieta deklaracje foramtów wierzcho³ków, klas zawieraj¹cych assety oraz Model3DFromFile.*/


#include "Common\ObjectDeleter.h"
#include "Common\macros_switches.h"
#include "ReferencedObject.h"
#include "ITexture.h"
#include "IShader.h"
#include "IBuffer.h"
#include "IRenderTarget.h"
#include "DirectXMath.h"


//definicje
#define WRONG_ID						0
#define WRONG_MODEL_FILE_ID				WRONG_ID
#define WRONG_MESH_ID					WRONG_ID
#define WRONG_TEXTURE_ID				WRONG_ID
#define WRONG_MATERIAL_ID				WRONG_ID

/// \def WRONG_ID B³êdny identyfikator assetu w klasie referenced_obbject







class ModelsManager;
struct ModelPart;
class BufferObject;
class TextureObject;
class VertexShaderObject;
class PixelShaderObject;
struct MeshPartObject;
struct MaterialObject;

/*
// W buforze wierzcho³ków znajduj¹ siê elementy typu VERT_INDEX.
// Definicja typu VERT_INDEX znajduje siê w pliku macros_switches.h i wygl¹da tak:

#if defined(INDEX_BUFFER_UINT16)
typedef UINT16 VERT_INDEX;
#elif defined(INDEX_BUFFER_UINT32)
typedef UINT32 VERT_INDEX;
#else
typedef UINT32 VERT_INDEX;
#endif

*/


//-------------------------------------------------------------------------------//
//	definicje wierzcho³ków



/// @brief Strutkura dla standardowego wierzcho³ka
typedef struct VertexNormalTexCord1
{
	DirectX::XMFLOAT3	position;		///<Pozycja wierzcho³ka
	DirectX::XMFLOAT3	normal;			///<Wektor normalny wierzcho³ka
	DirectX::XMFLOAT2	tex_cords;		///<Wspó³rzêdne tekstury
} VertexNormalTexCord1;


#ifndef __UNUSED
// Opis s³u¿¹cy do stworzenia layoutu zrozumia³ego dla shaderów w pliku vertex_layouts.cpp
extern D3D11_INPUT_ELEMENT_DESC VertexNormalTexCord1_desc[];
extern unsigned int VertexNormalTexCord1_desc_num_of_elements;
#endif


/// \brief Struktura wierzcho³ka stworzona z myœl¹ o GUI
typedef struct VertexTexCord1
{
	DirectX::XMFLOAT3	position;		///<Pozycja wierzcho³ka
	DirectX::XMFLOAT2	tex_cords;		///<Wspó³rzêdne tekstury
} VertexTexCord1;

#ifndef __UNUSED
// Opis s³u¿¹cy do stworzenia layoutu zrozumia³ego dla shaderów w pliku vertex_layouts.cpp
extern D3D11_INPUT_ELEMENT_DESC VertexTexCord1_desc[];
extern unsigned int VertexTexCord1_desc_num_of_elements;
#endif

//-------------------------------------------------------------------------------//
//	Enumeracje dla klasy Model3DFromFile i wszystkich obiektów zasobów


/** \brief Indeksy tekstur w tablicy ModelPart.

S¹ to wartoœci domyœlne u¿ywane przez wbudowane shadery.
W przypadku w³asnorêcznie pisanych shaderów nie trzeba siê trzymaæ tych sta³ych.*/
typedef enum TEXTURES_TYPES
{
#if ENGINE_MAX_TEXTURES > 0
	TEX_DIFFUSE			///<Tekstura dla kana³u diffuse
#endif
#if ENGINE_MAX_TEXTURES > 1
	, TEX_LIGHTMAP		///<Lightmapa
#endif
#if ENGINE_MAX_TEXTURES > 2
	, TEX_SPECULAR		///<Tekstura dla kana³u specular
#endif
#if ENGINE_MAX_TEXTURES > 3
	, TEX_BUMP_MAP		///<Bump mapa
#endif
#if ENGINE_MAX_TEXTURES > 4
	, TEX_AMBIENT		///<Tekstura dla kana³u ambient
#endif
#if ENGINE_MAX_TEXTURES > 5
	, TEX_DISPLACEMENT_MAP	///<Tekstura przemieszczeñ wierzcho³ków, w przypadku u¿ywania teselacji wierzcho³ków
#endif
#if ENGINE_MAX_TEXTURES > 6
	, TEX_OTHER1		///<Tekstura o dowolnym znaczeniu
#endif
#if ENGINE_MAX_TEXTURES > 7
	, TEX_OTHER2		///<Tekstura o dowolnym znaczeniu
#endif

} TEXTURES_TYPES;

/// \brief Definiuje offset bufora indeksów wzglêdem bufora wierzcho³ków. (Dla funkcji Model3DFromFile::add_index_buffer)
typedef enum VERTEX_BUFFER_OFFSET
{
	LAST = -1,
	BEGIN = 0,
};






/** @brief Struktura opisuje pojedyncz¹ cz¹stkê mesha o jednym materiale, teksturze i shaderach.

W zale¿noœci od zawartoœci pola index_buffer w strukturze ModelPart, mesh jest wyœwietlany w trybie
indeksowanym lub nie.
Je¿eli wartoœæ tego pola wynosi nullptr, to wtedy u¿ywane s¹ zmienne buffer offset i vertices count, które jednoznacznie wskazuj¹, która czêœæ bufora wierzcho³ków ma zostaæ wyœwietlona.

Je¿eli wskaŸnik index_buffer wskazuje na obiekt, to wtedy u¿ywany jest tryb indeksowany
i zmienne buffer_offset, vertices_count i base_vertex.

Klasa jest alokowana w Model3DFromFile i to w³aœnie ta klasa odpowiada za zwolnienie pamiêci.

Pomimo dziedziczenia po klasie referenced_object, nie jest u¿ywane pole unique_id. Dlatego
jest ono w kontruktorze ustawiane na WRONG_ID. MeshPartObject nie mog¹ byæ wspó³dzielone
miêdzy obiektami.*/
struct MeshPartObject : public referenced_object
{
	DirectX::XMFLOAT4X4		transform_matrix;	///<Macierz przekszta³cenia wzglêdem œrodka modelu
	unsigned int			buffer_offset;		///<Offset wzglêdem pocz¹tku bufora indeksów albo wierzcho³ków (zobacz: opis klasy)
	unsigned int			vertices_count;		///<Liczba wierzcho³ków do wyœwietlenia
	int						base_vertex;		///<Wartoœæ dodawana do ka¿dego indeksu przed przeczytaniem wierzcho³ka z bufora. (Tylko wersja indeksowana)
	bool					use_index_buf;		///<Informacja czy jest u¿ywany bufor indeksów czy nie
	
	/** @brief inicjuje objekt neutralnymi wartoœciami tzn. zerami, ustawia use_index_buf na false i
	ustawia macierz przekszta³cenia na macierz identycznoœciow¹.*/
	MeshPartObject( )
		: referenced_object( 0 )		//W tym przypadku identyfikator nie ma znaczenia
	{
		buffer_offset = 0;
		vertices_count = 0;
		base_vertex = 0;
		use_index_buf = false;
		//domyœlnie nie wykonujemy ¿adnego przekszta³cenia
		DirectX::XMMATRIX identity = DirectX::XMMatrixIdentity( );
		XMStoreFloat4x4( &transform_matrix, identity );
	}
};

/** @brief Struktura opisuj¹ca pojedyncz¹ czêœæ mesha gotow¹ do wyœwietlenia.

Meshe s¹ przechowywane w czêœciach, poniewa¿ do ró¿nych wierzcho³ków mog¹ byæ przypisane ró¿ne
materia³y, tekstury i inne elementy. Ta struktura zawiera wskaŸniki na te elementy.

Struktura nie zawiera bufora wierzcho³ków ani bufora indeksów. S¹ one przechowywane zewnêtrznie
w klasie Model3DFromFile lub Dynamic_mesh_object i na jeden mesh przypada tylko jeden bufor wierzcho³ków i maksymalnie
jeden bufor indeksów (mo¿e go nie byæ). 

Obecnoœc bufora indeksów nie oznacza, ¿e ca³y mesh jest wyœwietlany w trybie indeksowanym. Mozliwe jest mieszanie trybów.
Tryb odnosi siê wiêc nie do ca³ego mesha, a pojednyczego obiektu ModelPart.

Tablica texture zawiera wskaŸniki na obiekty tekstur, których maksymalna liczba wynosi ENGINE_MAX_TEXTURES.
Aby obs³u¿yæ wszystkie tekstury jakie mog¹ byc przypisane do obiektu, nale¿y podaæ mu odpowiedni shader, który
umie to zrobiæ. Znaczenie poszczególnych pól tablicy tekstur jest opisywane przez enumeracjê TEXTURES_TYPES
i w taki sposób wykorzystuj¹ je domyœlne shadery.
*/
struct ModelPart
{
	VertexShaderObject*		vertex_shader;
	PixelShaderObject*		pixel_shader;
	MaterialObject*			material;
	TextureObject*			texture[ENGINE_MAX_TEXTURES];
	MeshPartObject*			mesh;

	ModelPart( )
	{
		vertex_shader = nullptr;
		pixel_shader = nullptr;
		material = nullptr;
		mesh = nullptr;

#if ENGINE_MAX_TEXTURES > 0
		texture[0] = nullptr;
#endif
#if ENGINE_MAX_TEXTURES > 1
		texture[1] = nullptr;
#endif
#if ENGINE_MAX_TEXTURES > 2
		texture[2] = nullptr;
#endif
#if ENGINE_MAX_TEXTURES > 3
		texture[3] = nullptr;
#endif
#if ENGINE_MAX_TEXTURES > 4
		texture[4] = nullptr;
#endif
#if ENGINE_MAX_TEXTURES > 5
		texture[5] = nullptr;
#endif
#if ENGINE_MAX_TEXTURES > 6
		texture[6] = nullptr;
#endif
#if ENGINE_MAX_TEXTURES > 7
		texture[7] = nullptr;
#endif
	}
};


/** @brief Klasa przechowuj¹ca tekstury.

Klasa bazowa, która bêdzie u¿ywana przez obiekty silnika.
Powinny po niej odziedziczyæ obiekty konkretnego API graficznego,
¿eby zaimplementowaæ najwa¿niejsze funkcjonalnoœci.*/
class TextureObject : public ITexture
{
	friend ObjectDeleter<TextureObject>;
private:
protected:
	std::wstring				m_fileName;		///<Plik, z którego powsta³a tekstura lub po prostu nazwa tekstury, je¿eli zosta³a wygenerowana.

	//¯eby unikn¹æ pomy³ki, obiekt mo¿e byœ kasowany tylko przez ModelsManager. Zapewnia to ObjectDeleter.
	~TextureObject() default;
public:
	TextureObject();
	std::wstring&	GetFileName() { return m_fileName; }		///<Zwraca nazwê pliku, który pos³u¿y³ do stworzenia obiektu.

	inline bool operator==( TextureObject& object );
	inline bool operator==( const std::wstring& file_name );

	static TextureObject* create_from_file( const std::wstring& file_name );
};


/**Klasa dla render targetów.

Klasa jest jednoczeœnie tekstur¹. Umo¿liwia ustawienie zarówno jako tekstura
obiektu, jak i bufor do renderowania.*/
class RenderTargetObject : public TextureObject, public IRenderTarget
{
private:

protected:

public:

};


/** @brief Klasa przechowuj¹ca vertex shader*/
class VertexShaderObject : public IShader
{
	friend ObjectDeleter<VertexShaderObject>;
private:
protected:
	~VertexShaderObject() default;
public:
	VertexShaderObject();

	static VertexShaderObject* create_from_file( const std::wstring& file_name, const std::string& shader_name, const char* shader_model = "vs_4_0" );
	static VertexShaderObject* create_from_file( const std::wstring& file_name, const std::string& shader_name, ID3D11InputLayout** layout,
												 D3D11_INPUT_ELEMENT_DESC* layout_desc, unsigned int array_size, const char* shader_model = "vs_4_0" );
};

/**@brief Klasa przechowuj¹ca pixel shader*/
class PixelShaderObject : public IShader
{
	friend ObjectDeleter<PixelShaderObject>;
private:
protected:
	~PixelShaderObject() default;
public:
	PixelShaderObject();

	static PixelShaderObject* create_from_file( const std::wstring& file_name, const std::string& shader_name, const char* shader_model = "ps_4_0" );
};



/** \brief Obiekt opakowuj¹cy bufor.

Bufor mo¿e byæ zarówno buforem wierzcho³ków, indeksów jak i sta³ych.
*/
class BufferObject : public IBuffer
{
	friend ObjectDeleter<BufferObject>;
protected:
	unsigned int		m_elementSize;			///<Rozmiar elementu.
	unsigned int		m_elementCount;			///<Liczba elementów.

	~BufferObject() override;
public:
	BufferObject();

	inline unsigned int GetStride()				{ return m_elementSize; }		///<Zwraca rozmiar pojedynczego elementu w buforze.
	inline unsigned int	GetElementSize()		{ return m_elementSize; }		///<Zwraca rozmiar pojedynczego elementu w buforze.
	inline unsigned int GetElementCount()		{ return m_elementCount; }		///<Zwraca liczbê elementów w buforze.

	static BufferObject* create_from_memory( const void* buffer,
											 unsigned int element_size,
											 unsigned int vert_count,
											 unsigned int bind_flag,
											 D3D11_USAGE usage = D3D11_USAGE_DEFAULT );
};




/**@brief Struktura przechowuj¹ca materia³.

DirectX 11 nie ma w³asnych obiektów na materia³y, poniewa¿ nie ma ju¿ domyœlnego
potoku przetwarzania na karcie graficznej. Na wszystko trzeba napisaæ shader i dostarcza
mu siê takie dane, jakie siê chce dostarczyæ. Dlatego informacja o materia³ach bêdzie
przekazywana z buforze sta³ych.

Struktura zachowuje siê jak asset w zwi¹zku z czym mo¿e
byæ wspó³dzielona przez wiele obiektów. Z tego wzglêdu nie mo¿na jej u¿yæ bezpoœrednio w ConstantPerMesh,
poniewa¿ nie chcemy przekazywaæ do bufora sta³ych zmiennych odziedziczonych po referenced_object.
Zamiast tego trzeba tê strukture przepisaæ.

Zwracam uwagê, ¿e tylko kana³ Diffuse jest wektorem 4 wymiarowym, w którym sk³adowa w jest odpowiedzialna
za przezroczystoœæ. Pozosta³e s¹ takie tylko dlatego, ¿e jest to domyœlny format przechowywania danych 
w rejestrach karty graficznej i przypsiesza to operacjê kopiowania.
@see ConstantPerFrame
*/
typedef struct MaterialObject : public referenced_object
{
	friend ObjectDeleter<MaterialObject>;

	DirectX::XMFLOAT4		Diffuse;		//Sk³adowa przezroczystoœci odnosi siê do ca³ego materia³u
	DirectX::XMFLOAT4		Ambient;
	DirectX::XMFLOAT4		Specular;
	DirectX::XMFLOAT4		Emissive;
	float					Power;

	MaterialObject( unsigned int id = WRONG_ID ) : referenced_object( id ){}
	MaterialObject( const MaterialObject* material );

	void set_null_material();
} MaterialObject;


