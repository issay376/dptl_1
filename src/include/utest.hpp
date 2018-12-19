//
// utest.h --- unit test utility for Pointer Container Template Library
//
//	Version 2.0
//
//      Copyright (C) 2003, 2018 Kazunari Saitoh
//
//      This library is free software; you can redistribute it and/or
//      modify it under the terms of the GNU Lesser General Public
//      License as published by the Free Software Foundation; either
//      version 2.1 of the License, or (at your option) any later version.
//
//      This library is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//      Lesser General Public License for more details.
//
//      You should have received a copy of the GNU Lesser General Public
//      License along with this library; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

#ifndef __DPTL_UTEST_H
#define __DPTL_UTEST_H

#ifndef DPTL_NAMESPACE
#define DPTL_NAMESPACE dptl
#endif

#include <cstdio>
#include <cstdlib>
#include <dptl/deep_ptr.hpp>

#ifndef NO_NAMESPACE
namespace DPTL_NAMESPACE {
#endif // NO_NAMESPACE

// macros
// -----------------------------------------------------------------------------
#define AssertEqual unittpos( __FILE__, __LINE__ ).assertEqual
#define AssertNotEqual unittpos( __FILE__, __LINE__ ).assertNotEqual
#define AssertNull unittpos( __FILE__, __LINE__ ).assertNull
#define AssertZero unittpos( __FILE__, __LINE__ ).assertZero
#define AssertNonZero unittpos( __FILE__, __LINE__ ).assertNonZero
#define AssertTrue unittpos( __FILE__, __LINE__ ).assertTrue
#define AssertFalse unittpos( __FILE__, __LINE__ ).assertFalse
#define AssertError unittpos( __FILE__, __LINE__ ).assertError

// utility - dp_dup shortcut
// -----------------------------------------------------------------------------
template <typename T>           T  dpl( T v )             { return dp_dup<T>()( v ); }
template <typename T>           T* dpl( const T* p )      { return dp_dup<T*>()( p ); }
template <typename T>           T* dpl( const T (&a)[] )  { return dp_dup<T[]>()( a ); }
template <typename T, size_t N> T* dpl( const T (&a)[N] ) { return dp_dup<T[N]>()( a ); }

// -----------------------------------------------------------------------------
class unittpos
{
	const char*	m_file;
	unsigned	m_line;

    public:
	unittpos( const char* fn, unsigned ln ) : m_file( fn ), m_line( ln ) { }

	template <typename T, typename U> void assertEqual( T const& a, U const& b )
	{
		assertTrue( static_cast<typename const_p<T>::t const&>( a ) == static_cast<typename const_p<U>::t const&>( b ));
	}
	template <typename T, typename U> void assertEqual( unsigned n, T const& a, U const& b )
	{
		assertTrue( n, static_cast<typename const_p<T>::t const&>( a ) == static_cast<typename const_p<U>::t const&>( b ));
	}
	template <typename T, typename U> void assertEqual( const char* msg, T const& a, U const& b )
	{
		assertTrue( msg, static_cast<typename const_p<T>::t const&>( a ) == static_cast<typename const_p<U>::t const&>( b ));
	}

	template <typename T, typename U> void assertNotEqual( T const& a, U const& b )
	{
		assertFalse( static_cast<typename const_p<T>::t const&>( a ) == static_cast<typename const_p<U>::t const&>( b ));
	}
	template <typename T, typename U> void assertNotEqual( unsigned n, T const& a, U const& b )
	{
		assertFalse( n, static_cast<typename const_p<T>::t const&>( a ) == static_cast<typename const_p<U>::t const&>( b ));
	}
	template <typename T, typename U> void assertNotEqual( const char* msg, T const& a, U const& b )
	{
		assertFalse( msg, static_cast<typename const_p<T>::t const&>( a ) == static_cast<typename const_p<U>::t const&>( b ));
	}

	template <typename T> void assertNull( T const& a ) { assertTrue( a == nullptr ); }
	template <typename T> void assertNull( unsigned n, T const& a ) { assertTrue( n, a == nullptr ); }
	template <typename T> void assertNull( const char* msg, T const& a ) { assertTrue( msg, a == nullptr ); }

	template <typename T> void assertZero( T const& a ) { assertTrue( a == 0 ); }
	template <typename T> void assertZero( unsigned n, T const& a ) { assertTrue( n, a == 0 ); }
	template <typename T> void assertZero( const char* msg, T const& a ) { assertTrue( msg, a == 0 ); }

	template <typename T> void assertNonZero( T const& a ) { assertTrue( a != 0 ); }
	template <typename T> void assertNonZero( unsigned n, T const& a ) { assertTrue( n, a != 0 ); }
	template <typename T> void assertNonZero( const char* msg, T const& a ) { assertTrue( msg, a != 0 ); }

	template <typename T> void assertTrue( T const& a ) { assertTrue( "-", a ); }
	template <typename T> void assertTrue( unsigned n, T const& a )
	{
		char	buf[ 16 ];

		snprintf( buf, 16, "%u", n );
		assertTrue( buf, a );
	}
	template <typename T> void assertTrue( const char* msg, T const& a )
	{
		if ( a ) {
			printf( "ok: %s %s(%u)\n", msg, m_file, m_line );
		} else {
			printf( "fail: %s %s(%u)\n", msg, m_file, m_line );
			exit( 0 );
		}
	}

	template <typename T> void assertFalse( T const& a ) { assertTrue( !a ); }
	template <typename T> void assertFalse( unsigned n, T const& a ) { assertTrue( n, !a ); }
	template <typename T> void assertFalse( const char* msg, T const& a ) { assertTrue( msg, !a ); }

	void assertError() { assertTrue( false ); }
	void assertError( unsigned n ) { assertTrue( n, false ); }
	void assertError( const char* msg ) { assertTrue( msg, false ); }
};

// base for unittest class
// -----------------------------------------------------------------------------
class unittest
{
	const char*	m_testname;
	const char*	m_subtitle;
	int		m_pickup;

    public:
	unittest() : m_testname( nullptr ), m_subtitle( nullptr ), m_pickup( -1 ) { }
	unittest( const char* title, int n = -1 )
		: m_testname( title ? strdup( title ) : nullptr ), m_subtitle( nullptr ), m_pickup( n )
	{
		if ( m_testname ) printf( "\n*** %s start. ***\n", m_testname );
	}
	unittest( int argc, char** argv )
		: m_testname( nullptr ), m_subtitle( nullptr ), m_pickup( -1 )
	{
		if ( 1 < argc ) {
			int	n = std::strtoul( argv[ 1 ], nullptr, 16 );

			if ( 0 <= n && n < 0x100 ) {
				char	buf[ 16 ];

				m_pickup = n;
				sprintf( buf, "test%X", n );
				m_testname = strdup( buf );
				printf( "\n*** %s start. ***\n", m_testname );
			}
		}
	}
	virtual ~unittest()
	{
		printf( "\n*** %s done. ***\n", m_testname ? m_testname : "Unit test" );
		if ( m_testname ) delete [] m_testname;
		if ( m_subtitle ) delete [] m_subtitle;
	}

	void set_subtitle( const char* s )
	{
		if ( m_subtitle ) delete [] m_subtitle;
		m_subtitle = strdup( s );
		printf( "\n*** " );
		if ( m_testname ) printf( "%s : ", m_testname ); 
		printf( "%s ***\n", s );
	}

	const char* get_testname() { return m_testname; }
	const char* get_subtitle() { return m_subtitle; }

	virtual void test00() { }
	virtual void test01() { }
	virtual void test02() { }
	virtual void test03() { }
	virtual void test04() { }
	virtual void test05() { }
	virtual void test06() { }
	virtual void test07() { }
	virtual void test08() { }
	virtual void test09() { }
	virtual void test0A() { }
	virtual void test0B() { }
	virtual void test0C() { }
	virtual void test0D() { }
	virtual void test0E() { }
	virtual void test0F() { }

	virtual void test10() { }
	virtual void test11() { }
	virtual void test12() { }
	virtual void test13() { }
	virtual void test14() { }
	virtual void test15() { }
	virtual void test16() { }
	virtual void test17() { }
	virtual void test18() { }
	virtual void test19() { }
	virtual void test1A() { }
	virtual void test1B() { }
	virtual void test1C() { }
	virtual void test1D() { }
	virtual void test1E() { }
	virtual void test1F() { }

	virtual void test20() { }
	virtual void test21() { }
	virtual void test22() { }
	virtual void test23() { }
	virtual void test24() { }
	virtual void test25() { }
	virtual void test26() { }
	virtual void test27() { }
	virtual void test28() { }
	virtual void test29() { }
	virtual void test2A() { }
	virtual void test2B() { }
	virtual void test2C() { }
	virtual void test2D() { }
	virtual void test2E() { }
	virtual void test2F() { }

	virtual void test30() { }
	virtual void test31() { }
	virtual void test32() { }
	virtual void test33() { }
	virtual void test34() { }
	virtual void test35() { }
	virtual void test36() { }
	virtual void test37() { }
	virtual void test38() { }
	virtual void test39() { }
	virtual void test3A() { }
	virtual void test3B() { }
	virtual void test3C() { }
	virtual void test3D() { }
	virtual void test3E() { }
	virtual void test3F() { }

	virtual void test40() { }
	virtual void test41() { }
	virtual void test42() { }
	virtual void test43() { }
	virtual void test44() { }
	virtual void test45() { }
	virtual void test46() { }
	virtual void test47() { }
	virtual void test48() { }
	virtual void test49() { }
	virtual void test4A() { }
	virtual void test4B() { }
	virtual void test4C() { }
	virtual void test4D() { }
	virtual void test4E() { }
	virtual void test4F() { }

	virtual void test50() { }
	virtual void test51() { }
	virtual void test52() { }
	virtual void test53() { }
	virtual void test54() { }
	virtual void test55() { }
	virtual void test56() { }
	virtual void test57() { }
	virtual void test58() { }
	virtual void test59() { }
	virtual void test5A() { }
	virtual void test5B() { }
	virtual void test5C() { }
	virtual void test5D() { }
	virtual void test5E() { }
	virtual void test5F() { }

	virtual void test60() { }
	virtual void test61() { }
	virtual void test62() { }
	virtual void test63() { }
	virtual void test64() { }
	virtual void test65() { }
	virtual void test66() { }
	virtual void test67() { }
	virtual void test68() { }
	virtual void test69() { }
	virtual void test6A() { }
	virtual void test6B() { }
	virtual void test6C() { }
	virtual void test6D() { }
	virtual void test6E() { }
	virtual void test6F() { }

	virtual void test70() { }
	virtual void test71() { }
	virtual void test72() { }
	virtual void test73() { }
	virtual void test74() { }
	virtual void test75() { }
	virtual void test76() { }
	virtual void test77() { }
	virtual void test78() { }
	virtual void test79() { }
	virtual void test7A() { }
	virtual void test7B() { }
	virtual void test7C() { }
	virtual void test7D() { }
	virtual void test7E() { }
	virtual void test7F() { }

	virtual void test80() { }
	virtual void test81() { }
	virtual void test82() { }
	virtual void test83() { }
	virtual void test84() { }
	virtual void test85() { }
	virtual void test86() { }
	virtual void test87() { }
	virtual void test88() { }
	virtual void test89() { }
	virtual void test8A() { }
	virtual void test8B() { }
	virtual void test8C() { }
	virtual void test8D() { }
	virtual void test8E() { }
	virtual void test8F() { }

	virtual void test90() { }
	virtual void test91() { }
	virtual void test92() { }
	virtual void test93() { }
	virtual void test94() { }
	virtual void test95() { }
	virtual void test96() { }
	virtual void test97() { }
	virtual void test98() { }
	virtual void test99() { }
	virtual void test9A() { }
	virtual void test9B() { }
	virtual void test9C() { }
	virtual void test9D() { }
	virtual void test9E() { }
	virtual void test9F() { }

	virtual void testA0() { }
	virtual void testA1() { }
	virtual void testA2() { }
	virtual void testA3() { }
	virtual void testA4() { }
	virtual void testA5() { }
	virtual void testA6() { }
	virtual void testA7() { }
	virtual void testA8() { }
	virtual void testA9() { }
	virtual void testAA() { }
	virtual void testAB() { }
	virtual void testAC() { }
	virtual void testAD() { }
	virtual void testAE() { }
	virtual void testAF() { }

	virtual void testB0() { }
	virtual void testB1() { }
	virtual void testB2() { }
	virtual void testB3() { }
	virtual void testB4() { }
	virtual void testB5() { }
	virtual void testB6() { }
	virtual void testB7() { }
	virtual void testB8() { }
	virtual void testB9() { }
	virtual void testBA() { }
	virtual void testBB() { }
	virtual void testBC() { }
	virtual void testBD() { }
	virtual void testBE() { }
	virtual void testBF() { }

	virtual void testC0() { }
	virtual void testC1() { }
	virtual void testC2() { }
	virtual void testC3() { }
	virtual void testC4() { }
	virtual void testC5() { }
	virtual void testC6() { }
	virtual void testC7() { }
	virtual void testC8() { }
	virtual void testC9() { }
	virtual void testCA() { }
	virtual void testCB() { }
	virtual void testCC() { }
	virtual void testCD() { }
	virtual void testCE() { }
	virtual void testCF() { }

	virtual void testD0() { }
	virtual void testD1() { }
	virtual void testD2() { }
	virtual void testD3() { }
	virtual void testD4() { }
	virtual void testD5() { }
	virtual void testD6() { }
	virtual void testD7() { }
	virtual void testD8() { }
	virtual void testD9() { }
	virtual void testDA() { }
	virtual void testDB() { }
	virtual void testDC() { }
	virtual void testDD() { }
	virtual void testDE() { }
	virtual void testDF() { }

	virtual void testE0() { }
	virtual void testE1() { }
	virtual void testE2() { }
	virtual void testE3() { }
	virtual void testE4() { }
	virtual void testE5() { }
	virtual void testE6() { }
	virtual void testE7() { }
	virtual void testE8() { }
	virtual void testE9() { }
	virtual void testEA() { }
	virtual void testEB() { }
	virtual void testEC() { }
	virtual void testED() { }
	virtual void testEE() { }
	virtual void testEF() { }

	virtual void testF0() { }
	virtual void testF1() { }
	virtual void testF2() { }
	virtual void testF3() { }
	virtual void testF4() { }
	virtual void testF5() { }
	virtual void testF6() { }
	virtual void testF7() { }
	virtual void testF8() { }
	virtual void testF9() { }
	virtual void testFA() { }
	virtual void testFB() { }
	virtual void testFC() { }
	virtual void testFD() { }
	virtual void testFE() { }
	virtual void testFF() { }

	virtual void run() 
	{
		if ( m_pickup < 0 || m_pickup == 0x00 ) test00();
		if ( m_pickup < 0 || m_pickup == 0x01 ) test01();
		if ( m_pickup < 0 || m_pickup == 0x02 ) test02();
		if ( m_pickup < 0 || m_pickup == 0x03 ) test03();
		if ( m_pickup < 0 || m_pickup == 0x04 ) test04();
		if ( m_pickup < 0 || m_pickup == 0x05 ) test05();
		if ( m_pickup < 0 || m_pickup == 0x06 ) test06();
		if ( m_pickup < 0 || m_pickup == 0x07 ) test07();
		if ( m_pickup < 0 || m_pickup == 0x08 ) test08();
		if ( m_pickup < 0 || m_pickup == 0x09 ) test09();
		if ( m_pickup < 0 || m_pickup == 0x0A ) test0A();
		if ( m_pickup < 0 || m_pickup == 0x0B ) test0B();
		if ( m_pickup < 0 || m_pickup == 0x0C ) test0C();
		if ( m_pickup < 0 || m_pickup == 0x0D ) test0D();
		if ( m_pickup < 0 || m_pickup == 0x0E ) test0E();
		if ( m_pickup < 0 || m_pickup == 0x0F ) test0F();

		if ( m_pickup < 0 || m_pickup == 0x10 ) test10();
		if ( m_pickup < 0 || m_pickup == 0x11 ) test11();
		if ( m_pickup < 0 || m_pickup == 0x12 ) test12();
		if ( m_pickup < 0 || m_pickup == 0x13 ) test13();
		if ( m_pickup < 0 || m_pickup == 0x14 ) test14();
		if ( m_pickup < 0 || m_pickup == 0x15 ) test15();
		if ( m_pickup < 0 || m_pickup == 0x16 ) test16();
		if ( m_pickup < 0 || m_pickup == 0x17 ) test17();
		if ( m_pickup < 0 || m_pickup == 0x18 ) test18();
		if ( m_pickup < 0 || m_pickup == 0x19 ) test19();
		if ( m_pickup < 0 || m_pickup == 0x1A ) test1A();
		if ( m_pickup < 0 || m_pickup == 0x1B ) test1B();
		if ( m_pickup < 0 || m_pickup == 0x1C ) test1C();
		if ( m_pickup < 0 || m_pickup == 0x1D ) test1D();
		if ( m_pickup < 0 || m_pickup == 0x1E ) test1E();
		if ( m_pickup < 0 || m_pickup == 0x1F ) test1F();

		if ( m_pickup < 0 || m_pickup == 0x20 ) test20();
		if ( m_pickup < 0 || m_pickup == 0x21 ) test21();
		if ( m_pickup < 0 || m_pickup == 0x22 ) test22();
		if ( m_pickup < 0 || m_pickup == 0x23 ) test23();
		if ( m_pickup < 0 || m_pickup == 0x24 ) test24();
		if ( m_pickup < 0 || m_pickup == 0x25 ) test25();
		if ( m_pickup < 0 || m_pickup == 0x26 ) test26();
		if ( m_pickup < 0 || m_pickup == 0x27 ) test27();
		if ( m_pickup < 0 || m_pickup == 0x28 ) test28();
		if ( m_pickup < 0 || m_pickup == 0x29 ) test29();
		if ( m_pickup < 0 || m_pickup == 0x2A ) test2A();
		if ( m_pickup < 0 || m_pickup == 0x2B ) test2B();
		if ( m_pickup < 0 || m_pickup == 0x2C ) test2C();
		if ( m_pickup < 0 || m_pickup == 0x2D ) test2D();
		if ( m_pickup < 0 || m_pickup == 0x2E ) test2E();
		if ( m_pickup < 0 || m_pickup == 0x2F ) test2F();

		if ( m_pickup < 0 || m_pickup == 0x30 ) test30();
		if ( m_pickup < 0 || m_pickup == 0x31 ) test31();
		if ( m_pickup < 0 || m_pickup == 0x32 ) test32();
		if ( m_pickup < 0 || m_pickup == 0x33 ) test33();
		if ( m_pickup < 0 || m_pickup == 0x34 ) test34();
		if ( m_pickup < 0 || m_pickup == 0x35 ) test35();
		if ( m_pickup < 0 || m_pickup == 0x36 ) test36();
		if ( m_pickup < 0 || m_pickup == 0x37 ) test37();
		if ( m_pickup < 0 || m_pickup == 0x38 ) test38();
		if ( m_pickup < 0 || m_pickup == 0x39 ) test39();
		if ( m_pickup < 0 || m_pickup == 0x3A ) test3A();
		if ( m_pickup < 0 || m_pickup == 0x3B ) test3B();
		if ( m_pickup < 0 || m_pickup == 0x3C ) test3C();
		if ( m_pickup < 0 || m_pickup == 0x3D ) test3D();
		if ( m_pickup < 0 || m_pickup == 0x3E ) test3E();
		if ( m_pickup < 0 || m_pickup == 0x3F ) test3F();

		if ( m_pickup < 0 || m_pickup == 0x40 ) test40();
		if ( m_pickup < 0 || m_pickup == 0x41 ) test41();
		if ( m_pickup < 0 || m_pickup == 0x42 ) test42();
		if ( m_pickup < 0 || m_pickup == 0x43 ) test43();
		if ( m_pickup < 0 || m_pickup == 0x44 ) test44();
		if ( m_pickup < 0 || m_pickup == 0x45 ) test45();
		if ( m_pickup < 0 || m_pickup == 0x46 ) test46();
		if ( m_pickup < 0 || m_pickup == 0x47 ) test47();
		if ( m_pickup < 0 || m_pickup == 0x48 ) test48();
		if ( m_pickup < 0 || m_pickup == 0x49 ) test49();
		if ( m_pickup < 0 || m_pickup == 0x4A ) test4A();
		if ( m_pickup < 0 || m_pickup == 0x4B ) test4B();
		if ( m_pickup < 0 || m_pickup == 0x4C ) test4C();
		if ( m_pickup < 0 || m_pickup == 0x4D ) test4D();
		if ( m_pickup < 0 || m_pickup == 0x4E ) test4E();
		if ( m_pickup < 0 || m_pickup == 0x4F ) test4F();

		if ( m_pickup < 0 || m_pickup == 0x50 ) test50();
		if ( m_pickup < 0 || m_pickup == 0x51 ) test51();
		if ( m_pickup < 0 || m_pickup == 0x52 ) test52();
		if ( m_pickup < 0 || m_pickup == 0x53 ) test53();
		if ( m_pickup < 0 || m_pickup == 0x54 ) test54();
		if ( m_pickup < 0 || m_pickup == 0x55 ) test55();
		if ( m_pickup < 0 || m_pickup == 0x56 ) test56();
		if ( m_pickup < 0 || m_pickup == 0x57 ) test57();
		if ( m_pickup < 0 || m_pickup == 0x58 ) test58();
		if ( m_pickup < 0 || m_pickup == 0x59 ) test59();
		if ( m_pickup < 0 || m_pickup == 0x5A ) test5A();
		if ( m_pickup < 0 || m_pickup == 0x5B ) test5B();
		if ( m_pickup < 0 || m_pickup == 0x5C ) test5C();
		if ( m_pickup < 0 || m_pickup == 0x5D ) test5D();
		if ( m_pickup < 0 || m_pickup == 0x5E ) test5E();
		if ( m_pickup < 0 || m_pickup == 0x5F ) test5F();

		if ( m_pickup < 0 || m_pickup == 0x60 ) test60();
		if ( m_pickup < 0 || m_pickup == 0x61 ) test61();
		if ( m_pickup < 0 || m_pickup == 0x62 ) test62();
		if ( m_pickup < 0 || m_pickup == 0x63 ) test63();
		if ( m_pickup < 0 || m_pickup == 0x65 ) test65();
		if ( m_pickup < 0 || m_pickup == 0x65 ) test65();
		if ( m_pickup < 0 || m_pickup == 0x66 ) test66();
		if ( m_pickup < 0 || m_pickup == 0x67 ) test67();
		if ( m_pickup < 0 || m_pickup == 0x68 ) test68();
		if ( m_pickup < 0 || m_pickup == 0x69 ) test69();
		if ( m_pickup < 0 || m_pickup == 0x6A ) test6A();
		if ( m_pickup < 0 || m_pickup == 0x6B ) test6B();
		if ( m_pickup < 0 || m_pickup == 0x6C ) test6C();
		if ( m_pickup < 0 || m_pickup == 0x6D ) test6D();
		if ( m_pickup < 0 || m_pickup == 0x6E ) test6E();
		if ( m_pickup < 0 || m_pickup == 0x6F ) test6F();

		if ( m_pickup < 0 || m_pickup == 0x70 ) test70();
		if ( m_pickup < 0 || m_pickup == 0x71 ) test71();
		if ( m_pickup < 0 || m_pickup == 0x72 ) test72();
		if ( m_pickup < 0 || m_pickup == 0x73 ) test73();
		if ( m_pickup < 0 || m_pickup == 0x74 ) test74();
		if ( m_pickup < 0 || m_pickup == 0x75 ) test75();
		if ( m_pickup < 0 || m_pickup == 0x76 ) test76();
		if ( m_pickup < 0 || m_pickup == 0x77 ) test77();
		if ( m_pickup < 0 || m_pickup == 0x78 ) test78();
		if ( m_pickup < 0 || m_pickup == 0x79 ) test79();
		if ( m_pickup < 0 || m_pickup == 0x7A ) test7A();
		if ( m_pickup < 0 || m_pickup == 0x7B ) test7B();
		if ( m_pickup < 0 || m_pickup == 0x7C ) test7C();
		if ( m_pickup < 0 || m_pickup == 0x7D ) test7D();
		if ( m_pickup < 0 || m_pickup == 0x7E ) test7E();
		if ( m_pickup < 0 || m_pickup == 0x7F ) test7F();

		if ( m_pickup < 0 || m_pickup == 0x80 ) test80();
		if ( m_pickup < 0 || m_pickup == 0x81 ) test81();
		if ( m_pickup < 0 || m_pickup == 0x82 ) test82();
		if ( m_pickup < 0 || m_pickup == 0x83 ) test83();
		if ( m_pickup < 0 || m_pickup == 0x84 ) test84();
		if ( m_pickup < 0 || m_pickup == 0x85 ) test85();
		if ( m_pickup < 0 || m_pickup == 0x86 ) test86();
		if ( m_pickup < 0 || m_pickup == 0x87 ) test87();
		if ( m_pickup < 0 || m_pickup == 0x88 ) test88();
		if ( m_pickup < 0 || m_pickup == 0x89 ) test89();
		if ( m_pickup < 0 || m_pickup == 0x8A ) test8A();
		if ( m_pickup < 0 || m_pickup == 0x8B ) test8B();
		if ( m_pickup < 0 || m_pickup == 0x8C ) test8C();
		if ( m_pickup < 0 || m_pickup == 0x8D ) test8D();
		if ( m_pickup < 0 || m_pickup == 0x8E ) test8E();
		if ( m_pickup < 0 || m_pickup == 0x8F ) test8F();

		if ( m_pickup < 0 || m_pickup == 0x90 ) test90();
		if ( m_pickup < 0 || m_pickup == 0x91 ) test91();
		if ( m_pickup < 0 || m_pickup == 0x92 ) test92();
		if ( m_pickup < 0 || m_pickup == 0x93 ) test93();
		if ( m_pickup < 0 || m_pickup == 0x94 ) test94();
		if ( m_pickup < 0 || m_pickup == 0x95 ) test95();
		if ( m_pickup < 0 || m_pickup == 0x96 ) test96();
		if ( m_pickup < 0 || m_pickup == 0x97 ) test97();
		if ( m_pickup < 0 || m_pickup == 0x98 ) test98();
		if ( m_pickup < 0 || m_pickup == 0x99 ) test99();
		if ( m_pickup < 0 || m_pickup == 0x9A ) test9A();
		if ( m_pickup < 0 || m_pickup == 0x9B ) test9B();
		if ( m_pickup < 0 || m_pickup == 0x9C ) test9C();
		if ( m_pickup < 0 || m_pickup == 0x9D ) test9D();
		if ( m_pickup < 0 || m_pickup == 0x9E ) test9E();
		if ( m_pickup < 0 || m_pickup == 0x9F ) test9F();

		if ( m_pickup < 0 || m_pickup == 0xA0 ) testA0();
		if ( m_pickup < 0 || m_pickup == 0xA1 ) testA1();
		if ( m_pickup < 0 || m_pickup == 0xA2 ) testA2();
		if ( m_pickup < 0 || m_pickup == 0xA3 ) testA3();
		if ( m_pickup < 0 || m_pickup == 0xA4 ) testA4();
		if ( m_pickup < 0 || m_pickup == 0xA5 ) testA5();
		if ( m_pickup < 0 || m_pickup == 0xA6 ) testA6();
		if ( m_pickup < 0 || m_pickup == 0xA7 ) testA7();
		if ( m_pickup < 0 || m_pickup == 0xA8 ) testA8();
		if ( m_pickup < 0 || m_pickup == 0xA9 ) testA9();
		if ( m_pickup < 0 || m_pickup == 0xAA ) testAA();
		if ( m_pickup < 0 || m_pickup == 0xAB ) testAB();
		if ( m_pickup < 0 || m_pickup == 0xAC ) testAC();
		if ( m_pickup < 0 || m_pickup == 0xAD ) testAD();
		if ( m_pickup < 0 || m_pickup == 0xAE ) testAE();
		if ( m_pickup < 0 || m_pickup == 0xAF ) testAF();

		if ( m_pickup < 0 || m_pickup == 0xB0 ) testB0();
		if ( m_pickup < 0 || m_pickup == 0xB1 ) testB1();
		if ( m_pickup < 0 || m_pickup == 0xB2 ) testB2();
		if ( m_pickup < 0 || m_pickup == 0xB3 ) testB3();
		if ( m_pickup < 0 || m_pickup == 0xB4 ) testB4();
		if ( m_pickup < 0 || m_pickup == 0xB5 ) testB5();
		if ( m_pickup < 0 || m_pickup == 0xB6 ) testB6();
		if ( m_pickup < 0 || m_pickup == 0xB7 ) testB7();
		if ( m_pickup < 0 || m_pickup == 0xB8 ) testB8();
		if ( m_pickup < 0 || m_pickup == 0xB9 ) testB9();
		if ( m_pickup < 0 || m_pickup == 0xBA ) testBA();
		if ( m_pickup < 0 || m_pickup == 0xBB ) testBB();
		if ( m_pickup < 0 || m_pickup == 0xBC ) testBC();
		if ( m_pickup < 0 || m_pickup == 0xBD ) testBD();
		if ( m_pickup < 0 || m_pickup == 0xBE ) testBE();
		if ( m_pickup < 0 || m_pickup == 0xBF ) testBF();

		if ( m_pickup < 0 || m_pickup == 0xC0 ) testC0();
		if ( m_pickup < 0 || m_pickup == 0xC1 ) testC1();
		if ( m_pickup < 0 || m_pickup == 0xC2 ) testC2();
		if ( m_pickup < 0 || m_pickup == 0xC3 ) testC3();
		if ( m_pickup < 0 || m_pickup == 0xC4 ) testC4();
		if ( m_pickup < 0 || m_pickup == 0xC5 ) testC5();
		if ( m_pickup < 0 || m_pickup == 0xC6 ) testC6();
		if ( m_pickup < 0 || m_pickup == 0xC7 ) testC7();
		if ( m_pickup < 0 || m_pickup == 0xC8 ) testC8();
		if ( m_pickup < 0 || m_pickup == 0xC9 ) testC9();
		if ( m_pickup < 0 || m_pickup == 0xCA ) testCA();
		if ( m_pickup < 0 || m_pickup == 0xCB ) testCB();
		if ( m_pickup < 0 || m_pickup == 0xCC ) testCC();
		if ( m_pickup < 0 || m_pickup == 0xCD ) testCD();
		if ( m_pickup < 0 || m_pickup == 0xCE ) testCE();
		if ( m_pickup < 0 || m_pickup == 0xCF ) testCF();

		if ( m_pickup < 0 || m_pickup == 0xD0 ) testD0();
		if ( m_pickup < 0 || m_pickup == 0xD1 ) testD1();
		if ( m_pickup < 0 || m_pickup == 0xD2 ) testD2();
		if ( m_pickup < 0 || m_pickup == 0xD3 ) testD3();
		if ( m_pickup < 0 || m_pickup == 0xD4 ) testD4();
		if ( m_pickup < 0 || m_pickup == 0xD5 ) testD5();
		if ( m_pickup < 0 || m_pickup == 0xD6 ) testD6();
		if ( m_pickup < 0 || m_pickup == 0xD7 ) testD7();
		if ( m_pickup < 0 || m_pickup == 0xD8 ) testD8();
		if ( m_pickup < 0 || m_pickup == 0xD9 ) testD9();
		if ( m_pickup < 0 || m_pickup == 0xDA ) testDA();
		if ( m_pickup < 0 || m_pickup == 0xDB ) testDB();
		if ( m_pickup < 0 || m_pickup == 0xDC ) testDC();
		if ( m_pickup < 0 || m_pickup == 0xDD ) testDD();
		if ( m_pickup < 0 || m_pickup == 0xDE ) testDE();
		if ( m_pickup < 0 || m_pickup == 0xDF ) testDF();

		if ( m_pickup < 0 || m_pickup == 0xE0 ) testE0();
		if ( m_pickup < 0 || m_pickup == 0xE1 ) testE1();
		if ( m_pickup < 0 || m_pickup == 0xE2 ) testE2();
		if ( m_pickup < 0 || m_pickup == 0xE3 ) testE3();
		if ( m_pickup < 0 || m_pickup == 0xE4 ) testE4();
		if ( m_pickup < 0 || m_pickup == 0xE5 ) testE5();
		if ( m_pickup < 0 || m_pickup == 0xE6 ) testE6();
		if ( m_pickup < 0 || m_pickup == 0xE7 ) testE7();
		if ( m_pickup < 0 || m_pickup == 0xE8 ) testE8();
		if ( m_pickup < 0 || m_pickup == 0xE9 ) testE9();
		if ( m_pickup < 0 || m_pickup == 0xEA ) testEA();
		if ( m_pickup < 0 || m_pickup == 0xEB ) testEB();
		if ( m_pickup < 0 || m_pickup == 0xEC ) testEC();
		if ( m_pickup < 0 || m_pickup == 0xED ) testED();
		if ( m_pickup < 0 || m_pickup == 0xEE ) testEE();
		if ( m_pickup < 0 || m_pickup == 0xEF ) testEF();

		if ( m_pickup < 0 || m_pickup == 0xF0 ) testF0();
		if ( m_pickup < 0 || m_pickup == 0xF1 ) testF1();
		if ( m_pickup < 0 || m_pickup == 0xF2 ) testF2();
		if ( m_pickup < 0 || m_pickup == 0xF3 ) testF3();
		if ( m_pickup < 0 || m_pickup == 0xF4 ) testF4();
		if ( m_pickup < 0 || m_pickup == 0xF5 ) testF5();
		if ( m_pickup < 0 || m_pickup == 0xF6 ) testF6();
		if ( m_pickup < 0 || m_pickup == 0xF7 ) testF7();
		if ( m_pickup < 0 || m_pickup == 0xF8 ) testF8();
		if ( m_pickup < 0 || m_pickup == 0xF9 ) testF9();
		if ( m_pickup < 0 || m_pickup == 0xFA ) testFA();
		if ( m_pickup < 0 || m_pickup == 0xFB ) testFB();
		if ( m_pickup < 0 || m_pickup == 0xFC ) testFC();
		if ( m_pickup < 0 || m_pickup == 0xFD ) testFD();
		if ( m_pickup < 0 || m_pickup == 0xFE ) testFE();
		if ( m_pickup < 0 || m_pickup == 0xFF ) testFF();
	}
	
    private:
	unittest( unittest const& );
	unittest& operator=( unittest const& );
};

#ifndef NO_NAMESPACE
}
#endif
#endif
