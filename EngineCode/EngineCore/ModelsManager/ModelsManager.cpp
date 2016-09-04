#include "EngineCore/stdafx.h"
#include "ModelsManager.h"

#include "Loaders/ILoader.h"
#include "Loaders/FBX_files_loader/FBX_loader.h"
#include "Loaders/Texture/TextureLoader.h"

#include "Common/ObjectDeleter.h"
#include "Common/MacrosSwitches.h"
#include "GraphicAPI/ResourcesFactory.h"

#include "EngineCore/EngineHelpers/Converters.h"

#include "Common/MemoryLeaks.h"

using namespace DirectX;

//-------------------------------------------------------------------------------//
//							wersja DirectX11
//-------------------------------------------------------------------------------//

ModelsManager::ModelsManager( Engine* engine )
: m_engine( engine )
{
	Model3DFromFile::models_manager = this;

	//Loader plików FBX jest domyœlnym narzêdziem do wczytywania
	FBX_loader* fbx_loader = new FBX_loader( this );
	m_loader.push_back( fbx_loader );
}



ModelsManager::~ModelsManager( )
{
	// Kasujemy obiekty do wczytywania danych
	for ( unsigned int i = 0; i <  m_loader.size(); ++i )
		delete m_loader[i];
}




/**@brief Dopasowuje najlepszy z domyœlnych shaderów, który pasuje do podanej tablicy
tekstur. Tablica ma tyle elementów ile zmienna @ref ENGINE_MAX_TEXTURES.

Ka¿da pozycja w tablicy ma przypisane domyœlne znaczenie zgodnie z enumeracj¹ 
@ref TextureUse. Najlepszy shader jest wybierany na podstawie obecnoœci
lub nieobecnoœci tekstury w tablicy.

@todo Oddelegowaæ jakiœ inny obiekt do obœ³ugi wartoœci domyœlnych albo przemyœleæ lepiej jak to powinno w³aœciwie wygl¹daæ.

@param[in] textures Tablica tekstur z obiektu ModelPart.
@return Zwraca obiekt vertex shadera.*/
VertexShader* ModelsManager::FindBestVertexShader( TextureObject** textures )
{
	// Na razie nie mamy innych domyœlnych shaderów
	return m_vertexShader.get( DEFAULT_VERTEX_SHADER_STRING );
}


/**@brief Dopasowuje najlepszy z domyœlnych shaderów, który pasuje do podanej tablicy
tekstur. Tablica ma tyle elementów ile zmienna @ref ENGINE_MAX_TEXTURES.

Ka¿da pozycja w tablicy ma przypisane domyœlne znaczenie zgodnie z enumeracj¹
@ref TextureUse. Najlepszy shader jest wybierany na podstawie obecnoœci
lub nieobecnoœci tekstury w tablicy.

@todo Oddelegowaæ jakiœ inny obiekt do obœ³ugi wartoœci domyœlnych albo przemyœleæ lepiej jak to powinno w³aœciwie wygl¹daæ.

@param[in] textures Tablica tekstur z obiektu ModelPart.
@return Zwraca obiekt pixel shadera.*/
PixelShader* ModelsManager::FindBestPixelShader( TextureObject** textures )
{
	PixelShader* return_shader = nullptr;

	// Na razie nie ma innych tekstur ni¿ diffuse, wiêc algorytm nie jest skomplikowany
	if ( textures[TextureUse::TEX_DIFFUSE] )
		return_shader = m_pixelShader.get( DEFAULT_TEX_DIFFUSE_PIXEL_SHADER_PATH );
	
	
	if ( !return_shader )	// Je¿eli nadal jest nullptrem to dajemy mu domyœlny shader
		return_shader = m_pixelShader.get( DEFAULT_PIXEL_SHADER_STRING );

	return return_shader;
}


/** @brief Znajduje Loader pasuj¹cy do pliku podanego w parametrze.
@param[in] path Œcie¿ka do pliku, dla której szukamy loadera.
@return WskaŸnik na odpowiedni loader lub nullptr, je¿eli nie znaleziono pasuj¹cego.*/
ILoader* ModelsManager::FindLoader( const std::wstring& path )
{
	for ( unsigned int i = 0; i <  m_loader.size( ); ++i )
	if ( m_loader[i]->can_load( path ) )
		return m_loader[i];
	return nullptr;
}


//-------------------------------------------------------------------------------//
//							funkcje do zarzadzania assetami
//-------------------------------------------------------------------------------//



/**@brief Wczytuje model z podanego pliku.
@param[in] file Plik do wczytania
@return Jedna z wartoœci @ref ModelsManagerResult. Funkcja mo¿e zwróciæ @ref ModelsManagerResult::MODELS_MANAGER_OK,
@ref ModelsManagerResult::MODELS_MANAGER_LOADER_NOT_FOUND lub @ref ModelsManagerResult::MODELS_MANAGER_CANNOT_LOAD.*/
ModelsManagerResult ModelsManager::LoadModelFromFile( const std::wstring& file )
{
	// Sprawdzamy czy plik nie zosta³ ju¿ wczytany
	Model3DFromFile* new_model = m_fileModel.get( file );
	if ( new_model != nullptr )
		return ModelsManagerResult::MODELS_MANAGER_OK;	// Udajemy, ¿e wszystko posz³o dobrze

	// Sprawdzamy, który loader potrafi otworzyæ plik
	ILoader* loader = FindLoader( file );
	if ( loader == nullptr )
		return ModelsManagerResult::MODELS_MANAGER_LOADER_NOT_FOUND;		// ¯aden nie potrafi

	// Tworzymy obiekt Model3DFromFile, do którego loader wpisze zawartoœæ pliku
	new_model = new Model3DFromFile( file );

	// Wszystkie operacje wpisywania musz¹ byæ zamkniête tymi wywo³aniami
	new_model->BeginEdit();
	LoaderResult result = loader->load_mesh( new_model, file );
	new_model->EndEdit();

	if ( result != LoaderResult::MESH_LOADING_OK )
	{	// load_mesh powinno zwróciæ 0
		// Destruktor jest prywatny, wiêc nie mo¿emy kasowaæ obiektu bezpoœrednio.
		ObjectDeleter< Model3DFromFile>::delete_object( new_model, ObjectDeleterKey< Model3DFromFile>() );
		return ModelsManagerResult::MODELS_MANAGER_CANNOT_LOAD;
	}

	// Dodajemy model do tablic
	m_fileModel.UnsafeAdd( file, new_model );

	return ModelsManagerResult::MODELS_MANAGER_OK;
}


/**@brief Tworzy nowy render target.

Funkcja dodaje stworzony obiekt do tablicy m_renderTarget. Je¿eli tekstury
bufora colorów, g³êbokoœci i stencilu nie s¹ nullptrami, to i one s¹ dodawane do tablicy m_texture.

Tekstury te maj¹ nazwy jak render target + dodany jest cz³on
- ::color
- ::depth
- ::stencil

@todo Przy dodawaniu tekstur nie jest sprawdzane czy one ju¿ istniej¹. Trzeba albo to sprawdzaæ, albo zapewniæ
np. jak¹œ polityk¹ nazewnictwa, ¿e w ten sposób nie nadpisujemy istniej¹cej tekstury.

@param[in] name Nazwa identyfikuj¹ca render target.
@param[in] renderTargetDescriptor Deskryptor opisuj¹cy parametry render targetu.
@return Zwraca stworzony obiekt lub nullptr w przypadku niepowodzenia. Je¿eli render target ju¿ istnia³, to zwracany jest istniej¹cy obiekt.
*/
RenderTargetObject* ModelsManager::CreateRenderTarget( const std::wstring& name, const RenderTargetDescriptor& renderTargetDescriptor )
{
	RenderTargetObject* newRenderTarget = m_renderTarget.get( name );
	if( !newRenderTarget )
	{
		newRenderTarget = ResourcesFactory::CreateRenderTarget( name, renderTargetDescriptor );
		if( !newRenderTarget )	return nullptr;

		m_renderTarget.UnsafeAdd( name, newRenderTarget );
		
		auto colorBuff = newRenderTarget->GetColorBuffer();
		if( colorBuff )
			m_texture.UnsafeAdd( Converters::FromString( colorBuff->GetFilePath().String(), std::wstring() ), colorBuff );

		auto depthBuffer = newRenderTarget->GetDepthBuffer();
		if( depthBuffer )
			m_texture.UnsafeAdd( Converters::FromString( depthBuffer->GetFilePath().String(), std::wstring() ), depthBuffer );

		auto stencilBuffer = newRenderTarget->GetStencilBuffer();
		if( stencilBuffer )
			m_texture.UnsafeAdd( Converters::FromString( stencilBuffer->GetFilePath().String(), std::wstring() ), stencilBuffer );
	}

	return newRenderTarget;
}

/**@brief */
ResourcePtr<MeshAsset> ModelsManager::CreateMesh( const std::wstring& name, MeshAssetInitData&& initData )
{
	VertexBufferInitData vertexInit;
	vertexInit.Data = initData.VertexBuffer.GetMemory< uint8 >();
	vertexInit.Usage = ResourceUsage::RESOURCE_USAGE_STATIC;
	vertexInit.ElementSize = initData.VertexSize;
	vertexInit.NumElements = initData.NumVerticies;
	vertexInit.VertexLayout = initData.VertexLayout;
	vertexInit.Topology = initData.Topology;

	auto vertexBuffer = CreateVertexBuffer( name, vertexInit );
	if( !vertexBuffer )
		return nullptr;

	ResourcePtr< BufferObject > indexBuffer;
	if( !initData.IndexBuffer.IsNull() )
	{
		IndexBufferInitData indexInit;
		indexInit.Data = initData.IndexBuffer.GetMemory< uint8 >();
		indexInit.ElementSize = 2;
		if( initData.ExtendedIndex )
			indexInit.ElementSize = 4;
		indexInit.Topology = initData.Topology;
		indexInit.NumElements = initData.NumIndicies;
		indexInit.Usage = ResourceUsage::RESOURCE_USAGE_STATIC;

		auto indexBuffer = CreateIndexBuffer( name, indexInit );
		if( !indexBuffer )
			return nullptr;
	}

	MeshAssetInitWithExistingData meshData;
	meshData.MeshSegments = std::move( initData.MeshSegments );
	meshData.VertexLayout = std::move( initData.VertexLayout );
	meshData.VertexBuffer = std::move( vertexBuffer );
	meshData.IndexBuffer = std::move( indexBuffer );

	return CreateMesh( name, std::move( meshData ) );
}

/**@brief */
ResourcePtr<MeshAsset> ModelsManager::CreateMesh( const std::wstring& name, MeshAssetInitWithExistingData&& initData )
{
	assert( false );
	return ResourcePtr<MeshAsset>();
}



/**@brief Dodaje materia³ do ModelsManagera, je¿eli jeszcze nie istnia³.
@note Funkcja nie dodaje odwo³ania do obiektu, bo nie zak³ada, ¿e ktoœ go od razu u¿yje.
W ka¿dym miejscu, gdzie zostanie przypisany zwrócony obiekt, nale¿y pamiêtaæ o dodaniu odwo³ania oraz
skasowaniu go, gdy obiekt przestanie byæ u¿ywany.

@note Je¿eli materia³ ju¿ istnia³ (jego nazwa), to ten podany w parametrze nie zostanie dodany.
Oznacza to, ¿e za jego zwolnienie odpowiada ten, kto go stworzy³. Trzeba zawsze sprawdziæ czy
zwrócona wartoœæ jest tym samym co podaliœmy.

@todo Nie mo¿e tak zostaæ, ¿e ktoœ dodaje materia³ i musi sprawdziæ czy nie dosta³ innego. Nie mo¿na
te¿ zmuszaæ kogoœ do zwalniania pamiêci po materiale.

@param[in] material Materia³, który ma zostaæ dodany
@param[in] materialName Nazwa materia³u. Do materia³u bêdzie mo¿na siê odwo³aæ podaj¹c ci¹g znaków
[nazwa_pliku]::[nazwa_materia³u]. Oznacza to, ¿e mog¹ istnieæ dwa takie same materia³y, poniewa¿ nie jest sprawdzana
zawartoœæ, a jedynie nazwy.
@return Zwraca wskaŸnik na dodany materia³.*/
MaterialObject* ModelsManager::AddMaterial( MaterialObject* addMaterial, const std::wstring& materialName )
{
	MaterialObject* newMaterial = m_material.get( materialName );
	if ( !newMaterial )
		m_material.UnsafeAdd( materialName, addMaterial );	// Dodaliœmy materia³

	return newMaterial;
}


/**@brief Dodaje renderTarget do ModelsManagera, je¿eli jeszcze nie istnia³.
@note Funkcja nie dodaje odwo³ania do obiektu, bo nie zak³ada, ¿e ktoœ go od razu u¿yje.
W ka¿dym miejscu, gdzie zostanie przypisany zwrócony obiekt, nale¿y pamiêtaæ o dodaniu odwo³ania oraz
skasowaniu go, gdy obiekt przestanie byæ u¿ywany.

@note Je¿eli renderTarget ju¿ istnia³ (jego nazwa), to ten podany w parametrze nie zostanie dodany.
Oznacza to, ¿e za jego zwolnienie odpowiada ten, kto go stworzy³. Trzeba zawsze sprawdziæ czy
zwrócona wartoœæ jest tym samym co podaliœmy.

@todo Nie mo¿e tak zostaæ, ¿e ktoœ dodaje renderTarget i musi sprawdziæ czy nie dosta³ innego. Nie mo¿na
te¿ zmuszaæ kogoœ do zwalniania pamiêci po renderTargecie. Wogóle dodawanie renderTargetów musi siê odbywaæ jakoœ inaczej.
Najlepiej, ¿eby by³y one tworzone przez ModelsManager, ale wtedy trzeba wymyœleæ sposób dodawania renderTargetu zwi¹zanego z buforem okna.

@param[in] renderTarget renderTarget, który ma zostaæ dodany.
@param[in] name Nazwa renderTargetu. Do materia³u bêdzie mo¿na siê odwo³aæ podaj¹c ci¹g znaków
@return Zwraca wskaŸnik na dodany renderTarget.*/
RenderTargetObject* ModelsManager::AddRenderTarget( RenderTargetObject* renderTarget, const std::wstring& name )
{
	RenderTargetObject* newRenderTarget = m_renderTarget.get( name );
	if ( !newRenderTarget )
		m_renderTarget.UnsafeAdd( name, renderTarget );	// Dodaliœmy materia³

	return newRenderTarget;
}

/**@brief Dodaje vertex shader do ModelsManagera. Je¿eli obiekt ju¿ istnia³, to nie jest tworzony nowy.
@note Funkcja nie dodaje odwo³ania do obiektu, bo nie zak³ada, ¿e ktoœ go od razu u¿yje.
W ka¿dym miejscu, gdzie zostanie przypisany zwrócony obiekt, nale¿y pamiêtaæ o dodaniu odwo³ania oraz
skasowaniu go, gdy obiekt przestanie byæ u¿ywany.

@param[in] fileName Nazwa pliku, w którym znajduje siê vertex shader.
@param[in] shaderEntry Nazwa funkcji od której ma siê zacz¹æ wykonywanie shadera.
@return Zwraca obiekt dodanego shadera. Zwraca nullptr, je¿eli shadera nie uda³o siê skompilowaæ.*/
VertexShader* ModelsManager::LoadVertexShader( const std::wstring& fileName, const std::string& shaderEntry )
{
	VertexShader* shader = m_vertexShader.get( fileName );
	if ( !shader )
	{
		// Nie by³o shadera, trzeba go stworzyæ i dodaæ
		shader = ResourcesFactory::CreateVertexShaderFromFile( fileName, shaderEntry );
		if ( !shader )		// shader móg³ mieæ z³y format, a nie chcemy dodawaæ nullptra do ModelsManagera
			return nullptr;

		m_vertexShader.UnsafeAdd( fileName, shader );	// Dodaliœmy teksturê
	}

	return shader;
}

/**@brief Dodaje vertex shader do ModelsManagera. Je¿eli obiekt ju¿ istnia³, to nie jest tworzony nowy.
Tworzy te¿ layout wierzcho³ka zwi¹zany z tym shaderem i zwraca go w zmiennej layout.

Je¿eli vertex shader wczeœniej istnia³, to stworzenie layoutu wymaga ponownego skompilowania shadera. Shader taki jest potem
kasowany i nie zostaje zdublowany w ModelsManagerze, ale niepotrzebna praca zostaje w³o¿ona. Jest wiêc zadaniem programisty, ¿eby
do takich rzeczy dochodzi³o jak najrzadziej.

@note Funkcja nie dodaje odwo³ania do obiektu, bo nie zak³ada, ¿e ktoœ go od razu u¿yje.
W ka¿dym miejscu, gdzie zostanie przypisany zwrócony obiekt, nale¿y pamiêtaæ o dodaniu odwo³ania oraz
skasowaniu go, gdy obiekt przestanie byæ u¿ywany.

@param[in] fileName Nazwa pliku, w którym znajduje siê vertex shader.
@param[in] shaderEntry Nazwa funkcji od której ma siê zacz¹æ wykonywanie shadera.
@param[out] layout W zmiennej umieszczany jest wskaŸnik na layout wierzcho³ka. Nawet je¿eli shader siê nie skompilowa³, to pole mo¿e mieæ wartoœæ inn¹ ni¿ nullptr.
Dzieje siê tak wtedy, gdy layout istnia³ ju¿ wczeœniej.
@attention Je¿eli vertex shader wczeœniej istnia³, to stworzenie layoutu wymaga ponownego skompilowania shadera. Shader taki jest potem 
kasowany i nie zostaje zdublowany w ModelsManagerze, ale niepotrzebna praca zostaje w³o¿ona. Jest wiêc zadaniem programisty, ¿eby
do takich rzeczy dochodzi³o jak najrzadziej.
@param[in] layoutDesc Deskryptor opisujacy tworzony layout.
@return Zwraca obiekt dodanego shadera. Zwraca nullptr, je¿eli shadera nie uda³o siê skompilowaæ.*/
VertexShader* ModelsManager::LoadVertexShader( const std::wstring& fileName,
													const std::string& shaderEntry,
													ShaderInputLayout** layout,
													InputLayoutDescriptor* layoutDesc )
{
	/// @todo Ten kod to jakiœ totalny shit. Jak komuœ siê bêdzie nudzi³o kiedyœ (ha ha), to mo¿e niech poprawi.
	*layout = nullptr;
	VertexShader* shader = m_vertexShader.get( fileName );
	VertexShader* newShader = nullptr;
	ShaderInputLayout* inputLayout = m_vertexLayout.get( layoutDesc->GetName() );


	// Tworzymy potrzebne obiekty
	if( !inputLayout )
	{
		// Tworzymy shader niezale¿nie czy istnieje. Inaczej nie moglibyœmy stworzyæ layoutu.
		// Shader zostanie potem usuniêty.
		newShader = ResourcesFactory::CreateVertexShaderFromFile( fileName, shaderEntry, layout, layoutDesc );
		if ( !newShader )		// shader móg³ mieæ z³y format, a nie chcemy dodawaæ nullptra do ModelsManagera
			return nullptr;		// layout te¿ jest nullptrem, nie trzeba siê martwiæ.
	}
	else if( !shader )
	{
		// Layout istnieje, ale shader nie.
		newShader = ResourcesFactory::CreateVertexShaderFromFile( fileName, shaderEntry );
		*layout = inputLayout;	// Je¿eli layout istnia³, to przepisujemy go na wyjœcie. Je¿eli nie to i tak bêdzie nullptr.
		if ( !newShader )		// shader móg³ mieæ z³y format, a nie chcemy dodawaæ nullptra do ModelsManagera
			return nullptr;
	}
	else
	{// Wszystkie obiekty istania³y ju¿ wczeœniej.
		*layout = inputLayout;
		return shader;
	}

	// Nowo powsta³e obiekty musz¹ zostaæ dodane do kontenerów.
	if ( !shader )
	{
		// Nie by³o shadera, trzeba go dodaæ
		m_vertexShader.UnsafeAdd( fileName, newShader );	// Dodaliœmy shader
		shader = newShader;
	}
	else
	{	// Shader ju¿ by³, wiêc kasujemy nowy
		// Destruktor jest prywatny, wiêc nie mo¿emy kasowaæ obiektu bezpoœrednio.
		ObjectDeleter< VertexShader>::delete_object( shader, ObjectDeleterKey< VertexShader>() );
	}

	if( !inputLayout )	// Layoutu nie by³o wczeœniej wiêc dodajemy.
		m_vertexLayout.UnsafeAdd( layoutDesc->GetName(), *layout );

	return shader;
}


/**@brief Dodaje pixel shader do ModelsManagera. Je¿eli obiekt ju¿ istnia³, to nie jest tworzony nowy.
@note Funkcja nie dodaje odwo³ania do obiektu, bo nie zak³ada, ¿e ktoœ go od razu u¿yje.
W ka¿dym miejscu, gdzie zostanie przypisany zwrócony obiekt, nale¿y pamiêtaæ o dodaniu odwo³ania oraz
skasowaniu go, gdy obiekt przestanie byæ u¿ywany.

@param[in] fileName Nazwa pliku, w którym znajduje siê pixel shader.
@param[in] shaderEntry Nazwa funkcji od której ma siê zacz¹æ wykonywanie shadera.
@return Zwraca obiekt dodanego shadera. Zwraca nullptr, je¿eli shadera nie uda³o siê skompilowaæ.*/
PixelShader* ModelsManager::LoadPixelShader( const std::wstring& fileName, const std::string& shaderEntry )
{
	PixelShader* shader = m_pixelShader.get( fileName );
	if ( !shader )
	{
		// Nie by³o shadera, trzeba go stworzyæ i dodaæ
		shader = ResourcesFactory::CreatePixelShaderFromFile( fileName, shaderEntry );
		if ( !shader )		// shader móg³ mieæ z³y format, a nie chcemy dodawaæ nullptra do ModelsManagera
			return nullptr;

		m_pixelShader.UnsafeAdd( fileName, shader );	// Dodaliœmy teksturê
	}

	return shader;
}

/**@brief Dodaje teksturê do ModelManagera, je¿eli jeszcze nie istnia³a.
@note Funkcja nie dodaje odwo³ania do obiektu, bo nie zak³ada, ¿e ktoœ go od razu u¿yje.
W ka¿dym miejscu, gdzie zostanie przypisany zwrócony obiekt, nale¿y pamiêtaæ o dodaniu odwo³ania oraz
skasowaniu go, gdy obiekt przestanie byæ u¿ywany.

@param[in] fileName Œcie¿ka do tekstury

@return Zwraca wskaŸnik na dodan¹ teksturê lub nullptr, je¿eli nie da³o siê wczytaæ.*/
TextureObject* ModelsManager::LoadTexture( const std::wstring& fileName )
{
	TextureObject* tex = m_texture.get( fileName );
	if ( !tex )
	{
		// Nie by³o tekstury, trzeba j¹ stworzyæ i dodaæ
		TextureInfo texInfo;
		texInfo.FilePath = filesystem::Path( fileName );
		texInfo.GenerateMipMaps = true;
		texInfo.MipMapFilter = MipMapFilter::Lanczos4;

		MemoryChunk texData = TextureLoader::LoadTexture( texInfo.FilePath, texInfo );

		tex = ResourcesFactory::CreateTextureFromMemory( texData, std::move( texInfo ) );
		if ( !tex )		// Tekstura mog³a mieæ z³y format, a nie chcemy dodawaæ nullptra do ModelsManagera
			return nullptr;

		m_texture.UnsafeAdd( fileName, tex );	// Dodaliœmy teksturê
	}

	return tex;
}

/**@brief Dodaje do ModelsManagera bufor wierzcho³ków.
Je¿eli pod tak¹ nazw¹ istnieje jakiœ bufor, to zostanie zwrócony wskaŸnik na niego.
@note Funkcja nie dodaje odwo³ania do obiektu, bo nie zak³ada, ¿e ktoœ go od razu u¿yje.
W ka¿dym miejscu, gdzie zostanie przypisany zwrócony obiekt, nale¿y pamiêtaæ o dodaniu odwo³ania oraz
skasowaniu go, gdy obiekt przestanie byæ u¿ywany.

@param[in] name Nazwa bufora, po której mo¿na siê bêdzie odwo³aæ.
@param[in] buffer WskaŸnik na bufor z danym, które maj¹ byæ przeniesione do bufora DirectXowego.
@param[in] elementSize Rozmiar pojedynczego elementu w buforze.
@param[in] vertCount Liczba wierzcho³ków/indeksów w buforze.
@return Dodany bufor wierzcho³ków. Zwraca nullptr, je¿eli nie uda³o siê stworzyæ bufora.*/
ResourcePtr< BufferObject > ModelsManager::CreateVertexBuffer( const std::wstring& name, const void* buffer, unsigned int elementSize, unsigned int vertCount )
{
	VertexBufferInitData initData;
	initData.Data = (const uint8*)buffer;
	initData.ElementSize = elementSize;
	initData.NumElements = vertCount;
	
	return CreateVertexBuffer( name, initData );
}

/**@brief Creates vetex buffer.

@return Returns buffer or nullptr if name already exists or buffer creation failed.*/
ResourcePtr<BufferObject>	ModelsManager::CreateVertexBuffer		( const std::wstring& name, const VertexBufferInitData& data )
{
	BufferObject* vertexBuff = m_vertexBuffer.get( name );
	if ( vertexBuff )	// Je¿eli znaleŸliœmy bufor, to zwracamy nullptr
		return ResourcePtr<BufferObject>();
	
	
	vertexBuff = ResourcesFactory::CreateBufferFromMemory( name, data.Data, data.CreateBufferInfo() );
	if ( !vertexBuff )		// Bufor móg³ siê nie stworzyæ, a nie chcemy dodawaæ nullptra do ModelsManagera
		return nullptr;

	m_vertexBuffer.UnsafeAdd( name, vertexBuff );	// Dodaliœmy bufor
	return ResourcePtr<BufferObject>( vertexBuff );
}

/**@brief Dodaje do ModelsManagera bufor indeksów.
Je¿eli pod tak¹ nazw¹ istnieje jakiœ bufor, to zostanie zwrócony wskaŸnik na niego.
@note Funkcja nie dodaje odwo³ania do obiektu, bo nie zak³ada, ¿e ktoœ go od razu u¿yje.
W ka¿dym miejscu, gdzie zostanie przypisany zwrócony obiekt, nale¿y pamiêtaæ o dodaniu odwo³ania oraz
skasowaniu go, gdy obiekt przestanie byæ u¿ywany.

@param[in] name Nazwa bufora, po której mo¿na siê bêdzie odwo³aæ.
@param[in] buffer WskaŸnik na bufor z danym, które maj¹ byæ przeniesione do bufora DirectXowego.
@param[in] elementSize Rozmiar pojedynczego elementu w buforze.
@param[in] vertCount Liczba wierzcho³ków/indeksów w buforze.
@return Dodany bufor indeksów. Zwraca nullptr, je¿eli nie uda³o siê stworzyæ bufora.*/
ResourcePtr< BufferObject > ModelsManager::CreateIndexBuffer( const std::wstring& name, const void* buffer, unsigned int elementSize, unsigned int vertCount )
{
	IndexBufferInitData initData;
	initData.Data = (const uint8*)buffer;
	initData.ElementSize = elementSize;
	initData.NumElements = vertCount;

	return CreateIndexBuffer( name, initData );
}

/**@brief Vreates index buffer.

@return Returns buffer or nullptr if name already exists or buffer creation failed.*/
ResourcePtr<BufferObject>	ModelsManager::CreateIndexBuffer		( const std::wstring& name, const IndexBufferInitData& data )
{
	BufferObject* indexBuff = m_indexBuffer.get( name );
	if ( indexBuff )	// Je¿eli znaleŸliœmy bufor, to zwracamy nullptr
		return ResourcePtr<BufferObject>();
	
	
	indexBuff = ResourcesFactory::CreateBufferFromMemory( name, data.Data, data.CreateBufferInfo() );
	if ( !indexBuff )		// Bufor móg³ siê nie stworzyæ, a nie chcemy dodawaæ nullptra do ModelsManagera
		return nullptr;

	m_indexBuffer.UnsafeAdd( name, indexBuff );	// Dodaliœmy bufor
	return ResourcePtr<BufferObject>( indexBuff );
}

/**@brief Dodaje do ModelsManagera bufor sta³ch dla shadera.
Je¿eli pod tak¹ nazw¹ istnieje jakiœ bufor, to zostanie zwrócony wskaŸnik na niego.
@note Funkcja nie dodaje odwo³ania do obiektu, bo nie zak³ada, ¿e ktoœ go od razu u¿yje.
W ka¿dym miejscu, gdzie zostanie przypisany zwrócony obiekt, nale¿y pamiêtaæ o dodaniu odwo³ania oraz
skasowaniu go, gdy obiekt przestanie byæ u¿ywany.

@param[in] name Nazwa bufora, po której mo¿na siê bêdzie odwo³aæ.
@param[in] buffer WskaŸnik na bufor z danym, które maj¹ byæ przeniesione do bufora DirectXowego.
@param[in] size Rozmiar bufora.
@return Dodany bufor indeksów. Zwraca nullptr, je¿eli nie uda³o siê stworzyæ bufora.*/
ResourcePtr< BufferObject >	ModelsManager::CreateConstantsBuffer( const std::wstring& name, const void* buffer, unsigned int size )
{
	ConstantBufferInitData initData;
	initData.Data = (const uint8*)buffer;
	initData.ElementSize = size;
	initData.NumElements = 1;

	return CreateConstantsBuffer( name, initData );
}

/**@brief Creates constant buffer.

@return Returns buffer or nullptr if name already exists or buffer creation failed.*/
ResourcePtr<BufferObject>	ModelsManager::CreateConstantsBuffer		( const std::wstring& name, const ConstantBufferInitData& data )
{
	BufferObject* constBuff = m_constantBuffer.get( name );
	if ( constBuff )	// Je¿eli znaleŸliœmy bufor, to zwracamy nullptr
		return ResourcePtr<BufferObject>();
	
	
	constBuff = ResourcesFactory::CreateBufferFromMemory( name, data.Data, data.CreateBufferInfo() );
	if ( !constBuff )		// Bufor móg³ siê nie stworzyæ, a nie chcemy dodawaæ nullptra do ModelsManagera
		return nullptr;

	m_constantBuffer.UnsafeAdd( name, constBuff );	// Dodaliœmy bufor
	return ResourcePtr<BufferObject>( constBuff );
}

//====================================================================================//
//			Listowanie assetów
//====================================================================================//


/**@brief Listowanie buforów wierzcho³ków.*/
std::vector< ResourcePtr< BufferObject > >		ModelsManager::ListVertexBuffers()
{
	return m_vertexBuffer.List();
}

/**@brief Listowanie buforów indeksów.*/
std::vector< ResourcePtr< BufferObject > >		ModelsManager::ListIndexBuffers()
{
	return m_indexBuffer.List();
}

/**@brief Listowanie buforów sta³ych.*/
std::vector< ResourcePtr< BufferObject > >		ModelsManager::ListConstantBuffers()
{
	return m_constantBuffer.List();
}

/**@brief Listowanie layoutów wierzcho³ków.*/
std::vector< ResourcePtr< ShaderInputLayout > > ModelsManager::ListShaderLayouts()
{
	return m_vertexLayout.List();
}

/**@brief Listowanie tekstur.*/
std::vector< ResourcePtr< TextureObject > >		ModelsManager::ListTextures()
{
	return m_texture.List();
}

/**@brief Listowanie vertex shaderów.*/
std::vector< ResourcePtr< VertexShader > >		ModelsManager::ListVertexShaders()
{
	return m_vertexShader.List();
}

/**@brief Listowanie pixel shaderów.*/
std::vector< ResourcePtr< PixelShader > >		ModelsManager::ListPixelShaders()
{
	return m_pixelShader.List();
}

/**@brief Listowanie materia³ów.*/
std::vector< ResourcePtr< MaterialObject > >	ModelsManager::ListMaterials()
{
	return m_material.List();
}

/**@brief Listowanie render targetów.*/
std::vector< ResourcePtr< RenderTargetObject > > ModelsManager::ListRenderTargets()
{
	return m_renderTarget.List();
}

/**@brief Listowanie meshy.*/
std::vector< ResourcePtr< Model3DFromFile > >	ModelsManager::ListMeshes()
{
	return m_fileModel.List();
}

