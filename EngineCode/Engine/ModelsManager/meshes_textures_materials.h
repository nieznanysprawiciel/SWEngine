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
//	definicje wierzcho�k�w



// Strutkura dla standardowego wierzcho�ka
typedef struct VertexNormalTexCord1
{
	DirectX::XMFLOAT3	position;
	DirectX::XMFLOAT3	normal;
	DirectX::XMFLOAT2	tex_cords;
} VertexNormalTexCord1;

// Opis s�u��cy do stworzenia layoutu zrozumia�ego dla shader�w w pliku vertex_layouts.cpp
extern D3D11_INPUT_ELEMENT_DESC VertexNormalTexCord1_desc[];
extern unsigned int VertexNormalTexCord1_desc_num_of_elements;

// Struktura stworzona z my�l� o GUI
typedef struct VertexTexCord1
{
	DirectX::XMFLOAT3	position;
	DirectX::XMFLOAT2	tex_cords;
} VertexTexCord1;

// Opis s�u��cy do stworzenia layoutu zrozumia�ego dla shader�w w pliku vertex_layouts.cpp
extern D3D11_INPUT_ELEMENT_DESC VertexTexCord1_desc[];
extern unsigned int VertexTexCord1_desc_num_of_elements;


//-------------------------------------------------------------------------------//
//	Enumeracje dla klasy Model3DFromFile i wszystkich obiekt�w zasob�w


/*Indeksy tekstur w tablicy ModelPart.
S� to warto�ci domy�lne u�ywane przez wbudowane shadery.
W przypadku w�asnor�cznie pisanych shader�w nie trzeba si� trzyma�
tych sta�ych.*/
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


/*Obiekty MaterialObject, TextureObject i MeshObject przechowuj� odpowiednio dane materia��w,
*dane tekstur lub bufor wierzcho�k�w.
*
*Do ka�dego pojedynczego obiektu mog� istnie� wilokrotne odwo�ania w klasie Model3DFromFile,
*a tak�e w obiektach dziedzicz�cych po Dynamic_mesh_object.
*Z tego wzgl�du istniej� zmienne file_references i object_references.
*Pierwsza okre�la, ile razy wyst�puj� odwo�ania do obiektu w klasie Model3DFromFile.
*(uwaga: nie zliczamy, ile klas si� odwo�uje. Je�eli w klasie pojawiaj� si� 3 odwo�ania, to licz� si� jako 3. Taka konwencja
*u�atwia zliczanie i zwalnianie pami�ci)
*Druga zmienna okre�la, ile wystepuje odwo�a� bezpo�rednich przez obiekty, kt�re b�d� nast�pnie wy�wietlane. Oznacza to, �e przypisuj�c
*jakiemus obiektowi plik z modelem, musimy zinkrementowa� other_references o tyle, ile by�o odwo�a� w tym pliku.
*�aden obiekt nie powinien by� kasowany, dop�ki istniej� do niego odwo�ania.

Zmienna unique_id jest na pocz�tku ustawiana na 0. Jej faktyczne ustawienie odbywa robi klasa ResourceContainer.
Jest to wymagane do u�atwienia wielow�tkowo�ci. Inaczej mog�yby si� zacz�� powtarza� identyfikatory.
**/

class referenced_object
{//definicja w pliku Model3DFormFile
	friend ModelsManager;
private:
	unsigned int	file_references;	//liczba plik�w, kt�re sie odwo�uj�
	unsigned int	object_references;	//liczba modeli, kt�re si� odwo�uj�
	unsigned int	unique_id;			//unikalny identyfikator materia�u

protected:
	virtual ~referenced_object( );		//Nie ka�dy mo�e skasowa� obiekt

	inline void set_id( unsigned int id ) { unique_id = id; }
public:
	referenced_object( int id );


	//sprawdza czy mo�na zwolni� zmienn�
	bool can_delete( unsigned int& file_ref, unsigned int& other_ref );

	/*Funkcje s�u��ce do zarz�dzania odwo�aniami.
	*Nale�y pilnowa�, aby wszystkie funkcje, kt�re modyfikuj� jakiekolwiek przypisania obiekt�w
	*do tekstur, materia��w i meshy, modyfikowa�y r�wnie� ilo�� odwo�a�.
	*U�ytkownik silnika powinien mie� udost�pnion� wartstw� po�redniczac�, �eby nie musia�
	*pami�ta� o odwo�aniach.*/
	inline void add_file_reference( ) { ++file_references; }
	inline void add_object_reference( ) { ++object_references; }
	inline void delete_file_reference( ) { --file_references; }
	inline void delete_object_reference( ) { --object_references; }

	inline unsigned int get_id() { return unique_id; }
};





/*Struktura opisuje pojedyncz� cz�stk� mesha o jednym materiale, teksturze i shaderach.
W zale�no�ci od zawarto�ci pola index_buffer w strukturze ModelPart, mesh jest wy�wietlany w trybie
indeksowanym lub nie.

Je�eli warto�� tego pola wynosi nullptr, to wtedy u�ywane s�
zmienne buffer offset i vertices count, kt�re jednoznacznie wskazuj�, kt�ra cz�� bufora
wierzcho�k�w ma zosta� wy�wietlona.

Je�eli wska�nik index_buffer wskazuje na obiekt, to wtedy u�ywany jest tryb indeksowany
i zmienne buffer_offset i vertices_count s� u�ywane wzgl�dem bufora indeks�w.

Klasa jest alokowana w Model3DFromFile i to w�asnie ta klasa odpowiada za zwolnienie pami�ci.

Pomimo dziedziczenia po klasie referenced_object, nie jest u�ywane pole unique_id. Dlatego
jest ono w kontruktorze ustawiane na 0. MeshPartObjecty nie mog� byc wsp�dzielone
mi�dzy obiektami.*/
struct MeshPartObject : public referenced_object
{
	DirectX::XMFLOAT4X4		transform_matrix;	//macierz przekszta�cenia wzgl�dem �rodka modelu
	unsigned int			buffer_offset;
	unsigned int			vertices_count;

	MeshPartObject( )
		: referenced_object( 0 )		//W tym przypadku identyfikator nie ma znaczenia
	{
		buffer_offset = 0;
		vertices_count = 0;
		//domy�lnie nie wykonujemy �adnego przekszta�cenia
		DirectX::XMMATRIX identity = DirectX::XMMatrixIdentity( );
		XMStoreFloat4x4( &transform_matrix, identity );
	}
};

/*Meshe s� przechowywane w cz�ciach, poniewa� do r�nych wierzcho�k�w mog� by� przypisane r�ne
materia�y, tekstury i inne elementy. Ta struktura zawiera wszystkie mo�liwe elementy dla pojedynczej
cz�ci mesha.

Je�eli index_buffer jest nullptrem, oznacza to, �e u�ywany jest tryb bezpo�redni dost�pu
do bufora wierzcho�k�w, je�eli jest tam wska�nik na bufor indeks�w, to u�ywany jest tryb indeksowany.

Zasadniczo bufor indeks�w podobnie jak bufor wierzcho�k�w, wyst�puje jeden na ca�y mesh.
Jednak nie ca�y mesh musi by� nim indeksowany, niekt�re fragmenty mog� by� wy�wietlane w
trybie bezpo�rednim. Dlatego wska�nik na bufor indeks�w znajduje sie na tym poziomie, �eby
informowa�, kt�ry tryb jest u�ywany.*/
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


/*Istniej� obiekty nazywaj�ce si� ID3D11Texture2D i s� teksturami, ale stworzenie takiego
nie wystarczy, �eby shader m�g� go u�y�. Do tego trzeba stworzy� widok i w�a�nie jego
przechowuje ta funkcja.*/
class TextureObject : public referenced_object, public DX11_interfaces_container
{
	friend TextureObject;
	friend ModelsManager;
	friend ResourceContainer<TextureObject*>;
private:
	std::wstring					file_path;			//�cie�ka do tekstury (nie do pliku z meshem)
	ID3D11ShaderResourceView*		texture;
protected:
	//�eby unikn�� pomy�ki, obiekt mo�e by� kasowany tylko przez ModelsManager.
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

/*Bufor wierzcho�k�w i bufor u�ywaj� tego samego interfejsu ID3D11Buffer,
dlatego nie ma oddzielnych klas.

Obiekty tego typu mog� tak�e s�u�y� do przekazywania parametr�w shaderom.*/
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


/*DirectX 11 nie ma w�asnych obiekt�w na materia�y, poniewa� nie ma ju� domy�lnego
potoku przetwarzania na karcie graficznej. Na wszystko trzeba napisa� shader i dostarcza
mu si� takie dane, jakie si� chce dostarczy�. Dlatego informacja o materia�ach b�dzie
przekazywana z buforze sta�ych.*/
typedef struct MaterialObject : public referenced_object
{
	friend ResourceContainer<MaterialObject*>;

	DirectX::XMFLOAT4		Diffuse;		//Sk�adowa przezroczysto�ci odnosi si� do ca�ego materia�u
	DirectX::XMFLOAT3		Ambient;
	DirectX::XMFLOAT3		Specular;
	DirectX::XMFLOAT3		Emissive;
	float					Power;

	MaterialObject( unsigned int id = WRONG_ID ) : referenced_object( id ){}
	MaterialObject( const MaterialObject* material );
} MaterialObject;



//-------------------------------------------------------------------------------//
//	Model3DFronFile



/*Struktury tymczasowe dla klasy Model3DFromFile, u�ywane podczas wype�niania
obiektu danymi. Wszystkie te struktury s� kasowane po zako�czeniu edycji.*/

struct TMP_data
{
	VertexNormalTexCord1*		vertices_tab	= nullptr;
	unsigned int				vertices_count	= 0;
	unsigned int*				indicies_tab	= nullptr;
	unsigned int				indicies_count	= 0;
	unsigned int				indicies_offset	= 0;	//offset w buforze wierzcho�k�w, wzgl�dem kt�rego lic�a si� indexy 
	ModelPart					new_part;
};


struct EditTMP
{
	int			current_pointer		= 0;
	TMP_data**	table			= nullptr;
	int			table_length	= 20;
};

/*Klasa s�u�y do przechowywania danych modelu wczytanego z pliku.
*Dzi�ki temu po ponownym u�yciu pliku dla innego obiektu, nie musimy wczytywa� modelu ponownie.
*Plik(i model jednocze�nie) mo�e by� identyfikowany po �cie�ce do pliku lub unikalnym identyfikatorem.
*
*Klasa zawiera tablic� element�w ModelPart, kt�re przechowuj� informacje potrzebne do wyrenderowania
*poszczeg�lnych cz�ci mesha. Meshe musza by� trzymane w cz�ciach, �eby mo�na by�o ka�dej z tych cz�ci
*przypisa� r�ne materia�y i parametry. Gdyby nie taka struktura, to wczytanie pliku nie by�oby mo�liwe.
*Meshe rzadko sk�adaj� si� z jednego kawa�ka.
*
*Wszystkie wierzcho�ki przechowywane s� tylko w jednym buforze vertex_buffer. Na podstawie zalece�
*w dokumentacji DrectXa, lepiej jest mie� jeden du�y bufor ni� wiele ma�ych.
*Poszczeg�lne cz�ci mesha zawieraj� albo bufor indeks�w, kt�ry decyduje, kt�re wierzcho�ki
*s� wy�wietlane, albo odpowiedni offset od pocz�tku bufora i ilo�� tr�jk�t�w.
*Mo�liwe jest r�wnie�, �e mesh b�dzie wy�wietlany w trybie mieszanym, u�ywaj�c
*dla ka�dej cz�ci raz bufora indeks�w, a raz offset�w.*/

class Model3DFromFile : public referenced_object
{
	friend class ModelsManager;
	friend ResourceContainer<Model3DFromFile*>;
private:
	static ModelsManager* models_manager;

	//tekstura i materia� odpowiadaj� meshowi spod danego indeksu
	std::vector<ModelPart>			model_parts;
	BufferObject*					vertex_buffer;
	std::wstring					file_path;

	//wskaxnik na tymczasow� strukture, kt�ra b�dzie u�ywana podczas wype�niania obiektu danymi
	EditTMP*						tmp_data;

protected:
	//�eby unikn�� pomy�ki, obiekt mo�e by� kasowany tylko przez ModelsManager.
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
//					wersja DirectX 9 ze star� struktur� danych
//-------------------------------------------------------------------------------//


class ModelsManager;
class TextureObject;
class MaterialObject;
class MeshObject;
class MeshPart;
class referenced_object;

//-------------------------------------------------------------------------------//
//	definicje wierzcho�k�w

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




/*Klasa s�u�y do przechowywania danych modelu wczytanego z pliku.
*Dzi�ki temu po ponownym u�yciu pliku dla innego obiektu, nie musimy wczytywa� modelu ponownie.
*Plik(i model jednoczesnie) mo�e by� identyfikowany po �cie�ce do pliku lub unikalnym identyfikatorem.
*
*Konwecja dotycz�ca wektor�w mesh_parts, materials i textures jest taka, �e
*meshowi pod danym indeksem w wektorze odpowiadaj� dane z pozosta�ych wektor�w pod tymi samymi indeksami.
*Wektory przechowuj� jedynie wska�niki, kt�re mog� wyst�powa� wielokrotnie w jednym wektorze.
*Meshe maj� dodatkowo macierz odpowiadaj�c� pierwotnemu przekszta�ceniu, jakiemu nale�y podda� dan� cz�� mesha.
*W ten spos�b jedna siatka trojk�t�w, mo�e by� wy�wietlona wielokrotnie z r�nymi materia�ami i teksturami oraz
*w r�nych miejscach, obr�cona i przeskalowana.
*
*Pe�ny zbi�r meshy, materia��w i tekstur zawieraj� tablice w klasie ModelsManager.
*
*Aby usun�� obiekt, powinni�my miec pewno��, �e �aden inny obiekt si� do niego nie odwo�uje.
*Do tego s�u�y zmienna references, przechowuj�ca liczb� obiekt�w, kt�re si� odwo�uj� oraz funkcja
*can_delete(), kt�ra pozwala sprawdzi� t� zmienn�.
*
*Z wyj�tkiem zawarto�ci wektora mesh_parts, wszystkie pozosta�e obiekty s� tylko kopiami wska�nik�w
*za zwolnienie pami�ci odpowiada klasa ModelsManager.*/

class Model3DFromFile
{
	friend class ModelsManager;
private:
	static ModelsManager* models_manager;

	unsigned int	references;			//liczba obiekt�w, kt�re si� odwo�uj�
	unsigned int	unique_id;			//unikalny identyfikator pliku (mozna si� odwo�ywa� po nazwie pliku lub po tym id)
	std::string		file_path;

	//tekstura i materia� odpowiadaj� meshowi spod danego indeksu
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

	//zarz�dzanie odwo�aniami
	bool can_delete( unsigned int& ref );
	void add_reference( );
	void delete_reference( );
};



/*Obiekty MaterialObject, TextureObject i MeshObject przechowuj� odpowiednio dane materia��w,
*dane tekstur lub bufor wierzcho�k�w.
*
*Do ka�dego pojedynczego obiektu mog� istnie� wilokrotne odwo�ania w klasie Model3DFromFile,
*a tak�e w obiektach dziedzicz�cych po Dynamic_mesh_object.
*Z tego wzgl�du istniej� zmienne file_references i object_references.
*Pierwsza okre�la, ile razy wyst�puj� odwo�ania do obiektu w klasie Model3DFromFile.
*(uwaga: nie zliczamy, ile klas si� odwo�uje. Je�eli w klasie pojawiaj� si� 3 odwo�ania, to licz� si� jako 3. Taka konwencja
*u�atwia zliczanie)
*Druga zmienna okre�la, ile wystepuje odwo�a� bezpo�rednich przez obiekty, kt�re b�d� nast�pnie wy�wietlane. Oznacza to, �e przypisuj�c
*jakiemus obiektowi plik z modelem, musimy zinkrementowa� other_references o tyle, ile by�o odwo�a� w tym pliku.
*�aden obiekt nie powinien by� kasowany, dop�ki istniej� do niego odwo�ania.
**/

class referenced_object
{//definicja w pliku Model3DFormFile
	friend ModelsManager;
private:
	unsigned int	file_references;	//liczba plik�w, kt�re sie odwo�uj�
	unsigned int	object_references;	//liczba modeli, kt�re si� odwo�uj�
	unsigned int	unique_id;			//unikalny identyfikator materia�u
public:
	referenced_object( int id );
	virtual ~referenced_object( );

	//sprawdza czy mo�na zwolni� zmienn�
	bool can_delete( unsigned int& file_ref, unsigned int& other_ref );

	/*Funkcje s�u��ce do zarz�dzania odwo�aniami.
	*Nale�y pilnowa�, aby wszystkie funkcje, kt�re modyfikuj� jakiekolwiek przypisania obiekt�w
	*do tekstur, materia��w i meshy, modyfikowa�y r�wnie� ilo�� odwo�a�.
	*U�ytkownik silnika powinien mie� udost�pnion� wartstwe po�redniczac�, �eby nie musia�
	*pami�ta� o odwo�aniach.*/
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
	std::wstring			file_path;			//�ciezka do tekstury (nie do pliku z meshem)
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
	MeshObject*				mesh_object;		//klasa zawieraj�ca wierzcho�ki danego mesha
	DirectX::XMFLOAT4X4		transform_matrix;	//macierz przekszta�cenia wst�pnego
};


class MeshObject : public referenced_object
{
	friend MeshObject;
	friend ModelsManager;
private:
public:
	LPDIRECT3DVERTEXBUFFER9	vertex_buffer;		//bufor wierzcho�k�w
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
