#include "stdafx.h"
#include "FBX_loader.h"


#include "Common\memory_leaks.h"


///@brief Kontruktr inicjuje obiekty FBX SDK, konieczne do wczytania modelu.
FBX_loader::FBX_loader(ModelsManager* models_manager)
	: ILoader(models_manager)
{
	fbx_manager = FbxManager::Create();
	fbx_IOsettings = FbxIOSettings::Create(fbx_manager, IOSROOT);
	fbx_manager->SetIOSettings(fbx_IOsettings);

	cur_model = nullptr;
}


FBX_loader::~FBX_loader()
{
	fbx_manager->Destroy();
	//nie zwalniamy fbx_IOsettings, bo zajmuje siê tym fbx_manager
}

/**@brief Sprawdza czy umie wczytaæ podany plik.
Funkcja robi to na podstawie rozszerzenia pliku.

Obs³ugiwane formatu plików to:
- FBX*/
bool FBX_loader::can_load(const std::wstring& name)
{
	// Funkcja porównujaca rozszerzenia ignoruj¹c ró¿nice w wielkoœci liter
	auto compare_fun = []( wchar_t* str1, wchar_t* str2 )->bool
	{
		for ( int i = 0; i < 4; ++i )
			if ( toupper( str1[i] ) != toupper( str2[i] ) )
				 return false;
		return true;
	};

	// Wyjmujemy z nazwy pliku jego rozszerzenie (razem z kropk¹)
	wchar_t extension[4];
	extension[0] = name[name.length() - 4];
	extension[1] = name[name.length( ) - 3];
	extension[2] = name[name.length( ) - 2];
	extension[3] = name[name.length( ) - 1];


	// Porównujemy rozszerzenie z obs³ugiwanymi formatami plików
	if ( compare_fun( extension, L".FBX" ) )
		return true;

	return false;
}


/*Funkcja wczytuje plik podany w parametrze, a odpowienie dane zapisuje w obiekcie podanym w pierwszym parametrze.
Po zakoñczeniu dzia³ania obiekt powinien byæ w stanie umo¿liwiaj¹cym natychmiastowe ponowne wywo³anie tej funkcji
do wczytania innego modelu. Ewentualne b³edy mo¿na sygnalizowaæ przez wartoœæ zwracan¹.

Poprawne wykonanie funkcji powinno zwróciæ wartoœæ MESH_LOADING_OK.*/
LOADER_RESULT FBX_loader::load_mesh( Model3DFromFile* new_file_mesh, const std::wstring& name )
{
	if (new_file_mesh == nullptr)
		return MESH_LOADING_WRONG;
	cur_model = new_file_mesh;

	FbxImporter* fbx_importer = FbxImporter::Create(fbx_manager, "");

	typedef std::codecvt_utf8<wchar_t> convert_type;
	std::wstring_convert<convert_type, wchar_t> converter;
	std::string converted_name = converter.to_bytes( name );

	if ( !fbx_importer->Initialize( converted_name.c_str(), -1, fbx_manager->GetIOSettings() ) )
	{
		//funkcj¹ GetStatus() mo¿na pobraæ informacje o b³êdzie, jakby by³o to potrzebne
		cur_model = nullptr;		//¿eby potem nie by³o pomy³ek
		return MESH_LOADING_WRONG;
	}

	//tworzymy obiekt sceny
	FbxScene* scene = FbxScene::Create(fbx_manager, "Scene");
	fbx_importer->Import(scene);
	fbx_importer->Destroy();		//po wczytaniu sceny nie jest potrzebny

	//pobieramy korzeñ drzewa i lecimy
	FbxNode* root_node = scene->GetRootNode();
	process_tree(root_node);


	cur_model = nullptr;		//¿eby potem nie by³o pomy³ek
	return MESH_LOADING_OK;
}

/**@brief Dobieramy siê do drzewa, zwróconego przez FBX SDK.

FBX SDK wczytuje plik i tworzy dla niego drzewo. Teraz naszym zadaniem jest przejœæ siê po tym drzewie
i wyci¹gn¹æ wszystkie istotne informacje.
(nie stosujemy tu zwyk³ej funkcji process_node, poniewa¿ root_node nie zawiera ¿adnych istotnych informacji
co wynika z za³o¿eñ przyjêtych przez twórców formatu FBX.
@param[in] root_node WskaŸnik na korzeñ drzewa rozk³adu pliku.*/
int FBX_loader::process_tree(FbxNode* root_node)
{
	if (root_node == nullptr)
		return MESH_LOADING_WRONG;	//zwracamy b³¹d, je¿eli root_node by³ niepoprawny

	for (int i = 0; i < root_node->GetChildCount(); i++)
		process_node(root_node->GetChild(i));

	return MESH_LOADING_OK;
}

/**@brief Przetwarzamy jeden wêze³ drzewa oraz rekurencyjnie wszystkie jego dzieci.
Wszystkie dane zostan¹ umieszczone w zmiennej cur_model.

Mo¿na bezpiecznie podaæ nullptr w parametrze.
@param[in] node Wêze³ drzewa do przetworzenia.*/
void FBX_loader::process_node(FbxNode* node)
{
	if (node == nullptr)
		return;

	//ustalamy macierz przekszta³cenia dla naszego node'a
	FbxAMatrix transform_matrix = node->EvaluateGlobalTransform();
	DirectX::XMFLOAT4X4 transformation;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			transformation(i, j) = static_cast<float>(transform_matrix.Get(i, j));

	//process_materials(node);		//dodajemy nowe materia³y do listy

	for (int i = 0; i < node->GetNodeAttributeCount(); i++)
		if ((node->GetNodeAttributeByIndex(i))->GetAttributeType() == FbxNodeAttribute::eMesh)
		{//interesuj¹ nas tylko meshe
			process_mesh(node, (FbxMesh*)node->GetNodeAttributeByIndex(i), transformation);
		}
	//

	//rekurencyjnie schodzimy po drzewie
	for (int i = 0; i < node->GetChildCount(); i++)
		process_node(node->GetChild(i));
}


/**@brief Przetwarzamy mesha przypisanego do jednego z node'ów drzewa.

Poniewa¿ do ró¿nych wierzcho³ków
w tym meshu mog¹ byæ przypisane ró¿ne materia³y, to musimy na tym etapie podzieliæ mesha na kilka czêœci
o tym samym materiale (teksturze). Potem meshe zostaj¹ dodane do klasy ModelsManager, a odpowiednie odwo³ania
umieszczone w obiekcie Model3DFromFile, który dostalismy przy wywo³aniu funkcji wczytuj¹cej plik.
@param[in] node Node zawieraj¹cy danego mesha
@param[in] mesh Obiekt zawieraj¹cy mesha
@param[in] transformation Macierz transformacji podanego node'a.*/
void FBX_loader::process_mesh(FbxNode* node, FbxMesh* mesh, const DirectX::XMFLOAT4X4& transformation)
{
	//je¿eli mesh siê ju¿ wczeœniej pojawi³ w innym nodzie, to mamy go zapisanego.
	//funkcja ta go przetworzy, a my nie musimy ju¿ tutaj nic robiæ
	/*if (process_existing_mesh(node, mesh, transformation))
		return;*/

	int material_count = node->GetMaterialCount();
	std::vector<VertexNormalTexCord1>** triangles;		//nasza tablica wierzcho³ków, normalnych i UVs (ta tablica bêdzie tylko tymczasowa)
	unsigned int polygon_count = mesh->GetPolygonCount();	//liczba wielok¹tów
	unsigned int vertex_counter = 0;						//potrzebne przy trybie mapowania FbxGeometryElement::eByPolygonVertex
	unsigned int polygon_counter = 0;						//potrzebne przy trybie mapowania materia³ów FbxGeometryElement::eByPolygon
	/*Potrzebujemy dwóch cunterów dla vertexów i polygonów, poniewa¿ polygony mog¹ teoretycznie mieæ wiêcej ni¿ 3 wierzcho³ki*/

	//tworzymy tyle vectorów na wierzcho³ki, ile mamy materia³ów
	if (material_count == 0)
		material_count = 1;		//potrzebujemy przynajmniej jednej tablicy
	triangles = new std::vector<VertexNormalTexCord1>*[material_count];
	for (int i = 0; i < material_count; ++i)
		triangles[i] = new std::vector < VertexNormalTexCord1 >;


	/*Iterujemy po wierzcho³kach i wpisujemy je do odpowiedniej tablicy w zaleznoœci od materia³u.*/
	for (unsigned int i = 0; i < polygon_count; ++i)
	{
		//pobieramy index materia³u (tylko raz na ka¿dy polygon)
		int material_index;
		material_index = read_material_index(mesh, polygon_counter);

		/*Tu siê dzieje ma³e oszustwo. Sprawdzamy tylko 3 wierzcho³ki, jakby to by³y trójk¹ty,
		a to moga byæ dowolne wielok¹ty. Tak naprawdê to tylko mamy nadziejê, ¿e nikomu nie przyjdzie
		do g³owy, ¿eby u¿yæ czegoœ innego ni¿ trojk¹tów, no ale trudno.*/
		for (int j = 0; j < 3; ++j)
		{
			VertexNormalTexCord1 cur_vertex;		//tutaj zapisujemy dane o wierzcho³ku
			int ctr_point = mesh->GetPolygonVertex(i, j);

			//przepisujemy pozycjê wierzcho³ka
			cur_vertex.position.x = static_cast<float>(mesh->GetControlPointAt(ctr_point).mData[0]);
			cur_vertex.position.y = static_cast<float>(mesh->GetControlPointAt(ctr_point).mData[1]);
			cur_vertex.position.z = static_cast<float>(mesh->GetControlPointAt(ctr_point).mData[2]);
		
			//przepisujemy normaln¹
			FbxVector4 fbx_normal;
			mesh->GetPolygonVertexNormal(i, j, fbx_normal);
			cur_vertex.normal.x = static_cast<float>(fbx_normal.mData[0]);
			cur_vertex.normal.y = static_cast<float>(fbx_normal.mData[1]);
			cur_vertex.normal.z = static_cast<float>(fbx_normal.mData[2]);

			//przepisujemy UVs
			/*bool mapped;
			FbxVector2 UVs;
			mesh->GetPolygonVertexUV(i, j, "DiffuseUV", UVs, mapped);
			cur_vertex.tex_cords1.x = static_cast<float>(UVs.mData[0]);
			cur_vertex.tex_cords1.y = static_cast<float>(UVs.mData[1]);*/
			read_UVs(mesh, ctr_point, vertex_counter, cur_vertex.tex_cords);

			//dodajemy wierzcho³ek do odpowiedniej tablicy
			triangles[material_index]->push_back(cur_vertex);

			++vertex_counter;		//zliczamy wierzcho³ki
		}

		++polygon_counter;
	}


	/*Zebraliœmy wszystkie dane do wektorów, to teraz trzeba je przenieœæ do naszego obiektu Model3DFromFile.*/
		//najpierw przypadek, w którym nie ma materia³u (czyli tekstury te¿ nie ma)
	if (node->GetMaterialCount() == 0)
	{
		cur_model->BeginPart();		// Te funkcje maj¹ otaczaæ dodawanie ka¿dego kolejnego parta

		//Nie trzeba dodawaæ nulli tak jak wczeœniej, bo one siê domyœlnie tam znajduj¹
		//cur_model->add_null_material();
		//cur_model->add_null_texture();
		cur_model->add_vertex_buffer( triangles[0]->data(), triangles[0]->size());
		cur_model->add_transformation( transformation );

		cur_model->EndPart( );		// Te funkcje maj¹ otaczaæ dodawanie ka¿dego kolejnego parta
	}
	else
	{
		//teraz wszystkie inne przypadki; iterujemy po materia³ach
		for ( int i = 0; i < material_count; ++i )
		{
			cur_model->BeginPart( );		// Te funkcje maj¹ otaczaæ dodawanie ka¿dego kolejnego parta

			// Obiekty do konwertowania stringów
			typedef std::codecvt_utf8<wchar_t> convert_type;
			std::wstring_convert<convert_type, wchar_t> converter;

			// Pobieramy materia³
			FbxSurfacePhong* material = static_cast<FbxSurfacePhong*>(node->GetMaterial( i ));
			MaterialObject engine_material;							// Ten materia³ jest tylko tymczasowy
			copy_material( engine_material, *material );			// Konwertujemy z formatu FBX na MaterialObject
			// Dodajemy do silnika, podajemy w drugim parametrze nazwê materia³u, która zostanie doklejona do œcie¿ki pliku
			cur_model->add_material( &engine_material, converter.from_bytes( material->GetName( )) );

			if ( material->Diffuse.GetSrcObjectCount() > 0 )
			{
				FbxFileTexture* texture = static_cast<FbxFileTexture*>(material->Diffuse.GetSrcObject());
				if ( texture != nullptr )
					cur_model->add_texture( converter.from_bytes( texture->GetFileName() ), TEXTURES_TYPES::TEX_DIFFUSE );
					// Je¿eli dodawanie siê nie uda, to tekstura pozostanie nullptrem
				//else //tutaj tekstura ma byæ nullem, ale tak siê dzieje domyœlnie
			}
			//else //tutaj tekstura ma byæ nullem, ale tak siê dzieje domyœlnie

			cur_model->add_vertex_buffer( triangles[i]->data( ), triangles[i]->size( ));
			cur_model->add_transformation( transformation );

			cur_model->EndPart( );		// Te funkcje maj¹ otaczaæ dodawanie ka¿dego kolejnego parta
		}
	}

	//czyœcimy stworzone tablice wierzcho³ków
	for (int i = 0; i < material_count; ++i)
		delete triangles[i];
	delete[] triangles;
}


/**@brief Dla podanego polygona odczytujemy jego indeks materia³u. Je¿eli nie ma ¿adnych materia³ów przypisanych
to dostajemy 0. Kiedy potem bêdziemy wybieraæ, do której tablicy mesha zapisaæ wierzcho³ek, wszystko
bêdzie dzia³a³o poprawnie nawet, jak materi¹³u nie bêdzie.*/
int FBX_loader::read_material_index(FbxMesh* mesh, unsigned int polygon_counter)
{
	if (mesh->GetElementMaterialCount() < 1)
		return 0;		//nie by³o materia³ów to mamy tylko jedn¹ tablicê do indeksowania

	FbxGeometryElementMaterial* material = mesh->GetElementMaterial();
	int index;

	switch (material->GetMappingMode())
	{
	case FbxGeometryElement::eAllSame:
		index = material->GetIndexArray()[0];		//mamy tylko jeden materia³ dla ca³ego mesha
		break;

	case FbxGeometryElement::eByPolygon:
		index = material->GetIndexArray()[polygon_counter];
		break;
	}
	return index;
}

/**@brief Wczytuje UVs dla podanego wierzcho³ka.*/
void FBX_loader::read_UVs(FbxMesh* mesh, int control_point, unsigned int vertex_counter, XMFLOAT2& UV_cords)
{
	if (mesh->GetUVLayerCount() < 1)
		return;
	
	FbxGeometryElementUV* UVs = mesh->GetElementUV();
	int index;

	switch (UVs->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
		switch (UVs->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			UV_cords.x = static_cast<float>(UVs->GetDirectArray().GetAt(control_point).mData[0]);
			UV_cords.y = static_cast<float>(UVs->GetDirectArray().GetAt(control_point).mData[1]);
			break;

		case FbxGeometryElement::eIndexToDirect:
			index = UVs->GetIndexArray().GetAt(control_point);
			UV_cords.x = static_cast<float>(UVs->GetDirectArray().GetAt(index).mData[0]);
			UV_cords.y = static_cast<float>(UVs->GetDirectArray().GetAt(index).mData[1]);
			break;
		}
		break;

	case FbxGeometryElement::eByPolygonVertex:
		switch (UVs->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			UV_cords.x = static_cast<float>(UVs->GetDirectArray().GetAt(vertex_counter).mData[0]);
			UV_cords.y = static_cast<float>(UVs->GetDirectArray().GetAt(vertex_counter).mData[1]);
			break;

		case FbxGeometryElement::eIndexToDirect:
			index = UVs->GetIndexArray().GetAt(vertex_counter);
			UV_cords.x = static_cast<float>(UVs->GetDirectArray().GetAt(index).mData[0]);
			UV_cords.y = static_cast<float>(UVs->GetDirectArray().GetAt(index).mData[1]);
			break;
		}
		break;
	}
}

#ifndef __UNUSED
/**@depracated Wersja dla DirectX11 nie potrzebuje ju¿ materia³ów DirectX9
@brief Kopiujemy materia³ konwertuj¹c go z formatu u¿ywanego przez FBX SDK do formatu directX.*/
void FBX_loader::copy_material(D3DMATERIAL9& directXmaterial, const FbxSurfacePhong& FBXmaterial)
{
	FbxDouble3 diffuse = static_cast<FbxDouble3>(FBXmaterial.Diffuse.Get());
	FbxDouble3 ambient = static_cast<FbxDouble3>(FBXmaterial.Ambient.Get());
	FbxDouble3 emissive = static_cast<FbxDouble3>(FBXmaterial.Emissive.Get());
	FbxDouble3 specular = static_cast<FbxDouble3>(FBXmaterial.Specular.Get());
	FbxDouble diffuse_factor = static_cast<FbxDouble>(FBXmaterial.DiffuseFactor.Get());
	FbxDouble ambient_factor = static_cast<FbxDouble>(FBXmaterial.AmbientFactor.Get());
	FbxDouble emissive_factor = static_cast<FbxDouble>(FBXmaterial.EmissiveFactor.Get());
	FbxDouble specular_factor = static_cast<FbxDouble>(FBXmaterial.SpecularFactor.Get());
	FbxDouble power = static_cast<FbxDouble>(FBXmaterial.Shininess.Get());
	FbxDouble transparent = static_cast<FbxDouble>(FBXmaterial.TransparencyFactor.Get());

	directXmaterial.Diffuse.a = static_cast<float>(transparent);

	directXmaterial.Diffuse.r = static_cast<float>(diffuse[0] * diffuse_factor);
	directXmaterial.Diffuse.g = static_cast<float>(diffuse[1] * diffuse_factor);
	directXmaterial.Diffuse.b = static_cast<float>(diffuse[2] * diffuse_factor);

	directXmaterial.Ambient.r = static_cast<float>(ambient[0] * ambient_factor);
	directXmaterial.Ambient.g = static_cast<float>(ambient[1] * ambient_factor);
	directXmaterial.Ambient.b = static_cast<float>(ambient[2] * ambient_factor);

	directXmaterial.Emissive.r = static_cast<float>(emissive[0] * emissive_factor);
	directXmaterial.Emissive.g = static_cast<float>(emissive[1] * emissive_factor);
	directXmaterial.Emissive.b = static_cast<float>(emissive[2] * emissive_factor);

	directXmaterial.Specular.r = static_cast<float>(specular[0] * specular_factor);
	directXmaterial.Specular.g = static_cast<float>(specular[1] * specular_factor);
	directXmaterial.Specular.b = static_cast<float>(specular[2] * specular_factor);

	directXmaterial.Power = static_cast<float>(power);
}

#endif

/**@brief Kopiujemy materia³ konwertuj¹c go z formatu u¿ywanego przez FBX SDK do formatu u¿ywanego w silniku.*/
void FBX_loader::copy_material( MaterialObject& engine_material, const FbxSurfacePhong& FBXmaterial )
{
	FbxDouble3 diffuse = static_cast<FbxDouble3>(FBXmaterial.Diffuse.Get( ));
	FbxDouble3 ambient = static_cast<FbxDouble3>(FBXmaterial.Ambient.Get( ));
	FbxDouble3 emissive = static_cast<FbxDouble3>(FBXmaterial.Emissive.Get( ));
	FbxDouble3 specular = static_cast<FbxDouble3>(FBXmaterial.Specular.Get( ));
	FbxDouble diffuse_factor = static_cast<FbxDouble>(FBXmaterial.DiffuseFactor.Get( ));
	FbxDouble ambient_factor = static_cast<FbxDouble>(FBXmaterial.AmbientFactor.Get( ));
	FbxDouble emissive_factor = static_cast<FbxDouble>(FBXmaterial.EmissiveFactor.Get( ));
	FbxDouble specular_factor = static_cast<FbxDouble>(FBXmaterial.SpecularFactor.Get( ));
	FbxDouble power = static_cast<FbxDouble>(FBXmaterial.Shininess.Get( ));
	FbxDouble transparent = static_cast<FbxDouble>(FBXmaterial.TransparencyFactor.Get( ));

	engine_material.Diffuse.w = static_cast<float>(transparent);

	engine_material.Diffuse.x = static_cast<float>(diffuse[0] * diffuse_factor);
	engine_material.Diffuse.y = static_cast<float>(diffuse[1] * diffuse_factor);
	engine_material.Diffuse.z = static_cast<float>(diffuse[2] * diffuse_factor);

	engine_material.Ambient.x = static_cast<float>(ambient[0] * ambient_factor);
	engine_material.Ambient.y = static_cast<float>(ambient[1] * ambient_factor);
	engine_material.Ambient.z = static_cast<float>(ambient[2] * ambient_factor);

	engine_material.Emissive.x = static_cast<float>(emissive[0] * emissive_factor);
	engine_material.Emissive.y = static_cast<float>(emissive[1] * emissive_factor);
	engine_material.Emissive.z = static_cast<float>(emissive[2] * emissive_factor);

	engine_material.Specular.x = static_cast<float>(specular[0] * specular_factor);
	engine_material.Specular.y = static_cast<float>(specular[1] * specular_factor);
	engine_material.Specular.z = static_cast<float>(specular[2] * specular_factor);

	engine_material.Power = static_cast<float>(power);
}


//--------------------------------------------------------------------------------------//
//									juz nieu¿ywane										//
//--------------------------------------------------------------------------------------//

#ifndef __UNUSED

/*Funkcja sprawdzi czy podanego mesha ju¿ wczeœniej nie przetwarzaliœmy. Je¿eli ju¿ taki by³, to wystarczy
wzi¹æ jego wskaŸnik i powi¹zaæ z now¹ macierza transforamcji. (Macierz transformacji nie mo¿e byæ taka sama
jak wczesniej, bo to by oznacza³o, ¿e wszystkie wierzcho³ki siê ze soba pokrywj¹, co nie ma sensu).

Je¿eli znajdziemy mesha na liœcie, to zostanie on przetworzony, a funkcja wyjdzie z wartoœci¹ TRUE.
Je¿eli nie znajdziemy mesha, to funkcja wyjdzie z wartoœci¹ FALSE, nie ruszaj¹c mesha.*/
bool FBX_loader::process_existing_mesh(FbxNode* node, FbxMesh* mesh, const DirectX::XMFLOAT4X4& transformation)
{

	return FALSE;
}

/*Sprawdzamy czy zarejestrowaliœmy ju¿ sobie lokalnie taki materia³.
Podejemy wskaŸnik FbxSurfacePhong* w parametrze, a dostajemy wskaŸnik na materia³ w zmiennej directXmaterial.
Zwraca TRUE je¿eli materia³ ju¿ istnia³.
Je¿eli materia³ nie istnia³ w pierwszym parametrze znajdzie siê nullptr.*/

bool FBX_loader::check_if_exists(MaterialObject** directXmaterial, FbxSurfacePhong* FBXmaterial)
{
	for (unsigned int i = 0; i < materials.size(); ++i)
		if (materials[i].second == FBXmaterial)
		{
			*directXmaterial = materials[i].first;
			return TRUE;
		}
	*directXmaterial = nullptr;
	return FALSE;
}


/*Zapisujemy parê MaterialObject-FBXmaterial na liœcie w celu ³atwiejszego dostêpu.*/
void FBX_loader::add_pair(MaterialObject* directXmaterial, FbxSurfacePhong* FBXmaterial)
{
	materials.push_back(std::make_pair(directXmaterial, FBXmaterial));
}


/*Pobieramy materialy przypisane do danego node'a, dodajemy do obiektu ModelsManager.
Ponadto tworzymy listê ³¹cz¹c¹ wskaŸniki na obiekty materia³ów w ModelsManager
z ich tymczasowymi odpowiednikami w pliku. Dziêki temu napotykaj¹c ten sam materia³
jesteœmy w stanie od razu stwierdziæ, jaki wskaŸnik w tej sytuacji przypisaæ, bez odpytywania
wy¿szej instancji.*/
void FBX_loader::process_materials(FbxNode* node)
{
	//pobieramy materia³y przypisane do danego node'a
	for (int i = 0; i < node->GetMaterialCount(); i++)
	{
		MaterialObject* directXmaterial_ptr;

		//tu trzeba bêdzie potem coœ zrobiæ, bo mo¿e byæ jeszcze FbxLambertSurface
		FbxSurfacePhong* material = (FbxSurfacePhong*)node->GetMaterial(i);
		if (!check_if_exists(&directXmaterial_ptr, material))	//je¿eli mamy ju¿ zarejestrowany materia³, to nic nie robimy na tym etapie
		{//materia³u nie mamy zapisanego, wiêc tworzymy
			D3DMATERIAL9 directXmaterial;
			unsigned int id;							//ModelsManager zwróci nam identyfikator materia³u do tej zmiennej
			copy_material(directXmaterial, *material);	//konwertujemy materia³

			//próbujemy dodaæ materia³ do ModelsManagera
			//Mo¿e byæ sytuacja, ¿e ktoœ zrobi³ dwa takie same materia³y, wtedy dostaniemy wskaŸnik na materia³,
			//który ju¿ wczesniej istnia³. Generalnie ta sytuacja nam nie przeszkadza. Wa¿ne ¿e i tak musimy sobie
			//zapisac parê materia³-MaterialObject, ¿eby móc siê potem odwo³ywaæ.
			MaterialObject* material_object = models_manager->add_material(directXmaterial, id);

			//dodajemy powi¹zanie materia³u z odpowiadaj¹cym mu obiektem
			add_pair(material_object, material);
		}

	}
}

#endif