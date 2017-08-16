/*
 *  CATCH v1.0 build 35 (master branch)
 *  Generated: 2014-09-11 06:59:23.903000
 *  ----------------------------------------------------------
 *  This file has been merged from multiple headers. Please don't edit it directly
 *  Copyright (c) 2012 Two Blue Cubes Ltd. All rights reserved.
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef TWOBLUECUBES_SINGLE_INCLUDE_CATCH_HPP_INCLUDED
#define TWOBLUECUBES_SINGLE_INCLUDE_CATCH_HPP_INCLUDED

#define TWOBLUECUBES_CATCH_HPP_INCLUDED

#ifdef __clang__
#pragma clang diagnostic ignored "-Wglobal-constructors"
#pragma clang diagnostic ignored "-Wvariadic-macros"
#pragma clang diagnostic ignored "-Wc99-extensions"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"
#endif

#ifdef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_RUNNER
#endif

#ifdef CATCH_CONFIG_RUNNER
#  ifndef CLARA_CONFIG_MAIN
#    define CLARA_CONFIG_MAIN_NOT_DEFINED
#    define CLARA_CONFIG_MAIN
#  endif
#endif

#if (_MANAGED == 1) || (_M_CEE == 1) // detect CLR
    #define INTERNAL_CATCH_VS_MANAGED
#else

#if defined(_WINDLL)
    // _WINDLL seems to be the only thing we can check for the existence of a native DLL.
    // It's possible that this is not enough for someone so allow it to be overridden...
    #if !defined( CATCH_CONFIG_MAIN ) && !defined( CATCH_CONFIG_RUNNER )
    #define INTERNAL_CATCH_VS_NATIVE
    #endif
#endif

#endif // detect CLR

#if defined(INTERNAL_CATCH_VS_MANAGED) || defined(INTERNAL_CATCH_VS_NATIVE)
    #define INTERNAL_CATCH_INLINE inline
    #define CATCH_CONFIG_RUNNER (1)
#else
    #define INTERNAL_CATCH_INLINE
#endif

// #included from: internal/catch_notimplemented_exception.h
#define TWOBLUECUBES_CATCH_NOTIMPLEMENTED_EXCEPTION_H_INCLUDED

// #included from: catch_common.h
#define TWOBLUECUBES_CATCH_COMMON_H_INCLUDED

#define INTERNAL_CATCH_UNIQUE_NAME_LINE2( name, line ) name##line
#define INTERNAL_CATCH_UNIQUE_NAME_LINE( name, line ) INTERNAL_CATCH_UNIQUE_NAME_LINE2( name, line )
#define INTERNAL_CATCH_UNIQUE_NAME( name ) INTERNAL_CATCH_UNIQUE_NAME_LINE( name, __LINE__ )

#define INTERNAL_CATCH_STRINGIFY2( expr ) #expr
#define INTERNAL_CATCH_STRINGIFY( expr ) INTERNAL_CATCH_STRINGIFY2( expr )

#include <sstream>
#include <stdexcept>
#include <algorithm>

// #included from: catch_compiler_capabilities.h
#define TWOBLUECUBES_CATCH_COMPILER_CAPABILITIES_HPP_INCLUDED

// Much of the following code is based on Boost (1.53)

#ifdef __clang__

#if __has_feature(cxx_nullptr)

#define CATCH_CONFIG_CPP11_NULLPTR

#endif

#endif // __clang__

////////////////////////////////////////////////////////////////////////////////
// Borland
#ifdef __BORLANDC__

#if (__BORLANDC__ > 0x582 )
//#define CATCH_CONFIG_SFINAE // Not confirmed
#endif

#endif // __BORLANDC__

////////////////////////////////////////////////////////////////////////////////
// EDG
#ifdef __EDG_VERSION__

#if (__EDG_VERSION__ > 238 )
//#define CATCH_CONFIG_SFINAE // Not confirmed
#endif

#endif // __EDG_VERSION__

////////////////////////////////////////////////////////////////////////////////
// Digital Mars
#ifdef __DMC__

#if (__DMC__ > 0x840 )
//#define CATCH_CONFIG_SFINAE // Not confirmed
#endif

#endif // __DMC__

////////////////////////////////////////////////////////////////////////////////
// GCC
#ifdef __GNUC__

#if __GNUC__ < 3

#if (__GNUC_MINOR__ >= 96 )
//#define CATCH_CONFIG_SFINAE
#endif

#elif __GNUC__ >= 3

// #define CATCH_CONFIG_SFINAE // Taking this out completely for now

#endif // __GNUC__ < 3

#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6 && defined(__GXX_EXPERIMENTAL_CXX0X__) )

#define CATCH_CONFIG_CPP11_NULLPTR
#endif

#endif // __GNUC__

////////////////////////////////////////////////////////////////////////////////
// Visual C++
#ifdef _MSC_VER

#if (_MSC_VER >= 1310 ) // (VC++ 7.0+)
//#define CATCH_CONFIG_SFINAE // Not confirmed
#endif

#endif // _MSC_VER

// Use variadic macros if the compiler supports them
#if ( defined _MSC_VER && _MSC_VER > 1400 && !defined __EDGE__) || \
    ( defined __WAVE__ && __WAVE_HAS_VARIADICS ) || \
    ( defined __GNUC__ && __GNUC__ >= 3 ) || \
    ( !defined __cplusplus && __STDC_VERSION__ >= 199901L || __cplusplus >= 201103L )

#ifndef CATCH_CONFIG_NO_VARIADIC_MACROS
#define CATCH_CONFIG_VARIADIC_MACROS
#endif

#endif

namespace Catch {

    class NonCopyable {
        NonCopyable( NonCopyable const& );
        void operator = ( NonCopyable const& );
    protected:
        NonCopyable() {}
        virtual ~NonCopyable();
    };

    class SafeBool {
    public:
        typedef void (SafeBool::*type)() const;

        static type makeSafe( bool value ) {
            return value ? &SafeBool::trueValue : 0;
        }
    private:
        void trueValue() const {}
    };

    template<typename ContainerT>
    inline void deleteAll( ContainerT& container ) {
        typename ContainerT::const_iterator it = container.begin();
        typename ContainerT::const_iterator itEnd = container.end();
        for(; it != itEnd; ++it )
            delete *it;
    }
    template<typename AssociativeContainerT>
    inline void deleteAllValues( AssociativeContainerT& container ) {
        typename AssociativeContainerT::const_iterator it = container.begin();
        typename AssociativeContainerT::const_iterator itEnd = container.end();
        for(; it != itEnd; ++it )
            delete it->second;
    }

    bool startsWith( std::string const& s, std::string const& prefix );
    bool endsWith( std::string const& s, std::string const& suffix );
    bool contains( std::string const& s, std::string const& infix );
    void toLowerInPlace( std::string& s );
    std::string toLower( std::string const& s );
    std::string trim( std::string const& str );

    struct pluralise {
        pluralise( std::size_t count, std::string const& label );

        friend std::ostream& operator << ( std::ostream& os, pluralise const& pluraliser );

        std::size_t m_count;
        std::string m_label;
    };

    struct SourceLineInfo {

        SourceLineInfo();
        SourceLineInfo( char const* _file, std::size_t _line );
        SourceLineInfo( SourceLineInfo const& other );
        bool empty() const;
        bool operator == ( SourceLineInfo const& other ) const;

        std::string file;
        std::size_t line;
    };

    std::ostream& operator << ( std::ostream& os, SourceLineInfo const& info );

    // This is just here to avoid compiler warnings with macro constants and boolean literals
    inline bool isTrue( bool value ){ return value; }

    void throwLogicError( std::string const& message, SourceLineInfo const& locationInfo );

    // Use this in variadic streaming macros to allow
    //    >> +StreamEndStop
    // as well as
    //    >> stuff +StreamEndStop
    struct StreamEndStop {
        std::string operator+() {
            return std::string();
        }
    };
    template<typename T>
    T const& operator + ( T const& value, StreamEndStop ) {
        return value;
    }
}

#define CATCH_INTERNAL_LINEINFO ::Catch::SourceLineInfo( __FILE__, static_cast<std::size_t>( __LINE__ ) )
#define CATCH_INTERNAL_ERROR( msg ) ::Catch::throwLogicError( msg, CATCH_INTERNAL_LINEINFO );

#include <ostream>

namespace Catch {

    class NotImplementedException : public std::exception
    {
    public:
        NotImplementedException( SourceLineInfo const& lineInfo );

        virtual ~NotImplementedException() throw() {}

        virtual const char* what() const throw();

    private:
        std::string m_what;
        SourceLineInfo m_lineInfo;
    };

} // end namespace Catch

///////////////////////////////////////////////////////////////////////////////
#define CATCH_NOT_IMPLEMENTED throw Catch::NotImplementedException( CATCH_INTERNAL_LINEINFO )

// #included from: internal/catch_context.h
#define TWOBLUECUBES_CATCH_CONTEXT_H_INCLUDED

// #included from: catch_interfaces_generators.h
#define TWOBLUECUBES_CATCH_INTERFACES_GENERATORS_H_INCLUDED

#include <string>

namespace Catch {

    struct IGeneratorInfo {
        virtual ~IGeneratorInfo();
        virtual bool moveNext() = 0;
        virtual std::size_t getCurrentIndex() const = 0;
    };

    struct IGeneratorsForTest {
        virtual ~IGeneratorsForTest();

        virtual IGeneratorInfo& getGeneratorInfo( std::string const& fileInfo, std::size_t size ) = 0;
        virtual bool moveNext() = 0;
    };

    IGeneratorsForTest* createGeneratorsForTest();

} // end namespace Catch

// #included from: catch_ptr.hpp
#define TWOBLUECUBES_CATCH_PTR_HPP_INCLUDED

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"
#endif

namespace Catch {

    // An intrusive reference counting smart pointer.
    // T must implement addRef() and release() methods
    // typically implementing the IShared interface
    template<typename T>
    class Ptr {
    public:
        Ptr() : m_p( NULL ){}
        Ptr( T* p ) : m_p( p ){
            if( m_p )
                m_p->addRef();
        }
        Ptr( Ptr const& other ) : m_p( other.m_p ){
            if( m_p )
                m_p->addRef();
        }
        ~Ptr(){
            if( m_p )
                m_p->release();
        }
        void reset() {
            if( m_p )
                m_p->release();
            m_p = NULL;
        }
        Ptr& operator = ( T* p ){
            Ptr temp( p );
            swap( temp );
            return *this;
        }
        Ptr& operator = ( Ptr const& other ){
            Ptr temp( other );
            swap( temp );
            return *this;
        }
        void swap( Ptr& other ) { std::swap( m_p, other.m_p ); }
        T* get() { return m_p; }
        const T* get() const{ return m_p; }
        T& operator*() const { return *m_p; }
        T* operator->() const { return m_p; }
        bool operator !() const { return m_p == NULL; }
        operator SafeBool::type() const { return SafeBool::makeSafe( m_p != NULL ); }

    private:
        T* m_p;
    };

    struct IShared : NonCopyable {
        virtual ~IShared();
        virtual void addRef() const = 0;
        virtual void release() const = 0;
    };

    template<typename T = IShared>
    struct SharedImpl : T {

        SharedImpl() : m_rc( 0 ){}

        virtual void addRef() const {
            ++m_rc;
        }
        virtual void release() const {
            if( --m_rc == 0 )
                delete this;
        }

        mutable unsigned int m_rc;
    };

} // end namespace Catch

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#include <memory>
#include <vector>
#include <stdlib.h>

namespace Catch {

    class TestCase;
    class Stream;
    struct IResultCapture;
    struct IRunner;
    struct IGeneratorsForTest;
    struct IConfig;

    struct IContext
    {
        virtual ~IContext();

        virtual IResultCapture& getResultCapture() = 0;
        virtual IRunner& getRunner() = 0;
        virtual size_t getGeneratorIndex( std::string const& fileInfo, size_t totalSize ) = 0;
        virtual bool advanceGeneratorsForCurrentTest() = 0;
        virtual Ptr<IConfig const> getConfig() const = 0;
    };

    struct IMutableContext : IContext
    {
        virtual ~IMutableContext();
        virtual void setResultCapture( IResultCapture* resultCapture ) = 0;
        virtual void setRunner( IRunner* runner ) = 0;
        virtual void setConfig( Ptr<IConfig const> const& config ) = 0;
    };

    IContext& getCurrentContext();
    IMutableContext& getCurrentMutableContext();
    void cleanUpContext();
    Stream createStream( std::string const& streamName );

}

// #included from: internal/catch_test_registry.hpp
#define TWOBLUECUBES_CATCH_TEST_REGISTRY_HPP_INCLUDED

// #included from: catch_interfaces_testcase.h
#define TWOBLUECUBES_CATCH_INTERFACES_TESTCASE_H_INCLUDED

#include <vector>

namespace Catch {

    class TestCaseFilters;

    struct ITestCase : IShared {
        virtual void invoke () const = 0;
    protected:
        virtual ~ITestCase();
    };

    class TestCase;

    struct ITestCaseRegistry {
        virtual ~ITestCaseRegistry();
        virtual std::vector<TestCase> const& getAllTests() const = 0;
        virtual std::vector<TestCase> getMatchingTestCases( std::string const& rawTestSpec ) const = 0;
    };
}

#if defined(INTERNAL_CATCH_VS_MANAGED) || defined(INTERNAL_CATCH_VS_NATIVE)
// #included from: catch_vs_test_registry.hpp
#define TWOBLUECUBES_CATCH_MSTEST_REGISTRY_HPP_INCLUDED

#ifdef INTERNAL_CATCH_VS_MANAGED

#include <windows.h>
using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;
using namespace Microsoft::VisualStudio::TestTools::UnitTesting;

namespace Catch {
    inline String^ convert_string_for_assert(const std::string& s)
    {
        String^ result = gcnew String(s.c_str());
        return result;
    }

}

#endif

#ifdef INTERNAL_CATCH_VS_NATIVE

#pragma warning( disable:4505 )   // required for including CppUnitTest.h at /W4

#include <CppUnitTest.h>

using Microsoft::VisualStudio::CppUnitTestFramework::Logger;
using Microsoft::VisualStudio::CppUnitTestFramework::Assert;
using Microsoft::VisualStudio::CppUnitTestFramework::__LineInfo;

#endif

#include <tchar.h>
#ifdef INTERNAL_CATCH_VS_NATIVE
#include <cvt/wstring>
#include <codecvt>
#endif

namespace Catch {

class MethodTestCase : public SharedImpl<ITestCase> {

    struct placeholder
    {
        virtual ~placeholder() {}
        virtual placeholder* clone() const = 0;
        virtual void invoke() const = 0;
    };

    template <typename C>
    struct holder : public placeholder
    {
        holder( void (C::*method)() ) : m_method( method ) {}
        virtual placeholder* clone() const {return new holder(*this);}
        void invoke() const {
            C obj;
            (obj.*m_method)();
        }
        void (C::*m_method)();
    };

    virtual void invoke() const
    {
        if( held ) held->invoke();
    }
public:
    template<typename C>
    MethodTestCase( void (C::*method)() ) : held(new holder<C>(method) ) {}
    ~MethodTestCase() { delete held;}

private:
    MethodTestCase();                       // not implemented
    MethodTestCase(const MethodTestCase&);  // not implemented
    MethodTestCase& operator=(const MethodTestCase&);  // not implemented

    placeholder* held;
};

typedef void(*TestFunction)();

struct NameAndDesc {
#ifdef INTERNAL_CATCH_VS_MANAGED
    NameAndDesc( const char* _name = "", const char* _description= "" )
    : name( _name ), description( _description )
    {}
    NameAndDesc( const char* _name, int )
    : name( _name ), description( "" )
    {}
#else
    NameAndDesc( const wchar_t* _name, const char* _description= ""  )
    : name(), description( _description )
    {
        assignName(_name);
    }
    NameAndDesc( const wchar_t* _name, int  )
    : name(), description( "" )
    {
        assignName(_name);
    }
    void assignName(const wchar_t* _name)
    {
        stdext::cvt::wstring_convert<std::codecvt_utf8<wchar_t> > conv1;
        std::string tmp = conv1.to_bytes(_name);
        if( tmp.empty() )
        {
            name = tmp;
        }
        else
        {
            std::string::iterator startIter = tmp.begin();
            if(*startIter == '\"')
            {
                ++startIter;
            }
            std::string::reverse_iterator endIter = tmp.rbegin();
            if(*endIter == '\"')
            {
                ++endIter;
            }
            name.assign(startIter, endIter.base());
        }
    }
#endif

    std::string name;
    std::string description;
};

struct AutoReg {

    AutoReg(    TestFunction function,
                SourceLineInfo const& lineInfo,
                NameAndDesc const& nameAndDesc );

    template<typename C>
    AutoReg(    void (C::*method)(),
                char const* className,
                NameAndDesc const& nameAndDesc,
                SourceLineInfo const& lineInfo ) {
        registerTestCase(   new MethodTestCase( method ),
                            className,
                            nameAndDesc,
                            lineInfo );
    }

    void registerTestCase(  ITestCase* testCase,
                            char const* className,
                            NameAndDesc const& nameAndDesc,
                            SourceLineInfo const& lineInfo );

    ~AutoReg();

private:
    AutoReg( AutoReg const& );
    void operator= ( AutoReg const& );
};

} // end namespace Catch

#ifdef INTERNAL_CATCH_VS_MANAGED

#define CATCH_INTERNAL_HANDLE_EMPTY_PARAM2( name ) name##""
#define CATCH_INTERNAL_HANDLE_EMPTY_PARAM(...) CATCH_INTERNAL_HANDLE_EMPTY_PARAM2( INTERNAL_CATCH_SPLIT_ARGS_2(__VA_ARGS__) )

#define INTERNAL_CATCH_CLASS_DEFINITION( cls ) \
            [TestClass] \
            public ref class cls

#define INTERNAL_CATCH_CLASS_CONTEXT \
        private: \
	        TestContext^ testContextInstance; \
        public: \
	        property Microsoft::VisualStudio::TestTools::UnitTesting::TestContext^ TestContext \
	        { \
		        Microsoft::VisualStudio::TestTools::UnitTesting::TestContext^ get() \
		        { \
			        return testContextInstance; \
		        } \
		        System::Void set(Microsoft::VisualStudio::TestTools::UnitTesting::TestContext^ value) \
		        { \
			        testContextInstance = value; \
		        } \
	        };

#define CATCH_INTERNAL_NAMESPACE( Ext )

#define INTERNAL_CATCH_TEST_METHOD( Count, UniqueExt, Name, Desc ) \
        public: \
            [TestMethod] \
            [Description( CATCH_INTERNAL_HANDLE_EMPTY_PARAM(Name) )] \
            [TestProperty( "Description", CATCH_INTERNAL_HANDLE_EMPTY_PARAM(Name) )] \
            void INTERNAL_CATCH_UNIQUE_NAME_LINE( C_A_T_C_H___M_E_T_H_O_D___, UniqueExt) () \
            { \
                Catch::NameAndDesc name_desc( CATCH_INTERNAL_HANDLE_EMPTY_PARAM(Name), Desc ); \
                CATCH_INTERNAL_RUN_SINGLE_TEST(Count); \
            }

#define BEGIN_INTERNAL_CATCH_BATCH_METHOD( Tags, UniqueExt ) \
        public: \
            [TestMethod] \
            [TestCategory( CATCH_INTERNAL_HANDLE_EMPTY_PARAM(Tags) )] \
            [Description( CATCH_INTERNAL_HANDLE_EMPTY_PARAM(Tags) )] \
            [TestProperty( "Description", CATCH_INTERNAL_HANDLE_EMPTY_PARAM(Tags) )] \
            void INTERNAL_CATCH_UNIQUE_NAME_LINE( C_A_T_C_H___M_E_T_H_O_D___, UniqueExt) ()

#define CHECK_FOR_TEST_CASE_CLASH

#define INTERNAL_CATCH_MAP_CATEGORY_TO_TAG( Category, Tag ) \
    INTERNAL_CATCH_MAP_CATEGORY_TO_TAG2( #Category, Tag, __COUNTER__ )

#define INTERNAL_CATCH_MAP_CATEGORY_TO_LIST( Category ) \
    INTERNAL_CATCH_MAP_CATEGORY_TO_LIST2( #Category, #Category, __COUNTER__ )

#define FAIL_STRING( str ) _T( str )

#else // detect CLR

// Native tests

#define INTERNAL_CATCH_CLASS_DEFINITION( cls ) \
            TEST_CLASS( cls )

#define INTERNAL_CATCH_CLASS_CONTEXT

#define CATCH_INTERNAL_NAMESPACE( Ext ) INTERNAL_CATCH_UNIQUE_NAME_LINE( C_A_T_C_H___N_S_, Ext )

#define TEST2( ... ) TEST_IMPL_2( (__VA_ARGS__, 2, 1) )
#define TEST_IMPL_2(tuple) TEST_IMPL2 tuple
#define TEST_IMPL2( INTERNAL_CATCH_SPLIT_ARG_1,INTERNAL_CATCH_SPLIT_ARG_2,N,...) L#INTERNAL_CATCH_SPLIT_ARG_1

#define CATCH_INTERNAL_HANDLE_EMPTY_PARAM(...) CATCH_INTERNAL_HANDLE_EMPTY_PARAM_IMPLW( (__VA_ARGS__, 2, 1) )
#define CATCH_INTERNAL_HANDLE_EMPTY_PARAM_IMPLW(tuple) CATCH_INTERNAL_HANDLE_EMPTY_PARAM_IMPL2W tuple
#define CATCH_INTERNAL_HANDLE_EMPTY_PARAM_IMPL2W( INTERNAL_CATCH_SPLIT_ARG_1,INTERNAL_CATCH_SPLIT_ARG_2,N,...) L#INTERNAL_CATCH_SPLIT_ARG_1

#define INTERNAL_CATCH_TEST_METHOD( Count, UniqueExt, Name, Desc ) \
        public: \
	    BEGIN_TEST_METHOD_ATTRIBUTE( INTERNAL_CATCH_UNIQUE_NAME_LINE( C_A_T_C_H___M_E_T_H_O_D___, UniqueExt) ) \
            TEST_OWNER( CATCH_INTERNAL_HANDLE_EMPTY_PARAM(Name) ) \
            TEST_DESCRIPTION( CATCH_INTERNAL_HANDLE_EMPTY_PARAM(Name) ) \
	    END_TEST_METHOD_ATTRIBUTE() \
    	TEST_METHOD( INTERNAL_CATCH_UNIQUE_NAME_LINE( C_A_T_C_H___M_E_T_H_O_D___, UniqueExt) ) \
        { \
            Catch::NameAndDesc name_desc(CATCH_INTERNAL_HANDLE_EMPTY_PARAM(Name), Desc ); \
            CATCH_INTERNAL_RUN_SINGLE_TEST(Count); \
        }

#define BEGIN_INTERNAL_CATCH_BATCH_METHOD( Tags, UniqueExt ) \
        public: \
	        BEGIN_TEST_METHOD_ATTRIBUTE( INTERNAL_CATCH_UNIQUE_NAME_LINE( C_A_T_C_H___M_E_T_H_O_D___, UniqueExt) ) \
                TEST_OWNER( CATCH_INTERNAL_HANDLE_EMPTY_PARAM(Tags) ) \
                TEST_DESCRIPTION( CATCH_INTERNAL_HANDLE_EMPTY_PARAM(Tags) ) \
	        END_TEST_METHOD_ATTRIBUTE() \
            TEST_METHOD( INTERNAL_CATCH_UNIQUE_NAME_LINE( C_A_T_C_H___M_E_T_H_O_D___, UniqueExt) )

#define CHECK_FOR_TEST_CASE_CLASH void INTERNAL_CATCH_UNIQUE_NAME_LINE( if_you_get_this_error_you_have_a_test_case_name_clash_please_put_a_namespace_around_the_test_case_at_line_, __LINE__ )() {}

#define INTERNAL_CATCH_MAP_CATEGORY_TO_TAG( Category, Tag ) \
    INTERNAL_CATCH_MAP_CATEGORY_TO_TAG2( Category, Tag, __COUNTER__ )

#define INTERNAL_CATCH_MAP_CATEGORY_TO_LIST( Category ) \
    INTERNAL_CATCH_MAP_CATEGORY_TO_LIST2( Category, #Category, __COUNTER__ )

#define FAIL_STRING( str ) WIDEN( str )

#endif // detect CLR

#define INTERNAL_CATCH_CONCAT_LINE_COUNTER( count ) INTERNAL_CATCH_UNIQUE_NAME_LINE( INTERNAL_CATCH_UNIQUE_NAME_LINE( __LINE__, _ ), count )

#define CATCH_INTERNAL_CONFIG_SHOW_SUCCESS2( v, Count ) \
    namespace { CatchOverrides::ConfigShowSuccessfulTests<Catch::IConfig const*> INTERNAL_CATCH_UNIQUE_NAME_LINE( C_A_T_C_H_____O_V_E_R_R_I_D_E____, INTERNAL_CATCH_CONCAT_LINE_COUNTER( Count ) )(__FILE__, __LINE__, v); }

#define CATCH_INTERNAL_CONFIG_WARN_MISSING_ASSERTIONS2( v, Count ) \
    namespace { CatchOverrides::ConfigWarnMissingAssertions<Catch::IConfig const*> INTERNAL_CATCH_UNIQUE_NAME_LINE( C_A_T_C_H_____O_V_E_R_R_I_D_E____, INTERNAL_CATCH_CONCAT_LINE_COUNTER( Count ) )(__FILE__, __LINE__, v); }

#define CATCH_INTERNAL_CONFIG_ABORT_AFTER2( v, Count ) \
    namespace { CatchOverrides::ConfigAbortAfter<Catch::IConfig const*> INTERNAL_CATCH_UNIQUE_NAME_LINE( C_A_T_C_H_____O_V_E_R_R_I_D_E____, INTERNAL_CATCH_CONCAT_LINE_COUNTER( Count ) )(__FILE__, __LINE__, v); }

#define CATCH_INTERNAL_CONFIG_ADD_TEST2( v, Count ) \
    namespace { CatchOverrides::ConfigAddTest<Catch::IConfig const*> INTERNAL_CATCH_UNIQUE_NAME_LINE( C_A_T_C_H_____O_V_E_R_R_I_D_E____, INTERNAL_CATCH_CONCAT_LINE_COUNTER( Count ) )(__FILE__, __LINE__, v); }

#define CATCH_INTERNAL_CONFIG_SHOW_SUCCESS( v ) \
    CATCH_INTERNAL_CONFIG_SHOW_SUCCESS2( v, __COUNTER__)

#define CATCH_INTERNAL_CONFIG_WARN_MISSING_ASSERTIONS( v ) \
    CATCH_INTERNAL_CONFIG_WARN_MISSING_ASSERTIONS2( v, __COUNTER__)

#define CATCH_INTERNAL_CONFIG_ABORT_AFTER( v ) \
    CATCH_INTERNAL_CONFIG_ABORT_AFTER2( v, __COUNTER__)

#define CATCH_INTERNAL_CONFIG_ADD_TEST( v ) \
    CATCH_INTERNAL_CONFIG_ADD_TEST2( v, __COUNTER__)

struct CollectConsoleOutput
{
    CollectConsoleOutput()
#if defined(INTERNAL_CATCH_VS_NATIVE) || _MSC_VER >= 1700
            : m_addLineFeeds(false)
#else
            : m_addLineFeeds(true)
#endif
    {}
    CollectConsoleOutput( bool v)
            : m_addLineFeeds(v)
    {}
    ~CollectConsoleOutput()
    {
        std::string output = oss.str();
        if( !output.empty() ) {
            if( m_addLineFeeds ) {
                std::string revised;
                replaceSingleLinefeed(output, revised);
                write_output_message(revised);
            }
            else {
                write_output_message(output);
            }
        }
    }
#if defined(INTERNAL_CATCH_VS_MANAGED)

    void write_output_message(const std::string& msg)
    {
        String^ tmp = gcnew String(msg.c_str());
        Console::WriteLine(tmp);
    }

#else // detect CLR

#if defined(INTERNAL_CATCH_VS_NATIVE)

#ifdef _UNICODE
    void write_output_message(const std::string& msg)
    {
        std::wstringstream _s;
        _s << msg.c_str();
        std::wstring ws = _s.str();
        Logger::WriteMessage(ws.c_str());
    }
#else
    void write_output_message(const std::string& msg)
    {
        Logger::WriteMessage(msg.c_str());
    }
#endif

#endif // _WINDLL

#endif  // detect CLR
    void replaceSingleLinefeed(const std::string& s, std::string& result)
    {
        bool needr(false);
        for(std::string::const_iterator it = s.begin(); it != s.end(); ++it ) {
            if( *it == '\r' ) {
                needr = false;
            }
            else if( *it == '\n' && needr ) {
                needr = false;
                result += '\r';
                result += *it;
            }
            else {
                needr = true;
            }
            result += *it;
        }
    }

    std::streambuf* rdbuf() {return oss.rdbuf(); }

    std::ostringstream oss;
    bool m_addLineFeeds;
};

#define CATCH_INTERNAL_RUN_SINGLE_TEST( Count ) \
        {   CatchOverrides::ConfigGuard cg; \
            Catch::ConfigData cd(cg.value().get()); \
            cd.name = name_desc.name; \
            cd.showSuccessfulTests = CatchOverrides::Config<Catch::IConfig const*>::instance().includeSuccessfulResults(__FILE__, __LINE__ ); \
            cd.warnings            = (CatchOverrides::Config<Catch::IConfig const*>::instance().warnAboutMissingAssertions(__FILE__, __LINE__ ) ? Catch::WarnAbout::NoAssertions : Catch::WarnAbout::Nothing); \
            cd.abortAfter          = CatchOverrides::Config<Catch::IConfig const*>::instance().abortAfter(__FILE__, __LINE__ ); \
            CollectConsoleOutput ptr; { \
                Catch::Ptr<Catch::Config> config(new Catch::Config(cd)); \
                config->setStreamBuf(ptr.rdbuf()); \
                Catch::RunContext context(config.get(), Catch::getRegistryHub().getReporterRegistry().create( "console", config.get())); \
                std::vector<Catch::TestCase> testCase = Catch::getRegistryHub().getTestCaseRegistry().getMatchingTestCases(name_desc.name); \
                if( testCase.empty() ) Assert::Fail(FAIL_STRING("No tests match")); \
                if( testCase.size() > 1 ) Assert::Fail(FAIL_STRING("More than one test with the same name")); \
                context.testGroupStarting( "", 0, 1 ); \
                Catch::Totals totals = context.runTest(*testCase.begin()); \
                context.testGroupEnded( "", totals, 0, 1 ); \
                if( totals.assertions.failed > 0 ) { \
                    INTERNAL_CATCH_TEST_THROW_FAILURE \
                } \
            } \
        }

#define INTERNAL_CATCH_TESTCASE2( Count, Name, Desc ) \
    CHECK_FOR_TEST_CASE_CLASH \
    static void INTERNAL_CATCH_UNIQUE_NAME_LINE( C_A_T_C_H____T_E_S_T____, INTERNAL_CATCH_CONCAT_LINE_COUNTER( Count ) )(); \
    namespace CATCH_INTERNAL_NAMESPACE( INTERNAL_CATCH_CONCAT_LINE_COUNTER( Count ) ) { \
        Catch::AutoReg INTERNAL_CATCH_UNIQUE_NAME( autoRegistrar )( & INTERNAL_CATCH_UNIQUE_NAME_LINE( C_A_T_C_H____T_E_S_T____, INTERNAL_CATCH_CONCAT_LINE_COUNTER( Count ) ), CATCH_INTERNAL_LINEINFO, Catch::NameAndDesc(CATCH_INTERNAL_HANDLE_EMPTY_PARAM(Name), Desc) ); \
        CatchOverrides::ConfigReset<Catch::IConfig const*> INTERNAL_CATCH_UNIQUE_NAME_LINE( C_A_T_C_H____T_E_S_T____C_O_N_F_I_G___, INTERNAL_CATCH_CONCAT_LINE_COUNTER( Count ) )(__FILE__, __LINE__, 1); \
        INTERNAL_CATCH_CLASS_DEFINITION( INTERNAL_CATCH_UNIQUE_NAME_LINE( C_A_T_C_H____T_E_S_T____C_L_A_S_S___, INTERNAL_CATCH_CONCAT_LINE_COUNTER( Count ) ) ) \
        { \
            INTERNAL_CATCH_CLASS_CONTEXT \
            INTERNAL_CATCH_TEST_METHOD( Count, INTERNAL_CATCH_CONCAT_LINE_COUNTER( Count ), Name, Desc ) \
        }; \
    } \
    void INTERNAL_CATCH_UNIQUE_NAME_LINE( C_A_T_C_H____T_E_S_T____, INTERNAL_CATCH_CONCAT_LINE_COUNTER( Count ) )()

#define INTERNAL_CATCH_METHOD_AS_TEST_CASE2( QualifiedMethod, Count, Name, Desc ) \
    CHECK_FOR_TEST_CASE_CLASH \
    namespace CATCH_INTERNAL_NAMESPACE( INTERNAL_CATCH_CONCAT_LINE_COUNTER( Count ) ) { \
        Catch::AutoReg INTERNAL_CATCH_UNIQUE_NAME( autoRegistrar )( & QualifiedMethod, "&" # QualifiedMethod, Catch::NameAndDesc(CATCH_INTERNAL_HANDLE_EMPTY_PARAM(Name), Desc), CATCH_INTERNAL_LINEINFO ); \
        CatchOverrides::ConfigReset<Catch::IConfig const*> INTERNAL_CATCH_UNIQUE_NAME_LINE( C_A_T_C_H____T_E_S_T____C_O_N_F_I_G___, INTERNAL_CATCH_CONCAT_LINE_COUNTER( Count ) )(__FILE__, __LINE__, 1); \
        INTERNAL_CATCH_CLASS_DEFINITION( INTERNAL_CATCH_UNIQUE_NAME_LINE( C_A_T_C_H____T_E_S_T____C_L_A_S_S___, INTERNAL_CATCH_CONCAT_LINE_COUNTER( Count ) ) ) \
        { \
            INTERNAL_CATCH_CLASS_CONTEXT \
            INTERNAL_CATCH_TEST_METHOD( Count, INTERNAL_CATCH_CONCAT_LINE_COUNTER( Count ), Name, Desc ) \
        }; \
    };

#define INTERNAL_CATCH_TEST_CASE_METHOD2( ClassName, Count, TestName, Desc ) \
    CHECK_FOR_TEST_CASE_CLASH \
    struct INTERNAL_CATCH_UNIQUE_NAME_LINE( C_A_T_C_H____T_E_S_T____, INTERNAL_CATCH_CONCAT_LINE_COUNTER( Count ) ) : ClassName { \
        void test(); \
        static void invoke() { INTERNAL_CATCH_UNIQUE_NAME_LINE( C_A_T_C_H____T_E_S_T____, INTERNAL_CATCH_CONCAT_LINE_COUNTER( Count ) ) tmp; tmp.test(); } \
    }; \
    namespace CATCH_INTERNAL_NAMESPACE( INTERNAL_CATCH_CONCAT_LINE_COUNTER( Count ) ) { \
        Catch::AutoReg INTERNAL_CATCH_UNIQUE_NAME( autoRegistrar )( & INTERNAL_CATCH_UNIQUE_NAME_LINE( C_A_T_C_H____T_E_S_T____, INTERNAL_CATCH_CONCAT_LINE_COUNTER( Count ) )::invoke, CATCH_INTERNAL_LINEINFO, Catch::NameAndDesc(CATCH_INTERNAL_HANDLE_EMPTY_PARAM(TestName), Desc) ); \
        CatchOverrides::ConfigReset<Catch::IConfig const*> INTERNAL_CATCH_UNIQUE_NAME_LINE( C_A_T_C_H____T_E_S_T____C_O_N_F_I_G___, INTERNAL_CATCH_CONCAT_LINE_COUNTER( Count ) )(__FILE__, __LINE__, 1); \
        INTERNAL_CATCH_CLASS_DEFINITION( INTERNAL_CATCH_UNIQUE_NAME_LINE( C_A_T_C_H____T_E_S_T____C_L_A_S_S___, INTERNAL_CATCH_CONCAT_LINE_COUNTER( Count ) ) ) \
        { \
            INTERNAL_CATCH_CLASS_CONTEXT \
            INTERNAL_CATCH_TEST_METHOD( Count, INTERNAL_CATCH_CONCAT_LINE_COUNTER( Count ), TestName, Desc ) \
        }; \
    } \
    void INTERNAL_CATCH_UNIQUE_NAME_LINE( C_A_T_C_H____T_E_S_T____, INTERNAL_CATCH_CONCAT_LINE_COUNTER( Count ) )::test()

#if defined(INTERNAL_CATCH_VS_MANAGED)

    #define INTERNAL_CATCH_TEST_REPORT_BATCH_FAILURE( count ) \
    { \
        std::stringstream _sf; \
        _sf << count << " assertions failed - check output for results."; \
        std::string fail = _sf.str(); \
        Assert::Fail(Catch::convert_string_for_assert(fail)); \
    }

#else

    #define INTERNAL_CATCH_TEST_REPORT_BATCH_FAILURE( count ) \
    { \
        std::wstringstream _s; \
        _s << count << " assertions failed - check output for results."; \
        std::wstring ws = _s.str(); \
        Assert::Fail(ws.c_str()); \
    }
#endif

#define INTERNAL_CATCH_MAP_CATEGORY_TO_TAG2( Category, Tag, Count ) \
    CHECK_FOR_TEST_CASE_CLASH \
    namespace CATCH_INTERNAL_NAMESPACE( INTERNAL_CATCH_CONCAT_LINE_COUNTER( Count ) ) { \
        CatchOverrides::ConfigReset<Catch::IConfig const*> INTERNAL_CATCH_UNIQUE_NAME_LINE( C_A_T_C_H____T_E_S_T____C_O_N_F_I_G___, INTERNAL_CATCH_CONCAT_LINE_COUNTER( Count ) )(__FILE__, __LINE__, -1); \
        INTERNAL_CATCH_CLASS_DEFINITION( INTERNAL_CATCH_UNIQUE_NAME_LINE( C_A_T_C_H____T_E_S_T____C_L_A_S_S___, INTERNAL_CATCH_CONCAT_LINE_COUNTER( Count ) ) ) \
        { \
            INTERNAL_CATCH_CLASS_CONTEXT \
	        BEGIN_INTERNAL_CATCH_BATCH_METHOD( Category, INTERNAL_CATCH_CONCAT_LINE_COUNTER( Count ) ) \
            { \
                Catch::ConfigData cd; \
                cd.showSuccessfulTests = CatchOverrides::Config<Catch::IConfig const*>::instance().includeSuccessfulResults(__FILE__, __LINE__ ); \
                cd.warnings            = (CatchOverrides::Config<Catch::IConfig const*>::instance().warnAboutMissingAssertions(__FILE__, __LINE__ ) ? Catch::WarnAbout::NoAssertions : Catch::WarnAbout::Nothing); \
                cd.abortAfter          = CatchOverrides::Config<Catch::IConfig const*>::instance().abortAfter(__FILE__, __LINE__ ); \
                cd.name = "Batch run using tag : " Tag; \
                cd.testsOrTags.push_back( Tag ); \
                CollectConsoleOutput ptr(false); { \
                    Catch::Ptr<Catch::Config> config(new Catch::Config(cd)); \
                    config->setStreamBuf(ptr.rdbuf()); \
                    Catch::Runner runner(config); \
                    Catch::Totals totals = runner.runTests(); \
                    if( totals.assertions.failed > 0 ) { \
                        INTERNAL_CATCH_TEST_REPORT_BATCH_FAILURE(totals.assertions.failed) \
                    } \
                } \
            } \
        }; \
    }

#define INTERNAL_CATCH_MAP_CATEGORY_TO_LIST2( Category, CategoryName, Count ) \
    CHECK_FOR_TEST_CASE_CLASH \
    namespace CATCH_INTERNAL_NAMESPACE( INTERNAL_CATCH_CONCAT_LINE_COUNTER( Count ) ) { \
        CatchOverrides::ConfigReset<Catch::IConfig const*> INTERNAL_CATCH_UNIQUE_NAME_LINE( C_A_T_C_H____T_E_S_T____C_O_N_F_I_G___, INTERNAL_CATCH_CONCAT_LINE_COUNTER( Count ) )(__FILE__, __LINE__, -1); \
        INTERNAL_CATCH_CLASS_DEFINITION( INTERNAL_CATCH_UNIQUE_NAME_LINE( C_A_T_C_H____T_E_S_T____C_L_A_S_S___, INTERNAL_CATCH_CONCAT_LINE_COUNTER( Count ) ) ) \
        { \
            INTERNAL_CATCH_CLASS_CONTEXT \
	        BEGIN_INTERNAL_CATCH_BATCH_METHOD( Category, INTERNAL_CATCH_CONCAT_LINE_COUNTER( Count ) ) \
            { \
                Catch::ConfigData cd; \
                cd.showSuccessfulTests = CatchOverrides::Config<Catch::IConfig const*>::instance().includeSuccessfulResults(__FILE__, __LINE__ ); \
                cd.warnings            = (CatchOverrides::Config<Catch::IConfig const*>::instance().warnAboutMissingAssertions(__FILE__, __LINE__ ) ? Catch::WarnAbout::NoAssertions : Catch::WarnAbout::Nothing); \
                cd.abortAfter          = CatchOverrides::Config<Catch::IConfig const*>::instance().abortAfter(__FILE__, __LINE__ ); \
                cd.name = "Batch run using category : " CategoryName; \
                std::vector<std::string> stringNames = CatchOverrides::Config<Catch::IConfig const*>::instance().listOfTests(__FILE__, __LINE__ ); \
                CollectConsoleOutput ptr(false); { \
                    Catch::Ptr<Catch::Config> config(new Catch::Config(cd)); \
                    config->setStreamBuf(ptr.rdbuf()); \
                    Catch::RunContext context(config.get(), Catch::getRegistryHub().getReporterRegistry().create( "console", config.get())); \
                    Catch::Totals totals; \
                    context.testGroupStarting( "", 0, 1 ); \
                    for( std::vector<std::string>::iterator it = stringNames.begin(); it != stringNames.end(); ++it ) { \
                        std::vector<Catch::TestCase> testCase = Catch::getRegistryHub().getTestCaseRegistry().getMatchingTestCases(*it); \
                        if( testCase.empty() ) Assert::Fail(FAIL_STRING("No tests match")); \
                        if( testCase.size() > 1 ) Assert::Fail(FAIL_STRING("More than one test with the same name")); \
                        totals += context.runTest(*testCase.begin()); \
                    } \
                    context.testGroupEnded( "", totals, 0, 1 ); \
                    if( totals.assertions.failed > 0 ) { \
                        INTERNAL_CATCH_TEST_REPORT_BATCH_FAILURE(totals.assertions.failed) \
                    } \
                } \
            } \
        }; \
    }

// #undef CATCH_CONFIG_VARIADIC_MACROS

#ifdef CATCH_CONFIG_VARIADIC_MACROS

    #define INTERNAL_CATCH_SPLIT_ARGS_2( ... ) INTERNAL_CATCH_SPLIT_ARGS_IMPL_2((__VA_ARGS__, 2,1))
    #define INTERNAL_CATCH_SPLIT_ARGS_IMPL_2(tuple) INTERNAL_CATCH_SPLIT_ARGS_IMPL2 tuple
    #define INTERNAL_CATCH_SPLIT_ARGS_IMPL2(INTERNAL_CATCH_SPLIT_ARG_1,INTERNAL_CATCH_SPLIT_ARG_2,N,...) INTERNAL_CATCH_SPLIT_ARG_1
    #define INTERNAL_CATCH_SPLIT_TAGS( ... ) INTERNAL_CATCH_SPLIT_TAGS_IMPL((__VA_ARGS__, 2,1))
    #define INTERNAL_CATCH_SPLIT_TAGS_IMPL(tuple) INTERNAL_CATCH_SPLIT_TAGS_IMPL_ tuple
    #define INTERNAL_CATCH_SPLIT_TAGS_IMPL_(INTERNAL_CATCH_SPLIT_ARG_1,INTERNAL_CATCH_SPLIT_ARG_2,N,...) INTERNAL_CATCH_SPLIT_ARG_2

    #define INTERNAL_CATCH_TESTCASE( ... ) \
        INTERNAL_CATCH_TESTCASE2( __COUNTER__ , INTERNAL_CATCH_SPLIT_ARGS_2(__VA_ARGS__), INTERNAL_CATCH_SPLIT_TAGS(__VA_ARGS__) )

    ///////////////////////////////////////////////////////////////////////////////
    #define INTERNAL_CATCH_METHOD_AS_TEST_CASE( QualifiedMethod, ... ) \
        INTERNAL_CATCH_METHOD_AS_TEST_CASE2( QualifiedMethod, __COUNTER__, INTERNAL_CATCH_SPLIT_ARGS_2(__VA_ARGS__), INTERNAL_CATCH_SPLIT_TAGS(__VA_ARGS__) )

    ///////////////////////////////////////////////////////////////////////////////
    #define INTERNAL_CATCH_TEST_CASE_METHOD( ClassName, ... )\
        INTERNAL_CATCH_TEST_CASE_METHOD2(ClassName, __COUNTER__, INTERNAL_CATCH_SPLIT_ARGS_2(__VA_ARGS__), INTERNAL_CATCH_SPLIT_TAGS(__VA_ARGS__) )

#else
    ///////////////////////////////////////////////////////////////////////////////

    #define INTERNAL_CATCH_TESTCASE( Name, Desc ) \
        INTERNAL_CATCH_TESTCASE2( __COUNTER__ , Name, Desc )

    ///////////////////////////////////////////////////////////////////////////////
    #define INTERNAL_CATCH_METHOD_AS_TEST_CASE( QualifiedMethod, Name, Desc ) \
        INTERNAL_CATCH_METHOD_AS_TEST_CASE2( QualifiedMethod, __COUNTER__, Name, Desc )

    ///////////////////////////////////////////////////////////////////////////////
    #define INTERNAL_CATCH_TEST_CASE_METHOD( ClassName, TestName, Desc )\
        INTERNAL_CATCH_TEST_CASE_METHOD2(ClassName, __COUNTER__, TestName, Desc )

#endif

#else // defined(INTERNAL_CATCH_VS_MANAGED) || defined(INTERNAL_CATCH_VS_NATIVE)

namespace Catch {

template<typename C>
class MethodTestCase : public SharedImpl<ITestCase> {

public:
    MethodTestCase( void (C::*method)() ) : m_method( method ) {}

    virtual void invoke() const {
        C obj;
        (obj.*m_method)();
    }

private:
    virtual ~MethodTestCase() {}

    void (C::*m_method)();
};

typedef void(*TestFunction)();

struct NameAndDesc {
    NameAndDesc( const char* _name = "", const char* _description= "" )
    : name( _name ), description( _description )
    {}

    const char* name;
    const char* description;
};

struct AutoReg {

    AutoReg(    TestFunction function,
                SourceLineInfo const& lineInfo,
                NameAndDesc const& nameAndDesc );

    template<typename C>
    AutoReg(    void (C::*method)(),
                char const* className,
                NameAndDesc const& nameAndDesc,
                SourceLineInfo const& lineInfo ) {
        registerTestCase(   new MethodTestCase<C>( method ),
                            className,
                            nameAndDesc,
                            lineInfo );
    }

    void registerTestCase(  ITestCase* testCase,
                            char const* className,
                            NameAndDesc const& nameAndDesc,
                            SourceLineInfo const& lineInfo );

    ~AutoReg();

private:
    AutoReg( AutoReg const& );
    void operator= ( AutoReg const& );
};

} // end namespace Catch

#ifdef CATCH_CONFIG_VARIADIC_MACROS
    ///////////////////////////////////////////////////////////////////////////////
    #define INTERNAL_CATCH_TESTCASE( ... ) \
        static void INTERNAL_CATCH_UNIQUE_NAME( ____C_A_T_C_H____T_E_S_T____ )(); \
        namespace{ Catch::AutoReg INTERNAL_CATCH_UNIQUE_NAME( autoRegistrar )( &INTERNAL_CATCH_UNIQUE_NAME(  ____C_A_T_C_H____T_E_S_T____ ), CATCH_INTERNAL_LINEINFO, Catch::NameAndDesc( __VA_ARGS__ ) ); }\
        static void INTERNAL_CATCH_UNIQUE_NAME(  ____C_A_T_C_H____T_E_S_T____ )()

    ///////////////////////////////////////////////////////////////////////////////
    #define INTERNAL_CATCH_METHOD_AS_TEST_CASE( QualifiedMethod, ... ) \
        namespace{ Catch::AutoReg INTERNAL_CATCH_UNIQUE_NAME( autoRegistrar )( &QualifiedMethod, "&" #QualifiedMethod, Catch::NameAndDesc( __VA_ARGS__ ), CATCH_INTERNAL_LINEINFO ); }

    ///////////////////////////////////////////////////////////////////////////////
    #define INTERNAL_CATCH_TEST_CASE_METHOD( ClassName, ... )\
        namespace{ \
            struct INTERNAL_CATCH_UNIQUE_NAME( ____C_A_T_C_H____T_E_S_T____ ) : ClassName{ \
                void test(); \
            }; \
            Catch::AutoReg INTERNAL_CATCH_UNIQUE_NAME( autoRegistrar ) ( &INTERNAL_CATCH_UNIQUE_NAME( ____C_A_T_C_H____T_E_S_T____ )::test, #ClassName, Catch::NameAndDesc( __VA_ARGS__ ), CATCH_INTERNAL_LINEINFO ); \
        } \
        void INTERNAL_CATCH_UNIQUE_NAME( ____C_A_T_C_H____T_E_S_T____ )::test()

#else
    ///////////////////////////////////////////////////////////////////////////////
    #define INTERNAL_CATCH_TESTCASE( Name, Desc ) \
        static void INTERNAL_CATCH_UNIQUE_NAME( ____C_A_T_C_H____T_E_S_T____ )(); \
        namespace{ Catch::AutoReg INTERNAL_CATCH_UNIQUE_NAME( autoRegistrar )( &INTERNAL_CATCH_UNIQUE_NAME(  ____C_A_T_C_H____T_E_S_T____ ), CATCH_INTERNAL_LINEINFO, Catch::NameAndDesc( Name, Desc ) ); }\
        static void INTERNAL_CATCH_UNIQUE_NAME(  ____C_A_T_C_H____T_E_S_T____ )()

    ///////////////////////////////////////////////////////////////////////////////
    #define INTERNAL_CATCH_METHOD_AS_TEST_CASE( QualifiedMethod, Name, Desc ) \
        namespace{ Catch::AutoReg INTERNAL_CATCH_UNIQUE_NAME( autoRegistrar )( &QualifiedMethod, "&" #QualifiedMethod, Catch::NameAndDesc( Name, Desc ), CATCH_INTERNAL_LINEINFO ); }

    ///////////////////////////////////////////////////////////////////////////////
    #define INTERNAL_CATCH_TEST_CASE_METHOD( ClassName, TestName, Desc )\
        namespace{ \
            struct INTERNAL_CATCH_UNIQUE_NAME( ____C_A_T_C_H____T_E_S_T____ ) : ClassName{ \
                void test(); \
            }; \
            Catch::AutoReg INTERNAL_CATCH_UNIQUE_NAME( autoRegistrar ) ( &INTERNAL_CATCH_UNIQUE_NAME( ____C_A_T_C_H____T_E_S_T____ )::test, #ClassName, Catch::NameAndDesc( TestName, Desc ), CATCH_INTERNAL_LINEINFO ); \
        } \
        void INTERNAL_CATCH_UNIQUE_NAME( ____C_A_T_C_H____T_E_S_T____ )::test()

#endif

#endif // defined(INTERNAL_CATCH_VS_MANAGED) || defined(INTERNAL_CATCH_VS_NATIVE)

// #included from: internal/catch_capture.hpp
#define TWOBLUECUBES_CATCH_CAPTURE_HPP_INCLUDED

// #included from: catch_expression_decomposer.hpp
#define TWOBLUECUBES_CATCH_EXPRESSION_DECOMPOSER_HPP_INCLUDED

// #included from: catch_expression_lhs.hpp
#define TWOBLUECUBES_CATCH_EXPRESSION_LHS_HPP_INCLUDED

// #included from: catch_expressionresult_builder.h
#define TWOBLUECUBES_CATCH_ASSERTIONRESULT_BUILDER_H_INCLUDED

// #included from: catch_tostring.hpp
#define TWOBLUECUBES_CATCH_TOSTRING_HPP_INCLUDED

// #included from: catch_sfinae.hpp
#define TWOBLUECUBES_CATCH_SFINAE_HPP_INCLUDED

// Try to detect if the current compiler supports SFINAE

namespace Catch {

    struct TrueType {
        static const bool value = true;
        typedef void Enable;
        char sizer[1];
    };
    struct FalseType {
        static const bool value = false;
        typedef void Disable;
        char sizer[2];
    };

#ifdef CATCH_CONFIG_SFINAE

    template<bool> struct NotABooleanExpression;

    template<bool c> struct If : NotABooleanExpression<c> {};
    template<> struct If<true> : TrueType {};
    template<> struct If<false> : FalseType {};

    template<int size> struct SizedIf;
    template<> struct SizedIf<sizeof(TrueType)> : TrueType {};
    template<> struct SizedIf<sizeof(FalseType)> : FalseType {};

#endif // CATCH_CONFIG_SFINAE

} // end namespace Catch

#include <sstream>
#include <iomanip>
#include <limits>
#include <vector>

#ifdef __OBJC__
// #included from: catch_objc_arc.hpp
#define TWOBLUECUBES_CATCH_OBJC_ARC_HPP_INCLUDED

#import <Foundation/Foundation.h>

#ifdef __has_feature
#define CATCH_ARC_ENABLED __has_feature(objc_arc)
#else
#define CATCH_ARC_ENABLED 0
#endif

void arcSafeRelease( NSObject* obj );
id performOptionalSelector( id obj, SEL sel );

#if !CATCH_ARC_ENABLED
inline void arcSafeRelease( NSObject* obj ) {
    [obj release];
}
inline id performOptionalSelector( id obj, SEL sel ) {
    if( [obj respondsToSelector: sel] )
        return [obj performSelector: sel];
    return nil;
}
#define CATCH_UNSAFE_UNRETAINED
#define CATCH_ARC_STRONG
#else
inline void arcSafeRelease( NSObject* ){}
inline id performOptionalSelector( id obj, SEL sel ) {
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Warc-performSelector-leaks"
#endif
    if( [obj respondsToSelector: sel] )
        return [obj performSelector: sel];
#ifdef __clang__
#pragma clang diagnostic pop
#endif
    return nil;
}
#define CATCH_UNSAFE_UNRETAINED __unsafe_unretained
#define CATCH_ARC_STRONG __strong
#endif

#endif

namespace Catch {
namespace Detail {

// SFINAE is currently disabled by default for all compilers.
// If the non SFINAE version of IsStreamInsertable is ambiguous for you
// and your compiler supports SFINAE, try #defining CATCH_CONFIG_SFINAE
#ifdef CATCH_CONFIG_SFINAE

    template<typename T>
    class IsStreamInsertableHelper {
        template<int N> struct TrueIfSizeable : TrueType {};

        template<typename T2>
        static TrueIfSizeable<sizeof((*(std::ostream*)0) << *((T2 const*)0))> dummy(T2*);
        static FalseType dummy(...);

    public:
        typedef SizedIf<sizeof(dummy((T*)0))> type;
    };

    template<typename T>
    struct IsStreamInsertable : IsStreamInsertableHelper<T>::type {};

#else

    struct BorgType {
        template<typename T> BorgType( T const& );
    };

    TrueType& testStreamable( std::ostream& );
    FalseType testStreamable( FalseType );

    FalseType operator<<( std::ostream const&, BorgType const& );

    template<typename T>
    struct IsStreamInsertable {
        static std::ostream &s;
        static T  const&t;
        enum { value = sizeof( testStreamable(s << t) ) == sizeof( TrueType ) };
    };

#endif

    template<bool C>
    struct StringMakerBase {
        template<typename T>
        static std::string convert( T const& ) { return "{?}"; }
    };

    template<>
    struct StringMakerBase<true> {
        template<typename T>
        static std::string convert( T const& _value ) {
            std::ostringstream oss;
            oss << _value;
            return oss.str();
        }
    };

    // For display purposes only.
    // Anything implicitly convertible to void* ends up here
    inline std::string rawMemoryToString(void* p)
    {
        std::ostringstream oss;
#ifdef _MSC_VER
        oss << "0x" << p;
#else
        oss << p;
#endif
        return oss.str();
    }

    // For display purposes only.
    // Does not consider endian-ness
    template<typename T>
    std::string rawMemoryToString( T value ) {
        union {
            T typedValue;
            unsigned char bytes[sizeof(T)];
        };

        typedValue = value;

        std::ostringstream oss;
        oss << "0x";
        for( unsigned char* cp = bytes; cp < bytes+sizeof(T); ++cp )
            oss << std::hex << std::setw(2) << std::setfill('0') << (unsigned int)*cp;
        return oss.str();
    }

} // end namespace Detail

template<typename T>
std::string toString( T const& value );

template<typename T>
struct StringMaker :
    Detail::StringMakerBase<Detail::IsStreamInsertable<T>::value> {};

template<typename T>
struct StringMaker<T*> {
    template<typename U>
    static std::string convert( U* p ) {
        if( !p )
            return INTERNAL_CATCH_STRINGIFY( NULL );
        else
            return Detail::rawMemoryToString( p );
    }
};

template<typename R, typename C>
struct StringMaker<R C::*> {
    static std::string convert( R C::* p ) {
        if( !p )
            return INTERNAL_CATCH_STRINGIFY( NULL );
        else
            return Detail::rawMemoryToString( p );
    }
};

namespace Detail {
    template<typename InputIterator>
    std::string rangeToString( InputIterator first, InputIterator last );
}

template<typename T, typename Allocator>
struct StringMaker<std::vector<T, Allocator> > {
    static std::string convert( std::vector<T,Allocator> const& v ) {
        return Detail::rangeToString( v.begin(), v.end() );
    }
};

namespace Detail {
    template<typename T>
    inline std::string makeString( T const& value ) {
        return StringMaker<T>::convert( value );
    }
} // end namespace Detail

/// \brief converts any type to a string
///
/// The default template forwards on to ostringstream - except when an
/// ostringstream overload does not exist - in which case it attempts to detect
/// that and writes {?}.
/// Overload (not specialise) this template for custom typs that you don't want
/// to provide an ostream overload for.
template<typename T>
std::string toString( T const& value ) {
    return StringMaker<T>::convert( value );
}

// Built in overloads

inline std::string toString( std::string const& value ) {
    return "\"" + value + "\"";
}

inline std::string toString( std::wstring const& value ) {
    std::ostringstream oss;
    oss << "\"";
    for(size_t i = 0; i < value.size(); ++i )
        oss << static_cast<char>( value[i] <= 0xff ? value[i] : '?');
    oss << "\"";
    return oss.str();
}

inline std::string toString( const char* const value ) {
    return value ? Catch::toString( std::string( value ) ) : std::string( "{null string}" );
}

inline std::string toString( char* const value ) {
    return Catch::toString( static_cast<const char*>( value ) );
}

inline std::string toString( int value ) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

inline std::string toString( unsigned long value ) {
    std::ostringstream oss;
    if( value > 8192 )
        oss << "0x" << std::hex << value;
    else
        oss << value;
    return oss.str();
}

inline std::string toString( unsigned int value ) {
    return toString( static_cast<unsigned long>( value ) );
}

inline std::string toString( const double value ) {
    std::ostringstream oss;
    oss << std::setprecision( 10 )
        << std::fixed
        << value;
    std::string d = oss.str();
    std::size_t i = d.find_last_not_of( '0' );
    if( i != std::string::npos && i != d.size()-1 ) {
        if( d[i] == '.' )
            i++;
        d = d.substr( 0, i+1 );
    }
    return d;
}

inline std::string toString( bool value ) {
    return value ? "true" : "false";
}

inline std::string toString( char value ) {
    return value < ' '
        ? toString( static_cast<unsigned int>( value ) )
        : Detail::makeString( value );
}

inline std::string toString( signed char value ) {
    return toString( static_cast<char>( value ) );
}

inline std::string toString( unsigned char value ) {
    return toString( static_cast<char>( value ) );
}

#ifdef CATCH_CONFIG_CPP11_NULLPTR
inline std::string toString( std::nullptr_t ) {
    return "nullptr";
}
#endif

#ifdef __OBJC__
    inline std::string toString( NSString const * const& nsstring ) {
        if( !nsstring )
            return "nil";
        return std::string( "@\"" ) + [nsstring UTF8String] + "\"";
    }
    inline std::string toString( NSString * CATCH_ARC_STRONG const& nsstring ) {
        if( !nsstring )
            return "nil";
        return std::string( "@\"" ) + [nsstring UTF8String] + "\"";
    }
    inline std::string toString( NSObject* const& nsObject ) {
        return toString( [nsObject description] );
    }
#endif

    namespace Detail {
    template<typename InputIterator>
    std::string rangeToString( InputIterator first, InputIterator last ) {
        std::ostringstream oss;
        oss << "{ ";
        if( first != last ) {
            oss << toString( *first );
            for( ++first ; first != last ; ++first ) {
                oss << ", " << toString( *first );
            }
        }
        oss << " }";
        return oss.str();
    }
}

} // end namespace Catch

// #included from: catch_assertionresult.h
#define TWOBLUECUBES_CATCH_ASSERTIONRESULT_H_INCLUDED

#include <string>
// #included from: catch_result_type.h
#define TWOBLUECUBES_CATCH_RESULT_TYPE_H_INCLUDED

namespace Catch {

    // ResultWas::OfType enum
    struct ResultWas { enum OfType {
        Unknown = -1,
        Ok = 0,
        Info = 1,
        Warning = 2,

        FailureBit = 0x10,

        ExpressionFailed = FailureBit | 1,
        ExplicitFailure = FailureBit | 2,

        Exception = 0x100 | FailureBit,

        ThrewException = Exception | 1,
        DidntThrowException = Exception | 2

    }; };

    inline bool isOk( ResultWas::OfType resultType ) {
        return ( resultType & ResultWas::FailureBit ) == 0;
    }
    inline bool isJustInfo( int flags ) {
        return flags == ResultWas::Info;
    }

    // ResultAction::Value enum
    struct ResultAction { enum Value {
        None,
        Failed = 1, // Failure - but no debug break if Debug bit not set
        Debug = 2,  // If this bit is set, invoke the debugger
        Abort = 4   // Test run should abort
    }; };

    // ResultDisposition::Flags enum
    struct ResultDisposition { enum Flags {
            Normal = 0x00,

            ContinueOnFailure = 0x01,   // Failures fail test, but execution continues
            NegateResult = 0x02,        // Prefix expressiom with !
            SuppressFail = 0x04         // Failures are reported but do not fail the test
    }; };

    inline ResultDisposition::Flags operator | ( ResultDisposition::Flags lhs, ResultDisposition::Flags rhs ) {
        return static_cast<ResultDisposition::Flags>( static_cast<int>( lhs ) | static_cast<int>( rhs ) );
    }

    inline bool shouldContinueOnFailure( int flags )    { return ( flags & ResultDisposition::ContinueOnFailure ) != 0; }
    inline bool shouldNegate( int flags )               { return ( flags & ResultDisposition::NegateResult ) != 0; }
    inline bool shouldSuppressFailure( int flags )      { return ( flags & ResultDisposition::SuppressFail ) != 0; }

} // end namespace Catch


namespace Catch {

    struct AssertionInfo
    {
        AssertionInfo() {}
        AssertionInfo(  std::string const& _macroName,
                        SourceLineInfo const& _lineInfo,
                        std::string const& _capturedExpression,
                        ResultDisposition::Flags _resultDisposition );

        std::string macroName;
        SourceLineInfo lineInfo;
        std::string capturedExpression;
        ResultDisposition::Flags resultDisposition;
    };

    struct AssertionResultData
    {
        AssertionResultData() : resultType( ResultWas::Unknown ) {}

        std::string reconstructedExpression;
        std::string message;
        ResultWas::OfType resultType;
    };

    class AssertionResult {
    public:
        AssertionResult();
        AssertionResult( AssertionInfo const& info, AssertionResultData const& data );
        ~AssertionResult();

        bool isOk() const;
        bool succeeded() const;
        ResultWas::OfType getResultType() const;
        bool hasExpression() const;
        bool hasMessage() const;
        std::string getExpression() const;
        std::string getExpressionInMacro() const;
        bool hasExpandedExpression() const;
        std::string getExpandedExpression() const;
        std::string getMessage() const;
        SourceLineInfo getSourceInfo() const;
        std::string getTestMacroName() const;

    protected:
        AssertionInfo m_info;
        AssertionResultData m_resultData;
    };

} // end namespace Catch

// #included from: catch_evaluate.hpp
#define TWOBLUECUBES_CATCH_EVALUATE_HPP_INCLUDED

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4389) // '==' : signed/unsigned mismatch
#endif

namespace Catch {
namespace Internal {

    enum Operator {
        IsEqualTo,
        IsNotEqualTo,
        IsLessThan,
        IsGreaterThan,
        IsLessThanOrEqualTo,
        IsGreaterThanOrEqualTo
    };

    template<Operator Op> struct OperatorTraits             { static const char* getName(){ return "*error*"; } };
    template<> struct OperatorTraits<IsEqualTo>             { static const char* getName(){ return "=="; } };
    template<> struct OperatorTraits<IsNotEqualTo>          { static const char* getName(){ return "!="; } };
    template<> struct OperatorTraits<IsLessThan>            { static const char* getName(){ return "<"; } };
    template<> struct OperatorTraits<IsGreaterThan>         { static const char* getName(){ return ">"; } };
    template<> struct OperatorTraits<IsLessThanOrEqualTo>   { static const char* getName(){ return "<="; } };
    template<> struct OperatorTraits<IsGreaterThanOrEqualTo>{ static const char* getName(){ return ">="; } };

    template<typename T>
    inline T& opCast(T const& t) { return const_cast<T&>(t); }

// nullptr_t support based on pull request #154 from Konstantin Baumann
#ifdef CATCH_CONFIG_CPP11_NULLPTR
    inline std::nullptr_t opCast(std::nullptr_t) { return nullptr; }
#endif // CATCH_CONFIG_CPP11_NULLPTR

    // So the compare overloads can be operator agnostic we convey the operator as a template
    // enum, which is used to specialise an Evaluator for doing the comparison.
    template<typename T1, typename T2, Operator Op>
    class Evaluator{};

    template<typename T1, typename T2>
    struct Evaluator<T1, T2, IsEqualTo> {
        static bool evaluate( T1 const& lhs, T2 const& rhs) {
            return opCast( lhs ) ==  opCast( rhs );
        }
    };
    template<typename T1, typename T2>
    struct Evaluator<T1, T2, IsNotEqualTo> {
        static bool evaluate( T1 const& lhs, T2 const& rhs ) {
            return opCast( lhs ) != opCast( rhs );
        }
    };
    template<typename T1, typename T2>
    struct Evaluator<T1, T2, IsLessThan> {
        static bool evaluate( T1 const& lhs, T2 const& rhs ) {
            return opCast( lhs ) < opCast( rhs );
        }
    };
    template<typename T1, typename T2>
    struct Evaluator<T1, T2, IsGreaterThan> {
        static bool evaluate( T1 const& lhs, T2 const& rhs ) {
            return opCast( lhs ) > opCast( rhs );
        }
    };
    template<typename T1, typename T2>
    struct Evaluator<T1, T2, IsGreaterThanOrEqualTo> {
        static bool evaluate( T1 const& lhs, T2 const& rhs ) {
            return opCast( lhs ) >= opCast( rhs );
        }
    };
    template<typename T1, typename T2>
    struct Evaluator<T1, T2, IsLessThanOrEqualTo> {
        static bool evaluate( T1 const& lhs, T2 const& rhs ) {
            return opCast( lhs ) <= opCast( rhs );
        }
    };

    template<Operator Op, typename T1, typename T2>
    bool applyEvaluator( T1 const& lhs, T2 const& rhs ) {
        return Evaluator<T1, T2, Op>::evaluate( lhs, rhs );
    }

    // This level of indirection allows us to specialise for integer types
    // to avoid signed/ unsigned warnings

    // "base" overload
    template<Operator Op, typename T1, typename T2>
    bool compare( T1 const& lhs, T2 const& rhs ) {
        return Evaluator<T1, T2, Op>::evaluate( lhs, rhs );
    }

    // unsigned X to int
    template<Operator Op> bool compare( unsigned int lhs, int rhs ) {
        return applyEvaluator<Op>( lhs, static_cast<unsigned int>( rhs ) );
    }
    template<Operator Op> bool compare( unsigned long lhs, int rhs ) {
        return applyEvaluator<Op>( lhs, static_cast<unsigned int>( rhs ) );
    }
    template<Operator Op> bool compare( unsigned char lhs, int rhs ) {
        return applyEvaluator<Op>( lhs, static_cast<unsigned int>( rhs ) );
    }

    // unsigned X to long
    template<Operator Op> bool compare( unsigned int lhs, long rhs ) {
        return applyEvaluator<Op>( lhs, static_cast<unsigned long>( rhs ) );
    }
    template<Operator Op> bool compare( unsigned long lhs, long rhs ) {
        return applyEvaluator<Op>( lhs, static_cast<unsigned long>( rhs ) );
    }
    template<Operator Op> bool compare( unsigned char lhs, long rhs ) {
        return applyEvaluator<Op>( lhs, static_cast<unsigned long>( rhs ) );
    }

    // int to unsigned X
    template<Operator Op> bool compare( int lhs, unsigned int rhs ) {
        return applyEvaluator<Op>( static_cast<unsigned int>( lhs ), rhs );
    }
    template<Operator Op> bool compare( int lhs, unsigned long rhs ) {
        return applyEvaluator<Op>( static_cast<unsigned int>( lhs ), rhs );
    }
    template<Operator Op> bool compare( int lhs, unsigned char rhs ) {
        return applyEvaluator<Op>( static_cast<unsigned int>( lhs ), rhs );
    }

    // long to unsigned X
    template<Operator Op> bool compare( long lhs, unsigned int rhs ) {
        return applyEvaluator<Op>( static_cast<unsigned long>( lhs ), rhs );
    }
    template<Operator Op> bool compare( long lhs, unsigned long rhs ) {
        return applyEvaluator<Op>( static_cast<unsigned long>( lhs ), rhs );
    }
    template<Operator Op> bool compare( long lhs, unsigned char rhs ) {
        return applyEvaluator<Op>( static_cast<unsigned long>( lhs ), rhs );
    }

    // pointer to long (when comparing against NULL)
    template<Operator Op, typename T> bool compare( long lhs, T* rhs ) {
        return Evaluator<T*, T*, Op>::evaluate( reinterpret_cast<T*>( lhs ), rhs );
    }
    template<Operator Op, typename T> bool compare( T* lhs, long rhs ) {
        return Evaluator<T*, T*, Op>::evaluate( lhs, reinterpret_cast<T*>( rhs ) );
    }

    // pointer to int (when comparing against NULL)
    template<Operator Op, typename T> bool compare( int lhs, T* rhs ) {
        return Evaluator<T*, T*, Op>::evaluate( reinterpret_cast<T*>( lhs ), rhs );
    }
    template<Operator Op, typename T> bool compare( T* lhs, int rhs ) {
        return Evaluator<T*, T*, Op>::evaluate( lhs, reinterpret_cast<T*>( rhs ) );
    }

#ifdef CATCH_CONFIG_CPP11_NULLPTR
    // pointer to nullptr_t (when comparing against nullptr)
    template<Operator Op, typename T> bool compare( std::nullptr_t, T* rhs ) {
        return Evaluator<T*, T*, Op>::evaluate( NULL, rhs );
    }
    template<Operator Op, typename T> bool compare( T* lhs, std::nullptr_t ) {
        return Evaluator<T*, T*, Op>::evaluate( lhs, NULL );
    }
#endif // CATCH_CONFIG_CPP11_NULLPTR

} // end of namespace Internal
} // end of namespace Catch

#ifdef _MSC_VER
#pragma warning(pop)
#endif

namespace Catch {

struct STATIC_ASSERT_Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison;

// Wraps the (stringised versions of) the lhs, operator and rhs of an expression - as well as
// the result of evaluating it. This is used to build an AssertionResult object
class ExpressionResultBuilder {
public:

    ExpressionResultBuilder( ResultWas::OfType resultType = ResultWas::Unknown );
    ExpressionResultBuilder( ExpressionResultBuilder const& other );
    ExpressionResultBuilder& operator=(ExpressionResultBuilder const& other );

    ExpressionResultBuilder& setResultType( ResultWas::OfType result );
    ExpressionResultBuilder& setResultType( bool result );
    ExpressionResultBuilder& setLhs( std::string const& lhs );
    ExpressionResultBuilder& setRhs( std::string const& rhs );
    ExpressionResultBuilder& setOp( std::string const& op );

    ExpressionResultBuilder& endExpression( ResultDisposition::Flags resultDisposition );

    template<typename T>
    ExpressionResultBuilder& operator << ( T const& value ) {
        m_stream << value;
        return *this;
    }

    std::string reconstructExpression( AssertionInfo const& info ) const;

    AssertionResult buildResult( AssertionInfo const& info ) const;

    template<typename RhsT> STATIC_ASSERT_Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison& operator && ( RhsT const& );
    template<typename RhsT> STATIC_ASSERT_Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison& operator || ( RhsT const& );

private:
    AssertionResultData m_data;
    struct ExprComponents {
        ExprComponents() : shouldNegate( false ) {}
        bool shouldNegate;
        std::string lhs, rhs, op;
    } m_exprComponents;
    std::ostringstream m_stream;
};

} // end namespace Catch

namespace Catch {

// Wraps the LHS of an expression and captures the operator and RHS (if any) - wrapping them all
// in an ExpressionResultBuilder object
template<typename T>
class ExpressionLhs {
    void operator = ( ExpressionLhs const& );

public:
    ExpressionLhs( T lhs ) : m_lhs( lhs ) {}

    template<typename RhsT>
    ExpressionResultBuilder& operator == ( RhsT const& rhs ) {
        return captureExpression<Internal::IsEqualTo>( rhs );
    }

    template<typename RhsT>
    ExpressionResultBuilder& operator != ( RhsT const& rhs ) {
        return captureExpression<Internal::IsNotEqualTo>( rhs );
    }

    template<typename RhsT>
    ExpressionResultBuilder& operator < ( RhsT const& rhs ) {
        return captureExpression<Internal::IsLessThan>( rhs );
    }

    template<typename RhsT>
    ExpressionResultBuilder& operator > ( RhsT const& rhs ) {
        return captureExpression<Internal::IsGreaterThan>( rhs );
    }

    template<typename RhsT>
    ExpressionResultBuilder& operator <= ( RhsT const& rhs ) {
        return captureExpression<Internal::IsLessThanOrEqualTo>( rhs );
    }

    template<typename RhsT>
    ExpressionResultBuilder& operator >= ( RhsT const& rhs ) {
        return captureExpression<Internal::IsGreaterThanOrEqualTo>( rhs );
    }

    ExpressionResultBuilder& operator == ( bool rhs ) {
        return captureExpression<Internal::IsEqualTo>( rhs );
    }

    ExpressionResultBuilder& operator != ( bool rhs ) {
        return captureExpression<Internal::IsNotEqualTo>( rhs );
    }

    ExpressionResultBuilder& endExpression( ResultDisposition::Flags resultDisposition ) {
        bool value = m_lhs ? true : false;
        return m_result
            .setLhs( Catch::toString( value ) )
            .setResultType( value )
            .endExpression( resultDisposition );
    }

    // Only simple binary expressions are allowed on the LHS.
    // If more complex compositions are required then place the sub expression in parentheses
    template<typename RhsT> STATIC_ASSERT_Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison& operator + ( RhsT const& );
    template<typename RhsT> STATIC_ASSERT_Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison& operator - ( RhsT const& );
    template<typename RhsT> STATIC_ASSERT_Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison& operator / ( RhsT const& );
    template<typename RhsT> STATIC_ASSERT_Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison& operator * ( RhsT const& );
    template<typename RhsT> STATIC_ASSERT_Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison& operator && ( RhsT const& );
    template<typename RhsT> STATIC_ASSERT_Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison& operator || ( RhsT const& );

private:
    template<Internal::Operator Op, typename RhsT>
    ExpressionResultBuilder& captureExpression( RhsT const& rhs ) {
        return m_result
            .setResultType( Internal::compare<Op>( m_lhs, rhs ) )
            .setLhs( Catch::toString( m_lhs ) )
            .setRhs( Catch::toString( rhs ) )
            .setOp( Internal::OperatorTraits<Op>::getName() );
    }

private:
    ExpressionResultBuilder m_result;
    T m_lhs;
};

} // end namespace Catch

namespace Catch {

// Captures the LHS of the expression and wraps it in an Expression Lhs object
class ExpressionDecomposer {
public:

    template<typename T>
    ExpressionLhs<T const&> operator->* ( T const& operand ) {
        return ExpressionLhs<T const&>( operand );
    }

    ExpressionLhs<bool> operator->* ( bool value ) {
        return ExpressionLhs<bool>( value );
    }
};

} // end namespace Catch

// #included from: catch_message.h
#define TWOBLUECUBES_CATCH_MESSAGE_H_INCLUDED

#include <string>

namespace Catch {

    struct MessageInfo {
        MessageInfo(    std::string const& _macroName,
                        SourceLineInfo const& _lineInfo,
                        ResultWas::OfType _type );

        std::string macroName;
        SourceLineInfo lineInfo;
        ResultWas::OfType type;
        std::string message;
        unsigned int sequence;

        bool operator == ( MessageInfo const& other ) const {
            return sequence == other.sequence;
        }
        bool operator < ( MessageInfo const& other ) const {
            return sequence < other.sequence;
        }
    };

    struct MessageBuilder {
        MessageBuilder( std::string const& macroName,
                        SourceLineInfo const& lineInfo,
                        ResultWas::OfType type )
        : m_info( macroName, lineInfo, type )
        {}

        template<typename T>
        MessageBuilder& operator << ( T const& value ) {
            m_stream << value;
            return *this;
        }

        MessageInfo m_info;
        std::ostringstream m_stream;
    };

    class ScopedMessage {
    public:
        ScopedMessage( MessageBuilder const& builder );
        ~ScopedMessage();

        MessageInfo m_info;
    };

} // end namespace Catch

// #included from: catch_interfaces_capture.h
#define TWOBLUECUBES_CATCH_INTERFACES_CAPTURE_H_INCLUDED

#include <string>

namespace Catch {

    class TestCase;
    class ExpressionResultBuilder;
    class AssertionResult;
    struct AssertionInfo;
    struct SectionInfo;
    struct MessageInfo;
    class ScopedMessageBuilder;
    struct Counts;

    struct IResultCapture {

        virtual ~IResultCapture();

        virtual void assertionEnded( AssertionResult const& result ) = 0;
        virtual bool sectionStarted(    SectionInfo const& sectionInfo,
                                        Counts& assertions ) = 0;
        virtual void sectionEnded( SectionInfo const& name, Counts const& assertions, double _durationInSeconds ) = 0;
        virtual void pushScopedMessage( MessageInfo const& message ) = 0;
        virtual void popScopedMessage( MessageInfo const& message ) = 0;

        virtual bool shouldDebugBreak() const = 0;

        virtual ResultAction::Value acceptExpression( ExpressionResultBuilder const& assertionResult, AssertionInfo const& assertionInfo ) = 0;

        virtual std::string getCurrentTestName() const = 0;
        virtual const AssertionResult* getLastResult() const = 0;
    };

    struct NullResultCapture : public IResultCapture {

        virtual void assertionEnded( AssertionResult const& ) {}
        virtual bool sectionStarted(    SectionInfo const& ,
            Counts&  ) {return false;}
        virtual void sectionEnded( SectionInfo const& , Counts const& , double ) {}
        virtual void pushScopedMessage( MessageInfo const& ) {}
        virtual void popScopedMessage( MessageInfo const& ) {}

        virtual bool shouldDebugBreak() const {return false;}

        virtual ResultAction::Value acceptExpression( ExpressionResultBuilder const& , AssertionInfo const&  ) {return ResultAction::Abort;}

        virtual std::string getCurrentTestName() const {return std::string();}
        virtual const AssertionResult* getLastResult() const {return NULL;}
    };
}

// #included from: catch_debugger.h
#define TWOBLUECUBES_CATCH_DEBUGGER_H_INCLUDED

// #included from: catch_platform.h
#define TWOBLUECUBES_CATCH_PLATFORM_H_INCLUDED

#if defined(__MAC_OS_X_VERSION_MIN_REQUIRED)
#define CATCH_PLATFORM_MAC
#elif  defined(__IPHONE_OS_VERSION_MIN_REQUIRED)
#define CATCH_PLATFORM_IPHONE
#elif defined(WIN32) || defined(__WIN32__) || defined(_WIN32) || defined(_MSC_VER)
#define CATCH_PLATFORM_WINDOWS
#endif

#include <string>

namespace Catch{

    bool isDebuggerActive();
    void writeToDebugConsole( std::string const& text );
}

#ifdef CATCH_PLATFORM_MAC

    // The following code snippet based on:
    // http://cocoawithlove.com/2008/03/break-into-debugger.html
    #ifdef DEBUG
        #if defined(__ppc64__) || defined(__ppc__)
            #define CATCH_BREAK_INTO_DEBUGGER() \
                if( Catch::isDebuggerActive() ) { \
                    __asm__("li r0, 20\nsc\nnop\nli r0, 37\nli r4, 2\nsc\nnop\n" \
                    : : : "memory","r0","r3","r4" ); \
                }
        #else
            #define CATCH_BREAK_INTO_DEBUGGER() if( Catch::isDebuggerActive() ) {__asm__("int $3\n" : : );}
        #endif
    #endif

#elif defined(_MSC_VER)
    #define CATCH_BREAK_INTO_DEBUGGER() if( Catch::isDebuggerActive() ) { __debugbreak(); }
#elif defined(__MINGW32__)
    extern "C" __declspec(dllimport) void __stdcall DebugBreak();
    #define CATCH_BREAK_INTO_DEBUGGER() if( Catch::isDebuggerActive() ) { DebugBreak(); }
#endif

#ifndef CATCH_BREAK_INTO_DEBUGGER
#define CATCH_BREAK_INTO_DEBUGGER() Catch::isTrue( true );
#endif

// #included from: catch_interfaces_reporter.h
#define TWOBLUECUBES_CATCH_INTERFACES_REPORTER_H_INCLUDED

// #included from: catch_section_info.h
#define TWOBLUECUBES_CATCH_SECTION_INFO_H_INCLUDED

namespace Catch {

    struct SectionInfo {
        SectionInfo(    std::string const& _name,
                        std::string const& _description,
                        SourceLineInfo const& _lineInfo )
        :   name( _name ),
            description( _description ),
            lineInfo( _lineInfo )
        {}

        std::string name;
        std::string description;
        SourceLineInfo lineInfo;
    };

} // end namespace Catch

// #included from: catch_totals.hpp
#define TWOBLUECUBES_CATCH_TOTALS_HPP_INCLUDED

#include <cstddef>

namespace Catch {

    struct Counts {
        Counts() : passed( 0 ), failed( 0 ) {}

        Counts operator - ( Counts const& other ) const {
            Counts diff;
            diff.passed = passed - other.passed;
            diff.failed = failed - other.failed;
            return diff;
        }
        Counts& operator += ( Counts const& other ) {
            passed += other.passed;
            failed += other.failed;
            return *this;
        }

        std::size_t total() const {
            return passed + failed;
        }

        std::size_t passed;
        std::size_t failed;
    };

    struct Totals {

        Totals operator - ( Totals const& other ) const {
            Totals diff;
            diff.assertions = assertions - other.assertions;
            diff.testCases = testCases - other.testCases;
            return diff;
        }

        Totals delta( Totals const& prevTotals ) const {
            Totals diff = *this - prevTotals;
            if( diff.assertions.failed > 0 )
                ++diff.testCases.failed;
            else
                ++diff.testCases.passed;
            return diff;
        }

        Totals& operator += ( Totals const& other ) {
            assertions += other.assertions;
            testCases += other.testCases;
            return *this;
        }

        Counts assertions;
        Counts testCases;
    };
}

// #included from: catch_config.hpp
#define TWOBLUECUBES_CATCH_CONFIG_HPP_INCLUDED

// #included from: catch_test_spec.h
#define TWOBLUECUBES_CATCH_TEST_SPEC_H_INCLUDED

// #included from: catch_tags.h
#define TWOBLUECUBES_CATCH_TAGS_H_INCLUDED

#include <string>
#include <set>
#include <map>
#include <vector>

#ifdef __clang__
#pragma clang diagnostic ignored "-Wpadded"
#endif

namespace Catch {
    class TagParser {
    public:
        virtual ~TagParser();

        void parse( std::string const& str );

    protected:
        virtual void acceptTag( std::string const& tag ) = 0;
        virtual void acceptChar( char c ) = 0;
        virtual void endParse() {}

    private:
    };

    class TagExtracter : public TagParser {
    public:

        TagExtracter( std::set<std::string>& tags );
        virtual ~TagExtracter();

        void parse( std::string& description );

    private:
        virtual void acceptTag( std::string const& tag );
        virtual void acceptChar( char c );

        TagExtracter& operator=(TagExtracter const&);

        std::set<std::string>& m_tags;
        std::string m_remainder;
    };

    class Tag {
    public:
        Tag();
        Tag( std::string const& name, bool isNegated );
        std::string getName() const;
        bool isNegated() const;
        bool operator ! () const;

    private:
        std::string m_name;
        bool m_isNegated;
    };

    class TagSet {
        typedef std::map<std::string, Tag> TagMap;
    public:
        void add( Tag const& tag );
        bool empty() const;
        bool matches( std::set<std::string> const& tags ) const;

    private:
        TagMap m_tags;
    };

    class TagExpression {
    public:
        bool matches( std::set<std::string> const& tags ) const;

    private:
        friend class TagExpressionParser;

        std::vector<TagSet> m_tagSets;
    };

    class TagExpressionParser : public TagParser {
    public:
        TagExpressionParser( TagExpression& exp );
        ~TagExpressionParser();

    private:
        virtual void acceptTag( std::string const& tag );
        virtual void acceptChar( char c );
        virtual void endParse();

        TagExpressionParser& operator=(TagExpressionParser const&);

        bool m_isNegated;
        TagSet m_currentTagSet;
        TagExpression& m_exp;
    };

} // end namespace Catch

#include <string>
#include <vector>

namespace Catch {

    class TestCase;

    struct IfFilterMatches{ enum DoWhat {
        AutoDetectBehaviour,
        IncludeTests,
        ExcludeTests
    }; };

    class TestCaseFilter {
        enum WildcardPosition {
            NoWildcard = 0,
            WildcardAtStart = 1,
            WildcardAtEnd = 2,
            WildcardAtBothEnds = WildcardAtStart | WildcardAtEnd
        };

    public:
        TestCaseFilter( std::string const& testSpec, IfFilterMatches::DoWhat matchBehaviour = IfFilterMatches::AutoDetectBehaviour );

        IfFilterMatches::DoWhat getFilterType() const;
        bool shouldInclude( TestCase const& testCase ) const;

    private:
        bool isMatch( TestCase const& testCase ) const;

        std::string m_stringToMatch;
        IfFilterMatches::DoWhat m_filterType;
        WildcardPosition m_wildcardPosition;
    };

    class TestCaseFilters {
    public:
        TestCaseFilters( std::string const& name );
        std::string getName() const;
        void addFilter( TestCaseFilter const& filter );
        void addTags( std::string const& tagPattern );
        bool shouldInclude( TestCase const& testCase ) const;

    private:
        std::vector<TagExpression> m_tagExpressions;
        std::vector<TestCaseFilter> m_inclusionFilters;
        std::vector<TestCaseFilter> m_exclusionFilters;
        std::string m_name;
    };

}

// #included from: catch_interfaces_config.h
#define TWOBLUECUBES_CATCH_INTERFACES_CONFIG_H_INCLUDED

#include <iostream>
#include <string>

namespace Catch {

    struct Verbosity { enum Level {
        NoOutput = 0,
        Quiet,
        Normal
    }; };

    struct WarnAbout { enum What {
        Nothing = 0x00,
        NoAssertions = 0x01
    }; };

    struct ShowDurations { enum OrNot {
        DefaultForReporter,
        Always,
        Never
    }; };

    struct IConfig : IShared {

        virtual ~IConfig();

        virtual bool allowThrows() const = 0;
        virtual std::ostream& stream() const = 0;
        virtual std::string name() const = 0;
        virtual bool includeSuccessfulResults() const = 0;
        virtual bool shouldDebugBreak() const = 0;
        virtual bool warnAboutMissingAssertions() const = 0;
        virtual int abortAfter() const = 0;
        virtual ShowDurations::OrNot showDurations() const = 0;
    };
}

// #included from: catch_stream.h
#define TWOBLUECUBES_CATCH_STREAM_H_INCLUDED

#include <streambuf>

#ifdef __clang__
#pragma clang diagnostic ignored "-Wpadded"
#endif

namespace Catch {

    class Stream {
    public:
        Stream();
        Stream( std::streambuf* _streamBuf, bool _isOwned );
        void release();

        std::streambuf* streamBuf;

    private:
        bool isOwned;
    };
}

#include <memory>
#include <vector>
#include <string>
#include <iostream>

#ifndef CATCH_CONFIG_CONSOLE_WIDTH
#define CATCH_CONFIG_CONSOLE_WIDTH 80
#endif

namespace CatchOverrides {

    class ConfigGuard {
    public:
        ConfigGuard()
            : origConfig(Catch::getCurrentContext().getConfig())
        {}
        ~ConfigGuard() {
            Catch::getCurrentMutableContext().setConfig(origConfig);
        }
        const Catch::Ptr<Catch::IConfig const>& value() const {return origConfig;}
    private:
        ConfigGuard(const ConfigGuard&);
        ConfigGuard& operator=(const ConfigGuard&);

        const Catch::Ptr<Catch::IConfig const> origConfig;
    };

    enum OverrideType { OverrideUpdate, OverrideReset}; // Note: ordered; update must be before reset

    template <typename T>
    class ConfigLineData {
        typedef std::map<std::pair<int,OverrideType>, T> LineData;
        typedef std::map<std::string, LineData> FileLineData;
    public:
        T getValueForFileLine(const std::string& file, int line) const {
            T result(false);
            typename FileLineData::const_iterator it = m_data.find(file);
            if( it != m_data.end() ) {
                typename LineData::const_iterator start = it->second.begin();
                typename LineData::const_iterator end = it->second.end();
                for( typename LineData::const_iterator lineIt = it->second.begin(); lineIt != it->second.end(); ++lineIt ) {
                    const std::pair<int,OverrideType>& current = lineIt->first;
                    if( current.second == OverrideReset ) {
                        if( line == current.first ) {
                            result = lineIt->second;
                            end = lineIt;
                            break;
                        }
                        else
                            start = lineIt;
                    }
                }
                for( typename LineData::const_iterator lineIt = start; lineIt != end; ++lineIt ) {
                    const std::pair<int,OverrideType>& current = lineIt->first;
                    if( current.second == OverrideUpdate ) {
                        if( line < current.first )
                            break;
                        result = lineIt->second;
                    }
                }
            }
            return result;
        }
        void recordValueForFileLine(const std::string& file, OverrideType overRide, int line, const T& v)
        {
            typename FileLineData::iterator it = m_data.find(file);
            if( it == m_data.end() ) {
                LineData tmp;
                std::pair<int,OverrideType> current = std::make_pair(line, overRide);
                tmp.insert(std::make_pair(current,v));
                m_data.insert(std::make_pair(file, tmp));
            }
            else {
                std::pair<int,OverrideType> current = std::make_pair(line, overRide);
                typename LineData::iterator lineIt = it->second.find(current);
                if( lineIt == it->second.end() ) {
                    it->second.insert(std::make_pair(current,v));
                }
                else {
                    lineIt->second = v;
                }
            }
        }
    private:
        FileLineData m_data;
    };

    template <typename T>
    class Config {
        typedef std::map<std::pair<int,OverrideType>, bool> BoolLineData;
        typedef std::map<std::string, BoolLineData> FileBoolLineData;
        typedef std::map<std::pair<int,OverrideType>, int> LineData;
        typedef std::map<std::string, LineData> FileLineData;
        typedef std::multimap<std::pair<int,OverrideType>, std::string> StringLineData;
        typedef std::map<std::string, StringLineData> FileStringLineData;
    public:
        bool includeSuccessfulResults(const std::string& file, int line) const {
            return showSuccessfulTestsData.getValueForFileLine(file,line);
        }
        void insertSuccessfulResults(const std::string& file, OverrideType overRide, int line, bool v) {
            showSuccessfulTestsData.recordValueForFileLine(file, overRide, line, v);
        }
        bool warnAboutMissingAssertions(const std::string& file, int line) const {
            return missingAssertionData.getValueForFileLine(file,line);
        }
        void insertMissingAssertions(const std::string& file, OverrideType overRide, int line, bool v) {
            missingAssertionData.recordValueForFileLine(file, overRide, line, v);
        }
        int abortAfter(const std::string& file, int line) const {
            return abortAfterData.getValueForFileLine(file,line);
        }
        void insertAbortAfter(const std::string& file, OverrideType overRide, int line, int v) {
            abortAfterData.recordValueForFileLine(file, overRide, line, v);
        }
        std::vector<std::string> listOfTests(const std::string& file, int c) const {
            std::vector<std::string> result;
            FileStringLineData::const_iterator it = testData.find(file);
            if( it != testData.end() ) {
                StringLineData::const_iterator start = it->second.begin();
                StringLineData::const_iterator end = it->second.end();
                for( StringLineData::const_iterator lineIt = it->second.begin(); lineIt != it->second.end(); ++lineIt ) {
                    const std::pair<int,OverrideType>& current = lineIt->first;
                    if( current.second == OverrideReset ) {
                        if( c == current.first ) {
                            end = lineIt;
                            break;
                        }
                        else
                            start = lineIt;
                    }
                }
                for( StringLineData::const_iterator lineIt = start; lineIt != end; ++lineIt ) {
                    const std::pair<int,OverrideType>& current = lineIt->first;
                    if( current.second == OverrideUpdate ) {
                        if( c < current.first )
                            break;
                        result.push_back(lineIt->second);
                    }
                }
            }
            return result;
        }
        void insertTest(const std::string& file, OverrideType overRide, int c, const std::string& v) {
            FileStringLineData::iterator it = testData.find(file);
            if( it == testData.end() ) {
                StringLineData tmp;
                std::pair<int,OverrideType> current = std::make_pair(c, overRide);
                tmp.insert(std::make_pair(current,v));
                testData.insert(std::make_pair(file, tmp));
            }
            else {
                std::pair<int,OverrideType> current = std::make_pair(c, overRide);
                it->second.insert(std::make_pair(current,v));
            }
        }
        static Config<T>& instance() {
            if( !s_instance ) {
                s_instance = new Config<T>();
            }
            return *s_instance;
        }
    private:
        ConfigLineData<bool> showSuccessfulTestsData;
        ConfigLineData<bool> missingAssertionData;
        ConfigLineData<int> abortAfterData;
        FileStringLineData testData;

        static Config<T>* s_instance;
    };
    template <typename T>
    Config<T>* Config<T>::s_instance = NULL;

    template <typename T>
    struct ConfigReset {
        ConfigReset( const std::string& file, int c, int defaultAbortAfter ) {
            Config<T>::instance().insertSuccessfulResults(file, OverrideReset, c, false);
            Config<T>::instance().insertMissingAssertions(file, OverrideReset, c, false);
            Config<T>::instance().insertAbortAfter(file, OverrideReset, c, defaultAbortAfter);
            Config<T>::instance().insertTest(file, OverrideReset, c, "");
        }
    };

    template <typename T>
    struct ConfigShowSuccessfulTests {
        template <typename U>
        ConfigShowSuccessfulTests( const std::string& file, int c, U v ) {
            Config<T>::instance().insertSuccessfulResults(file, OverrideUpdate, c, v ? true : false);
        }
    };

    template <typename T>
    struct ConfigWarnMissingAssertions {
        template <typename U>
        ConfigWarnMissingAssertions( const std::string& file, int c, U v ) {
            Config<T>::instance().insertMissingAssertions(file, OverrideUpdate, c, v ? true : false);
        }
    };

    template <typename T>
    struct ConfigAbortAfter {
        template <typename U>
        ConfigAbortAfter( const std::string& file, int c, U v ) {
            Config<T>::instance().insertAbortAfter(file, OverrideUpdate, c, v);
        }
    };

    template <typename T>
    struct ConfigAddTest {
        template <typename U>
        ConfigAddTest( const std::string& file, int c, U v ) {
            Config<T>::instance().insertTest(file, OverrideUpdate, c, v);
        }
    };
}

namespace Catch {

    struct ConfigData {

        ConfigData()
        :   listTests( false ),
            listTags( false ),
            listReporters( false ),
            listTestNamesOnly( false ),
            showSuccessfulTests( false ),
            shouldDebugBreak( false ),
            noThrow( false ),
            showHelp( false ),
            abortAfter( -1 ),
            verbosity( Verbosity::Normal ),
            warnings( WarnAbout::Nothing ),
            showDurations( ShowDurations::DefaultForReporter )
        {}

        explicit ConfigData(const IConfig* other)
        :   listTests( false ),
            listTags( false ),
            listReporters( false ),
            showSuccessfulTests( other ? other->includeSuccessfulResults() : false ),
            shouldDebugBreak( false ),
            noThrow( other ? !other->allowThrows() : false ),
            showHelp( false ),
            abortAfter( -1 ),
            verbosity( Verbosity::Normal ),
            warnings( other ? (other->warnAboutMissingAssertions() ? WarnAbout::NoAssertions : WarnAbout::Nothing) : WarnAbout::Nothing ),
            showDurations( other ? other->showDurations() : ShowDurations::DefaultForReporter ),
            name( other ? other->name() : std::string() )
        {}

        bool listTests;
        bool listTags;
        bool listReporters;
        bool listTestNamesOnly;

        bool showSuccessfulTests;
        bool shouldDebugBreak;
        bool noThrow;
        bool showHelp;

        int abortAfter;

        Verbosity::Level verbosity;
        WarnAbout::What warnings;
        ShowDurations::OrNot showDurations;

        std::string reporterName;
        std::string outputFilename;
        std::string name;
        std::string processName;

        std::vector<std::string> testsOrTags;
    };

    class Config : public SharedImpl<IConfig> {
    private:
        Config( Config const& other );
        Config& operator = ( Config const& other );
        virtual void dummy();
    public:

        Config()
        :   m_os( std::cout.rdbuf() )
        {}

        Config( ConfigData const& data )
        :   m_data( data ),
            m_os( std::cout.rdbuf() )
        {
            if( !data.testsOrTags.empty() ) {
                std::string groupName;
                for( std::size_t i = 0; i < data.testsOrTags.size(); ++i ) {
                    if( i != 0 )
                        groupName += " ";
                    groupName += data.testsOrTags[i];
                }
                TestCaseFilters filters( groupName );
                for( std::size_t i = 0; i < data.testsOrTags.size(); ++i ) {
                    std::string filter = data.testsOrTags[i];
                    if( startsWith( filter, "[" ) || startsWith( filter, "~[" ) )
                        filters.addTags( filter );
                    else
                        filters.addFilter( TestCaseFilter( filter ) );
                }
                m_filterSets.push_back( filters );
            }
        }

        virtual ~Config() {
            m_os.rdbuf( std::cout.rdbuf() );
            m_stream.release();
        }

        void setFilename( std::string const& filename ) {
            m_data.outputFilename = filename;
        }

        std::string const& getFilename() const {
            return m_data.outputFilename ;
        }

        bool listTests() const { return m_data.listTests; }
        bool listTestNamesOnly() const { return m_data.listTestNamesOnly; }
        bool listTags() const { return m_data.listTags; }
        bool listReporters() const { return m_data.listReporters; }

        std::string getProcessName() const {
            return m_data.processName;
        }

        bool shouldDebugBreak() const {
            return m_data.shouldDebugBreak;
        }

        void setStreamBuf( std::streambuf* buf ) {
            m_os.rdbuf( buf ? buf : std::cout.rdbuf() );
        }

        void useStream( std::string const& streamName ) {
            Stream stream = createStream( streamName );
            setStreamBuf( stream.streamBuf );
            m_stream.release();
            m_stream = stream;
        }

        std::string getReporterName() const { return m_data.reporterName; }

        void addTestSpec( std::string const& testSpec ) {
            TestCaseFilters filters( testSpec );
            filters.addFilter( TestCaseFilter( testSpec ) );
            m_filterSets.push_back( filters );
        }

        int abortAfter() const {
            return m_data.abortAfter;
        }

        std::vector<TestCaseFilters> const& filters() const {
            return m_filterSets;
        }

        bool showHelp() const { return m_data.showHelp; }

        // IConfig interface
        virtual bool allowThrows() const        { return !m_data.noThrow; }
        virtual std::ostream& stream() const    { return m_os; }
        virtual std::string name() const        { return m_data.name.empty() ? m_data.processName : m_data.name; }
        virtual bool includeSuccessfulResults() const   { return m_data.showSuccessfulTests; }
        virtual bool warnAboutMissingAssertions() const { return m_data.warnings & WarnAbout::NoAssertions; }
        virtual ShowDurations::OrNot showDurations() const { return m_data.showDurations; }

    private:
        ConfigData m_data;

        Stream m_stream;
        mutable std::ostream m_os;
        std::vector<TestCaseFilters> m_filterSets;
    };

} // end namespace Catch

// #included from: catch_test_case_info.h
#define TWOBLUECUBES_CATCH_TEST_CASE_INFO_H_INCLUDED

#include <string>
#include <set>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"
#endif

namespace Catch {

    struct ITestCase;

    struct TestCaseInfo {
        TestCaseInfo(   std::string const& _name,
                        std::string const& _className,
                        std::string const& _description,
                        std::set<std::string> const& _tags,
                        bool _isHidden,
                        SourceLineInfo const& _lineInfo );

        TestCaseInfo( TestCaseInfo const& other );

        std::string name;
        std::string className;
        std::string description;
        std::set<std::string> tags;
        std::string tagsAsString;
        SourceLineInfo lineInfo;
        bool isHidden;
    };

    class TestCase : protected TestCaseInfo {
    public:

        TestCase( ITestCase* testCase, TestCaseInfo const& info );
        TestCase( TestCase const& other );

        TestCase withName( std::string const& _newName ) const;

        void invoke() const;

        TestCaseInfo const& getTestCaseInfo() const;

        bool isHidden() const;
        bool hasTag( std::string const& tag ) const;
        bool matchesTags( std::string const& tagPattern ) const;
        std::set<std::string> const& getTags() const;

        void swap( TestCase& other );
        bool operator == ( TestCase const& other ) const;
        bool operator < ( TestCase const& other ) const;
        TestCase& operator = ( TestCase const& other );

    private:
        Ptr<ITestCase> test;
    };

    TestCase makeTestCase(  ITestCase* testCase,
                            std::string const& className,
                            std::string const& name,
                            std::string const& description,
                            SourceLineInfo const& lineInfo );
}

#ifdef __clang__
#pragma clang diagnostic pop
#endif

// #included from: catch_option.hpp
#define TWOBLUECUBES_CATCH_OPTION_HPP_INCLUDED

namespace Catch {

    // An optional type
    template<typename T>
    class Option {
    public:
        Option() : nullableValue( NULL ) {}
        Option( T const& _value )
        : nullableValue( new( storage ) T( _value ) )
        {}
        Option( Option const& _other )
        : nullableValue( _other ? new( storage ) T( *_other ) : NULL )
        {}

        ~Option() {
            reset();
        }

        Option& operator= ( Option const& _other ) {
            if( &_other != this ) {
                reset();
                if( _other )
                    nullableValue = new( storage ) T( *_other );
            }
            return *this;
        }
        Option& operator = ( T const& _value ) {
            reset();
            nullableValue = new( storage ) T( _value );
            return *this;
        }

        void reset() {
            if( nullableValue )
                nullableValue->~T();
            nullableValue = NULL;
        }

        T& operator*() { return *nullableValue; }
        T const& operator*() const { return *nullableValue; }
        T* operator->() { return nullableValue; }
        const T* operator->() const { return nullableValue; }

        T valueOr( T const& defaultValue ) const {
            return nullableValue ? *nullableValue : defaultValue;
        }

        bool some() const { return nullableValue != NULL; }
        bool none() const { return nullableValue == NULL; }

        bool operator !() const { return nullableValue == NULL; }
        operator SafeBool::type() const {
            return SafeBool::makeSafe( some() );
        }

    private:
        T* nullableValue;
        char storage[sizeof(T)];
    };

} // end namespace Catch

#include <string>
#include <ostream>
#include <map>
#include <assert.h>

namespace Catch
{
    struct ReporterConfig {
        explicit ReporterConfig( Ptr<IConfig> const& _fullConfig )
        :   m_stream( &_fullConfig->stream() ), m_fullConfig( _fullConfig ) {}

        ReporterConfig( Ptr<IConfig> const& _fullConfig, std::ostream& _stream )
        :   m_stream( &_stream ), m_fullConfig( _fullConfig ) {}

        std::ostream& stream() const    { return *m_stream; }
        Ptr<IConfig> fullConfig() const { return m_fullConfig; }

    private:
        std::ostream* m_stream;
        Ptr<IConfig> m_fullConfig;
    };

    struct ReporterPreferences {
        ReporterPreferences()
        : shouldRedirectStdOut( false )
        {}

        bool shouldRedirectStdOut;
    };

    template<typename T>
    struct LazyStat : Option<T> {
        LazyStat() : used( false ) {}
        LazyStat& operator=( T const& _value ) {
            Option<T>::operator=( _value );
            used = false;
            return *this;
        }
        void reset() {
            Option<T>::reset();
            used = false;
        }
        bool used;
    };

    struct TestRunInfo {
        TestRunInfo( std::string const& _name ) : name( _name ) {}
        std::string name;
    };
    struct GroupInfo {
        GroupInfo(  std::string const& _name,
                    std::size_t _groupIndex,
                    std::size_t _groupsCount )
        :   name( _name ),
            groupIndex( _groupIndex ),
            groupsCounts( _groupsCount )
        {}

        std::string name;
        std::size_t groupIndex;
        std::size_t groupsCounts;
    };

    struct AssertionStats {
        AssertionStats( AssertionResult const& _assertionResult,
                        std::vector<MessageInfo> const& _infoMessages,
                        Totals const& _totals )
        :   assertionResult( _assertionResult ),
            infoMessages( _infoMessages ),
            totals( _totals )
        {
            if( assertionResult.hasMessage() ) {
                // Copy message into messages list.
                // !TBD This should have been done earlier, somewhere
                MessageBuilder builder( assertionResult.getTestMacroName(), assertionResult.getSourceInfo(), assertionResult.getResultType() );
                builder << assertionResult.getMessage();
                builder.m_info.message = builder.m_stream.str();

                infoMessages.push_back( builder.m_info );
            }
        }
        virtual ~AssertionStats();

        AssertionResult assertionResult;
        std::vector<MessageInfo> infoMessages;
        Totals totals;
    };

    struct SectionStats {
        SectionStats(   SectionInfo const& _sectionInfo,
                        Counts const& _assertions,
                        double _durationInSeconds,
                        bool _missingAssertions )
        :   sectionInfo( _sectionInfo ),
            assertions( _assertions ),
            durationInSeconds( _durationInSeconds ),
            missingAssertions( _missingAssertions )
        {}
        virtual ~SectionStats();

        SectionInfo sectionInfo;
        Counts assertions;
        double durationInSeconds;
        bool missingAssertions;
    };

    struct TestCaseStats {
        TestCaseStats(  TestCaseInfo const& _testInfo,
                        Totals const& _totals,
                        std::string const& _stdOut,
                        std::string const& _stdErr,
                        bool _aborting )
        : testInfo( _testInfo ),
            totals( _totals ),
            stdOut( _stdOut ),
            stdErr( _stdErr ),
            aborting( _aborting )
        {}
        virtual ~TestCaseStats();

        TestCaseInfo testInfo;
        Totals totals;
        std::string stdOut;
        std::string stdErr;
        bool aborting;
    };

    struct TestGroupStats {
        TestGroupStats( GroupInfo const& _groupInfo,
                        Totals const& _totals,
                        bool _aborting )
        :   groupInfo( _groupInfo ),
            totals( _totals ),
            aborting( _aborting )
        {}
        TestGroupStats( GroupInfo const& _groupInfo )
        :   groupInfo( _groupInfo ),
            aborting( false )
        {}
        virtual ~TestGroupStats();

        GroupInfo groupInfo;
        Totals totals;
        bool aborting;
    };

    struct TestRunStats {
        TestRunStats(   TestRunInfo const& _runInfo,
                        Totals const& _totals,
                        bool _aborting )
        :   runInfo( _runInfo ),
            totals( _totals ),
            aborting( _aborting )
        {}
        TestRunStats( TestRunStats const& _other )
        :   runInfo( _other.runInfo ),
            totals( _other.totals ),
            aborting( _other.aborting )
        {}
        virtual ~TestRunStats();

        TestRunInfo runInfo;
        Totals totals;
        bool aborting;
    };

    struct IStreamingReporter : IShared {
        virtual ~IStreamingReporter();

        // Implementing class must also provide the following static method:
        // static std::string getDescription();

        virtual ReporterPreferences getPreferences() const = 0;

        virtual void noMatchingTestCases( std::string const& spec ) = 0;

        virtual void testRunStarting( TestRunInfo const& testRunInfo ) = 0;
        virtual void testGroupStarting( GroupInfo const& groupInfo ) = 0;

        virtual void testCaseStarting( TestCaseInfo const& testInfo ) = 0;
        virtual void sectionStarting( SectionInfo const& sectionInfo ) = 0;

        virtual void assertionStarting( AssertionInfo const& assertionInfo ) = 0;

        virtual bool assertionEnded( AssertionStats const& assertionStats ) = 0;
        virtual void sectionEnded( SectionStats const& sectionStats ) = 0;
        virtual void testCaseEnded( TestCaseStats const& testCaseStats ) = 0;
        virtual void testGroupEnded( TestGroupStats const& testGroupStats ) = 0;
        virtual void testRunEnded( TestRunStats const& testRunStats ) = 0;
    };

    struct IReporterFactory {
        virtual ~IReporterFactory();
        virtual IStreamingReporter* create( ReporterConfig const& config ) const = 0;
        virtual std::string getDescription() const = 0;
    };

    struct IReporterRegistry {
        typedef std::map<std::string, IReporterFactory*> FactoryMap;

        virtual ~IReporterRegistry();
        virtual IStreamingReporter* create( std::string const& name, Ptr<IConfig> const& config ) const = 0;
        virtual FactoryMap const& getFactories() const = 0;
    };

}

#include <ostream>

namespace Catch {

    inline IResultCapture& getResultCapture() {
        return getCurrentContext().getResultCapture();
    }

    template<typename MatcherT>
    ExpressionResultBuilder expressionResultBuilderFromMatcher( MatcherT const& matcher,
                                                                std::string const& matcherCallAsString ) {
        std::string matcherAsString = matcher.toString();
        if( matcherAsString == "{?}" )
            matcherAsString = matcherCallAsString;
        return ExpressionResultBuilder()
            .setRhs( matcherAsString )
            .setOp( "matches" );
    }

    template<typename MatcherT, typename ArgT>
    ExpressionResultBuilder expressionResultBuilderFromMatcher( MatcherT const& matcher,
                                                                ArgT const& arg,
                                                                std::string const& matcherCallAsString ) {
        return expressionResultBuilderFromMatcher( matcher, matcherCallAsString )
            .setLhs( Catch::toString( arg ) )
            .setResultType( matcher.match( arg ) );
    }

    template<typename MatcherT, typename ArgT>
    ExpressionResultBuilder expressionResultBuilderFromMatcher( MatcherT const& matcher,
                                                                ArgT* arg,
                                                                std::string const& matcherCallAsString ) {
        return expressionResultBuilderFromMatcher( matcher, matcherCallAsString )
            .setLhs( Catch::toString( arg ) )
            .setResultType( matcher.match( arg ) );
    }

    struct TestFailureException{};

} // end namespace Catch

///////////////////////////////////////////////////////////////////////////////
#define INTERNAL_CATCH_ASSERTIONINFO_NAME INTERNAL_CATCH_UNIQUE_NAME( __assertionInfo )

#if !defined(INTERNAL_CATCH_VS_MANAGED) && !defined(INTERNAL_CATCH_VS_NATIVE)

    // normal Catch
    #define INTERNAL_CATCH_TEST_THROW_FAILURE throw Catch::TestFailureException();

#else   // VS integration

#if defined(INTERNAL_CATCH_VS_MANAGED)

    #define INTERNAL_CATCH_TEST_THROW_FAILURE \
    { \
        Catch::IResultCapture& cap = Catch::getResultCapture(); \
        const Catch::AssertionResult* r = cap.getLastResult(); \
        std::stringstream _sf; \
        _sf << r->getExpressionInMacro().c_str() << ", " << r->getMessage().c_str(); \
        std::string fail = _sf.str(); \
        Assert::Fail(Catch::convert_string_for_assert(fail)); \
    }

#else

#if defined(INTERNAL_CATCH_VS_NATIVE)
    #define INTERNAL_CATCH_TEST_THROW_FAILURE \
    { \
        Catch::IResultCapture& cap = Catch::getResultCapture(); \
        const Catch::AssertionResult* r = cap.getLastResult(); \
        std::wstringstream _s; \
        _s << r->getSourceInfo().file.c_str(); \
        std::wstring ws = _s.str(); \
        std::string testName = cap.getCurrentTestName(); \
        __LineInfo li(ws.c_str(), testName.c_str(), r->getSourceInfo().line); \
        std::wstringstream _sf; \
        _sf << r->getExpandedExpression().c_str() << ", " << r->getMessage().c_str(); \
        std::wstring ws2 = _sf.str(); \
        Assert::Fail(ws2.c_str(), &li); \
    }

#endif // INTERNAL_CATCH_VS_MANAGED

#endif // detect CLR

#endif   // VS integration

///////////////////////////////////////////////////////////////////////////////
#define INTERNAL_CATCH_ACCEPT_EXPR( evaluatedExpr, resultDisposition, originalExpr ) \
    if( Catch::ResultAction::Value internal_catch_action = Catch::getResultCapture().acceptExpression( evaluatedExpr, INTERNAL_CATCH_ASSERTIONINFO_NAME )  ) { \
        if( internal_catch_action & Catch::ResultAction::Debug ) CATCH_BREAK_INTO_DEBUGGER(); \
        if( internal_catch_action & Catch::ResultAction::Abort ) { INTERNAL_CATCH_TEST_THROW_FAILURE } \
        if( !Catch::shouldContinueOnFailure( resultDisposition ) ) { throw Catch::TestFailureException(); } \
        Catch::isTrue( false && originalExpr ); \
    }

///////////////////////////////////////////////////////////////////////////////
#define INTERNAL_CATCH_ACCEPT_INFO( expr, macroName, resultDisposition ) \
    Catch::AssertionInfo INTERNAL_CATCH_ASSERTIONINFO_NAME( macroName, CATCH_INTERNAL_LINEINFO, expr, resultDisposition );

///////////////////////////////////////////////////////////////////////////////
#define INTERNAL_CATCH_TEST( expr, resultDisposition, macroName ) \
    do { \
        INTERNAL_CATCH_ACCEPT_INFO( #expr, macroName, resultDisposition ); \
        try { \
            INTERNAL_CATCH_ACCEPT_EXPR( ( Catch::ExpressionDecomposer()->*expr ).endExpression( resultDisposition ), resultDisposition, expr ); \
        } catch( const Catch::TestFailureException& ) { \
            throw; \
        } catch( ... ) { \
            INTERNAL_CATCH_ACCEPT_EXPR( Catch::ExpressionResultBuilder( Catch::ResultWas::ThrewException ) << Catch::translateActiveException(), \
                Catch::ResultDisposition::Normal, expr ); \
        } \
    } while( Catch::isTrue( false ) )

///////////////////////////////////////////////////////////////////////////////
#define INTERNAL_CATCH_IF( expr, resultDisposition, macroName ) \
    INTERNAL_CATCH_TEST( expr, resultDisposition, macroName ); \
    if( Catch::getResultCapture().getLastResult()->succeeded() )

///////////////////////////////////////////////////////////////////////////////
#define INTERNAL_CATCH_ELSE( expr, resultDisposition, macroName ) \
    INTERNAL_CATCH_TEST( expr, resultDisposition, macroName ); \
    if( !Catch::getResultCapture().getLastResult()->succeeded() )

///////////////////////////////////////////////////////////////////////////////
#define INTERNAL_CATCH_NO_THROW( expr, resultDisposition, macroName ) \
    do { \
        INTERNAL_CATCH_ACCEPT_INFO( #expr, macroName, resultDisposition ); \
        try { \
            expr; \
            INTERNAL_CATCH_ACCEPT_EXPR( Catch::ExpressionResultBuilder( Catch::ResultWas::Ok ), resultDisposition, false ); \
        } \
        catch( ... ) { \
            INTERNAL_CATCH_ACCEPT_EXPR( Catch::ExpressionResultBuilder( Catch::ResultWas::ThrewException ) << Catch::translateActiveException(), resultDisposition, false ); \
        } \
} while( Catch::isTrue( false ) )

///////////////////////////////////////////////////////////////////////////////
#define INTERNAL_CATCH_THROWS_IMPL( expr, exceptionType, resultDisposition ) \
    try { \
        if( Catch::getCurrentContext().getConfig()->allowThrows() ) { \
            expr; \
            INTERNAL_CATCH_ACCEPT_EXPR( Catch::ExpressionResultBuilder( Catch::ResultWas::DidntThrowException ), resultDisposition, false ); \
        } \
    } \
    catch( const Catch::TestFailureException& ) { \
        throw; \
    } \
    catch( exceptionType ) { \
        INTERNAL_CATCH_ACCEPT_EXPR( Catch::ExpressionResultBuilder( Catch::ResultWas::Ok ), resultDisposition, false ); \
    }

///////////////////////////////////////////////////////////////////////////////
#define INTERNAL_CATCH_THROWS( expr, exceptionType, resultDisposition, macroName ) \
    do { \
        INTERNAL_CATCH_ACCEPT_INFO( #expr, macroName, resultDisposition ); \
        INTERNAL_CATCH_THROWS_IMPL( expr, exceptionType, resultDisposition ) \
    } while( Catch::isTrue( false ) )

///////////////////////////////////////////////////////////////////////////////
#define INTERNAL_CATCH_THROWS_AS( expr, exceptionType, resultDisposition, macroName ) \
    do { \
        INTERNAL_CATCH_ACCEPT_INFO( #expr, macroName, resultDisposition ); \
        INTERNAL_CATCH_THROWS_IMPL( expr, exceptionType, resultDisposition ) \
        catch( ... ) { \
            INTERNAL_CATCH_ACCEPT_EXPR( ( Catch::ExpressionResultBuilder( Catch::ResultWas::ThrewException ) << Catch::translateActiveException() ), \
                resultDisposition | Catch::ResultDisposition::ContinueOnFailure, false ); \
        } \
    } while( Catch::isTrue( false ) )

///////////////////////////////////////////////////////////////////////////////
#ifdef CATCH_CONFIG_VARIADIC_MACROS
    #define INTERNAL_CATCH_MSG( messageType, resultDisposition, macroName, ... ) \
        do { \
            INTERNAL_CATCH_ACCEPT_INFO( "", macroName, resultDisposition ); \
            INTERNAL_CATCH_ACCEPT_EXPR( Catch::ExpressionResultBuilder( messageType ) << __VA_ARGS__ +::Catch::StreamEndStop(), resultDisposition, true ) \
        } while( Catch::isTrue( false ) )
#else
    #define INTERNAL_CATCH_MSG( messageType, resultDisposition, macroName, log ) \
        do { \
            INTERNAL_CATCH_ACCEPT_INFO( "", macroName, resultDisposition ); \
            INTERNAL_CATCH_ACCEPT_EXPR( Catch::ExpressionResultBuilder( messageType ) << log, resultDisposition, true ) \
        } while( Catch::isTrue( false ) )
#endif

///////////////////////////////////////////////////////////////////////////////
#define INTERNAL_CATCH_INFO( log, macroName ) \
    Catch::ScopedMessage INTERNAL_CATCH_UNIQUE_NAME( scopedMessage ) = Catch::MessageBuilder( macroName, CATCH_INTERNAL_LINEINFO, Catch::ResultWas::Info ) << log;

///////////////////////////////////////////////////////////////////////////////
#define INTERNAL_CHECK_THAT( arg, matcher, resultDisposition, macroName ) \
    do { \
        INTERNAL_CATCH_ACCEPT_INFO( #arg " " #matcher, macroName, resultDisposition ); \
        try { \
            INTERNAL_CATCH_ACCEPT_EXPR( ( Catch::expressionResultBuilderFromMatcher( ::Catch::Matchers::matcher, arg, #matcher ) ), resultDisposition, false ); \
        } catch( const Catch::TestFailureException& ) { \
            throw; \
        } catch( ... ) { \
            INTERNAL_CATCH_ACCEPT_EXPR( ( Catch::ExpressionResultBuilder( Catch::ResultWas::ThrewException ) << Catch::translateActiveException() ), \
                resultDisposition | Catch::ResultDisposition::ContinueOnFailure, false ); \
        } \
    } while( Catch::isTrue( false ) )

// #included from: internal/catch_section.h
#define TWOBLUECUBES_CATCH_SECTION_H_INCLUDED

// #included from: catch_timer.h
#define TWOBLUECUBES_CATCH_TIMER_H_INCLUDED

#ifdef CATCH_PLATFORM_WINDOWS
typedef unsigned long long uint64_t;
#else
#include <stdint.h>
#endif

namespace Catch {

    class Timer {
    public:
        Timer() : m_ticks( 0 ) {}
        void start();
        unsigned int getElapsedNanoseconds() const;
        unsigned int getElapsedMilliseconds() const;
        double getElapsedSeconds() const;

    private:
        uint64_t m_ticks;
    };

} // namespace Catch

#include <string>

namespace Catch {

    class Section {
    public:
        Section(    SourceLineInfo const& lineInfo,
                    std::string const& name,
                    std::string const& description = "" );
        ~Section();

        // This indicates whether the section should be executed or not
        operator bool();

    private:

        SectionInfo m_info;

        std::string m_name;
        Counts m_assertions;
        bool m_sectionIncluded;
        Timer m_timer;
    };

} // end namespace Catch

#ifdef CATCH_CONFIG_VARIADIC_MACROS
    #define INTERNAL_CATCH_SECTION( ... ) \
        if( Catch::Section INTERNAL_CATCH_UNIQUE_NAME( catch_internal_Section ) = Catch::Section( CATCH_INTERNAL_LINEINFO, __VA_ARGS__ ) )
#else
    #define INTERNAL_CATCH_SECTION( name, desc ) \
        if( Catch::Section INTERNAL_CATCH_UNIQUE_NAME( catch_internal_Section ) = Catch::Section( CATCH_INTERNAL_LINEINFO, name, desc ) )
#endif

// #included from: internal/catch_generators.hpp
#define TWOBLUECUBES_CATCH_GENERATORS_HPP_INCLUDED

#include <iterator>
#include <vector>
#include <string>
#include <stdlib.h>

namespace Catch {

template<typename T>
struct IGenerator {
    virtual ~IGenerator() {}
    virtual T getValue( std::size_t index ) const = 0;
    virtual std::size_t size () const = 0;
};

template<typename T>
class BetweenGenerator : public IGenerator<T> {
public:
    BetweenGenerator( T from, T to ) : m_from( from ), m_to( to ){}

    virtual T getValue( std::size_t index ) const {
        return m_from+static_cast<int>( index );
    }

    virtual std::size_t size() const {
        return static_cast<std::size_t>( 1+m_to-m_from );
    }

private:

    T m_from;
    T m_to;
};

template<typename T>
class ValuesGenerator : public IGenerator<T> {
public:
    ValuesGenerator(){}

    void add( T value ) {
        m_values.push_back( value );
    }

    virtual T getValue( std::size_t index ) const {
        return m_values[index];
    }

    virtual std::size_t size() const {
        return m_values.size();
    }

private:
    std::vector<T> m_values;
};

template<typename T>
class CompositeGenerator {
public:
    CompositeGenerator() : m_totalSize( 0 ) {}

    // *** Move semantics, similar to auto_ptr ***
    CompositeGenerator( CompositeGenerator& other )
    :   m_fileInfo( other.m_fileInfo ),
        m_totalSize( 0 )
    {
        move( other );
    }

    CompositeGenerator& setFileInfo( const char* fileInfo ) {
        m_fileInfo = fileInfo;
        return *this;
    }

    ~CompositeGenerator() {
        deleteAll( m_composed );
    }

    operator T () const {
        size_t overallIndex = getCurrentContext().getGeneratorIndex( m_fileInfo, m_totalSize );

        typename std::vector<const IGenerator<T>*>::const_iterator it = m_composed.begin();
        typename std::vector<const IGenerator<T>*>::const_iterator itEnd = m_composed.end();
        for( size_t index = 0; it != itEnd; ++it )
        {
            const IGenerator<T>* generator = *it;
            if( overallIndex >= index && overallIndex < index + generator->size() )
            {
                return generator->getValue( overallIndex-index );
            }
            index += generator->size();
        }
        CATCH_INTERNAL_ERROR( "Indexed past end of generated range" );
        return T(); // Suppress spurious "not all control paths return a value" warning in Visual Studio - if you know how to fix this please do so
    }

    void add( const IGenerator<T>* generator ) {
        m_totalSize += generator->size();
        m_composed.push_back( generator );
    }

    CompositeGenerator& then( CompositeGenerator& other ) {
        move( other );
        return *this;
    }

    CompositeGenerator& then( T value ) {
        ValuesGenerator<T>* valuesGen = new ValuesGenerator<T>();
        valuesGen->add( value );
        add( valuesGen );
        return *this;
    }

private:

    void move( CompositeGenerator& other ) {
        std::copy( other.m_composed.begin(), other.m_composed.end(), std::back_inserter( m_composed ) );
        m_totalSize += other.m_totalSize;
        other.m_composed.clear();
    }

    std::vector<const IGenerator<T>*> m_composed;
    std::string m_fileInfo;
    size_t m_totalSize;
};

namespace Generators
{
    template<typename T>
    CompositeGenerator<T> between( T from, T to ) {
        CompositeGenerator<T> generators;
        generators.add( new BetweenGenerator<T>( from, to ) );
        return generators;
    }

    template<typename T>
    CompositeGenerator<T> values( T val1, T val2 ) {
        CompositeGenerator<T> generators;
        ValuesGenerator<T>* valuesGen = new ValuesGenerator<T>();
        valuesGen->add( val1 );
        valuesGen->add( val2 );
        generators.add( valuesGen );
        return generators;
    }

    template<typename T>
    CompositeGenerator<T> values( T val1, T val2, T val3 ){
        CompositeGenerator<T> generators;
        ValuesGenerator<T>* valuesGen = new ValuesGenerator<T>();
        valuesGen->add( val1 );
        valuesGen->add( val2 );
        valuesGen->add( val3 );
        generators.add( valuesGen );
        return generators;
    }

    template<typename T>
    CompositeGenerator<T> values( T val1, T val2, T val3, T val4 ) {
        CompositeGenerator<T> generators;
        ValuesGenerator<T>* valuesGen = new ValuesGenerator<T>();
        valuesGen->add( val1 );
        valuesGen->add( val2 );
        valuesGen->add( val3 );
        valuesGen->add( val4 );
        generators.add( valuesGen );
        return generators;
    }

} // end namespace Generators

using namespace Generators;

} // end namespace Catch

#define INTERNAL_CATCH_LINESTR2( line ) #line
#define INTERNAL_CATCH_LINESTR( line ) INTERNAL_CATCH_LINESTR2( line )

#define INTERNAL_CATCH_GENERATE( expr ) expr.setFileInfo( __FILE__ "(" INTERNAL_CATCH_LINESTR( __LINE__ ) ")" )

// #included from: internal/catch_interfaces_exception.h
#define TWOBLUECUBES_CATCH_INTERFACES_EXCEPTION_H_INCLUDED

#include <string>
// #included from: catch_interfaces_registry_hub.h
#define TWOBLUECUBES_CATCH_INTERFACES_REGISTRY_HUB_H_INCLUDED

#include <string>

namespace Catch {

    class TestCase;
    struct ITestCaseRegistry;
    struct IExceptionTranslatorRegistry;
    struct IExceptionTranslator;
    struct IReporterRegistry;
    struct IReporterFactory;

    struct IRegistryHub {
        virtual ~IRegistryHub();

        virtual IReporterRegistry const& getReporterRegistry() const = 0;
        virtual ITestCaseRegistry const& getTestCaseRegistry() const = 0;
        virtual IExceptionTranslatorRegistry& getExceptionTranslatorRegistry() = 0;
    };

    struct IMutableRegistryHub {
        virtual ~IMutableRegistryHub();
        virtual void registerReporter( std::string const& name, IReporterFactory* factory ) = 0;
        virtual void registerTest( TestCase const& testInfo ) = 0;
        virtual void registerTranslator( const IExceptionTranslator* translator ) = 0;
    };

    IRegistryHub& getRegistryHub();
    IMutableRegistryHub& getMutableRegistryHub();
    void cleanUp();
    std::string translateActiveException();

}


namespace Catch {

    typedef std::string(*exceptionTranslateFunction)();

    struct IExceptionTranslator {
        virtual ~IExceptionTranslator();
        virtual std::string translate() const = 0;
    };

    struct IExceptionTranslatorRegistry {
        virtual ~IExceptionTranslatorRegistry();

        virtual std::string translateActiveException() const = 0;
    };

    class ExceptionTranslatorRegistrar {
        template<typename T>
        class ExceptionTranslator : public IExceptionTranslator {
        public:

            ExceptionTranslator( std::string(*translateFunction)( T& ) )
            : m_translateFunction( translateFunction )
            {}

            virtual std::string translate() const {
                try {
                    throw;
                }
                catch( T& ex ) {
                    return m_translateFunction( ex );
                }
            }

        protected:
            std::string(*m_translateFunction)( T& );
        };

    public:
        template<typename T>
        ExceptionTranslatorRegistrar( std::string(*translateFunction)( T& ) ) {
            getMutableRegistryHub().registerTranslator
                ( new ExceptionTranslator<T>( translateFunction ) );
        }
    };
}

///////////////////////////////////////////////////////////////////////////////
#define INTERNAL_CATCH_TRANSLATE_EXCEPTION( signature ) \
    static std::string INTERNAL_CATCH_UNIQUE_NAME( catch_internal_ExceptionTranslator )( signature ); \
    namespace{ Catch::ExceptionTranslatorRegistrar INTERNAL_CATCH_UNIQUE_NAME( catch_internal_ExceptionRegistrar )( &INTERNAL_CATCH_UNIQUE_NAME( catch_internal_ExceptionTranslator ) ); }\
    static std::string INTERNAL_CATCH_UNIQUE_NAME(  catch_internal_ExceptionTranslator )( signature )

// #included from: internal/catch_approx.hpp
#define TWOBLUECUBES_CATCH_APPROX_HPP_INCLUDED

#include <cmath>
#include <limits>

namespace Catch {
namespace Detail {

    class Approx {
    public:
        explicit Approx ( double value )
        :   m_epsilon( std::numeric_limits<float>::epsilon()*100 ),
            m_scale( 1.0 ),
            m_value( value )
        {}

        Approx( Approx const& other )
        :   m_epsilon( other.m_epsilon ),
            m_scale( other.m_scale ),
            m_value( other.m_value )
        {}

        static Approx custom() {
            return Approx( 0 );
        }

        Approx operator()( double value ) {
            Approx approx( value );
            approx.epsilon( m_epsilon );
            approx.scale( m_scale );
            return approx;
        }

        friend bool operator == ( double lhs, Approx const& rhs ) {
            // Thanks to Richard Harris for his help refining this formula
            return fabs( lhs - rhs.m_value ) < rhs.m_epsilon * (rhs.m_scale + (std::max)( fabs(lhs), fabs(rhs.m_value) ) );
        }

        friend bool operator == ( Approx const& lhs, double rhs ) {
            return operator==( rhs, lhs );
        }

        friend bool operator != ( double lhs, Approx const& rhs ) {
            return !operator==( lhs, rhs );
        }

        friend bool operator != ( Approx const& lhs, double rhs ) {
            return !operator==( rhs, lhs );
        }

        Approx& epsilon( double newEpsilon ) {
            m_epsilon = newEpsilon;
            return *this;
        }

        Approx& scale( double newScale ) {
            m_scale = newScale;
            return *this;
        }

        std::string toString() const {
            std::ostringstream oss;
            oss << "Approx( " << Catch::toString( m_value ) << " )";
            return oss.str();
        }

    private:
        double m_epsilon;
        double m_scale;
        double m_value;
    };
}

template<>
inline std::string toString<Detail::Approx>( Detail::Approx const& value ) {
    return value.toString();
}

} // end namespace Catch

// #included from: internal/catch_matchers.hpp
#define TWOBLUECUBES_CATCH_MATCHERS_HPP_INCLUDED

namespace Catch {
namespace Matchers {
    namespace Impl {

    template<typename ExpressionT>
    struct Matcher : SharedImpl<IShared>
    {
        typedef ExpressionT ExpressionType;

        virtual ~Matcher() {}
        virtual Ptr<Matcher> clone() const = 0;
        virtual bool match( ExpressionT const& expr ) const = 0;
        virtual std::string toString() const = 0;
    };

    template<typename DerivedT, typename ExpressionT>
    struct MatcherImpl : Matcher<ExpressionT> {

        virtual Ptr<Matcher<ExpressionT> > clone() const {
            return Ptr<Matcher<ExpressionT> >( new DerivedT( static_cast<DerivedT const&>( *this ) ) );
        }
    };

    namespace Generic {

        template<typename ExpressionT>
        class AllOf : public MatcherImpl<AllOf<ExpressionT>, ExpressionT> {
        public:

            AllOf() {}
            AllOf( AllOf const& other ) : m_matchers( other.m_matchers ) {}

            AllOf& add( Matcher<ExpressionT> const& matcher ) {
                m_matchers.push_back( matcher.clone() );
                return *this;
            }
            virtual bool match( ExpressionT const& expr ) const
            {
                for( std::size_t i = 0; i < m_matchers.size(); ++i )
                    if( !m_matchers[i]->match( expr ) )
                        return false;
                return true;
            }
            virtual std::string toString() const {
                std::ostringstream oss;
                oss << "( ";
                for( std::size_t i = 0; i < m_matchers.size(); ++i ) {
                    if( i != 0 )
                        oss << " and ";
                    oss << m_matchers[i]->toString();
                }
                oss << " )";
                return oss.str();
            }

        private:
            std::vector<Ptr<Matcher<ExpressionT> > > m_matchers;
        };

        template<typename ExpressionT>
        class AnyOf : public MatcherImpl<AnyOf<ExpressionT>, ExpressionT> {
        public:

            AnyOf() {}
            AnyOf( AnyOf const& other ) : m_matchers( other.m_matchers ) {}

            AnyOf& add( Matcher<ExpressionT> const& matcher ) {
                m_matchers.push_back( matcher.clone() );
                return *this;
            }
            virtual bool match( ExpressionT const& expr ) const
            {
                for( std::size_t i = 0; i < m_matchers.size(); ++i )
                    if( m_matchers[i]->match( expr ) )
                        return true;
                return false;
            }
            virtual std::string toString() const {
                std::ostringstream oss;
                oss << "( ";
                for( std::size_t i = 0; i < m_matchers.size(); ++i ) {
                    if( i != 0 )
                        oss << " or ";
                    oss << m_matchers[i]->toString();
                }
                oss << " )";
                return oss.str();
            }

        private:
            std::vector<Ptr<Matcher<ExpressionT> > > m_matchers;
        };

    }

    namespace StdString {

        inline std::string makeString( std::string const& str ) { return str; }
        inline std::string makeString( const char* str ) { return str ? std::string( str ) : std::string(); }

        struct Equals : MatcherImpl<Equals, std::string> {
            Equals( std::string const& str ) : m_str( str ){}
            Equals( Equals const& other ) : m_str( other.m_str ){}

            virtual ~Equals();

            virtual bool match( std::string const& expr ) const {
                return m_str == expr;
            }
            virtual std::string toString() const {
                return "equals: \"" + m_str + "\"";
            }

            std::string m_str;
        };

        struct Contains : MatcherImpl<Contains, std::string> {
            Contains( std::string const& substr ) : m_substr( substr ){}
            Contains( Contains const& other ) : m_substr( other.m_substr ){}

            virtual ~Contains();

            virtual bool match( std::string const& expr ) const {
                return expr.find( m_substr ) != std::string::npos;
            }
            virtual std::string toString() const {
                return "contains: \"" + m_substr + "\"";
            }

            std::string m_substr;
        };

        struct StartsWith : MatcherImpl<StartsWith, std::string> {
            StartsWith( std::string const& substr ) : m_substr( substr ){}
            StartsWith( StartsWith const& other ) : m_substr( other.m_substr ){}

            virtual ~StartsWith();

            virtual bool match( std::string const& expr ) const {
                return expr.find( m_substr ) == 0;
            }
            virtual std::string toString() const {
                return "starts with: \"" + m_substr + "\"";
            }

            std::string m_substr;
        };

        struct EndsWith : MatcherImpl<EndsWith, std::string> {
            EndsWith( std::string const& substr ) : m_substr( substr ){}
            EndsWith( EndsWith const& other ) : m_substr( other.m_substr ){}

            virtual ~EndsWith();

            virtual bool match( std::string const& expr ) const {
                return expr.find( m_substr ) == expr.size() - m_substr.size();
            }
            virtual std::string toString() const {
                return "ends with: \"" + m_substr + "\"";
            }

            std::string m_substr;
        };
    } // namespace StdString
    } // namespace Impl

    // The following functions create the actual matcher objects.
    // This allows the types to be inferred
    template<typename ExpressionT>
    inline Impl::Generic::AllOf<ExpressionT> AllOf( Impl::Matcher<ExpressionT> const& m1,
                                                    Impl::Matcher<ExpressionT> const& m2 ) {
        return Impl::Generic::AllOf<ExpressionT>().add( m1 ).add( m2 );
    }
    template<typename ExpressionT>
    inline Impl::Generic::AllOf<ExpressionT> AllOf( Impl::Matcher<ExpressionT> const& m1,
                                                    Impl::Matcher<ExpressionT> const& m2,
                                                    Impl::Matcher<ExpressionT> const& m3 ) {
        return Impl::Generic::AllOf<ExpressionT>().add( m1 ).add( m2 ).add( m3 );
    }
    template<typename ExpressionT>
    inline Impl::Generic::AnyOf<ExpressionT> AnyOf( Impl::Matcher<ExpressionT> const& m1,
                                                    Impl::Matcher<ExpressionT> const& m2 ) {
        return Impl::Generic::AnyOf<ExpressionT>().add( m1 ).add( m2 );
    }
    template<typename ExpressionT>
    inline Impl::Generic::AnyOf<ExpressionT> AnyOf( Impl::Matcher<ExpressionT> const& m1,
                                                    Impl::Matcher<ExpressionT> const& m2,
                                                    Impl::Matcher<ExpressionT> const& m3 ) {
        return Impl::Generic::AnyOf<ExpressionT>().add( m1 ).add( m2 ).add( m3 );
    }

    inline Impl::StdString::Equals      Equals( std::string const& str ) {
        return Impl::StdString::Equals( str );
    }
    inline Impl::StdString::Equals      Equals( const char* str ) {
        return Impl::StdString::Equals( Impl::StdString::makeString( str ) );
    }
    inline Impl::StdString::Contains    Contains( std::string const& substr ) {
        return Impl::StdString::Contains( substr );
    }
    inline Impl::StdString::Contains    Contains( const char* substr ) {
        return Impl::StdString::Contains( Impl::StdString::makeString( substr ) );
    }
    inline Impl::StdString::StartsWith  StartsWith( std::string const& substr ) {
        return Impl::StdString::StartsWith( substr );
    }
    inline Impl::StdString::StartsWith  StartsWith( const char* substr ) {
        return Impl::StdString::StartsWith( Impl::StdString::makeString( substr ) );
    }
    inline Impl::StdString::EndsWith    EndsWith( std::string const& substr ) {
        return Impl::StdString::EndsWith( substr );
    }
    inline Impl::StdString::EndsWith    EndsWith( const char* substr ) {
        return Impl::StdString::EndsWith( Impl::StdString::makeString( substr ) );
    }

} // namespace Matchers

using namespace Matchers;

} // namespace Catch

// These files are included here so the single_include script doesn't put them
// in the conditionally compiled sections
// #included from: internal/catch_interfaces_runner.h
#define TWOBLUECUBES_CATCH_INTERFACES_RUNNER_H_INCLUDED

namespace Catch {
    class TestCase;

    struct IRunner {
        virtual ~IRunner();
    };

    struct NullRunner : public IRunner
    {
    };
}


#ifdef __OBJC__
// #included from: internal/catch_objc.hpp
#define TWOBLUECUBES_CATCH_OBJC_HPP_INCLUDED

#import <objc/runtime.h>

#include <string>

// NB. Any general catch headers included here must be included
// in catch.hpp first to make sure they are included by the single
// header for non obj-usage

///////////////////////////////////////////////////////////////////////////////
// This protocol is really only here for (self) documenting purposes, since
// all its methods are optional.
@protocol OcFixture

@optional

-(void) setUp;
-(void) tearDown;

@end

namespace Catch {

    class OcMethod : public SharedImpl<ITestCase> {

    public:
        OcMethod( Class cls, SEL sel ) : m_cls( cls ), m_sel( sel ) {}

        virtual void invoke() const {
            id obj = [[m_cls alloc] init];

            performOptionalSelector( obj, @selector(setUp)  );
            performOptionalSelector( obj, m_sel );
            performOptionalSelector( obj, @selector(tearDown)  );

            arcSafeRelease( obj );
        }
    private:
        virtual ~OcMethod() {}

        Class m_cls;
        SEL m_sel;
    };

    namespace Detail{

        inline std::string getAnnotation(   Class cls,
                                            std::string const& annotationName,
                                            std::string const& testCaseName ) {
            NSString* selStr = [[NSString alloc] initWithFormat:@"Catch_%s_%s", annotationName.c_str(), testCaseName.c_str()];
            SEL sel = NSSelectorFromString( selStr );
            arcSafeRelease( selStr );
            id value = performOptionalSelector( cls, sel );
            if( value )
                return [(NSString*)value UTF8String];
            return "";
        }
    }

    inline size_t registerTestMethods() {
        size_t noTestMethods = 0;
        int noClasses = objc_getClassList( NULL, 0 );

        Class* classes = (CATCH_UNSAFE_UNRETAINED Class *)malloc( sizeof(Class) * noClasses);
        objc_getClassList( classes, noClasses );

        for( int c = 0; c < noClasses; c++ ) {
            Class cls = classes[c];
            {
                u_int count;
                Method* methods = class_copyMethodList( cls, &count );
                for( u_int m = 0; m < count ; m++ ) {
                    SEL selector = method_getName(methods[m]);
                    std::string methodName = sel_getName(selector);
                    if( startsWith( methodName, "Catch_TestCase_" ) ) {
                        std::string testCaseName = methodName.substr( 15 );
                        std::string name = Detail::getAnnotation( cls, "Name", testCaseName );
                        std::string desc = Detail::getAnnotation( cls, "Description", testCaseName );
                        const char* className = class_getName( cls );

                        getMutableRegistryHub().registerTest( makeTestCase( new OcMethod( cls, selector ), className, name.c_str(), desc.c_str(), SourceLineInfo() ) );
                        noTestMethods++;
                    }
                }
                free(methods);
            }
        }
        return noTestMethods;
    }

    namespace Matchers {
        namespace Impl {
        namespace NSStringMatchers {

            template<typename MatcherT>
            struct StringHolder : MatcherImpl<MatcherT, NSString*>{
                StringHolder( NSString* substr ) : m_substr( [substr copy] ){}
                StringHolder( StringHolder const& other ) : m_substr( [other.m_substr copy] ){}
                StringHolder() {
                    arcSafeRelease( m_substr );
                }

                NSString* m_substr;
            };

            struct Equals : StringHolder<Equals> {
                Equals( NSString* substr ) : StringHolder( substr ){}

                virtual bool match( ExpressionType const& str ) const {
                    return  (str != nil || m_substr == nil ) &&
                            [str isEqualToString:m_substr];
                }

                virtual std::string toString() const {
                    return "equals string: \"" + Catch::toString( m_substr ) + "\"";
                }
            };

            struct Contains : StringHolder<Contains> {
                Contains( NSString* substr ) : StringHolder( substr ){}

                virtual bool match( ExpressionType const& str ) const {
                    return  (str != nil || m_substr == nil ) &&
                            [str rangeOfString:m_substr].location != NSNotFound;
                }

                virtual std::string toString() const {
                    return "contains string: \"" + Catch::toString( m_substr ) + "\"";
                }
            };

            struct StartsWith : StringHolder<StartsWith> {
                StartsWith( NSString* substr ) : StringHolder( substr ){}

                virtual bool match( ExpressionType const& str ) const {
                    return  (str != nil || m_substr == nil ) &&
                            [str rangeOfString:m_substr].location == 0;
                }

                virtual std::string toString() const {
                    return "starts with: \"" + Catch::toString( m_substr ) + "\"";
                }
            };
            struct EndsWith : StringHolder<EndsWith> {
                EndsWith( NSString* substr ) : StringHolder( substr ){}

                virtual bool match( ExpressionType const& str ) const {
                    return  (str != nil || m_substr == nil ) &&
                            [str rangeOfString:m_substr].location == [str length] - [m_substr length];
                }

                virtual std::string toString() const {
                    return "ends with: \"" + Catch::toString( m_substr ) + "\"";
                }
            };

        } // namespace NSStringMatchers
        } // namespace Impl

        inline Impl::NSStringMatchers::Equals
            Equals( NSString* substr ){ return Impl::NSStringMatchers::Equals( substr ); }

        inline Impl::NSStringMatchers::Contains
            Contains( NSString* substr ){ return Impl::NSStringMatchers::Contains( substr ); }

        inline Impl::NSStringMatchers::StartsWith
            StartsWith( NSString* substr ){ return Impl::NSStringMatchers::StartsWith( substr ); }

        inline Impl::NSStringMatchers::EndsWith
            EndsWith( NSString* substr ){ return Impl::NSStringMatchers::EndsWith( substr ); }

    } // namespace Matchers

    using namespace Matchers;

} // namespace Catch

///////////////////////////////////////////////////////////////////////////////
#define OC_TEST_CASE( name, desc )\
+(NSString*) INTERNAL_CATCH_UNIQUE_NAME( Catch_Name_test ) \
{\
return @ name; \
}\
+(NSString*) INTERNAL_CATCH_UNIQUE_NAME( Catch_Description_test ) \
{ \
return @ desc; \
} \
-(void) INTERNAL_CATCH_UNIQUE_NAME( Catch_TestCase_test )

#endif

#ifdef CATCH_CONFIG_RUNNER
// #included from: internal/catch_impl.hpp
#define TWOBLUECUBES_CATCH_IMPL_HPP_INCLUDED

// Collect all the implementation files together here
// These are the equivalent of what would usually be cpp files

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
#endif

// #included from: catch_runner.hpp
#define TWOBLUECUBES_CATCH_RUNNER_HPP_INCLUDED

// #included from: internal/catch_text.h
#define TWOBLUECUBES_CATCH_TEXT_H_INCLUDED

#define TBC_TEXT_FORMAT_CONSOLE_WIDTH CATCH_CONFIG_CONSOLE_WIDTH

#define CLICHE_TBC_TEXT_FORMAT_OUTER_NAMESPACE Catch
// #included from: ../external/tbc_text_format.h
// Only use header guard if we are not using an outer namespace
#ifndef CLICHE_TBC_TEXT_FORMAT_OUTER_NAMESPACE
# ifdef TWOBLUECUBES_TEXT_FORMAT_H_INCLUDED
#  ifndef TWOBLUECUBES_TEXT_FORMAT_H_ALREADY_INCLUDED
#   define TWOBLUECUBES_TEXT_FORMAT_H_ALREADY_INCLUDED
#  endif
# else
#  define TWOBLUECUBES_TEXT_FORMAT_H_INCLUDED
# endif
#endif
#ifndef TWOBLUECUBES_TEXT_FORMAT_H_ALREADY_INCLUDED
#include <string>
#include <vector>
#include <sstream>

// Use optional outer namespace
#ifdef CLICHE_TBC_TEXT_FORMAT_OUTER_NAMESPACE
namespace CLICHE_TBC_TEXT_FORMAT_OUTER_NAMESPACE {
#endif

namespace Tbc {

#ifdef TBC_TEXT_FORMAT_CONSOLE_WIDTH
    const unsigned int consoleWidth = TBC_TEXT_FORMAT_CONSOLE_WIDTH;
#else
    const unsigned int consoleWidth = 80;
#endif

    struct TextAttributes {
        TextAttributes()
        :   initialIndent( std::string::npos ),
            indent( 0 ),
            width( consoleWidth-1 ),
            tabChar( '\t' )
        {}

        TextAttributes& setInitialIndent( std::size_t _value )  { initialIndent = _value; return *this; }
        TextAttributes& setIndent( std::size_t _value )         { indent = _value; return *this; }
        TextAttributes& setWidth( std::size_t _value )          { width = _value; return *this; }
        TextAttributes& setTabChar( char _value )               { tabChar = _value; return *this; }

        std::size_t initialIndent;  // indent of first line, or npos
        std::size_t indent;         // indent of subsequent lines, or all if initialIndent is npos
        std::size_t width;          // maximum width of text, including indent. Longer text will wrap
        char tabChar;               // If this char is seen the indent is changed to current pos
    };

    class Text {
    public:
        Text( std::string const& _str, TextAttributes const& _attr = TextAttributes() )
        : attr( _attr )
        {
            std::string wrappableChars = " [({.,/|\\-";
            std::size_t indent = _attr.initialIndent != std::string::npos
                ? _attr.initialIndent
                : _attr.indent;
            std::string remainder = _str;

            while( !remainder.empty() ) {
                if( lines.size() >= 1000 ) {
                    lines.push_back( "... message truncated due to excessive size" );
                    return;
                }
                std::size_t tabPos = std::string::npos;
                std::size_t width = (std::min)( remainder.size(), _attr.width - indent );
                std::size_t pos = remainder.find_first_of( '\n' );
                if( pos <= width ) {
                    width = pos;
                }
                pos = remainder.find_last_of( _attr.tabChar, width );
                if( pos != std::string::npos ) {
                    tabPos = pos;
                    if( remainder[width] == '\n' )
                        width--;
                    remainder = remainder.substr( 0, tabPos ) + remainder.substr( tabPos+1 );
                }

                if( width == remainder.size() ) {
                    spliceLine( indent, remainder, width );
                }
                else if( remainder[width] == '\n' ) {
                    spliceLine( indent, remainder, width );
                    if( width <= 1 || remainder.size() != 1 )
                        remainder = remainder.substr( 1 );
                    indent = _attr.indent;
                }
                else {
                    pos = remainder.find_last_of( wrappableChars, width );
                    if( pos != std::string::npos && pos > 0 ) {
                        spliceLine( indent, remainder, pos );
                        if( remainder[0] == ' ' )
                            remainder = remainder.substr( 1 );
                    }
                    else {
                        spliceLine( indent, remainder, width-1 );
                        lines.back() += "-";
                    }
                    if( lines.size() == 1 )
                        indent = _attr.indent;
                    if( tabPos != std::string::npos )
                        indent += tabPos;
                }
            }
        }

        void spliceLine( std::size_t _indent, std::string& _remainder, std::size_t _pos ) {
            lines.push_back( std::string( _indent, ' ' ) + _remainder.substr( 0, _pos ) );
            _remainder = _remainder.substr( _pos );
        }

        typedef std::vector<std::string>::const_iterator const_iterator;

        const_iterator begin() const { return lines.begin(); }
        const_iterator end() const { return lines.end(); }
        std::string const& last() const { return lines.back(); }
        std::size_t size() const { return lines.size(); }
        std::string const& operator[]( std::size_t _index ) const { return lines[_index]; }
        std::string toString() const {
            std::ostringstream oss;
            oss << *this;
            return oss.str();
        }

        inline friend std::ostream& operator << ( std::ostream& _stream, Text const& _text ) {
            for( Text::const_iterator it = _text.begin(), itEnd = _text.end();
                it != itEnd; ++it ) {
                if( it != _text.begin() )
                    _stream << "\n";
                _stream << *it;
            }
            return _stream;
        }

    private:
        std::string str;
        TextAttributes attr;
        std::vector<std::string> lines;
    };

} // end namespace Tbc

#ifdef CLICHE_TBC_TEXT_FORMAT_OUTER_NAMESPACE
} // end outer namespace
#endif

#endif // TWOBLUECUBES_TEXT_FORMAT_H_ALREADY_INCLUDED
#undef CLICHE_TBC_TEXT_FORMAT_OUTER_NAMESPACE

namespace Catch {
    using Tbc::Text;
    using Tbc::TextAttributes;
}

// #included from: internal/catch_commandline.hpp
#define TWOBLUECUBES_CATCH_COMMANDLINE_HPP_INCLUDED

#ifdef CLARA_CONFIG_MAIN
// #included from: catch_clara.h
#define TWOBLUECUBES_CATCH_CLARA_H_INCLUDED

// Use Catch's value for console width (store Clara's off to the side, if present)
#ifdef CLARA_CONFIG_CONSOLE_WIDTH
#define CATCH_TEMP_CLARA_CONFIG_CONSOLE_WIDTH CLARA_CONFIG_CONSOLE_WIDTH
#undef CLARA_CONFIG_CONSOLE_WIDTH
#endif
#define CLARA_CONFIG_CONSOLE_WIDTH CATCH_CONFIG_CONSOLE_WIDTH

// Declare Clara inside the Catch namespace
#define STITCH_CLARA_OPEN_NAMESPACE namespace Catch {
// #included from: ../external/clara.h

// Only use header guard if we are not using an outer namespace
#if !defined(TWOBLUECUBES_CLARA_H_INCLUDED) || defined(STITCH_CLARA_OPEN_NAMESPACE)

#ifndef STITCH_CLARA_OPEN_NAMESPACE
#define TWOBLUECUBES_CLARA_H_INCLUDED
#define STITCH_CLARA_OPEN_NAMESPACE
#define STITCH_CLARA_CLOSE_NAMESPACE
#else
#define STITCH_CLARA_CLOSE_NAMESPACE }
#endif

#define STITCH_TBC_TEXT_FORMAT_OPEN_NAMESPACE STITCH_CLARA_OPEN_NAMESPACE

// ----------- #included from tbc_text_format.h -----------

// Only use header guard if we are not using an outer namespace
#if !defined(TBC_TEXT_FORMAT_H_INCLUDED) || defined(STITCH_TBC_TEXT_FORMAT_OUTER_NAMESPACE)
#ifndef STITCH_TBC_TEXT_FORMAT_OUTER_NAMESPACE
#define TBC_TEXT_FORMAT_H_INCLUDED
#endif

#include <string>
#include <vector>
#include <sstream>

// Use optional outer namespace
#ifdef STITCH_TBC_TEXT_FORMAT_OUTER_NAMESPACE
namespace STITCH_TBC_TEXT_FORMAT_OUTER_NAMESPACE {
#endif

namespace Tbc {

#ifdef TBC_TEXT_FORMAT_CONSOLE_WIDTH
    const unsigned int consoleWidth = TBC_TEXT_FORMAT_CONSOLE_WIDTH;
#else
    const unsigned int consoleWidth = 80;
#endif

    struct TextAttributes {
        TextAttributes()
        :   initialIndent( std::string::npos ),
            indent( 0 ),
            width( consoleWidth-1 ),
            tabChar( '\t' )
        {}

        TextAttributes& setInitialIndent( std::size_t _value )  { initialIndent = _value; return *this; }
        TextAttributes& setIndent( std::size_t _value )         { indent = _value; return *this; }
        TextAttributes& setWidth( std::size_t _value )          { width = _value; return *this; }
        TextAttributes& setTabChar( char _value )               { tabChar = _value; return *this; }

        std::size_t initialIndent;  // indent of first line, or npos
        std::size_t indent;         // indent of subsequent lines, or all if initialIndent is npos
        std::size_t width;          // maximum width of text, including indent. Longer text will wrap
        char tabChar;               // If this char is seen the indent is changed to current pos
    };

    class Text {
    public:
        Text( std::string const& _str, TextAttributes const& _attr = TextAttributes() )
        : attr( _attr )
        {
            std::string wrappableChars = " [({.,/|\\-";
            std::size_t indent = _attr.initialIndent != std::string::npos
                ? _attr.initialIndent
                : _attr.indent;
            std::string remainder = _str;

            while( !remainder.empty() ) {
                if( lines.size() >= 1000 ) {
                    lines.push_back( "... message truncated due to excessive size" );
                    return;
                }
                std::size_t tabPos = std::string::npos;
                std::size_t width = (std::min)( remainder.size(), _attr.width - indent );
                std::size_t pos = remainder.find_first_of( '\n' );
                if( pos <= width ) {
                    width = pos;
                }
                pos = remainder.find_last_of( _attr.tabChar, width );
                if( pos != std::string::npos ) {
                    tabPos = pos;
                    if( remainder[width] == '\n' )
                        width--;
                    remainder = remainder.substr( 0, tabPos ) + remainder.substr( tabPos+1 );
                }

                if( width == remainder.size() ) {
                    spliceLine( indent, remainder, width );
                }
                else if( remainder[width] == '\n' ) {
                    spliceLine( indent, remainder, width );
                    if( width <= 1 || remainder.size() != 1 )
                        remainder = remainder.substr( 1 );
                    indent = _attr.indent;
                }
                else {
                    pos = remainder.find_last_of( wrappableChars, width );
                    if( pos != std::string::npos && pos > 0 ) {
                        spliceLine( indent, remainder, pos );
                        if( remainder[0] == ' ' )
                            remainder = remainder.substr( 1 );
                    }
                    else {
                        spliceLine( indent, remainder, width-1 );
                        lines.back() += "-";
                    }
                    if( lines.size() == 1 )
                        indent = _attr.indent;
                    if( tabPos != std::string::npos )
                        indent += tabPos;
                }
            }
        }

        void spliceLine( std::size_t _indent, std::string& _remainder, std::size_t _pos ) {
            lines.push_back( std::string( _indent, ' ' ) + _remainder.substr( 0, _pos ) );
            _remainder = _remainder.substr( _pos );
        }

        typedef std::vector<std::string>::const_iterator const_iterator;

        const_iterator begin() const { return lines.begin(); }
        const_iterator end() const { return lines.end(); }
        std::string const& last() const { return lines.back(); }
        std::size_t size() const { return lines.size(); }
        std::string const& operator[]( std::size_t _index ) const { return lines[_index]; }
        std::string toString() const {
            std::ostringstream oss;
            oss << *this;
            return oss.str();
        }

        inline friend std::ostream& operator << ( std::ostream& _stream, Text const& _text ) {
            for( Text::const_iterator it = _text.begin(), itEnd = _text.end();
                it != itEnd; ++it ) {
                if( it != _text.begin() )
                    _stream << "\n";
                _stream << *it;
            }
            return _stream;
        }

    private:
        std::string str;
        TextAttributes attr;
        std::vector<std::string> lines;
    };

} // end namespace Tbc

#ifdef STITCH_TBC_TEXT_FORMAT_OUTER_NAMESPACE
} // end outer namespace
#endif

#endif // TBC_TEXT_FORMAT_H_INCLUDED

// ----------- end of #include from tbc_text_format.h -----------
// ........... back in /Users/philnash/Dev/OSS/Clara/srcs/clara.h

#undef STITCH_TBC_TEXT_FORMAT_OPEN_NAMESPACE

#include <map>
#include <algorithm>
#include <stdexcept>
#include <memory>

// Use optional outer namespace
#ifdef STITCH_CLARA_OPEN_NAMESPACE
STITCH_CLARA_OPEN_NAMESPACE
#endif

namespace Clara {

    struct UnpositionalTag {};

    extern UnpositionalTag _;

#ifdef CLARA_CONFIG_MAIN
    UnpositionalTag _;
#endif

    namespace Detail {

#ifdef CLARA_CONSOLE_WIDTH
    const unsigned int consoleWidth = CLARA_CONFIG_CONSOLE_WIDTH;
#else
    const unsigned int consoleWidth = 80;
#endif

        using namespace Tbc;

        inline bool startsWith( std::string const& str, std::string const& prefix ) {
            return str.size() >= prefix.size() && str.substr( 0, prefix.size() ) == prefix;
        }

        template<typename T> struct RemoveConstRef{ typedef T type; };
        template<typename T> struct RemoveConstRef<T&>{ typedef T type; };
        template<typename T> struct RemoveConstRef<T const&>{ typedef T type; };
        template<typename T> struct RemoveConstRef<T const>{ typedef T type; };

        template<typename T>    struct IsBool       { static const bool value = false; };
        template<>              struct IsBool<bool> { static const bool value = true; };

        template<typename T>
        void convertInto( std::string const& _source, T& _dest ) {
            std::stringstream ss;
            ss << _source;
            ss >> _dest;
            if( ss.fail() )
                throw std::runtime_error( "Unable to convert " + _source + " to destination type" );
        }
        inline void convertInto( std::string const& _source, std::string& _dest ) {
            _dest = _source;
        }
        inline void convertInto( std::string const& _source, bool& _dest ) {
            std::string sourceLC = _source;
            std::transform( sourceLC.begin(), sourceLC.end(), sourceLC.begin(), ::tolower );
            if( sourceLC == "y" || sourceLC == "1" || sourceLC == "true" || sourceLC == "yes" || sourceLC == "on" )
                _dest = true;
            else if( sourceLC == "n" || sourceLC == "0" || sourceLC == "false" || sourceLC == "no" || sourceLC == "off" )
                _dest = false;
            else
                throw std::runtime_error( "Expected a boolean value but did not recognise:\n  '" + _source + "'" );
        }
        inline void convertInto( bool _source, bool& _dest ) {
            _dest = _source;
        }
        template<typename T>
        inline void convertInto( bool, T& ) {
            throw std::runtime_error( "Invalid conversion" );
        }

        template<typename ConfigT>
        struct IArgFunction {
            virtual ~IArgFunction() {}
            virtual void set( ConfigT& config, std::string const& value ) const = 0;
            virtual void setFlag( ConfigT& config ) const = 0;
            virtual bool takesArg() const = 0;
            virtual IArgFunction* clone() const = 0;
        };

        template<typename ConfigT>
        class BoundArgFunction {
        public:
            BoundArgFunction() : functionObj( NULL ) {}
            BoundArgFunction( IArgFunction<ConfigT>* _functionObj ) : functionObj( _functionObj ) {}
            BoundArgFunction( BoundArgFunction const& other ) : functionObj( other.functionObj ? other.functionObj->clone() : NULL ) {}
            BoundArgFunction& operator = ( BoundArgFunction const& other ) {
                IArgFunction<ConfigT>* newFunctionObj = other.functionObj ? other.functionObj->clone() : NULL;
                delete functionObj;
                functionObj = newFunctionObj;
                return *this;
            }
            ~BoundArgFunction() { delete functionObj; }

            void set( ConfigT& config, std::string const& value ) const {
                functionObj->set( config, value );
            }
            void setFlag( ConfigT& config ) const {
                functionObj->setFlag( config );
            }
            bool takesArg() const { return functionObj->takesArg(); }

            bool isSet() const {
                return functionObj != NULL;
            }
        private:
            IArgFunction<ConfigT>* functionObj;
        };

        template<typename C>
        struct NullBinder : IArgFunction<C>{
            virtual void set( C&, std::string const& ) const {}
            virtual void setFlag( C& ) const {}
            virtual bool takesArg() const { return true; }
            virtual IArgFunction<C>* clone() const { return new NullBinder( *this ); }
        };

        template<typename C, typename M>
        struct BoundDataMember : IArgFunction<C>{
            BoundDataMember( M C::* _member ) : member( _member ) {}
            virtual void set( C& p, std::string const& stringValue ) const {
                convertInto( stringValue, p.*member );
            }
            virtual void setFlag( C& p ) const {
                convertInto( true, p.*member );
            }
            virtual bool takesArg() const { return !IsBool<M>::value; }
            virtual IArgFunction<C>* clone() const { return new BoundDataMember( *this ); }
            M C::* member;
        };
        template<typename C, typename M>
        struct BoundUnaryMethod : IArgFunction<C>{
            BoundUnaryMethod( void (C::*_member)( M ) ) : member( _member ) {}
            virtual void set( C& p, std::string const& stringValue ) const {
                typename RemoveConstRef<M>::type value;
                convertInto( stringValue, value );
                (p.*member)( value );
            }
            virtual void setFlag( C& p ) const {
                typename RemoveConstRef<M>::type value;
                convertInto( true, value );
                (p.*member)( value );
            }
            virtual bool takesArg() const { return !IsBool<M>::value; }
            virtual IArgFunction<C>* clone() const { return new BoundUnaryMethod( *this ); }
            void (C::*member)( M );
        };
        template<typename C>
        struct BoundNullaryMethod : IArgFunction<C>{
            BoundNullaryMethod( void (C::*_member)() ) : member( _member ) {}
            virtual void set( C& p, std::string const& stringValue ) const {
                bool value;
                convertInto( stringValue, value );
                if( value )
                    (p.*member)();
            }
            virtual void setFlag( C& p ) const {
                (p.*member)();
            }
            virtual bool takesArg() const { return false; }
            virtual IArgFunction<C>* clone() const { return new BoundNullaryMethod( *this ); }
            void (C::*member)();
        };

        template<typename C>
        struct BoundUnaryFunction : IArgFunction<C>{
            BoundUnaryFunction( void (*_function)( C& ) ) : function( _function ) {}
            virtual void set( C& obj, std::string const& stringValue ) const {
                bool value;
                convertInto( stringValue, value );
                if( value )
                    function( obj );
            }
            virtual void setFlag( C& p ) const {
                function( p );
            }
            virtual bool takesArg() const { return false; }
            virtual IArgFunction<C>* clone() const { return new BoundUnaryFunction( *this ); }
            void (*function)( C& );
        };

        template<typename C, typename T>
        struct BoundBinaryFunction : IArgFunction<C>{
            BoundBinaryFunction( void (*_function)( C&, T ) ) : function( _function ) {}
            virtual void set( C& obj, std::string const& stringValue ) const {
                typename RemoveConstRef<T>::type value;
                convertInto( stringValue, value );
                function( obj, value );
            }
            virtual void setFlag( C& obj ) const {
                typename RemoveConstRef<T>::type value;
                convertInto( true, value );
                function( obj, value );
            }
            virtual bool takesArg() const { return !IsBool<T>::value; }
            virtual IArgFunction<C>* clone() const { return new BoundBinaryFunction( *this ); }
            void (*function)( C&, T );
        };

    } // namespace Detail

    struct Parser {
        Parser() : separators( " \t=:" ) {}

        struct Token {
            enum Type { Positional, ShortOpt, LongOpt };
            Token( Type _type, std::string const& _data ) : type( _type ), data( _data ) {}
            Type type;
            std::string data;
        };

        void parseIntoTokens( int argc, char const * const * argv, std::vector<Parser::Token>& tokens ) const {
            const std::string doubleDash = "--";
            for( int i = 1; i < argc && argv[i] != doubleDash; ++i )
                parseIntoTokens( argv[i] , tokens);
        }
        void parseIntoTokens( std::string arg, std::vector<Parser::Token>& tokens ) const {
            while( !arg.empty() ) {
                Parser::Token token( Parser::Token::Positional, arg );
                arg = "";
                if( token.data[0] == '-' ) {
                    if( token.data.size() > 1 && token.data[1] == '-' ) {
                        token = Parser::Token( Parser::Token::LongOpt, token.data.substr( 2 ) );
                    }
                    else {
                        token = Parser::Token( Parser::Token::ShortOpt, token.data.substr( 1 ) );
                        if( token.data.size() > 1 && separators.find( token.data[1] ) == std::string::npos ) {
                            arg = "-" + token.data.substr( 1 );
                            token.data = token.data.substr( 0, 1 );
                        }
                    }
                }
                if( token.type != Parser::Token::Positional ) {
                    std::size_t pos = token.data.find_first_of( separators );
                    if( pos != std::string::npos ) {
                        arg = token.data.substr( pos+1 );
                        token.data = token.data.substr( 0, pos );
                    }
                }
                tokens.push_back( token );
            }
        }
        std::string separators;
    };

    template<typename ConfigT>
    struct CommonArgProperties {
        CommonArgProperties() {}
        CommonArgProperties( Detail::BoundArgFunction<ConfigT> const& _boundField ) : boundField( _boundField ) {}

        Detail::BoundArgFunction<ConfigT> boundField;
        std::string description;
        std::string detail;
        std::string placeholder; // Only value if boundField takes an arg

        bool takesArg() const {
            return !placeholder.empty();
        }
        void validate() const {
            if( !boundField.isSet() )
                throw std::logic_error( "option not bound" );
        }
    };
    struct OptionArgProperties {
        std::vector<std::string> shortNames;
        std::string longName;

        bool hasShortName( std::string const& shortName ) const {
            return std::find( shortNames.begin(), shortNames.end(), shortName ) != shortNames.end();
        }
        bool hasLongName( std::string const& _longName ) const {
            return _longName == longName;
        }
    };
    struct PositionalArgProperties {
        PositionalArgProperties() : position( -1 ) {}
        int position; // -1 means non-positional (floating)

        bool isFixedPositional() const {
            return position != -1;
        }
    };

    template<typename ConfigT>
    class CommandLine {

        struct Arg : CommonArgProperties<ConfigT>, OptionArgProperties, PositionalArgProperties {
            Arg() {}
            Arg( Detail::BoundArgFunction<ConfigT> const& _boundField ) : CommonArgProperties<ConfigT>( _boundField ) {}

            using CommonArgProperties<ConfigT>::placeholder; // !TBD

            std::string dbgName() const {
                if( !longName.empty() )
                    return "--" + longName;
                if( !shortNames.empty() )
                    return "-" + shortNames[0];
                return "positional args";
            }
            std::string commands() const {
                std::ostringstream oss;
                bool first = true;
                std::vector<std::string>::const_iterator it = shortNames.begin(), itEnd = shortNames.end();
                for(; it != itEnd; ++it ) {
                    if( first )
                        first = false;
                    else
                        oss << ", ";
                    oss << "-" << *it;
                }
                if( !longName.empty() ) {
                    if( !first )
                        oss << ", ";
                    oss << "--" << longName;
                }
                if( !placeholder.empty() )
                    oss << " <" << placeholder << ">";
                return oss.str();
            }
        };

        // NOTE: std::auto_ptr is deprecated in c++11/c++0x
#if defined(__cplusplus) && __cplusplus > 199711L
        typedef std::unique_ptr<Arg> ArgAutoPtr;
#else
        typedef std::auto_ptr<Arg> ArgAutoPtr;
#endif

        friend void addOptName( Arg& arg, std::string const& optName )
        {
            if( optName.empty() )
                return;
            if( Detail::startsWith( optName, "--" ) ) {
                if( !arg.longName.empty() )
                    throw std::logic_error( "Only one long opt may be specified. '"
                        + arg.longName
                        + "' already specified, now attempting to add '"
                        + optName + "'" );
                arg.longName = optName.substr( 2 );
            }
            else if( Detail::startsWith( optName, "-" ) )
                arg.shortNames.push_back( optName.substr( 1 ) );
            else
                throw std::logic_error( "option must begin with - or --. Option was: '" + optName + "'" );
        }
        friend void setPositionalArg( Arg& arg, int position )
        {
            arg.position = position;
        }

        class ArgBuilder {
        public:
            ArgBuilder( Arg* arg ) : m_arg( arg ) {}

            // Bind a non-boolean data member (requires placeholder string)
            template<typename C, typename M>
            void bind( M C::* field, std::string const& placeholder ) {
                m_arg->boundField = new Detail::BoundDataMember<C,M>( field );
                m_arg->placeholder = placeholder;
            }
            // Bind a boolean data member (no placeholder required)
            template<typename C>
            void bind( bool C::* field ) {
                m_arg->boundField = new Detail::BoundDataMember<C,bool>( field );
            }

            // Bind a method taking a single, non-boolean argument (requires a placeholder string)
            template<typename C, typename M>
            void bind( void (C::* unaryMethod)( M ), std::string const& placeholder ) {
                m_arg->boundField = new Detail::BoundUnaryMethod<C,M>( unaryMethod );
                m_arg->placeholder = placeholder;
            }

            // Bind a method taking a single, boolean argument (no placeholder string required)
            template<typename C>
            void bind( void (C::* unaryMethod)( bool ) ) {
                m_arg->boundField = new Detail::BoundUnaryMethod<C,bool>( unaryMethod );
            }

            // Bind a method that takes no arguments (will be called if opt is present)
            template<typename C>
            void bind( void (C::* nullaryMethod)() ) {
                m_arg->boundField = new Detail::BoundNullaryMethod<C>( nullaryMethod );
            }

            // Bind a free function taking a single argument - the object to operate on (no placeholder string required)
            template<typename C>
            void bind( void (* unaryFunction)( C& ) ) {
                m_arg->boundField = new Detail::BoundUnaryFunction<C>( unaryFunction );
            }

            // Bind a free function taking a single argument - the object to operate on (requires a placeholder string)
            template<typename C, typename T>
            void bind( void (* binaryFunction)( C&, T ), std::string const& placeholder ) {
                m_arg->boundField = new Detail::BoundBinaryFunction<C, T>( binaryFunction );
                m_arg->placeholder = placeholder;
            }

            ArgBuilder& describe( std::string const& description ) {
                m_arg->description = description;
                return *this;
            }
            ArgBuilder& detail( std::string const& detail ) {
                m_arg->detail = detail;
                return *this;
            }

        protected:
            Arg* m_arg;
        };

        class OptBuilder : public ArgBuilder {
        public:
            OptBuilder( Arg* arg ) : ArgBuilder( arg ) {}
            OptBuilder( OptBuilder& other ) : ArgBuilder( other ) {}

            OptBuilder& operator[]( std::string const& optName ) {
                addOptName( *ArgBuilder::m_arg, optName );
                return *this;
            }
        };

    public:

        CommandLine()
        :   m_boundProcessName( new Detail::NullBinder<ConfigT>() ),
            m_highestSpecifiedArgPosition( 0 ),
            m_throwOnUnrecognisedTokens( false )
        {}
        CommandLine( CommandLine const& other )
        :   m_boundProcessName( other.m_boundProcessName ),
            m_options ( other.m_options ),
            m_positionalArgs( other.m_positionalArgs ),
            m_highestSpecifiedArgPosition( other.m_highestSpecifiedArgPosition ),
            m_throwOnUnrecognisedTokens( other.m_throwOnUnrecognisedTokens )
        {
            if( other.m_floatingArg.get() )
                m_floatingArg = ArgAutoPtr( new Arg( *other.m_floatingArg ) );
        }

        CommandLine& setThrowOnUnrecognisedTokens( bool shouldThrow = true ) {
            m_throwOnUnrecognisedTokens = shouldThrow;
            return *this;
        }

        OptBuilder operator[]( std::string const& optName ) {
            m_options.push_back( Arg() );
            addOptName( m_options.back(), optName );
            OptBuilder builder( &m_options.back() );
            return builder;
        }

        ArgBuilder operator[]( int position ) {
            m_positionalArgs.insert( std::make_pair( position, Arg() ) );
            if( position > m_highestSpecifiedArgPosition )
                m_highestSpecifiedArgPosition = position;
            setPositionalArg( m_positionalArgs[position], position );
            ArgBuilder builder( &m_positionalArgs[position] );
            return builder;
        }

        // Invoke this with the _ instance
        ArgBuilder operator[]( UnpositionalTag ) {
            if( m_floatingArg.get() )
                throw std::logic_error( "Only one unpositional argument can be added" );
            m_floatingArg = ArgAutoPtr( new Arg() );
            ArgBuilder builder( m_floatingArg.get() );
            return builder;
        }

        template<typename C, typename M>
        void bindProcessName( M C::* field ) {
            m_boundProcessName = new Detail::BoundDataMember<C,M>( field );
        }
        template<typename C, typename M>
        void bindProcessName( void (C::*_unaryMethod)( M ) ) {
            m_boundProcessName = new Detail::BoundUnaryMethod<C,M>( _unaryMethod );
        }

        void optUsage( std::ostream& os, std::size_t indent = 0, std::size_t width = Detail::consoleWidth ) const {
            typename std::vector<Arg>::const_iterator itBegin = m_options.begin(), itEnd = m_options.end(), it;
            std::size_t maxWidth = 0;
            for( it = itBegin; it != itEnd; ++it )
                maxWidth = (std::max)( maxWidth, it->commands().size() );

            for( it = itBegin; it != itEnd; ++it ) {
                Detail::Text usage( it->commands(), Detail::TextAttributes()
                                                        .setWidth( maxWidth+indent )
                                                        .setIndent( indent ) );
                Detail::Text desc( it->description, Detail::TextAttributes()
                                                        .setWidth( width - maxWidth - 3 ) );

                for( std::size_t i = 0; i < (std::max)( usage.size(), desc.size() ); ++i ) {
                    std::string usageCol = i < usage.size() ? usage[i] : "";
                    os << usageCol;

                    if( i < desc.size() && !desc[i].empty() )
                        os  << std::string( indent + 2 + maxWidth - usageCol.size(), ' ' )
                            << desc[i];
                    os << "\n";
                }
            }
        }
        std::string optUsage() const {
            std::ostringstream oss;
            optUsage( oss );
            return oss.str();
        }

        void argSynopsis( std::ostream& os ) const {
            for( int i = 1; i <= m_highestSpecifiedArgPosition; ++i ) {
                if( i > 1 )
                    os << " ";
                typename std::map<int, Arg>::const_iterator it = m_positionalArgs.find( i );
                if( it != m_positionalArgs.end() )
                    os << "<" << it->second.placeholder << ">";
                else if( m_floatingArg.get() )
                    os << "<" << m_floatingArg->placeholder << ">";
                else
                    throw std::logic_error( "non consecutive positional arguments with no floating args" );
            }
            // !TBD No indication of mandatory args
            if( m_floatingArg.get() ) {
                if( m_highestSpecifiedArgPosition > 1 )
                    os << " ";
                os << "[<" << m_floatingArg->placeholder << "> ...]";
            }
        }
        std::string argSynopsis() const {
            std::ostringstream oss;
            argSynopsis( oss );
            return oss.str();
        }

        void usage( std::ostream& os, std::string const& procName ) const {
            validate();
            os << "usage:\n  " << procName << " ";
            argSynopsis( os );
            if( !m_options.empty() ) {
                os << " [options]\n\nwhere options are: \n";
                optUsage( os, 2 );
            }
            os << "\n";
        }
        std::string usage( std::string const& procName ) const {
            std::ostringstream oss;
            usage( oss, procName );
            return oss.str();
        }

        ConfigT parse( int argc, char const * const * argv ) const {
            ConfigT config;
            parseInto( argc, argv, config );
            return config;
        }

        std::vector<Parser::Token> parseInto( int argc, char const * const * argv, ConfigT& config ) const {
            std::string processName = argv[0];
            std::size_t lastSlash = processName.find_last_of( "/\\" );
            if( lastSlash != std::string::npos )
                processName = processName.substr( lastSlash+1 );
            m_boundProcessName.set( config, processName );
            std::vector<Parser::Token> tokens;
            Parser parser;
            parser.parseIntoTokens( argc, argv, tokens );
            return populate( tokens, config );
        }

        std::vector<Parser::Token> populate( std::vector<Parser::Token> const& tokens, ConfigT& config ) const {
            validate();
            std::vector<Parser::Token> unusedTokens = populateOptions( tokens, config );
            unusedTokens = populateFixedArgs( unusedTokens, config );
            unusedTokens = populateFloatingArgs( unusedTokens, config );
            return unusedTokens;
        }

        std::vector<Parser::Token> populateOptions( std::vector<Parser::Token> const& tokens, ConfigT& config ) const {
            std::vector<Parser::Token> unusedTokens;
            std::vector<std::string> errors;
            for( std::size_t i = 0; i < tokens.size(); ++i ) {
                Parser::Token const& token = tokens[i];
                typename std::vector<Arg>::const_iterator it = m_options.begin(), itEnd = m_options.end();
                for(; it != itEnd; ++it ) {
                    Arg const& arg = *it;

                    try {
                        if( ( token.type == Parser::Token::ShortOpt && arg.hasShortName( token.data ) ) ||
                            ( token.type == Parser::Token::LongOpt && arg.hasLongName( token.data ) ) ) {
                            if( arg.takesArg() ) {
                                if( i == tokens.size()-1 || tokens[i+1].type != Parser::Token::Positional )
                                    errors.push_back( "Expected argument to option: " + token.data );
                                else
                                    arg.boundField.set( config, tokens[++i].data );
                            }
                            else {
                                arg.boundField.setFlag( config );
                            }
                            break;
                        }
                    }
                    catch( std::exception& ex ) {
                        errors.push_back( std::string( ex.what() ) + "\n- while parsing: (" + arg.commands() + ")" );
                    }
                }
                if( it == itEnd ) {
                    if( token.type == Parser::Token::Positional || !m_throwOnUnrecognisedTokens )
                        unusedTokens.push_back( token );
                    else if( m_throwOnUnrecognisedTokens )
                        errors.push_back( "unrecognised option: " + token.data );
                }
            }
            if( !errors.empty() ) {
                std::ostringstream oss;
                for( std::vector<std::string>::const_iterator it = errors.begin(), itEnd = errors.end();
                        it != itEnd;
                        ++it ) {
                    if( it != errors.begin() )
                        oss << "\n";
                    oss << *it;
                }
                throw std::runtime_error( oss.str() );
            }
            return unusedTokens;
        }
        std::vector<Parser::Token> populateFixedArgs( std::vector<Parser::Token> const& tokens, ConfigT& config ) const {
            std::vector<Parser::Token> unusedTokens;
            int position = 1;
            for( std::size_t i = 0; i < tokens.size(); ++i ) {
                Parser::Token const& token = tokens[i];
                typename std::map<int, Arg>::const_iterator it = m_positionalArgs.find( position );
                if( it != m_positionalArgs.end() )
                    it->second.boundField.set( config, token.data );
                else
                    unusedTokens.push_back( token );
                if( token.type == Parser::Token::Positional )
                    position++;
            }
            return unusedTokens;
        }
        std::vector<Parser::Token> populateFloatingArgs( std::vector<Parser::Token> const& tokens, ConfigT& config ) const {
            if( !m_floatingArg.get() )
                return tokens;
            std::vector<Parser::Token> unusedTokens;
            for( std::size_t i = 0; i < tokens.size(); ++i ) {
                Parser::Token const& token = tokens[i];
                if( token.type == Parser::Token::Positional )
                    m_floatingArg->boundField.set( config, token.data );
                else
                    unusedTokens.push_back( token );
            }
            return unusedTokens;
        }

        void validate() const
        {
            if( m_options.empty() && m_positionalArgs.empty() && !m_floatingArg.get() )
                throw std::logic_error( "No options or arguments specified" );

            for( typename std::vector<Arg>::const_iterator  it = m_options.begin(),
                                                            itEnd = m_options.end();
                    it != itEnd; ++it )
                it->validate();
        }

    private:
        Detail::BoundArgFunction<ConfigT> m_boundProcessName;
        std::vector<Arg> m_options;
        std::map<int, Arg> m_positionalArgs;
        ArgAutoPtr m_floatingArg;
        int m_highestSpecifiedArgPosition;
        bool m_throwOnUnrecognisedTokens;
    };

} // end namespace Clara

STITCH_CLARA_CLOSE_NAMESPACE
#undef STITCH_CLARA_OPEN_NAMESPACE
#undef STITCH_CLARA_CLOSE_NAMESPACE

#endif // TWOBLUECUBES_CLARA_H_INCLUDED
#undef STITCH_CLARA_OPEN_NAMESPACE

// Restore Clara's value for console width, if present
#ifdef CATCH_TEMP_CLARA_CONFIG_CONSOLE_WIDTH
#define CLARA_CONFIG_CONSOLE_WIDTH CATCH_TEMP_CLARA_CONFIG_CONSOLE_WIDTH
#undef CATCH_TEMP_CLARA_CONFIG_CONSOLE_WIDTH
#endif

#endif

#include <fstream>

namespace Catch {

    inline void abortAfterFirst( ConfigData& config ) { config.abortAfter = 1; }
    inline void abortAfterX( ConfigData& config, int x ) {
        if( x < 1 )
            throw std::runtime_error( "Value after -x or --abortAfter must be greater than zero" );
        config.abortAfter = x;
    }
    inline void addTestOrTags( ConfigData& config, std::string const& _testSpec ) { config.testsOrTags.push_back( _testSpec ); }

    inline void addWarning( ConfigData& config, std::string const& _warning ) {
        if( _warning == "NoAssertions" )
            config.warnings = (WarnAbout::What)( config.warnings | WarnAbout::NoAssertions );
        else
            throw std::runtime_error( "Unrecognised warning: '" + _warning + "'" );

    }
    inline void setVerbosity( ConfigData& config, int level ) {
        // !TBD: accept strings?
        config.verbosity = (Verbosity::Level)level;
    }
    inline void setShowDurations( ConfigData& config, bool _showDurations ) {
        config.showDurations = _showDurations
            ? ShowDurations::Always
            : ShowDurations::Never;
    }
    inline void loadTestNamesFromFile( ConfigData& config, std::string const& _filename ) {
        std::ifstream f( _filename.c_str() );
        if( !f.is_open() )
            throw std::domain_error( "Unable to load input file: " + _filename );

        std::string line;
        while( std::getline( f, line ) ) {
            line = trim(line);
            if( !line.empty() && !startsWith( line, "#" ) )
                addTestOrTags( config, line );
        }
    }

#ifdef CLARA_CONFIG_MAIN
    inline Clara::CommandLine<ConfigData> makeCommandLineParser() {

        using namespace Clara;
        CommandLine<ConfigData> cli;

        cli.bindProcessName( &ConfigData::processName );

        cli["-?"]["-h"]["--help"]
            .describe( "display usage information" )
            .bind( &ConfigData::showHelp );

        cli["-l"]["--list-tests"]
            .describe( "list all/matching test cases" )
            .bind( &ConfigData::listTests );

        cli["-t"]["--list-tags"]
            .describe( "list all/matching tags" )
            .bind( &ConfigData::listTags );

        cli["-s"]["--success"]
            .describe( "include successful tests in output" )
            .bind( &ConfigData::showSuccessfulTests );

        cli["-b"]["--break"]
            .describe( "break into debugger on failure" )
            .bind( &ConfigData::shouldDebugBreak );

        cli["-e"]["--nothrow"]
            .describe( "skip exception tests" )
            .bind( &ConfigData::noThrow );

        cli["-o"]["--out"]
            .describe( "output filename" )
            .bind( &ConfigData::outputFilename, "filename" );

        cli["-r"]["--reporter"]
//            .placeholder( "name[:filename]" )
            .describe( "reporter to use (defaults to console)" )
            .bind( &ConfigData::reporterName, "name" );

        cli["-n"]["--name"]
            .describe( "suite name" )
            .bind( &ConfigData::name, "name" );

        cli["-a"]["--abort"]
            .describe( "abort at first failure" )
            .bind( &abortAfterFirst );

        cli["-x"]["--abortx"]
            .describe( "abort after x failures" )
            .bind( &abortAfterX, "no. failures" );

        cli["-w"]["--warn"]
            .describe( "enable warnings" )
            .bind( &addWarning, "warning name" );

// - needs updating if reinstated
//        cli.into( &setVerbosity )
//            .describe( "level of verbosity (0=no output)" )
//            .shortOpt( "v")
//            .longOpt( "verbosity" )
//            .placeholder( "level" );

        cli[_]
            .describe( "which test or tests to use" )
            .bind( &addTestOrTags, "test name, pattern or tags" );

        cli["-d"]["--durations"]
            .describe( "show test durations" )
            .bind( &setShowDurations, "yes/no" );

        cli["-f"]["--input-file"]
            .describe( "load test names to run from a file" )
            .bind( &loadTestNamesFromFile, "filename" );

        // Less common commands which don't have a short form
        cli["--list-test-names-only"]
            .describe( "list all/matching test cases names only" )
            .bind( &ConfigData::listTestNamesOnly );

        cli["--list-reporters"]
            .describe( "list all reporters" )
            .bind( &ConfigData::listReporters );

        return cli;
    }
#endif

} // end namespace Catch

// #included from: internal/catch_list.hpp
#define TWOBLUECUBES_CATCH_LIST_HPP_INCLUDED

// #included from: catch_console_colour.hpp
#define TWOBLUECUBES_CATCH_CONSOLE_COLOUR_HPP_INCLUDED

namespace Catch {

    struct Colour {
        enum Code {
            None = 0,

            White,
            Red,
            Green,
            Blue,
            Cyan,
            Yellow,
            Grey,

            Bright = 0x10,

            BrightRed = Bright | Red,
            BrightGreen = Bright | Green,
            LightGrey = Bright | Grey,
            BrightWhite = Bright | White,

            // By intention
            FileName = LightGrey,
            ResultError = BrightRed,
            ResultSuccess = BrightGreen,

            Error = BrightRed,
            Success = Green,

            OriginalExpression = Cyan,
            ReconstructedExpression = Yellow,

            SecondaryText = LightGrey,
            Headers = White
        };

        // Use constructed object for RAII guard
        Colour( Code _colourCode );
        ~Colour();
    };

} // end namespace Catch

#include <limits>
#include <algorithm>

namespace Catch {
    inline bool matchesFilters( std::vector<TestCaseFilters> const& filters, TestCase const& testCase ) {
        std::vector<TestCaseFilters>::const_iterator it = filters.begin();
        std::vector<TestCaseFilters>::const_iterator itEnd = filters.end();
        for(; it != itEnd; ++it )
            if( !it->shouldInclude( testCase ) )
                return false;
        return true;
    }

    inline std::size_t listTests( Config const& config ) {
        if( config.filters().empty() )
            std::cout << "All available test cases:\n";
        else
            std::cout << "Matching test cases:\n";

        std::size_t matchedTests = 0;
        TextAttributes nameAttr, tagsAttr;
        nameAttr.setInitialIndent( 2 ).setIndent( 4 );
        tagsAttr.setIndent( 6 );

        std::vector<TestCase> const& allTests = getRegistryHub().getTestCaseRegistry().getAllTests();
        for( std::vector<TestCase>::const_iterator it = allTests.begin(), itEnd = allTests.end();
                it != itEnd;
                ++it )
            if( matchesFilters( config.filters(), *it ) ) {
                matchedTests++;
                TestCaseInfo const& testCaseInfo = it->getTestCaseInfo();
                Colour::Code colour = testCaseInfo.isHidden
                    ? Colour::SecondaryText
                    : Colour::None;
                Colour colourGuard( colour );

                std::cout << Text( testCaseInfo.name, nameAttr ) << std::endl;
                if( !testCaseInfo.tags.empty() )
                    std::cout << Text( testCaseInfo.tagsAsString, tagsAttr ) << std::endl;
            }

        if( config.filters().empty() )
            std::cout << pluralise( matchedTests, "test case" ) << "\n" << std::endl;
        else
            std::cout << pluralise( matchedTests, "matching test case" ) << "\n" << std::endl;
        return matchedTests;
    }

    inline std::size_t listTestsNamesOnly( Config const& config ) {
        std::size_t matchedTests = 0;
        std::vector<TestCase> const& allTests = getRegistryHub().getTestCaseRegistry().getAllTests();
        for( std::vector<TestCase>::const_iterator it = allTests.begin(), itEnd = allTests.end();
                it != itEnd;
                ++it )
            if( matchesFilters( config.filters(), *it ) ) {
                matchedTests++;
                TestCaseInfo const& testCaseInfo = it->getTestCaseInfo();
                std::cout << testCaseInfo.name << std::endl;
            }
        return matchedTests;
    }

    inline std::size_t listTags( Config const& config ) {
        if( config.filters().empty() )
            std::cout << "All available tags:\n";
        else
            std::cout << "Matching tags:\n";

        std::map<std::string, int> tagCounts;

        std::vector<TestCase> const& allTests = getRegistryHub().getTestCaseRegistry().getAllTests();
        for( std::vector<TestCase>::const_iterator  it = allTests.begin(),
                                                    itEnd = allTests.end();
                it != itEnd;
                ++it ) {
            if( matchesFilters( config.filters(), *it ) ) {
                for( std::set<std::string>::const_iterator  tagIt = it->getTestCaseInfo().tags.begin(),
                                                            tagItEnd = it->getTestCaseInfo().tags.end();
                        tagIt != tagItEnd;
                        ++tagIt ) {
                    std::string tagName = *tagIt;
                    std::map<std::string, int>::iterator countIt = tagCounts.find( tagName );
                    if( countIt == tagCounts.end() )
                        tagCounts.insert( std::make_pair( tagName, 1 ) );
                    else
                        countIt->second++;
                }
            }
        }

        for( std::map<std::string, int>::const_iterator countIt = tagCounts.begin(),
                                                        countItEnd = tagCounts.end();
                countIt != countItEnd;
                ++countIt ) {
            std::ostringstream oss;
            oss << "  " << countIt->second << "  ";
            Text wrapper( "[" + countIt->first + "]", TextAttributes()
                                                        .setInitialIndent( 0 )
                                                        .setIndent( oss.str().size() )
                                                        .setWidth( CATCH_CONFIG_CONSOLE_WIDTH-10 ) );
            std::cout << oss.str() << wrapper << "\n";
        }
        std::cout << pluralise( tagCounts.size(), "tag" ) << "\n" << std::endl;
        return tagCounts.size();
    }

    inline std::size_t listReporters( Config const& /*config*/ ) {
        std::cout << "Available reports:\n";
        IReporterRegistry::FactoryMap const& factories = getRegistryHub().getReporterRegistry().getFactories();
        IReporterRegistry::FactoryMap::const_iterator itBegin = factories.begin(), itEnd = factories.end(), it;
        std::size_t maxNameLen = 0;
        for(it = itBegin; it != itEnd; ++it )
            maxNameLen = (std::max)( maxNameLen, it->first.size() );

        for(it = itBegin; it != itEnd; ++it ) {
            Text wrapper( it->second->getDescription(), TextAttributes()
                                                        .setInitialIndent( 0 )
                                                        .setIndent( 7+maxNameLen )
                                                        .setWidth( CATCH_CONFIG_CONSOLE_WIDTH - maxNameLen-8 ) );
            std::cout << "  "
                    << it->first
                    << ":"
                    << std::string( maxNameLen - it->first.size() + 2, ' ' )
                    << wrapper << "\n";
        }
        std::cout << std::endl;
        return factories.size();
    }

    inline Option<std::size_t> list( Config const& config ) {
        Option<std::size_t> listedCount;
        if( config.listTests() )
            listedCount = listedCount.valueOr(0) + listTests( config );
        if( config.listTestNamesOnly() )
            listedCount = listedCount.valueOr(0) + listTestsNamesOnly( config );
        if( config.listTags() )
            listedCount = listedCount.valueOr(0) + listTags( config );
        if( config.listReporters() )
            listedCount = listedCount.valueOr(0) + listReporters( config );
        return listedCount;
    }

} // end namespace Catch

// #included from: internal/catch_runner_impl.hpp
#define TWOBLUECUBES_CATCH_RUNNER_IMPL_HPP_INCLUDED

// #included from: catch_test_case_tracker.hpp
#define TWOBLUECUBES_CATCH_TEST_CASE_TRACKER_HPP_INCLUDED

#include <map>
#include <string>
#include <assert.h>

namespace Catch {
namespace SectionTracking {

    class TrackedSection {

        typedef std::map<std::string, TrackedSection> TrackedSections;

    public:
        enum RunState {
            NotStarted,
            Executing,
            ExecutingChildren,
            Completed
        };

        TrackedSection( std::string const& name, TrackedSection* parent )
        :   m_name( name ), m_runState( NotStarted ), m_parent( parent )
        {}

        RunState runState() const { return m_runState; }

        void addChild( std::string const& childName ) {
            m_children.insert( std::make_pair( childName, TrackedSection( childName, this ) ) );
        }
        TrackedSection* getChild( std::string const& childName ) {
            return &m_children.find( childName )->second;
        }

        void enter() {
            if( m_runState == NotStarted )
                m_runState = Executing;
        }
        void leave() {
            for( TrackedSections::const_iterator it = m_children.begin(), itEnd = m_children.end();
                    it != itEnd;
                    ++it )
                if( it->second.runState() != Completed ) {
                    m_runState = ExecutingChildren;
                    return;
                }
            m_runState = Completed;
        }
        TrackedSection* getParent() {
            return m_parent;
        }
        bool hasChildren() const {
            return !m_children.empty();
        }

    private:
        std::string m_name;
        RunState m_runState;
        TrackedSections m_children;
        TrackedSection* m_parent;

    };

    class TestCaseTracker {
    public:
        TestCaseTracker( std::string const& testCaseName )
        :   m_testCase( testCaseName, NULL ),
            m_currentSection( &m_testCase ),
            m_completedASectionThisRun( false )
        {}

        bool enterSection( std::string const& name ) {
            if( m_completedASectionThisRun )
                return false;
            if( m_currentSection->runState() == TrackedSection::Executing ) {
                m_currentSection->addChild( name );
                return false;
            }
            else {
                TrackedSection* child = m_currentSection->getChild( name );
                if( child->runState() != TrackedSection::Completed ) {
                    m_currentSection = child;
                    m_currentSection->enter();
                    return true;
                }
                return false;
            }
        }
        void leaveSection() {
            m_currentSection->leave();
            m_currentSection = m_currentSection->getParent();
            assert( m_currentSection != NULL );
            m_completedASectionThisRun = true;
        }

        bool currentSectionHasChildren() const {
            return m_currentSection->hasChildren();
        }
        bool isCompleted() const {
            return m_testCase.runState() == TrackedSection::Completed;
        }

        class Guard {
        public:
            Guard( TestCaseTracker& tracker )
            : m_tracker( tracker )
            {
                m_tracker.enterTestCase();
            }
            ~Guard() {
                m_tracker.leaveTestCase();
            }
        private:
            Guard( Guard const& );
            void operator = ( Guard const& );
            TestCaseTracker& m_tracker;
        };

    private:
        void enterTestCase() {
            m_currentSection = &m_testCase;
            m_completedASectionThisRun = false;
            m_testCase.enter();
        }
        void leaveTestCase() {
            m_testCase.leave();
        }

        TrackedSection m_testCase;
        TrackedSection* m_currentSection;
        bool m_completedASectionThisRun;
    };

} // namespace SectionTracking

using SectionTracking::TestCaseTracker;

} // namespace Catch

#include <set>
#include <string>

namespace Catch {

    class StreamRedirect {

    public:
        StreamRedirect( std::ostream& stream, std::string& targetString )
        :   m_stream( stream ),
            m_prevBuf( stream.rdbuf() ),
            m_targetString( targetString )
        {
            stream.rdbuf( m_oss.rdbuf() );
        }

        ~StreamRedirect() {
            m_targetString += m_oss.str();
            m_stream.rdbuf( m_prevBuf );
        }

    private:
        std::ostream& m_stream;
        std::streambuf* m_prevBuf;
        std::ostringstream m_oss;
        std::string& m_targetString;
    };

    ///////////////////////////////////////////////////////////////////////////
    class RunContext : public IResultCapture, public IRunner {

        RunContext( RunContext const& );
        void operator =( RunContext const& );

    public:

        explicit RunContext( Ptr<IConfig const> const& config, Ptr<IStreamingReporter> const& reporter )
        :   m_runInfo( config->name() ),
            m_context( getCurrentMutableContext() ),
            m_activeTestCase( NULL ),
            m_config( config ),
            m_reporter( reporter ),
            m_prevRunner( &m_context.getRunner() ),
            m_prevResultCapture( &m_context.getResultCapture() ),
            m_prevConfig( m_context.getConfig() )
        {
            m_context.setRunner( this );
            m_context.setConfig( m_config );
            m_context.setResultCapture( this );
            m_reporter->testRunStarting( m_runInfo );
        }

        virtual ~RunContext() {
            m_reporter->testRunEnded( TestRunStats( m_runInfo, m_totals, aborting() ) );
            m_context.setRunner( m_prevRunner );
            m_context.setConfig( NULL );
            m_context.setResultCapture( m_prevResultCapture );
            m_context.setConfig( m_prevConfig );
        }

        void testGroupStarting( std::string const& testSpec, std::size_t groupIndex, std::size_t groupsCount ) {
            m_reporter->testGroupStarting( GroupInfo( testSpec, groupIndex, groupsCount ) );
        }
        void testGroupEnded( std::string const& testSpec, Totals const& totals, std::size_t groupIndex, std::size_t groupsCount ) {
            m_reporter->testGroupEnded( TestGroupStats( GroupInfo( testSpec, groupIndex, groupsCount ), totals, aborting() ) );
        }

        Totals runMatching( std::string const& testSpec, std::size_t groupIndex, std::size_t groupsCount ) {

            std::vector<TestCase> matchingTests = getRegistryHub().getTestCaseRegistry().getMatchingTestCases( testSpec );

            Totals totals;

            testGroupStarting( testSpec, groupIndex, groupsCount );

            std::vector<TestCase>::const_iterator it = matchingTests.begin();
            std::vector<TestCase>::const_iterator itEnd = matchingTests.end();
            for(; it != itEnd; ++it )
                totals += runTest( *it );

            testGroupEnded( testSpec, totals, groupIndex, groupsCount );
            return totals;
        }

        Totals runTest( TestCase const& testCase ) {

            std::string redirectedCout;
            std::string redirectedCerr;

            TestCaseInfo testInfo = testCase.getTestCaseInfo();

            UnwindTestCaseOnCompletion finaliser(*this, m_totals, m_reporter, testInfo, redirectedCout, redirectedCerr);

            m_activeTestCase = &testCase;
            m_testCaseTracker = TestCaseTracker( testInfo.name );
            do {
                do {
                    runCurrentTest( redirectedCout, redirectedCerr );
                }
                while( !m_testCaseTracker->isCompleted() && !aborting() );
            }
            while( getCurrentContext().advanceGeneratorsForCurrentTest() && !aborting() );

            m_activeTestCase = NULL;
            m_testCaseTracker.reset();

            return finaliser.report();
        }

        Ptr<IConfig const> config() const {
            return m_config;
        }

    private: // IResultCapture

        virtual ResultAction::Value acceptExpression( ExpressionResultBuilder const& assertionResult, AssertionInfo const& assertionInfo ) {
            m_lastAssertionInfo = assertionInfo;
            return actOnCurrentResult( assertionResult.buildResult( assertionInfo ) );
        }

        virtual void assertionEnded( AssertionResult const& result ) {
            if( result.getResultType() == ResultWas::Ok ) {
                m_totals.assertions.passed++;
            }
            else if( !result.isOk() ) {
                m_totals.assertions.failed++;
            }

            if( m_reporter->assertionEnded( AssertionStats( result, m_messages, m_totals ) ) )
                m_messages.clear();

            // Reset working state
            m_lastAssertionInfo = AssertionInfo( "", m_lastAssertionInfo.lineInfo, "{Unknown expression after the reported line}" , m_lastAssertionInfo.resultDisposition );
        }

        virtual bool sectionStarted (
            SectionInfo const& sectionInfo,
            Counts& assertions
        )
        {
            std::ostringstream oss;
            oss << sectionInfo.name << "@" << sectionInfo.lineInfo;

            if( !m_testCaseTracker->enterSection( oss.str() ) )
                return false;

            m_lastAssertionInfo.lineInfo = sectionInfo.lineInfo;

            m_reporter->sectionStarting( sectionInfo );

            assertions = m_totals.assertions;

            return true;
        }
        bool testForMissingAssertions( Counts& assertions ) {
            if( assertions.total() != 0 ||
                    !m_config->warnAboutMissingAssertions() ||
                    m_testCaseTracker->currentSectionHasChildren() )
                return false;
            m_totals.assertions.failed++;
            assertions.failed++;
            return true;
        }

        void unwindSection(SectionInfo const& info, Counts const& prevAssertions, double _durationInSeconds ) {
            Counts assertions = m_totals.assertions - prevAssertions;
            bool missingAssertions = testForMissingAssertions( assertions );

            m_testCaseTracker->leaveSection();

            m_reporter->sectionEnded( SectionStats( info, assertions, _durationInSeconds, missingAssertions ) );
            m_messages.clear();
        }

        virtual void sectionEnded( SectionInfo const& info, Counts const& prevAssertions, double _durationInSeconds ) {
            if( std::uncaught_exception() ) {
                m_unfinishedSections.push_back( UnfinishedSections( info, prevAssertions, _durationInSeconds ) );
                return;
            }

            unwindSection(info, prevAssertions, _durationInSeconds);
        }

        virtual void pushScopedMessage( MessageInfo const& message ) {
            m_messages.push_back( message );
        }

        virtual void popScopedMessage( MessageInfo const& message ) {
            m_messages.erase( std::remove( m_messages.begin(), m_messages.end(), message ), m_messages.end() );
        }

        virtual bool shouldDebugBreak() const {
            return m_config->shouldDebugBreak();
        }

        virtual std::string getCurrentTestName() const {
            return m_activeTestCase
                ? m_activeTestCase->getTestCaseInfo().name
                : "";
        }

        virtual const AssertionResult* getLastResult() const {
            return &m_lastResult;
        }

    public:
        // !TBD We need to do this another way!
        bool aborting() const {
            return m_totals.assertions.failed == static_cast<std::size_t>( m_config->abortAfter() );
        }

    private:

        ResultAction::Value actOnCurrentResult( AssertionResult const& result ) {
            m_lastResult = result;
            assertionEnded( m_lastResult );

            ResultAction::Value action = ResultAction::None;

            if( !m_lastResult.isOk() ) {
                action = ResultAction::Failed;
                if( shouldDebugBreak() )
                    action = (ResultAction::Value)( action | ResultAction::Debug );
                if( aborting() )
                    action = (ResultAction::Value)( action | ResultAction::Abort );
            }
            return action;
        }

        void runCurrentTest( std::string& redirectedCout, std::string& redirectedCerr ) {
            TestCaseInfo const& testCaseInfo = m_activeTestCase->getTestCaseInfo();

            UnwindSectionOnCompletion finaliser(*this, m_totals, m_reporter, testCaseInfo, m_unfinishedSections, m_messages);
            try {
                m_lastAssertionInfo = AssertionInfo( "TEST_CASE", testCaseInfo.lineInfo, "", ResultDisposition::Normal );
                TestCaseTracker::Guard guard( *m_testCaseTracker );

                finaliser.startTimer();
                if( m_reporter->getPreferences().shouldRedirectStdOut ) {
                    StreamRedirect coutRedir( std::cout, redirectedCout );
                    StreamRedirect cerrRedir( std::cerr, redirectedCerr );
                    m_activeTestCase->invoke();
                }
                else {
                    m_activeTestCase->invoke();
                }
                finaliser.stopTimer();
            }
            catch( const Catch::TestFailureException& ) {
                // This just means the test was aborted due to failure
            }
            catch(...) {
                ExpressionResultBuilder exResult( ResultWas::ThrewException );
                exResult << translateActiveException();
                actOnCurrentResult( exResult.buildResult( m_lastAssertionInfo )  );
            }
        }

    private:
        struct UnfinishedSections {
            UnfinishedSections( SectionInfo const& _info, Counts const& _prevAssertions, double _durationInSeconds )
            : info( _info ), prevAssertions( _prevAssertions ), durationInSeconds( _durationInSeconds )
            {}

            SectionInfo info;
            Counts prevAssertions;
            double durationInSeconds;
        };

        class UnwindSectionOnCompletion
        {
        public:
            UnwindSectionOnCompletion(RunContext& context, Totals& totals, Ptr<IStreamingReporter>& reporter, TestCaseInfo const& testCaseInfo,
                std::vector<UnfinishedSections>& unfinishedSections, std::vector<MessageInfo>& messages)
                : m_context(context)
                , m_totals(totals)
                , m_reporter(reporter)
                , m_testCaseSection( testCaseInfo.name, testCaseInfo.description, testCaseInfo.lineInfo )
                , m_unfinishedSections(unfinishedSections)
                , m_messages(messages)
                , m_duration(0.0)
            {
                m_prevAssertions = m_totals.assertions;
                m_reporter->sectionStarting( m_testCaseSection );
            }
            ~UnwindSectionOnCompletion()
            {
                // If sections ended prematurely due to an exception we stored their
                // infos here so we can tear them down.
                for( std::vector<UnfinishedSections>::const_iterator it = m_unfinishedSections.begin(),
                            itEnd = m_unfinishedSections.end();
                        it != itEnd;
                        ++it ) {
                    m_context.unwindSection( it->info, it->prevAssertions, it->durationInSeconds );
                }
                m_unfinishedSections.clear();
                m_messages.clear();

                Counts assertions = m_totals.assertions - m_prevAssertions;
                bool missingAssertions = m_context.testForMissingAssertions( assertions );

                SectionStats testCaseSectionStats( m_testCaseSection, assertions, m_duration, missingAssertions );
                m_reporter->sectionEnded( testCaseSectionStats );
            }
            void startTimer()
            {
                m_timer.start();
            }
            void stopTimer()
            {
                m_duration = m_timer.getElapsedSeconds();
            }
        private:
            // non-copyable
            UnwindSectionOnCompletion(const UnwindSectionOnCompletion&);
            UnwindSectionOnCompletion& operator=(const UnwindSectionOnCompletion&);

            RunContext& m_context;
            Totals& m_totals;
            Ptr<IStreamingReporter>& m_reporter;
            SectionInfo m_testCaseSection;
            std::vector<UnfinishedSections>& m_unfinishedSections;
            std::vector<MessageInfo>& m_messages;
            Timer m_timer;
            Counts m_prevAssertions;
            double m_duration;
        };

        class UnwindTestCaseOnCompletion
        {
        public:
            UnwindTestCaseOnCompletion(RunContext& context, Totals& totals, Ptr<IStreamingReporter>& reporter, TestCaseInfo& testInfo,
                std::string& redirectedCout, std::string& redirectedCerr)
                : m_context(context), m_totals(totals), m_reporter(reporter), m_testInfo(testInfo)
                , m_redirectedCout(redirectedCout), m_redirectedCerr(redirectedCerr)
                , m_reported(false)
            {
                m_prevTotals = m_totals;
                m_reporter->testCaseStarting( m_testInfo );
            }
            ~UnwindTestCaseOnCompletion()
            {
                if( !m_reported )
                {
                    report();
                }
            }
            Totals report()
            {
                m_reported = true;
                Totals deltaTotals = m_totals.delta( m_prevTotals );
                m_totals.testCases += deltaTotals.testCases;
                m_reporter->testCaseEnded( TestCaseStats(   m_testInfo,
                                                            deltaTotals,
                                                            m_redirectedCout,
                                                            m_redirectedCerr,
                                                            m_context.aborting() ) );
                return deltaTotals;
            }
        private:
            // non-copyable
            UnwindTestCaseOnCompletion(const UnwindTestCaseOnCompletion&);
            UnwindTestCaseOnCompletion& operator=(const UnwindTestCaseOnCompletion&);

            RunContext& m_context;
            Totals& m_totals;
            Ptr<IStreamingReporter>& m_reporter;
            TestCaseInfo& m_testInfo;
            std::string& m_redirectedCout;
            std::string& m_redirectedCerr;
            bool m_reported;
            Totals m_prevTotals;
        };

        TestRunInfo m_runInfo;
        IMutableContext& m_context;
        TestCase const* m_activeTestCase;
        Option<TestCaseTracker> m_testCaseTracker;
        AssertionResult m_lastResult;

        Ptr<IConfig const> m_config;
        Totals m_totals;
        Ptr<IStreamingReporter> m_reporter;
        std::vector<MessageInfo> m_messages;
        IRunner* m_prevRunner;
        IResultCapture* m_prevResultCapture;
        Ptr<IConfig const> m_prevConfig;
        AssertionInfo m_lastAssertionInfo;
        std::vector<UnfinishedSections> m_unfinishedSections;
    };

} // end namespace Catch

// #included from: internal/catch_version.h
#define TWOBLUECUBES_CATCH_VERSION_H_INCLUDED

namespace Catch {

    // Versioning information
    struct Version {
        Version(    unsigned int _majorVersion,
                    unsigned int _minorVersion,
                    unsigned int _buildNumber,
                    std::string const& _branchName )
        :   majorVersion( _majorVersion ),
            minorVersion( _minorVersion ),
            buildNumber( _buildNumber ),
            branchName( _branchName )
        {}

        const unsigned int majorVersion;
        const unsigned int minorVersion;
        const unsigned int buildNumber;
        const std::string branchName;

    private:
        void operator=( Version const& );
    };

    template <typename T>
    struct LibraryVersionInfo
    {
        static const T value;
    };

    typedef LibraryVersionInfo<Version> libraryVersion;
}

#include <fstream>
#include <stdlib.h>
#include <limits>

namespace Catch {

    class Runner {

    public:
        Runner( Ptr<Config> const& config )
        :   m_config( config )
        {
            openStream();
            makeReporter();
        }

        Totals runTests() {

            std::vector<TestCaseFilters> filterGroups = m_config->filters();
            if( filterGroups.empty() ) {
                TestCaseFilters filterGroup( "" );
                filterGroups.push_back( filterGroup );
            }

            RunContext context( m_config.get(), m_reporter );

            Totals totals;

            for( std::size_t i=0; i < filterGroups.size() && !context.aborting(); ++i ) {
                context.testGroupStarting( filterGroups[i].getName(), i, filterGroups.size() );
                totals += runTestsForGroup( context, filterGroups[i] );
                context.testGroupEnded( filterGroups[i].getName(), totals, i, filterGroups.size() );
            }
            return totals;
        }

        Totals runTestsForGroup( RunContext& context, const TestCaseFilters& filterGroup ) {
            Totals totals;
            std::vector<TestCase>::const_iterator it = getRegistryHub().getTestCaseRegistry().getAllTests().begin();
            std::vector<TestCase>::const_iterator itEnd = getRegistryHub().getTestCaseRegistry().getAllTests().end();
            int testsRunForGroup = 0;
            for(; it != itEnd; ++it ) {
                if( filterGroup.shouldInclude( *it ) ) {
                    testsRunForGroup++;
                    if( m_testsAlreadyRun.find( *it ) == m_testsAlreadyRun.end() ) {

                        if( context.aborting() )
                            break;

                        totals += context.runTest( *it );
                        m_testsAlreadyRun.insert( *it );
                    }
                }
            }
            if( testsRunForGroup == 0 && !filterGroup.getName().empty() )
                m_reporter->noMatchingTestCases( filterGroup.getName() );
            return totals;

        }

    private:
        void openStream() {
            // Open output file, if specified
            if( !m_config->getFilename().empty() ) {
                m_ofs.open( m_config->getFilename().c_str() );
                if( m_ofs.fail() ) {
                    std::ostringstream oss;
                    oss << "Unable to open file: '" << m_config->getFilename() << "'";
                    throw std::domain_error( oss.str() );
                }
                m_config->setStreamBuf( m_ofs.rdbuf() );
            }
        }
        void makeReporter() {
            std::string reporterName = m_config->getReporterName().empty()
                ? "console"
                : m_config->getReporterName();

            m_reporter = getRegistryHub().getReporterRegistry().create( reporterName, m_config.get() );
            if( !m_reporter ) {
                std::ostringstream oss;
                oss << "No reporter registered with name: '" << reporterName << "'";
                throw std::domain_error( oss.str() );
            }
        }

    private:
        Ptr<Config> m_config;
        std::ofstream m_ofs;
        Ptr<IStreamingReporter> m_reporter;
        std::set<TestCase> m_testsAlreadyRun;
    };

#if !defined(INTERNAL_CATCH_VS_MANAGED) && !defined(INTERNAL_CATCH_VS_NATIVE)

    class Session {
        static bool alreadyInstantiated;

    public:

        struct OnUnusedOptions { enum DoWhat { Ignore, Fail }; };

        Session()
        : m_cli( makeCommandLineParser() ) {
            if( alreadyInstantiated ) {
                std::string msg = "Only one instance of Catch::Session can ever be used";
                std::cerr << msg << std::endl;
                throw std::logic_error( msg );
            }
            alreadyInstantiated = true;
        }
        ~Session() {
            Catch::cleanUp();
        }

        void showHelp( std::string const& processName ) {
            std::cout << "\nCatch v"    << libraryVersion::value.majorVersion << "."
                                        << libraryVersion::value.minorVersion << " build "
                                        << libraryVersion::value.buildNumber;
            if( libraryVersion::value.branchName != "master" )
                std::cout << " (" << libraryVersion::value.branchName << " branch)";
            std::cout << "\n";

            m_cli.usage( std::cout, processName );
            std::cout << "For more detail usage please see the project docs\n" << std::endl;
        }

        int applyCommandLine( int argc, char* const argv[], OnUnusedOptions::DoWhat unusedOptionBehaviour = OnUnusedOptions::Fail ) {
            try {
                m_cli.setThrowOnUnrecognisedTokens( unusedOptionBehaviour == OnUnusedOptions::Fail );
                m_unusedTokens = m_cli.parseInto( argc, argv, m_configData );
                if( m_configData.showHelp )
                    showHelp( m_configData.processName );
                m_config.reset();
            }
            catch( std::exception& ex ) {
                {
                    Colour colourGuard( Colour::Red );
                    std::cerr   << "\nError(s) in input:\n"
                                << Text( ex.what(), TextAttributes().setIndent(2) )
                                << "\n\n";
                }
                m_cli.usage( std::cout, m_configData.processName );
                return (std::numeric_limits<int>::max)();
            }
            return 0;
        }

        void useConfigData( ConfigData const& _configData ) {
            m_configData = _configData;
            m_config.reset();
        }

        int run( int argc, char* const argv[] ) {

            int returnCode = applyCommandLine( argc, argv );
            if( returnCode == 0 )
                returnCode = run();
            return returnCode;
        }

        int run() {
            if( m_configData.showHelp )
                return 0;

            try
            {
                config(); // Force config to be constructed
                Runner runner( m_config );

                // Handle list request
                if( Option<std::size_t> listed = list( config() ) )
                    return static_cast<int>( *listed );

                return static_cast<int>( runner.runTests().assertions.failed );
            }
            catch( std::exception& ex ) {
                std::cerr << ex.what() << std::endl;
                return (std::numeric_limits<int>::max)();
            }
        }

        Clara::CommandLine<ConfigData> const& cli() const {
            return m_cli;
        }
        std::vector<Clara::Parser::Token> const& unusedTokens() const {
            return m_unusedTokens;
        }
        ConfigData& configData() {
            return m_configData;
        }
        Config& config() {
            if( !m_config )
                m_config = new Config( m_configData );
            return *m_config;
        }

    private:
        Clara::CommandLine<ConfigData> m_cli;
        std::vector<Clara::Parser::Token> m_unusedTokens;
        ConfigData m_configData;
        Ptr<Config> m_config;
    };

    bool Session::alreadyInstantiated = false;

#endif // !VS_MANAGED && !VS_NATIVE

} // end namespace Catch

// #included from: catch_registry_hub.hpp
#define TWOBLUECUBES_CATCH_REGISTRY_HUB_HPP_INCLUDED

// #included from: catch_test_case_registry_impl.hpp
#define TWOBLUECUBES_CATCH_TEST_CASE_REGISTRY_IMPL_HPP_INCLUDED

#include <vector>
#include <set>
#include <sstream>
#include <iostream>

namespace Catch {

    class TestRegistry : public ITestCaseRegistry {
    public:
        TestRegistry() : m_unnamedCount( 0 ) {}
        virtual ~TestRegistry();

        virtual void registerTest( TestCase const& testCase ) {
            std::string name = testCase.getTestCaseInfo().name;
            if( name == "" ) {
                std::ostringstream oss;
                oss << "Anonymous test case " << ++m_unnamedCount;
                return registerTest( testCase.withName( oss.str() ) );
            }

            if( m_functions.find( testCase ) == m_functions.end() ) {
                m_functions.insert( testCase );
                m_functionsInOrder.push_back( testCase );
                if( !testCase.isHidden() )
                    m_nonHiddenFunctions.push_back( testCase );
            }
            else {
                TestCase const& prev = *m_functions.find( testCase );
                std::cerr   << "error: TEST_CASE( \"" << name << "\" ) already defined.\n"
                            << "\tFirst seen at " << prev.getTestCaseInfo().lineInfo << "\n"
                            << "\tRedefined at " << testCase.getTestCaseInfo().lineInfo << std::endl;
                exit(1);
            }
        }

        virtual std::vector<TestCase> const& getAllTests() const {
            return m_functionsInOrder;
        }

        virtual std::vector<TestCase> const& getAllNonHiddenTests() const {
            return m_nonHiddenFunctions;
        }

        // !TBD deprecated
        virtual std::vector<TestCase> getMatchingTestCases( std::string const& rawTestSpec ) const {
            std::vector<TestCase> matchingTests;
            getMatchingTestCases( rawTestSpec, matchingTests );
            return matchingTests;
        }

        // !TBD deprecated
        virtual void getMatchingTestCases( std::string const& rawTestSpec, std::vector<TestCase>& matchingTestsOut ) const {
            TestCaseFilter filter( rawTestSpec );

            std::vector<TestCase>::const_iterator it = m_functionsInOrder.begin();
            std::vector<TestCase>::const_iterator itEnd = m_functionsInOrder.end();
            for(; it != itEnd; ++it ) {
                if( filter.shouldInclude( *it ) ) {
                    matchingTestsOut.push_back( *it );
                }
            }
        }
        virtual void getMatchingTestCases( TestCaseFilters const& filters, std::vector<TestCase>& matchingTestsOut ) const {
            std::vector<TestCase>::const_iterator it = m_functionsInOrder.begin();
            std::vector<TestCase>::const_iterator itEnd = m_functionsInOrder.end();
            // !TBD: replace with algorithm
            for(; it != itEnd; ++it )
                if( filters.shouldInclude( *it ) )
                    matchingTestsOut.push_back( *it );
        }

    private:

        std::set<TestCase> m_functions;
        std::vector<TestCase> m_functionsInOrder;
        std::vector<TestCase> m_nonHiddenFunctions;
        size_t m_unnamedCount;
    };

    ///////////////////////////////////////////////////////////////////////////

    class FreeFunctionTestCase : public SharedImpl<ITestCase> {
    public:

        FreeFunctionTestCase( TestFunction fun ) : m_fun( fun ) {}

        virtual void invoke() const {
            m_fun();
        }

    private:
        virtual ~FreeFunctionTestCase();

        TestFunction m_fun;
    };

    inline std::string extractClassName( std::string const& classOrQualifiedMethodName ) {
        std::string className = classOrQualifiedMethodName;
        if( startsWith( className, "&" ) )
        {
            std::size_t lastColons = className.rfind( "::" );
            std::size_t penultimateColons = className.rfind( "::", lastColons-1 );
            if( penultimateColons == std::string::npos )
                penultimateColons = 1;
            className = className.substr( penultimateColons, lastColons-penultimateColons );
        }
        return className;
    }

    ///////////////////////////////////////////////////////////////////////////

    INTERNAL_CATCH_INLINE AutoReg::AutoReg(   TestFunction function,
                        SourceLineInfo const& lineInfo,
                        NameAndDesc const& nameAndDesc ) {
        registerTestCase( new FreeFunctionTestCase( function ), "", nameAndDesc, lineInfo );
    }

    INTERNAL_CATCH_INLINE AutoReg::~AutoReg() {}

    INTERNAL_CATCH_INLINE void AutoReg::registerTestCase( ITestCase* testCase,
                                    char const* classOrQualifiedMethodName,
                                    NameAndDesc const& nameAndDesc,
                                    SourceLineInfo const& lineInfo ) {

        getMutableRegistryHub().registerTest
            ( makeTestCase( testCase,
                            extractClassName( classOrQualifiedMethodName ),
                            nameAndDesc.name,
                            nameAndDesc.description,
                            lineInfo ) );
    }

} // end namespace Catch

// #included from: catch_reporter_registry.hpp
#define TWOBLUECUBES_CATCH_REPORTER_REGISTRY_HPP_INCLUDED

#include <map>

namespace Catch {

    class ReporterRegistry : public IReporterRegistry {

    public:

        virtual ~ReporterRegistry() {
            deleteAllValues( m_factories );
        }

        virtual IStreamingReporter* create( std::string const& name, Ptr<IConfig> const& config ) const {
            FactoryMap::const_iterator it =  m_factories.find( name );
            if( it == m_factories.end() )
                return NULL;
            return it->second->create( ReporterConfig( config ) );
        }

        void registerReporter( std::string const& name, IReporterFactory* factory ) {
            m_factories.insert( std::make_pair( name, factory ) );
        }

        FactoryMap const& getFactories() const {
            return m_factories;
        }

    private:
        FactoryMap m_factories;
    };
}

// #included from: catch_exception_translator_registry.hpp
#define TWOBLUECUBES_CATCH_EXCEPTION_TRANSLATOR_REGISTRY_HPP_INCLUDED

#ifdef __OBJC__
#import "Foundation/Foundation.h"
#endif

namespace Catch {

    class ExceptionTranslatorRegistry : public IExceptionTranslatorRegistry {
    public:
        ~ExceptionTranslatorRegistry() {
            deleteAll( m_translators );
        }

        virtual void registerTranslator( const IExceptionTranslator* translator ) {
            m_translators.push_back( translator );
        }

        virtual std::string translateActiveException() const {
            try {
#ifdef __OBJC__
                // In Objective-C try objective-c exceptions first
                @try {
                    throw;
                }
                @catch (NSException *exception) {
                    return toString( [exception description] );
                }
#else
                throw;
#endif
            }
            catch( std::exception& ex ) {
                return ex.what();
            }
            catch( std::string& msg ) {
                return msg;
            }
            catch( const char* msg ) {
                return msg;
            }
            catch(...) {
                return tryTranslators( m_translators.begin() );
            }
        }

        std::string tryTranslators( std::vector<const IExceptionTranslator*>::const_iterator it ) const {
            if( it == m_translators.end() )
                return "Unknown exception";

            try {
                return (*it)->translate();
            }
            catch(...) {
                return tryTranslators( it+1 );
            }
        }

    private:
        std::vector<const IExceptionTranslator*> m_translators;
    };
}

namespace Catch {

    class RegistryHub : public IRegistryHub, public IMutableRegistryHub {

        RegistryHub( RegistryHub const& );
        void operator=( RegistryHub const& );

    public: // IRegistryHub
        RegistryHub() {
        }
        virtual IReporterRegistry const& getReporterRegistry() const {
            return m_reporterRegistry;
        }
        virtual ITestCaseRegistry const& getTestCaseRegistry() const {
            return m_testCaseRegistry;
        }
        virtual IExceptionTranslatorRegistry& getExceptionTranslatorRegistry() {
            return m_exceptionTranslatorRegistry;
        }

    public: // IMutableRegistryHub
        virtual void registerReporter( std::string const& name, IReporterFactory* factory ) {
            m_reporterRegistry.registerReporter( name, factory );
        }
        virtual void registerTest( TestCase const& testInfo ) {
            m_testCaseRegistry.registerTest( testInfo );
        }
        virtual void registerTranslator( const IExceptionTranslator* translator ) {
            m_exceptionTranslatorRegistry.registerTranslator( translator );
        }

    private:
        TestRegistry m_testCaseRegistry;
        ReporterRegistry m_reporterRegistry;
        ExceptionTranslatorRegistry m_exceptionTranslatorRegistry;
    };

    // Single, global, instance
    template <typename T>
    struct GlobalRegistryHub
    {
        static T*& instance()
        {
            if( !theRegistryHub )
                theRegistryHub = new T();
            return theRegistryHub;
        }
        static T* theRegistryHub;
    };
    template <typename T>
    T* GlobalRegistryHub<T>::theRegistryHub = NULL;

    INTERNAL_CATCH_INLINE IRegistryHub& getRegistryHub() {
        return *GlobalRegistryHub<RegistryHub>::instance();
    }
    INTERNAL_CATCH_INLINE IMutableRegistryHub& getMutableRegistryHub() {
        return *GlobalRegistryHub<RegistryHub>::instance();
    }
    INTERNAL_CATCH_INLINE void cleanUp() {
        delete GlobalRegistryHub<RegistryHub>::instance();
        GlobalRegistryHub<RegistryHub>::instance() = NULL;
        cleanUpContext();
    }
    INTERNAL_CATCH_INLINE std::string translateActiveException() {
        return getRegistryHub().getExceptionTranslatorRegistry().translateActiveException();
    }

} // end namespace Catch

// #included from: catch_notimplemented_exception.hpp
#define TWOBLUECUBES_CATCH_NOTIMPLEMENTED_EXCEPTION_HPP_INCLUDED

#include <ostream>

namespace Catch {

    INTERNAL_CATCH_INLINE NotImplementedException::NotImplementedException( SourceLineInfo const& lineInfo )
    :   m_lineInfo( lineInfo ) {
        std::ostringstream oss;
        oss << lineInfo << ": function ";
        oss << "not implemented";
        m_what = oss.str();
    }

    INTERNAL_CATCH_INLINE const char* NotImplementedException::what() const throw() {
        return m_what.c_str();
    }

} // end namespace Catch

// #included from: catch_context_impl.hpp
#define TWOBLUECUBES_CATCH_CONTEXT_IMPL_HPP_INCLUDED

// #included from: catch_stream.hpp
#define TWOBLUECUBES_CATCH_STREAM_HPP_INCLUDED

// #included from: catch_streambuf.h
#define TWOBLUECUBES_CATCH_STREAMBUF_H_INCLUDED

#include <streambuf>

namespace Catch {

    class StreamBufBase : public std::streambuf {
    public:
        virtual ~StreamBufBase() throw();
    };
}

#include <stdexcept>
#include <cstdio>

namespace Catch {

    template<typename WriterF, size_t bufferSize=256>
    class StreamBufImpl : public StreamBufBase {
        char data[bufferSize];
        WriterF m_writer;

    public:
        StreamBufImpl() {
            setp( data, data + sizeof(data) );
        }

        ~StreamBufImpl() throw() {
            sync();
        }

    private:
        int overflow( int c ) {
            sync();

            if( c != EOF ) {
                if( pbase() == epptr() )
                    m_writer( std::string( 1, static_cast<char>( c ) ) );
                else
                    sputc( static_cast<char>( c ) );
            }
            return 0;
        }

        int sync() {
            if( pbase() != pptr() ) {
                m_writer( std::string( pbase(), static_cast<std::string::size_type>( pptr() - pbase() ) ) );
                setp( pbase(), epptr() );
            }
            return 0;
        }
    };

    ///////////////////////////////////////////////////////////////////////////

    struct OutputDebugWriter {

        void operator()( std::string const&str ) {
            writeToDebugConsole( str );
        }
    };

    INTERNAL_CATCH_INLINE Stream::Stream()
    : streamBuf( NULL ), isOwned( false )
    {}

    INTERNAL_CATCH_INLINE Stream::Stream( std::streambuf* _streamBuf, bool _isOwned )
    : streamBuf( _streamBuf ), isOwned( _isOwned )
    {}

    INTERNAL_CATCH_INLINE void Stream::release() {
        if( isOwned ) {
            delete streamBuf;
            streamBuf = NULL;
            isOwned = false;
        }
    }
}

namespace Catch {

    template <typename Runner, typename ResultCapture>
    class Context : public IMutableContext {

        Context() : m_config( NULL ), m_runner( &nullRunner ), m_resultCapture( &nullResultCapture ) {}
        Context( Context const& );
        void operator=( Context const& );

    public: // IContext
        virtual IResultCapture& getResultCapture() {
            return *m_resultCapture;
        }
        virtual IRunner& getRunner() {
            return *m_runner;
        }
        virtual size_t getGeneratorIndex( std::string const& fileInfo, size_t totalSize ) {
            return getGeneratorsForCurrentTest()
            .getGeneratorInfo( fileInfo, totalSize )
            .getCurrentIndex();
        }
        virtual bool advanceGeneratorsForCurrentTest() {
            IGeneratorsForTest* generators = findGeneratorsForCurrentTest();
            return generators && generators->moveNext();
        }

        virtual Ptr<IConfig const> getConfig() const {
            return m_config;
        }

    public: // IMutableContext
        virtual void setResultCapture( IResultCapture* resultCapture ) {
            m_resultCapture = resultCapture;
        }
        virtual void setRunner( IRunner* runner ) {
            m_runner = runner;
        }
        virtual void setConfig( Ptr<IConfig const> const& config ) {
            m_config = config;
        }

        friend IMutableContext& getCurrentMutableContext();

    private:
        IGeneratorsForTest* findGeneratorsForCurrentTest() {
            std::string testName = getResultCapture().getCurrentTestName();

            std::map<std::string, IGeneratorsForTest*>::const_iterator it =
            m_generatorsByTestName.find( testName );
            return it != m_generatorsByTestName.end()
                ? it->second
                : NULL;
        }

        IGeneratorsForTest& getGeneratorsForCurrentTest() {
            IGeneratorsForTest* generators = findGeneratorsForCurrentTest();
            if( !generators ) {
                std::string testName = getResultCapture().getCurrentTestName();
                generators = createGeneratorsForTest();
                m_generatorsByTestName.insert( std::make_pair( testName, generators ) );
            }
            return *generators;
        }

    private:
        Ptr<IConfig const> m_config;
        IRunner* m_runner;
        IResultCapture* m_resultCapture;
        std::map<std::string, IGeneratorsForTest*> m_generatorsByTestName;

        static ResultCapture nullResultCapture;
        static Runner nullRunner;
    public:
        static Context* currentContext;
    };

    template <typename Runner, typename ResultCapture>
    ResultCapture Context<Runner, ResultCapture>::nullResultCapture;
    template <typename Runner, typename ResultCapture>
    Runner Context<Runner, ResultCapture>::nullRunner;
    template <typename Runner, typename ResultCapture>
    Context<Runner,ResultCapture>* Context<Runner, ResultCapture>::currentContext = NULL;

    typedef Context<NullRunner, NullResultCapture> DefaultContext;
    INTERNAL_CATCH_INLINE IMutableContext& getCurrentMutableContext() {
        if( !DefaultContext::currentContext )
            DefaultContext::currentContext = new DefaultContext();
        return *DefaultContext::currentContext;
    }
    INTERNAL_CATCH_INLINE IContext& getCurrentContext() {
        return getCurrentMutableContext();
    }

    INTERNAL_CATCH_INLINE Stream createStream( std::string const& streamName ) {
        if( streamName == "stdout" ) return Stream( std::cout.rdbuf(), false );
        if( streamName == "stderr" ) return Stream( std::cerr.rdbuf(), false );
        if( streamName == "debug" ) return Stream( new StreamBufImpl<OutputDebugWriter>, true );

        throw std::domain_error( "Unknown stream: " + streamName );
    }

    INTERNAL_CATCH_INLINE void cleanUpContext() {
        delete DefaultContext::currentContext;
        DefaultContext::currentContext = NULL;
    }
}

// #included from: catch_console_colour_impl.hpp
#define TWOBLUECUBES_CATCH_CONSOLE_COLOUR_IMPL_HPP_INCLUDED

#if defined ( CATCH_PLATFORM_WINDOWS ) /////////////////////////////////////////

#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifdef __AFXDLL
#include <AfxWin.h>
#else
#include <windows.h>
#endif

namespace Catch {
namespace {

    class Win32ColourImpl {
    public:
        Win32ColourImpl() : stdoutHandle( GetStdHandle(STD_OUTPUT_HANDLE) )
        {
            CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
            GetConsoleScreenBufferInfo( stdoutHandle, &csbiInfo );
            originalAttributes = csbiInfo.wAttributes;
        }

        void use( Colour::Code _colourCode ) {
            switch( _colourCode ) {
                case Colour::None:      return setTextAttribute( originalAttributes );
                case Colour::White:     return setTextAttribute( FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE );
                case Colour::Red:       return setTextAttribute( FOREGROUND_RED );
                case Colour::Green:     return setTextAttribute( FOREGROUND_GREEN );
                case Colour::Blue:      return setTextAttribute( FOREGROUND_BLUE );
                case Colour::Cyan:      return setTextAttribute( FOREGROUND_BLUE | FOREGROUND_GREEN );
                case Colour::Yellow:    return setTextAttribute( FOREGROUND_RED | FOREGROUND_GREEN );
                case Colour::Grey:      return setTextAttribute( 0 );

                case Colour::LightGrey:     return setTextAttribute( FOREGROUND_INTENSITY );
                case Colour::BrightRed:     return setTextAttribute( FOREGROUND_INTENSITY | FOREGROUND_RED );
                case Colour::BrightGreen:   return setTextAttribute( FOREGROUND_INTENSITY | FOREGROUND_GREEN );
                case Colour::BrightWhite:   return setTextAttribute( FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE );

                case Colour::Bright: throw std::logic_error( "not a colour" );
            }
        }

    private:
        void setTextAttribute( WORD _textAttribute ) {
            SetConsoleTextAttribute( stdoutHandle, _textAttribute );
        }
        HANDLE stdoutHandle;
        WORD originalAttributes;
    };

    inline bool shouldUseColourForPlatform() {
        return true;
    }

    typedef Win32ColourImpl PlatformColourImpl;

} // end anon namespace
} // end namespace Catch

#else // Not Windows - assumed to be POSIX compatible //////////////////////////

#include <unistd.h>

namespace Catch {
namespace {

    // use POSIX/ ANSI console terminal codes
    // Thanks to Adam Strzelecki for original contribution
    // (http://github.com/nanoant)
    // https://github.com/philsquared/Catch/pull/131
    class PosixColourImpl {
    public:
        void use( Colour::Code _colourCode ) {
            switch( _colourCode ) {
                case Colour::None:
                case Colour::White:     return setColour( "[0m" );
                case Colour::Red:       return setColour( "[0;31m" );
                case Colour::Green:     return setColour( "[0;32m" );
                case Colour::Blue:      return setColour( "[0:34m" );
                case Colour::Cyan:      return setColour( "[0;36m" );
                case Colour::Yellow:    return setColour( "[0;33m" );
                case Colour::Grey:      return setColour( "[1;30m" );

                case Colour::LightGrey:     return setColour( "[0;37m" );
                case Colour::BrightRed:     return setColour( "[1;31m" );
                case Colour::BrightGreen:   return setColour( "[1;32m" );
                case Colour::BrightWhite:   return setColour( "[1;37m" );

                case Colour::Bright: throw std::logic_error( "not a colour" );
            }
        }
    private:
        void setColour( const char* _escapeCode ) {
            std::cout << '\033' << _escapeCode;
        }
    };

    inline bool shouldUseColourForPlatform() {
        return isatty(STDOUT_FILENO);
    }

    typedef PosixColourImpl PlatformColourImpl;

} // end anon namespace
} // end namespace Catch

#endif // not Windows

namespace Catch {

    template <typename Impl>
    struct ColourChange
    {
        static Impl impl;
        static const bool shouldUseColour;
    };
    template <typename Impl>
    Impl ColourChange<Impl>::impl;
    template <typename Impl>
    const bool ColourChange<Impl>::shouldUseColour = shouldUseColourForPlatform() &&
                                            !isDebuggerActive();;

    INTERNAL_CATCH_INLINE Colour::Colour( Code _colourCode ) {
        if( ColourChange<PlatformColourImpl>::shouldUseColour ) ColourChange<PlatformColourImpl>::impl.use( _colourCode );
    }
    INTERNAL_CATCH_INLINE Colour::~Colour() {
        if( ColourChange<PlatformColourImpl>::shouldUseColour ) ColourChange<PlatformColourImpl>::impl.use( Colour::None );
    }

} // end namespace Catch

// #included from: catch_generators_impl.hpp
#define TWOBLUECUBES_CATCH_GENERATORS_IMPL_HPP_INCLUDED

#include <vector>
#include <string>
#include <map>

namespace Catch {

    struct GeneratorInfo : IGeneratorInfo {

        GeneratorInfo( std::size_t size )
        :   m_size( size ),
            m_currentIndex( 0 )
        {}

        bool moveNext() {
            if( ++m_currentIndex == m_size ) {
                m_currentIndex = 0;
                return false;
            }
            return true;
        }

        std::size_t getCurrentIndex() const {
            return m_currentIndex;
        }

        std::size_t m_size;
        std::size_t m_currentIndex;
    };

    ///////////////////////////////////////////////////////////////////////////

    class GeneratorsForTest : public IGeneratorsForTest {

    public:
        ~GeneratorsForTest() {
            deleteAll( m_generatorsInOrder );
        }

        IGeneratorInfo& getGeneratorInfo( std::string const& fileInfo, std::size_t size ) {
            std::map<std::string, IGeneratorInfo*>::const_iterator it = m_generatorsByName.find( fileInfo );
            if( it == m_generatorsByName.end() ) {
                IGeneratorInfo* info = new GeneratorInfo( size );
                m_generatorsByName.insert( std::make_pair( fileInfo, info ) );
                m_generatorsInOrder.push_back( info );
                return *info;
            }
            return *it->second;
        }

        bool moveNext() {
            std::vector<IGeneratorInfo*>::const_iterator it = m_generatorsInOrder.begin();
            std::vector<IGeneratorInfo*>::const_iterator itEnd = m_generatorsInOrder.end();
            for(; it != itEnd; ++it ) {
                if( (*it)->moveNext() )
                    return true;
            }
            return false;
        }

    private:
        std::map<std::string, IGeneratorInfo*> m_generatorsByName;
        std::vector<IGeneratorInfo*> m_generatorsInOrder;
    };

    INTERNAL_CATCH_INLINE IGeneratorsForTest* createGeneratorsForTest()
    {
        return new GeneratorsForTest();
    }

} // end namespace Catch

// #included from: catch_assertionresult.hpp
#define TWOBLUECUBES_CATCH_ASSERTIONRESULT_HPP_INCLUDED

namespace Catch {

    INTERNAL_CATCH_INLINE AssertionInfo::AssertionInfo(   std::string const& _macroName,
                                    SourceLineInfo const& _lineInfo,
                                    std::string const& _capturedExpression,
                                    ResultDisposition::Flags _resultDisposition )
    :   macroName( _macroName ),
        lineInfo( _lineInfo ),
        capturedExpression( _capturedExpression ),
        resultDisposition( _resultDisposition )
    {}

    INTERNAL_CATCH_INLINE AssertionResult::AssertionResult() {}

    INTERNAL_CATCH_INLINE AssertionResult::AssertionResult( AssertionInfo const& info, AssertionResultData const& data )
    :   m_info( info ),
        m_resultData( data )
    {}

    INTERNAL_CATCH_INLINE AssertionResult::~AssertionResult() {}

    // Result was a success
    INTERNAL_CATCH_INLINE bool AssertionResult::succeeded() const {
        return Catch::isOk( m_resultData.resultType );
    }

    // Result was a success, or failure is suppressed
    INTERNAL_CATCH_INLINE bool AssertionResult::isOk() const {
        return Catch::isOk( m_resultData.resultType ) || shouldSuppressFailure( m_info.resultDisposition );
    }

    INTERNAL_CATCH_INLINE ResultWas::OfType AssertionResult::getResultType() const {
        return m_resultData.resultType;
    }

    INTERNAL_CATCH_INLINE bool AssertionResult::hasExpression() const {
        return !m_info.capturedExpression.empty();
    }

    INTERNAL_CATCH_INLINE bool AssertionResult::hasMessage() const {
        return !m_resultData.message.empty();
    }

    INTERNAL_CATCH_INLINE std::string AssertionResult::getExpression() const {
        if( shouldNegate( m_info.resultDisposition ) )
            return "!" + m_info.capturedExpression;
        else
            return m_info.capturedExpression;
    }
    INTERNAL_CATCH_INLINE std::string AssertionResult::getExpressionInMacro() const {
        if( m_info.macroName.empty() )
            return m_info.capturedExpression;
        else
            return m_info.macroName + "( " + m_info.capturedExpression + " )";
    }

    INTERNAL_CATCH_INLINE bool AssertionResult::hasExpandedExpression() const {
        return hasExpression() && getExpandedExpression() != getExpression();
    }

    INTERNAL_CATCH_INLINE std::string AssertionResult::getExpandedExpression() const {
        return m_resultData.reconstructedExpression;
    }

    INTERNAL_CATCH_INLINE std::string AssertionResult::getMessage() const {
        return m_resultData.message;
    }
    INTERNAL_CATCH_INLINE SourceLineInfo AssertionResult::getSourceInfo() const {
        return m_info.lineInfo;
    }

    INTERNAL_CATCH_INLINE std::string AssertionResult::getTestMacroName() const {
        return m_info.macroName;
    }

} // end namespace Catch

// #included from: catch_expressionresult_builder.hpp
#define TWOBLUECUBES_CATCH_EXPRESSIONRESULT_BUILDER_HPP_INCLUDED

#include <assert.h>

namespace Catch {

    INTERNAL_CATCH_INLINE ExpressionResultBuilder::ExpressionResultBuilder( ResultWas::OfType resultType ) {
        m_data.resultType = resultType;
    }
    INTERNAL_CATCH_INLINE ExpressionResultBuilder::ExpressionResultBuilder( ExpressionResultBuilder const& other )
    :   m_data( other.m_data ),
        m_exprComponents( other.m_exprComponents )
    {
        m_stream << other.m_stream.str();
    }
    INTERNAL_CATCH_INLINE ExpressionResultBuilder& ExpressionResultBuilder::operator=(ExpressionResultBuilder const& other ) {
        m_data = other.m_data;
        m_exprComponents = other.m_exprComponents;
        m_stream.str("");
        m_stream << other.m_stream.str();
        return *this;
    }
    INTERNAL_CATCH_INLINE ExpressionResultBuilder& ExpressionResultBuilder::setResultType( ResultWas::OfType result ) {
        m_data.resultType = result;
        return *this;
    }
    INTERNAL_CATCH_INLINE ExpressionResultBuilder& ExpressionResultBuilder::setResultType( bool result ) {
        m_data.resultType = result ? ResultWas::Ok : ResultWas::ExpressionFailed;
        return *this;
    }
    INTERNAL_CATCH_INLINE ExpressionResultBuilder& ExpressionResultBuilder::endExpression( ResultDisposition::Flags resultDisposition ) {
        m_exprComponents.shouldNegate = shouldNegate( resultDisposition );
        return *this;
    }
    INTERNAL_CATCH_INLINE ExpressionResultBuilder& ExpressionResultBuilder::setLhs( std::string const& lhs ) {
        m_exprComponents.lhs = lhs;
        return *this;
    }
    INTERNAL_CATCH_INLINE ExpressionResultBuilder& ExpressionResultBuilder::setRhs( std::string const& rhs ) {
        m_exprComponents.rhs = rhs;
        return *this;
    }
    INTERNAL_CATCH_INLINE ExpressionResultBuilder& ExpressionResultBuilder::setOp( std::string const& op ) {
        m_exprComponents.op = op;
        return *this;
    }
    INTERNAL_CATCH_INLINE AssertionResult ExpressionResultBuilder::buildResult( AssertionInfo const& info ) const
    {
        assert( m_data.resultType != ResultWas::Unknown );

        AssertionResultData data = m_data;

        // Flip bool results if shouldNegate is set
        if( m_exprComponents.shouldNegate && data.resultType == ResultWas::Ok )
            data.resultType = ResultWas::ExpressionFailed;
        else if( m_exprComponents.shouldNegate && data.resultType == ResultWas::ExpressionFailed )
            data.resultType = ResultWas::Ok;

        data.message = m_stream.str();
        data.reconstructedExpression = reconstructExpression( info );
        if( m_exprComponents.shouldNegate ) {
            if( m_exprComponents.op == "" )
                data.reconstructedExpression = "!" + data.reconstructedExpression;
            else
                data.reconstructedExpression = "!(" + data.reconstructedExpression + ")";
        }
        return AssertionResult( info, data );
    }
    INTERNAL_CATCH_INLINE std::string ExpressionResultBuilder::reconstructExpression( AssertionInfo const& info ) const {
        if( m_exprComponents.op == "" )
            return m_exprComponents.lhs.empty() ? info.capturedExpression : m_exprComponents.op + m_exprComponents.lhs;
        else if( m_exprComponents.op == "matches" )
            return m_exprComponents.lhs + " " + m_exprComponents.rhs;
        else if( m_exprComponents.op != "!" ) {
            if( m_exprComponents.lhs.size() + m_exprComponents.rhs.size() < 40 &&
                m_exprComponents.lhs.find("\n") == std::string::npos &&
                m_exprComponents.rhs.find("\n") == std::string::npos )
                return m_exprComponents.lhs + " " + m_exprComponents.op + " " + m_exprComponents.rhs;
            else
                return m_exprComponents.lhs + "\n" + m_exprComponents.op + "\n" + m_exprComponents.rhs;
        }
        else
            return "{can't expand - use " + info.macroName + "_FALSE( " + info.capturedExpression.substr(1) + " ) instead of " + info.macroName + "( " + info.capturedExpression + " ) for better diagnostics}";
    }

} // end namespace Catch

// #included from: catch_test_case_info.hpp
#define TWOBLUECUBES_CATCH_TEST_CASE_INFO_HPP_INCLUDED

namespace Catch {

    INTERNAL_CATCH_INLINE TestCase makeTestCase(  ITestCase* _testCase,
                            std::string const& _className,
                            std::string const& _name,
                            std::string const& _descOrTags,
                            SourceLineInfo const& _lineInfo )
    {
        std::string desc = _descOrTags;
        bool isHidden( startsWith( _name, "./" ) ); // Legacy support
        std::set<std::string> tags;
        TagExtracter( tags ).parse( desc );
        if( tags.find( "hide" ) != tags.end() || tags.find( "." ) != tags.end() )
            isHidden = true;

        if( isHidden ) {
            tags.insert( "hide" );
            tags.insert( "." );
        }
        TestCaseInfo info( _name, _className, desc, tags, isHidden, _lineInfo );
        return TestCase( _testCase, info );
    }

    INTERNAL_CATCH_INLINE TestCaseInfo::TestCaseInfo( std::string const& _name,
                                std::string const& _className,
                                std::string const& _description,
                                std::set<std::string> const& _tags,
                                bool _isHidden,
                                SourceLineInfo const& _lineInfo )
    :   name( _name ),
        className( _className ),
        description( _description ),
        tags( _tags ),
        lineInfo( _lineInfo ),
        isHidden( _isHidden )
    {
        std::ostringstream oss;
        for( std::set<std::string>::const_iterator it = _tags.begin(), itEnd = _tags.end(); it != itEnd; ++it )
            oss << "[" << *it << "]";
        tagsAsString = oss.str();
    }

    INTERNAL_CATCH_INLINE TestCaseInfo::TestCaseInfo( TestCaseInfo const& other )
    :   name( other.name ),
        className( other.className ),
        description( other.description ),
        tags( other.tags ),
        tagsAsString( other.tagsAsString ),
        lineInfo( other.lineInfo ),
        isHidden( other.isHidden )
    {}

    INTERNAL_CATCH_INLINE TestCase::TestCase( ITestCase* testCase, TestCaseInfo const& info ) : TestCaseInfo( info ), test( testCase ) {}

    INTERNAL_CATCH_INLINE TestCase::TestCase( TestCase const& other )
    :   TestCaseInfo( other ),
        test( other.test )
    {}

    INTERNAL_CATCH_INLINE TestCase TestCase::withName( std::string const& _newName ) const {
        TestCase other( *this );
        other.name = _newName;
        return other;
    }

    INTERNAL_CATCH_INLINE void TestCase::invoke() const {
        test->invoke();
    }

    INTERNAL_CATCH_INLINE bool TestCase::isHidden() const {
        return TestCaseInfo::isHidden;
    }

    INTERNAL_CATCH_INLINE bool TestCase::hasTag( std::string const& tag ) const {
        return tags.find( toLower( tag ) ) != tags.end();
    }
    INTERNAL_CATCH_INLINE bool TestCase::matchesTags( std::string const& tagPattern ) const {
        TagExpression exp;
        TagExpressionParser( exp ).parse( tagPattern );
        return exp.matches( tags );
    }
    INTERNAL_CATCH_INLINE std::set<std::string> const& TestCase::getTags() const {
        return tags;
    }

    INTERNAL_CATCH_INLINE void TestCase::swap( TestCase& other ) {
        test.swap( other.test );
        className.swap( other.className );
        name.swap( other.name );
        description.swap( other.description );
        std::swap( lineInfo, other.lineInfo );
    }

    INTERNAL_CATCH_INLINE bool TestCase::operator == ( TestCase const& other ) const {
        return  test.get() == other.test.get() &&
                name == other.name &&
                className == other.className;
    }

    INTERNAL_CATCH_INLINE bool TestCase::operator < ( TestCase const& other ) const {
        return name < other.name;
    }
    INTERNAL_CATCH_INLINE TestCase& TestCase::operator = ( TestCase const& other ) {
        TestCase temp( other );
        swap( temp );
        return *this;
    }

    INTERNAL_CATCH_INLINE TestCaseInfo const& TestCase::getTestCaseInfo() const
    {
        return *this;
    }

} // end namespace Catch

// #included from: catch_tags.hpp
#define TWOBLUECUBES_CATCH_TAGS_HPP_INCLUDED

namespace Catch {
    INTERNAL_CATCH_INLINE TagParser::~TagParser() {}

    INTERNAL_CATCH_INLINE void TagParser::parse( std::string const& str ) {
        std::size_t pos = 0;
        while( pos < str.size() ) {
            char c = str[pos];
            if( c == '[' ) {
                std::size_t end = str.find_first_of( ']', pos );
                if( end != std::string::npos ) {
                    acceptTag( str.substr( pos+1, end-pos-1 ) );
                    pos = end+1;
                }
                else {
                    acceptChar( c );
                    pos++;
                }
            }
            else {
                acceptChar( c );
                pos++;
            }
        }
        endParse();
    }

    INTERNAL_CATCH_INLINE TagExtracter::TagExtracter( std::set<std::string>& tags )
    :   m_tags( tags )
    {}

    INTERNAL_CATCH_INLINE TagExtracter::~TagExtracter() {}

    INTERNAL_CATCH_INLINE void TagExtracter::parse( std::string& description ) {
        TagParser::parse( description );
        description = m_remainder;
    }

    INTERNAL_CATCH_INLINE void TagExtracter::acceptTag( std::string const& tag ) {
        m_tags.insert( toLower( tag ) );
    }
    INTERNAL_CATCH_INLINE void TagExtracter::acceptChar( char c ) {
        m_remainder += c;
    }

    INTERNAL_CATCH_INLINE Tag::Tag() : m_isNegated( false ) {}
    INTERNAL_CATCH_INLINE Tag::Tag( std::string const& name, bool isNegated )
    :   m_name( name ),
        m_isNegated( isNegated )
    {}

    INTERNAL_CATCH_INLINE std::string Tag::getName() const {
        return m_name;
    }
    INTERNAL_CATCH_INLINE bool Tag::isNegated() const {
        return m_isNegated;
    }

    INTERNAL_CATCH_INLINE bool Tag::operator ! () const {
        return m_name.empty();
    }

    INTERNAL_CATCH_INLINE void TagSet::add( Tag const& tag ) {
        m_tags.insert( std::make_pair( toLower( tag.getName() ), tag ) );
    }

    INTERNAL_CATCH_INLINE bool TagSet::empty() const {
        return m_tags.empty();
    }

    INTERNAL_CATCH_INLINE bool TagSet::matches( std::set<std::string> const& tags ) const {
        for(    TagMap::const_iterator
                    it = m_tags.begin(), itEnd = m_tags.end();
                it != itEnd;
                ++it ) {
            bool found = tags.find( it->first ) != tags.end();
            if( found == it->second.isNegated() )
                return false;
        }
        return true;
    }

    INTERNAL_CATCH_INLINE bool TagExpression::matches( std::set<std::string> const& tags ) const {
        for(    std::vector<TagSet>::const_iterator
                    it = m_tagSets.begin(), itEnd = m_tagSets.end();
                it != itEnd;
                ++it )
            if( it->matches( tags ) )
                return true;
        return false;
    }

    INTERNAL_CATCH_INLINE TagExpressionParser::TagExpressionParser( TagExpression& exp )
    :   m_isNegated( false ),
        m_exp( exp )
    {}

    INTERNAL_CATCH_INLINE TagExpressionParser::~TagExpressionParser() {}

    INTERNAL_CATCH_INLINE void TagExpressionParser::acceptTag( std::string const& tag ) {
        m_currentTagSet.add( Tag( tag, m_isNegated ) );
        m_isNegated = false;
    }

    INTERNAL_CATCH_INLINE void TagExpressionParser::acceptChar( char c ) {
        switch( c ) {
            case '~':
                m_isNegated = true;
                break;
            case ',':
                m_exp.m_tagSets.push_back( m_currentTagSet );
                m_currentTagSet = TagSet();
                break;
        }
    }

    INTERNAL_CATCH_INLINE void TagExpressionParser::endParse() {
        if( !m_currentTagSet.empty() )
            m_exp.m_tagSets.push_back( m_currentTagSet );
    }

} // end namespace Catch

// #included from: catch_test_spec.hpp
#define TWOBLUECUBES_CATCH_TEST_SPEC_HPP_INCLUDED

namespace Catch {

    INTERNAL_CATCH_INLINE TestCaseFilter::TestCaseFilter( std::string const& testSpec, IfFilterMatches::DoWhat matchBehaviour )
    :   m_stringToMatch( toLower( testSpec ) ),
        m_filterType( matchBehaviour ),
        m_wildcardPosition( NoWildcard )
    {
        if( m_filterType == IfFilterMatches::AutoDetectBehaviour ) {
            if( startsWith( m_stringToMatch, "exclude:" ) ) {
                m_stringToMatch = m_stringToMatch.substr( 8 );
                m_filterType = IfFilterMatches::ExcludeTests;
            }
            else if( startsWith( m_stringToMatch, "~" ) ) {
                m_stringToMatch = m_stringToMatch.substr( 1 );
                m_filterType = IfFilterMatches::ExcludeTests;
            }
            else {
                m_filterType = IfFilterMatches::IncludeTests;
            }
        }

        if( startsWith( m_stringToMatch, "*" ) ) {
            m_stringToMatch = m_stringToMatch.substr( 1 );
            m_wildcardPosition = (WildcardPosition)( m_wildcardPosition | WildcardAtStart );
        }
        if( endsWith( m_stringToMatch, "*" ) ) {
            m_stringToMatch = m_stringToMatch.substr( 0, m_stringToMatch.size()-1 );
            m_wildcardPosition = (WildcardPosition)( m_wildcardPosition | WildcardAtEnd );
        }
    }

    INTERNAL_CATCH_INLINE IfFilterMatches::DoWhat TestCaseFilter::getFilterType() const {
        return m_filterType;
    }

    INTERNAL_CATCH_INLINE bool TestCaseFilter::shouldInclude( TestCase const& testCase ) const {
        return isMatch( testCase ) == (m_filterType == IfFilterMatches::IncludeTests);
    }

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunreachable-code"
#endif

    INTERNAL_CATCH_INLINE bool TestCaseFilter::isMatch( TestCase const& testCase ) const {
        std::string name = testCase.getTestCaseInfo().name;
        toLowerInPlace( name );

        switch( m_wildcardPosition ) {
            case NoWildcard:
                return m_stringToMatch == name;
            case WildcardAtStart:
                return endsWith( name, m_stringToMatch );
            case WildcardAtEnd:
                return startsWith( name, m_stringToMatch );
            case WildcardAtBothEnds:
                return contains( name, m_stringToMatch );
        }
        throw std::logic_error( "Unhandled wildcard type" );
    }

#ifdef __clang__
#pragma clang diagnostic pop
#endif

    INTERNAL_CATCH_INLINE TestCaseFilters::TestCaseFilters( std::string const& name ) : m_name( name ) {}

    INTERNAL_CATCH_INLINE std::string TestCaseFilters::getName() const {
        return m_name;
    }

    INTERNAL_CATCH_INLINE void TestCaseFilters::addFilter( TestCaseFilter const& filter ) {
        if( filter.getFilterType() == IfFilterMatches::ExcludeTests )
            m_exclusionFilters.push_back( filter );
        else
            m_inclusionFilters.push_back( filter );
    }

    INTERNAL_CATCH_INLINE void TestCaseFilters::addTags( std::string const& tagPattern ) {
        TagExpression exp;
        TagExpressionParser( exp ).parse( tagPattern );

        m_tagExpressions.push_back( exp );
    }

    INTERNAL_CATCH_INLINE bool TestCaseFilters::shouldInclude( TestCase const& testCase ) const {
        if( !m_tagExpressions.empty() ) {
            std::vector<TagExpression>::const_iterator it = m_tagExpressions.begin();
            std::vector<TagExpression>::const_iterator itEnd = m_tagExpressions.end();
            for(; it != itEnd; ++it )
                if( it->matches( testCase.getTags() ) )
                    break;
            if( it == itEnd )
                return false;
        }

        if( !m_inclusionFilters.empty() ) {
            std::vector<TestCaseFilter>::const_iterator it = m_inclusionFilters.begin();
            std::vector<TestCaseFilter>::const_iterator itEnd = m_inclusionFilters.end();
            for(; it != itEnd; ++it )
                if( it->shouldInclude( testCase ) )
                    break;
            if( it == itEnd )
                return false;
        }
        else if( m_exclusionFilters.empty() && m_tagExpressions.empty() ) {
            return !testCase.isHidden();
        }

        std::vector<TestCaseFilter>::const_iterator it = m_exclusionFilters.begin();
        std::vector<TestCaseFilter>::const_iterator itEnd = m_exclusionFilters.end();
        for(; it != itEnd; ++it )
            if( !it->shouldInclude( testCase ) )
                return false;
        return true;
    }
}

// #included from: catch_version.hpp
#define TWOBLUECUBES_CATCH_VERSION_HPP_INCLUDED

namespace Catch {

    // These numbers are maintained by a script
    template <typename T>
    const T LibraryVersionInfo<T>::value( 1, 0, 35, "master" );
}

// #included from: catch_message.hpp
#define TWOBLUECUBES_CATCH_MESSAGE_HPP_INCLUDED

namespace Catch {

    template <typename T>
    struct MessageInfoCounter {
        // This may need protecting if threading support is added
        static T globalCount;
    };
    template <typename T>
    T MessageInfoCounter<T>::globalCount = T();

    INTERNAL_CATCH_INLINE MessageInfo::MessageInfo(   std::string const& _macroName,
                                SourceLineInfo const& _lineInfo,
                                ResultWas::OfType _type )
    :   macroName( _macroName ),
        lineInfo( _lineInfo ),
        type( _type ),
        sequence( ++MessageInfoCounter<unsigned int>::globalCount )
    {}

    ////////////////////////////////////////////////////////////////////////////

    INTERNAL_CATCH_INLINE ScopedMessage::ScopedMessage( MessageBuilder const& builder )
    : m_info( builder.m_info )
    {
        m_info.message = builder.m_stream.str();
        getResultCapture().pushScopedMessage( m_info );
    }
    INTERNAL_CATCH_INLINE ScopedMessage::~ScopedMessage() {
        getResultCapture().popScopedMessage( m_info );
    }

} // end namespace Catch

// #included from: catch_legacy_reporter_adapter.hpp
#define TWOBLUECUBES_CATCH_LEGACY_REPORTER_ADAPTER_HPP_INCLUDED

// #included from: catch_legacy_reporter_adapter.h
#define TWOBLUECUBES_CATCH_LEGACY_REPORTER_ADAPTER_H_INCLUDED

namespace Catch
{
    // Deprecated
    struct IReporter : IShared {
        virtual ~IReporter();

        virtual bool shouldRedirectStdout() const = 0;

        virtual void StartTesting() = 0;
        virtual void EndTesting( Totals const& totals ) = 0;
        virtual void StartGroup( std::string const& groupName ) = 0;
        virtual void EndGroup( std::string const& groupName, Totals const& totals ) = 0;
        virtual void StartTestCase( TestCaseInfo const& testInfo ) = 0;
        virtual void EndTestCase( TestCaseInfo const& testInfo, Totals const& totals, std::string const& stdOut, std::string const& stdErr ) = 0;
        virtual void StartSection( std::string const& sectionName, std::string const& description ) = 0;
        virtual void EndSection( std::string const& sectionName, Counts const& assertions ) = 0;
        virtual void NoAssertionsInSection( std::string const& sectionName ) = 0;
        virtual void NoAssertionsInTestCase( std::string const& testName ) = 0;
        virtual void Aborted() = 0;
        virtual void Result( AssertionResult const& result ) = 0;
    };

    class LegacyReporterAdapter : public SharedImpl<IStreamingReporter>
    {
    public:
        LegacyReporterAdapter( Ptr<IReporter> const& legacyReporter );
        virtual ~LegacyReporterAdapter();

        virtual ReporterPreferences getPreferences() const;
        virtual void noMatchingTestCases( std::string const& );
        virtual void testRunStarting( TestRunInfo const& );
        virtual void testGroupStarting( GroupInfo const& groupInfo );
        virtual void testCaseStarting( TestCaseInfo const& testInfo );
        virtual void sectionStarting( SectionInfo const& sectionInfo );
        virtual void assertionStarting( AssertionInfo const& );
        virtual bool assertionEnded( AssertionStats const& assertionStats );
        virtual void sectionEnded( SectionStats const& sectionStats );
        virtual void testCaseEnded( TestCaseStats const& testCaseStats );
        virtual void testGroupEnded( TestGroupStats const& testGroupStats );
        virtual void testRunEnded( TestRunStats const& testRunStats );

    private:
        Ptr<IReporter> m_legacyReporter;
    };
}

namespace Catch
{
    INTERNAL_CATCH_INLINE LegacyReporterAdapter::LegacyReporterAdapter( Ptr<IReporter> const& legacyReporter )
    :   m_legacyReporter( legacyReporter )
    {}
    INTERNAL_CATCH_INLINE LegacyReporterAdapter::~LegacyReporterAdapter() {}

    INTERNAL_CATCH_INLINE ReporterPreferences LegacyReporterAdapter::getPreferences() const {
        ReporterPreferences prefs;
        prefs.shouldRedirectStdOut = m_legacyReporter->shouldRedirectStdout();
        return prefs;
    }

    INTERNAL_CATCH_INLINE void LegacyReporterAdapter::noMatchingTestCases( std::string const& ) {}
    INTERNAL_CATCH_INLINE void LegacyReporterAdapter::testRunStarting( TestRunInfo const& ) {
        m_legacyReporter->StartTesting();
    }
    INTERNAL_CATCH_INLINE void LegacyReporterAdapter::testGroupStarting( GroupInfo const& groupInfo ) {
        m_legacyReporter->StartGroup( groupInfo.name );
    }
    INTERNAL_CATCH_INLINE void LegacyReporterAdapter::testCaseStarting( TestCaseInfo const& testInfo ) {
        m_legacyReporter->StartTestCase( testInfo );
    }
    INTERNAL_CATCH_INLINE void LegacyReporterAdapter::sectionStarting( SectionInfo const& sectionInfo ) {
        m_legacyReporter->StartSection( sectionInfo.name, sectionInfo.description );
    }
    INTERNAL_CATCH_INLINE void LegacyReporterAdapter::assertionStarting( AssertionInfo const& ) {
        // Not on legacy interface
    }

    INTERNAL_CATCH_INLINE bool LegacyReporterAdapter::assertionEnded( AssertionStats const& assertionStats ) {
        if( assertionStats.assertionResult.getResultType() != ResultWas::Ok ) {
            for( std::vector<MessageInfo>::const_iterator it = assertionStats.infoMessages.begin(), itEnd = assertionStats.infoMessages.end();
                    it != itEnd;
                    ++it ) {
                if( it->type == ResultWas::Info ) {
                    ExpressionResultBuilder expressionBuilder( it->type );
                        expressionBuilder << it->message;
                    AssertionInfo info( it->macroName, it->lineInfo, "", ResultDisposition::Normal );
                    AssertionResult result = expressionBuilder.buildResult( info );
                    m_legacyReporter->Result( result );
                }
            }
        }
        m_legacyReporter->Result( assertionStats.assertionResult );
        return true;
    }
    INTERNAL_CATCH_INLINE void LegacyReporterAdapter::sectionEnded( SectionStats const& sectionStats ) {
        if( sectionStats.missingAssertions )
            m_legacyReporter->NoAssertionsInSection( sectionStats.sectionInfo.name );
        m_legacyReporter->EndSection( sectionStats.sectionInfo.name, sectionStats.assertions );
    }
    INTERNAL_CATCH_INLINE void LegacyReporterAdapter::testCaseEnded( TestCaseStats const& testCaseStats ) {
        m_legacyReporter->EndTestCase
            (   testCaseStats.testInfo,
                testCaseStats.totals,
                testCaseStats.stdOut,
                testCaseStats.stdErr );
    }
    INTERNAL_CATCH_INLINE void LegacyReporterAdapter::testGroupEnded( TestGroupStats const& testGroupStats ) {
        if( testGroupStats.aborting )
            m_legacyReporter->Aborted();
        m_legacyReporter->EndGroup( testGroupStats.groupInfo.name, testGroupStats.totals );
    }
    INTERNAL_CATCH_INLINE void LegacyReporterAdapter::testRunEnded( TestRunStats const& testRunStats ) {
        m_legacyReporter->EndTesting( testRunStats.totals );
    }
}

// #included from: catch_timer.hpp

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++11-long-long"
#endif

#ifdef CATCH_PLATFORM_WINDOWS
#include <windows.h>
#else
#include <sys/time.h>
#endif

namespace Catch {

    namespace {
#ifdef CATCH_PLATFORM_WINDOWS
        template <typename T>
        struct CounterDefaults
        {
            static T hz;
            static T hzo;
        };
        template <typename T>
        T CounterDefaults<T>::hz = 0;
        template <typename T>
        T CounterDefaults<T>::hzo = 0;

        INTERNAL_CATCH_INLINE uint64_t getCurrentTicks() {
            if (!CounterDefaults<uint64_t>::hz) {
                QueryPerformanceFrequency((LARGE_INTEGER*)&CounterDefaults<uint64_t>::hz);
                QueryPerformanceCounter((LARGE_INTEGER*)&CounterDefaults<uint64_t>::hzo);
            }
            uint64_t t;
            QueryPerformanceCounter((LARGE_INTEGER*)&t);
            return ((t-CounterDefaults<uint64_t>::hzo)*1000000)/CounterDefaults<uint64_t>::hz;
        }
#else
        INTERNAL_CATCH_INLINE uint64_t getCurrentTicks() {
            timeval t;
            gettimeofday(&t,NULL);
            return (uint64_t)t.tv_sec * 1000000ull + (uint64_t)t.tv_usec;
        }
#endif
    }

    INTERNAL_CATCH_INLINE void Timer::start() {
        m_ticks = getCurrentTicks();
    }
    INTERNAL_CATCH_INLINE unsigned int Timer::getElapsedNanoseconds() const {
        return (unsigned int)(getCurrentTicks() - m_ticks);
    }
    INTERNAL_CATCH_INLINE unsigned int Timer::getElapsedMilliseconds() const {
        return (unsigned int)((getCurrentTicks() - m_ticks)/1000);
    }
    INTERNAL_CATCH_INLINE double Timer::getElapsedSeconds() const {
        return (getCurrentTicks() - m_ticks)/1000000.0;
    }

} // namespace Catch

#ifdef __clang__
#pragma clang diagnostic pop
#endif
// #included from: catch_common.hpp
#define TWOBLUECUBES_CATCH_COMMON_HPP_INCLUDED

namespace Catch {

    INTERNAL_CATCH_INLINE bool startsWith( std::string const& s, std::string const& prefix ) {
        return s.size() >= prefix.size() && s.substr( 0, prefix.size() ) == prefix;
    }
    INTERNAL_CATCH_INLINE bool endsWith( std::string const& s, std::string const& suffix ) {
        return s.size() >= suffix.size() && s.substr( s.size()-suffix.size(), suffix.size() ) == suffix;
    }
    INTERNAL_CATCH_INLINE bool contains( std::string const& s, std::string const& infix ) {
        return s.find( infix ) != std::string::npos;
    }
    INTERNAL_CATCH_INLINE void toLowerInPlace( std::string& s ) {
        std::transform( s.begin(), s.end(), s.begin(), ::tolower );
    }
    INTERNAL_CATCH_INLINE std::string toLower( std::string const& s ) {
        std::string lc = s;
        toLowerInPlace( lc );
        return lc;
    }
    INTERNAL_CATCH_INLINE std::string trim( std::string const& str ) {
        static char const* whitespaceChars = "\n\r\t ";
        std::string::size_type start = str.find_first_not_of( whitespaceChars );
        std::string::size_type end = str.find_last_not_of( whitespaceChars );

        return start != std::string::npos ? str.substr( start, 1+end-start ) : "";
    }

    INTERNAL_CATCH_INLINE pluralise::pluralise( std::size_t count, std::string const& label )
    :   m_count( count ),
        m_label( label )
    {}

    INTERNAL_CATCH_INLINE std::ostream& operator << ( std::ostream& os, pluralise const& pluraliser ) {
        os << pluraliser.m_count << " " << pluraliser.m_label;
        if( pluraliser.m_count != 1 )
            os << "s";
        return os;
    }

    INTERNAL_CATCH_INLINE SourceLineInfo::SourceLineInfo() : line( 0 ){}
    INTERNAL_CATCH_INLINE SourceLineInfo::SourceLineInfo( char const* _file, std::size_t _line )
    :   file( _file ),
        line( _line )
    {}
    INTERNAL_CATCH_INLINE SourceLineInfo::SourceLineInfo( SourceLineInfo const& other )
    :   file( other.file ),
        line( other.line )
    {}
    INTERNAL_CATCH_INLINE bool SourceLineInfo::empty() const {
        return file.empty();
    }
    INTERNAL_CATCH_INLINE bool SourceLineInfo::operator == ( SourceLineInfo const& other ) const {
        return line == other.line && file == other.file;
    }

    INTERNAL_CATCH_INLINE std::ostream& operator << ( std::ostream& os, SourceLineInfo const& info ) {
#ifndef __GNUG__
        os << info.file << "(" << info.line << ")";
#else
        os << info.file << ":" << info.line;
#endif
        return os;
    }

    INTERNAL_CATCH_INLINE void throwLogicError( std::string const& message, SourceLineInfo const& locationInfo ) {
        std::ostringstream oss;
        oss << locationInfo << ": Internal Catch error: '" << message << "'";
        if( isTrue( true ))
            throw std::logic_error( oss.str() );
    }
}

// #included from: catch_section.hpp
#define TWOBLUECUBES_CATCH_SECTION_HPP_INCLUDED

namespace Catch {

    INTERNAL_CATCH_INLINE Section::Section(   SourceLineInfo const& lineInfo,
                        std::string const& name,
                        std::string const& description )
    :   m_info( name, description, lineInfo ),
        m_sectionIncluded( getCurrentContext().getResultCapture().sectionStarted( m_info, m_assertions ) )
    {
        m_timer.start();
    }

    INTERNAL_CATCH_INLINE Section::~Section() {
        if( m_sectionIncluded )
            getCurrentContext().getResultCapture().sectionEnded( m_info, m_assertions, m_timer.getElapsedSeconds() );
    }

    // This indicates whether the section should be executed or not
    INTERNAL_CATCH_INLINE Section::operator bool() {
        return m_sectionIncluded;
    }

} // end namespace Catch

// #included from: catch_debugger.hpp
#define TWOBLUECUBES_CATCH_DEBUGGER_HPP_INCLUDED

#include <iostream>

#ifdef CATCH_PLATFORM_MAC

    #include <assert.h>
    #include <stdbool.h>
    #include <sys/types.h>
    #include <unistd.h>
    #include <sys/sysctl.h>

    namespace Catch{

        // The following function is taken directly from the following technical note:
        // http://developer.apple.com/library/mac/#qa/qa2004/qa1361.html

        // Returns true if the current process is being debugged (either
        // running under the debugger or has a debugger attached post facto).
        INTERNAL_CATCH_INLINE bool isDebuggerActive(){

            int                 mib[4];
            struct kinfo_proc   info;
            size_t              size;

            // Initialize the flags so that, if sysctl fails for some bizarre
            // reason, we get a predictable result.

            info.kp_proc.p_flag = 0;

            // Initialize mib, which tells sysctl the info we want, in this case
            // we're looking for information about a specific process ID.

            mib[0] = CTL_KERN;
            mib[1] = KERN_PROC;
            mib[2] = KERN_PROC_PID;
            mib[3] = getpid();

            // Call sysctl.

            size = sizeof(info);
            if( sysctl(mib, sizeof(mib) / sizeof(*mib), &info, &size, NULL, 0) != 0 ) {
                std::cerr << "\n** Call to sysctl failed - unable to determine if debugger is active **\n" << std::endl;
                return false;
            }

            // We're being debugged if the P_TRACED flag is set.

            return ( (info.kp_proc.p_flag & P_TRACED) != 0 );
        }
    } // namespace Catch

#elif defined(_MSC_VER)
    extern "C" __declspec(dllimport) int __stdcall IsDebuggerPresent();
    namespace Catch {
        INTERNAL_CATCH_INLINE bool isDebuggerActive() {
            return IsDebuggerPresent() != 0;
        }
    }
#elif defined(__MINGW32__)
    extern "C" __declspec(dllimport) int __stdcall IsDebuggerPresent();
    namespace Catch {
        INTERNAL_CATCH_INLINE bool isDebuggerActive() {
            return IsDebuggerPresent() != 0;
        }
    }
#else
    namespace Catch {
       inline bool isDebuggerActive() { return false; }
    }
#endif // Platform

#ifdef CATCH_PLATFORM_WINDOWS
    extern "C" __declspec(dllimport) void __stdcall OutputDebugStringA( const char* );
    namespace Catch {
        INTERNAL_CATCH_INLINE void writeToDebugConsole( std::string const& text ) {
            ::OutputDebugStringA( text.c_str() );
        }
    }
#else
    namespace Catch {
        INTERNAL_CATCH_INLINE void writeToDebugConsole( std::string const& text ) {
            // !TBD: Need a version for Mac/ XCode and other IDEs
            std::cout << text;
        }
    }
#endif // Platform

// #included from: ../reporters/catch_reporter_xml.hpp
#define TWOBLUECUBES_CATCH_REPORTER_XML_HPP_INCLUDED

// #included from: catch_reporter_bases.hpp
#define TWOBLUECUBES_CATCH_REPORTER_BASES_HPP_INCLUDED

namespace Catch {

    struct StreamingReporterBase : SharedImpl<IStreamingReporter> {

        StreamingReporterBase( ReporterConfig const& _config )
        :   m_config( _config.fullConfig() ),
            stream( _config.stream() )
        {}

        virtual ~StreamingReporterBase();

        virtual void noMatchingTestCases( std::string const& ) {}

        virtual void testRunStarting( TestRunInfo const& _testRunInfo ) {
            currentTestRunInfo = _testRunInfo;
        }
        virtual void testGroupStarting( GroupInfo const& _groupInfo ) {
            currentGroupInfo = _groupInfo;
        }

        virtual void testCaseStarting( TestCaseInfo const& _testInfo ) {
            currentTestCaseInfo = _testInfo;
        }
        virtual void sectionStarting( SectionInfo const& _sectionInfo ) {
            m_sectionStack.push_back( _sectionInfo );
        }

        virtual void sectionEnded( SectionStats const& /* _sectionStats */ ) {
            m_sectionStack.pop_back();
        }
        virtual void testCaseEnded( TestCaseStats const& /* _testCaseStats */ ) {
            currentTestCaseInfo.reset();
            assert( m_sectionStack.empty() );
        }
        virtual void testGroupEnded( TestGroupStats const& /* _testGroupStats */ ) {
            currentGroupInfo.reset();
        }
        virtual void testRunEnded( TestRunStats const& /* _testRunStats */ ) {
            currentTestCaseInfo.reset();
            currentGroupInfo.reset();
            currentTestRunInfo.reset();
        }

        Ptr<IConfig> m_config;
        std::ostream& stream;

        LazyStat<TestRunInfo> currentTestRunInfo;
        LazyStat<GroupInfo> currentGroupInfo;
        LazyStat<TestCaseInfo> currentTestCaseInfo;

        std::vector<SectionInfo> m_sectionStack;
    };

    struct CumulativeReporterBase : SharedImpl<IStreamingReporter> {
        template<typename T, typename ChildNodeT>
        struct Node : SharedImpl<> {
            explicit Node( T const& _value ) : value( _value ) {}
            virtual ~Node() {}

            typedef std::vector<Ptr<ChildNodeT> > ChildNodes;
            T value;
            ChildNodes children;
        };
        struct SectionNode : SharedImpl<> {
            explicit SectionNode( SectionStats const& _stats ) : stats( _stats ) {}
            virtual ~SectionNode();

            bool operator == ( SectionNode const& other ) const {
                return stats.sectionInfo.lineInfo == other.stats.sectionInfo.lineInfo;
            }
            bool operator == ( Ptr<SectionNode> const& other ) const {
                return operator==( *other );
            }

            SectionStats stats;
            typedef std::vector<Ptr<SectionNode> > ChildSections;
            typedef std::vector<AssertionStats> Assertions;
            ChildSections childSections;
            Assertions assertions;
            std::string stdOut;
            std::string stdErr;
        };

        struct BySectionInfo {
            BySectionInfo( SectionInfo const& other ) : m_other( other ) {}
            bool operator() ( Ptr<SectionNode> const& node ) const {
                return node->stats.sectionInfo.lineInfo == m_other.lineInfo;
            }
        private:
            BySectionInfo& operator=( BySectionInfo const& other ); // = delete;

            SectionInfo const& m_other;
        };

        typedef Node<TestCaseStats, SectionNode> TestCaseNode;
        typedef Node<TestGroupStats, TestCaseNode> TestGroupNode;
        typedef Node<TestRunStats, TestGroupNode> TestRunNode;

        CumulativeReporterBase( ReporterConfig const& _config )
        :   m_config( _config.fullConfig() ),
            stream( _config.stream() )
        {}
        ~CumulativeReporterBase();

        virtual void testRunStarting( TestRunInfo const& ) {}
        virtual void testGroupStarting( GroupInfo const& ) {}

        virtual void testCaseStarting( TestCaseInfo const& ) {}

        virtual void sectionStarting( SectionInfo const& sectionInfo ) {
            SectionStats incompleteStats( sectionInfo, Counts(), 0, false );
            Ptr<SectionNode> node;
            if( m_sectionStack.empty() ) {
                if( !m_rootSection )
                    m_rootSection = new SectionNode( incompleteStats );
                node = m_rootSection;
            }
            else {
                SectionNode& parentNode = *m_sectionStack.back();
                SectionNode::ChildSections::const_iterator it =
                    std::find_if(   parentNode.childSections.begin(),
                                    parentNode.childSections.end(),
                                    BySectionInfo( sectionInfo ) );
                if( it == parentNode.childSections.end() ) {
                    node = new SectionNode( incompleteStats );
                    parentNode.childSections.push_back( node );
                }
                else
                    node = *it;
            }
            m_sectionStack.push_back( node );
            m_deepestSection = node;
        }

        virtual void assertionStarting( AssertionInfo const& ) {}

        virtual bool assertionEnded( AssertionStats const& assertionStats ) {
            assert( !m_sectionStack.empty() );
            SectionNode& sectionNode = *m_sectionStack.back();
            sectionNode.assertions.push_back( assertionStats );
            return true;
        }
        virtual void sectionEnded( SectionStats const& sectionStats ) {
            assert( !m_sectionStack.empty() );
            SectionNode& node = *m_sectionStack.back();
            node.stats = sectionStats;
            m_sectionStack.pop_back();
        }
        virtual void testCaseEnded( TestCaseStats const& testCaseStats ) {
            Ptr<TestCaseNode> node = new TestCaseNode( testCaseStats );
            assert( m_sectionStack.size() == 0 );
            node->children.push_back( m_rootSection );
            m_testCases.push_back( node );
            m_rootSection.reset();

            assert( m_deepestSection );
            m_deepestSection->stdOut = testCaseStats.stdOut;
            m_deepestSection->stdErr = testCaseStats.stdErr;
        }
        virtual void testGroupEnded( TestGroupStats const& testGroupStats ) {
            Ptr<TestGroupNode> node = new TestGroupNode( testGroupStats );
            node->children.swap( m_testCases );
            m_testGroups.push_back( node );
        }
        virtual void testRunEnded( TestRunStats const& testRunStats ) {
            Ptr<TestRunNode> node = new TestRunNode( testRunStats );
            node->children.swap( m_testGroups );
            m_testRuns.push_back( node );
            testRunEndedCumulative();
        }
        virtual void testRunEndedCumulative() = 0;

        Ptr<IConfig> m_config;
        std::ostream& stream;
        std::vector<AssertionStats> m_assertions;
        std::vector<std::vector<Ptr<SectionNode> > > m_sections;
        std::vector<Ptr<TestCaseNode> > m_testCases;
        std::vector<Ptr<TestGroupNode> > m_testGroups;

        std::vector<Ptr<TestRunNode> > m_testRuns;

        Ptr<SectionNode> m_rootSection;
        Ptr<SectionNode> m_deepestSection;
        std::vector<Ptr<SectionNode> > m_sectionStack;

    };

} // end namespace Catch

// #included from: ../internal/catch_reporter_registrars.hpp
#define TWOBLUECUBES_CATCH_REPORTER_REGISTRARS_HPP_INCLUDED

namespace Catch {

    template<typename T>
    class LegacyReporterRegistrar {

        class ReporterFactory : public IReporterFactory {
            virtual IStreamingReporter* create( ReporterConfig const& config ) const {
                return new LegacyReporterAdapter( new T( config ) );
            }

            virtual std::string getDescription() const {
                return T::getDescription();
            }
        };

    public:

        LegacyReporterRegistrar( std::string const& name ) {
            getMutableRegistryHub().registerReporter( name, new ReporterFactory() );
        }
    };

    template<typename T>
    class ReporterRegistrar {

        class ReporterFactory : public IReporterFactory {

            // *** Please Note ***:
            // - If you end up here looking at a compiler error because it's trying to register
            // your custom reporter class be aware that the native reporter interface has changed
            // to IStreamingReporter. The "legacy" interface, IReporter, is still supported via
            // an adapter. Just use REGISTER_LEGACY_REPORTER to take advantage of the adapter.
            // However please consider updating to the new interface as the old one is now
            // deprecated and will probably be removed quite soon!
            // Please contact me via github if you have any questions at all about this.
            // In fact, ideally, please contact me anyway to let me know you've hit this - as I have
            // no idea who is actually using custom reporters at all (possibly no-one!).
            // The new interface is designed to minimise exposure to interface changes in the future.
            virtual IStreamingReporter* create( ReporterConfig const& config ) const {
                return new T( config );
            }

            virtual std::string getDescription() const {
                return T::getDescription();
            }
        };

    public:

        ReporterRegistrar( std::string const& name ) {
            getMutableRegistryHub().registerReporter( name, new ReporterFactory() );
        }
    };
}

#define INTERNAL_CATCH_REGISTER_LEGACY_REPORTER( name, reporterType ) \
    namespace{ Catch::LegacyReporterRegistrar<reporterType> catch_internal_RegistrarFor##reporterType( name ); }
#define INTERNAL_CATCH_REGISTER_REPORTER( name, reporterType ) \
    namespace{ Catch::ReporterRegistrar<reporterType> catch_internal_RegistrarFor##reporterType( name ); }

// #included from: ../internal/catch_xmlwriter.hpp
#define TWOBLUECUBES_CATCH_XMLWRITER_HPP_INCLUDED

#include <sstream>
#include <iostream>
#include <string>
#include <vector>

namespace Catch {

    class XmlWriter {
    public:

        class ScopedElement {
        public:
            ScopedElement( XmlWriter* writer )
            :   m_writer( writer )
            {}

            ScopedElement( ScopedElement const& other )
            :   m_writer( other.m_writer ){
                other.m_writer = NULL;
            }

            ~ScopedElement() {
                if( m_writer )
                    m_writer->endElement();
            }

            ScopedElement& writeText( std::string const& text, bool indent = true ) {
                m_writer->writeText( text, indent );
                return *this;
            }

            template<typename T>
            ScopedElement& writeAttribute( std::string const& name, T const& attribute ) {
                m_writer->writeAttribute( name, attribute );
                return *this;
            }

        private:
            mutable XmlWriter* m_writer;
        };

        XmlWriter()
        :   m_tagIsOpen( false ),
            m_needsNewline( false ),
            m_os( &std::cout )
        {}

        XmlWriter( std::ostream& os )
        :   m_tagIsOpen( false ),
            m_needsNewline( false ),
            m_os( &os )
        {}

        ~XmlWriter() {
            while( !m_tags.empty() )
                endElement();
        }

        XmlWriter& operator = ( XmlWriter const& other ) {
            XmlWriter temp( other );
            swap( temp );
            return *this;
        }

        void swap( XmlWriter& other ) {
            std::swap( m_tagIsOpen, other.m_tagIsOpen );
            std::swap( m_needsNewline, other.m_needsNewline );
            std::swap( m_tags, other.m_tags );
            std::swap( m_indent, other.m_indent );
            std::swap( m_os, other.m_os );
        }

        XmlWriter& startElement( std::string const& name ) {
            ensureTagClosed();
            newlineIfNecessary();
            stream() << m_indent << "<" << name;
            m_tags.push_back( name );
            m_indent += "  ";
            m_tagIsOpen = true;
            return *this;
        }

        ScopedElement scopedElement( std::string const& name ) {
            ScopedElement scoped( this );
            startElement( name );
            return scoped;
        }

        XmlWriter& endElement() {
            newlineIfNecessary();
            m_indent = m_indent.substr( 0, m_indent.size()-2 );
            if( m_tagIsOpen ) {
                stream() << "/>\n";
                m_tagIsOpen = false;
            }
            else {
                stream() << m_indent << "</" << m_tags.back() << ">\n";
            }
            m_tags.pop_back();
            return *this;
        }

        XmlWriter& writeAttribute( std::string const& name, std::string const& attribute ) {
            if( !name.empty() && !attribute.empty() ) {
                stream() << " " << name << "=\"";
                writeEncodedText( attribute );
                stream() << "\"";
            }
            return *this;
        }

        XmlWriter& writeAttribute( std::string const& name, bool attribute ) {
            stream() << " " << name << "=\"" << ( attribute ? "true" : "false" ) << "\"";
            return *this;
        }

        template<typename T>
        XmlWriter& writeAttribute( std::string const& name, T const& attribute ) {
            if( !name.empty() )
                stream() << " " << name << "=\"" << attribute << "\"";
            return *this;
        }

        XmlWriter& writeText( std::string const& text, bool indent = true ) {
            if( !text.empty() ){
                bool tagWasOpen = m_tagIsOpen;
                ensureTagClosed();
                if( tagWasOpen && indent )
                    stream() << m_indent;
                writeEncodedText( text );
                m_needsNewline = true;
            }
            return *this;
        }

        XmlWriter& writeComment( std::string const& text ) {
            ensureTagClosed();
            stream() << m_indent << "<!--" << text << "-->";
            m_needsNewline = true;
            return *this;
        }

        XmlWriter& writeBlankLine() {
            ensureTagClosed();
            stream() << "\n";
            return *this;
        }

    private:

        std::ostream& stream() {
            return *m_os;
        }

        void ensureTagClosed() {
            if( m_tagIsOpen ) {
                stream() << ">\n";
                m_tagIsOpen = false;
            }
        }

        void newlineIfNecessary() {
            if( m_needsNewline ) {
                stream() << "\n";
                m_needsNewline = false;
            }
        }

        void writeEncodedText( std::string const& text ) {
            static const char* charsToEncode = "<&\"";
            std::string mtext = text;
            std::string::size_type pos = mtext.find_first_of( charsToEncode );
            while( pos != std::string::npos ) {
                stream() << mtext.substr( 0, pos );

                switch( mtext[pos] ) {
                    case '<':
                        stream() << "&lt;";
                        break;
                    case '&':
                        stream() << "&amp;";
                        break;
                    case '\"':
                        stream() << "&quot;";
                        break;
                }
                mtext = mtext.substr( pos+1 );
                pos = mtext.find_first_of( charsToEncode );
            }
            stream() << mtext;
        }

        bool m_tagIsOpen;
        bool m_needsNewline;
        std::vector<std::string> m_tags;
        std::string m_indent;
        std::ostream* m_os;
    };

}
namespace Catch {
    class XmlReporter : public SharedImpl<IReporter> {
    public:
        XmlReporter( ReporterConfig const& config ) : m_config( config ), m_sectionDepth( 0 ) {}

        static std::string getDescription() {
            return "Reports test results as an XML document";
        }
        virtual ~XmlReporter();

    private: // IReporter

        virtual bool shouldRedirectStdout() const {
            return true;
        }

        virtual void StartTesting() {
            m_xml = XmlWriter( m_config.stream() );
            m_xml.startElement( "Catch" );
            if( !m_config.fullConfig()->name().empty() )
                m_xml.writeAttribute( "name", m_config.fullConfig()->name() );
        }

        virtual void EndTesting( const Totals& totals ) {
            m_xml.scopedElement( "OverallResults" )
                .writeAttribute( "successes", totals.assertions.passed )
                .writeAttribute( "failures", totals.assertions.failed );
            m_xml.endElement();
        }

        virtual void StartGroup( const std::string& groupName ) {
            m_xml.startElement( "Group" )
                .writeAttribute( "name", groupName );
        }

        virtual void EndGroup( const std::string&, const Totals& totals ) {
            m_xml.scopedElement( "OverallResults" )
                .writeAttribute( "successes", totals.assertions.passed )
                .writeAttribute( "failures", totals.assertions.failed );
            m_xml.endElement();
        }

        virtual void StartSection( const std::string& sectionName, const std::string& description ) {
            if( m_sectionDepth++ > 0 ) {
                m_xml.startElement( "Section" )
                    .writeAttribute( "name", trim( sectionName ) )
                    .writeAttribute( "description", description );
            }
        }
        virtual void NoAssertionsInSection( const std::string& ) {}
        virtual void NoAssertionsInTestCase( const std::string& ) {}

        virtual void EndSection( const std::string& /*sectionName*/, const Counts& assertions ) {
            if( --m_sectionDepth > 0 ) {
                m_xml.scopedElement( "OverallResults" )
                    .writeAttribute( "successes", assertions.passed )
                    .writeAttribute( "failures", assertions.failed );
                m_xml.endElement();
            }
        }

        virtual void StartTestCase( const Catch::TestCaseInfo& testInfo ) {
            m_xml.startElement( "TestCase" ).writeAttribute( "name", trim( testInfo.name ) );
            m_currentTestSuccess = true;
        }

        virtual void Result( const Catch::AssertionResult& assertionResult ) {
            if( !m_config.fullConfig()->includeSuccessfulResults() && assertionResult.getResultType() == ResultWas::Ok )
                return;

            if( assertionResult.hasExpression() ) {
                m_xml.startElement( "Expression" )
                    .writeAttribute( "success", assertionResult.succeeded() )
                    .writeAttribute( "filename", assertionResult.getSourceInfo().file )
                    .writeAttribute( "line", assertionResult.getSourceInfo().line );

                m_xml.scopedElement( "Original" )
                    .writeText( assertionResult.getExpression() );
                m_xml.scopedElement( "Expanded" )
                    .writeText( assertionResult.getExpandedExpression() );
                m_currentTestSuccess &= assertionResult.succeeded();
            }

            switch( assertionResult.getResultType() ) {
                case ResultWas::ThrewException:
                    m_xml.scopedElement( "Exception" )
                        .writeAttribute( "filename", assertionResult.getSourceInfo().file )
                        .writeAttribute( "line", assertionResult.getSourceInfo().line )
                        .writeText( assertionResult.getMessage() );
                    m_currentTestSuccess = false;
                    break;
                case ResultWas::Info:
                    m_xml.scopedElement( "Info" )
                        .writeText( assertionResult.getMessage() );
                    break;
                case ResultWas::Warning:
                    m_xml.scopedElement( "Warning" )
                        .writeText( assertionResult.getMessage() );
                    break;
                case ResultWas::ExplicitFailure:
                    m_xml.scopedElement( "Failure" )
                        .writeText( assertionResult.getMessage() );
                    m_currentTestSuccess = false;
                    break;
                case ResultWas::Unknown:
                case ResultWas::Ok:
                case ResultWas::FailureBit:
                case ResultWas::ExpressionFailed:
                case ResultWas::Exception:
                case ResultWas::DidntThrowException:
                    break;
            }
            if( assertionResult.hasExpression() )
                m_xml.endElement();
        }

        virtual void Aborted() {
            // !TBD
        }

        virtual void EndTestCase( const Catch::TestCaseInfo&, const Totals&, const std::string&, const std::string& ) {
            m_xml.scopedElement( "OverallResult" ).writeAttribute( "success", m_currentTestSuccess );
            m_xml.endElement();
        }

    private:
        ReporterConfig m_config;
        bool m_currentTestSuccess;
        XmlWriter m_xml;
        int m_sectionDepth;
    };

} // end namespace Catch

// #included from: ../reporters/catch_reporter_junit.hpp
#define TWOBLUECUBES_CATCH_REPORTER_JUNIT_HPP_INCLUDED

#include <assert.h>

namespace Catch {

    class JunitReporter : public CumulativeReporterBase {
    public:
        JunitReporter( ReporterConfig const& _config )
        :   CumulativeReporterBase( _config ),
            xml( _config.stream() )
        {}

        ~JunitReporter();

        static std::string getDescription() {
            return "Reports test results in an XML format that looks like Ant's junitreport target";
        }

        virtual void noMatchingTestCases( std::string const& /*spec*/ ) {}

        virtual ReporterPreferences getPreferences() const {
            ReporterPreferences prefs;
            prefs.shouldRedirectStdOut = true;
            return prefs;
        }

        virtual void testRunStarting( TestRunInfo const& runInfo ) {
            CumulativeReporterBase::testRunStarting( runInfo );
            xml.startElement( "testsuites" );
        }

        virtual void testGroupStarting( GroupInfo const& groupInfo ) {
            suiteTimer.start();
            stdOutForSuite.str("");
            stdErrForSuite.str("");
            unexpectedExceptions = 0;
            CumulativeReporterBase::testGroupStarting( groupInfo );
        }

        virtual bool assertionEnded( AssertionStats const& assertionStats ) {
            if( assertionStats.assertionResult.getResultType() == ResultWas::ThrewException )
                unexpectedExceptions++;
            return CumulativeReporterBase::assertionEnded( assertionStats );
        }

        virtual void testCaseEnded( TestCaseStats const& testCaseStats ) {
            stdOutForSuite << testCaseStats.stdOut;
            stdErrForSuite << testCaseStats.stdErr;
            CumulativeReporterBase::testCaseEnded( testCaseStats );
        }

        virtual void testGroupEnded( TestGroupStats const& testGroupStats ) {
            double suiteTime = suiteTimer.getElapsedSeconds();
            CumulativeReporterBase::testGroupEnded( testGroupStats );
            writeGroup( *m_testGroups.back(), suiteTime );
        }

        virtual void testRunEndedCumulative() {
            xml.endElement();
        }

        void writeGroup( TestGroupNode const& groupNode, double suiteTime ) {
            XmlWriter::ScopedElement e = xml.scopedElement( "testsuite" );
            TestGroupStats const& stats = groupNode.value;
            xml.writeAttribute( "name", stats.groupInfo.name );
            xml.writeAttribute( "errors", unexpectedExceptions );
            xml.writeAttribute( "failures", stats.totals.assertions.failed-unexpectedExceptions );
            xml.writeAttribute( "tests", stats.totals.assertions.total() );
            xml.writeAttribute( "hostname", "tbd" ); // !TBD
            if( m_config->showDurations() == ShowDurations::Never )
                xml.writeAttribute( "time", "" );
            else
                xml.writeAttribute( "time", suiteTime );
            xml.writeAttribute( "timestamp", "tbd" ); // !TBD

            // Write test cases
            for( TestGroupNode::ChildNodes::const_iterator
                    it = groupNode.children.begin(), itEnd = groupNode.children.end();
                    it != itEnd;
                    ++it )
                writeTestCase( **it );

            xml.scopedElement( "system-out" ).writeText( trim( stdOutForSuite.str() ), false );
            xml.scopedElement( "system-err" ).writeText( trim( stdErrForSuite.str() ), false );
        }

        void writeTestCase( TestCaseNode const& testCaseNode ) {
            TestCaseStats const& stats = testCaseNode.value;

            // All test cases have exactly one section - which represents the
            // test case itself. That section may have 0-n nested sections
            assert( testCaseNode.children.size() == 1 );
            SectionNode const& rootSection = *testCaseNode.children.front();

            std::string className = stats.testInfo.className;

            if( className.empty() ) {
                if( rootSection.childSections.empty() )
                    className = "global";
            }
            writeSection( className, "", rootSection );
        }

        void writeSection(  std::string const& className,
                            std::string const& rootName,
                            SectionNode const& sectionNode ) {
            std::string name = trim( sectionNode.stats.sectionInfo.name );
            if( !rootName.empty() )
                name = rootName + "/" + name;

            if( !sectionNode.assertions.empty() ||
                !sectionNode.stdOut.empty() ||
                !sectionNode.stdErr.empty() ) {
                XmlWriter::ScopedElement e = xml.scopedElement( "testcase" );
                if( className.empty() ) {
                    xml.writeAttribute( "classname", name );
                    xml.writeAttribute( "name", "root" );
                }
                else {
                    xml.writeAttribute( "classname", className );
                    xml.writeAttribute( "name", name );
                }
                xml.writeAttribute( "time", toString( sectionNode.stats.durationInSeconds ) );

                writeAssertions( sectionNode );

                if( !sectionNode.stdOut.empty() )
                    xml.scopedElement( "system-out" ).writeText( trim( sectionNode.stdOut ), false );
                if( !sectionNode.stdErr.empty() )
                    xml.scopedElement( "system-err" ).writeText( trim( sectionNode.stdErr ), false );
            }
            for( SectionNode::ChildSections::const_iterator
                    it = sectionNode.childSections.begin(),
                    itEnd = sectionNode.childSections.end();
                    it != itEnd;
                    ++it )
                if( className.empty() )
                    writeSection( name, "", **it );
                else
                    writeSection( className, name, **it );
        }

        void writeAssertions( SectionNode const& sectionNode ) {
            for( SectionNode::Assertions::const_iterator
                    it = sectionNode.assertions.begin(), itEnd = sectionNode.assertions.end();
                    it != itEnd;
                    ++it )
                writeAssertion( *it );
        }
        void writeAssertion( AssertionStats const& stats ) {
            AssertionResult const& result = stats.assertionResult;
            if( !result.isOk() ) {
                std::string elementName;
                switch( result.getResultType() ) {
                    case ResultWas::ThrewException:
                        elementName = "error";
                        break;
                    case ResultWas::ExplicitFailure:
                        elementName = "failure";
                        break;
                    case ResultWas::ExpressionFailed:
                        elementName = "failure";
                        break;
                    case ResultWas::DidntThrowException:
                        elementName = "failure";
                        break;

                    // We should never see these here:
                    case ResultWas::Info:
                    case ResultWas::Warning:
                    case ResultWas::Ok:
                    case ResultWas::Unknown:
                    case ResultWas::FailureBit:
                    case ResultWas::Exception:
                        elementName = "internalError";
                        break;
                }

                XmlWriter::ScopedElement e = xml.scopedElement( elementName );

                xml.writeAttribute( "message", result.getExpandedExpression() );
                xml.writeAttribute( "type", result.getTestMacroName() );

                std::ostringstream oss;
                if( !result.getMessage().empty() )
                    oss << result.getMessage() << "\n";
                for( std::vector<MessageInfo>::const_iterator
                        it = stats.infoMessages.begin(),
                        itEnd = stats.infoMessages.end();
                            it != itEnd;
                            ++it )
                    if( it->type == ResultWas::Info )
                        oss << it->message << "\n";

                oss << "at " << result.getSourceInfo();
                xml.writeText( oss.str(), false );
            }
        }

        XmlWriter xml;
        Timer suiteTimer;
        std::ostringstream stdOutForSuite;
        std::ostringstream stdErrForSuite;
        unsigned int unexpectedExceptions;
    };

    INTERNAL_CATCH_REGISTER_REPORTER( "junit", JunitReporter )

} // end namespace Catch

// #included from: ../reporters/catch_reporter_console.hpp
#define TWOBLUECUBES_CATCH_REPORTER_CONSOLE_HPP_INCLUDED

namespace Catch {

#if defined(INTERNAL_CATCH_VS_MANAGED) || defined(INTERNAL_CATCH_VS_NATIVE)
    static const bool DefaultRedirectStdout = true;
#else
    static const bool DefaultRedirectStdout = false;
#endif
    struct ConsoleReporter : StreamingReporterBase {
        ConsoleReporter( ReporterConfig const& _config )
        :   StreamingReporterBase( _config ),
            m_headerPrinted( false ),
            m_atLeastOneTestCasePrinted( false )
        {}

        virtual ~ConsoleReporter();
        static std::string getDescription() {
            return "Reports test results as plain lines of text";
        }
        virtual ReporterPreferences getPreferences() const {
            ReporterPreferences prefs;
            prefs.shouldRedirectStdOut = DefaultRedirectStdout;
            return prefs;
        }

        virtual void noMatchingTestCases( std::string const& spec ) {
            stream << "No test cases matched '" << spec << "'" << std::endl;
        }

        virtual void assertionStarting( AssertionInfo const& ) {
        }

        virtual bool assertionEnded( AssertionStats const& _assertionStats ) {
            AssertionResult const& result = _assertionStats.assertionResult;

            bool printInfoMessages = true;

            // Drop out if result was successful and we're not printing those
            if( !m_config->includeSuccessfulResults() && result.isOk() ) {
                if( result.getResultType() != ResultWas::Warning )
                    return false;
                printInfoMessages = false;
            }

            lazyPrint();

            AssertionPrinter printer( stream, _assertionStats, printInfoMessages );
            printer.print();
            stream << std::endl;
            return true;
        }

        virtual void sectionStarting( SectionInfo const& _sectionInfo ) {
            m_headerPrinted = false;
            StreamingReporterBase::sectionStarting( _sectionInfo );
        }
        virtual void sectionEnded( SectionStats const& _sectionStats ) {
            if( _sectionStats.missingAssertions ) {
                lazyPrint();
                Colour colour( Colour::ResultError );
                if( m_sectionStack.size() > 1 )
                    stream << "\nNo assertions in section";
                else
                    stream << "\nNo assertions in test case";
                stream << " '" << _sectionStats.sectionInfo.name << "'\n" << std::endl;
            }
            if( m_headerPrinted ) {
                if( m_config->showDurations() == ShowDurations::Always )
                    stream << "Completed in " << _sectionStats.durationInSeconds << "s" << std::endl;
                m_headerPrinted = false;
            }
            else {
                if( m_config->showDurations() == ShowDurations::Always )
                    stream << _sectionStats.sectionInfo.name << " completed in " << _sectionStats.durationInSeconds << "s" << std::endl;
            }
            StreamingReporterBase::sectionEnded( _sectionStats );
        }

        virtual void testCaseEnded( TestCaseStats const& _testCaseStats ) {
            if( getPreferences().shouldRedirectStdOut )
            {
                stream << _testCaseStats.stdOut;
                stream << _testCaseStats.stdErr;
            }
            StreamingReporterBase::testCaseEnded( _testCaseStats );
            m_headerPrinted = false;
        }
        virtual void testGroupEnded( TestGroupStats const& _testGroupStats ) {
            if( currentGroupInfo.used ) {
                printSummaryDivider();
                stream << "Summary for group '" << _testGroupStats.groupInfo.name << "':\n";
                printTotals( _testGroupStats.totals );
                stream << "\n" << std::endl;
            }
            StreamingReporterBase::testGroupEnded( _testGroupStats );
        }
        virtual void testRunEnded( TestRunStats const& _testRunStats ) {
            if( m_atLeastOneTestCasePrinted )
                printTotalsDivider();
            printTotals( _testRunStats.totals );
            stream << "\n" << std::endl;
            StreamingReporterBase::testRunEnded( _testRunStats );
        }

    private:

        class AssertionPrinter {
            void operator= ( AssertionPrinter const& );
        public:
            AssertionPrinter( std::ostream& _stream, AssertionStats const& _stats, bool _printInfoMessages )
            :   stream( _stream ),
                stats( _stats ),
                result( _stats.assertionResult ),
                colour( Colour::None ),
                message( result.getMessage() ),
                messages( _stats.infoMessages ),
                printInfoMessages( _printInfoMessages )
            {
                switch( result.getResultType() ) {
                    case ResultWas::Ok:
                        colour = Colour::Success;
                        passOrFail = "PASSED";
                        //if( result.hasMessage() )
                        if( _stats.infoMessages.size() == 1 )
                            messageLabel = "with message";
                        if( _stats.infoMessages.size() > 1 )
                            messageLabel = "with messages";
                        break;
                    case ResultWas::ExpressionFailed:
                        if( result.isOk() ) {
                            colour = Colour::Success;
                            passOrFail = "FAILED - but was ok";
                        }
                        else {
                            colour = Colour::Error;
                            passOrFail = "FAILED";
                        }
                        if( _stats.infoMessages.size() == 1 )
                            messageLabel = "with message";
                        if( _stats.infoMessages.size() > 1 )
                            messageLabel = "with messages";
                        break;
                    case ResultWas::ThrewException:
                        colour = Colour::Error;
                        passOrFail = "FAILED";
                        messageLabel = "due to unexpected exception with message";
                        break;
                    case ResultWas::DidntThrowException:
                        colour = Colour::Error;
                        passOrFail = "FAILED";
                        messageLabel = "because no exception was thrown where one was expected";
                        break;
                    case ResultWas::Info:
                        messageLabel = "info";
                        break;
                    case ResultWas::Warning:
                        messageLabel = "warning";
                        break;
                    case ResultWas::ExplicitFailure:
                        passOrFail = "FAILED";
                        colour = Colour::Error;
                        if( _stats.infoMessages.size() == 1 )
                            messageLabel = "explicitly with message";
                        if( _stats.infoMessages.size() > 1 )
                            messageLabel = "explicitly with messages";
                        break;
                    // These cases are here to prevent compiler warnings
                    case ResultWas::Unknown:
                    case ResultWas::FailureBit:
                    case ResultWas::Exception:
                        passOrFail = "** internal error **";
                        colour = Colour::Error;
                        break;
                }
            }

            void print() const {
                printSourceInfo();
                if( stats.totals.assertions.total() > 0 ) {
                    if( result.isOk() )
                        stream << "\n";
                    printResultType();
                    printOriginalExpression();
                    printReconstructedExpression();
                }
                else {
                    stream << "\n";
                }
                printMessage();
            }

        private:
            void printResultType() const {
                if( !passOrFail.empty() ) {
                    Colour colourGuard( colour );
                    stream << passOrFail << ":\n";
                }
            }
            void printOriginalExpression() const {
                if( result.hasExpression() ) {
                    Colour colourGuard( Colour::OriginalExpression );
                    stream  << "  ";
                    stream << result.getExpressionInMacro();
                    stream << "\n";
                }
            }
            void printReconstructedExpression() const {
                if( result.hasExpandedExpression() ) {
                    stream << "with expansion:\n";
                    Colour colourGuard( Colour::ReconstructedExpression );
                    stream << Text( result.getExpandedExpression(), TextAttributes().setIndent(2) ) << "\n";
                }
            }
            void printMessage() const {
                if( !messageLabel.empty() )
                    stream << messageLabel << ":" << "\n";
                for( std::vector<MessageInfo>::const_iterator it = messages.begin(), itEnd = messages.end();
                        it != itEnd;
                        ++it ) {
                    // If this assertion is a warning ignore any INFO messages
                    if( printInfoMessages || it->type != ResultWas::Info )
                        stream << Text( it->message, TextAttributes().setIndent(2) ) << "\n";
                }
            }
            void printSourceInfo() const {
                Colour colourGuard( Colour::FileName );
                stream << result.getSourceInfo() << ": ";
            }

            std::ostream& stream;
            AssertionStats const& stats;
            AssertionResult const& result;
            Colour::Code colour;
            std::string passOrFail;
            std::string messageLabel;
            std::string message;
            std::vector<MessageInfo> messages;
            bool printInfoMessages;
        };

        void lazyPrint() {

            if( !currentTestRunInfo.used )
                lazyPrintRunInfo();
            if( !currentGroupInfo.used )
                lazyPrintGroupInfo();

            if( !m_headerPrinted ) {
                printTestCaseAndSectionHeader();
                m_headerPrinted = true;
            }
            m_atLeastOneTestCasePrinted = true;
        }
        void lazyPrintRunInfo() {
            stream  << "\n" << getTildes() << "\n";
            Colour colour( Colour::SecondaryText );
            stream  << currentTestRunInfo->name
                    << " is a Catch v"  << libraryVersion::value.majorVersion << "."
                    << libraryVersion::value.minorVersion << " b"
                    << libraryVersion::value.buildNumber;
            if( libraryVersion::value.branchName != "master" )
                stream << " (" << libraryVersion::value.branchName << ")";
            stream  << " host application.\n"
                    << "Run with -? for options\n\n";

            currentTestRunInfo.used = true;
        }
        void lazyPrintGroupInfo() {
            if( !currentGroupInfo->name.empty() && currentGroupInfo->groupsCounts > 1 ) {
                printClosedHeader( "Group: " + currentGroupInfo->name );
                currentGroupInfo.used = true;
            }
        }
        void printTestCaseAndSectionHeader() {
            assert( !m_sectionStack.empty() );
            printOpenHeader( currentTestCaseInfo->name );

            if( m_sectionStack.size() > 1 ) {
                Colour colourGuard( Colour::Headers );

                std::vector<SectionInfo>::const_iterator
                    it = m_sectionStack.begin()+1, // Skip first section (test case)
                    itEnd = m_sectionStack.end();
                for( ; it != itEnd; ++it )
                    printHeaderString( it->name, 2 );
            }

            SourceLineInfo lineInfo = m_sectionStack.front().lineInfo;

            if( !lineInfo.empty() ){
                stream << getDashes() << "\n";
                Colour colourGuard( Colour::FileName );
                stream << lineInfo << "\n";
            }
            stream << getDots() << "\n" << std::endl;
        }

        void printClosedHeader( std::string const& _name ) {
            printOpenHeader( _name );
            stream << getDots() << "\n";
        }
        void printOpenHeader( std::string const& _name ) {
            stream  << getDashes() << "\n";
            {
                Colour colourGuard( Colour::Headers );
                printHeaderString( _name );
            }
        }

        // if string has a : in first line will set indent to follow it on
        // subsequent lines
        void printHeaderString( std::string const& _string, std::size_t indent = 0 ) {
            std::size_t i = _string.find( ": " );
            if( i != std::string::npos )
                i+=2;
            else
                i = 0;
            stream << Text( _string, TextAttributes()
                                        .setIndent( indent+i)
                                        .setInitialIndent( indent ) ) << "\n";
        }

        void printTotals( const Totals& totals ) {
            if( totals.testCases.total() == 0 ) {
                stream << "No tests ran";
            }
            else if( totals.assertions.total() == 0 ) {
                Colour colour( Colour::Yellow );
                printCounts( "test case", totals.testCases );
                stream << " (no assertions)";
            }
            else if( totals.assertions.failed ) {
                Colour colour( Colour::ResultError );
                printCounts( "test case", totals.testCases );
                if( totals.testCases.failed > 0 ) {
                    stream << " (";
                    printCounts( "assertion", totals.assertions );
                    stream << ")";
                }
            }
            else {
                Colour colour( Colour::ResultSuccess );
                stream << "All tests passed ("
                        << pluralise( totals.assertions.passed, "assertion" ) << " in "
                        << pluralise( totals.testCases.passed, "test case" ) << ")";
            }
        }
        void printCounts( std::string const& label, Counts const& counts ) {
            if( counts.total() == 1 ) {
                stream << "1 " << label << " - ";
                if( counts.failed )
                    stream << "failed";
                else
                    stream << "passed";
            }
            else {
                stream << counts.total() << " " << label << "s ";
                if( counts.passed ) {
                    if( counts.failed )
                        stream << "- " << counts.failed << " failed";
                    else if( counts.passed == 2 )
                        stream << "- both passed";
                    else
                        stream << "- all passed";
                }
                else {
                    if( counts.failed == 2 )
                        stream << "- both failed";
                    else
                        stream << "- all failed";
                }
            }
        }

        void printTotalsDivider() {
            stream << getDoubleDashes() << "\n";
        }
        void printSummaryDivider() {
            stream << getDashes() << "\n";
        }
#if defined(INTERNAL_CATCH_VS_MANAGED) || defined(INTERNAL_CATCH_VS_NATIVE)
        static std::string getDashes() {
            const std::string dashes( CATCH_CONFIG_CONSOLE_WIDTH-1, '-' );
            return dashes;
        }
        static std::string getDots() {
            const std::string dots( CATCH_CONFIG_CONSOLE_WIDTH-1, '.' );
            return dots;
        }
        static std::string getDoubleDashes() {
            const std::string doubleDashes( CATCH_CONFIG_CONSOLE_WIDTH-1, '=' );
            return doubleDashes;
        }
        static std::string getTildes() {
            const std::string dots( CATCH_CONFIG_CONSOLE_WIDTH-1, '~' );
            return dots;
        }
#else
        static std::string const& getDashes() {
            static const std::string dashes( CATCH_CONFIG_CONSOLE_WIDTH-1, '-' );
            return dashes;
        }
        static std::string const& getDots() {
            static const std::string dots( CATCH_CONFIG_CONSOLE_WIDTH-1, '.' );
            return dots;
        }
        static std::string const& getDoubleDashes() {
            static const std::string doubleDashes( CATCH_CONFIG_CONSOLE_WIDTH-1, '=' );
            return doubleDashes;
        }
        static std::string const& getTildes() {
            static const std::string dots( CATCH_CONFIG_CONSOLE_WIDTH-1, '~' );
            return dots;
        }
#endif

    private:
        bool m_headerPrinted;
        bool m_atLeastOneTestCasePrinted;
    };

    INTERNAL_CATCH_REGISTER_REPORTER( "console", ConsoleReporter )

} // end namespace Catch

namespace Catch {
    INTERNAL_CATCH_INLINE NonCopyable::~NonCopyable() {}
    INTERNAL_CATCH_INLINE IShared::~IShared() {}
    INTERNAL_CATCH_INLINE StreamBufBase::~StreamBufBase() throw() {}
    INTERNAL_CATCH_INLINE IContext::~IContext() {}
    INTERNAL_CATCH_INLINE IResultCapture::~IResultCapture() {}
    INTERNAL_CATCH_INLINE ITestCase::~ITestCase() {}
    INTERNAL_CATCH_INLINE ITestCaseRegistry::~ITestCaseRegistry() {}
    INTERNAL_CATCH_INLINE IRegistryHub::~IRegistryHub() {}
    INTERNAL_CATCH_INLINE IMutableRegistryHub::~IMutableRegistryHub() {}
    INTERNAL_CATCH_INLINE IExceptionTranslator::~IExceptionTranslator() {}
    INTERNAL_CATCH_INLINE IExceptionTranslatorRegistry::~IExceptionTranslatorRegistry() {}
    INTERNAL_CATCH_INLINE IReporter::~IReporter() {}
    INTERNAL_CATCH_INLINE IReporterFactory::~IReporterFactory() {}
    INTERNAL_CATCH_INLINE IReporterRegistry::~IReporterRegistry() {}
    INTERNAL_CATCH_INLINE IStreamingReporter::~IStreamingReporter() {}
    INTERNAL_CATCH_INLINE AssertionStats::~AssertionStats() {}
    INTERNAL_CATCH_INLINE SectionStats::~SectionStats() {}
    INTERNAL_CATCH_INLINE TestCaseStats::~TestCaseStats() {}
    INTERNAL_CATCH_INLINE TestGroupStats::~TestGroupStats() {}
    INTERNAL_CATCH_INLINE TestRunStats::~TestRunStats() {}
    INTERNAL_CATCH_INLINE CumulativeReporterBase::SectionNode::~SectionNode() {}
    INTERNAL_CATCH_INLINE CumulativeReporterBase::~CumulativeReporterBase() {}

    INTERNAL_CATCH_INLINE StreamingReporterBase::~StreamingReporterBase() {}
    INTERNAL_CATCH_INLINE ConsoleReporter::~ConsoleReporter() {}
    INTERNAL_CATCH_INLINE IRunner::~IRunner() {}
    INTERNAL_CATCH_INLINE IMutableContext::~IMutableContext() {}
    INTERNAL_CATCH_INLINE IConfig::~IConfig() {}
    INTERNAL_CATCH_INLINE XmlReporter::~XmlReporter() {}
    INTERNAL_CATCH_INLINE JunitReporter::~JunitReporter() {}
    INTERNAL_CATCH_INLINE TestRegistry::~TestRegistry() {}
    INTERNAL_CATCH_INLINE FreeFunctionTestCase::~FreeFunctionTestCase() {}
    INTERNAL_CATCH_INLINE IGeneratorInfo::~IGeneratorInfo() {}
    INTERNAL_CATCH_INLINE IGeneratorsForTest::~IGeneratorsForTest() {}

    INTERNAL_CATCH_INLINE Matchers::Impl::StdString::Equals::~Equals() {}
    INTERNAL_CATCH_INLINE Matchers::Impl::StdString::Contains::~Contains() {}
    INTERNAL_CATCH_INLINE Matchers::Impl::StdString::StartsWith::~StartsWith() {}
    INTERNAL_CATCH_INLINE Matchers::Impl::StdString::EndsWith::~EndsWith() {}

    INTERNAL_CATCH_INLINE void Config::dummy() {}

    INTERNAL_CATCH_REGISTER_LEGACY_REPORTER( "xml", XmlReporter )
}

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif

#ifdef CATCH_CONFIG_MAIN
// #included from: internal/catch_default_main.hpp
#define TWOBLUECUBES_CATCH_DEFAULT_MAIN_HPP_INCLUDED

#ifndef __OBJC__

// Standard C/C++ main entry point
int main (int argc, char * const argv[]) {
    return Catch::Session().run( argc, argv );
}

#else // __OBJC__

// Objective-C entry point
int main (int argc, char * const argv[]) {
#if !CATCH_ARC_ENABLED
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
#endif

    Catch::registerTestMethods();
    int result = Catch::Session().run( argc, (char* const*)argv );

#if !CATCH_ARC_ENABLED
    [pool drain];
#endif

    return result;
}

#endif // __OBJC__

#endif

#ifdef CLARA_CONFIG_MAIN_NOT_DEFINED
#  undef CLARA_CONFIG_MAIN
#endif

//////

// If this config identifier is defined then all CATCH macros are prefixed with CATCH_
#ifdef CATCH_CONFIG_PREFIX_ALL

#define CATCH_REQUIRE( expr ) INTERNAL_CATCH_TEST( expr, Catch::ResultDisposition::Normal, "CATCH_REQUIRE" )
#define CATCH_REQUIRE_FALSE( expr ) INTERNAL_CATCH_TEST( expr, Catch::ResultDisposition::Normal | Catch::ResultDisposition::NegateResult, "CATCH_REQUIRE_FALSE" )

#define CATCH_REQUIRE_THROWS( expr ) INTERNAL_CATCH_THROWS( expr, ..., Catch::ResultDisposition::Normal, "CATCH_REQUIRE_THROWS" )
#define CATCH_REQUIRE_THROWS_AS( expr, exceptionType ) INTERNAL_CATCH_THROWS_AS( expr, exceptionType, Catch::ResultDisposition::Normal, "CATCH_REQUIRE_THROWS_AS" )
#define CATCH_REQUIRE_NOTHROW( expr ) INTERNAL_CATCH_NO_THROW( expr, Catch::ResultDisposition::Normal, "CATCH_REQUIRE_NOTHROW" )

#define CATCH_CHECK( expr ) INTERNAL_CATCH_TEST( expr, Catch::ResultDisposition::ContinueOnFailure, "CATCH_CHECK" )
#define CATCH_CHECK_FALSE( expr ) INTERNAL_CATCH_TEST( expr, Catch::ResultDisposition::ContinueOnFailure | Catch::ResultDisposition::NegateResult, "CATCH_CHECK_FALSE" )
#define CATCH_CHECKED_IF( expr ) INTERNAL_CATCH_IF( expr, Catch::ResultDisposition::ContinueOnFailure, "CATCH_CHECKED_IF" )
#define CATCH_CHECKED_ELSE( expr ) INTERNAL_CATCH_ELSE( expr, Catch::ResultDisposition::ContinueOnFailure, "CATCH_CHECKED_ELSE" )
#define CATCH_CHECK_NOFAIL( expr ) INTERNAL_CATCH_TEST( expr, Catch::ResultDisposition::ContinueOnFailure | Catch::ResultDisposition::SuppressFail, "CATCH_CHECK_NOFAIL" )

#define CATCH_CHECK_THROWS( expr )  INTERNAL_CATCH_THROWS( expr, ..., Catch::ResultDisposition::ContinueOnFailure, "CATCH_CHECK_THROWS" )
#define CATCH_CHECK_THROWS_AS( expr, exceptionType ) INTERNAL_CATCH_THROWS_AS( expr, exceptionType, Catch::ResultDisposition::ContinueOnFailure, "CATCH_CHECK_THROWS_AS" )
#define CATCH_CHECK_NOTHROW( expr ) INTERNAL_CATCH_NO_THROW( expr, Catch::ResultDisposition::ContinueOnFailure, "CATCH_CHECK_NOTHROW" )

#define CHECK_THAT( arg, matcher ) INTERNAL_CHECK_THAT( arg, matcher, Catch::ResultDisposition::ContinueOnFailure, "CATCH_CHECK_THAT" )
#define CATCH_REQUIRE_THAT( arg, matcher ) INTERNAL_CHECK_THAT( arg, matcher, Catch::ResultDisposition::Normal, "CATCH_REQUIRE_THAT" )

#define CATCH_INFO( msg ) INTERNAL_CATCH_INFO( msg, "CATCH_INFO" )
#define CATCH_WARN( msg ) INTERNAL_CATCH_MSG( Catch::ResultWas::Warning, Catch::ResultDisposition::ContinueOnFailure, "CATCH_WARN", msg )
#define CATCH_SCOPED_INFO( msg ) INTERNAL_CATCH_INFO( msg, "CATCH_INFO" )
#define CATCH_CAPTURE( msg ) INTERNAL_CATCH_INFO( #msg " := " << msg, "CATCH_CAPTURE" )
#define CATCH_SCOPED_CAPTURE( msg ) INTERNAL_CATCH_INFO( #msg " := " << msg, "CATCH_CAPTURE" )

#ifdef CATCH_CONFIG_VARIADIC_MACROS
    #define CATCH_TEST_CASE( ... ) INTERNAL_CATCH_TESTCASE( __VA_ARGS__ )
    #define CATCH_TEST_CASE_METHOD( className, ... ) INTERNAL_CATCH_TEST_CASE_METHOD( className, __VA_ARGS__ )
    #define CATCH_METHOD_AS_TEST_CASE( method, ... ) INTERNAL_CATCH_METHOD_AS_TEST_CASE( method, __VA_ARGS__ )
    #define CATCH_SECTION( ... ) INTERNAL_CATCH_SECTION( __VA_ARGS__ )
    #define CATCH_FAIL( ... ) INTERNAL_CATCH_MSG( Catch::ResultWas::ExplicitFailure, Catch::ResultDisposition::Normal, "CATCH_FAIL", __VA_ARGS__ )
    #define CATCH_SUCCEED( ... ) INTERNAL_CATCH_MSG( Catch::ResultWas::Ok, Catch::ResultDisposition::ContinueOnFailure, "CATCH_SUCCEED", __VA_ARGS__ )
#else
    #define CATCH_TEST_CASE( name, description ) INTERNAL_CATCH_TESTCASE( name, description )
    #define CATCH_TEST_CASE_METHOD( className, name, description ) INTERNAL_CATCH_TEST_CASE_METHOD( className, name, description )
    #define CATCH_METHOD_AS_TEST_CASE( method, name, description ) INTERNAL_CATCH_METHOD_AS_TEST_CASE( method, name, description )
    #define CATCH_SECTION( name, description ) INTERNAL_CATCH_SECTION( name, description )
    #define CATCH_FAIL( msg ) INTERNAL_CATCH_MSG( Catch::ResultWas::ExplicitFailure, Catch::ResultDisposition::Normal, "CATCH_FAIL", msg )
    #define CATCH_SUCCEED( msg ) INTERNAL_CATCH_MSG( Catch::ResultWas::Ok, Catch::ResultDisposition::ContinueOnFailure, "CATCH_SUCCEED", msg )
#endif
#define CATCH_ANON_TEST_CASE() INTERNAL_CATCH_TESTCASE( "", "" )

#define CATCH_REGISTER_REPORTER( name, reporterType ) INTERNAL_CATCH_REGISTER_REPORTER( name, reporterType )
#define CATCH_REGISTER_LEGACY_REPORTER( name, reporterType ) INTERNAL_CATCH_REGISTER_LEGACY_REPORTER( name, reporterType )

#define CATCH_GENERATE( expr) INTERNAL_CATCH_GENERATE( expr )

// "BDD-style" convenience wrappers
#ifdef CATCH_CONFIG_VARIADIC_MACROS
#define CATCH_SCENARIO( ... ) CATCH_TEST_CASE( "Scenario: " __VA_ARGS__ )
#else
#define CATCH_SCENARIO( name, tags ) CATCH_TEST_CASE( "Scenario: " name, tags )
#endif
#define CATCH_GIVEN( desc )    CATCH_SECTION( "Given: " desc, "" )
#define CATCH_WHEN( desc )     CATCH_SECTION( " When: " desc, "" )
#define CATCH_AND_WHEN( desc ) CATCH_SECTION( "  And: " desc, "" )
#define CATCH_THEN( desc )     CATCH_SECTION( " Then: " desc, "" )
#define CATCH_AND_THEN( desc ) CATCH_SECTION( "  And: " desc, "" )

// If CATCH_CONFIG_PREFIX_ALL is not defined then the CATCH_ prefix is not required
#else

#define REQUIRE( expr ) INTERNAL_CATCH_TEST( expr, Catch::ResultDisposition::Normal, "REQUIRE" )
#define REQUIRE_FALSE( expr ) INTERNAL_CATCH_TEST( expr, Catch::ResultDisposition::Normal | Catch::ResultDisposition::NegateResult, "REQUIRE_FALSE" )

#define REQUIRE_THROWS( expr ) INTERNAL_CATCH_THROWS( expr, ..., Catch::ResultDisposition::Normal, "REQUIRE_THROWS" )
#define REQUIRE_THROWS_AS( expr, exceptionType ) INTERNAL_CATCH_THROWS_AS( expr, exceptionType, Catch::ResultDisposition::Normal, "REQUIRE_THROWS_AS" )
#define REQUIRE_NOTHROW( expr ) INTERNAL_CATCH_NO_THROW( expr, Catch::ResultDisposition::Normal, "REQUIRE_NOTHROW" )

#define CHECK( expr ) INTERNAL_CATCH_TEST( expr, Catch::ResultDisposition::ContinueOnFailure, "CHECK" )
#define CHECK_FALSE( expr ) INTERNAL_CATCH_TEST( expr, Catch::ResultDisposition::ContinueOnFailure | Catch::ResultDisposition::NegateResult, "CHECK_FALSE" )
#define CHECKED_IF( expr ) INTERNAL_CATCH_IF( expr, Catch::ResultDisposition::ContinueOnFailure, "CHECKED_IF" )
#define CHECKED_ELSE( expr ) INTERNAL_CATCH_ELSE( expr, Catch::ResultDisposition::ContinueOnFailure, "CHECKED_ELSE" )
#define CHECK_NOFAIL( expr ) INTERNAL_CATCH_TEST( expr, Catch::ResultDisposition::ContinueOnFailure | Catch::ResultDisposition::SuppressFail, "CHECK_NOFAIL" )

#define CHECK_THROWS( expr )  INTERNAL_CATCH_THROWS( expr, ..., Catch::ResultDisposition::ContinueOnFailure, "CHECK_THROWS" )
#define CHECK_THROWS_AS( expr, exceptionType ) INTERNAL_CATCH_THROWS_AS( expr, exceptionType, Catch::ResultDisposition::ContinueOnFailure, "CHECK_THROWS_AS" )
#define CHECK_NOTHROW( expr ) INTERNAL_CATCH_NO_THROW( expr, Catch::ResultDisposition::ContinueOnFailure, "CHECK_NOTHROW" )

#define CHECK_THAT( arg, matcher ) INTERNAL_CHECK_THAT( arg, matcher, Catch::ResultDisposition::ContinueOnFailure, "CHECK_THAT" )
#define REQUIRE_THAT( arg, matcher ) INTERNAL_CHECK_THAT( arg, matcher, Catch::ResultDisposition::Normal, "REQUIRE_THAT" )

#define INFO( msg ) INTERNAL_CATCH_INFO( msg, "INFO" )
#define WARN( msg ) INTERNAL_CATCH_MSG( Catch::ResultWas::Warning, Catch::ResultDisposition::ContinueOnFailure, "WARN", msg )
#define SCOPED_INFO( msg ) INTERNAL_CATCH_INFO( msg, "INFO" )
#define CAPTURE( msg ) INTERNAL_CATCH_INFO( #msg " := " << msg, "CAPTURE" )
#define SCOPED_CAPTURE( msg ) INTERNAL_CATCH_INFO( #msg " := " << msg, "CAPTURE" )

#ifdef CATCH_CONFIG_VARIADIC_MACROS
    #define TEST_CASE( ... ) INTERNAL_CATCH_TESTCASE( __VA_ARGS__ )
    #define TEST_CASE_METHOD( className, ... ) INTERNAL_CATCH_TEST_CASE_METHOD( className, __VA_ARGS__ )
    #define METHOD_AS_TEST_CASE( method, ... ) INTERNAL_CATCH_METHOD_AS_TEST_CASE( method, __VA_ARGS__ )
    #define SECTION( ... ) INTERNAL_CATCH_SECTION( __VA_ARGS__ )
    #define FAIL( ... ) INTERNAL_CATCH_MSG( Catch::ResultWas::ExplicitFailure, Catch::ResultDisposition::Normal, "FAIL", __VA_ARGS__ )
    #define SUCCEED( ... ) INTERNAL_CATCH_MSG( Catch::ResultWas::Ok, Catch::ResultDisposition::ContinueOnFailure, "SUCCEED", __VA_ARGS__ )
#else
    #define TEST_CASE( name, description ) INTERNAL_CATCH_TESTCASE( name, description )
    #define TEST_CASE_METHOD( className, name, description ) INTERNAL_CATCH_TEST_CASE_METHOD( className, name, description )
    #define METHOD_AS_TEST_CASE( method, name, description ) INTERNAL_CATCH_METHOD_AS_TEST_CASE( method, name, description )
    #define SECTION( name, description ) INTERNAL_CATCH_SECTION( name, description )
    #define FAIL( msg ) INTERNAL_CATCH_MSG( Catch::ResultWas::ExplicitFailure, Catch::ResultDisposition::Normal, "FAIL", msg )
    #define SUCCEED( msg ) INTERNAL_CATCH_MSG( Catch::ResultWas::Ok, Catch::ResultDisposition::ContinueOnFailure, "SUCCEED", msg )
#endif
#define ANON_TEST_CASE() INTERNAL_CATCH_TESTCASE( "", "" )

#define REGISTER_REPORTER( name, reporterType ) INTERNAL_CATCH_REGISTER_REPORTER( name, reporterType )
#define REGISTER_LEGACY_REPORTER( name, reporterType ) INTERNAL_CATCH_REGISTER_LEGACY_REPORTER( name, reporterType )

#define GENERATE( expr) INTERNAL_CATCH_GENERATE( expr )

#endif

#define CATCH_TRANSLATE_EXCEPTION( signature ) INTERNAL_CATCH_TRANSLATE_EXCEPTION( signature )

// "BDD-style" convenience wrappers
#ifdef CATCH_CONFIG_VARIADIC_MACROS
#define SCENARIO( ... ) TEST_CASE( "Scenario: " __VA_ARGS__ )
#else
#define SCENARIO( name, tags ) TEST_CASE( "Scenario: " name, tags )
#endif
#define GIVEN( desc )    SECTION( "   Given: " desc, "" )
#define WHEN( desc )     SECTION( "    When: " desc, "" )
#define AND_WHEN( desc ) SECTION( "And when: " desc, "" )
#define THEN( desc )     SECTION( "    Then: " desc, "" )
#define AND_THEN( desc ) SECTION( "     And: " desc, "" )

#if defined(INTERNAL_CATCH_VS_MANAGED) || defined(INTERNAL_CATCH_VS_NATIVE)
#define CATCH_MAP_CATEGORY_TO_TAG( Category, Tag ) INTERNAL_CATCH_MAP_CATEGORY_TO_TAG( Category, Tag )
#define CATCH_CONFIG_SHOW_SUCCESS( v ) CATCH_INTERNAL_CONFIG_SHOW_SUCCESS( v )
#define CATCH_CONFIG_WARN_MISSING_ASSERTIONS( v ) CATCH_INTERNAL_CONFIG_WARN_MISSING_ASSERTIONS( v )
#define CATCH_CONFIG_ABORT_AFTER( v ) CATCH_INTERNAL_CONFIG_ABORT_AFTER( v )
#else
#define CATCH_MAP_CATEGORY_TO_TAG( Category, Tag )
#define CATCH_CONFIG_SHOW_SUCCESS( v )
#define CATCH_CONFIG_WARN_MISSING_ASSERTIONS( v )
#define CATCH_CONFIG_ABORT_AFTER( v )
#endif

using Catch::Detail::Approx;

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif // TWOBLUECUBES_SINGLE_INCLUDE_CATCH_HPP_INCLUDED
