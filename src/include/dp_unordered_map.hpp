//
// dp_unordered_map.hpp - sample customization of std::unordered_map/std::unordered_multimap for deep pointer
//
//      2019/01/08, version 1.0.1
//
//      © Kazunari Saitoh, 2018-2019.
//      Distributed under the Boost Software License, Version 1.0.
//      (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//

#ifndef __DPTL_DP_UNORDERED_MAP_HPP
#define __DPTL_DP_UNORDERED_MAP_HPP

#include <unordered_map>
#include <dptl/deep_ptr.hpp>
                
#ifndef DPTL_NAMESPACE
#define DPTL_NAMESPACE dptl
#endif  
                
#ifndef NO_NAMESPACE    
namespace DPTL_NAMESPACE {
#endif          

//
// unordered_map for deep pointer
// -----------------------------------------------------------------------------
template <typename K, typename T, typename Hash = dp_hash<dp_type<K>,std::extent<K,1>::value>>
class __dp_unordered_map : public std::unordered_map<dp_const<K>,T,Hash>
{
	typedef dp_const<K>				KD;
	typedef dp_const_type<K>			KT;
	typedef dp_const_pointer<K>			KP;
	typedef dp_type<T>				MT;
	typedef dp_pointer<T>				MP;
	typedef std::unordered_map<KD,T,Hash>		super;

    public:
	typedef typename super::key_type		key_type;
	typedef typename super::mapped_type 		mapped_type;
	typedef typename super::value_type 		value_type;
	typedef typename super::hasher			hasher;
	typedef typename super::key_equal		key_equal;
	typedef typename super::allocator_type		allocator_type;
	typedef typename super::reference		reference;
	typedef typename super::const_reference		const_reference;
	typedef typename super::pointer			pointer;
	typedef typename super::const_pointer		const_pointer;
	typedef typename super::iterator		iterator;
	typedef typename super::const_iterator		const_iterator;
	typedef typename super::local_iterator		local_iterator;
	typedef typename super::const_local_iterator	const_local_iterator;
	typedef typename super::difference_type		difference_type;
	typedef typename super::size_type		size_type;

	typedef KP					key_pointer;
	typedef MP					mapped_pointer;
	typedef std::pair<KP,MP>			value_pointer;

#if defined(__cplusplus) && (__cplusplus >= 201402)
	// ---- for c++14
	__dp_unordered_map() = default;
	explicit __dp_unordered_map( size_type n, const hasher& hf = hasher(), const key_equal& eql = key_equal(),
					const allocator_type& alloc = allocator_type()) : super( n, hf, eql, alloc ) { }
	explicit __dp_unordered_map( const allocator_type& alloc ) : super( alloc ) { }
	__dp_unordered_map( size_type n, const allocator_type& alloc ) : super( n, alloc ) { }
	__dp_unordered_map( size_type n, const hasher& hf, const allocator_type& alloc ) : super( n, hf, alloc ) { }

	template <class II>
	__dp_unordered_map( II first, II last, size_type n = MinimumHashSize,
				const hasher& hf = hasher(), const key_equal& eql = key_equal(),
				const allocator_type& alloc = allocator_type()) : super( first, last, n, hf, eql, alloc ) { }
	template <class II>
	__dp_unordered_map( II first, II last, size_type n, const allocator_type& alloc ) : super( first, last, n, alloc ) { }
	template <class II>
	__dp_unordered_map( II first, II last, size_type n, const hasher& hf,
				const allocator_type& alloc ) : super( first, last, n, hf, alloc ) { }

	__dp_unordered_map( const __dp_unordered_map& ) = default;
	__dp_unordered_map( const __dp_unordered_map& um, const allocator_type& alloc ) : super( um, alloc ) { }  
	__dp_unordered_map( __dp_unordered_map&& ) = default;
	__dp_unordered_map( __dp_unordered_map&& um, const allocator_type& alloc ) : super( std::move( um ), alloc ) { }  

	__dp_unordered_map( std::initializer_list<value_pointer> il, size_type n = MinimumHashSize, 
				const hasher& hf = hasher(), const key_equal& eql = key_equal(),
					const allocator_type& alloc = allocator_type()) : super( n, hf, eql, alloc )
	{
		for ( auto&& p : il ) super::emplace( p.first, p.second );
	}
	__dp_unordered_map( std::initializer_list<value_pointer> il, size_type n,
				const allocator_type& alloc ) : super( n, alloc )
	{
		for ( auto&& p : il ) super::emplace( p.first, p.second );
	}
	__dp_unordered_map( std::initializer_list<value_pointer> il, size_type n, const hasher& hf,
				const allocator_type& alloc ) : super( n, hf, alloc )
	{
		for ( auto&& p : il ) super::emplace( p.first, p.second );
	}

#elif defined(__cplusplus) && (__cplusplus >= 201103)
	// ---- for c++11
	explicit __dp_unordered_map( size_type n = MinimumHashSize, const hasher& hf = hasher(),
					const key_equal& eql = key_equal(), const allocator_type& alloc = allocator_type())
				: super( n, hf, eql, alloc ) { }
	explicit __dp_unordered_map( const allocator_type& alloc ) : super( MinimumHashSize, alloc ) { }

	template <class II>
	__dp_unordered_map( II first, II last, size_type n = MinimumHashSize, const hasher& hf = hasher(),
				const key_equal& eql = key_equal(), const allocator_type& alloc = allocator_type())
				: super( first, last, n, hf, eql, alloc ) { }

	__dp_unordered_map( const __dp_unordered_map& ) = default;
	__dp_unordered_map( const __dp_unordered_map& um, const allocator_type& alloc ) : super( um, alloc ) { }  
	__dp_unordered_map( __dp_unordered_map&& ) = default;
	__dp_unordered_map( __dp_unordered_map&& um, const allocator_type& alloc ) : super( std::move( um ), alloc ) { }  

	__dp_unordered_map( std::initializer_list<value_pointer> il, size_type n = MinimumHashSize, 
				const hasher& hf = hasher(), const key_equal& eql = key_equal(),
					const allocator_type& alloc = allocator_type()) : super( n, hf, eql, alloc )
	{
		for ( auto&& p : il ) super::emplace( p.first, p.second );
	}
#else
#error "c++11 up required"
#endif

	__dp_unordered_map& operator=( const __dp_unordered_map& m )
	{
		super::clear();
		for ( auto&& p : m ) super::emplace( p.first, p.second );
		return *this;
	}
	__dp_unordered_map& operator=( __dp_unordered_map&& ) = default;
	__dp_unordered_map& operator=( std::initializer_list<value_pointer> il )
	{
		super::clear();
		for ( auto&& p : il ) super::emplace( p.first, p.second );
		return *this;
	}

	~__dp_unordered_map() = default;

	// wrapper for appropriate casting of raw pointer
	template <typename... Args>
	std::pair<iterator, bool> emplace( Args&&... args )
	{
		return super::emplace( std::forward<Args>( args )... );
	}

	template <typename... Args>
	iterator emplace_hint( const_iterator pos, Args&&... args )
	{
		return super::emplace_hint( pos, std::forward<Args>( args )... );
	}

	std::pair<iterator, bool> insert( const value_type& v )
	{
		return super::insert( v );
	}
	template <typename P, typename = std::enable_if_t<std::is_constructible<value_type,P&&>::value>>
	std::pair<iterator, bool> insert( P&& v )
	{
		return super::insert( std::forward<decltype( v )>( v ));
	}
	iterator insert( const_iterator i, const value_type& v )
	{
		return super::insert( i, v );
	}
	template <typename P, typename = std::enable_if_t<std::is_constructible<value_type,P&&>::value>>
	iterator insert( const_iterator i, P&& v )
	{
		return super::insert( i, std::forward<decltype( v )>( v ));
	}
	template <class II> void insert( II fst, II lst )
	{
		return super::insert( fst, lst );
	}
	void insert( std::initializer_list<value_pointer> il )
	{
		for ( auto&& p : il ) super::emplace( p.first, p.second );
	}

	// wrapper for appropriate casting of raw pointer avoiding unnecessary duplication
	T& at( const key_pointer& k )
	{
		return super::at( reinterpret_cast<const KD&>( k ));
	}
	const T& at( const key_pointer& k ) const
	{
		return super::at( reinterpret_cast<const KD&>( k ));
	}

	T& operator[]( const key_pointer& k )
	{
		return super::operator[]( k );
	}
	T& operator[]( key_pointer&& k ) 
	{
		return super::operator[]( std::move( k ));
	}

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

//
// alias for deep pointer unordered_map, key: K=Q*/Q[]/Q[N], mapped-value: T=V*/V[]/V[N]
//
//      dpp_unordered_map: unordered_map for deep pointer key and deep pointer mapped-value
//      dpr_unordered_map: unordered_map for deep pointer key and deep reference pointer mapped-value
//      dpn_unordered_map: unordered_map for deep pointer key and non-pointer/non-array mapped-value
//
//      drp_unordered_map: unordered_map for deep reference pointer key and deep pointer mapped-value
//      drr_unordered_map: unordered_map for deep reference pointer key and deep reference pointer mapped-value
//      drn_unordered_map: unordered_map for deep reference pointer key and non-pointer/non-array mapped-value
//
//      dnp_unordered_map: unordered_map for non-pointer/non-array key and deep pointer mapped-value
//      dnr_unordered_map: unordered_map for non-pointer/non-array key and deep reference pointer mapped-value
//
// -----------------------------------------------------------------------------
template <typename K, typename T> using dpp_unordered_map = __dp_unordered_map<deep_ptr<K>, deep_ptr<T>>;
template <typename K, typename T> using dpr_unordered_map = __dp_unordered_map<deep_ptr<K>, dref_ptr<T>>;
template <typename K, typename T> using dpn_unordered_map = __dp_unordered_map<deep_ptr<K>, T>;

template <typename K, typename T> using drp_unordered_map = __dp_unordered_map<dref_ptr<K>, deep_ptr<T>>;
template <typename K, typename T> using drr_unordered_map = __dp_unordered_map<dref_ptr<K>, dref_ptr<T>>;
template <typename K, typename T> using drn_unordered_map = __dp_unordered_map<dref_ptr<K>, T>;

template <typename K, typename T> using dnp_unordered_map = __dp_unordered_map<K, deep_ptr<T>>;
template <typename K, typename T> using dnr_unordered_map = __dp_unordered_map<K, dref_ptr<T>>;


//
// unordered_multimap for deep pointer
// -----------------------------------------------------------------------------
template <typename K, typename T, typename Hash = dp_hash<dp_type<K>,std::extent<K,1>::value>>
class __dp_unordered_multimap : public std::unordered_multimap<dp_const<K>,T,Hash>
{
	typedef dp_const<K>				KD;
	typedef dp_const_type<K>			KT;
	typedef dp_const_pointer<K>			KP;
	typedef dp_type<T>				MT;
	typedef dp_pointer<T>				MP;
	typedef std::unordered_multimap<KD,T,Hash>	super;

    public:
	typedef typename super::key_type		key_type;
	typedef typename super::mapped_type 		mapped_type;
	typedef typename super::value_type 		value_type;
	typedef typename super::hasher			hasher;
	typedef typename super::key_equal		key_equal;
	typedef typename super::allocator_type		allocator_type;
	typedef typename super::reference		reference;
	typedef typename super::const_reference		const_reference;
	typedef typename super::pointer			pointer;
	typedef typename super::const_pointer		const_pointer;
	typedef typename super::iterator		iterator;
	typedef typename super::const_iterator		const_iterator;
	typedef typename super::local_iterator		local_iterator;
	typedef typename super::const_local_iterator	const_local_iterator;
	typedef typename super::difference_type		difference_type;
	typedef typename super::size_type		size_type;

	typedef KP					key_pointer;
	typedef MP					mapped_pointer;
	typedef std::pair<KP,MP>			value_pointer;

#if defined(__cplusplus) && (__cplusplus >= 201402)
	// ---- for c++14
	__dp_unordered_multimap() = default;
	explicit __dp_unordered_multimap( size_type n, const hasher& hf = hasher(), const key_equal& eql = key_equal(),
				const allocator_type& alloc = allocator_type()) : super( n, hf, eql, alloc ) { }
	explicit __dp_unordered_multimap( const allocator_type& alloc ) : super( alloc ) { }
	__dp_unordered_multimap( size_type n, const allocator_type& alloc ) : super( n, alloc ) { }
	__dp_unordered_multimap( size_type n, const hasher& hf, const allocator_type& alloc ) : super( n, hf, alloc ) { }

	template <class II>
	__dp_unordered_multimap( II first, II last, size_type n = MinimumHashSize,
				const hasher& hf = hasher(), const key_equal& eql = key_equal(),
				const allocator_type& alloc = allocator_type()) : super( first, last, n, hf, eql, alloc ) { }
	template <class II>
	__dp_unordered_multimap( II first, II last, size_type n, const allocator_type& alloc ) : super( first, last, n, alloc ) { }
	template <class II>
	__dp_unordered_multimap( II first, II last, size_type n, const hasher& hf,
				const allocator_type& alloc ) : super( first, last, n, hf, alloc ) { }

	__dp_unordered_multimap( const __dp_unordered_multimap& ) = default;
	__dp_unordered_multimap( const __dp_unordered_multimap& um, const allocator_type& alloc ) : super( um, alloc ) { }  
	__dp_unordered_multimap( __dp_unordered_multimap&& ) = default;
	__dp_unordered_multimap( __dp_unordered_multimap&& um, const allocator_type& alloc ) : super( std::move( um ), alloc ) { }  

	__dp_unordered_multimap( std::initializer_list<value_pointer> il, size_type n = MinimumHashSize, 
				const hasher& hf = hasher(), const key_equal& eql = key_equal(),
					const allocator_type& alloc = allocator_type()) : super( n, hf, eql, alloc )
	{
		for ( auto&& p : il ) super::emplace( p.first, p.second );
	}
	__dp_unordered_multimap( std::initializer_list<value_pointer> il, size_type n,
				const allocator_type& alloc ) : super( n, alloc )
	{
		for ( auto&& p : il ) super::emplace( p.first, p.second );
	}
	__dp_unordered_multimap( std::initializer_list<value_pointer> il, size_type n, const hasher& hf,
				const allocator_type& alloc ) : super( n, hf, alloc )
	{
		for ( auto&& p : il ) super::emplace( p.first, p.second );
	}

#elif defined(__cplusplus) && (__cplusplus >= 201103)
	// ---- for c++11
	explicit __dp_unordered_multimap( size_type n = MinimumHashSize, const hasher& hf = hasher(),
					const key_equal& eql = key_equal(), const allocator_type& alloc = allocator_type())
				: super( n, hf, eql, alloc ) { }
	explicit __dp_unordered_multimap( const allocator_type& alloc )
				: super( MinimumHashSize, hasher(), allocator_type(), alloc ) { }

	template <class II>
	__dp_unordered_multimap( II first, II last, size_type n = MinimumHashSize, const hasher& hf = hasher(),
				const key_equal& eql = key_equal(), const allocator_type& alloc = allocator_type())
				: super( first, last, n, hf, eql, alloc ) { }

	__dp_unordered_multimap( const __dp_unordered_multimap& ) = default;
	__dp_unordered_multimap( const __dp_unordered_multimap& um, const allocator_type& alloc ) : super( um, alloc ) { }  
	__dp_unordered_multimap( __dp_unordered_multimap&& ) = default;
	__dp_unordered_multimap( __dp_unordered_multimap&& um, const allocator_type& alloc ) : super( std::move( um ), alloc ) { }  

	__dp_unordered_multimap( std::initializer_list<value_pointer> il, size_type n = MinimumHashSize, 
				const hasher& hf = hasher(), const key_equal& eql = key_equal(),
				const allocator_type& alloc = allocator_type()) : super( n, hf, eql, alloc )
	{
		for ( auto&& p : il ) super::emplace( p.first, p.second );
	}
#else
#error "c++11 up required"
#endif

	__dp_unordered_multimap& operator=( const __dp_unordered_multimap& m )
	{
		super::clear();
		for ( auto&& p : m ) super::emplace( p.first, p.second );
		return *this;
	}
	__dp_unordered_multimap& operator=( __dp_unordered_multimap&& ) = default;
	__dp_unordered_multimap& operator=( std::initializer_list<value_pointer> il )
	{
		super::clear();
		for ( auto&& p : il ) super::emplace( p.first, p.second );
		return *this;
	}

	~__dp_unordered_multimap() = default;

	// wrapper for appropriate casting of raw pointer
	template <typename... Args>
	iterator emplace( Args&&... args )
	{
		return super::emplace( std::forward<Args>( args )... );
	}

	template <typename... Args>
	iterator emplace_hint( const_iterator pos, Args&&... args )
	{
		return super::emplace_hint( pos, std::forward<Args>( args )... );
	}

	iterator insert( const value_type& v )
	{
		return super::insert( v );
	}
	template <typename P, typename = std::enable_if_t<std::is_constructible<value_type,P&&>::value>>
	iterator insert( P&& v )
	{
		return super::insert( std::forward<decltype( v )>( v ));
	}
	iterator insert( const_iterator i, const value_type& v )
	{
		return super::insert( i, v );
	}
	template <typename P, typename = std::enable_if_t<std::is_constructible<value_type,P&&>::value>>
	iterator insert( const_iterator i, P&& v )
	{
		return super::insert( i, std::forward<decltype( v )>( v ));
	}
	template <class II> void insert( II fst, II lst )
	{
		return super::insert( fst, lst );
	}
	void insert( std::initializer_list<value_pointer> il )
	{
		for ( auto&& p : il ) super::emplace( p.first, p.second );
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

//
// alias for deep pointer unordered_multimap, key: K=Q*/Q[]/Q[N], mapped-value: T=V*/V[]/V[N]
//
//      dpp_unordered_multimap: unordered_multimap for deep pointer key and deep pointer mapped-value
//      dpr_unordered_multimap: unordered_multimap for deep pointer key and deep reference pointer mapped-value
//      dpn_unordered_multimap: unordered_multimap for deep pointer key and non-pointer/non-array mapped-value
//
//      drp_unordered_multimap: unordered_multimap for deep reference pointer key and deep pointer mapped-value
//      drr_unordered_multimap: unordered_multimap for deep reference pointer key and deep reference pointer mapped-value
//      drn_unordered_multimap: unordered_multimap for deep reference pointer key and non-pointer/non-array mapped-value
//
//      dnp_unordered_multimap: unordered_multimap for non-pointer/non-array key and deep pointer mapped-value
//      dnr_unordered_multimap: unordered_multimap for non-pointer/non-array key and deep reference pointer mapped-value
//
// -----------------------------------------------------------------------------
template <typename K, typename T> using dpp_unordered_multimap = __dp_unordered_multimap<deep_ptr<K>, deep_ptr<T>>;
template <typename K, typename T> using dpr_unordered_multimap = __dp_unordered_multimap<deep_ptr<K>, dref_ptr<T>>;
template <typename K, typename T> using dpn_unordered_multimap = __dp_unordered_multimap<deep_ptr<K>, T>;

template <typename K, typename T> using drp_unordered_multimap = __dp_unordered_multimap<dref_ptr<K>, deep_ptr<T>>;
template <typename K, typename T> using drr_unordered_multimap = __dp_unordered_multimap<dref_ptr<K>, dref_ptr<T>>;
template <typename K, typename T> using drn_unordered_multimap = __dp_unordered_multimap<dref_ptr<K>, T>;

template <typename K, typename T> using dnp_unordered_multimap = __dp_unordered_multimap<K, deep_ptr<T>>;
template <typename K, typename T> using dnr_unordered_multimap = __dp_unordered_multimap<K, dref_ptr<T>>;


#ifndef NO_NAMESPACE
}       
#endif
        
#endif // __DPTL_DP_UNORDERD_MAP_HPP
