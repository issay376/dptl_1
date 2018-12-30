//
// dp_set.hpp - sample customization of std::set/multiset for deep pointer
//
//      2018/12/30, version 0.1
//
//      © Kazunari Saitoh, 2018-2019.
//      Distributed under the Boost Software License, Version 1.0.
//      (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//

#ifndef __DPTL_DP_SET_HPP
#define __DPTL_DP_SET_HPP

#include <set>
#include <dptl/deep_ptr.hpp>
                
#ifndef DPTL_NAMESPACE
#define DPTL_NAMESPACE dptl
#endif  
                
#ifndef NO_NAMESPACE    
namespace DPTL_NAMESPACE {
#endif          

//                
// policy based pointer set 
// -----------------------------------------------------------------------------
template <typename K> 
class __dp_set : public std::set<dp_const<K>>
{
	typedef dp_const<K>				KD;
	typedef dp_const_type<K>			KT;
	typedef dp_const_pointer<K>			KP;
	typedef std::set<KD>				super;

    public:
	typedef typename super::key_type		key_type;	
	typedef typename super::value_type		value_type;
	typedef typename super::key_compare		key_compare;	
	typedef typename super::value_compare		value_compare;
	typedef typename super::allocator_type		allocator_type;
	typedef typename super::reference		reference;
	typedef typename super::const_reference		const_reference;
	typedef typename super::pointer			pointer;
	typedef typename super::const_pointer		const_pointer;
	typedef typename super::iterator		iterator;
	typedef typename super::const_iterator		const_iterator;
	typedef typename super::reverse_iterator	reverse_iterator;
	typedef typename super::const_reverse_iterator	const_reverse_iterator;
	typedef typename super::difference_type		difference_type;
	typedef typename super::size_type		size_type;

	typedef KP					key_pointer;
	typedef KP					value_pointer;

#if defined(__cplusplus) && (__cplusplus >= 201402)
	// ---- for c++14
	__dp_set() = default;
	explicit __dp_set( const key_compare& comp,
		         const allocator_type& alloc = allocator_type()) : super( comp, alloc ) { }
	explicit __dp_set( const allocator_type& alloc ) : super( alloc ) { }

	template <class II>
	__dp_set( II first, II last, const key_compare& comp,
		const allocator_type& alloc = allocator_type()) : super( first, last, comp, alloc ) { }
	template <class II>
	__dp_set( II first, II last, const allocator_type& alloc = allocator_type()) : super( first, last, alloc ) { }

	__dp_set( const __dp_set& ) = default;
	__dp_set( const __dp_set& s, const allocator_type& alloc ) : super( s, alloc ) { }  
	__dp_set( __dp_set&& ) = default;
	__dp_set( __dp_set&& s, const allocator_type& alloc ) : super( std::move( s ), alloc ) { }  

	__dp_set( std::initializer_list<value_pointer> il,
		 const key_compare& comp,
		 const allocator_type& alloc = allocator_type()) : super( comp, alloc )
	{
		for ( auto&& p : il ) super::emplace( std::forward<decltype( p )>( p ));
	}
	__dp_set( std::initializer_list<value_pointer> il,
		 const allocator_type& alloc = allocator_type()) : super( alloc )
	{
		for ( auto&& p : il ) super::emplace( std::forward<decltype( p )>( p ));
	}

#elif defined(__cplusplus) && (__cplusplus >= 201103)
	// ---- for c++11
	explicit __dp_set( const key_compare& comp = key_compare(),
		         const allocator_type& alloc = allocator_type()) : super( comp, alloc ) { }
	explicit __dp_set( const allocator_type& alloc ) : super( alloc ) { }

	template <class II>
	__dp_set( II first, II last, const key_compare& comp = key_compare(),
		const allocator_type& alloc = allocator_type()) : super( first, last, comp, alloc ) { }

	__dp_set( const __dp_set& ) = default;
	__dp_set( const __dp_set& s, const allocator_type& alloc ) : super( s, alloc ) { }  
	__dp_set( __dp_set&& ) = default;
	__dp_set( __dp_set&& s, const allocator_type& alloc ) : super( s, alloc ) { }  

	__dp_set( std::initializer_list<value_pointer> il,
		 const key_compare& comp = key_compare(),
		 const allocator_type& alloc = allocator_type()) : super( comp, alloc )
	{
		for ( auto&& p : il ) super::emplace( std::forward<decltype( p )>( p ));
	}
#else
#error "c++11 up required"
#endif

	__dp_set& operator=( const __dp_set& ) = default;
	__dp_set& operator=( __dp_set&& ) = default;
	__dp_set& operator=( std::initializer_list<value_pointer> il )
	{
		super::clear();
		for ( auto&& p : il ) super::emplace( std::forward<decltype( p )>( p ));
		return *this;
	}

	~__dp_set() = default;

	// wrapper for appropriate casting of raw pointer
	template <typename... Args> std::pair<iterator,bool> emplace( Args&&... args )
	{
		return super::emplace( std::forward<Args>( args )... );
	}
	template <typename... Args> iterator emplace_hint( const_iterator pos, Args&&... args )
	{
		return super::emplace_hint( pos, std::forward<Args>( args )... );
	}

	std::pair<iterator, bool> insert( const value_type& p )
	{
		return super::insert( p );
	}
	std::pair<iterator, bool> insert( value_type&& p )
	{
		return super::insert( std::move( p ));
	}
	iterator insert( const_iterator i, const value_type& p )
	{
		return super::insert( i, p );
	}
	iterator insert( const_iterator i, value_type&& p )
	{
		return super::insert( i, std::move( p ));
	}
	template <class II> void insert( II fst, II lst )
	{
		return super::insert( fst, lst );
	}
	void insert( std::initializer_list<value_pointer> il )
	{
		for ( auto&& p : il ) super::emplace( std::forward<decltype( p )>( p ));
	}

	// wrapper for appropriate casting of raw pointer avoiding unnecessary duplication
	iterator find( const key_pointer& k )
	{
		return super::find( reinterpret_cast<const KD&>( k ));
	}
	const_iterator find( const key_pointer& k ) const
	{
		return super::find( reinterpret_cast<const KD&>( k ));
	}

	iterator upper_bound( const key_pointer& k )
	{
		return super::upper_bound( reinterpret_cast<const KD&>( k ));
	}
	const_iterator upper_bound( const key_pointer& k ) const
	{
		return super::upper_bound( reinterpret_cast<const KD&>( k ));
	}

	iterator lower_bound( const key_pointer& k )
	{
		return super::lower_bound( reinterpret_cast<const KD&>( k ));
	}
	const_iterator lower_bound( const key_pointer& k ) const
	{
		return super::lower_bound( reinterpret_cast<const KD&>( k ));
	}

	std::pair<iterator, iterator> equal_range( const key_pointer& k )
	{
		return super::equal_range( reinterpret_cast<const KD&>( k ));
	}
	std::pair<const_iterator, const_iterator> equal_range( const key_pointer& k ) const
	{
		return super::equal_range( reinterpret_cast<const KD&>( k ));
	}

	size_type erase( const key_pointer& k )
	{
		return super::erase( reinterpret_cast<const KD&>( k ));
	}
	void erase( iterator i ) { super::erase( i ); }
	void erase( iterator fst, iterator lst ) { super::erase( fst, lst ); }

	size_type count( const key_pointer& k )
	{
		return super::count( reinterpret_cast<const KD&>( k ));
	}
};


// policy based pointer multiset 
// -----------------------------------------------------------------------------
template <typename K> 
class __dp_multiset : public std::multiset<dp_const<K>>
{
	typedef dp_const<K>				KD;
	typedef dp_const_type<K>			KT;
	typedef dp_const_pointer<K>			KP;
	typedef std::multiset<KD>			super;

    public:
	typedef typename super::key_type		key_type;	
	typedef typename super::value_type		value_type;	
	typedef typename super::key_compare		key_compare;
	typedef typename super::value_compare		value_compare;
	typedef typename super::allocator_type		allocator_type;
	typedef typename super::reference		reference;
	typedef typename super::const_reference		const_reference;
	typedef typename super::pointer			pointer;
	typedef typename super::const_pointer		const_pointer;
	typedef typename super::iterator		iterator;
	typedef typename super::const_iterator		const_iterator;
	typedef typename super::reverse_iterator	reverse_iterator;
	typedef typename super::const_reverse_iterator	const_reverse_iterator;
	typedef typename super::difference_type		difference_type;
	typedef typename super::size_type		size_type;

	typedef KP					key_pointer;
	typedef KP					value_pointer;

#if defined(__cplusplus) && (__cplusplus >= 201402)
	// ---- for c++14
	__dp_multiset() = default;
	explicit __dp_multiset( const key_compare& comp,
		         const allocator_type& alloc = allocator_type()) : super( comp, alloc ) { }
	explicit __dp_multiset( const allocator_type& alloc ) : super( alloc ) { }

	template <class II>
	__dp_multiset( II first, II last, const key_compare& comp,
		const allocator_type& alloc = allocator_type()) : super( first, last, comp, alloc ) { }
	template <class II>
	__dp_multiset( II first, II last, const allocator_type& alloc = allocator_type()) : super( first, last, alloc ) { }

	__dp_multiset( const __dp_multiset& ) = default;
	__dp_multiset( const __dp_multiset& s, const allocator_type& alloc ) : super( s, alloc ) { }  
	__dp_multiset( __dp_multiset&& ) = default;
	__dp_multiset( __dp_multiset&& s, const allocator_type& alloc ) : super( std::move( s ), alloc ) { }  

	__dp_multiset( std::initializer_list<value_pointer> il,
		 const key_compare& comp,
		 const allocator_type& alloc = allocator_type()) : super( comp, alloc )
	{
		for ( auto&& p : il ) super::emplace( std::forward<decltype( p )>( p ));
	}
	__dp_multiset( std::initializer_list<value_pointer> il,
		 const allocator_type& alloc = allocator_type()) : super( alloc )
	{
		for ( auto&& p : il ) super::emplace( std::forward<decltype( p )>( p ));
	}

#elif defined(__cplusplus) && (__cplusplus >= 201103)
	// ---- for c++11
	explicit __dp_multiset( const key_compare& comp = key_compare(),
		         const allocator_type& alloc = allocator_type()) : super( comp, alloc ) { }
	explicit __dp_multiset( const allocator_type& alloc ) : super( alloc ) { }

	template <class II>
	__dp_multiset( II first, II last, const key_compare& comp = key_compare(),
		const allocator_type& alloc = allocator_type()) : super( first, last, comp, alloc ) { }

	__dp_multiset( const __dp_multiset& ) = default;
	__dp_multiset( const __dp_multiset& s, const allocator_type& alloc ) : super( s, alloc ) { }  
	__dp_multiset( __dp_multiset&& ) = default;
	__dp_multiset( __dp_multiset&& s, const allocator_type& alloc ) : super( std::move( s ), alloc ) { }  

	__dp_multiset( std::initializer_list<value_pointer> il,
		 const key_compare& comp = key_compare(),
		 const allocator_type& alloc = allocator_type()) : super( comp, alloc )
	{
		for ( auto&& p : il ) super::emplace( std::forward<decltype( p )>( p ));
	}
#else
#error "c++11 up required"
#endif

	__dp_multiset& operator=( const __dp_multiset& ) = default;
	__dp_multiset& operator=( __dp_multiset&& ) = default;
	__dp_multiset& operator=( std::initializer_list<value_pointer> il )
	{
		super::clear();
		for ( auto&& p : il ) super::emplace( std::forward<decltype( p )>( p ));
		return *this;
	}

	~__dp_multiset() = default;

	// wrapper for appropriate casting of raw pointer
	template <typename... Args> iterator emplace( Args&&... args )
	{
		return super::emplace( std::forward<Args>( args )... );
	}
	template <typename... Args> iterator emplace_hint( const_iterator pos, Args&&... args )
	{
		return super::emplace_hint( pos, std::forward<Args>( args )... );
	}

	iterator insert( const value_type& p )
	{
		return super::insert( p );
	}
	iterator insert( value_type&& p )
	{
		return super::insert( std::move( p ));
	}
	iterator insert( const_iterator i, const value_type& p )
	{
		return super::insert( i, p );
	}
	iterator insert( const_iterator i, value_type&& p )
	{
		return super::insert( i, std::move( p ));
	}
	template <class II> void insert( II fst, II lst )
	{
		return super::insert( fst, lst );
	}
	void insert( std::initializer_list<value_pointer> il )
	{
		for ( auto&& p : il ) super::emplace( std::forward<decltype( p )>( p ));
	}

	// wrapper for appropriate casting of raw pointer avoiding unnecessary duplication
	iterator find( const key_pointer& k )
	{
		return super::find( reinterpret_cast<const KD&>( k ));
	}
	const_iterator find( const key_pointer& k ) const
	{
		return super::find( reinterpret_cast<const KD&>( k ));
	}

	iterator upper_bound( const key_pointer& k )
	{
		return super::upper_bound( reinterpret_cast<const KD&>( k ));
	}
	const_iterator upper_bound( const key_pointer& k ) const
	{
		return super::upper_bound( reinterpret_cast<const KD&>( k ));
	}

	iterator lower_bound( const key_pointer& k )
	{
		return super::lower_bound( reinterpret_cast<const KD&>( k ));
	}
	const_iterator lower_bound( const key_pointer& k ) const
	{
		return super::lower_bound( reinterpret_cast<const KD&>( k ));
	}

	std::pair<iterator, iterator> equal_range( const key_pointer& k )
	{
		return super::equal_range( reinterpret_cast<const KD&>( k ));
	}
	std::pair<const_iterator, const_iterator> equal_range( const key_pointer& k ) const
	{
		return super::equal_range( reinterpret_cast<const KD&>( k ));
	}

	size_type erase( const key_pointer& k )
	{
		return super::erase( reinterpret_cast<const KD&>( k ));
	}
	void erase( iterator i ) { super::erase( i ); }
	void erase( iterator fst, iterator lst ) { super::erase( fst, lst ); }

	size_type count( const key_pointer& k )
	{
		return super::count( reinterpret_cast<const KD&>( k ));
	}
};

//
// alias for deep pointer set/multiset: T=V*/V[]/V[N]
//
//	dp_set: set for deep pointer
//	dr_set: set for deep reference pointer
//
//	dp_multiset: multiset for deep pointer
//	dr_multiset: multiset for deep reference pointer
// -----------------------------------------------------------------------------
template <typename T> using dp_set      = __dp_set<deep_ptr<T>>;
template <typename T> using dr_set      = __dp_set<dref_ptr<T>>;

template <typename T> using dp_multiset = __dp_multiset<deep_ptr<T>>;
template <typename T> using dr_multiset = __dp_multiset<dref_ptr<T>>;


#ifndef NO_NAMESPACE
}       
#endif
        
#endif // __DPTL_DP_SET_HPP


