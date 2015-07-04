#pragma once

#include "ReferencedObject.h"

class IBuffer	:	public referenced_object
{
private:

protected:
	IBuffer() : referenced_object( 0 ) {}
	virtual ~IBuffer();
public:

};

