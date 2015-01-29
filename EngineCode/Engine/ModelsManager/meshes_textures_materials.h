#pragma once


#include "..\..\stdafx.h"
#include "..\DX11_interfaces_container.h"
#include "ResourceContainer.h"

//definicje
#define WRONG_ID						0
#define WRONG_MODEL_FILE_ID				WRONG_ID
#define WRONG_MESH_ID					WRONG_ID
#define WRONG_TEXTURE_ID				WRONG_ID
#define WRONG_MATERIAL_ID				WRONG_ID


//-------------------------------------------------------------------------------//
//							wersja DirectX11
//-------------------------------------------------------------------------------//


class ModelsManager;
struct ModelPart;
class BufferObject;
class TextureObject;
class VertexShaderObject;
class PixelShaderObject;
struct MeshPartObject;
struct MaterialObject;

//-------------------------------------------------------------------------------//
//	definicje wierzcho³ków



// Strutkura dla standardowego wierzcho³ka
typedef struct VertexNormalTexCord1
{
	DirectX::XMFLOAT3	position;
	DirectX::XMFLOAT3	normal;
	DirectX::XMFLOAT2	tex_cords;
} VertexNormalTexCord1;

// Opis s³u¿¹cy do stworzenia layoutu zrozumia³ego dla shaderów w pliku vertex_layouts.cpp
extern D3D11_INPUT_ELEMENT_DESC VertexNormalTexCord1_desc[];
extern unsigned int VertexNormalTexCord1_desc_num_of_elements;

// Struktura stworzona z myœl¹ o GUI
typedef struct VertexTexCord1
{
	DirectX::XMFLOAT3	position;
	DirectX::XMFLOAT2	tex_cords;
} VertexTexCord1;

// Opis s³u¿¹cy do stworzenia layoutu zrozumia³ego dla shaderów w pliku vertex_layouts.cpp
extern D3D11_INPUT_ELEMENT_DESC VertexTexCord1_desc[];
extern unsigned int VertexTexCord1_desc_num_of_elements;


//-------------------------------------------------------------------------------//
//	Enumeracje dla klasy Model3DFromFile i wszystkich obiektów zasobów


/*Indeksy tekstur w tablicy ModelPart.
S¹ to wartoœci domyœlne u¿ywane przez wbudowane shadery.
W przypadku w³asnorêcznie pisanych shaderów nie trzeba siê trzymaæ
tych sta³ych.*/
typedef enum
{
	TEX_DIFFUSE,
	TEX_AMBIENT,
	TEX_SPECULAR,
	TEX_LIGHTMAP,
	TEX_BUMP_MAP,
	TEX_OTHER1,
	TEX_OTHER2,
	TEX_OTHER3
} TEXTURES_TYPES;


typedef enum VERTEX_BUFFER_OFFSET
{
	LAST = -1,
	BEGIN = 0,
};


/*Obiekty MaterialObject, TextureObject i MeshObject przechowuj¹ odpowiednio dane materia³ów,
*dane tekstur lub bufor wierzcho³ków.
*
*Do ka¿dego pojedynczego obiektu mog¹ istnieæ wilokrotne odwo³ania w klasie Model3DFromFile,
*a tak¿e w obiektach dziedzicz¹cych po Dynamic_mesh_object.
*Z tego wzglêdu istniej¹ zmienne file_references i object_references.
*Pierwsza okreœla, ile razy wystêpuj¹ odwo³ania do obiektu w klasie Model3DFromFile.
*(uwaga: nie zliczamy, ile klas siê odwo³uje. Je¿eli w klasie pojawiaj¹ siê 3 odwo³ania, to licz¹ siê jako 3. Taka konwencja
*u³atwia zliczanie i zwalnianie pamiêci)
*Druga zmienna okreœla, ile wystepuje odwo³añ bezpoœrednich przez obiekty, które bêd¹ nastêpnie wyœwietlane. Oznacza to, ¿e przypisuj¹c
*jakiemus obiektowi plik z modelem, musimy zinkrementowaæ other_references o tyle, ile by³o odwo³añ w tym pliku.
*¯aden obiekt nie powinien byæ kasowany, dopóki istniej¹ do niego odwo³ania.

Zmienna unique_id jest na pocz¹tku ustawiana na 0. Jej faktyczne ustawienie odbywa robi klasa ResourceContainer.
Jest to wymagane do u³atwienia wielow¹tkowoœci. Inaczej mog³yby siê zacz¹æ powtarzaæ identyfikatory.
**/

class referenced_object
{//definicja w pliku Model3DFormFile
	friend ModelsManager;
private:
	unsigned int	file_references;	//liczba plików, które sie odwo³uj¹
	unsigned int	object_references;	//liczba modeli, które siê odwo³uj¹
	unsigned int	unique_id;			//unikalny identyfikator materia³u

protected:
	virtual ~referenced_object( );		//Nie ka¿dy mo¿e skasowaæ obiekt

	inline void set_id( unsigned int id ) { unique_id = id; }
public:
	referenced_object( int id );


	//sprawdza czy mo¿na zwolniæ zmienn¹
	bool can_delete( unsigned int& file_ref, unsigned int& other_ref );

	/*Funkcje s³u¿¹ce do zarz¹dzania odwo³aniami.
	*Nale¿y pilnowaæ, aby wszystkie funkcje, które modyfikuj¹ jakiekolwiek przypisania obiektów
	*do tekstur, materia³ów i meshy, modyfikowa³y równie¿ iloœæ odwo³añ.
	*U¿ytkownik silnika powinien mieæ udostêpnion¹ wartstwê poœredniczac¹, ¿eby nie musia³
	*pamiêtaæ o odwo³aniach.*/
	inline void add_file_reference( ) { ++file_references; }
	inline void add_object_reference( ) { ++object_references; }
	inline void delete_file_reference( ) { --file_references; }
	inline void delete_object_reference( ) { --object_references; }

	inline unsigned int get_id() { return unique_id; }
};





/*Struktura opisuje pojedyncz¹ cz¹stkê mesha o jednym materiale, teksturze i shaderach.
W zale¿noœci od zawartoœci pola index_buffer w strukturze ModelPart, mesh jest wyœwietlany w trybie
indeksowanym lub nie.

Je¿eli wartoœæ tego pola wynosi nullptr, to wtedy u¿ywane s¹
zmienne buffer offset i vertices count, które jednoznacznie wskazuj¹, która czêœæ bufora
wierzcho³ków ma zostaæ wyœwietlona.

Je¿eli wskaŸnik index_buffer wskazuje na obiekt, to wtedy u¿ywany jest tryb indeksowany
i zmienne buffer_offset i vertices_count s¹ u¿ywane wzglêdem bufora indeksów.

Klasa jest alokowana w Model3DFromFile i to w³asnie ta klasa odpowiada za zwolnienie pamiêci.

Pomimo dziedziczenia po klasie referenced_object, nie jest u¿ywane pole unique_id. Dlatego
jest ono w kontruktorze ustawiane na 0. MeshPartObjecty nie mog¹ byc wspó³dzielone
miêdzy obiektami.*/
struct MeshPartObject : public referenced_object
{
	DirectX::XMFLOAT4X4		transform_matrix;	//macierz przekszta³cenia wzglêdem œrodka modelu
	unsigned int			buffer_offset;
	unsigned int			vertices_count;

	MeshPartObject( )
		: referenced_object( 0 )		//W tym przypadku identyfikator nie ma znaczenia
	{
		buffer_offset = 0;
		vertices_count = 0;
		//domyœlnie nie wykonujemy ¿adnego przekszta³cenia
		DirectX::XMMATRIX identity = DirectX::XMMatrixIdentity( );
		XMStoreFloat4x4( &transform_matrix, identity );
	}
};

/*Meshe s¹ przechowywane w czêœciach, poniewa¿ do ró¿nych wierzcho³ków mog¹ byæ przypisane ró¿ne
materia³y, tekstury i inne elementy. Ta struktura zawiera wszystkie mo¿liwe elementy dla pojedynczej
czêœci mesha.

Je¿eli index_buffer jest nullptrem, oznacza to, ¿e u¿ywany jest tryb bezpoœredni dostêpu
do bufora wierzcho³ków, je¿eli jest tam wskaŸnik na bufor indeksów, to u¿ywany jest tryb indeksowany.

Zasadniczo bufor indeksów podobnie jak bufor wierzcho³ków, wystêpuje jeden na ca³y mesh.
Jednak nie ca³y mesh musi byæ nim indeksowany, niektóre fragmenty mog¹ byæ wyœwietlane w
trybie bezpoœrednim. Dlatego wskaŸnik na bufor indeksów znajduje sie na tym poziomie, ¿eby
informowaæ, który tryb jest u¿ywany.*/
struct ModelPart
{
	VertexShaderObject*		vertex_shader;
	PixelShaderObject*		pixel_shader;
	MaterialObject*			material;
	TextureObject*			texture[ENGINE_MAX_TEXTURES];
	MeshPartObject*			mesh;
	BufferObject*			index_buffer;

	ModelPart( )
	{
		vertex_shader = nullptr;
		pixel_shader = nullptr;
		material = nullptr;
		mesh = nullptr;
		index_buffer = nullptr;
		texture[0] = nullptr;
		texture[1] = nullptr;
		texture[2] = nullptr;
		texture[3] = nullptr;
		texture[4] = nullptr;
		texture[5] = nullptr;
		texture[6] = nullptr;
		texture[7] = nullptr;
	}
};


/*Istniej¹ obiekty nazywaj¹ce siê ID3D11Texture2D i s¹ teksturami, ale stworzenie takiego
nie wystarczy, ¿eby shader móg³ go u¿yæ. Do tego trzeba stworzyæ widok i w³aœnie jego
przechowuje ta funkcja.*/
class TextureObject : public referenced_object, public DX11_interfaces_container
{
	friend TextureObject;
	friend ModelsManager;
	friend ResourceContainer<TextureObject*>;
private:
	std::wstring					file_path;			//œcie¿ka do tekstury (nie do pliku z meshem)
	ID3D11ShaderResourceView*		texture;
protected:
	//¯eby unikn¹æ pomy³ki, obiekt mo¿e byœ kasowany tylko przez ModelsManager.
	~TextureObject( ) override;
public:
	TextureObject( unsigned int id );
	TextureObject( unsigned int id, const std::wstring& path, ID3D11ShaderResourceView* tex );

	bool operator==(const TextureObject& object);
	bool operator==(const std::wstring& file_name);

	inline ID3D11ShaderResourceView* get( ) { return texture; }
	static TextureObject* create_from_file( const std::wstring& file_name );
};


class VertexShaderObject : public referenced_object, public DX11_interfaces_container
{
	friend ModelsManager;
	friend ResourceContainer<VertexShaderObject*>;
private:
	ID3D11VertexShader*		vertex_shader;
protected:
	~VertexShaderObject( ) override;
public:
	VertexShaderObject();
	VertexShaderObject( ID3D11VertexShader* shader ) : referenced_object( WRONG_ID ), vertex_shader( shader ){}

	inline ID3D11VertexShader* get( ) { return vertex_shader; }
	static VertexShaderObject* create_from_file( const std::wstring& file_name, const std::string& shader_name );
};


class PixelShaderObject : public referenced_object, public DX11_interfaces_container
{
	friend ModelsManager;
	friend ResourceContainer<PixelShaderObject*>;
private:
	ID3D11PixelShader*		pixel_shader;
protected:
	~PixelShaderObject( ) override;
public:
	PixelShaderObject();
	PixelShaderObject( ID3D11PixelShader* shader ) : referenced_object( WRONG_ID ), pixel_shader( shader ){}

	inline ID3D11PixelShader* get( ) { return pixel_shader; }
	static PixelShaderObject* create_from_file( const std::wstring& file_name, const std::string& shader_name );
};

/*Bufor wierzcho³ków i bufor u¿ywaj¹ tego samego interfejsu ID3D11Buffer,
dlatego nie ma oddzielnych klas.

Obiekty tego typu mog¹ tak¿e s³u¿yæ do przekazywania parametrów shaderom.*/
class BufferObject : public referenced_object, public DX11_interfaces_container
{
	friend ModelsManager;
	friend ResourceContainer<BufferObject*>;
private:
	ID3D11Buffer*		buffer;
protected:
	~BufferObject( ) override;
public:
	BufferObject( unsigned int id );

	inline ID3D11Buffer* get( ) { return buffer; }
};


/*DirectX 11 nie ma w³asnych obiektów na materia³y, poniewa¿ nie ma ju¿ domyœlnego
potoku przetwarzania na karcie graficznej. Na wszystko trzeba napisaæ shader i dostarcza
mu siê takie dane, jakie siê chce dostarczyæ. Dlatego informacja o materia³ach bêdzie
przekazywana z buforze sta³ych.*/
typedef struct MaterialObject : public referenced_object
{
	friend ResourceContainer<MaterialObject*>;

	DirectX::XMFLOAT4		Diffuse;		//Sk³adowa przezroczystoœci odnosi siê do ca³ego materia³u
	DirectX::XMFLOAT3		Ambient;
	DirectX::XMFLOAT3		Specular;
	DirectX::XMFLOAT3		Emissive;
	float					Power;

	MaterialObject( unsigned int id = WRONG_ID ) : referenced_object( id ){}
	MaterialObject( const MaterialObject* material );
} MaterialObject;



//-------------------------------------------------------------------------------//
//	Model3DFronFile



/*Struktury tymczasowe dla klasy Model3DFromFile, u¿ywane podczas wype³niania
obiektu danymi. Wszystkie te struktury s¹ kasowane po zakoñczeniu edycji.*/

struct TMP_data
{
	VertexNormalTexCord1*		vertices_tab	= nullptr;
	unsigned int				vertices_count	= 0;
	unsigned int*				indicies_tab	= nullptr;
	unsigned int				indicies_count	= 0;
	unsigned int				indicies_offset	= 0;	//offset w buforze wierzcho³ków, wzglêdem którego lic¿a siê indexy 
	ModelPart					new_part;
};


struct EditTMP
{
	int			current_pointer		= 0;
	TMP_data**	table			= nullptr;
	int			table_length	= 20;
};

/*Klasa s³u¿y do przechowywania danych modelu wczytanego z pliku.
*Dziêki temu po ponownym u¿yciu pliku dla innego obiektu, nie musimy wczytywaæ modelu ponownie.
*Plik(i model jednoczeœnie) mo¿e byæ identyfikowany po œcie¿ce do pliku lub unikalnym identyfikatorem.
*
*Klasa zawiera tablicê elementów ModelPart, które przechowuj¹ informacje potrzebne do wyrenderowania
*poszczególnych czêœci mesha. Meshe musza byæ trzymane w czêœciach, ¿eby mo¿na by³o ka¿dej z tych czêœci
*przypisaæ ró¿ne materia³y i parametry. Gdyby nie taka struktura, to wczytanie pliku nie by³oby mo¿liwe.
*Meshe rzadko sk³adaj¹ siê z jednego kawa³ka.
*
*Wszystkie wierzcho³ki przechowywane s¹ tylko w jednym buforze vertex_buffer. Na podstawie zaleceñ
*w dokumentacji DrectXa, lepiej jest mieæ jeden du¿y bufor ni¿ wiele ma³ych.
*Poszczególne czêœci mesha zawieraj¹ albo bufor indeksów, który decyduje, które wierzcho³ki
*s¹ wyœwietlane, albo odpowiedni offset od pocz¹tku bufora i iloœæ trójk¹tów.
*Mo¿liwe jest równie¿, ¿e mesh bêdzie wyœwietlany w trybie mieszanym, u¿ywaj¹c
*dla ka¿dej czêœci raz bufora indeksów, a raz offsetów.*/

class Model3DFromFile : public referenced_object
{
	friend class ModelsManager;
	friend ResourceContainer<Model3DFromFile*>;
private:
	static ModelsManager* models_manager;

	//tekstura i materia³ odpowiadaj¹ meshowi spod danego indeksu
	std::vector<ModelPart>			model_parts;
	BufferObject*					vertex_buffer;
	std::wstring					file_path;

	//wskaxnik na tymczasow¹ strukture, która bêdzie u¿ywana podczas wype³niania obiektu danymi
	EditTMP*						tmp_data;

protected:
	//¯eby unikn¹æ pomy³ki, obiekt mo¿e byœ kasowany tylko przez ModelsManager.
	~Model3DFromFile() override;

	unsigned int get_buffer_offset_to_last();
public:
	Model3DFromFile( const std::wstring& name );


	// Tworzenie obiektu
	void BeginEdit();
	void EndEdit();
	void BeginPart();
	void EndPart();

	unsigned int add_texture( const std::wstring& file_name, TEXTURES_TYPES type = TEX_DIFFUSE );
	unsigned int add_material( const MaterialObject* material, const std::wstring& material_name );
	unsigned int add_vertex_shader( const std::wstring& file_name );
	unsigned int add_pixel_shader( const std::wstring& file_name );
	unsigned int add_vertex_buffer( const VertexNormalTexCord1* buffer, unsigned int vert_count );
	void add_transformation( const DirectX::XMFLOAT4X4& transform );
	unsigned int add_index_buffer( const unsigned int* buffer, unsigned int ind_count, int vertex_buffer_offset );


	unsigned int get_parts_count( );
	const ModelPart* get_part( unsigned int index );

	inline BufferObject* get_vertex_buffer() { return vertex_buffer; }
};




#ifndef __UNUSED


//-------------------------------------------------------------------------------//
//					wersja DirectX 9 ze star¹ struktur¹ danych
//-------------------------------------------------------------------------------//


class ModelsManager;
class TextureObject;
class MaterialObject;
class MeshObject;
class MeshPart;
class referenced_object;

//-------------------------------------------------------------------------------//
//	definicje wierzcho³ków

using namespace DirectX;

//#define		VERTEX_NORMAL_TEXTCORDS1		(D3DFVF_XYZ  | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(0))
#define		VERTEX_NORMAL_TEXTCORDS1		(D3DFVF_XYZ  | D3DFVF_NORMAL | D3DFVF_TEX1)
typedef struct VertexFormat1
{
	XMFLOAT3	position;
	XMFLOAT3	normal;
	XMFLOAT2	tex_cords1;

	bool operator==(VertexFormat1& vertex2);
	bool operator!=(VertexFormat1& vertex2);
} Vertex_Normal_TexCords1;



#define		VERTEX_NORMAL_TEXTCORDS2		(D3DFVF_XYZ  | D3DFVF_NORMAL | D3DFVF_TEX2 | D3DFVF_TEXCOORDSIZE2(0) | D3DFVF_TEXCOORDSIZE2(1))
typedef struct VertexFormat2
{
	XMFLOAT3	position;
	XMFLOAT3	normal;
	XMFLOAT2	tex_cords1;
	XMFLOAT2	tex_cords2;

	bool operator==(VertexFormat2& vertex2);
	bool operator!=(VertexFormat2& vertex2);
} Vertex_Normal_TexCords2;


#define		VERTEX_TEXTCORDS1		(D3DFVF_XYZ  | D3DFVF_TEX2 | D3DFVF_TEXCOORDSIZE2(0))
typedef struct VertexFormat3
{
	XMFLOAT3	position;
	XMFLOAT2	tex_cords1;

	bool operator==(VertexFormat3& vertex2);
	bool operator!=(VertexFormat3& vertex2);
} Vertex_TexCords1;

typedef Vertex_TexCords1 VertexUI;




/*Klasa s³u¿y do przechowywania danych modelu wczytanego z pliku.
*Dziêki temu po ponownym u¿yciu pliku dla innego obiektu, nie musimy wczytywaæ modelu ponownie.
*Plik(i model jednoczesnie) mo¿e byæ identyfikowany po œcie¿ce do pliku lub unikalnym identyfikatorem.
*
*Konwecja dotycz¹ca wektorów mesh_parts, materials i textures jest taka, ¿e
*meshowi pod danym indeksem w wektorze odpowiadaj¹ dane z pozosta³ych wektorów pod tymi samymi indeksami.
*Wektory przechowuj¹ jedynie wskaŸniki, które mog¹ wystêpowaæ wielokrotnie w jednym wektorze.
*Meshe maj¹ dodatkowo macierz odpowiadaj¹c¹ pierwotnemu przekszta³ceniu, jakiemu nale¿y poddaæ dan¹ czêœæ mesha.
*W ten sposób jedna siatka trojk¹tów, mo¿e byæ wyœwietlona wielokrotnie z ró¿nymi materia³ami i teksturami oraz
*w ró¿nych miejscach, obrócona i przeskalowana.
*
*Pe³ny zbiór meshy, materia³ów i tekstur zawieraj¹ tablice w klasie ModelsManager.
*
*Aby usun¹æ obiekt, powinniœmy miec pewnoœæ, ¿e ¿aden inny obiekt siê do niego nie odwo³uje.
*Do tego s³u¿y zmienna references, przechowuj¹ca liczbê obiektów, które siê odwo³uj¹ oraz funkcja
*can_delete(), która pozwala sprawdziæ tê zmienn¹.
*
*Z wyj¹tkiem zawartoœci wektora mesh_parts, wszystkie pozosta³e obiekty s¹ tylko kopiami wskaŸników
*za zwolnienie pamiêci odpowiada klasa ModelsManager.*/

class Model3DFromFile
{
	friend class ModelsManager;
private:
	static ModelsManager* models_manager;

	unsigned int	references;			//liczba obiektów, które siê odwo³uj¹
	unsigned int	unique_id;			//unikalny identyfikator pliku (mozna siê odwo³ywaæ po nazwie pliku lub po tym id)
	std::string		file_path;

	//tekstura i materia³ odpowiadaj¹ meshowi spod danego indeksu
	std::vector<MeshPart*>			mesh_parts;
	std::vector<TextureObject*>		textures;
	std::vector<MaterialObject*>	materials;

public:
	Model3DFromFile( int id );
	~Model3DFromFile( );

	unsigned int add_material( const D3DMATERIAL9 &material );
	unsigned int add_texture( const std::wstring& path );
	unsigned int add_mesh( Vertex_Normal_TexCords1* vertices, unsigned int vert_num, const DirectX::XMFLOAT4X4& matrix );
	MaterialObject* add_material( unsigned int id );
	TextureObject* add_texture( unsigned int id );
	MeshPart* add_mesh( unsigned int id, DirectX::XMFLOAT4X4& matrix );
	void add_null_material( );
	void add_null_texture( );

	unsigned int get_parts_count( );
	MaterialObject* get_material( unsigned int index );
	TextureObject* get_texture( unsigned int index );
	MeshPart* get_mesh_part( unsigned int index );

	//zarz¹dzanie odwo³aniami
	bool can_delete( unsigned int& ref );
	void add_reference( );
	void delete_reference( );
};



/*Obiekty MaterialObject, TextureObject i MeshObject przechowuj¹ odpowiednio dane materia³ów,
*dane tekstur lub bufor wierzcho³ków.
*
*Do ka¿dego pojedynczego obiektu mog¹ istnieæ wilokrotne odwo³ania w klasie Model3DFromFile,
*a tak¿e w obiektach dziedzicz¹cych po Dynamic_mesh_object.
*Z tego wzglêdu istniej¹ zmienne file_references i object_references.
*Pierwsza okreœla, ile razy wystêpuj¹ odwo³ania do obiektu w klasie Model3DFromFile.
*(uwaga: nie zliczamy, ile klas siê odwo³uje. Je¿eli w klasie pojawiaj¹ siê 3 odwo³ania, to licz¹ siê jako 3. Taka konwencja
*u³atwia zliczanie)
*Druga zmienna okreœla, ile wystepuje odwo³añ bezpoœrednich przez obiekty, które bêd¹ nastêpnie wyœwietlane. Oznacza to, ¿e przypisuj¹c
*jakiemus obiektowi plik z modelem, musimy zinkrementowaæ other_references o tyle, ile by³o odwo³añ w tym pliku.
*¯aden obiekt nie powinien byæ kasowany, dopóki istniej¹ do niego odwo³ania.
**/

class referenced_object
{//definicja w pliku Model3DFormFile
	friend ModelsManager;
private:
	unsigned int	file_references;	//liczba plików, które sie odwo³uj¹
	unsigned int	object_references;	//liczba modeli, które siê odwo³uj¹
	unsigned int	unique_id;			//unikalny identyfikator materia³u
public:
	referenced_object( int id );
	virtual ~referenced_object( );

	//sprawdza czy mo¿na zwolniæ zmienn¹
	bool can_delete( unsigned int& file_ref, unsigned int& other_ref );

	/*Funkcje s³u¿¹ce do zarz¹dzania odwo³aniami.
	*Nale¿y pilnowaæ, aby wszystkie funkcje, które modyfikuj¹ jakiekolwiek przypisania obiektów
	*do tekstur, materia³ów i meshy, modyfikowa³y równie¿ iloœæ odwo³añ.
	*U¿ytkownik silnika powinien mieæ udostêpnion¹ wartstwe poœredniczac¹, ¿eby nie musia³
	*pamiêtaæ o odwo³aniach.*/
	inline void add_file_reference( ) { ++file_references; }
	inline void add_object_reference( ) { ++object_references; }
	inline void delete_file_reference( ) { --file_references; }
	inline void delete_object_reference( ) { --object_references; }
};


class TextureObject : public referenced_object
{
	friend TextureObject;
	friend ModelsManager;
private:
	std::wstring			file_path;			//œciezka do tekstury (nie do pliku z meshem)
public:
	LPDIRECT3DTEXTURE9		texture;

	TextureObject( unsigned int id );
	TextureObject( unsigned int id, const std::wstring& path, LPDIRECT3DTEXTURE9 tex );
	~TextureObject( ) override;

	bool operator==(const TextureObject& object);
	bool operator==(const std::wstring& file_name);

	inline LPDIRECT3DTEXTURE9 get_texture( ) { return texture; }
};


class MeshPart
{
	friend MeshPart;
public:
	MeshObject*				mesh_object;		//klasa zawieraj¹ca wierzcho³ki danego mesha
	DirectX::XMFLOAT4X4		transform_matrix;	//macierz przekszta³cenia wstêpnego
};


class MeshObject : public referenced_object
{
	friend MeshObject;
	friend ModelsManager;
private:
public:
	LPDIRECT3DVERTEXBUFFER9	vertex_buffer;		//bufor wierzcho³ków
	unsigned int			primitives_count;

	MeshObject( int id );
	~MeshObject( );

	inline LPDIRECT3DVERTEXBUFFER9 get_vertex_buffer( ) { return vertex_buffer; }

	//bool operator==(const Vertex_Normal_TexCords1& vertex);
};

class MaterialObject : public referenced_object
{
	friend MaterialObject;
	friend ModelsManager;
private:
	D3DMATERIAL9	material;

public:
	MaterialObject( unsigned int id );
	MaterialObject( unsigned int id, const MaterialObject& object );
	MaterialObject( unsigned int id, const D3DMATERIAL9& material2 );
	~MaterialObject( );

	bool operator==(const MaterialObject& object);
	bool operator==(const D3DMATERIAL9& object);

	inline D3DMATERIAL9* get_material( ) { return &material; }
};


#endif
