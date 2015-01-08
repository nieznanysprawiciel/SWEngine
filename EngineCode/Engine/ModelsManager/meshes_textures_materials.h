#pragma once


#include "..\..\stdafx.h"


//definicje
#define WRONG_ID						0
#define WRONG_MODEL_FILE_ID				WRONG_ID
#define WRONG_MESH_ID					WRONG_ID
#define WRONG_TEXTURE_ID				WRONG_ID
#define WRONG_MATERIAL_ID				WRONG_ID


class ModelsManager;
class TextureObject;
class MaterialObject;
class MeshObject;
class MeshPart;

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
	Model3DFromFile(int id);
	~Model3DFromFile();

	unsigned int add_material(const D3DMATERIAL9 &material);
	unsigned int add_texture(const std::wstring& path);
	unsigned int add_mesh(Vertex_Normal_TexCords1* vertices, unsigned int vert_num, const DirectX::XMFLOAT4X4& matrix);
	MaterialObject* add_material(unsigned int id);
	TextureObject* add_texture(unsigned int id);
	MeshPart* add_mesh(unsigned int id, DirectX::XMFLOAT4X4& matrix);
	void add_null_material();
	void add_null_texture();

	unsigned int get_parts_count();
	MaterialObject* get_material(unsigned int index);
	TextureObject* get_texture(unsigned int index);
	MeshPart* get_mesh_part(unsigned int index);

	//zarz�dzanie odwo�aniami
	bool can_delete(unsigned int& ref);
	void add_reference();
	void delete_reference();
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
	referenced_object(int id);
	virtual ~referenced_object();
	
	//sprawdza czy mo�na zwolni� zmienn�
	bool can_delete(unsigned int& file_ref, unsigned int& other_ref);

/*Funkcje s�u��ce do zarz�dzania odwo�aniami.
 *Nale�y pilnowa�, aby wszystkie funkcje, kt�re modyfikuj� jakiekolwiek przypisania obiekt�w
 *do tekstur, materia��w i meshy, modyfikowa�y r�wnie� ilo�� odwo�a�.
 *U�ytkownik silnika powinien mie� udost�pnion� wartstwe po�redniczac�, �eby nie musia�
 *pami�ta� o odwo�aniach.*/
	inline void add_file_reference() {++file_references;}
	inline void add_object_reference() {++object_references;}
	inline void delete_file_reference() {--file_references;}
	inline void delete_object_reference() {--object_references;}
};



class MaterialObject : public referenced_object
{
	friend MaterialObject;
	friend ModelsManager;
private:
	D3DMATERIAL9	material;

public:
	MaterialObject(unsigned int id);
	MaterialObject(unsigned int id, const MaterialObject& object);
	MaterialObject(unsigned int id, const D3DMATERIAL9& material2);
	~MaterialObject();

	bool operator==(const MaterialObject& object);
	bool operator==(const D3DMATERIAL9& object);

	inline D3DMATERIAL9* get_material() { return &material; }
};

class TextureObject : public referenced_object
{
	friend TextureObject;
	friend ModelsManager;
private:
	std::wstring			file_path;			//�ciezka do tekstury (nie do pliku z meshem)
public:
	LPDIRECT3DTEXTURE9		texture;

	TextureObject(unsigned int id);
	TextureObject(unsigned int id, const std::wstring& path, LPDIRECT3DTEXTURE9 tex);
	~TextureObject();

	bool operator==(const TextureObject& object);
	bool operator==(const std::wstring& file_name);

	inline LPDIRECT3DTEXTURE9 get_texture() { return texture; }
};

class MeshObject : public referenced_object
{
	friend MeshObject;
	friend ModelsManager;
private:
public:
	LPDIRECT3DVERTEXBUFFER9	vertex_buffer;		//bufor wierzcho�k�w
	unsigned int			primitives_count;

	MeshObject(int id);
	~MeshObject();

	inline LPDIRECT3DVERTEXBUFFER9 get_vertex_buffer() { return vertex_buffer; }

	//bool operator==(const Vertex_Normal_TexCords1& vertex);
};

class MeshPart
{
	friend MeshPart;
public:
	MeshObject*				mesh_object;		//klasa zawieraj�ca wierzcho�ki danego mesha
	DirectX::XMFLOAT4X4		transform_matrix;	//macierz przekszta�cenia wst�pnego
};


