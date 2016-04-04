//#include "variant_data_policy.h"
//
//namespace rttr
//{
//namespace detail
//{
//
//
//bool variant_data_policy_empty::invoke(variant_policy_operation op, const variant_data& src_data, argument_wrapper arg)
//{
//    switch (op)
//    {
//        case variant_policy_operation::DESTROY: 
//        case variant_policy_operation::CLONE:
//        case variant_policy_operation::SWAP:
//        {
//            break;
//        }
//        case variant_policy_operation::GET_VALUE:
//        {
//            arg.get_value<void*>() = nullptr;
//            break;
//        }
//        case variant_policy_operation::GET_TYPE:
//        {
//            arg.get_value<type>() = get_invalid_type();
//            break;
//        }
//        case variant_policy_operation::GET_PTR:
//        {
//            arg.get_value<void*>() = nullptr;
//            break;
//        }
//        case variant_policy_operation::GET_RAW_TYPE:
//        {
//            arg.get_value<type>() = get_invalid_type();
//            break;
//        }
//        case variant_policy_operation::GET_RAW_PTR:
//        {
//            arg.get_value<void*>() = nullptr;
//            break;
//        }
//        case variant_policy_operation::GET_ADDRESS_CONTAINER:
//        {
//            data_address_container& data        = arg.get_value<data_address_container>();
//
//            data.m_type                         = get_invalid_type();
//            data.m_wrapped_type                 = get_invalid_type();
//            data.m_data_address                 = nullptr;
//            data.m_data_address_wrapped_type    = nullptr;
//            break;
//        }
//        case variant_policy_operation::IS_ARRAY:
//        {
//            return false;
//        }
//        case variant_policy_operation::TO_ARRAY:
//        {
//            break;
//        }
//        case variant_policy_operation::IS_VALID:
//        {
//            return false;
//            break;
//        }
//        case variant_policy_operation::IS_NULLPTR:
//        {
//            return false;
//        }
//        case variant_policy_operation::CONVERT:
//        {
//            return false;
//        }
//        case variant_policy_operation::COMPARE_EQUAL:
//        {
//            const auto& param   = arg.get_value<std::tuple<const variant&, const variant&>>();
//            const variant& lhs  = std::get<0>(param);
//            const variant& rhs  = std::get<1>(param);
//            return !rhs.is_valid();
//        }
//        case variant_policy_operation::COMPARE_LESS:
//        {
//            return false;
//        }
//    }
//    return true;
//}
//
//
//}	//	detail
//}	//	rttr
