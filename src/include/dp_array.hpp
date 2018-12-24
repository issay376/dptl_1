//
// test implementation of policy based std::array
//
//
//

#ifndef __DPTL_DP_ARRAY_HPP
#define __DPTL_DP_ARRAY_HPP

#include <array>
#include <dptl/deep_ptr.hpp>
		
#ifndef DPTL_NAMESPACE
#define DPTL_NAMESPACE dptl
#endif  
		
#ifndef NO_NAMESPACE
namespace DPTL_NAMESPACE {
#endif

// policy based pointer set
// -----------------------------------------------------------------------------
template <typename T, size_t N>
class __dp_array : public std::array<T,N>
{
	typedef dp_type<T>				VT;
	typedef dp_pointer<T>				VP;
	typedef std::array<T,N>				super;

    public:
	typedef typename super::value_type		value_type;
	typedef typename super::reference		reference;
	typedef typename super::const_reference		const_reference;
	typedef typename super::pointer			pointer;
	typedef	typename super::const_pointer		const_pointer;
	typedef	typename super::iterator		iterator;
	typedef	typename super::const_iterator		const_iterator;
	typedef	typename super::reverse_iterator	reverse_iterator;
	typedef	typename super::const_reverse_iterator	const_reverse_iterator;
	typedef	typename super::difference_type		difference_type;
	typedef	typename super::size_type		size_type;

	typedef VP					value_pointer;

	__dp_array() = default;
	__dp_array( const __dp_array& ) = default;
	__dp_array( __dp_array&& ) = default;
	__dp_array( std::initializer_list<value_pointer> il )
	{
		const value_pointer* p = il.begin();
		for ( size_type i = 0; i < N; ++i ) {
			if ( p == il.end()) break;
			this->at( i ) = *p++;
		}
	}

	__dp_array& operator=( const __dp_array& ) = default;
	__dp_array& operator=( __dp_array&& ) = default;
	__dp_array& operator=( std::initializer_list<value_pointer> il )
	{
		return *this = __dp_array<T,N>( il );
	}

	~__dp_array() = default;

	// wrapper for appropriate casting of raw pointer avoiding unnecessary duplication
	void fill( const value_pointer& v )
	{
		super::fill( reinterpret_cast<const T&>( v ));
	}
};
		
// alias for array (for __dp_array.h)
// -----------------------------------------------------------------------------
template <typename T, size_t N> using dp_array = __dp_array<deep_ptr<T>,N>;
template <typename T, size_t N> using dr_array = __dp_array<dref_ptr<T>,N>;

#ifndef NO_NAMESPACE
}       
#endif
	
#endif // __DPTL_DP_ARRAY_HPP

