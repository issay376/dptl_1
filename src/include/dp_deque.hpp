//
// dp_deque.hpp - sample customization of std::deque for deep pointer
//
//
//

#ifndef __DPTL_DP_DEQUE_HPP
#define __DPTL_DP_DEQUE_HPP

#include <deque>
#include <dptl/deep_ptr.hpp>
		
#ifndef DPTL_NAMESPACE
#define DPTL_NAMESPACE dptl
#endif  
		
#ifndef NO_NAMESPACE
namespace DPTL_NAMESPACE {
#endif

// deque for deep_ptr
// -----------------------------------------------------------------------------
template <typename T>
class __dp_deque : public std::deque<T>
{
	typedef dp_type<T>		VT;		// T=deep_ptr<P*> => VT=P*, T=deep_ptr<V[]> => VT=V[]
	typedef dp_pointer<T>		VP;		// T=deep_ptr<P*> => VP=P*, T=deep_ptr<V[]> => VP=V*
	typedef std::deque<T>		super;

    public:
	typedef typename super::value_type		value_type;
	typedef typename super::allocator_type		allocator_type;
	typedef typename super::reference		reference;
	typedef typename super::const_reference		const_reference;
	typedef typename super::pointer			pointer;
	typedef typename super::const_pointer		const_pointer;
	typedef typename super::iterator		iterator;
	typedef typename super::const_iterator		const_iterator;
	typedef typename super::reverse_iterator	reverse_iterator;
	typedef typename super::const_reverse_iterator  const_reverse_iterator;
	typedef typename super::difference_type		difference_type;
	typedef typename super::size_type		size_type;

	typedef VP					value_pointer;

#if defined(__cplusplus) && (__cplusplus >= 201402)
	// ---- for c++14
	__dp_deque() = default;
	explicit __dp_deque( const allocator_type& alloc ) : super( alloc ) { }
	explicit __dp_deque( size_type n, const allocator_type& alloc = allocator_type()) : super( n, alloc ) { }
	__dp_deque( size_type n, const value_pointer& v, const allocator_type& alloc = allocator_type()) : super( alloc )
	{
		assign( n, v );
	}

	template <class II>
	__dp_deque( II first, II last, const allocator_type& alloc = allocator_type()) : super( first, last, alloc ) { }

	__dp_deque( const __dp_deque& ) = default;
	__dp_deque( const __dp_deque& l, const allocator_type& alloc ) : super( l, alloc ) { }
	__dp_deque( __dp_deque&& ) = default;
	__dp_deque( __dp_deque&& l, const allocator_type& alloc ) : super( std::move( l ), alloc ) { }

	__dp_deque( std::initializer_list<value_pointer> il,
		 const allocator_type& alloc = allocator_type()) : super( alloc )
	{
		for ( auto&& p : il ) super::push_back( std::forward<decltype( p )>( p ));
	}

#elif defined(__cplusplus) && (__cplusplus >= 201103)
	// ---- for c++11
	explicit __dp_deque( const allocator_type& alloc = allocator_type()) : super( alloc ) { }
	explicit __dp_deque( size_type n ) : super( n ) { }
	__dp_deque( size_type n, const value_pointer& v, const allocator_type& alloc = allocator_type()) : super( alloc )
	{
		assign( n, v );
	}

	template <class II>
	__dp_deque( II first, II last, const allocator_type& alloc = allocator_type()) : super( first, last, alloc ) { }

	__dp_deque( const __dp_deque& ) = default;
	__dp_deque( const __dp_deque& l, const allocator_type& alloc ) : super( l, alloc ) { }
	__dp_deque( __dp_deque&& ) = default;
	__dp_deque( __dp_deque&& l, const allocator_type& alloc ) : super( std::move( l ), alloc ) { }

	__dp_deque( std::initializer_list<value_pointer> il,
		 const allocator_type& alloc = allocator_type()) : super( alloc )
	{
		for ( auto&& p : il ) super::push_back( std::forward<decltype( p )>( p ));
	}
#else
#error "c++11 up required"
#endif

	__dp_deque& operator=( const __dp_deque& ) = default;
	__dp_deque& operator=( __dp_deque&& ) = default;
	__dp_deque& operator=( std::initializer_list<value_pointer> il )
	{
		super::clear();
		for ( auto&& p : il ) super::push_back( std::forward<decltype( p )>( p ));
		return *this;
	}

	~__dp_deque() = default;

	// wrapper for appropriate casting of raw pointer
	template <class II> void assign( II fst, II lst )
	{
		super::assign( fst, lst );
	}
	void assign( size_type n, const value_pointer& v )
	{
		super::assign( n, reinterpret_cast<const T&>( v ));
	}
	void assign( std::initializer_list<value_pointer> il )
	{
		super::clear();
		for ( auto&& p : il ) super::push_back( std::forward<decltype( p )>( p ));
	}

	template <typename... Args> iterator emplace( const_iterator pos, Args&&... args )
	{
		return super::emplace( pos, std::forward<Args>( args )... );
	}
	template <typename... Args> void emplace_back( Args&&... args )
	{
		super::emplace_back( std::forward<Args>( args )... );
	}
	template <typename... Args> void emplace_front( Args&&... args )
	{
		super::emplace_front( std::forward<Args>( args )... );
	}

	iterator insert( const_iterator i, const value_type& p )
	{
		return super::insert( i, p );
	}
	iterator insert( const_iterator i, value_type&& p )
	{
		return super::insert( i, std::move( p ));
	}
	iterator insert( const_iterator i, size_type n, const value_pointer& v )
	{
		return super::insert( i, n, reinterpret_cast<const T&>( v ));
	}
	template <class II> iterator insert( const_iterator i, II fst, II lst )
	{
		return super::insert( i, fst, lst );
	}
	iterator insert( const_iterator i, std::initializer_list<value_pointer> il )
	{
		for ( auto&& p = il.end(); p != il.begin(); ) i = insert( i, *--p );
		return reinterpret_cast<iterator&>( i );
	}

	template <typename P> void push_back( P&& p )  { super::push_back( std::forward<P>( p )); }
	template <typename P> void push_front( P&& p ) { super::push_front( std::forward<P>( p )); }

	void resize( size_type n ) { super::resize( n ); }
	void resize( size_type n, const value_pointer& v )
	{
		super::resize( n, reinterpret_cast<const T&>( v ));
	}
};

// alias for deep pointer deque: T=V*/V[]/V[N]
//
//	dp_deque: deque for deep pointer  
//	dr_deque: deque for deep reference pointer
// ----------------------------------------------------------------------------- 
template <typename T> using dp_deque = __dp_deque<deep_ptr<T>>;
template <typename T> using dr_deque = __dp_deque<dref_ptr<T>>;


#ifndef NO_NAMESPACE
}       
#endif
	
#endif // __DPTL_DP_DEQUE_HPP

