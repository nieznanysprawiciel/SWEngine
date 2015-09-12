#pragma once
/**@file AbstractTexturedRegion.h
@brief Plik zawiera deklaracj� klasy AbstractTexturedRegion.*/

#include "XGUI.h"

namespace XGUI
{
	class Texture;

	/**@brief Klasa jest interfejsem dla obiekt�w renderuj�cych oteksturowany region.*/
	class AbstractTexturedRegion : public Control
	{
	private:
		Texture*			texture;		///<Wska�nik na obiekt tekstury. Nie ma sk��dowych ani funkcji. S�u�y jedynie jako identyfikator dla renderera.
	protected:
		int8				enable_onChangeTexture : 1;		///<W��cza wywo�ywanie funkcji w momencie powstania eventu

		virtual void onChangeTexture();
	public:
		AbstractTexturedRegion( Control* parent );
		~AbstractTexturedRegion();

		inline Texture* getTexture() const { return texture; }	///<Zwraca aktualnie u�ywan� tekstur�.
		void setTexture( Texture* new_texture );
	};

}

