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

	//zarz¹dzanie odwo³aniami
	bool can_delete(unsigned int& ref);
	void add_reference();
	void delete_reference();
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
	referenced_object(int id);
	virtual ~referenced_object();
	
	//sprawdza czy mo¿na zwolniæ zmienn¹
	bool can_delete(unsigned int& file_ref, unsigned int& other_ref);

/*Funkcje s³u¿¹ce do zarz¹dzania odwo³aniami.
 *Nale¿y pilnowaæ, aby wszystkie funkcje, które modyfikuj¹ jakiekolwiek przypisania obiektów
 *do tekstur, materia³ów i meshy, modyfikowa³y równie¿ iloœæ odwo³añ.
 *U¿ytkownik silnika powinien mieæ udostêpnion¹ wartstwe poœredniczac¹, ¿eby nie musia³
 *pamiêtaæ o odwo³aniach.*/
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
	std::wstring			file_path;			//œciezka do tekstury (nie do pliku z meshem)
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
	LPDIRECT3DVERTEXBUFFER9	vertex_buffer;		//bufor wierzcho³ków
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
	MeshObject*				mesh_object;		//klasa zawieraj¹ca wierzcho³ki danego mesha
	DirectX::XMFLOAT4X4		transform_matrix;	//macierz przekszta³cenia wstêpnego
};


