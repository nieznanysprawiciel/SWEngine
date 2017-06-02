#pragma once
/**@file Model3DFromFile.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief Plik zawiera deklaracj� klasy Model3DFromFile i klas pomocniczych do tworzenia obiektu.*/

#include "swGraphicAPI/Resources/MeshResources.h"
#include "DefaultAssets.h"

#include <vector>


/// @brief Definiuje offset bufora indeks�w wzgl�dem bufora wierzcho�k�w. (Dla funkcji Model3DFromFile::add_index_buffer)
enum VERTEX_BUFFER_OFFSET
{
	LAST = -1,
	BEGIN = 0,
};


/**@brief Struktura tymczasowe dla klasy Model3DFromFile, u�ywane podczas wype�niania
obiektu danymi. Struktura jest kasowana po zakonczeniu edycji.*/
struct TMP_data
{
	VertexNormalTexCoord*		vertices_tab	= nullptr;
	unsigned int				vertices_count	= 0;
	VERT_INDEX*					indicies_tab = nullptr;
	unsigned int				indicies_count	= 0;
	unsigned int				indicies_offset	= 0;	//offset w buforze wierzcho�k�w, wzgl�dem kt�rego lic�a si� indexy 
	ModelPart					new_part;
};

/**@brief Struktura tymczasowe dla klasy Model3DFromFile, u�ywane podczas wype�niania
obiektu danymi. Struktura jest kasowana po zakonczeniu edycji.*/
struct EditTMP
{
	int			current_pointer		= 0;
	TMP_data**	table			= nullptr;
	int			table_length	= 20;
};

/**
@ingroup ResourcesManagment
@brief Klasa s�u�y do przechowywania danych modelu wczytanego z pliku.
Dzi�ki temu po ponownym u�yciu pliku dla innego obiektu, nie musimy wczytywa� modelu ponownie.
Plik(i model jednocze�nie) mo�e by� identyfikowany po �cie�ce do pliku lub unikalnym identyfikatorem.

Klasa zawiera tablic� element�w ModelPart, kt�re przechowuj� informacje potrzebne do wyrenderowania
poszczeg�lnych cz�ci mesha. Meshe musza by� trzymane w cz�ciach, �eby mo�na by�o ka�dej z tych cz�ci
przypisa� r�ne materia�y i parametry. Gdyby nie taka struktura, to wczytanie pliku nie by�oby mo�liwe.
Meshe rzadko sk�adaj� si� z jednego kawa�ka.

Wszystkie wierzcho�ki przechowywane s� tylko w jednym buforze vertex_buffer. Na podstawie zalece�
w dokumentacji DrectXa, lepiej jest mie� jeden du�y bufor ni� wiele ma�ych.
Poszczeg�lne cz�ci mesha zawieraj� albo bufor indeks�w, kt�ry decyduje, kt�re wierzcho�ki
s� wy�wietlane, albo odpowiedni offset od pocz�tku bufora i ilo�� tr�jk�t�w.
Mo�liwe jest r�wnie�, �e mesh b�dzie wy�wietlany w trybie mieszanym, u�ywaj�c
dla ka�dej cz�ci raz bufora indeks�w, a raz offset�w.

## Wype�nianie obiekt�w danymi
Obiekty s� wype�niane najcz�ciej przez loadery dla odpowiednich format�w. Wi�cej o pisaniu loader�w znajduje si�
tutaj: $ref MakingLoaders.

Aby wype�ni� obiekt danymi nale�y wywo�a� funkcj� BeginEdit, a po zako�czeniu EndEdit. BeginEdit tworzy struktur�
tymczasow�, do kt�rej b�d� zapisywane informacje. EndEdit przetwarza t� strutkur� do postaci docelowej. Struktura po�rednia
jest konieczna, poniewa� bufory wierzcho�k�w i indeks�w b�dziemy dostawali w cz�ciach, a musimy je scali� do pojedynczych
bufor�w i stworzy� obiekt DirectXa.

Nast�pnie mi�dzy wywo�aniami BeginEdit i EndEdit podaje si� wszystkie informacje, jakie maj� zosta� zawarte w poszczeg�lnych
obiektach ModelPart. Wype�nianie ka�dego parta musi by� otoczone wywo�aniem funkcji BeginPart i EndPart. BeginPart alokuje
pami�� dla kolejnego ModelParta, a EndEdit przesuwa aktualny indeks, pod kt�rego dodajemy cz�� mesha.

Niedodanie bufora indeks�w powoduje, �e aktualny Part jest wy�wietlany w trybie bezpo�rednim, w przeciwnym razie
jest u�ywany tryb indeksowany.
W strukturze musi istnie� przynajmniej jeden bufor wierzcho�k�w. Brak bufora wierzcho�k�w jest b��dem, ale zasadniczo
nie skutkuje niczym powa�nym, bo DisplayEngine pomija takie obiekty.

Nie ma konieczno�ci podawania bufora wierzcho�k�w dla ka�dego ModelParta. Bufory indeks�w mog� by� ustawiane wzgl�dem
poprzednio dodanych bufor�w wierzcho�k�w. B��dem jest za to niepodanie ani bufora wierzcho�k�w ani bufora indeks�w.
DisplayEngine b�dzie pr�bowa� wy�wietli� takiego mesha o liczbie wierzcho�k�w r�wnej 0, co niepotrzebnie spowalnia
program.

Je�eli dodane tekstury s� zgodne z domy�lnym przeznaczeniem (zdefiniowanym enumeracj� TextureUse), to nie ma potrzeby
dodawania w�asnych shader�w. Funkcja AssetsManager::FindBestVertexShader i AssetsManager::FindBestPixelShader znajd�
najlepiej pasuj�cy shader do podanej tablicy tekstur.

Je�eli nie zostanie podany �aden materia�, ModelPart dostanie domy�lny materia�, kt�ry jest ustawiany przez funkcj�
MaterialObject::SetNullMaterial.

@see BufferObject, ModelPart, TextureObject, VertexShader, PixelShader, MaterialObject, MeshPartObject

@todo Przepisa� tak, �eby by�o jako� bardziej estetycznie.
- Doda� klas� MemoryChunk zamiast go�ych wska�nik�w na tablic� wierzcho�k�w.
- Zastosowa� semantyk� move z C++11. Ograniczy� kopiowanie pami�ci na ile si� da.
- Uzupe�ni� o mo�liwo�� wczytywania layot�w i innych rzeczy, kt�rych brakuje.
- Wydzieli� funkcjonalno� budowania modelu w inne miejsce (tmp_data).*/

class Model3DFromFile : public ResourceObject
{
	RTTR_ENABLE( ResourceObject )
	RTTR_REGISTRATION_FRIEND

	friend ObjectDeleter<Model3DFromFile>;
	friend AssetsManager;
private:
	static AssetsManager* models_manager;

	//tekstura i materia� odpowiadaj� meshowi spod danego indeksu
	std::vector<ModelPart>			model_parts;		///<Vector zawieraj�cy info o poszczeg�lnych cz�ciach mesha
	BufferObject*					vertex_buffer;		///<Bufor wierzcho�k�w
	BufferObject*					index_buffer;		///<Bufor indeks�w
	std::wstring					m_filePath;			///<Plik, z kt�rego powsta� obiekt

	//wskaxnik na tymczasow� strukture, kt�ra b�dzie u�ywana podczas wype�niania obiektu danymi
	EditTMP*						tmp_data;			///<Dane tymczasowe u�ywane podczas wype�niania

protected:
	//�eby unikn�� pomy�ki, obiekt mo�e by� kasowany tylko przez AssetsManager.
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

	// Wype�nianie danymi
	unsigned int		add_texture			( const std::wstring& file_name, TextureUse type = TEX_DIFFUSE );
	unsigned int		add_material		( const MaterialObject* material, const std::wstring& material_name );
	unsigned int		add_vertex_shader	( const std::wstring& file_name );
	unsigned int		add_pixel_shader	( const std::wstring& file_name );
	unsigned int		add_vertex_buffer	( const VertexNormalTexCoord* buffer, unsigned int vert_count );
	void				add_transformation	( const DirectX::XMFLOAT4X4& transform );
	unsigned int		add_index_buffer	( const VERT_INDEX* buffer, unsigned int ind_count, int vertex_buffer_offset );


	Size				get_parts_count( );
	const ModelPart*	get_part( unsigned int index );

	inline BufferObject* get_vertex_buffer() { return vertex_buffer; }		///<Zwraca wska�nik na bufor wierzcho�k�w
	inline BufferObject* get_index_buffer() { return index_buffer; }		///<Zwraca wska�nik na bufor indeks�w

	virtual void		Serialize		( ISerializer* ser ) const override;
	virtual void		Deserialize		( IDeserializer* deser ) override;

	virtual std::string	GetResourceName	() const override;
};


