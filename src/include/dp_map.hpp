//
// dp_map.hpp - sample customization of std::map/multimap for deep pointer
//
//      2018/12/30, version 0.1
//
//      © Kazunari Saitoh, 2018-2019.
//      Distributed under the Boost Software License, Version 1.0.
//      (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//

#ifndef __DPTL_DP_MAP_HPP
#define __DPTL_DP_MAP_HPP

#include <map>
#include <dptl/deep_ptr.hpp>
                
#ifndef DPTL_NAMESPACE
#define DPTL_NAMESPACE dptl
#endif  
                
#ifndef NO_NAMESPACE    
namespace DPTL_NAMESPACE {
#endif          

//                
// map for deep pointer
// -----------------------------------------------------------------------------
template <typename K, typename T> 
class __dp_map : public std::map<dp_const<K>,T>
{
	typedef dp_const<K>				KD;
	typedef dp_const_type<K>			KT;
	typedef dp_const_pointer<K>			KP;
	typedef dp_type<T>				MT;
	typedef dp_pointer<T>				MP;
	typedef std::map<KD,T>				super;

    public:
	typedef typename super::key_type		key_type;
	typedef typename super::mapped_type		mapped_type;
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
	typedef MP					mapped_pointer;
	typedef std::pair<KP,MP>			value_pointer;

#if defined(__cplusplus) && (__cplusplus >= 201402)
	// ---- for c++14
	__dp_map() = default;
	explicit __dp_map( const key_compare& comp,
		         const allocator_type& alloc = allocator_type()) : super( comp, alloc ) { }
	explicit __dp_map( const allocator_type& alloc ) : super( alloc ) { }

	template <class II>
	__dp_map( II first, II last, const key_compare& comp,
		const allocator_type& alloc = allocator_type()) : super( first, last, comp, alloc ) { }
	template <class II>
	__dp_map( II first, II last, const allocator_type& alloc = allocator_type()) : super( first, last, alloc ) { }

	__dp_map( const __dp_map& ) = default;
	__dp_map( const __dp_map& s, const allocator_type& alloc ) : super( s, alloc ) { }  
	__dp_map( __dp_map&& ) = default;
	__dp_map( __dp_map&& s, const allocator_type& alloc ) : super( std::move( s ), alloc ) { }  

	__dp_map( std::initializer_list<value_pointer> il,
		 const key_compare& comp,
		 const allocator_type& alloc = allocator_type()) : super( comp, alloc )
	{
		for ( auto&& p : il ) super::emplace( p.first, p.second );
	}
	__dp_map( std::initializer_list<value_pointer> il,
		 const allocator_type& alloc = allocator_type()) : super( alloc )
	{
		for ( auto&& p : il ) super::emplace( p.first, p.second );
	}

#elif defined(__cplusplus) && (__cplusplus >= 201103)
	// ---- for c++11
	explicit __dp_map( const key_compare& comp = key_compare(),
		         const allocator_type& alloc = allocator_type()) : super( comp, alloc ) { }
	explicit __dp_map( const allocator_type& alloc ) : super( alloc ) { }

	template <class II>
	__dp_map( II first, II last, const key_compare& comp = key_compare(),
		const allocator_type& alloc = allocator_type()) : super( first, last, comp, alloc ) { }

	__dp_map( const __dp_map& ) = default;
	__dp_map( const __dp_map& s, const allocator_type& alloc ) : super( s, alloc ) { }  
	__dp_map( __dp_map&& ) = default;
	__dp_map( __dp_map&& s, const allocator_type& alloc ) : super( std::move( s ), alloc ) { }  

	__dp_map( std::initializer_list<value_pointer> il,
		 const key_compare& comp = key_compare(),
		 const allocator_type& alloc = allocator_type()) : super( comp, alloc )
	{
		for ( auto&& p : il ) super::emplace( p.first, p.second );
	}
#else
#error "c++11 up required"
#endif

	//__dp_map& operator=( const __dp_map& ) = default;
	__dp_map& operator=( const __dp_map& m )
	{
		super::clear();
		for ( auto&& p : m ) super::emplace( p.first, p.second );
		return *this;
	}
	__dp_map& operator=( __dp_map&& ) = default;
	__dp_map& operator=( std::initializer_list<value_pointer> il )
	{
		super::clear();
		for ( auto&& p : il ) super::emplace( p.first, p.second );
		return *this;
	}

	~__dp_map() = default;

	// wrapper for appropriate casting of raw pointer
	template <typename... Args> std::pair<iterator, bool> emplace( Args&&... args )
	{
		return super::emplace( std::forward<Args>( args )... );
	}
	template <typename... Args> iterator emplace_hint( const_iterator pos, Args&&... args )
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
	iterator erase( iterator i ) { return super::erase( i ); }
	iterator erase( iterator fst, iterator lst ) { return super::erase( fst, lst ); }

	size_type count( const key_pointer& k )
	{
		return super::count( reinterpret_cast<const KD&>( k ));
	}
};

//
// alias for deep pointer map, key: K=Q*/Q[]/Q[N], mapped-value: T=V*/V[]/V[N]
//
//	dpp_map: map for deep pointer key and deep pointer mapped-value
//	dpr_map: map for deep pointer key and deep reference pointer mapped-value
//	dpn_map: map for deep pointer key and non-pointer/non-array mapped-value
//
//	drp_map: map for deep reference pointer key and deep pointer mapped-value
//	drr_map: map for deep reference pointer key and deep reference pointer mapped-value
//	drn_map: map for deep reference pointer key and non-pointer/non-array mapped-value
//
//	dnp_map: map for non-pointer/non-array key and deep pointer mapped-value
//	dnr_map: map for non-pointer/non-array key and deep reference pointer mapped-value
//
// -----------------------------------------------------------------------------
template <typename K, typename T> using dpp_map = __dp_map<deep_ptr<K>, deep_ptr<T>>;
template <typename K, typename T> using dpr_map = __dp_map<deep_ptr<K>, dref_ptr<T>>;
template <typename K, typename T> using dpn_map = __dp_map<deep_ptr<K>, T>;

template <typename K, typename T> using drp_map = __dp_map<dref_ptr<K>, deep_ptr<T>>;
template <typename K, typename T> using drr_map = __dp_map<dref_ptr<K>, dref_ptr<T>>;
template <typename K, typename T> using drn_map = __dp_map<dref_ptr<K>, T>;

template <typename K, typename T> using dnp_map = __dp_map<K, deep_ptr<T>>;
template <typename K, typename T> using dnr_map = __dp_map<K, dref_ptr<T>>;

//
// multimap for deep pointer
// -----------------------------------------------------------------------------
template <typename K, typename T> 
class __dp_multimap : public std::multimap<dp_const<K>,T>
{
	typedef dp_const<K>				KD;
	typedef dp_const_type<K>			KT;
	typedef dp_const_pointer<K>			KP;
	typedef dp_type<T>				MT;
	typedef dp_pointer<T>				MP;
	typedef std::multimap<KD,T>			super;

    public:
	typedef typename super::key_type		key_type;
	typedef typename super::mapped_type		mapped_type;
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
	typedef MP					mapped_pointer;
	typedef std::pair<KP,MP>			value_pointer;

#if defined(__cplusplus) && (__cplusplus >= 201402)
	// ---- for c++14
	__dp_multimap() = default;
	explicit __dp_multimap( const key_compare& comp,
		         const allocator_type& alloc = allocator_type()) : super( comp, alloc ) { }
	explicit __dp_multimap( const allocator_type& alloc ) : super( alloc ) { }

	template <class II>
	__dp_multimap( II first, II last, const key_compare& comp,
		const allocator_type& alloc = allocator_type()) : super( first, last, comp, alloc ) { }
	template <class II>
	__dp_multimap( II first, II last, const allocator_type& alloc = allocator_type()) : super( first, last, alloc ) { }

	__dp_multimap( const __dp_multimap& ) = default;
	__dp_multimap( const __dp_multimap& s, const allocator_type& alloc ) : super( s, alloc ) { }  
	__dp_multimap( __dp_multimap&& ) = default;
	__dp_multimap( __dp_multimap&& s, const allocator_type& alloc ) : super( std::move( s ), alloc ) { }  

	__dp_multimap( std::initializer_list<value_pointer> il,
		 const key_compare& comp,
		 const allocator_type& alloc = allocator_type()) : super( comp, alloc )
	{
		for ( auto&& p : il ) super::emplace( p.first, p.second );
	}
	__dp_multimap( std::initializer_list<value_pointer> il,
		 const allocator_type& alloc = allocator_type()) : super( alloc )
	{
		for ( auto&& p : il ) super::emplace( p.first, p.second );
	}

#elif defined(__cplusplus) && (__cplusplus >= 201103)
	// ---- for c++11
	explicit __dp_multimap( const key_compare& comp = key_compare(),
		              const allocator_type& alloc = allocator_type()) : super( comp, alloc ) { }
	explicit __dp_multimap( const allocator_type& alloc ) : super( alloc ) { }

	template <class II>
	__dp_multimap( II first, II last, const key_compare& comp = key_compare(),
		     const allocator_type& alloc = allocator_type()) : super( first, last, comp, alloc ) { }

	__dp_multimap( const __dp_multimap& ) = default;
	__dp_multimap( const __dp_multimap& s, const allocator_type& alloc ) : super( s, alloc ) { }  
	__dp_multimap( __dp_multimap&& ) = default;
	__dp_multimap( __dp_multimap&& s, const allocator_type& alloc ) : super( std::move( s ), alloc ) { }  

	__dp_multimap( std::initializer_list<value_pointer> il,
		 const key_compare& comp = key_compare(),
		 const allocator_type& alloc = allocator_type()) : super( comp, alloc )
	{
		for ( auto&& p : il ) super::emplace( p.first, p.second );
	}
#else
#error "c++11 up required"
#endif

	//__dp_multimap& operator=( const __dp_multimap& ) = default;
	__dp_multimap& operator=( const __dp_multimap& m )
	{
		super::clear();
		for ( auto&& p : m ) super::emplace( p.first, p.second );
		return *this;
	}
	__dp_multimap& operator=( __dp_multimap&& ) = default;
	__dp_multimap& operator=( std::initializer_list<value_pointer> il )
	{
		super::clear();
		for ( auto&& p : il ) super::emplace( p.first, p.second );
		return *this;
	}

	~__dp_multimap() = default;

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
	iterator erase( iterator i ) { return super::erase( i ); }
	iterator erase( iterator fst, iterator lst ) { return super::erase( fst, lst ); }

	size_type count( const key_pointer& k )
	{
		return super::count( reinterpret_cast<const KD&>( k ));
	}
};

//
// alias for deep pointer multimap, key: K=Q*/Q[]/Q[N], mapped-value: T=V*/V[]/V[N]
//
//	dpp_multimap: multimap for deep pointer key and deep pointer mapped-value
//	dpr_multimap: multimap for deep pointer key and deep reference pointer mapped-value
//	dpn_multimap: multimap for deep pointer key and non-pointer/non-array mapped-value
//
//	drp_multimap: multimap for deep reference pointer key and deep pointer mapped-value
//	drr_multimap: multimap for deep reference pointer key and deep reference pointer mapped-value
//	drn_multimap: multimap for deep reference pointer key and non-pointer/non-array mapped-value
//
//	dnp_multimap: multimap for non-pointer/non-array key and deep pointer mapped-value
//	dnr_multimap: multimap for non-pointer/non-array key and deep reference pointer mapped-value
//
// -----------------------------------------------------------------------------
template <typename K, typename T> using dpp_multimap = __dp_multimap<deep_ptr<K>, deep_ptr<T>>;
template <typename K, typename T> using dpr_multimap = __dp_multimap<deep_ptr<K>, dref_ptr<T>>;
template <typename K, typename T> using dpn_multimap = __dp_multimap<deep_ptr<K>, T>;

template <typename K, typename T> using drp_multimap = __dp_multimap<dref_ptr<K>, deep_ptr<T>>;
template <typename K, typename T> using drr_multimap = __dp_multimap<dref_ptr<K>, dref_ptr<T>>;
template <typename K, typename T> using drn_multimap = __dp_multimap<dref_ptr<K>, T>;

template <typename K, typename T> using dnp_multimap = __dp_multimap<K, deep_ptr<T>>;
template <typename K, typename T> using dnr_multimap = __dp_multimap<K, dref_ptr<T>>;

#ifndef NO_NAMESPACE
}       
#endif
        
#endif // __DPTL_DP_MAP_HPP

