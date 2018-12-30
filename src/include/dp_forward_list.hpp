//
// dp_forward_list.hpp - sample customization of std::forward_list for deep pointer
//
//      2018/12/30, version 0.1
//
//      © Kazunari Saitoh, 2018-2019.
//      Distributed under the Boost Software License, Version 1.0.
//      (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//

#ifndef __DPTL_DP_FORWARD_LIST_HPP
#define __DPTL_DP_FORWARD_LIST_HPP

#include <forward_list>
#include <dptl/deep_ptr.hpp>
		
#ifndef DPTL_NAMESPACE
#define DPTL_NAMESPACE dptl
#endif  
		
#ifndef NO_NAMESPACE
namespace DPTL_NAMESPACE {
#endif

// policy based pointer set
// -----------------------------------------------------------------------------
template <typename T>
class __dp_forward_list : public std::forward_list<T>
{
	typedef dp_type<T>				VT;
	typedef dp_pointer<T>				VP;
	typedef std::forward_list<T>			super;

    public:
	typedef typename super::value_type		value_type;
	typedef typename super::allocator_type		allocator_type;
	typedef typename super::reference		reference;
	typedef typename super::const_reference		const_reference;
	typedef typename super::pointer			pointer;
	typedef typename super::const_pointer		const_pointer;
	typedef typename super::iterator		iterator;
	typedef typename super::const_iterator		const_iterator;
	typedef typename super::difference_type		difference_type;
	typedef typename super::size_type		size_type;

	typedef	VP					value_pointer;

#if defined(__cplusplus) && (__cplusplus >= 201402)
	__dp_forward_list() = default;
	explicit __dp_forward_list( const allocator_type& alloc ) : super( alloc ) { }
	explicit __dp_forward_list( size_type n, const allocator_type& alloc = allocator_type()) : super( n, alloc ) { }
	__dp_forward_list( size_type n, const value_pointer& v, const allocator_type& alloc = allocator_type()) : super( alloc )
	{
		assign( n, v );
	}

	template <class II>
	__dp_forward_list( II first, II last, const allocator_type& alloc = allocator_type()) : super( first, last, alloc ) { }

	__dp_forward_list( const __dp_forward_list& ) = default;
	__dp_forward_list( const __dp_forward_list& l, const allocator_type& alloc ) : super( l, alloc ) { }
	__dp_forward_list( __dp_forward_list&& ) = default;
	__dp_forward_list( __dp_forward_list&& l, const allocator_type& alloc ) : super( std::move( l ), alloc ) { }

	__dp_forward_list( std::initializer_list<value_pointer> il,
			const allocator_type& alloc = allocator_type()) : super( alloc )
	{
		auto	p = il.end();

		while ( p != il.begin()) super::push_front( *--p );
	}

#elif defined(__cplusplus) && (__cplusplus >= 201103)
	// ---- for c++11
	explicit __dp_forward_list( const allocator_type& alloc = allocator_type()) : super( alloc ) { }
	explicit __dp_forward_list( size_type n ) : super( n ) { }
	__dp_forward_list( size_type n, const value_pointer& v, const allocator_type& alloc = allocator_type()) : super( alloc )
	{
		assign( n, v );
	}

	template <class II>
	__dp_forward_list( II first, II last, const allocator_type& alloc = allocator_type()) : super( first, last, alloc ) { }

	__dp_forward_list( const __dp_forward_list& ) = default;
	__dp_forward_list( const __dp_forward_list& l, const allocator_type& alloc ) : super( l, alloc ) { }
	__dp_forward_list( __dp_forward_list&& ) = default;
	__dp_forward_list( __dp_forward_list&& l, const allocator_type& alloc ) : super( l, alloc ) { }

	__dp_forward_list( std::initializer_list<value_pointer> il,
			const allocator_type& alloc = allocator_type()) : super( alloc )
	{
		auto	p = il.end();

		while ( p != il.begin()) super::push_front( *--p );
	}
#else
#error "c++11 up required"
#endif

	__dp_forward_list& operator=( const __dp_forward_list& ) = default;
	__dp_forward_list& operator=( __dp_forward_list&& ) = default;
	__dp_forward_list& operator=( std::initializer_list<value_pointer> il )
	{
		auto	p = il.end();

		super::clear();
		while ( p != il.begin()) super::push_front( *--p );
		return *this;
	}

	~__dp_forward_list() = default;

	// wrapper for appropriate casting of raw pointer
	template <class II> void assign( II fst, II lst )
	{
		return super::assign( fst, lst );
	}
	void assign( size_type n, const value_pointer& v )
	{
		return super::assign( n, reinterpret_cast<const T&>( v ));
	}
	void assign( std::initializer_list<value_pointer> il )
	{
		auto	p = il.end();

		super::clear();
		while ( p != il.begin()) super::push_front( *--p );
	}

	iterator insert_after( const_iterator i, const value_type& p )
	{
		return super::insert_after( i, p );
	}
	iterator insert_after( const_iterator i, value_type&& p )
	{
		return super::insert_after( i, std::move( p ));
	}
	iterator insert_after( const_iterator i, size_type n, const value_pointer& v )
	{
		return super::insert_after( i, n, reinterpret_cast<const T&>( v ));
	}
	template <class II> iterator insert_after( const_iterator i, II fst, II lst )
	{
		return super::insert_after( i, fst, lst );
	}
	iterator insert_after( const_iterator i, std::initializer_list<value_pointer> il )
	{
		auto		p = il.end();
		iterator	r;

		if ( p == il.begin()) {
			r = reinterpret_cast<iterator&>( i );
		} else {
			r = insert_after( i, *--p );
			while ( p != il.begin()) super::insert_after( i, *--p );
		}
		return r;
	}

	template <typename... Args> iterator emplace_after( const_iterator pos, Args&&... args )
	{
		return super::emplace_after( pos, std::forward<Args>( args )... );
	}
	template <typename... Args> void emplace_front( Args&&... args )
	{
		super::emplace_front( std::forward<Args>( args )... );
	}

	// wrapper for appropriate casting of raw pointer avoiding unnecessary duplication
	void remove( const value_pointer& v )
	{
		super::remove( reinterpret_cast<const T&>( v ));
	}

	void resize( size_type n ) { super::resize( n ); }
	void resize( size_type n, const value_pointer& v )
	{
		super::resize( n, reinterpret_cast<const T&>( v ));
	}
};
		
// alias for deep pointer forward_list: T=V*/V[]/V[N]
//
//	dp_forward_list: forward_list for deep pointer
//	dr_forward_list: forward_list for deep reference pointer
// -----------------------------------------------------------------------------
template <typename T> using dp_forward_list = __dp_forward_list<deep_ptr<T>>;
template <typename T> using dr_forward_list = __dp_forward_list<dref_ptr<T>>;

#ifndef NO_NAMESPACE
}       
#endif
	
#endif // __DPTL_DP_FORWARD_LIST_HPP

