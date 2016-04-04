using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace EditorApp.Editor.Project.Actors.Properties
{
	/**Klasa bazowa dla własciwości oraz grup właściwości.
	
	Parametry aktorów mają strukturę hierarchiczną. Moga być typami prostymi:
	- int
	- float
	- bool
	- XMFloat2
	- XMFloat3
	- XMFloat4
		
	Parametry mogą być również strukturami/klasami, jeżeli dziedziczą po IEnableProperty.
		
	Dodatkowo właściwości mogą być grupowane w kategorie przy pomocy metadanych dołączanych
	do informacji i klasie.*/
	public class IProperty
	{
		//virtual System::String GetName() = 0;
	};

}
