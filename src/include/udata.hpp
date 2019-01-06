//
// udata.h --- test data class for unit test of Pointer Container Template Library
//
//      2019/01/07, version 1.0.0
//
//      © Kazunari Saitoh, 2018-2019.
//      Distributed under the Boost Software License, Version 1.0.
//      (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//

#ifndef __DPTL_UDATA_HPP
#define __DPTL_UDATA_HPP

#ifndef DPTL_NAMESPACE
#define DPTL_NAMESPACE dptl
#endif

#include <cstdio>
#include <cstdlib>
#include <deque>

#ifndef NO_NAMESPACE
namespace DPTL_NAMESPACE {
#endif // NO_NAMESPACE

// Implementation
// -----------------------------------------------------------------------------
// Note: This header has static variables.
// 	 There must be only one linkage for one executable.
//	 If the inline variable is available in future c++, it must be replaced
//	 inline version.
//
// variable-sized native array terminator
#ifndef __cpp_variable_templates
template <typename T> struct vtermi { static const T nator; };
template <typename T> const T vtermi<T>::nator = T();
#else
template <typename T> const T vterminator = T();
#endif

// int test::cntr = 0;			  // defined after test class declaration

// Specification
// -----------------------------------------------------------------------------
class base
{
    public:
	virtual ~base() = default;

	virtual base* clone() const = 0;
	int cmp( const base& b ) const
	{
		if ( typeid( *this ) == typeid( b )) 
			return cmpaux( b );
		else if ( typeid( *this ).before( typeid( b )) )
			return -1;
		else 
			return 1;
	} 
	virtual void print( FILE* f = stdout ) const = 0;

    protected:
	virtual int cmpaux( const base& b ) const = 0;
};

// -----------------------------------------------------------------------------
class tint : public base
{
	int	m_int;

    public:
	explicit tint( int n = 0 ) : m_int( n ) { }
	tint( tint const& ) = default;
	tint& operator=( tint const& ) = default;
	~tint() = default;

	int num() const { return m_int; }

	tint* clone() const override { return new tint( m_int ); } 
	void print( FILE* f = stdout ) const override
	{
		fprintf( f, "tint(%d)", m_int );
	}

    private:
	int cmpaux( const base& b ) const override
	{
		const tint&	i = static_cast<const tint&>( b );	// faster than dynamic_cast

		return ( m_int < i.m_int ) ? -1 : ( i.m_int < m_int ) ? 1 : 0;
	}
};

// -----------------------------------------------------------------------------
class tstr : public base
{
	char*	m_str;

    public:
	explicit tstr( const char* b = nullptr ) : m_str( b ? strdup( b ) : nullptr ) { }

	// --- sample of the class without copy constructor --- ( using dp_dup<tstr> )
	// tstr( tstr const& ts ) : m_str( ts.m_str ? strdup( ts.m_str ) : nullptr ) { } 
	// tstr& operator=( tstr const& ts ) 
	// {
	// 	if ( &ts != this ) {
	// 		delete [] m_str; 
	// 		m_str = ts.m_str ? strdup( ts.m_str ) : nullptr;
	// 	}
	// 	return *this;
	// }
	tstr( tstr const& ts ) = delete;
	tstr& operator=( tstr const& ts ) = delete;
	~tstr() { if ( m_str ) delete [] m_str; }

	const char*	cstr() const { return m_str; }

	tstr* clone() const override { return new tstr( m_str ? strdup( m_str ) : nullptr ); } 
	void print( FILE* f = stdout ) const override
	{
		fprintf( f, "tstr(\"%s\")", m_str ? m_str : "<null>" );
	}

    private:
	int cmpaux( const base& b ) const override
	{
		const tstr&	s = static_cast<const tstr&>( b );	// faster than dynamic_cast

		return ( m_str && s.m_str ) ? strcmp( m_str, s.m_str ) :
			( !m_str && !s.m_str ) ? 0 : !m_str ? -1 : 1;
	}
};

// --- sample of the class without copy constructor ---
//	Note: It's also defined base class dp_dup<> because base has no copy constructor 
template <> struct dp_dup<tstr*> { tstr* operator()( const tstr* ts ) { return ts->clone(); }};
template <> struct dp_dup<base*> { base* operator()( const base* bs ) { return bs->clone(); }};

// -----------------------------------------------------------------------------
class test : public base
{
	int	m_int;
	char*	m_str;

	static int cntr;

    public:
	test() : m_int( 0 ), m_str( nullptr )
	{
#ifdef _DEBUG
		fprintf( stderr, "%03d: ==> construct %p: ", cntr, this );
		print( stderr );
		fprintf( stderr, "\n" );
		++cntr;
#endif
	}
	test( int n, const char* b ) : m_int( n ), m_str( b ? strdup( b ) : nullptr )
	{
#ifdef _DEBUG
		fprintf( stderr, "%03d: ==> construct %p: ", cntr, this );
		print( stderr );
		fprintf( stderr, "\n" );
		++cntr;
#endif
	}
	test( const test& t ) : m_int( t.m_int ), m_str( t.m_str ? strdup( t.m_str ) : nullptr )
	{
#ifdef _DEBUG
		fprintf( stderr, "%03d: ==> copy_construct %p: ", cntr, this );
		print( stderr );
		fprintf( stderr, "\n" );
		++cntr;
#endif
	}
	test& operator=( const test& t )
	{
#ifdef _DEBUG
		fprintf( stderr, "%03d: === copy_assign %p: ", cntr, this );
		print( stderr );
		fprintf( stderr, " <= " );
		t.print( stderr );
		fprintf( stderr, "\n" );
#endif
		if ( &t != this ) {
			m_int = t.m_int;
			if ( m_str ) delete [] m_str;
			m_str = t.m_str ? strdup( t.m_str ) : nullptr;
		}
		return *this;
	}
	test( test&& t ) : m_int( t.m_int ), m_str( t.m_str )
	{
#ifdef _DEBUG
		fprintf( stderr, "%03d: ==> move_construct %p: ", cntr, this );
		print( stderr );
		fprintf( stderr, "\n" );
		++cntr;
#endif
		t.m_int = 0;
		t.m_str = nullptr;
	}
	test& operator=( test&& t )
	{
#ifdef _DEBUG
		fprintf( stderr, "%03d: === move_assign %p: ", cntr, this );
		print( stderr );
		fprintf( stderr, " <= " );
		t.print( stderr );
		fprintf( stderr, "\n" );
#endif
		if ( &t != this ) {
			m_int = t.m_int;
			if ( m_str ) delete [] m_str;
			m_str = t.m_str;

			t.m_int = 0;
			t.m_str = nullptr;
		}
		return *this;
	}
	~test()
	{
#ifdef _DEBUG
		--cntr;
		fprintf( stderr, "%03d: <== destruct %p: ", cntr, this );
		print( stderr );
		fprintf( stderr, "\n" );
#endif
		if ( m_str ) delete [] m_str;
	}

	const char*	cstr() const { return m_str; }
	int		num()  const { return m_int; }

	void set_cstr( const char* s )
	{
		if ( m_str != s ) {
			if ( m_str ) delete [] m_str;
			m_str = s ? strdup( s ) : nullptr; 
		}
	}
	void set_num( int i ) { m_int = i; }		
		
	base* clone() const override { return dp_dup<test*>()( this ); } 

	static int getCntr() { return cntr; }

#ifdef __OMT11_SRZ_HPP
	// serializer support
	void deserialize( deserializer& in )
	{
		in >> begin >> m_int >> rel( m_str ) >> end;
	}
	void serialize( serializer& out ) const
	{
		out << begin << m_int << m_str << end;
	}
#endif

	void print( FILE* f = stdout ) const override
	{
		if ( m_str ) 
			fprintf( f, "test(%d, \"%s\")", m_int, m_str );
		else
			fprintf( f, "test(%d, <null>)", m_int );
	}

    private:
	int cmpaux( const base& b ) const override
	{
		const test&	t = dynamic_cast<const test&>( b );

		return ( m_int < t.m_int ) ? -1 : 
	               ( t.m_int < m_int ) ? 1 :
		       ( m_str && t.m_str ) ? strcmp( m_str, t.m_str ) :
		       ( !m_str && !t.m_str ) ? 0 : !m_str ? -1 : 1;
	}
};

// comparison for test, tint, tstr
// -----------------------------------------------------------------------------
inline bool operator== ( const base& a, const base& b ) { return a.cmp( b ) == 0; }
inline bool operator!= ( const base& a, const base& b ) { return a.cmp( b ) != 0; }
inline bool operator<  ( const base& a, const base& b ) { return a.cmp( b ) < 0;  }
inline bool operator>  ( const base& a, const base& b ) { return a.cmp( b ) > 0; }
inline bool operator>= ( const base& a, const base& b ) { return a.cmp( b ) <= 0; }
inline bool operator<= ( const base& a, const base& b ) { return a.cmp( b ) >= 0; }

// -----------------------------------------------------------------------------
class basic_testdata
{
    protected:
	// test data
	const char* s0;	 	// "": zero length string
	const char* s1;
	const char* s2;
	const char* s3;
	const char* s4;
	const char* s5;
	const char* s6;
	const char* s7;
	const char* s8;
	const char* s9;

	test t0;		// default constructor will be applied
	test t1;
	test t2;
	test t3;
	test t4;
	test t5;
	test t6;
	test t7;
	test t8;
	test t9;

	tstr u0;		// default constructor will be applied
	tstr u1;
	tstr u2;
	tstr u3;
	tstr u4;
	tstr u5;
	tstr u6;
	tstr u7;
	tstr u8;
	tstr u9;

	tint i0;		// default constructor will be applied
	tint i1;
	tint i2;
	tint i3;
	tint i4;
	tint i5;
	tint i6;
	tint i7;
	tint i8;
	tint i9;

	test v0[ 1 ];		// variable sized native array
	test v1[ 4 ];
	test v2[ 3 ];
	test v3[ 4 ];
	test v4[ 2 ];
	test v5[ 5 ];
	test v6[ 4 ];
	test v7[ 3 ];
	test v8[ 2 ];
	test v9[ 2 ];

	test w0[ 3 ];		// fixed sized native array
	test w1[ 3 ];
	test w2[ 3 ];
	test w3[ 3 ];
	test w4[ 3 ];
	test w5[ 3 ];
	test w6[ 3 ];
	test w7[ 3 ];
	test w8[ 3 ];
	test w9[ 3 ];

    public:
	basic_testdata()
	      : s0( "" ),
		s1( "Earth" ),
		s2( "Jupiter" ),
		s3( "Mars" ),
		s4( "Mercury" ),
		s5( "Neptune" ),
		s6( "Mars" ),			 // same value with s3
		s7( "Saturn" ),
		s8( "Uranus" ),
		s9( "Venus" ),

		t0(),
		t1( 1, s1 ),
		t2( 2, s2 ),
		t3( 3, s3 ),
		t4( 4, s4 ),
		t5( 5, s5 ),
		t6( 3, s6 ),			// same value with t3
		t7( 7, s7 ),
		t8( 8, s8 ),
		t9( 9, s9 ),

		u0(),
		u1( s1 ),
		u2( s2 ),
		u3( s3 ),
		u4( s4 ),
		u5( s5 ),
		u6( s6 ),			// same value with u3
		u7( s7 ),
		u8( s8 ),
		u9( s9 ),

		i0(),
		i1( 1 ),
		i2( 2 ),
		i3( 3 ),
		i4( 4 ),
		i5( 5 ),
		i6( 3 ),			// same value with i3
		i7( 7 ),
		i8( 8 ),
		i9( 9 ),

		v0 { t0 },
		v1 { t1, t2, t3, t0 },
		v2 { t2, t3, t0 },
		v3 { t3, t4, t5, t0 },
		v4 { t4, t0 },
		v5 { t5, t6, t7, t8, t0 },
		v6 { t6, t4, t5, t0 },		// same value with v3
		v7 { t7, t8, t0 },
		v8 { t8, t0 },
		v9 { t9, t0 },

	        w0 {},
		w1 { t1, t2, t3 },
		w2 { t2, t3, t4 },
		w3 { t3, t4, t5 },
		w4 { t4, t5, t6 },
		w5 { t5, t6, t7 },
		w6 { t6, t4, t5 },		// same value with s3
		w7 { t7, t8, t9 },
		w8 { t8, t9, t0 },
		w9 { t9, t0, t0 }
	{ }
	~basic_testdata() = default;
};

// Implementation
// -----------------------------------------------------------------------------
int test::cntr = 0;

//
// Utilities

// dp_hash for test
// -----------------------------------------------------------------------------
template<> struct dp_hash<test>
{
	unsigned long operator()( const test& t ) const
	{
		dp_hash<const char*>    fs;
		dp_hash<unsigned long>  fn;

		return fs( t.cstr()) + fn( t.num()) * 5;
	}
};
template<> struct dp_hash<const test> : public dp_hash<test> { };

// length of variable-sized native array
// -----------------------------------------------------------------------------
template<typename T>
inline size_t vlength( const T* const& v )
{
        size_t          r = 0;
#ifndef __cpp_variable_templates
	const T		e = vtermi<T>::nator;  
#else
	const T		e = vterminator<T>;
#endif
        while ( v[ r ] != e ) ++r;

        return r;
}

// print() common
// -----------------------------------------------------------------------------
inline void print( const base* const& p ) { if ( p ) p->print( stdout ); else printf( "nullptr" ); }
inline void print( base const& t )	  { t.print( stdout ); }

inline void print( const char* const& s ) { if ( s ) printf( "\"%s\"", s ); else printf( "nullptr" ); }
inline void print( const char (&s)[0] )   { if ( s ) printf( "\"%s\"", s ); else printf( "nullptr" ); }
inline void print( const wchar_t* const& s ) { if ( s ) wprintf( L"\"%s\"", s ); else printf( "nullptr" ); }
inline void print( const wchar_t (&s)[0] )   { if ( s ) wprintf( L"\"%s\"", s ); else printf( "nullptr" ); }

inline void print( long const& i )	  { printf( "%ld", i ); }
inline void print( int const& i )	  { printf( "%d", i ); }
inline void print( char const& c )	  { printf( "'%c'", c ); }

// print native array
// -----------------------------------------------------------------------------
template <typename T>
inline void printv( const T* v ) 
{
#ifndef __cpp_variable_templates
	const T		e = vtermi<T>::nator;  
#else
	const T		e = vterminator<T>;
#endif
	printf( "[ " );
	for ( size_t i = 0; v[ i ] != e; ++i ) {
		if ( 0 < i ) printf( ", " );
		print( v[ i ] );
	}
	printf( " ]" );
}

template <typename T>
inline void printv( const T* v, size_t n ) 
{
	printf( "[ " );
	for ( size_t i = 0; i < n; ++i ) {
		if ( 0 < i ) printf( ", " );
		print( v[ i ] );
	}
	printf( " ]" );
}

// print with linefeed
// -----------------------------------------------------------------------------
template <typename T> inline void println(  T const& t )  	   { print(  t );    printf( "\n" ); }
template <typename T> inline void printvln( T const& t ) 	   { printv( t );    printf( "\n" ); }
template <typename T> inline void printvln( T const& t, size_t n ) { printv( t, n ); printf( "\n" ); }

// print_element for print pcontainer
// -----------------------------------------------------------------------------
template <typename T, size_t N = 0> struct print_element	{ void operator()( T const& p )		{ print( p ); }};
template <typename V>               struct print_element<V*>	{ void operator()( const V* const& p )	{ print( p ); }};
template <typename V>               struct print_element<V[]>	{ void operator()( const V* const& p )	{ printv( p ); }};
template <typename V, size_t N>	    struct print_element<V[N]>	{ void operator()( const V* const& p )	{ printv( p, N ); }};

template <> struct print_element<char[]> 		{ void operator()( const char* const& p )    { print( p ); }};
template <> struct print_element<const char[]> 		{ void operator()( const char* const& p )    { print( p ); }};
template <> struct print_element<wchar_t[]> 		{ void operator()( const wchar_t* const& p ) { print( p ); }};
template <> struct print_element<const wchar_t[]> 	{ void operator()( const wchar_t* const& p ) { print( p ); }};

// STL container for deep pointer
// -----------------------------------------------------------------------------
#ifdef __DPTL_DP_LIST_HPP
template <typename T>
inline void print( __dp_list<T> const& pd )
{
        printf( "---> %p(%zu)\n", &pd, pd.size());
        for ( auto i = pd.begin(); i != pd.end(); ++i ) {
                if ( *i ) print_element<dp_type<T>>()( *i ); else printf( "null" );
                printf( "\n" );
        }
        printf( "<---\n" );
}

template <typename T>
inline void print( std::list<T> const& pd )
{
        printf( "---> %p(%zu)\n", &pd, pd.size());
        for ( auto i = pd.begin(); i != pd.end(); ++i ) {
                if ( *i ) print_element<T>()( *i ); else printf( "null" );
                printf( "\n" );
        }
        printf( "<---\n" );
}
#endif

// -----------------------------------------------------------------------------
#ifdef __DPTL_DP_FORWARD_LIST_HPP
template <typename T>
inline void print( __dp_forward_list<T> const& pd )
{
	size_t	n;

        printf( "---> %p\n", &pd );
        for ( auto i = pd.begin(); i != pd.end(); ++i, ++n ) {
                if ( *i ) print_element<dp_type<T>>()( *i ); else printf( "null" );
                printf( "\n" );
        }
        printf( "<---(%lu)\n", n );
}

template <typename T>
inline void print( std::forward_list<T> const& pd )
{
	size_t	n;

        printf( "---> %p\n", &pd );
        for ( auto i = pd.begin(); i != pd.end(); ++i, ++n ) {
                if ( *i ) print_element<T>()( *i ); else printf( "null" );
                printf( "\n" );
        }
        printf( "<---(%lu)\n", n );
}
#endif

// -----------------------------------------------------------------------------
#ifdef __DPTL_DP_DEQUE_HPP
template <typename T>
inline void print( __dp_deque<T> const& pd )
{
        printf( "---> %p(%zu)\n", &pd, pd.size());
        for ( auto i = pd.begin(); i != pd.end(); ++i ) {
                if ( auto&& p = i->get()) print_element<dp_type<T>>()( p ); else printf( "null" );
                printf( "\n" );
        }
        printf( "<---\n" );
}

template <typename T>
inline void print( std::deque<T> const& pd )
{
        printf( "---> %p(%zu)\n", &pd, pd.size());
        for ( auto i = pd.begin(); i != pd.end(); ++i ) {
                if ( auto&& p = i->get()) print_element<T>()( p ); else printf( "null" );
                printf( "\n" );
        }
        printf( "<---\n" );
}
#endif

// -----------------------------------------------------------------------------
#ifdef __DPTL_DP_SET_HPP
template <typename T>
inline void print( __dp_set<T> const& pd )
{
        printf( "---> %p(%zu)\n", &pd, pd.size());
        for ( auto i = pd.begin(); i != pd.end(); ++i ) {
                if ( *i ) print_element<dp_type<T>>()( *i ); else printf( "null" );
                printf( "\n" );
        }
        printf( "<---\n" );
}

template <typename T>
inline void print( std::set<T> const& pd )
{
        printf( "---> %p(%zu)\n", &pd, pd.size());
        for ( auto i = pd.begin(); i != pd.end(); ++i ) {
                if ( *i ) print_element<T>()( *i ); else printf( "null" );
                printf( "\n" );
        }
        printf( "<---\n" );
}

template <typename T>
inline void print( __dp_multiset<T> const& pd )
{
        printf( "--=> %p(%zu)\n", &pd, pd.size());
        for ( auto i = pd.begin(); i != pd.end(); ++i ) {
                if ( *i ) print_element<dp_type<T>>()( *i ); else printf( "null" );
                printf( "\n" );
        }
        printf( "<=--\n" );
}

template <typename T>
inline void print( std::multiset<T> const& pd )
{
        printf( "--=> %p(%zu)\n", &pd, pd.size());
        for ( auto i = pd.begin(); i != pd.end(); ++i ) {
                if ( *i ) print_element<T>()( *i ); else printf( "null" );
                printf( "\n" );
        }
        printf( "<=--\n" );
}
#endif

// -----------------------------------------------------------------------------
#ifdef __DPTL_DP_UNORDERED_SET_HPP
template <typename T>
inline void print( __dp_unordered_set<T> const& pd )
{
        printf( "---> %p(%zu)\n", &pd, pd.size());
        for ( auto i = pd.begin(); i != pd.end(); ++i ) {
                if ( *i ) print_element<dp_type<T>>()( *i ); else printf( "null" );
                printf( "\n" );
        }
        printf( "<---\n" );
}

template <typename T>
inline void print( std::unordered_set<T> const& pd )
{
        printf( "---> %p(%zu)\n", &pd, pd.size());
        for ( auto i = pd.begin(); i != pd.end(); ++i ) {
                if ( *i ) print_element<T>()( *i ); else printf( "null" );
                printf( "\n" );
        }
        printf( "<---\n" );
}

template <typename T>
inline void print( __dp_unordered_multiset<T> const& pd )
{
        printf( "--=> %p(%zu)\n", &pd, pd.size());
        for ( auto i = pd.begin(); i != pd.end(); ++i ) {
                if ( *i ) print_element<dp_type<T>>()( *i ); else printf( "null" );
                printf( "\n" );
        }
        printf( "<=--\n" );
}

template <typename T>
inline void print( std::unordered_multiset<T> const& pd )
{
        printf( "--=> %p(%zu)\n", &pd, pd.size());
        for ( auto i = pd.begin(); i != pd.end(); ++i ) {
                if ( *i ) print_element<T>()( *i ); else printf( "null" );
                printf( "\n" );
        }
        printf( "<=--\n" );
}
#endif

// -----------------------------------------------------------------------------
#ifdef __DPTL_DP_MAP_HPP
template <typename K, typename T>
inline void print( __dp_map<K,T> const& pd )
{
        printf( "---> %p(%zu)\n", &pd, pd.size());
        for ( auto i = pd.begin(); i != pd.end(); ++i ) {
                if ( i->first ) print_element<dp_const_type<K>>()( i->first ); else printf( "null" );
                printf( " -> " );
                if ( i->second ) print_element<dp_type<T>>()( i->second ); else printf( "null" );
                printf( "\n" );
        }
        printf( "<---\n" );
}

template <typename K, typename T>
inline void print( __dp_multimap<K,T> const& pd )
{
        printf( "--=> %p(%zu)\n", &pd, pd.size());
        for ( auto i = pd.begin(); i != pd.end(); ++i ) {
                if ( i->first ) print_element<dp_const_type<K>>()( i->first ); else printf( "null" );
                printf( " -> " );
                if ( i->second ) print_element<dp_type<T>>()( i->second ); else printf( "null" );
                printf( "\n" );
        }
        printf( "<=--\n" );
}
#endif

// -----------------------------------------------------------------------------
#ifdef __DPTL_DP_UNORDERED_MAP_HPP
template <typename K, typename T>
inline void print( __dp_unordered_map<K,T> const& pd )
{
        printf( "---> %p(%zu)\n", &pd, pd.size());
        for ( auto i = pd.begin(); i != pd.end(); ++i ) {
                if ( i->first ) print_element<dp_const_type<K>>()( i->first ); else printf( "null" );
                printf( " -> " );
                if ( i->second ) print_element<dp_type<T>>()( i->second ); else printf( "null" );
                printf( "\n" );
        }
        printf( "<---\n" );
}

template <typename K, typename T>
inline void print( __dp_unordered_multimap<K,T> const& pd )
{
        printf( "--=> %p(%zu)\n", &pd, pd.size());
        for ( auto i = pd.begin(); i != pd.end(); ++i ) {
                if ( i->first ) print_element<dp_const_type<K>>()( i->first ); else printf( "null" );
                printf( " -> " );
                if ( i->second ) print_element<dp_type<T>>()( i->second ); else printf( "null" );
                printf( "\n" );
        }
        printf( "<=--\n" );
}
#endif

// -----------------------------------------------------------------------------
#ifdef __DPTL_DP_ARRAY_HPP
template <typename T, size_t N>
inline void print( __dp_array<T,N> const& va )
{       
        printf( "---> %p(%zu/%zu)\n", &va, va.size(), N );
        for ( auto i = va.begin(); i != va.end(); ++i ) {
                if ( *i ) print_element<dp_type<T>>()( *i ); else printf( "null" );
                printf( "\n" );
        }
        printf( "<---\n" );
}

template <typename T, size_t N>
inline void print( std::array<T,N> const& va )
{       
        printf( "---> %p(%zu/%zu)\n", &va, va.size(), N );
        for ( auto i = va.begin(); i != va.end(); ++i ) {
                if ( *i ) print_element<T>()( *i ); else printf( "null" );
                printf( "\n" );
        }
        printf( "<---\n" );
}
#endif

// -----------------------------------------------------------------------------
#ifdef __DPTL_DP_VECTOR_HPP
template <typename T>
inline void print( __dp_vector<T> const& vc )
{       
        printf( "---> %p(%zu/%zu)\n", &vc, vc.size(), vc.capacity());
        for ( auto i = vc.begin(); i != vc.end(); ++i ) {
                if ( *i ) print_element<dp_type<T>>()( *i ); else printf( "null" );
                printf( "\n" );
        }
        printf( "<---\n" );
}

template <typename T>
inline void print( std::vector<T> const& vc )
{       
        printf( "---> %p(%zu/%zu)\n", &vc, vc.size(), vc.capacity());
        for ( auto i = vc.begin(); i != vc.end(); ++i ) {
                if ( *i ) print_element<T>()( *i ); else printf( "null" );
                printf( "\n" );
        }
        printf( "<---\n" );
}
#endif

#ifndef NO_NAMESPACE
}
#endif
#endif
