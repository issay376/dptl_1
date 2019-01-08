//
// dp_unordered_set.hpp - sample customization of std::unordered_set/std::unordered_multiset for deep pointer
//
//      2019/01/08, version 1.0.1
//
//      © Kazunari Saitoh, 2018-2019.
//      Distributed under the Boost Software License, Version 1.0.
//      (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//

#ifndef __DPTL_DP_UNORDERED_SET_HPP
#define __DPTL_DP_UNORDERED_SET_HPP

#include <unordered_set>
#include <dptl/deep_ptr.hpp>
                
#ifndef DPTL_NAMESPACE
#define DPTL_NAMESPACE dptl
#endif  
                
#ifndef NO_NAMESPACE    
namespace DPTL_NAMESPACE {
#endif          

// policy based pointer unordered_set 
// -----------------------------------------------------------------------------
template <typename K, typename Hash = dp_hash<dp_type<K>,std::extent<K,1>::value>> 
class __dp_unordered_set : public std::unordered_set<dp_const<K>,Hash>
{
	typedef dp_const<K>				KD;
	typedef dp_const_type<K>			KT;
	typedef dp_const_pointer<K>			KP;
	typedef std::unordered_set<KD,Hash>		super;

    public:
	typedef typename super::key_type		key_type;	
	typedef typename super::value_type		value_type;
	typedef typename super::hasher			hasher;
	typedef typename super::key_equal		key_equal;
	typedef typename super::allocator_type		allocator_type;
	typedef typename super::reference		reference;
	typedef typename super::const_reference		const_reference;
	typedef typename super::pointer			pointer;
	typedef typename super::const_pointer		const_pointer;
	typedef typename super::iterator		iterator;
	typedef typename super::const_iterator		const_iterator;
	typedef typename super::local_iterator	reverse_iterator;
	typedef typename super::const_local_iterator	const_reverse_iterator;
	typedef typename super::difference_type		difference_type;
	typedef typename super::size_type		size_type;

	typedef KP					key_pointer;
	typedef KP					value_pointer;

#if defined(__cplusplus) && (__cplusplus >= 201402)
	// ---- for c++14
	__dp_unordered_set() = default;
	explicit __dp_unordered_set( size_type n, const hasher& hf = hasher(), const key_equal& eql = key_equal(),
		         const allocator_type& alloc = allocator_type()) : super( n, hf, eql, alloc ) { }
	explicit __dp_unordered_set( const allocator_type& alloc ) : super( alloc ) { }
	explicit __dp_unordered_set( size_type n, const allocator_type& alloc ) : super( n, alloc ) { }
	explicit __dp_unordered_set( size_type n, const hasher& hf, const allocator_type& alloc ) : super( n, hf, alloc ) { }

	template <class II>
	__dp_unordered_set( II first, II last, size_type n = MinimumHashSize, 
			  const hasher& hf = hasher(), const key_equal& eql = key_equal(),
			  const allocator_type& alloc = allocator_type()) : super( first, last, n, hf, eql, alloc ) { }
	template <class II>
	__dp_unordered_set( II first, II last, size_type n,
			  const allocator_type& alloc ) : super( first, last, n, alloc ) { }
	template <class II>
	__dp_unordered_set( II first, II last, size_type n, const hasher& hf,
			  const allocator_type& alloc ) : super( first, last, n, hf, alloc ) { }

	__dp_unordered_set( const __dp_unordered_set& ) = default;
	__dp_unordered_set( const __dp_unordered_set& s, const allocator_type& alloc ) : super( s, alloc ) { }  
	__dp_unordered_set( __dp_unordered_set&& ) = default;
	__dp_unordered_set( __dp_unordered_set&& s, const allocator_type& alloc ) : super( std::move( s ), alloc ) { }  

	__dp_unordered_set( std::initializer_list<value_pointer> il, size_type n = MinimumHashSize, 
			  const hasher& hf = hasher(), const key_equal& eql = key_equal(),
			  const allocator_type& alloc = allocator_type()) : super( n, hf, eql, alloc )
	{
		for ( auto&& p : il ) super::emplace( std::forward<decltype( p )>( p ));
	}
	__dp_unordered_set( std::initializer_list<value_pointer> il, size_type n,
			  const allocator_type& alloc ) : super( n, alloc )
	{
		for ( auto&& p : il ) super::emplace( std::forward<decltype( p )>( p ));
	}
	__dp_unordered_set( std::initializer_list<value_pointer> il, size_type n,
			  const hasher& hf, const allocator_type& alloc ) : super( n, hf, alloc )
	{
		for ( auto&& p : il ) super::emplace( std::forward<decltype( p )>( p ));
	}

#elif defined(__cplusplus) && (__cplusplus >= 201103)
	// ---- for c++11
        explicit __dp_unordered_set( size_type n = MinimumHashSize, const hasher& hf = hasher(),
                                   const key_equal& eql = key_equal(), const allocator_type& alloc = allocator_type())
                                : super( n, hf, eql, alloc ) { }
        explicit __dp_unordered_set( const allocator_type& alloc ) : super( MinimumHashSize, alloc ) { }

        template <class II>
        __dp_unordered_set( II first, II last, size_type n = MinimumHashSize, const hasher& hf = hasher(),
                         const key_equal& eql = key_equal(), const allocator_type& alloc = allocator_type())
                                : super( first, last, n, hf, eql, alloc ) { }

        __dp_unordered_set( const __dp_unordered_set& ) = default;
        __dp_unordered_set( const __dp_unordered_set& um, const allocator_type& alloc ) : super( um, alloc ) { }
        __dp_unordered_set( __dp_unordered_set&& ) = default;
        __dp_unordered_set( __dp_unordered_set&& um, const allocator_type& alloc ) : super( std::move( um ), alloc ) { }

        __dp_unordered_set( std::initializer_list<value_pointer> il, size_type n = MinimumHashSize,
                         const hasher& hf = hasher(), const key_equal& eql = key_equal(),
                         const allocator_type& alloc = allocator_type()) : super( n, hf, eql, alloc )
        {
                for ( auto&& p : il ) super::emplace( std::forward<decltype( p )>( p ));
        }
#else
#error "c++11 up required"
#endif

	__dp_unordered_set& operator=( const __dp_unordered_set& ) = default;
	__dp_unordered_set& operator=( __dp_unordered_set&& ) = default;
	__dp_unordered_set& operator=( std::initializer_list<value_pointer> il )
	{
		super::clear();
		for ( auto&& p : il ) super::emplace( std::forward<decltype( p )>( p ));
		return *this;
	}

	~__dp_unordered_set() = default;

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
		for ( auto&& p : il ) super::insert( std::forward<decltype( p )>( p ));
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
	iterator erase( const_iterator i ) { return super::erase( i ); }
	iterator erase( const_iterator fst, const_iterator lst ) { return super::erase( fst, lst ); }

	size_type bucket( const key_pointer& k )
	{
		return super::bucket( reinterpret_cast<const KD&>( k ));
	}

	size_type count( const key_pointer& k )
	{
		return super::count( reinterpret_cast<const KD&>( k ));
	}
};


// policy based pointer unordered_multiset 
// -----------------------------------------------------------------------------
template <typename K, typename Hash = dp_hash<dp_type<K>,std::extent<K,1>::value>> 
class __dp_unordered_multiset : public std::unordered_multiset<dp_const<K>,Hash>
{
	typedef dp_const<K>				KD;
	typedef dp_const_type<K>			KT;
	typedef dp_const_pointer<K>			KP;
	typedef std::unordered_multiset<KD,Hash>	super;

    public:
	typedef typename super::key_type		key_type;	
	typedef typename super::value_type		value_type;
	typedef typename super::hasher			hasher;
	typedef typename super::key_equal		key_equal;
	typedef typename super::allocator_type		allocator_type;
	typedef typename super::reference		reference;
	typedef typename super::const_reference		const_reference;
	typedef typename super::pointer			pointer;
	typedef typename super::const_pointer		const_pointer;
	typedef typename super::iterator		iterator;
	typedef typename super::const_iterator		const_iterator;
	typedef typename super::local_iterator	reverse_iterator;
	typedef typename super::const_local_iterator	const_reverse_iterator;
	typedef typename super::difference_type		difference_type;
	typedef typename super::size_type		size_type;

	typedef KP					key_pointer;
	typedef KP					value_pointer;

#if defined(__cplusplus) && (__cplusplus >= 201402)
	// ---- for c++14
	__dp_unordered_multiset() = default;
	explicit __dp_unordered_multiset( size_type n, const hasher& hf = hasher(), const key_equal& eql = key_equal(),
		         const allocator_type& alloc = allocator_type()) : super( n, hf, eql, alloc ) { }
	explicit __dp_unordered_multiset( const allocator_type& alloc ) : super( alloc ) { }
	explicit __dp_unordered_multiset( size_type n, const allocator_type& alloc ) : super( n, alloc ) { }
	explicit __dp_unordered_multiset( size_type n, const hasher& hf, const allocator_type& alloc ) : super( n, hf, alloc ) { }

	template <class II>
	__dp_unordered_multiset( II first, II last, size_type n = MinimumHashSize, 
			  const hasher& hf = hasher(), const key_equal& eql = key_equal(),
			  const allocator_type& alloc = allocator_type()) : super( first, last, n, hf, eql, alloc ) { }
	template <class II>
	__dp_unordered_multiset( II first, II last, size_type n,
			  const allocator_type& alloc ) : super( first, last, n, alloc ) { }
	template <class II>
	__dp_unordered_multiset( II first, II last, size_type n, const hasher& hf,
			  const allocator_type& alloc ) : super( first, last, n, hf, alloc ) { }

	__dp_unordered_multiset( const __dp_unordered_multiset& ) = default;
	__dp_unordered_multiset( const __dp_unordered_multiset& s, const allocator_type& alloc ) : super( s, alloc ) { }  
	__dp_unordered_multiset( __dp_unordered_multiset&& ) = default;
	__dp_unordered_multiset( __dp_unordered_multiset&& s, const allocator_type& alloc ) : super( std::move( s ), alloc ) { }  

	__dp_unordered_multiset( std::initializer_list<value_pointer> il, size_type n = MinimumHashSize, 
			  const hasher& hf = hasher(), const key_equal& eql = key_equal(),
			  const allocator_type& alloc = allocator_type()) : super( n, hf, eql, alloc )
	{
		for ( auto&& p : il ) super::emplace( std::forward<decltype( p )>( p ));
	}
	__dp_unordered_multiset( std::initializer_list<value_pointer> il, size_type n,
			  const allocator_type& alloc ) : super( n, alloc )
	{
		for ( auto&& p : il ) super::emplace( std::forward<decltype( p )>( p ));
	}
	__dp_unordered_multiset( std::initializer_list<value_pointer> il, size_type n,
			  const hasher& hf, const allocator_type& alloc ) : super( n, hf, alloc )
	{
		for ( auto&& p : il ) super::emplace( std::forward<decltype( p )>( p ));
	}

#elif defined(__cplusplus) && (__cplusplus >= 201103)
	// ---- for c++11
        explicit __dp_unordered_multiset( size_type n = MinimumHashSize, const hasher& hf = hasher(),
                                   const key_equal& eql = key_equal(), const allocator_type& alloc = allocator_type())
                                : super( n, hf, eql, alloc ) { }
        explicit __dp_unordered_multiset( const allocator_type& alloc ) : super( MinimumHashSize, alloc ) { }

        template <class II>
        __dp_unordered_multiset( II first, II last, size_type n = MinimumHashSize, const hasher& hf = hasher(),
                         const key_equal& eql = key_equal(), const allocator_type& alloc = allocator_type())
                                : super( first, last, n, hf, eql, alloc ) { }

        __dp_unordered_multiset( const __dp_unordered_multiset& ) = default;
        __dp_unordered_multiset( const __dp_unordered_multiset& um, const allocator_type& alloc ) : super( um, alloc ) { }
        __dp_unordered_multiset( __dp_unordered_multiset&& ) = default;
        __dp_unordered_multiset( __dp_unordered_multiset&& um, const allocator_type& alloc ) : super( std::move( um ), alloc ) { }

        __dp_unordered_multiset( std::initializer_list<value_pointer> il, size_type n = MinimumHashSize,
                         const hasher& hf = hasher(), const key_equal& eql = key_equal(),
                         const allocator_type& alloc = allocator_type()) : super( n, hf, eql, alloc )
        {
                for ( auto&& p : il ) super::emplace( std::forward<decltype( p )>( p ));
        }
#else
#error "c++11 up required"
#endif

	__dp_unordered_multiset& operator=( const __dp_unordered_multiset& ) = default;
	__dp_unordered_multiset& operator=( __dp_unordered_multiset&& ) = default;
	__dp_unordered_multiset& operator=( std::initializer_list<value_pointer> il )
	{
		super::clear();
		for ( auto&& p : il ) super::emplace( std::forward<decltype( p )>( p ));
		return *this;
	}

	~__dp_unordered_multiset() = default;

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
		for ( auto&& p : il ) super::insert( std::forward<decltype( p )>( p ));
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
	iterator erase( const_iterator i ) { return super::erase( i ); }
	iterator erase( const_iterator fst, const_iterator lst ) { return super::erase( fst, lst ); }

	size_type bucket( const key_pointer& k )
	{
		return super::bucket( reinterpret_cast<const KD&>( k ));
	}

	size_type count( const key_pointer& k )
	{
		return super::count( reinterpret_cast<const KD&>( k ));
	}
};

// alias for deep pointer unorder/set/unorder_multiset: T=V*/V[]/V[N]
//
//	dp_unordered_set: unordered_set for deep pointer
//	dr_unordered_set: unordered_set for deep reference pointer
//
//	dp_unordered_multiset: unordered_multiset for deep pointer
//	dr_unordered_multiset: unordered_multiset for deep reference pointer
// -----------------------------------------------------------------------------
template <typename T> using dp_unordered_set      = __dp_unordered_set<deep_ptr<T>>;
template <typename T> using dr_unordered_set      = __dp_unordered_set<dref_ptr<T>>;

template <typename T> using dp_unordered_multiset = __dp_unordered_multiset<deep_ptr<T>>;
template <typename T> using dr_unordered_multiset = __dp_unordered_multiset<dref_ptr<T>>;


#ifndef NO_NAMESPACE
}       
#endif
        
#endif // __DPTL_DP_UNORDERED_SET_HPP
