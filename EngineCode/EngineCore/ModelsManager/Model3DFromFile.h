#pragma once
/**@file Model3DFromFile.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Plik zawiera deklaracjê klasy Model3DFromFile i klas pomocniczych do tworzenia obiektu.*/

#include "swGraphicAPI/Resources/MeshResources.h"
#include "DefaultAssets.h"

#include <vector>


/// @brief Definiuje offset bufora indeksów wzglêdem bufora wierzcho³ków. (Dla funkcji Model3DFromFile::add_index_buffer)
enum VERTEX_BUFFER_OFFSET
{
	LAST = -1,
	BEGIN = 0,
};


/**@brief Struktura tymczasowe dla klasy Model3DFromFile, u¿ywane podczas wype³niania
obiektu danymi. Struktura jest kasowana po zakonczeniu edycji.*/
struct TMP_data
{
	VertexNormalTexCoord*		vertices_tab	= nullptr;
	unsigned int				vertices_count	= 0;
	VERT_INDEX*					indicies_tab = nullptr;
	unsigned int				indicies_count	= 0;
	unsigned int				indicies_offset	= 0;	//offset w buforze wierzcho³ków, wzglêdem którego lic¿a siê indexy 
	ModelPart					new_part;
};

/**@brief Struktura tymczasowe dla klasy Model3DFromFile, u¿ywane podczas wype³niania
obiektu danymi. Struktura jest kasowana po zakonczeniu edycji.*/
struct EditTMP
{
	int			current_pointer		= 0;
	TMP_data**	table			= nullptr;
	int			table_length	= 20;
};

/**
@ingroup ResourcesManagment
@brief Klasa s³u¿y do przechowywania danych modelu wczytanego z pliku.
Dziêki temu po ponownym u¿yciu pliku dla innego obiektu, nie musimy wczytywaæ modelu ponownie.
Plik(i model jednoczeœnie) mo¿e byæ identyfikowany po œcie¿ce do pliku lub unikalnym identyfikatorem.

Klasa zawiera tablicê elementów ModelPart, które przechowuj¹ informacje potrzebne do wyrenderowania
poszczególnych czêœci mesha. Meshe musza byæ trzymane w czêœciach, ¿eby mo¿na by³o ka¿dej z tych czêœci
przypisaæ ró¿ne materia³y i parametry. Gdyby nie taka struktura, to wczytanie pliku nie by³oby mo¿liwe.
Meshe rzadko sk³adaj¹ siê z jednego kawa³ka.

Wszystkie wierzcho³ki przechowywane s¹ tylko w jednym buforze vertex_buffer. Na podstawie zaleceñ
w dokumentacji DrectXa, lepiej jest mieæ jeden du¿y bufor ni¿ wiele ma³ych.
Poszczególne czêœci mesha zawieraj¹ albo bufor indeksów, który decyduje, które wierzcho³ki
s¹ wyœwietlane, albo odpowiedni offset od pocz¹tku bufora i iloœæ trójk¹tów.
Mo¿liwe jest równie¿, ¿e mesh bêdzie wyœwietlany w trybie mieszanym, u¿ywaj¹c
dla ka¿dej czêœci raz bufora indeksów, a raz offsetów.

## Wype³nianie obiektów danymi
Obiekty s¹ wype³niane najczêœciej przez loadery dla odpowiednich formatów. Wiêcej o pisaniu loaderów znajduje siê
tutaj: $ref MakingLoaders.

Aby wype³niæ obiekt danymi nale¿y wywo³aæ funkcjê BeginEdit, a po zakoñczeniu EndEdit. BeginEdit tworzy strukturê
tymczasow¹, do której bêd¹ zapisywane informacje. EndEdit przetwarza tê strutkurê do postaci docelowej. Struktura poœrednia
jest konieczna, poniewa¿ bufory wierzcho³ków i indeksów bêdziemy dostawali w czêœciach, a musimy je scaliæ do pojedynczych
buforów i stworzyæ obiekt DirectXa.

Nastêpnie miêdzy wywo³aniami BeginEdit i EndEdit podaje siê wszystkie informacje, jakie maj¹ zostaæ zawarte w poszczególnych
obiektach ModelPart. Wype³nianie ka¿dego parta musi byæ otoczone wywo³aniem funkcji BeginPart i EndPart. BeginPart alokuje
pamiêæ dla kolejnego ModelParta, a EndEdit przesuwa aktualny indeks, pod którego dodajemy czêœæ mesha.

Niedodanie bufora indeksów powoduje, ¿e aktualny Part jest wyœwietlany w trybie bezpoœrednim, w przeciwnym razie
jest u¿ywany tryb indeksowany.
W strukturze musi istnieæ przynajmniej jeden bufor wierzcho³ków. Brak bufora wierzcho³ków jest b³êdem, ale zasadniczo
nie skutkuje niczym powa¿nym, bo DisplayEngine pomija takie obiekty.

Nie ma koniecznoœci podawania bufora wierzcho³ków dla ka¿dego ModelParta. Bufory indeksów mog¹ byæ ustawiane wzglêdem
poprzednio dodanych buforów wierzcho³ków. B³êdem jest za to niepodanie ani bufora wierzcho³ków ani bufora indeksów.
DisplayEngine bêdzie próbowa³ wyœwietliæ takiego mesha o liczbie wierzcho³ków równej 0, co niepotrzebnie spowalnia
program.

Je¿eli dodane tekstury s¹ zgodne z domyœlnym przeznaczeniem (zdefiniowanym enumeracj¹ TextureUse), to nie ma potrzeby
dodawania w³asnych shaderów. Funkcja AssetsManager::FindBestVertexShader i AssetsManager::FindBestPixelShader znajd¹
najlepiej pasuj¹cy shader do podanej tablicy tekstur.

Je¿eli nie zostanie podany ¿aden materia³, ModelPart dostanie domyœlny materia³, który jest ustawiany przez funkcjê
MaterialObject::SetNullMaterial.

@see BufferObject, ModelPart, TextureObject, VertexShader, PixelShader, MaterialObject, MeshPartObject

@todo Przepisaæ tak, ¿eby by³o jakoœ bardziej estetycznie.
- Dodaæ klasê MemoryChunk zamiast go³ych wskaŸników na tablicê wierzcho³ków.
- Zastosowaæ semantykê move z C++11. Ograniczyæ kopiowanie pamiêci na ile siê da.
- Uzupe³niæ o mo¿liwoœæ wczytywania layotów i innych rzeczy, których brakuje.
- Wydzieliæ funkcjonalnoœ budowania modelu w inne miejsce (tmp_data).*/

class Model3DFromFile : public ResourceObject
{
	RTTR_ENABLE( ResourceObject )
	RTTR_REGISTRATION_FRIEND

	friend ObjectDeleter<Model3DFromFile>;
	friend AssetsManager;
private:
	static AssetsManager* models_manager;

	//tekstura i materia³ odpowiadaj¹ meshowi spod danego indeksu
	std::vector<ModelPart>			model_parts;		///<Vector zawieraj¹cy info o poszczególnych czêœciach mesha
	BufferObject*					vertex_buffer;		///<Bufor wierzcho³ków
	BufferObject*					index_buffer;		///<Bufor indeksów
	std::wstring					m_filePath;			///<Plik, z którego powsta³ obiekt

	//wskaxnik na tymczasow¹ strukture, która bêdzie u¿ywana podczas wype³niania obiektu danymi
	EditTMP*						tmp_data;			///<Dane tymczasowe u¿ywane podczas wype³niania

protected:
	//¯eby unikn¹æ pomy³ki, obiekt mo¿e byœ kasowany tylko przez AssetsManager.
	~Model3DFromFile() override;

	unsigned int get_buffer_offset_to_last();

	void EndEdit_vertex_buffer_processing();
	void EndEdit_index_buffer_processing();
	void EndEdit_prepare_ModelPart();
public:
	Model3DFromFile( const std::wstring& name );


	// Tworzenie obiektu
	void BeginEdit();
	void EndEdit();
	void BeginPart();
	void EndPart();

	// Wype³nianie danymi
	unsigned int		add_texture			( const std::wstring& file_name, TextureUse type = TEX_DIFFUSE );
	unsigned int		add_material		( const MaterialObject* material, const std::wstring& material_name );
	unsigned int		add_vertex_shader	( const std::wstring& file_name );
	unsigned int		add_pixel_shader	( const std::wstring& file_name );
	unsigned int		add_vertex_buffer	( const VertexNormalTexCoord* buffer, unsigned int vert_count );
	void				add_transformation	( const DirectX::XMFLOAT4X4& transform );
	unsigned int		add_index_buffer	( const VERT_INDEX* buffer, unsigned int ind_count, int vertex_buffer_offset );


	Size				get_parts_count( );
	const ModelPart*	get_part( unsigned int index );

	inline BufferObject* get_vertex_buffer() { return vertex_buffer; }		///<Zwraca wskaŸnik na bufor wierzcho³ków
	inline BufferObject* get_index_buffer() { return index_buffer; }		///<Zwraca wskaŸnik na bufor indeksów

	virtual void		Serialize		( ISerializer* ser ) const override;
	virtual void		Deserialize		( IDeserializer* deser ) override;

	virtual std::string	GetResourceName	() const override;
};


