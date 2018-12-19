//
// deep_ptr.hpp --- deep pointer: adapter for STL and STL like value-based container
//
//	2018/11/29	K.Saitoh; all right reserved
//

#ifndef __DPTL_DPADAPTER_H
#define __DPTL_DPADAPTER_H

#ifndef DPTL_NAMESPACE
#define DPTL_NAMESPACE dptl
#endif

#include <algorithm>
#include <cwchar>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <initializer_list>
#include <iterator>
#include <locale>
#include <limits>
#include <stdexcept>
#include <type_traits>
#include <utility>

// c++14 compatible type transformation traits for c++11
#ifndef __cpp_lib_transformation_trait_aliases
namespace std {
	template <bool C, typename T, typename F> using conditional_t      = typename conditional<C,T,F>::type;
	template <bool C, typename T = void>      using enable_if_t	   = typename enable_if<C,T>::type;
	template <typename T>			  using add_pointer_t      = typename add_pointer<T>::type;
	template <typename T>			  using remove_reference_t = typename remove_reference<T>::type;
	template <typename T>			  using remove_const_t     = typename remove_const<T>::type;
	template <typename T>			  using remove_cv_t	   = typename remove_cv<T>::type;
}
#endif

#ifndef NO_NAMESPACE
namespace DPTL_NAMESPACE {
#endif

// Implementation depended constant
// -----------------------------------------------------------------------------
static const size_t MinimumHashSize = 256;

// Type Deducers ( use like 'typename const_p<T>::t' )
// ============================================================================
template <typename T>
struct func_p
{
	typedef std::conditional_t<std::is_function<T>::value, std::add_pointer_t<T>, T> t;
};

// Pointer deducer for native array
template <typename T, size_t N = 0> struct value_p		{ typedef typename func_p<T>::t t; };
template <typename V>		    struct value_p<V[]>		{ typedef V* t; };
template <typename V, size_t N>     struct value_p<V[N]>	{ typedef V* t; };

// Const pointer deducer
template <typename T, size_t N = 0> struct const_p		{ typedef typename func_p<T>::t t; };
template <typename V>		    struct const_p<V*>		{ typedef const V* t; };
template <typename V>		    struct const_p<V[]>		{ typedef const V* t; };
template <typename V, size_t N>     struct const_p<V[N]>	{ typedef const V* t; };

// Const array deducer
template <typename T, size_t N = 0> struct const_a		{ typedef typename func_p<T>::t t; };
template <typename V>		    struct const_a<V*>		{ typedef const V* t; };
template <typename V>		    struct const_a<V[]>		{ typedef const V t[]; };
template <typename V, size_t N>     struct const_a<V[N]>	{ typedef const V t[N]; };

// Common Function Objects (dp_hash, dp_eql, dp_less, dp_dup, dp_del)
// ============================================================================

// dp_hash
// ----------------------------------------------------------------------------
template <typename K, size_t N = 0> struct dp_hash;

// ----------------------------------------------------------------------------
template <typename K> struct dp_hash_varray
{
	size_t operator()( const K* const& key ) const
	{
		size_t  r = 0;
		const K t = K();

		if ( key ) {
			for ( const K* p = key; *p != t; ++p ) r += ( r << 2 ) + dp_hash<K>()( *p );
		}
		return r;
	}
};

template <typename K> struct dp_hash_id
{
	size_t operator()( K const& key ) const
	{
		return static_cast<size_t>( key );
	}
};

// ----------------------------------------------------------------------------
struct dp_hash_pvoid
{
	size_t operator()( const void* key ) const
	{
		size_t  r = 0;
		size_t  x = reinterpret_cast<size_t>( key );

		while ( 0 < x )	{
			r += x + x + x;
			x >>= 5;
		}
		return r;
	}
};

// ----------------------------------------------------------------------------
template <typename K, size_t N> struct dp_hash
{
	size_t operator()( K const& key ) const
	{
		size_t  r = 0;
		size_t  x = static_cast<size_t>( key );

		while ( 0 < x )	{
			r += x + x + x;
			x >>= 5;
		}
		return r;
	}
};

// partially specialized dp_hashs
// ----------------------------------------------------------------------------
template <> struct dp_hash<char>	: public dp_hash_id<char> { };
template <> struct dp_hash<const char>  : public dp_hash_id<char> { };

template <> struct dp_hash<void*>       : public dp_hash_pvoid { };
template <> struct dp_hash<const void*> : public dp_hash_pvoid { };

template <typename K> struct dp_hash<K*>
{
	size_t operator()( const K* const& key ) const	{ return key ? dp_hash<K>()( *key ) : 0; }
};
template <typename K>	   struct dp_hash<K[]>  : public dp_hash_varray<K>	{ };
template <typename K, size_t N> struct dp_hash<K[N]>
{
	size_t operator()( const K* const& key ) const
	{
		size_t  r = 0;

		if ( key ) {
			for ( size_t i = 0; i < N; ++i ) r += ( r << 2 ) + dp_hash<K>()( key[ i ] );
		}
		return r;
	}
};

// dp_eql
// ----------------------------------------------------------------------------
template <typename K, size_t N = 0> struct dp_eql
{
	bool operator()( K const& k1, K const& k2 ) const { return k1 == k2; }
};

// ----------------------------------------------------------------------------
template <typename K> struct dp_eql_varray
{
	const K	 term = K();

	bool operator()( const K* const& k1, const K* const& k2 ) const
	{
		if      ( k1 == nullptr && k2 == nullptr ) return  true;
		else if ( k1 == nullptr || k2 == nullptr ) return false;

		const K*	a = k1;
		const K*	b = k2;

		for ( ; *a != term && *b != term && *a == *b; ++a, ++b ) ;

		if ( *a == term && *b == term ) return true;
		else				return false;
	}
};

struct dp_eql_pvoid
{
	bool operator()( const void* const& k1, const void* const& k2 ) const	{ return k1 == k2; }
};

// partially specialized dp_eqls
// ----------------------------------------------------------------------------
template <> struct dp_eql<void*>	   : dp_eql_pvoid	{ };
template <> struct dp_eql<const void*>     : dp_eql_pvoid	{ };

template <typename K> struct dp_eql<K*>
{
	bool operator()( const K* const& k1, const K* const& k2 ) const
	{
		if      ( k1 == nullptr && k2 == nullptr ) return true;
		else if ( k1 == nullptr || k2 == nullptr ) return false;
		else if ( *k1 == *k2 )			   return true;
		else					   return false;
	}
};
template <typename K> struct dp_eql<K[]> : public dp_eql_varray<K>	{ };
template <typename K, size_t N> struct dp_eql<K[N]>
{
	bool operator()( const K* const& k1, const K* const& k2 ) const
	{
		if      ( k1 == nullptr && k2 == nullptr ) return true;
		else if ( k1 == nullptr || k2 == nullptr ) return false;

		size_t  i;

		for ( i = 0; i < N; ++i ) if ( k1[ i ] != k2[ i ] ) break;
		return i == N;
	}
};

// dp_cmp
// ----------------------------------------------------------------------------
template <typename K, size_t N = 0> struct dp_cmp
{
	int operator()( K const& k1, K const& k2 ) const
	{
		if      ( k1 <  k2 ) return -1;
		else if ( k1 == k2 ) return  0;
		else		     return  1;
	}
};

// ----------------------------------------------------------------------------
template <typename K> struct dp_cmp_varray
{
	const K	 term = K();

	int operator()( const K* const& k1, const K* const& k2 ) const
	{
		if ( k1 == nullptr || k2 == nullptr ) {
			if      ( k1 != nullptr ) return  1;
			else if ( k2 != nullptr ) return -1;
			else			  return  0;
		} else	{
			const K*	a;
			const K*	b;

			for ( a = k1, b = k2; *a != term && *b != term && *a == *b; ++a, ++b ) ;

			if ( *a == term && *b == term )	return  0;
			else if ( *a == term )		return -1;
			else if ( *b == term )		return  1;
			else if ( *a < *b )		return -1;
			else				return  1;
		}
	}
};

struct dp_cmp_pvoid
{
	int operator()( const void* const& k1, const void* const& k2 ) const
	{
		if      ( k1 <  k2 ) return -1;
		else if ( k1 == k2 ) return  0;
		else		     return  1;
	}
};

// partially specialized dp_cmps
// ----------------------------------------------------------------------------
template <> struct dp_cmp<void*>	   : public dp_cmp_pvoid	{ };
template <> struct dp_cmp<const void*>     : public dp_cmp_pvoid	{ };

template <typename K> struct dp_cmp<K*>
{
	int operator()( const K* const& k1, const K* const& k2 ) const
	{
		if ( k1 == nullptr || k2 == nullptr ) {
			if      ( k1 != nullptr ) return  1;
			else if ( k2 != nullptr ) return -1;
			else			  return  0;
		} else	{
			if      ( *k1 == *k2 )	  return  0;
			else if ( *k1 <  *k2 )	  return -1;
			else			  return  1;
		}
	}
};
template <typename K>      struct dp_cmp<K[]>  : public dp_cmp_varray<K>	{ };
template <typename K, size_t N> struct dp_cmp<K[N]>
{
	int operator()( const K* const& k1, const K* const& k2 ) const
	{
		if ( k1 == nullptr || k2 == nullptr ) {
			if      ( k1 != nullptr )       return  1;
			else if ( k2 != nullptr )       return -1;
			else			    return  0;
		} else {
			int     r = 0;

			for ( size_t i = 0; i < N; ++i ) {
				if ( k1[ i ] < k2[ i ] )	{ r = -1; break; }
				if ( k2[ i ] < k1[ i ] )	{ r =  1; break; }
			}
			return r;
		}
	}
};

// dp_less
// ----------------------------------------------------------------------------
template <typename K, size_t N = 0> struct dp_less
{
	bool operator()( K const& k1, K const& k2 ) const { return k1 < k2; }
};

// specialized dp_lesss
// ----------------------------------------------------------------------------
template <> struct dp_less<void*>
{
	bool operator()( const void* const& k1, const void* const& k2 ) const { return k1 < k2; }
};
template <> struct dp_less<const void*>
{
	bool operator()( const void* const& k1, const void* const& k2 ) const { return k1 < k2; }
};

template <typename K> struct dp_less<K*>
{
	bool operator()( const K* const& k1, const K* const& k2 ) const
	{
		return ( k1 && k2 ) ? *k1 < *k2 : k2 != nullptr;
	}
};

template <typename K> struct dp_less<K[]>
{
	bool operator()( const K* const& k1, const K* const& k2 ) const
	{
		if ( k1 && k2 )
		{
			size_t  i;

			for ( i = 0; k1[ i ] != K() && k2[ i ] != K(); ++i ) {
				if ( k1[ i ] < k2[ i ] ) return true;
				if ( k1[ i ] > k2[ i ] ) return false;
			}
			return k2[ i ] != K();
		} else {
			return k2 != nullptr;
		}
	}
};

template <typename K, size_t N> struct dp_less<K[N]>
{
	bool operator()( const K* const& k1, const K* const& k2 ) const
	{
		if ( k1 && k2 ) {
			size_t  i;

			for ( i = 0; i < N; ++i ) {
				if ( k1[ i ] < k2[ i ] ) return true;
				if ( k1[ i ] > k2[ i ] ) return false;
			}
			return i < N;
		} else {
			return k2 != nullptr;
		}
	}
};

//
// Note: If you use a special memory manager and the element pointer of container
//      will be created/deleted not by 'new' or 'delete', please make specialized
//      dp_dup/dp_del appropriately.
//
// dp_dup
// ----------------------------------------------------------------------------
template <typename K, size_t N = 0> struct dp_dup	{ /* not implemented */ };

template <typename K> struct dp_dup<K*>
{
	K* operator()( K const* const& p ) { return p ? new K( *p ) : nullptr; }
};

template <typename K> struct dp_dup<K[]>
{
	typedef std::remove_const_t<K>  V;

	V* operator()( const V* const& p )
	{
		V*      r;

		if ( p ) {
			unsigned	n = 0;

			for ( const V* q = p; *q != V(); ++q ) ++n;

			r = new V[ n + 1 ];
			for ( unsigned i = 0; i < n; ++i ) r[ i ] = p[ i ];
			r[ n ] = V();
		} else {
			r = nullptr;
		}
		return r;
	}
};

template <typename K, size_t N> struct dp_dup<K[N]>
{
	typedef std::remove_const_t<K>  V;

	V* operator()( const V* const& p )
	{
		V*      r;

		if ( p ) {
			r = new V[ N ];
			for ( unsigned i = 0; i < N; ++i ) r[ i ] = p[ i ];
		} else {
			r = nullptr;
		}
		return r;
	}
};

// dp_del
// ----------------------------------------------------------------------------
template <typename K, size_t N = 0> struct dp_del	{ /* not implemented */ };

template <typename K> struct dp_del<K*>
{
	void operator()( K const* const& p )	{ if ( p ) delete p; }
};

template <typename K> struct dp_del<K[]>
{
	void operator()( K const* const& p )	{ if ( p ) delete [] p; }
};

template <typename K, size_t N> struct dp_del<K[N]>
{
	void operator()( K const* const& p )	{ if ( p ) delete [] p; }
};


//
// Deep pointer for STL and STL like value-based container element
//
// Objectives:
//      - support to use STL and STL like value-based container with native pointers and arrays
//      - maximize pointer and array usablility as of non-pointer value for STL containers
//	- minimize overhead of performance and usability 
//
// ============================================================================

// deep pointer base class
// -----------------------------------------------------------------------------
template <typename T>
class dp_base
{
    protected:
	typename value_p<T>::t		m_p;

    public:
	typedef typename value_p<T>::t	value_ptr;	// T*, T[], T[N] => T*
	typedef typename const_p<T>::t	const_ptr;	// T*, T[], T[N] => const T*

	// constructor/assignment operator/destructor
	//
	// Note: Default constructors/assignemt operators are not overridden by template version of them,
	//       and it is necessary to define explicitly.
	//
	dp_base()				: m_p( nullptr ) { }
	dp_base( dp_base const& p ) 		: m_p( p.get()) { }
	dp_base( dp_base&& p )      		: m_p( p.get()) { }
	dp_base& operator=( dp_base const& p )	{ m_p = p.get(); return *this; }
	dp_base& operator=( dp_base&& p )	{ m_p = p.get(); return *this; }   
	~dp_base()				{ }

	dp_base( value_ptr const& p ) : m_p( p ) { }
	dp_base( value_ptr&& p )
	{
		if ( m_p != p ) {
			m_p = p;
			p = nullptr;
		}
	}

	// utilities
	value_ptr& get()		{ return m_p; }
	value_ptr const& get() const	{ return m_p; }
	value_ptr release()		// release pointer with ownership
	{
		value_ptr	r = m_p;

		m_p = nullptr;
		return r;
	}

	void set( value_ptr const& p )
	{
		m_p = p;
	}
	void set( value_ptr&& p )
	{
		if ( m_p != p ) {
			m_p = p;
			p = nullptr;
		}
	}

	// utilities - for deep_ptr only
	void copy( const_ptr const& p )
	{
		if ( m_p != p )	{
			if ( m_p ) dp_del<T>()( m_p );
			m_p = dp_dup<T>()( p );	
		}
	}
	void move( value_ptr&& p )
	{
		if ( m_p != p )	{
			if ( m_p ) dp_del<T>()( m_p );
			m_p = p;
		}
	}

	// comparison operators
	template <typename U> bool operator==( dp_base<U> const& r ) const
	{
		auto q = r.get();
 		return this->m_p && q && dp_eql<T>()( m_p, q ) || !m_p && !q;
	}
	template <typename P> bool operator!=( P const& r ) const { return !( *this == r ); }

	template <typename U> bool operator< ( dp_base<U> const& r ) const
	{
		auto q = r.get();
		return this->m_p && q && dp_less<T>()( m_p, q ) || !m_p && q;
	}
	template <typename P> bool operator<=( P const& r ) const { return !( r < *this ); }
	template <typename P> bool operator> ( P const& r ) const { return r < *this; }
	template <typename P> bool operator>=( P const& r ) const { return !( *this < r ); }
};

// deep pointer
//
//    Note: The universal reference version of constructor/assignment operator
//	    will works also as default one.
//
// -----------------------------------------------------------------------------
template <typename T, size_t N = 0> class deep_ptr	{ };       

template <typename T>
class deep_ptr<T*> : public dp_base<T*>
{       
    public:
	typedef typename dp_base<T*>::value_ptr	value_ptr;
	typedef typename dp_base<T*>::const_ptr	const_ptr;

	deep_ptr() = default;
	//deep_ptr( deep_ptr const& p ) : dp_base<T*>( dp_dup<T*>()( p.get())) { }
	//deep_ptr( deep_ptr&& p )      : dp_base<T*>( p.release()) { }
	//deep_ptr& operator=( deep_ptr const& p ) { this->copy( p.get()); return *this; }
	//deep_ptr& operator=( deep_ptr&& p )      { this->move( p.release()); return *this; }
	//~deep_ptr() { if ( this->m_p ) dp_del<T*>()( this->m_p ); }

	//template <typename U, typename = std::enable_if_t<std::is_assignable<T,U>::value>>
	// deep_ptr( dp_base<U*> const& p ) : dp_base<T*>( dp_dup<T*>()( p.get())) { }
	//template <typename U, typename = std::enable_if_t<std::is_assignable<T,U>::value>>
	// deep_ptr( dp_base<U*>&& p )      : dp_base<T*>( p.release() ) { }
	//template <typename U, typename = std::enable_if_t<std::is_assignable<T,U>::value>>
	// deep_ptr& operator=( dp_base<U*> const& p ) { this->copy( p.get()); return *this; }
	//template <typename U, typename = std::enable_if_t<std::is_assignable<T,U>::value>>
	// deep_ptr& operator=( dp_base<U*>&& p )	   { this->move( p.release()); return *this; }   

	//deep_ptr( value_ptr const& p ) : dp_base<T*>( dp_dup<T*>()( p )) { }
	//deep_ptr( value_ptr&& p )      : dp_base<T*>( p )		   { p = nullptr; }
	//deep_ptr& operator=( value_ptr const& p ) { this->copy( p ); return *this; }
	//deep_ptr& operator=( value_ptr&& p )      { this->move( std::move( p )); p = nullptr; return *this; }

	deep_ptr( deep_ptr const& p ) : dp_base<T*>( dp_dup<T*>()( p.get()))
	{
		fprintf( stderr, "- dcc %p\n", this->m_p );
	}
	deep_ptr( deep_ptr&& p ) : dp_base<T*>( p.release())
	{
		fprintf( stderr, "- dmc %p\n", this->m_p );
	}
	deep_ptr& operator=( deep_ptr const& p )
	{
		fprintf( stderr, "- dca %p <- %p\n", this->m_p, p.get());
		this->copy( p.get());
		return *this;
	}
	deep_ptr& operator=( deep_ptr&& p )
	{
		fprintf( stderr, "- dma %p <- %p\n", this->m_p, p.get());
		this->move( p.release());
		return *this;
	}
	~deep_ptr() { if ( this->m_p ) dp_del<T*>()( this->m_p ); }

	template <typename U, typename = std::enable_if_t<std::is_assignable<T,U>::value>>
	deep_ptr( dp_base<U*> const& p ) : dp_base<T*>( dp_dup<T*>()( p.get()))
	{
		fprintf( stderr, "- dtcc %p\n", this->m_p );
	}
	template <typename U, typename = std::enable_if_t<std::is_assignable<T,U>::value>>
	deep_ptr( dp_base<U*>&& p )      : dp_base<T*>( p.release() )
	{
		fprintf( stderr, "- dtmc %p\n", this->m_p );
	}
	template <typename U, typename = std::enable_if_t<std::is_assignable<T,U>::value>>
	deep_ptr& operator=( dp_base<U*> const& p )
	{
		fprintf( stderr, "- dtca %p <- %p\n", this->m_p, p.get());
		this->copy( p.get());
		return *this;
	}
	template <typename U, typename = std::enable_if_t<std::is_assignable<T,U>::value>>
	deep_ptr& operator=( dp_base<U*>&& p )
	{
		fprintf( stderr, "- dtma %p <- %p\n", this->m_p, p.get());
		this->move( p.release());
		return *this;
	}   

	deep_ptr( const_ptr const& p ) : dp_base<T*>( dp_dup<T*>()( p ))
	{
		fprintf( stderr, "- dpcc %p\n", this->m_p );
	}
	deep_ptr( value_ptr&& p )      : dp_base<T*>( p )
	{
		p = nullptr;
		fprintf( stderr, "- dpmc %p\n", this->m_p );
	}
	deep_ptr& operator=( const_ptr const& p )
	{
		fprintf( stderr, "- dpca %p <- %p\n", this->m_p, p );
		this->copy( p );
		return *this;
	}
	deep_ptr& operator=( value_ptr&& p )
	{
		fprintf( stderr, "- dpma %p <- %p\n", this->m_p, p );
		this->move( std::move( p ));
		p = nullptr;
		return *this;
	}

	operator value_ptr()		{ return this->m_p; }
	operator const_ptr() const	{ return this->m_p; }

	T& operator*()			{ return *this->m_p; }
	const T& operator*() const	{ return *this->m_p; }
	T* operator->()			{ return this->m_p; }
	const T* operator->() const	{ return this->m_p; }
};

template <typename T>
class deep_ptr<T[]> : public dp_base<T[]>
{       
    public:
	typedef typename dp_base<T[]>::value_ptr	value_ptr;
	typedef typename dp_base<T[]>::const_ptr	const_ptr;

	deep_ptr() = default;
	deep_ptr( deep_ptr const& p ) : dp_base<T[]>( dp_dup<T[]>()( p.get())) { }
	deep_ptr( deep_ptr&& p )      : dp_base<T[]>( p.release()) { }
	deep_ptr& operator=( deep_ptr const& p ) { this->copy( p.get()); return *this; }
	deep_ptr& operator=( deep_ptr&& p )      { this->move( p.release()); return *this; }
	~deep_ptr() { if ( this->m_p ) dp_del<T[]>()( this->m_p ); }

	template <typename U, typename = std::enable_if_t<std::is_assignable<T,U>::value>>
	 deep_ptr( dp_base<U*> const& p ) : dp_base<T[]>( dp_dup<T[]>()( p.get())) { }
	template <typename U, typename = std::enable_if_t<std::is_assignable<T,U>::value>>
	 deep_ptr( dp_base<U*>&& p )      : dp_base<T[]>( p.release() ) { }
	template <typename U, typename = std::enable_if_t<std::is_assignable<T,U>::value>>
	 deep_ptr& operator=( dp_base<U*> const& p ) { this->copy( p.get()); return *this; }
	template <typename U, typename = std::enable_if_t<std::is_assignable<T,U>::value>>
	 deep_ptr& operator=( dp_base<U*>&& p )	 { this->move( p.release()); return *this; }   

	deep_ptr( const_ptr const& p ) : dp_base<T[]>( dp_dup<T[]>()( p )) { }
	deep_ptr( value_ptr&& p )      : dp_base<T[]>( p ) { p = nullptr; }
	deep_ptr& operator=( const_ptr const& p ) { this->copy( p ); return *this; }
	deep_ptr& operator=( value_ptr&& p )      { this->move( std::move( p )); p = nullptr; return *this; }

	operator value_ptr()			{ return this->m_p; }
	operator const_ptr() const		{ return this->m_p; }

	T& operator[]( size_t n )		{ return this->m_p[ n ]; }
	const T& operator[]( size_t n ) const	{ return this->m_p[ n ]; }
};

template <typename T, size_t N>
class deep_ptr<T[N]> : public dp_base<T[N]>
{       
    public:
	typedef typename dp_base<T[N]>::value_ptr	value_ptr;
	typedef typename dp_base<T[N]>::const_ptr	const_ptr;

	deep_ptr() = default;
	deep_ptr( deep_ptr const& p ) : dp_base<T[N]>( dp_dup<T[N]>()( p.get())) { }
	deep_ptr( deep_ptr&& p )      : dp_base<T[N]>( p.release()) { }
	deep_ptr& operator=( deep_ptr const& p ) { this->copy( p.get()); return *this; }
	deep_ptr& operator=( deep_ptr&& p )      { this->move( p.release()); return *this; }
	~deep_ptr() { if ( this->m_p ) dp_del<T[N]>()( this->m_p ); }

	template <typename U, typename = std::enable_if_t<std::is_assignable<T,U>::value>>
	 deep_ptr( dp_base<U[N]> const& p ) : dp_base<T[N]>( dp_dup<T[N]>()( p.get())) { }
	template <typename U, typename = std::enable_if_t<std::is_assignable<T,U>::value>>
	 deep_ptr( dp_base<U[N]>&& p )      : dp_base<T[N]>( p.release() ) { }
	template <typename U, typename = std::enable_if_t<std::is_assignable<T,U>::value>>
	 deep_ptr& operator=( dp_base<U[N]> const& p ) { this->copy( p.get()); return *this; }
	template <typename U, typename = std::enable_if_t<std::is_assignable<T,U>::value>>
	 deep_ptr& operator=( dp_base<U[N]>&& p )	 { this->move( p.release()); return *this; }   

	deep_ptr( const_ptr const& p ) : dp_base<T[N]>( dp_dup<T[N]>()( p )) { }
	deep_ptr( value_ptr&& p )      : dp_base<T[N]>( p ) { p = nullptr; }
	deep_ptr& operator=( const_ptr const& p ) { this->copy( p ); return *this; }
	deep_ptr& operator=( value_ptr&& p )      { this->move( std::move( p )); p = nullptr; return *this; }

	operator value_ptr()			{ return this->m_p; }
	operator const_ptr() const		{ return this->m_p; }

	T& operator[]( size_t n )		{ return this->m_p[ n ]; }
	const T& operator[]( size_t n ) const	{ return this->m_p[ n ]; }
};

//
// deep reference pointer
//
//	using no deep-copy, no deep-move, but refer with deep comparison
// -----------------------------------------------------------------------------
template <typename T, size_t N = 0> class dref_ptr	{ };       

template <typename T>
class dref_ptr<T*> : public dp_base<T*>
{       
    public:
	typedef typename dp_base<T*>::value_ptr		value_ptr;

	dref_ptr() = default;
	dref_ptr( dref_ptr const& ) = default;
	dref_ptr( dref_ptr&& p ) = default;
	dref_ptr& operator=( dref_ptr const& ) = default;
	dref_ptr& operator=( dref_ptr&& p ) = default;
	~dref_ptr() = default;

	dref_ptr( dp_base<T*> const& p ) : dp_base<T*>( const_cast<value_ptr>( p.get())) { }
	dref_ptr& operator=( dp_base<T*> const& p ) { this->set( p.get()); return *this; }

	dref_ptr( value_ptr const& p ) : dp_base<T*>( p ) { }
	dref_ptr& operator=( value_ptr const& p ) { this->set( p ); return *this; }

	operator value_ptr&()		  { return this->m_p; }
	operator const value_ptr&() const { return this->m_p; }

	const T& operator*() const	{ return *this->m_p; }
	const T* operator->() const	{ return this->m_p; }
};

template <typename T>
class dref_ptr<T[]> : public dp_base<T[]>
{       
    public:
	typedef typename dp_base<T[]>::value_ptr	value_ptr;

	dref_ptr() = default;
	dref_ptr( dref_ptr const& ) = default;
	dref_ptr( dref_ptr&& ) = default;
	dref_ptr& operator=( dref_ptr const& ) = default;
	dref_ptr& operator=( dref_ptr&& ) = default;
	~dref_ptr() = default;

	template <typename U, typename = std::enable_if_t<std::is_assignable<T,U>::value>>
	 dref_ptr( dp_base<U[]> const& p ) : dp_base<T[]>( p.get()) { }
	template <typename U, typename = std::enable_if_t<std::is_assignable<T,U>::value>>
	 dref_ptr& operator=( dp_base<U[]> const& p ) { this->set( p.get()); return *this; }

	dref_ptr( value_ptr const& p ) : dp_base<T[]>( p ) { }
	dref_ptr& operator=( value_ptr const& p ) { this->set( p ); return *this; }

	operator value_ptr&()             { return this->m_p; }
	operator const value_ptr&() const { return this->m_p; }

	const T& operator[]( size_t n ) const	{ return this->m_p[ n ]; }
};

template <typename T, size_t N>
class dref_ptr<T[N]> : public dp_base<T[N]>
{       
    public:
	typedef typename dp_base<T[N]>::value_ptr	value_ptr;

	dref_ptr() = default;
	dref_ptr( dref_ptr const& ) = default;
	dref_ptr( dref_ptr&& ) = default;
	dref_ptr& operator=( dref_ptr const& ) = default;
	dref_ptr& operator=( dref_ptr&& ) = default;
	~dref_ptr() = default;

	template <typename U, typename = std::enable_if_t<std::is_assignable<T,U>::value>>
	 dref_ptr( dp_base<U[N]> const& p ) : dp_base<T[N]>( p.get()) { }
	template <typename U, typename = std::enable_if_t<std::is_assignable<T,U>::value>>
	 dref_ptr& operator=( dp_base<U[N]> const& p ) { this->set( p.get()); return *this; }

	dref_ptr( value_ptr const& p ) : dp_base<T[N]>( p ) { }
	dref_ptr& operator=( value_ptr const& p ) { this->set( p ); return *this; }

	operator value_ptr&()             { return this->m_p; }
	operator const value_ptr&() const { return this->m_p; }

	const T& operator[]( size_t n ) const	{ return this->m_p[ n ]; }
};

//
// Type deducers
//
//	dp_pointer<T>:		deep_ptr<T*>/deep_ptr<T[]> => T*
//	dp_const_pointer<T>:	deep_ptr<T*>/deep_ptr<T[]> => const T*
//	dp_type<T>:		deep_ptr<T*> => T*/deep_ptr<T[]> => T[]
//	dp_const_type<T>:	deep_ptr<T*> => const T*/deep_ptr<T[]> => const T[]
//	dp_const<T>:		deep_ptr<T*> => deep_ptr<const T*>/deep_ptr<T[]> => deep_ptr<const T[]>
//
//	ex. dp_pointer<deep_ptr<T*>> -> T*, dp_pointer<dref_ptr<T[8]>> -> T*
// -----------------------------------------------------------------------------
template <typename T> struct dp_ptr_aux { typedef T t; };
template <typename T> struct dp_ptr_aux<deep_ptr<T>> { typedef typename value_p<T>::t t; };
template <typename T> struct dp_ptr_aux<dref_ptr<T>> { typedef typename value_p<T>::t t; };
template <typename T> struct dp_cpt_aux { typedef const T t; };
template <typename T> struct dp_cpt_aux<deep_ptr<T>> { typedef typename const_p<T>::t t; };
template <typename T> struct dp_cpt_aux<dref_ptr<T>> { typedef typename const_p<T>::t t; };
template <typename T> struct dp_typ_aux { typedef T t; };
template <typename T> struct dp_typ_aux<deep_ptr<T>> { typedef T t; };
template <typename T> struct dp_typ_aux<dref_ptr<T>> { typedef T t; };
template <typename T> struct dp_cty_aux { typedef const T t; };
template <typename T> struct dp_cty_aux<deep_ptr<T>> { typedef typename const_a<T>::t t; };
template <typename T> struct dp_cty_aux<dref_ptr<T>> { typedef typename const_a<T>::t t; };
template <typename T> struct dp_cdp_aux { typedef const T t; };
template <typename T> struct dp_cdp_aux<deep_ptr<T>> { typedef deep_ptr<typename const_a<T>::t> t; };
template <typename T> struct dp_cdp_aux<dref_ptr<T>> { typedef dref_ptr<typename const_a<T>::t> t; };

template <typename T> using dp_pointer		= typename dp_ptr_aux<T>::t;
template <typename T> using dp_const_pointer	= typename dp_cpt_aux<T>::t;
template <typename T> using dp_type		= typename dp_typ_aux<T>::t;
template <typename T> using dp_const_type	= typename dp_cty_aux<T>::t;
template <typename T> using dp_const		= typename dp_cdp_aux<T>::t;

#ifndef NO_NAMESPACE
}
#endif

#endif // __DPTL_DPADAPTER_H

