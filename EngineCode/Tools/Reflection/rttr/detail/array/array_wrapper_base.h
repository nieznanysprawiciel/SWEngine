/************************************************************************************
*                                                                                   *
*   Copyright (c) 2014, 2015 - 2016 Axel Menzel <info@rttr.org>                     *
*                                                                                   *
*   This file is part of RTTR (Run Time Type Reflection)                            *
*   License: MIT License                                                            *
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

#ifndef RTTR_ARRAY_WRAPPER_BASE_H_
#define RTTR_ARRAY_WRAPPER_BASE_H_

#include "rttr/detail/base/core_prerequisites.h"

#include <vector>
#include <memory>
#include <cstddef>

namespace rttr
{
class type;
class variant;
class instance;
class argument;

namespace detail
{

class RTTR_API array_wrapper_base
{
    public:
        array_wrapper_base() {}
		virtual ~array_wrapper_base();;
		virtual bool        is_valid() const;
		virtual bool        is_dynamic() const;
		virtual std::size_t get_rank() const;
		virtual type        get_rank_type( std::size_t index ) const;
		virtual type        get_type() const;
		virtual bool        is_raw_array() const;
      
		virtual std::size_t get_size() const;
		virtual std::size_t get_size( std::size_t index_1 ) const;
		virtual std::size_t get_size( std::size_t index_1, std::size_t index_2 ) const;
		virtual std::size_t get_size_variadic( const std::vector<std::size_t>& index_list ) const;

		virtual bool set_size( std::size_t new_size );
		virtual bool set_size( std::size_t new_size, std::size_t index_1 );
		virtual bool set_size( std::size_t new_size, std::size_t index_1, std::size_t index_2 );
		virtual bool set_size_variadic( std::size_t new_size, const std::vector<std::size_t>& index_list );
        
		virtual bool set_value( argument& arg );
		virtual bool set_value( std::size_t index_1, argument& arg );
		virtual bool set_value( std::size_t index_1, std::size_t index_2, argument& arg );
		virtual bool set_value( std::size_t index_1, std::size_t index_2, std::size_t index_3, argument& arg );
		virtual bool set_value_variadic( const std::vector<std::size_t>& index_list, argument& arg );

		virtual variant get_value( std::size_t index_1 ) const;
		virtual variant get_value( std::size_t index_1, std::size_t index_2 ) const;
		virtual variant get_value( std::size_t index_1, std::size_t index_2, std::size_t index_3 ) const;
		virtual variant get_value_variadic( const std::vector<std::size_t>& index_list ) const;

		virtual bool insert_value( std::size_t index_1, argument& arg );
		virtual bool insert_value( std::size_t index_1, std::size_t index_2, argument& arg );
		virtual bool insert_value( std::size_t index_1, std::size_t index_2, std::size_t index_3, argument& arg );
		virtual bool insert_value_variadic( const std::vector<std::size_t>& index_list, argument& arg );

		virtual bool remove_value( std::size_t index_1 );
		virtual bool remove_value( std::size_t index_1, std::size_t index_2 );
		virtual bool remove_value( std::size_t index_1, std::size_t index_2, std::size_t index_3 );
		virtual bool remove_value_variadic( const std::vector<std::size_t>& index_list );

		virtual std::unique_ptr<array_wrapper_base> clone() const;
};

} // end namespace detail
} // end namespace rttr

#endif // RTTR_ARRAY_WRAPPER_BASE_H_
