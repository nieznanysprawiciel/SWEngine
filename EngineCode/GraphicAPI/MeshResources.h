#pragma once
/**
@file MeshResources.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Plik zawiera deklaracje formatów wierzcho³ków oraz klas zawieraj¹cych assety shadery itp.
*/


#include "Common/ObjectDeleter.h"
#include "Common/MacrosSwitches.h"
#include "Common/System/Path.h"

#include "GraphicAPI/ResourceObject.h"
#include "GraphicAPI/IShader.h"
#include "GraphicAPI/IBuffer.h"
#include "GraphicAPI/IRenderTarget.h"
#include "IShaderInputLayout.h"
#include "GraphicAPI/GraphicAPIConstants.h"
#include "GraphicAPI/ResourcePtr.h"

#include <DirectXMath.h>


//definicje
/** @def WRONG_ID
B³êdny identyfikator assetu w klasie @ref ResourceObject.*/
#define WRONG_ID						0

/**@defgroup Resources Zasoby
@ingroup ResourcesManagment
@ingroup GraphicAPI
@brief Niskopoziomowe zasoby zwi¹zane z api graficznym.

Zasoby s¹ silnie zale¿ne od u¿ywanej platformy sprzêtowej. W celu oddzielenia referencji do
API graficznego od @ref EngineCore, wszystkie obiekty silnika u¿ywaj¹ jedynie klas bazowych, które
s¹ implementowane przez poszczególne API graficzne. Aby zobaczyæ konkretne implementacje tych klas
zobacz @ref GraphicAPI.

Zasoby nigdy nie s¹ tworzone bezpoœrednio. Zamiast tego u¿ywa siê klasy @ref ResourcesFactory, któr¹
implementuje konkretne API graficzne.

Poniewa¿ zasoby mog¹ byæ wspó³dzielone przez wiele obiektów w silniku, istnieje mechanizm zliczania
odwo³añ do obiektów implementowany przez klasê @ref ResourceObject.*/

class ModelsManager;
struct ModelPart;
class BufferObject;
class TextureObject;
class VertexShader;
class PixelShader;
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


static const std::wstring RENDER_TARGET_COLOR_BUFFER_NAME = L"::color";
static const std::wstring RENDER_TARGET_DEPTH_BUFFER_NAME = L"::depth";
static const std::wstring RENDER_TARGET_STENCIL_BUFFER_NAME = L"::stencil";


/**@brief Indeksy tekstur w tablicy ModelPart.

S¹ to wartoœci domyœlne u¿ywane przez wbudowane shadery.
W przypadku w³asnorêcznie pisanych shaderów nie trzeba siê trzymaæ tych sta³ych.*/
enum TextureUse
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

};



//----------------------------------------------------------------------------------------------//
//								Mesh i Model													//
//----------------------------------------------------------------------------------------------//

/** @brief Struktura opisuje pojedyncz¹ cz¹stkê mesha o jednym materiale, teksturze i shaderach.
@ingroup Resources
@ingroup GraphicAPI

W zale¿noœci od zawartoœci pola index_buffer w strukturze ModelPart, mesh jest wyœwietlany w trybie
indeksowanym lub nie.
Je¿eli wartoœæ tego pola wynosi nullptr, to wtedy u¿ywane s¹ zmienne buffer offset i vertices count, które jednoznacznie wskazuj¹, która czêœæ bufora wierzcho³ków ma zostaæ wyœwietlona.

Je¿eli wskaŸnik index_buffer wskazuje na obiekt, to wtedy u¿ywany jest tryb indeksowany
i zmienne buffer_offset, vertices_count i base_vertex.

Klasa jest alokowana w Model3DFromFile i to w³aœnie ta klasa odpowiada za zwolnienie pamiêci.

Pomimo dziedziczenia po klasie ResourceObject, nie jest u¿ywane pole m_uniqueId. Dlatego
jest ono w kontruktorze ustawiane na WRONG_ID. MeshPartObject nie mog¹ byæ wspó³dzielone
miêdzy obiektami.*/
struct MeshPartObject : public ResourceObject
{
	DirectX::XMFLOAT4X4		transform_matrix;	///<Macierz przekszta³cenia wzglêdem œrodka modelu
	unsigned int			buffer_offset;		///<Offset wzglêdem pocz¹tku bufora indeksów albo wierzcho³ków (zobacz: opis klasy)
	unsigned int			vertices_count;		///<Liczba wierzcho³ków do wyœwietlenia
	int						base_vertex;		///<Wartoœæ dodawana do ka¿dego indeksu przed przeczytaniem wierzcho³ka z bufora. (Tylko wersja indeksowana)
	bool					use_index_buf;		///<Informacja czy jest u¿ywany bufor indeksów czy nie
	
	/** @brief inicjuje objekt neutralnymi wartoœciami tzn. zerami, ustawia use_index_buf na false i
	ustawia macierz przekszta³cenia na macierz identycznoœciow¹.*/
	MeshPartObject( )
		: ResourceObject( 0 )		//W tym przypadku identyfikator nie ma znaczenia
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

/**@brief Struktura opisuj¹ca pojedyncz¹ czêœæ mesha gotow¹ do wyœwietlenia.
@deprecated Zobacz @ref MeshAsset
@ingroup Resources
@ingroup GraphicAPI

Meshe s¹ przechowywane w czêœciach, poniewa¿ do ró¿nych wierzcho³ków mog¹ byæ przypisane ró¿ne
materia³y, tekstury i inne elementy. Ta struktura zawiera wskaŸniki na te elementy.

Struktura nie zawiera bufora wierzcho³ków ani bufora indeksów. S¹ one przechowywane zewnêtrznie
w klasie Model3DFromFile lub Dynamic_mesh_object i na jeden mesh przypada tylko jeden bufor wierzcho³ków i maksymalnie
jeden bufor indeksów (mo¿e go nie byæ). 

Obecnoœc bufora indeksów nie oznacza, ¿e ca³y mesh jest wyœwietlany w trybie indeksowanym. Mozliwe jest mieszanie trybów.
Tryb odnosi siê wiêc nie do ca³ego mesha, a pojednyczego obiektu ModelPart.

Tablica texture zawiera wskaŸniki na obiekty tekstur, których maksymalna liczba wynosi ENGINE_MAX_TEXTURES.
Aby obs³u¿yæ wszystkie tekstury jakie mog¹ byc przypisane do obiektu, nale¿y podaæ mu odpowiedni shader, który
umie to zrobiæ. Znaczenie poszczególnych pól tablicy tekstur jest opisywane przez enumeracjê TextureUse
i w taki sposób wykorzystuj¹ je domyœlne shadery.
*/
struct ModelPart
{
	VertexShader*			vertex_shader;
	PixelShader*			pixel_shader;
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

//----------------------------------------------------------------------------------------------//
//								TextureObject													//
//----------------------------------------------------------------------------------------------//

/**@brief Tryby filtrowania tekstur.

Poni¿sze tryby filtrowania s¹ u¿ywane przy tworzeniu mipmap.
Istnieje jeszcze drugi etap filtrowania przy próbkowania w pixel shaderze,
do którego odnosi siê inna enumeracja.

@ingroup Resources
@ingroup GraphicAPI
*/
enum class MipMapFilter : short
{
	Box = 0,
	Tent,
	Bell,
	bSpline,
	Mitchell,
	Lanczos3,
	Blackman,
	Lanczos4,
	Lanczos6,
	Lanczos12,
	Kaiser,
	Gaussian,
	Catmullrom,
	QuadraticInterpolation,
	QuadraticAproximation,
	QuadraticMix,

	Unknown
};


/**@brief Deskryptor tekstury.

@ingroup Resources
@ingroup GraphicAPI
*/
struct TextureInfo
{
	uint16				TextureWidth;				///< Szerokoœæ tekstury w pikselach.
	uint16				TextureHeight;				///< Wysokoœæ tekstury w pikselach.
	uint16				ArraySize;					///< Liczba elementów tablicy.
	bool				CPURead : 1;				///< Pozwala na odczyt tekstury przez CPU.
	bool				CPUWrite : 1;				///< Pozwala na zapis tekstury przez CPU.
	bool				AllowShareResource : 1;		///< Pozwala na dostêp do zasoby z wielu API graficznych i pomiêdzy kontekstami.
	bool				IsCubeMap : 1;				///< Nale¿y ustawiæ je¿eli tekstura jest cubemap¹.
	bool				GenerateMipMaps : 1;		///< Automatyczne generowanie mipmap.
	TextureType			TextureType;				///< Typ tekstury (liczba wymiarów, multsampling). Na razie tekstura nie mo¿e byæ inna ni¿ dwuwymiarowa (mo¿e byæ tablic¹).
	ResourceUsage		Usage;						///< Sposób u¿ycia render targetu. Wp³ywa na optymalizacje u³o¿enia w pamiêci.
	ResourceFormat		Format;						///< Format tekstury (liczba kana³ów, liczba bitów na kana³ itp)
	MipMapFilter		MipMapFilter;				///< Tryb filtrowania tekstury. U¿ywany tylko je¿eli ustawiono GenerateMipMaps na true.
	uint16				MipMapLevels;				///< Liczba poziomów mipmap. 1 oznacza tylko teksturê oryginaln¹.
	uint16				CutOffMipMaps;				///< Usuwa podan¹ liczbê poziomów mipmap. Przydatne gdy nie potrzebujemy tekstur zbyt wysokiej rozdzielczoœci (np. stosuj¹c dynamiczny LoD).
													///< Ustawienie wartoœci 1 oznacza, ¿e oryginalna tekstura zostanie zast¹piona pierwsz¹ mipmap¹ w kolejnoœci.

	uint32				MemorySize;					///<Pamiêæ zajmowana przez teksturê.
	filesystem::Path	FilePath;					///< Œcie¿ka do pliku z tekstur¹ lub jej nazwa.
	
	TextureInfo()
	{
		ArraySize = 1;
		CPURead = false;
		CPUWrite = false;
		AllowShareResource = false;
		IsCubeMap = false;
		GenerateMipMaps = false;
		Usage = ResourceUsage::RESOURCE_USAGE_DEFAULT;
		MipMapFilter = MipMapFilter::Unknown;
		MipMapLevels = 1;
		CutOffMipMaps = 0;
		TextureType = TextureType::TEXTURE_TYPE_TEXTURE2D;
		MemorySize = 0;
	}
};

/** @brief Klasa przechowuj¹ca tekstury.
@ingroup Resources
@ingroup GraphicAPI

Klasa bazowa, która bêdzie u¿ywana przez obiekty silnika.
Powinny po niej odziedziczyæ obiekty konkretnego API graficznego,
¿eby zaimplementowaæ najwa¿niejsze funkcjonalnoœci.*/
class TextureObject : public ResourceObject
{
	friend ObjectDeleter<TextureObject>;
private:
protected:
	/// ¯eby unikn¹æ pomy³ki, obiekt mo¿e byœ kasowany tylko przez ModelsManager. Zapewnia to ObjectDeleter.
	virtual ~TextureObject() = default;
public:
	TextureObject() : ResourceObject( 0 ) {}

	virtual const filesystem::Path&		GetFilePath		() const = 0;		///<Zwraca nazwê pliku, który pos³u¿y³ do stworzenia obiektu

	virtual MemoryChunk					CopyData		() const = 0;		///<Kopiuje dane z bufora i umieszcza je w zwracanym MemoryChunku.
	virtual const TextureInfo&			GetDescriptor	() const = 0;		///<Pozwala pobraæ deskrytpro tekstury.

	inline bool operator==( TextureObject& object );
	inline bool operator==( const std::wstring& file_name );
};

//----------------------------------------------------------------------------------------------//
//								RenderTargetObject												//
//----------------------------------------------------------------------------------------------//

/**@brief Struktura u¿ywana do tworzenia render targetu.
@ingroup GraphicAPI*/
struct RenderTargetDescriptor
{
	uint16				TextureWidth;				///<Szerokoœæ tekstury w pikselach.
	uint16				TextureHeight;				///<Wysokoœæ tekstury w pikselach.
	uint16				ArraySize;					///<Liczba elementów tablicy.
	bool				CPURead : 1;				///<Pozwala na odczyt tekstury przez CPU.
	bool				CPUWrite : 1;				///<Pozwala na zapis tekstury przez CPU.
	bool				AllowShareResource : 1;		///<Pozwala na dostêp do zasoby z wielu API graficznych i pomiêdzy kontekstami.
	bool				IsCubeMap : 1;				///<Nale¿y ustawiæ je¿eli tekstura jest cubemap¹.
	uint8				NumSamples;					///<Liczba próbek w przypadku stosowania multisamplingu.
	uint16				SamplesQuality;				///<Jakoœæ próbek przy multisamplingu.
	TextureType			TextureType;				///<Typ tekstury (liczba wymiarów, multsampling). Tekstura nie mo¿e byæ inna ni¿ dwuwymiarowa (mo¿e byæ tablic¹).
	ResourceFormat		ColorBuffFormat;			///<Format bufora kolorów.
	DepthStencilFormat	DepthStencilFormat;			///<Format bufora g³êbokoœci i stencilu.
	ResourceUsage		Usage;						///<Sposób u¿ycia render targetu. Wp³ywa na optymalizacje u³o¿enia w pamiêci.

	/**@brief Ustawia domyœlne wartoœci deskryptora.
	
	Ustawiane s¹ pola CPURead, CPUWrite, AllowShareResource, IsCubeMap, Usage.
	Te zmienne s¹ u¿ywane rzadko i dlatego powinny mieæ takie wartoœci, ¿eby nie trzeba by³o ich jawnie ustawiaæ.
	Pozosta³e wartoœci u¿ytkownik i tak musi zdefiniowaæ samemu, wiêc nie ma co nadk³adaæ pracy.
	
	Pola NumSamples i SamplesQuality s¹ ignorowane, je¿eli TextureType nie zosta³ ustawiony na teksturê z multisamplingiem.
	Pole ArraySize jest ignorowane, je¿eli tekstura nie jest tablic¹.*/
	RenderTargetDescriptor()
	{
		CPURead = 0;
		CPUWrite = 0;
		AllowShareResource = 0;
		IsCubeMap = 0;
		Usage = ResourceUsage::RESOURCE_USAGE_DEFAULT;
	}

	/**@brief Tworzy strukture TextureInfo wype³nion¹ danymi zgodnymi z deskryptorem RenderTargetu.
	
	@attention Funkcja nie ustawia formatu tekstury. Nie da siê wywnioskowaæ formatu na podstawie deskryptora.*/
	TextureInfo		CreateTextureInfo() const
	{
		TextureInfo texInfo;
		texInfo.TextureWidth = TextureWidth;
		texInfo.TextureHeight = TextureHeight;
		texInfo.ArraySize = ArraySize;
		texInfo.CPURead = CPURead;
		texInfo.CPUWrite = CPUWrite;
		texInfo.AllowShareResource = AllowShareResource;
		texInfo.IsCubeMap = IsCubeMap;
		texInfo.TextureType = TextureType;
		texInfo.Usage = Usage;

		return texInfo;
	}
};

/**@brief Klasa dla render targetów.
@ingroup Resources
@ingroup GraphicAPI

Klasa umo¿liwia pobranie jednej z tekstur sk³adowych i udostêpnienie dla shaderów.
Je¿eli API graficzne nie pozwala na oddzielne trzymanie bufora g³êbokoœci i stencilu,
to mo¿e tu byæ przechowywany ten sam obiekt. Ewentualnie mog¹ byæ to dwa obiekty,
które przechowuj¹ inny widok, ale fizycznie odwo³uj¹ siê do tej samej pamiêci.*/
class RenderTargetObject : public IRenderTarget
{
private:
protected:
	TextureObject*			m_colorBuffer;			///<Pozwala na dostêp do bufora kolorów dla innych obiektów. Mo¿e byæ nullptrem.
	TextureObject*			m_depthBuffer;			///<Pozwala na dostêp do bufora g³êbokoœci. Mo¿e byæ nullptrem.
	TextureObject*			m_stencilBuffer;		///<Pozwala na dostêp do bufora stencil. Mo¿e byæ nulltrem.
public:
	RenderTargetObject( TextureObject* colorBuffer, TextureObject* depthBuffer, TextureObject* stencilBuffer );
	virtual ~RenderTargetObject();

	inline TextureObject*		GetColorBuffer()			{ return m_colorBuffer; }		///<Zwraca obiekt bufora kolorów.
	inline TextureObject*		GetDepthBuffer()			{ return m_depthBuffer; }		///<Zwraca obiekt bufora g³êbokoœci.
	inline TextureObject*		GetStencilBuffer()			{ return m_stencilBuffer; }		///<Zwraca obiekt bufora stencilu.
};

//----------------------------------------------------------------------------------------------//
//								ShaderInputLayout											//
//----------------------------------------------------------------------------------------------//

/**@brief Klasa przechowuje layout wierzcho³ka trafiaj¹cego do
vertex shadera.
@ingroup Resources
@ingroup GraphicAPI*/
class ShaderInputLayout : public IShaderInputLayout
{
	friend ObjectDeleter< ShaderInputLayout >;
private:
protected:
	virtual ~ShaderInputLayout() = default;
public:
	ShaderInputLayout() = default;
};

/**@brief Klasa przechowuje opis layoutu wierzcho³ka, na podstawie którego
tworzony jest obiekt layoutu.
@ingroup GraphicAPI*/
class InputLayoutDescriptor
{
private:
	std::wstring				m_inputLayoutName;
protected:
public:
	InputLayoutDescriptor( const std::wstring& layoutName ) : m_inputLayoutName( layoutName ){}
	virtual ~InputLayoutDescriptor() = default;

	virtual void		AddRow		( const char* semanticName, ResourceFormat format, unsigned int inputSlot, unsigned int byteOffset, bool perInstance, unsigned int instanceDataStep ) = 0;

	std::wstring&		GetName		() { return m_inputLayoutName; }
};


/**@brief Typ shadera.
@ingroup GraphicAPI*/
enum class ShaderType
{
	VertexShader,
	PixelShader,
	GeometryShader,
	TesselationControlShader,
	TesselationEvaluationShader,
	ComputeShader
};

//----------------------------------------------------------------------------------------------//
//								VertexShader													//
//----------------------------------------------------------------------------------------------//

/** @brief Klasa przechowuj¹ca vertex shader.
@ingroup Resources
@ingroup GraphicAPI*/
class VertexShader : public IShader
{
	friend ObjectDeleter< VertexShader >;
private:
protected:
	~VertexShader() = default;
public:
	VertexShader() = default;
};

//----------------------------------------------------------------------------------------------//
//								PixelShader														//
//----------------------------------------------------------------------------------------------//

/**@brief Klasa przechowuj¹ca pixel shader.
@ingroup Resources
@ingroup GraphicAPI*/
class PixelShader : public IShader
{
	friend ObjectDeleter< PixelShader >;
private:
protected:
	~PixelShader() = default;
public:
	PixelShader() = default;
};

//----------------------------------------------------------------------------------------------//
//								GeometryShader													//
//----------------------------------------------------------------------------------------------//

/**@brief Klasa przechowuj¹ca pixel shader.
@ingroup Resources
@ingroup GraphicAPI*/
class GeometryShader : public IShader
{
	friend ObjectDeleter<GeometryShader>;
private:
protected:
	~GeometryShader() = default;
public:
	GeometryShader() = default;
};

//----------------------------------------------------------------------------------------------//
//								ControlShader													//
//----------------------------------------------------------------------------------------------//

/**@brief Klasa przechowuj¹ca pixel shader.
@ingroup Resources
@ingroup GraphicAPI*/
class ControlShader : public IShader
{
	friend ObjectDeleter< ControlShader >;
private:
protected:
	~ControlShader() = default;
public:
	ControlShader() = default;
};

//----------------------------------------------------------------------------------------------//
//								EvaluationShader												//
//----------------------------------------------------------------------------------------------//

/**@brief Klasa przechowuj¹ca pixel shader.
@ingroup Resources
@ingroup GraphicAPI*/
class EvaluationShader : public IShader
{
	friend ObjectDeleter< EvaluationShader >;
private:
protected:
	~EvaluationShader() = default;
public:
	EvaluationShader() = default;
};

//----------------------------------------------------------------------------------------------//
//								ComputeShader													//
//----------------------------------------------------------------------------------------------//

/**@brief Klasa przechowuj¹ca compute shader
@ingroup Resources
@ingroup GraphicAPI*/
class ComputeShader : public IShader
{
	friend ObjectDeleter<ComputeShader>;
private:
protected:
	~ComputeShader() = default;
public:
	ComputeShader() = default;
};

//----------------------------------------------------------------------------------------------//
//								BufferObject													//
//----------------------------------------------------------------------------------------------//

/**@brief Obiekt opakowuj¹cy bufor.
@ingroup Resources
@ingroup GraphicAPI

Bufor mo¿e byæ zarówno buforem wierzcho³ków, indeksów jak i sta³ych.
*/
class BufferObject : public IBuffer
{
	friend ObjectDeleter<BufferObject>;
protected:
	unsigned int		m_elementSize;			///<Rozmiar elementu.
	unsigned int		m_elementCount;			///<Liczba elementów.

	~BufferObject() = default;
public:
	BufferObject( unsigned int elementSize, unsigned int elementCount );

	inline unsigned int GetStride()				{ return m_elementSize; }		///<Zwraca rozmiar pojedynczego elementu w buforze.
	inline unsigned int	GetElementSize()		{ return m_elementSize; }		///<Zwraca rozmiar pojedynczego elementu w buforze.
	inline unsigned int GetElementCount()		{ return m_elementCount; }		///<Zwraca liczbê elementów w buforze.
};

//----------------------------------------------------------------------------------------------//
//								MaterialObject													//
//----------------------------------------------------------------------------------------------//


/**@brief Struktura przechowuj¹ca materia³.
@ingroup Resources
@ingroup GraphicAPI

DirectX 11 nie ma w³asnych obiektów na materia³y, poniewa¿ nie ma ju¿ domyœlnego
potoku przetwarzania na karcie graficznej. Na wszystko trzeba napisaæ shader i dostarcza
mu siê takie dane, jakie siê chce dostarczyæ. Dlatego informacja o materia³ach bêdzie
przekazywana z buforze sta³ych.

Struktura zachowuje siê jak asset w zwi¹zku z czym mo¿e
byæ wspó³dzielona przez wiele obiektów. Z tego wzglêdu nie mo¿na jej u¿yæ bezpoœrednio w ConstantPerMesh,
poniewa¿ nie chcemy przekazywaæ do bufora sta³ych zmiennych odziedziczonych po ResourceObject.
Zamiast tego trzeba tê strukture przepisaæ.

Zwracam uwagê, ¿e tylko kana³ Diffuse jest wektorem 4 wymiarowym, w którym sk³adowa w jest odpowiedzialna
za przezroczystoœæ. Pozosta³e s¹ takie tylko dlatego, ¿e jest to domyœlny format przechowywania danych 
w rejestrach karty graficznej i przyspiesza to operacjê kopiowania.
@see ConstantPerFrame
*/
typedef struct MaterialObject : public ResourceObject
{
	friend ObjectDeleter<MaterialObject>;

	DirectX::XMFLOAT4		Diffuse;		//Sk³adowa przezroczystoœci odnosi siê do ca³ego materia³u
	DirectX::XMFLOAT4		Ambient;
	DirectX::XMFLOAT4		Specular;
	DirectX::XMFLOAT4		Emissive;
	float					Power;

	MaterialObject( unsigned int id = WRONG_ID ) : ResourceObject( id ){}
	MaterialObject( const MaterialObject* material );

	void SetNullMaterial();
} MaterialObject;


