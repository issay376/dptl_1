//
// dp_list.hpp - sample customization of std::list for deep pointer
//
//
//

#ifndef __DPTL_DP_LIST_HPP
#define __DPTL_DP_LIST_HPP

#include <list>
#include <dptl/deep_ptr.hpp>
		
#ifndef DPTL_NAMESPACE
#define DPTL_NAMESPACE dptl
#endif  
		
#ifndef NO_NAMESPACE
namespace DPTL_NAMESPACE {
#endif

// policy based pointer set
// -----------------------------------------------------------------------------
template <typename T, Policy P>
class pp_list : public std::list<ppointer<T,P>>
{
	typedef std::list<ppointer<T,P>>		super;

    public:
	typedef typename value_p<T>::t			value_type;
	typedef typename super::allocator_type	  allocator_type;
	typedef typename super::reference	       reference;
	typedef typename super::const_reference	 const_reference;
	typedef typename super::pointer		 pointer;
	typedef typename super::const_pointer	   const_pointer;
	typedef typename super::iterator		iterator;
	typedef typename super::const_iterator	  const_iterator;
	typedef typename super::reverse_iterator	reverse_iterator;
	typedef typename super::const_reverse_iterator  const_reverse_iterator;
	typedef typename super::difference_type	 difference_type;
	typedef typename super::size_type	       size_type;

#if defined(__cplusplus) && (__cplusplus >= 201402)
	pp_list() = default;
	explicit pp_list( const allocator_type& alloc ) : super( alloc ) { }
	explicit pp_list( size_type n, const allocator_type& alloc = allocator_type()) : super( n, alloc ) { }
	pp_list( size_type n, const value_type& v, const allocator_type& alloc = allocator_type()) : super( alloc )
	{
		assign( n, v );
	}

	template <class II>
	pp_list( II first, II last, const allocator_type& alloc = allocator_type()) : super( first, last, alloc ) { }

	pp_list( const pp_list& ) = default;
	pp_list( const pp_list& l, const allocator_type& alloc ) : super( l, alloc ) { }
	pp_list( pp_list&& ) = default;
	pp_list( pp_list&& l, const allocator_type& alloc ) : super( std::move( l ), alloc ) { }

	pp_list( std::initializer_list<value_type> il,
		 const allocator_type& alloc = allocator_type()) : super( alloc )
	{
		for ( auto p : il ) super::push_back( p );
	}

#elif defined(__cplusplus) && (__cplusplus >= 201103)
	// ---- for c++11
	explicit pp_list( const allocator_type& alloc = allocator_type()) : super( alloc ) { }
	explicit pp_list( size_type n ) : super( n ) { }
	pp_list( size_type n, const value_type& v, const allocator_type& alloc = allocator_type()) : super( alloc )
	{
		assign( n, v );
	}

	template <class II>
	pp_list( II first, II last, const allocator_type& alloc = allocator_type()) : super( first, last, alloc ) { }

	pp_list( const pp_list& ) = default;
	pp_list( const pp_list& l, const allocator_type& alloc ) : super( l, alloc ) { }
	pp_list( pp_list&& ) = default;
	pp_list( pp_list&& l, const allocator_type& alloc ) : super( std::move( l ), alloc ) { }

	pp_list( std::initializer_list<value_type> il,
		 const allocator_type& alloc = allocator_type()) : super( alloc )
	{
		for ( auto p : il ) super::push_back( p );
	}
#else
#error "c++11 up required"
#endif

	pp_list& operator=( const pp_list& ) = default;
	pp_list& operator=( pp_list&& ) = default;
	pp_list& operator=( std::initializer_list<value_type> il )
	{
		super::clear();
		for ( auto p : il ) super::push_back( p );
		return *this;
	}

	~pp_list() = default;

	// wrapper for appropriate casting of raw pointer
	template <class II> void assign( II fst, II lst )
	{
		super::assign( fst, lst );
	}
	void assign( size_type n, const value_type& v )
	{
		refer_ptr<T>   p( v );
		super::assign( n, reinterpret_cast<ppointer<T,P>&>( p ));
	}
	void assign( std::initializer_list<value_type> il )
	{
		super::clear();
		for ( auto p : il ) super::push_back( p );
	}

	iterator insert( const_iterator i, const value_type& p )
	{
		return super::insert( i, ppointer<T,P>( p ));
	}
	iterator insert( const_iterator i, value_type&& p )
	{
		return super::insert( i, ppointer<T,P>( p ));
	}
	iterator insert( const_iterator i, size_type n, const value_type& v )
	{
		refer_ptr<T>   p( v );
		return super::insert( i, n, reinterpret_cast<ppointer<T,P>&>( p ));
	}
	template <class II> iterator insert( const_iterator i, II fst, II lst )
	{
		return super::insert( i, fst, lst );
	}
        iterator insert( const_iterator i, std::initializer_list<value_type> il )
        {
		// 'reinterpret_cast' is unavoidable to support null initializer_list,
		// but it's possible to ommit it (or limit it only) in the case of null il:
		//	iterator	r/* = reinterpret_cast<iterator&>( i )*/; // comment out if support null il
		//	for ( const value_type* p = il.end(); p != il.begin(); ) i = r = insert( i, *--p );
		//	return r;
		//
                for ( const value_type* p = il.end(); p != il.begin(); ) i = insert( i, *--p );
                return reinterpret_cast<iterator&>( i );
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

	// wrapper for appropriate casting of raw pointer avoiding unnecessary duplication
	void remove( const value_type& k )
	{
		refer_ptr<T>   p( k );
		super::remove( reinterpret_cast<ppointer<T,P>&>( p ));
	}

	void resize( size_type n ) { super::resize( n ); }
	void resize( size_type n, const value_type& v )
	{
		refer_ptr<T>   p( v );
		super::resize( n, reinterpret_cast<ppointer<T,P>&>( p ));
	}
};
		
// alias for list (for pp_list.h)
// -----------------------------------------------------------------------------
template <typename T> using pr_list = pp_list<T,pRefer>;
template <typename T> using ps_list = pp_list<T,pStore>;
template <typename T> using pc_list = pp_list<T,pCopy>;

#ifndef NO_NAMESPACE
}       
#endif
	
#endif // __DPTL_DP_LIST_HPP

