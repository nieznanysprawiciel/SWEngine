$intro =
'#pragma once

#include "../../MeshResources.h"



class '

$inherit = "	:	public "

$classCode1 =
"
{
private:

protected:"


$classCode2 =
"				get()			{ return ; }

};
"

foreach ($file in dir)
{
	if( $file.name -match "(DX11\w+).cpp" )
	{
		$matches[1]
		
	}
	if( $file.name -match "(DX11\w+).h" )
	{
		$className = $matches[1]
		$parentClassName = $className.remove(0, 4) + "Object"
		$ctordtor =
"
	~$className();
public:
	$className();
"
		
		
		$intro + $className + $inherit + $parentClassName + $classCode1 + $ctordtor + $classCode2 | out-file $file.name
	}
}