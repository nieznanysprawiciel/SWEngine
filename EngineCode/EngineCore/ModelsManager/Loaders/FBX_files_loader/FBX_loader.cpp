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
	//nie zwalniamy fbx_IOsettings, bo zajmuje si� tym fbx_manager
}

/**@brief Sprawdza czy umie wczyta� podany plik.
Funkcja robi to na podstawie rozszerzenia pliku.

Obs�ugiwane formatu plik�w to:
- FBX*/
bool FBX_loader::can_load(const std::wstring& name)
{
	// Funkcja por�wnujaca rozszerzenia ignoruj�c r�nice w wielko�ci liter
	auto compare_fun = []( wchar_t* str1, wchar_t* str2 )->bool
	{
		for ( int i = 0; i < 4; ++i )
			if ( toupper( str1[i] ) != toupper( str2[i] ) )
				 return false;
		return true;
	};

	// Wyjmujemy z nazwy pliku jego rozszerzenie (razem z kropk�)
	wchar_t extension[4];
	extension[0] = name[name.length() - 4];
	extension[1] = name[name.length( ) - 3];
	extension[2] = name[name.length( ) - 2];
	extension[3] = name[name.length( ) - 1];


	// Por�wnujemy rozszerzenie z obs�ugiwanymi formatami plik�w
	if ( compare_fun( extension, L".FBX" ) )
		return true;

	return false;
}


/*Funkcja wczytuje plik podany w parametrze, a odpowienie dane zapisuje w obiekcie podanym w pierwszym parametrze.
Po zako�czeniu dzia�ania obiekt powinien by� w stanie umo�liwiaj�cym natychmiastowe ponowne wywo�anie tej funkcji
do wczytania innego modelu. Ewentualne b�edy mo�na sygnalizowa� przez warto�� zwracan�.

Poprawne wykonanie funkcji powinno zwr�ci� warto�� MESH_LOADING_OK.*/
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
		//funkcj� GetStatus() mo�na pobra� informacje o b��dzie, jakby by�o to potrzebne
		cur_model = nullptr;		//�eby potem nie by�o pomy�ek
		return MESH_LOADING_WRONG;
	}

	//tworzymy obiekt sceny
	FbxScene* scene = FbxScene::Create(fbx_manager, "Scene");
	fbx_importer->Import(scene);
	fbx_importer->Destroy();		//po wczytaniu sceny nie jest potrzebny

	//pobieramy korze� drzewa i lecimy
	FbxNode* root_node = scene->GetRootNode();
	process_tree(root_node);


	cur_model = nullptr;		//�eby potem nie by�o pomy�ek
	return MESH_LOADING_OK;
}

/**@brief Dobieramy si� do drzewa, zwr�conego przez FBX SDK.

FBX SDK wczytuje plik i tworzy dla niego drzewo. Teraz naszym zadaniem jest przej�� si� po tym drzewie
i wyci�gn�� wszystkie istotne informacje.
(nie stosujemy tu zwyk�ej funkcji process_node, poniewa� root_node nie zawiera �adnych istotnych informacji
co wynika z za�o�e� przyj�tych przez tw�rc�w formatu FBX.
@param[in] root_node Wska�nik na korze� drzewa rozk�adu pliku.*/
int FBX_loader::process_tree(FbxNode* root_node)
{
	if (root_node == nullptr)
		return MESH_LOADING_WRONG;	//zwracamy b��d, je�eli root_node by� niepoprawny

	for (int i = 0; i < root_node->GetChildCount(); i++)
		process_node(root_node->GetChild(i));

	return MESH_LOADING_OK;
}

/**@brief Przetwarzamy jeden w�ze� drzewa oraz rekurencyjnie wszystkie jego dzieci.
Wszystkie dane zostan� umieszczone w zmiennej cur_model.

Mo�na bezpiecznie poda� nullptr w parametrze.
@param[in] node W�ze� drzewa do przetworzenia.*/
void FBX_loader::process_node(FbxNode* node)
{
	if (node == nullptr)
		return;

	//ustalamy macierz przekszta�cenia dla naszego node'a
	FbxAMatrix transform_matrix = node->EvaluateGlobalTransform();
	DirectX::XMFLOAT4X4 transformation;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			transformation(i, j) = static_cast<float>(transform_matrix.Get(i, j));

	//process_materials(node);		//dodajemy nowe materia�y do listy

	for (int i = 0; i < node->GetNodeAttributeCount(); i++)
		if ((node->GetNodeAttributeByIndex(i))->GetAttributeType() == FbxNodeAttribute::eMesh)
		{//interesuj� nas tylko meshe
			process_mesh(node, (FbxMesh*)node->GetNodeAttributeByIndex(i), transformation);
		}
	//

	//rekurencyjnie schodzimy po drzewie
	for (int i = 0; i < node->GetChildCount(); i++)
		process_node(node->GetChild(i));
}


/**@brief Przetwarzamy mesha przypisanego do jednego z node'�w drzewa.

Poniewa� do r�nych wierzcho�k�w
w tym meshu mog� by� przypisane r�ne materia�y, to musimy na tym etapie podzieli� mesha na kilka cz�ci
o tym samym materiale (teksturze). Potem meshe zostaj� dodane do klasy ModelsManager, a odpowiednie odwo�ania
umieszczone w obiekcie Model3DFromFile, kt�ry dostalismy przy wywo�aniu funkcji wczytuj�cej plik.
@param[in] node Node zawieraj�cy danego mesha
@param[in] mesh Obiekt zawieraj�cy mesha
@param[in] transformation Macierz transformacji podanego node'a.*/
void FBX_loader::process_mesh(FbxNode* node, FbxMesh* mesh, const DirectX::XMFLOAT4X4& transformation)
{
	//je�eli mesh si� ju� wcze�niej pojawi� w innym nodzie, to mamy go zapisanego.
	//funkcja ta go przetworzy, a my nie musimy ju� tutaj nic robi�
	/*if (process_existing_mesh(node, mesh, transformation))
		return;*/

	int material_count = node->GetMaterialCount();
	std::vector<VertexNormalTexCord1>** triangles;		//nasza tablica wierzcho�k�w, normalnych i UVs (ta tablica b�dzie tylko tymczasowa)
	unsigned int polygon_count = mesh->GetPolygonCount();	//liczba wielok�t�w
	unsigned int vertex_counter = 0;						//potrzebne przy trybie mapowania FbxGeometryElement::eByPolygonVertex
	unsigned int polygon_counter = 0;						//potrzebne przy trybie mapowania materia��w FbxGeometryElement::eByPolygon
	/*Potrzebujemy dw�ch cunter�w dla vertex�w i polygon�w, poniewa� polygony mog� teoretycznie mie� wi�cej ni� 3 wierzcho�ki*/

	//tworzymy tyle vector�w na wierzcho�ki, ile mamy materia��w
	if (material_count == 0)
		material_count = 1;		//potrzebujemy przynajmniej jednej tablicy
	triangles = new std::vector<VertexNormalTexCord1>*[material_count];
	for (int i = 0; i < material_count; ++i)
		triangles[i] = new std::vector < VertexNormalTexCord1 >;


	/*Iterujemy po wierzcho�kach i wpisujemy je do odpowiedniej tablicy w zalezno�ci od materia�u.*/
	for (unsigned int i = 0; i < polygon_count; ++i)
	{
		//pobieramy index materia�u (tylko raz na ka�dy polygon)
		int material_index;
		material_index = read_material_index(mesh, polygon_counter);

		/*Tu si� dzieje ma�e oszustwo. Sprawdzamy tylko 3 wierzcho�ki, jakby to by�y tr�jk�ty,
		a to moga by� dowolne wielok�ty. Tak naprawd� to tylko mamy nadziej�, �e nikomu nie przyjdzie
		do g�owy, �eby u�y� czego� innego ni� trojk�t�w, no ale trudno.*/
		for (int j = 0; j < 3; ++j)
		{
			VertexNormalTexCord1 cur_vertex;		//tutaj zapisujemy dane o wierzcho�ku
			int ctr_point = mesh->GetPolygonVertex(i, j);

			//przepisujemy pozycj� wierzcho�ka
			cur_vertex.position.x = static_cast<float>(mesh->GetControlPointAt(ctr_point).mData[0]);
			cur_vertex.position.y = static_cast<float>(mesh->GetControlPointAt(ctr_point).mData[1]);
			cur_vertex.position.z = static_cast<float>(mesh->GetControlPointAt(ctr_point).mData[2]);
		
			//przepisujemy normaln�
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

			//dodajemy wierzcho�ek do odpowiedniej tablicy
			triangles[material_index]->push_back(cur_vertex);

			++vertex_counter;		//zliczamy wierzcho�ki
		}

		++polygon_counter;
	}


	/*Zebrali�my wszystkie dane do wektor�w, to teraz trzeba je przenie�� do naszego obiektu Model3DFromFile.*/
		//najpierw przypadek, w kt�rym nie ma materia�u (czyli tekstury te� nie ma)
	if (node->GetMaterialCount() == 0)
	{
		cur_model->BeginPart();		// Te funkcje maj� otacza� dodawanie ka�dego kolejnego parta

		//Nie trzeba dodawa� nulli tak jak wcze�niej, bo one si� domy�lnie tam znajduj�
		//cur_model->add_null_material();
		//cur_model->add_null_texture();
		cur_model->add_vertex_buffer( triangles[0]->data(), triangles[0]->size());
		cur_model->add_transformation( transformation );

		cur_model->EndPart( );		// Te funkcje maj� otacza� dodawanie ka�dego kolejnego parta
	}
	else
	{
		//teraz wszystkie inne przypadki; iterujemy po materia�ach
		for ( int i = 0; i < material_count; ++i )
		{
			cur_model->BeginPart( );		// Te funkcje maj� otacza� dodawanie ka�dego kolejnego parta

			// Obiekty do konwertowania string�w
			typedef std::codecvt_utf8<wchar_t> convert_type;
			std::wstring_convert<convert_type, wchar_t> converter;

			// Pobieramy materia�
			FbxSurfacePhong* material = static_cast<FbxSurfacePhong*>(node->GetMaterial( i ));
			MaterialObject engine_material;							// Ten materia� jest tylko tymczasowy
			copy_material( engine_material, *material );			// Konwertujemy z formatu FBX na MaterialObject
			// Dodajemy do silnika, podajemy w drugim parametrze nazw� materia�u, kt�ra zostanie doklejona do �cie�ki pliku
			cur_model->add_material( &engine_material, converter.from_bytes( material->GetName( )) );

			if ( material->Diffuse.GetSrcObjectCount() > 0 )
			{
				FbxFileTexture* texture = static_cast<FbxFileTexture*>(material->Diffuse.GetSrcObject());
				if ( texture != nullptr )
					cur_model->add_texture( converter.from_bytes( texture->GetFileName() ), TEXTURES_TYPES::TEX_DIFFUSE );
					// Je�eli dodawanie si� nie uda, to tekstura pozostanie nullptrem
				//else //tutaj tekstura ma by� nullem, ale tak si� dzieje domy�lnie
			}
			//else //tutaj tekstura ma by� nullem, ale tak si� dzieje domy�lnie

			cur_model->add_vertex_buffer( triangles[i]->data( ), triangles[i]->size( ));
			cur_model->add_transformation( transformation );

			cur_model->EndPart( );		// Te funkcje maj� otacza� dodawanie ka�dego kolejnego parta
		}
	}

	//czy�cimy stworzone tablice wierzcho�k�w
	for (int i = 0; i < material_count; ++i)
		delete triangles[i];
	delete[] triangles;
}


/**@brief Dla podanego polygona odczytujemy jego indeks materia�u. Je�eli nie ma �adnych materia��w przypisanych
to dostajemy 0. Kiedy potem b�dziemy wybiera�, do kt�rej tablicy mesha zapisa� wierzcho�ek, wszystko
b�dzie dzia�a�o poprawnie nawet, jak materi��u nie b�dzie.*/
int FBX_loader::read_material_index(FbxMesh* mesh, unsigned int polygon_counter)
{
	if (mesh->GetElementMaterialCount() < 1)
		return 0;		//nie by�o materia��w to mamy tylko jedn� tablic� do indeksowania

	FbxGeometryElementMaterial* material = mesh->GetElementMaterial();
	int index;

	switch (material->GetMappingMode())
	{
	case FbxGeometryElement::eAllSame:
		index = material->GetIndexArray()[0];		//mamy tylko jeden materia� dla ca�ego mesha
		break;

	case FbxGeometryElement::eByPolygon:
		index = material->GetIndexArray()[polygon_counter];
		break;
	}
	return index;
}

/**@brief Wczytuje UVs dla podanego wierzcho�ka.*/
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
/**@depracated Wersja dla DirectX11 nie potrzebuje ju� materia��w DirectX9
@brief Kopiujemy materia� konwertuj�c go z formatu u�ywanego przez FBX SDK do formatu directX.*/
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

/**@brief Kopiujemy materia� konwertuj�c go z formatu u�ywanego przez FBX SDK do formatu u�ywanego w silniku.*/
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
//									juz nieu�ywane										//
//--------------------------------------------------------------------------------------//

#ifndef __UNUSED

/*Funkcja sprawdzi czy podanego mesha ju� wcze�niej nie przetwarzali�my. Je�eli ju� taki by�, to wystarczy
wzi�� jego wska�nik i powi�za� z now� macierza transforamcji. (Macierz transformacji nie mo�e by� taka sama
jak wczesniej, bo to by oznacza�o, �e wszystkie wierzcho�ki si� ze soba pokrywj�, co nie ma sensu).

Je�eli znajdziemy mesha na li�cie, to zostanie on przetworzony, a funkcja wyjdzie z warto�ci� TRUE.
Je�eli nie znajdziemy mesha, to funkcja wyjdzie z warto�ci� FALSE, nie ruszaj�c mesha.*/
bool FBX_loader::process_existing_mesh(FbxNode* node, FbxMesh* mesh, const DirectX::XMFLOAT4X4& transformation)
{

	return FALSE;
}

/*Sprawdzamy czy zarejestrowali�my ju� sobie lokalnie taki materia�.
Podejemy wska�nik FbxSurfacePhong* w parametrze, a dostajemy wska�nik na materia� w zmiennej directXmaterial.
Zwraca TRUE je�eli materia� ju� istnia�.
Je�eli materia� nie istnia� w pierwszym parametrze znajdzie si� nullptr.*/

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


/*Zapisujemy par� MaterialObject-FBXmaterial na li�cie w celu �atwiejszego dost�pu.*/
void FBX_loader::add_pair(MaterialObject* directXmaterial, FbxSurfacePhong* FBXmaterial)
{
	materials.push_back(std::make_pair(directXmaterial, FBXmaterial));
}


/*Pobieramy materialy przypisane do danego node'a, dodajemy do obiektu ModelsManager.
Ponadto tworzymy list� ��cz�c� wska�niki na obiekty materia��w w ModelsManager
z ich tymczasowymi odpowiednikami w pliku. Dzi�ki temu napotykaj�c ten sam materia�
jeste�my w stanie od razu stwierdzi�, jaki wska�nik w tej sytuacji przypisa�, bez odpytywania
wy�szej instancji.*/
void FBX_loader::process_materials(FbxNode* node)
{
	//pobieramy materia�y przypisane do danego node'a
	for (int i = 0; i < node->GetMaterialCount(); i++)
	{
		MaterialObject* directXmaterial_ptr;

		//tu trzeba b�dzie potem co� zrobi�, bo mo�e by� jeszcze FbxLambertSurface
		FbxSurfacePhong* material = (FbxSurfacePhong*)node->GetMaterial(i);
		if (!check_if_exists(&directXmaterial_ptr, material))	//je�eli mamy ju� zarejestrowany materia�, to nic nie robimy na tym etapie
		{//materia�u nie mamy zapisanego, wi�c tworzymy
			D3DMATERIAL9 directXmaterial;
			unsigned int id;							//ModelsManager zwr�ci nam identyfikator materia�u do tej zmiennej
			copy_material(directXmaterial, *material);	//konwertujemy materia�

			//pr�bujemy doda� materia� do ModelsManagera
			//Mo�e by� sytuacja, �e kto� zrobi� dwa takie same materia�y, wtedy dostaniemy wska�nik na materia�,
			//kt�ry ju� wczesniej istnia�. Generalnie ta sytuacja nam nie przeszkadza. Wa�ne �e i tak musimy sobie
			//zapisac par� materia�-MaterialObject, �eby m�c si� potem odwo�ywa�.
			MaterialObject* material_object = models_manager->add_material(directXmaterial, id);

			//dodajemy powi�zanie materia�u z odpowiadaj�cym mu obiektem
			add_pair(material_object, material);
		}

	}
}

#endif