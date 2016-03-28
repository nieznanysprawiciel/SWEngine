/************************************************************************************
*                                                                                   *
*   Copyright (c) 2013 Axel Menzel <info@axelmenzel.de>                             *
*                                                                                   *
*   This file is part of the Runtime Type Reflection System (RTTR).                 *
*                                                                                   *
*   Permission is hereby granted, free of charge, to any person obtaining           *
*   a copy of this software and associated documentation files (the "Software"),    *
*   to deal in the Software without restriction, including without limitation       *
*   the rights to use, copy, modify, merge, publish, distribute, sublicense,        *
*   and/or sell copies of the Software, and to permit persons to whom the           *
*   Software is furnished to do so, subject to the following conditions:            *
*                                                                                   *
*   The above copyright notice and this permission notice shall be included in      *
*   all copies or substantial portions of the Software.                             *
*                                                                                   *
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR      *
*   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,        *
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE     *
*   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER          *
*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   *
*   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE   *
*   SOFTWARE.                                                                       *
*                                                                                   *
*************************************************************************************/

#include "TypeInfo.h"

#include <map>
#include <typeinfo>


#include "RegisterClass.h"


using namespace std;

typedef map<const string, const RTTR::TypeInfo::TypeId> NameToTag;
typedef map< RTTR::TypeInfo::TypeId, std::vector< std::unique_ptr< IMetaProperty > > > ClassToProperties;

#define RTTR_MAX_TYPE_COUNT 32767
#define RTTR_MAX_INHERIT_TYPES_COUNT 50

static std::string              *g_nameList         = NULL;
static RTTR::TypeInfo::TypeId   *g_inheritList      = NULL;
static RTTR::TypeInfo::TypeId   *g_rawTypeList      = NULL;

struct TypeInfoData
{
    TypeInfoData()
    :   globalIDCounter(0)
    {
        g_nameList          = nameList;
        g_inheritList       = inheritList;
        g_rawTypeList       = rawTypeList;
    }

    static TypeInfoData& instance()
    {
        static TypeInfoData obj;
        return obj;
    }

    NameToTag				nameToTagMap;
	ClassToProperties		classToPropertyMap;

    RTTR::TypeInfo::TypeId  globalIDCounter;
    std::string             nameList[RTTR_MAX_TYPE_COUNT];
    RTTR::TypeInfo::TypeId  inheritList[RTTR_MAX_TYPE_COUNT * RTTR_MAX_INHERIT_TYPES_COUNT];
    RTTR::TypeInfo::TypeId  rawTypeList[RTTR_MAX_TYPE_COUNT];
};

namespace RTTR
{

/////////////////////////////////////////////////////////////////////////////////////////

std::string TypeInfo::getName() const
{
    if (isValid())
        return g_nameList[m_id];
    else
        return std::string();
}

/////////////////////////////////////////////////////////////////////////////////////////

TypeInfo TypeInfo::getRawType() const
{
    if (isValid())
        return TypeInfo(g_rawTypeList[m_id]);
    else
        return TypeInfo();
}

/////////////////////////////////////////////////////////////////////////////////////////

bool TypeInfo::isTypeDerivedFrom(const TypeInfo& other) const
{
    const TypeInfo::TypeId thisRawId = g_rawTypeList[m_id];
    const TypeInfo::TypeId otherRawId = g_rawTypeList[other.m_id];
    if (thisRawId == otherRawId)
        return true;

    const int row = RTTR_MAX_INHERIT_TYPES_COUNT * thisRawId;
    for (int i = 0; i < RTTR_MAX_INHERIT_TYPES_COUNT; ++i)
    {
        const TypeInfo::TypeId currId = g_inheritList[row + i];
        if (currId == otherRawId)
            return true;
        if (currId == 0) // invalid id
            return false;
    }
    return false;
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

namespace impl
{


RTTR::TypeInfo::TypeId registerTypeOnly( const char* name, const TypeInfo& rawTypeInfo, 
                           const std::vector<TypeInfo>& baseClassList )
{
	TypeInfoData& data = TypeInfoData::instance();

    const pair<NameToTag::iterator,bool> ret = data.nameToTagMap.insert(make_pair(name, ++data.globalIDCounter));
    if (ret.second)
    {
        g_nameList[data.globalIDCounter] = name;
        const TypeInfo::TypeId rawId = ((rawTypeInfo.getId() == 0) ? data.globalIDCounter : rawTypeInfo.getId());
        g_rawTypeList[data.globalIDCounter] = rawId;
        const int row = RTTR_MAX_INHERIT_TYPES_COUNT * rawId;
        int index = 0;
        // to do remove double entries
        for (std::vector<TypeInfo>::const_iterator itr = baseClassList.begin();
             itr != baseClassList.end();
             ++itr, ++index)
        {
            g_inheritList[row + index] = (*itr).getId();
        }
    } // else cannot happen!


    return ret.first->second;
}

/////////////////////////////////////////////////////////////////////////////////////////

TypeInfo registerOrGetType(const char* name, const TypeInfo& rawTypeInfo, 
                           const std::vector<TypeInfo>& baseClassList)
{
    TypeInfoData& data = TypeInfoData::instance();
    {
        NameToTag::const_iterator itr = data.nameToTagMap.find(name);
        if (itr != data.nameToTagMap.end())
            return TypeInfo(itr->second);
    }

	return TypeInfo( registerTypeOnly( name, rawTypeInfo, baseClassList ) );
}

void impl::registerProperties( const TypeInfo& rawTypeInfo, ::RTTR::ClassMetaInfoContainer (*metaDataCreateFun)() )
{
	TypeInfoData& data = TypeInfoData::instance();

	if( metaDataCreateFun != nullptr )
	{
		auto rawId = rawTypeInfo.getRawType().getId();

		auto iter = data.classToPropertyMap.find( rawId );
		if( iter == data.classToPropertyMap.end() )
		{
			ClassMetaInfoContainer container = metaDataCreateFun();
			data.classToPropertyMap.insert( std::make_pair( rawId, std::move( container.properties ) ) );
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace impl

} // end namespace RTTR
