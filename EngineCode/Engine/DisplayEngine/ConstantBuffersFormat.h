#include "..\..\stdafx.h"
#include "..\ModelsManager\meshes_textures_materials.h"

using namespace DirectX;

/** \brief Bufor sta�ych dla shadera zmieniaj�cy si� raz na klatk�. 

Bufory sta�ych dla shader�w musz� mie� wielko�� b�d�c� wielokrotno�ci� 16.
Wszystkie dane w GPU s� przechowywane w postaci wektor�w 4-wymiarowych.
Dlatego lepiej nie wysy�a� danych o innej liczbie wymiar�w, bo na pewno co� 
si� zepsuje.

Ewentualnie je�eli chcemy usprawni� dost�p do danych po strnie CPU i podawa�
wektory 3-wymiarowe, to trzeba uzupe�ni� czwarta wsp�rz�dn� jakim� pustym floatem.*/

typedef struct ConstantPerFrame
{
	XMFLOAT4X4		view_matrix;
	XMFLOAT4X4		projection_matrix;
	XMFLOAT4		light_direction[ENGINE_MAX_LIGHTS];		// Wsp�rz�dne s� zanegowane, �eby shader mia� mniej roboty
	XMFLOAT4		light_color[ENGINE_MAX_LIGHTS];
	XMFLOAT4		ambient_light;
	float			time;
	float			time_lag;
} ConstantPerFrame;


/** \brief Bufor sta�ych dla shadera zmieniaj�cy si� dla ka�dej cz�ci mesha.

Obiekt zawiera macierz przekszta�cenia �wiata dla danej cz�ci obiektu oraz dane o materia�ach.
Materia�y s� wektorami 4-wymiarowymi tylko ze wzgl�du na to, �e takie s� rejestry na GPU.
U�ywane s� tylko pierwsze 3 elementy wektora. Jedynie w kanale Diffuse ostatnia wsp�rz�dna odpowiada za przezroczysto��.
*/
typedef struct ConstantPerMesh
{
	XMMATRIX				world_matrix;
	DirectX::XMFLOAT4		Diffuse;		//Sk�adowa przezroczysto�ci odnosi si� do ca�ego materia�u
	DirectX::XMFLOAT4		Ambient;
	DirectX::XMFLOAT4		Specular;
	DirectX::XMFLOAT4		Emissive;
	float					Power;
} ConstantPerMesh;
