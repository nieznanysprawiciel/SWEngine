#include "array_wrapper_base.h"

#include "rttr/variant.h"

//====================================================================================//
//			This files contains definitions of virtual functions to prevent
//			from creating multiple definitions in libs
//====================================================================================//

inline rttr::detail::array_wrapper_base::~array_wrapper_base() { }

inline bool rttr::detail::array_wrapper_base::is_valid() const { return false; }

inline bool rttr::detail::array_wrapper_base::is_dynamic() const { return false; }

inline std::size_t rttr::detail::array_wrapper_base::get_rank() const { return 0; }

inline rttr::type rttr::detail::array_wrapper_base::get_rank_type( std::size_t index ) const { return get_invalid_type(); }

inline rttr::type rttr::detail::array_wrapper_base::get_type() const { return get_invalid_type(); }

inline bool rttr::detail::array_wrapper_base::is_raw_array() const { return false; }

inline std::size_t rttr::detail::array_wrapper_base::get_size() const { return 0; }

inline std::size_t rttr::detail::array_wrapper_base::get_size( std::size_t index_1 ) const { return 0; }

inline std::size_t rttr::detail::array_wrapper_base::get_size( std::size_t index_1, std::size_t index_2 ) const { return 0; }

inline std::size_t rttr::detail::array_wrapper_base::get_size_variadic( const std::vector<std::size_t>& index_list ) const { return 0; }

inline bool rttr::detail::array_wrapper_base::set_size( std::size_t new_size ) { return false; }

inline bool rttr::detail::array_wrapper_base::set_size( std::size_t new_size, std::size_t index_1 ) { return false; }

inline bool rttr::detail::array_wrapper_base::set_size( std::size_t new_size, std::size_t index_1, std::size_t index_2 ) { return false; }

inline bool rttr::detail::array_wrapper_base::set_size_variadic( std::size_t new_size, const std::vector<std::size_t>& index_list ) { return false; }

inline bool rttr::detail::array_wrapper_base::set_value( argument & arg ) { return false; }

inline bool rttr::detail::array_wrapper_base::set_value( std::size_t index_1, argument & arg ) { return false; }

inline bool rttr::detail::array_wrapper_base::set_value( std::size_t index_1, std::size_t index_2, argument & arg ) { return false; }

inline bool rttr::detail::array_wrapper_base::set_value( std::size_t index_1, std::size_t index_2, std::size_t index_3, argument & arg ) { return false; }

inline bool rttr::detail::array_wrapper_base::set_value_variadic( const std::vector<std::size_t>& index_list, argument & arg ) { return false; }

inline rttr::variant rttr::detail::array_wrapper_base::get_value( std::size_t index_1 ) const { return variant(); }

inline rttr::variant rttr::detail::array_wrapper_base::get_value( std::size_t index_1, std::size_t index_2 ) const { return variant(); }

inline rttr::variant rttr::detail::array_wrapper_base::get_value( std::size_t index_1, std::size_t index_2, std::size_t index_3 ) const { return variant(); }

inline rttr::variant rttr::detail::array_wrapper_base::get_value_variadic( const std::vector<std::size_t>& index_list ) const { return variant(); }

inline bool rttr::detail::array_wrapper_base::insert_value( std::size_t index_1, argument & arg ) { return false; }

inline bool rttr::detail::array_wrapper_base::insert_value( std::size_t index_1, std::size_t index_2, argument & arg ) { return false; }

inline bool rttr::detail::array_wrapper_base::insert_value( std::size_t index_1, std::size_t index_2, std::size_t index_3, argument & arg ) { return false; }

inline bool rttr::detail::array_wrapper_base::insert_value_variadic( const std::vector<std::size_t>& index_list, argument & arg ) { return false; }

inline bool rttr::detail::array_wrapper_base::remove_value( std::size_t index_1 ) { return false; }

inline bool rttr::detail::array_wrapper_base::remove_value( std::size_t index_1, std::size_t index_2 ) { return false; }

inline bool rttr::detail::array_wrapper_base::remove_value( std::size_t index_1, std::size_t index_2, std::size_t index_3 ) { return false; }

inline bool rttr::detail::array_wrapper_base::remove_value_variadic( const std::vector<std::size_t>& index_list ) { return false; }

inline std::unique_ptr<rttr::detail::array_wrapper_base> rttr::detail::array_wrapper_base::clone() const { return detail::make_unique<array_wrapper_base>(); }
