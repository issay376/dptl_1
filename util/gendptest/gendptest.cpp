//
// gendptest.cpp --- deep pointer combination test code generator for pcontainers
//
//      Version 2.0
//
//      Copyright (C) 2016, 2018 Kazunari Saitoh
//
//      This library is free software; you can redistribute it and/or
//      modify it under the terms of the GNU Lesser General Public
//      License as published by the Free Software Foundation; either
//      version 2.1 of the License, or (at your option) any later version.
//
//      This library is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//      Lesser General Public License for more details.
//
//      You should have received a copy of the GNU Lesser General Public
//      License along with this library; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
//
// Usage: gendptest [-d][-s|+s][-p pcontainer] in-file [out-file]\n" );
//
//	-p pcontainer	target pcontainer
//	in-file		input filename
//	out-file	output filename ( defualt: stdout )
//
//	-d		debug output of parsed tokens
//
//	(policy pointer/deep pointer STL container support)
//	-s		policy pointer - suppress int value test unit (for value container, default)
//	+s		policy pointer - write int key/value test unit (for key-value container: <int,dp> and <dp,int>)
//
// Test code template tags;
//
//	- unit -
//
//	$W"subtitle"${ ... code ... $}		test for various keys and values
//	$V"subtitle"${ ... code ... $}		test for various values
//	$K"subtitle"${ ... code ... $}		test for various keys
//	$U"subtitle"${ ... code ... $}		test unit without key and value variation
//
//	$X${ ... code ... $}			auxiliary code for top level
//
//	- code -
//
//	$a		target container type with template argument ( "dp_xxx<T*>", "rp_xxx<T*>", std::xxx )
//	$i		target container template id ( "dp_xxx", "dr_xxx", std::xxx )
//	$p		target mode character ( "d", "r", "n" )
//
//	$t		test value data type ( "test*", "const char*" )
//	$c		test value const data type ( "const test*", "const char*" )
//	$v		test value native data type ( "test[3]", "const char[]" )
//	$k		test value const native data type ( "const test[3]", "const char[]" )
//	$0 .. $9	test value data ( 's1'/'&t1'/'v1'/'w1'/'1' ... defined in udata.hpp )
//	$n		test value null
//
//	$d tkn		duplicate if store policy value
//	$d$" ... $"
//	$d${ ... code ... $}
//
//	@t		test key data type ( "test*", "const char*" )
//	@v		test key native data type ( "test[3]", "const char[]" )
//	@0 .. @9	test key data   ( 's1'/'&t1'/'v1'/'w1'/'1' ... defined in udata.hpp )
//	@n		test key null
//
//	@d tkn		duplicate if store policy key (depreciated)
//	@d$" ... $"
//	@d${ ... code ... $}
//
//	- policy combination loop code -
//
//	$P${ ... code ... $}	policy combination loop of cross-policy assignment
//
//	$b		auxiliary container type with template argument
//	$j		auxiliary container template id
//	$q		auxiliary mode character ( "d", "r" ):
//
//	- conditional code - 
//
//	$R${ ... code ... $}	code if value is refer pointer
//	$D${ ... code ... $}	code if value is deep pointer
//	$N${ ... code ... $}	code if value is non-pointer data
//
//	$Z${ ... code ... $}	code if value is 'const char[]'
//	$T${ ... code ... $}	code if value is 'test*'
//	$A${ ... code ... $}	code if value is 'test[]'
//	$F${ ... code ... $}	code if value is 'test[3]'
//	$I${ ... code ... $}	code if value is 'int'
//
//	@R${ ... code ... $}	code if key is refer pointer
//	@D${ ... code ... $}	code if key is deep pointer
//	@N${ ... code ... $}	code if key is non-pointer data
//
//	@Z${ ... code ... $}	code if key is 'const char[]'
//	@T${ ... code ... $}	code if key is 'test*'
//	@A${ ... code ... $}	code if key is 'test[]'
//	@F${ ... code ... $}	code if key is 'test[3]'
//	@I${ ... code ... $}	code if key is 'int'
//
//	$&			conditional sequence (neglect following text constant)
//

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <pctl/plist.h>

using namespace pctl;

//
// --- specification ---
//

const size_t	LINESIZE = 1028;
const int	Indent = 8;

// token type
// ------------------------------------------------------------------
enum type {
	tk_error = -1,
	
	tk_constant = 0,			// str

	tk_begin,
	tk_end,

	tk_target_container,
	tk_auxiliary_container,
	tk_target_template,
	tk_auxiliary_template,

	tk_unit,				// (lst)
	tk_value_unit,				// (lst)
	tk_key_unit,				// (lst)
	tk_key_value_unit,			// (lst)
	tk_auxiliary,				// (lst)
	tk_assginable,				// (lst)

	// token type (value)	
	tk_value_type,
	tk_const_value_type,
	tk_native_value_type,
	tk_const_native_value_type,
	tk_value_data,				// chr: '0'..'9'
	tk_value_null,

	tk_value_mode_char,
	tk_value_auxiliary_mode_char,

	tk_value_duplicate_if,			// tkn

	// token type (key)	
	tk_key_type,
	tk_native_key_type,
	tk_key_data,				// chr: '0'..'9'
	tk_key_null,

	tk_key_mode_char,
	tk_key_auxiliary_mode_char,

	tk_key_duplicate_if,			// tkn

	// conditional type
	tk_conditional,				// (lst)

	tk_if_value_rptr,
	tk_if_value_dptr,
	tk_if_value_data,

	tk_if_value_cstring,
	tk_if_value_pointer,
	tk_if_value_varray,
	tk_if_value_farray,
	tk_if_value_int,

	tk_if_key_rptr,
	tk_if_key_dptr,
	tk_if_key_data,

	tk_if_key_cstring,
	tk_if_key_pointer,
	tk_if_key_varray,
	tk_if_key_farray,
	tk_if_key_int,

	tk_conditional_sequence			 // Note: it must be defined at the end of conditional tokens
};

// unit type
// ------------------------------------------------------------------
const int ut_auxiliary	= 0x0000;
const int ut_test	= 0x0010;
const int ut_key	= 0x0011;
const int ut_value	= 0x0012;
const int ut_key_value	= 0x0013;
	
// token subtype of test data (for tk_conditional)
// ------------------------------------------------------------------
const int st_value_cstring = 0x00001;
const int st_value_pointer = 0x00002;
const int st_value_varray  = 0x00004;
const int st_value_farray  = 0x00008;
const int st_value_int     = 0x00010;
const int st_value_mask    = 0x0001F;

const int st_key_cstring   = 0x00040;
const int st_key_pointer   = 0x00080;
const int st_key_varray    = 0x00100;
const int st_key_farray    = 0x00200;
const int st_key_int       = 0x00480; 
const int st_key_mask      = 0x007C0;

const int st_value_rptr    = 0x01000;
const int st_value_dptr    = 0x02000;
const int st_value_data    = 0x04000;
const int st_valptr_mask   = 0x07000;

const int st_key_rptr      = 0x10000;
const int st_key_dptr      = 0x20000;
const int st_key_data      = 0x40000;
const int st_keyptr_mask   = 0x70000;

// for generator/writer
// --------------------------------------------------------------------------------
typedef int data;

const data dt_null = 0;
const data dt_pointer = 1;
const data dt_varray = 2;
const data dt_farray = 3;
const data dt_cstring = 4;
const data dt_int = 5;
const data dt_end = 6;

typedef int mode;

const mode md_null = 0;
const mode md_rptr = 1;
const mode md_dptr = 2;
const mode md_data = 3;
const mode md_end = 4;

// utilities
// --------------------------------------------------------------------------------
inline const char* token_name( type tk )
{
	const char* r = nullptr;

	if      ( tk == tk_constant ) r = "constant";
	else if ( tk == tk_begin ) r = "begin";
	else if ( tk == tk_end ) r = "end";

	else if ( tk == tk_unit ) r = "unit";
	else if ( tk == tk_value_unit ) r = "value_unit";
	else if ( tk == tk_key_unit ) r = "key_unit";
	else if ( tk == tk_key_value_unit ) r = "key_value_unit";
	else if ( tk == tk_auxiliary ) r = "auxiliary";
	else if ( tk == tk_conditional ) r = "conditional";

	else if ( tk == tk_target_container ) r = "target_container";
	else if ( tk == tk_auxiliary_container ) r = "auxiliary_container";
	else if ( tk == tk_target_template ) r = "target_template";
	else if ( tk == tk_auxiliary_template ) r = "auxiliary_template";

	else if ( tk == tk_value_type ) r = "value_type";
	else if ( tk == tk_const_value_type ) r = "const_value_type";
	else if ( tk == tk_native_value_type ) r = "native_value_type";
	else if ( tk == tk_const_native_value_type ) r = "const_native_value_type";
	else if ( tk == tk_value_data ) r = "value_data";
	else if ( tk == tk_value_null ) r = "value_null";

	else if ( tk == tk_value_mode_char ) r = "value_mode_character";
	else if ( tk == tk_value_auxiliary_mode_char ) r = "value_auxiliary_mode_character";

	else if ( tk == tk_value_duplicate_if ) r = "value_duplicate_if";

	else if ( tk == tk_key_type ) r = "key_type";
	else if ( tk == tk_native_key_type ) r = "native_key_type";
	else if ( tk == tk_key_data ) r = "key_data";
	else if ( tk == tk_key_null ) r = "key_null";

	else if ( tk == tk_key_mode_char ) r = "key_mode_character";
	else if ( tk == tk_key_auxiliary_mode_char ) r = "key_auxiliary_mode_character";

	else if ( tk == tk_key_duplicate_if ) r = "key_duplicate_if";

	return r ? r : "error";
}

// --------------------------------------------------------------------------------
inline const char* data_type( data dt )
{
	return dt == dt_pointer	? "test*" :
	       dt == dt_varray	? "test*" :
	       dt == dt_farray	? "test*" :
	       dt == dt_cstring	? "const char*" : 
	       dt == dt_int	? "int" : "ERROR";
}

inline const char* data_ntype( data dt )
{
	return dt == dt_pointer	? "test*" :
	       dt == dt_varray	? "test[]" :
	       dt == dt_farray	? "test[3]" :
	       dt == dt_cstring	? "const char[]" : 
	       dt == dt_int	? "int" : "ERROR";
}

inline const char* data_char( data dt )
{
	return dt == dt_pointer	? "&t" :
	       dt == dt_varray	? "v" :
	       dt == dt_farray	? "w" :
	       dt == dt_cstring	? "s" : 
	       dt == dt_int	? "" : "ERROR";
}

inline const char* data_literal( data dt )
{
	return dt == dt_pointer	? "pointer" :
	       dt == dt_varray	? "v-array" :
	       dt == dt_farray	? "f-array" :
	       dt == dt_cstring	? "c-string" : 
	       dt == dt_int	? "int value" : "ERROR";
}

inline const char* mode_char( mode md )
{
	return md == md_rptr ? "r" :
	       md == md_dptr ? "p" :
	       md == md_data ? "n" : "ERROR";
}

inline const char* mode_literal( mode md )
{
	return md == md_rptr ? "reference pointer" :
	       md == md_dptr ? "deep pointer" :
	       md == md_data ? "data" : "ERROR";
}

// data classes
// --------------------------------------------------------------------------------
class token
{
    public:

    protected:
	type	m_type;

    public:
	explicit token( type t ) : m_type( t ) { }
	token( const token& ) = delete;
	token& operator=( const token& ) = delete;
	virtual ~token() = default;

	operator bool() { return int( m_type ) >= 0; }

	virtual void store( token* t ) { }
	virtual void set( int f ) { }
	virtual void end( token* t ) { }

	virtual int chr() const { return -1; }
	virtual const char* str() const { return nullptr; }
	virtual token* get() { return nullptr; }
	virtual bool check( mode vp, data vt, mode kp, data kt ) const { return false; }

	// for debug
	virtual void print( unsigned int lv = 0 ) const
	{
		fprintf( stderr, "%*.stoken %s\n", lv, "", token_name( m_type ));
	}
	const char* name() const { return token_name( m_type ); }

	// token type checker
	// ------------------------------------------------------------------
	bool is( type t ) const { return t == m_type; }
	bool is_conditional() const { return tk_conditional <= m_type && m_type < tk_conditional_sequence; }
	bool is_duplicate() const { return m_type == tk_value_duplicate_if || m_type == tk_key_duplicate_if; }
};

// --------------------------------------------------------------------------------
class token_chr : public token
{
	int	m_int;

    public:
	token_chr( type t, int d ) : token( t ), m_int( d ) { }
	
	int chr() const override { return m_int; }
	bool check( mode vp, data vt, mode kp, data kt ) const override
	{
		bool r = true;

		if (( m_int & st_keyptr_mask ) != 0 ) {
			if ( kp == md_rptr )	r = ( m_int & st_key_rptr ) != 0;
			if ( kp == md_dptr )	r = ( m_int & st_key_dptr ) != 0;
			if ( kp == md_data )	r = ( m_int & st_key_data ) != 0;
		}
		if (( m_int & st_valptr_mask ) != 0 ) {
			if ( vp == md_rptr )	r = ( m_int & st_value_rptr ) != 0;
			if ( vp == md_dptr )	r = ( m_int & st_value_dptr ) != 0;
			if ( vp == md_data )	r = ( m_int & st_value_data ) != 0;
		}
		if (( m_int & st_key_mask ) != 0 ) {
			if ( kt == dt_cstring ) r = ( m_int & st_key_cstring ) != 0;
			if ( kt == dt_pointer ) r = ( m_int & st_key_pointer ) != 0;
			if ( kt == dt_varray )  r = ( m_int & st_key_varray )  != 0;
			if ( kt == dt_farray )  r = ( m_int & st_key_farray )  != 0;
			if ( kt == dt_int )     r = ( m_int & st_key_int )     != 0;
		}
		if (( m_int & st_value_mask ) != 0 ) {
			if ( vt == dt_cstring ) r = ( m_int & st_value_cstring ) != 0; 
			if ( vt == dt_pointer ) r = ( m_int & st_value_pointer ) != 0;
			if ( vt == dt_varray )  r = ( m_int & st_value_varray )  != 0;
			if ( vt == dt_farray )  r = ( m_int & st_value_farray )  != 0;
			if ( vt == dt_int )     r = ( m_int & st_value_int )     != 0;
		}
		return r;
	}

	// for debug
	void print( unsigned int lv = 0 ) const override
	{
		fprintf( stderr, "%*.stoken_chr %s", lv, "", token_name( m_type ));

		if ( m_type == tk_conditional ) 
			fprintf( stderr, "[%X,%X]\n", ( m_int & st_key_mask ) >> 5, m_int & st_value_mask );
		else 
			fprintf( stderr, "(%c)\n", m_int );
	}
};

// --------------------------------------------------------------------------------
class token_str : public token
{
	char*	m_str;

    public:
	token_str( type t, const char* s ) : token( t ), m_str( strdup( s )) { }
	const char* str() const override { return m_str; }
	~token_str() { delete [] m_str; }

	// for debug
	void print( unsigned int lv = 0 ) const override
	{
		fprintf( stderr, "%*.stoken_str %s \"%s\"\n", lv, "", token_name( m_type ), m_str );
	}
};

// --------------------------------------------------------------------------------
class token_dup : public token
{
	token*	m_tkn;

    public:
	token_dup( type t ) : token( t ), m_tkn( nullptr ) { }
	~token_dup() { delete m_tkn; } 
	
	void store( token* p ) override { if ( m_tkn ) delete m_tkn; m_tkn = p; }
	token* get() override { return m_tkn; }

	// for debug
	void print( unsigned int lv = 0 ) const override
	{
		fprintf( stderr, "%*.stoken_dup %s\n", lv, "", token_name( m_type ));
		m_tkn->print( lv + 1 );
	}
};

// --------------------------------------------------------------------------------
class token_lst : public token
{
    protected:
	pslist<token*>			m_lst;
	token*				m_end;
	char*				m_sub;
	int				m_ut;
	pslist<token*>::iterator	m_i;

    public:
	explicit token_lst( const char* s = nullptr )
		: token( tk_begin ), m_end( nullptr ), m_sub( s ? strdup( s ) : nullptr ),
		  m_ut( ut_auxiliary ), m_i( m_lst.begin())
	{ }
	~token_lst()
	{
		if ( m_end ) delete m_end;
		if ( m_sub ) delete [] m_sub;
	} 

	void store( token* p ) override { m_lst.enq( p ); }
	void end( token* p ) override   { m_end = p; }

	void init() { m_i = m_lst.begin(); } 
	token* next() { return m_i ? *m_i++ : nullptr; }
	token* get() override { return m_i ? *m_i : nullptr; }
	const char* subtitle() const { return m_sub; }

	void set_type( int ut ) { m_ut = ut; }
	bool is_test() const { return ( m_ut & ut_test ) == ut_test; }
	bool key_test() const { return ( m_ut & ut_key ) == ut_key; }
	bool value_test() const { return ( m_ut & ut_value ) == ut_value; }

	// for debug
	void print( unsigned int lv = 0 ) const override
	{
		fprintf( stderr, "%*.stoken_lst %s", lv, "", token_name( m_type ));
		if ( m_sub ) fprintf( stderr, "(%s)", m_sub );
		fprintf( stderr, " {\n" );
		for ( auto i = m_lst.begin(); i != m_lst.end(); ++i ) ( *i )->print( lv + 2 );
		if ( m_end ) fprintf( stderr, "%*.s} end\n", lv, "" );
	}
};

// process classes
// --------------------------------------------------------------------------------
class parser
{
	FILE*				m_in;
	unsigned			m_line;
	char				m_buf[ LINESIZE ];
	pslist<token_lst*>		m_units;
	pslist<token_lst*>::iterator	m_i;
	token*				m_unget;

    public:
	parser( const char* fn ) : m_in( fopen( fn, "r" )), m_line( 1 ), m_unget( nullptr )
	{
		m_buf[ 0 ] = '\0';

		if ( m_in == nullptr ) {
			error( "file %s cannot open for input", fn );
			exit( 1 );
		}
	}
	parser( parser const& ) = delete;
	parser operator=( parser const& ) = delete;
	~parser()
	{
		if ( m_unget ) delete m_unget;
		fclose( m_in );
	}

	operator bool() const { return m_in && !feof( m_in ); }

	// tokenizer
	token* get_token( const char* subtitle = nullptr );

	// parser
	void parse();

	// retreival	
	void init() 	 { m_i = m_units.begin(); }
	token_lst* get() { return m_i ? *m_i++ : nullptr; }
	const pslist<token_lst*>& units() const { return m_units; }

	unsigned unit_count() const { return m_units.size(); }

	// for debug
	void print() const
	{
		for ( auto i = m_units.begin(); i != m_units.end(); ++i ) ( *i )->print();
	}

    private:
	token* get_value( const char* subtitle );
	token* get_key();
	token* get_const( int a );
	token* get_expression();
	const char* get_subtitle();
	void unget_token( token* p )
	{
		if ( m_unget ) delete m_unget;
		m_unget = p;
	}

	token_lst* parse_unit( int ut );
	token_lst* parse_auxiliary();
	token* parse_block( token* p );
	token* parse_conditional( token* p );
	token* parse_duplicate( token* p );

	void error( const char* fmt, ... );
};

// --------------------------------------------------------------------------------
class writer
{
	FILE*		m_out;
	const char*	m_fn;
	const char*	m_pcontainer;
	int		m_indent;
	int		m_testno;

    public:
	writer( const char* pc, const char* fn = nullptr, int stl = 0 )
		: m_out( fn ? fopen( fn, "w" ) : stdout ), m_fn( fn ), m_pcontainer( pc ), m_indent( 0 ), m_testno( 0 )
	{
		if ( m_out == nullptr ) {
			fprintf( stderr, "Error: file %s cannot open for output\n", fn );
			exit( 1 );
		}
	}
	~writer() { if ( m_out != stdout ) fclose( m_out ); }

	void write_header( const pslist<token_lst*>& us, bool keyed );
	void write_footer();

	void write_unit( token_lst* ut, mode vp = md_null, data vt = dt_null, mode kp = md_null, data kt = dt_null );
	void write_auxiliary( token_lst* ut );

    private:
	void write_block( token_lst* ut, mode vp, data vt, mode kp, data kt );
	void write_token( token* ut, mode vp, data vt, mode kp, data kt, mode xp = md_null, mode yp = md_null );

	void write_target_container( mode vp, data vt, mode kp, data kt );
	void write_target_template( mode vp, mode kp );
	void write_mode_char( mode m );

	void write_constant( const char* s );
	void write_type( data vt );
	void write_const_type( data vt );
	void write_native_type( data vt );
	void write_const_native_type( data vt );
	void write_data( data vt, int n );
	void write_null( data vt );

	void indent() { for ( int i = 0; i < m_indent; ++i ) putc( '\t', m_out ); }
	int test_number() { return ++m_testno; }
};

// --------------------------------------------------------------------------------
class generator
{
	parser	m_p;
	writer	m_w;
	bool	m_keyed;
	int	m_stl;

    public:
	generator( const char* pc, const char* in, const char* out = nullptr, int stl = 0 )
		: m_p( in ), m_w( pc, out, stl ), m_stl( stl )
	{
		m_keyed = ( strcmp( pc, "dp_multimap"           ) == 0 ||
			    strcmp( pc, "dp_map"                ) == 0 ||
			    strcmp( pc, "dp_unordered_multimap" ) == 0 ||
			    strcmp( pc, "dp_unordered_map"      ) == 0 );
	}

	void generate();
	void key_value_test();
	void value_test();
};

//
// --- implementation ---
// 

// parser - tokenize
// --------------------------------------------------------------------------------
token* parser::get_token( const char* subtitle )
{
	token*	r = nullptr;
	int	a;

	if ( m_unget ) {
		r = m_unget;
		m_unget = nullptr;
	} else if ( 0 <= ( a = getc( m_in ))) {
		if 	( a == '$' )	r = get_value( subtitle );
		else if ( a == '@' )	r = get_key();
		else 			r = get_const( a );
	}
	return r;
}

// --------------------------------------------------------------------------------
//      ${		tk_begin
//      $}		tk_end
//
//      $a		tk_target_container
//	$b		tk_auxiliary_container
//      $i		tk_target_template
//	$j		tk_auxiliary_template
//
//      $t		tk_value_type
//      $c		tk_const_value_type
//      $v		tk_native_value_type
//      $k		tk_const_native_value_type
//      $0 .. $9	tk_value_data (chr)
//      $n		tk_value_null
//
//      $p		tk_value_mode_char
//	$q		tk_value_auxiliary_mode_char
//
//      $d tkn		tk_value_duplicate_if (tkn)
//
//	$U		tk_unit
//      $K		tk_key_unit
//      $V		tk_value_unit
//      $W		tk_key_value_unit
//	$X		tk_auxiliary
//      $P		tk_assginable
//      $Z		tk_if_value_cstring
//      $T		tk_if_value_pointer
//      $A		tk_if_value_varray
//      $F		tk_if_value_farray
//      $I		tk_if_value_int
//
//	$R		tk_if_value_rptr
//	$D		tk_if_value_dptr
//	$N		tk_if_value_data
//
//	$&		tk_conditional_sequence
//
token* parser::get_value( const char* subtitle )
{
	token*	r = nullptr;

	switch ( int a = getc( m_in )) {
		case '{': r = new token_lst( subtitle ); break;
		case '}': r = new token( tk_end ); break;

		case '"': r = get_expression(); break;

		case 'a': r = new token( tk_target_container ); break;
		case 'b': r = new token( tk_auxiliary_container ); break;
		case 'i': r = new token( tk_target_template ); break;
		case 'j': r = new token( tk_auxiliary_template ); break;

		case 't': r = new token( tk_value_type ); break;
		case 'c': r = new token( tk_const_value_type ); break;
		case 'v': r = new token( tk_native_value_type ); break;
		case 'k': r = new token( tk_const_native_value_type ); break;
		case '0': 
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': r = new token_chr( tk_value_data, a ); break;
		case 'n': r = new token( tk_value_null ); break;

		case 'p': r = new token( tk_value_mode_char ); break;
		case 'q': r = new token( tk_value_auxiliary_mode_char ); break;

		case 'd': r = new token_dup( tk_value_duplicate_if ); break;

		case 'U': r = new token( tk_unit ); break;
		case 'K': r = new token( tk_key_unit ); break;
		case 'V': r = new token( tk_value_unit ); break;
		case 'W': r = new token( tk_key_value_unit ); break;
		case 'X': r = new token( tk_auxiliary ); break;
		case 'Z': r = new token( tk_if_value_cstring ); break;
		case 'T': r = new token( tk_if_value_pointer ); break;
		case 'A': r = new token( tk_if_value_varray ); break;
		case 'F': r = new token( tk_if_value_farray ); break;
		case 'I': r = new token( tk_if_value_int ); break;

		case 'R': r = new token( tk_if_value_rptr ); break;
		case 'D': r = new token( tk_if_value_dptr ); break;
		case 'N': r = new token( tk_if_value_data ); break;

		case '&': r = new token( tk_conditional_sequence ); break;

		default:
			error( "illegal value tag $'%c'", a );
			break;
	}
	return r;
}

// --------------------------------------------------------------------------------
//      @t		tk_key_type
//      @v		tk_native_key_type
//      @0 .. @9	tk_key_data (chr)
//      @n		tk_key_null
//
//      @p		tk_key_mode_char
//	@q		tk_key_auxiliary_mode_char
//
//      @d tkn		tk_key_duplicate_if (tkn)
//
//      @Z		tk_if_key_cstring
//      @T		tk_if_key_pointer
//      @A		tk_if_key_varray
//      @F		tk_if_key_farray
//      @I		tk_if_key_int
//
//	@R		tk_if_key_rptr
//	@D		tk_if_key_dptr
//	@N		tk_if_key_data
//
token* parser::get_key()
{
	token*	r = nullptr;

	switch ( int a = getc( m_in )) {
		case 't': r = new token( tk_key_type ); break;
		case 'v': r = new token( tk_native_key_type ); break;
		case '0': 
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': r = new token_chr( tk_key_data, a ); break;
		case 'n': r = new token( tk_key_null ); break;

		case 'p': r = new token( tk_key_mode_char ); break;
		case 'q': r = new token( tk_key_auxiliary_mode_char ); break;

		case 'd': r = new token_dup( tk_key_duplicate_if ); break;

		case 'Z': r = new token( tk_if_key_cstring ); break;
		case 'T': r = new token( tk_if_key_pointer ); break;
		case 'A': r = new token( tk_if_key_varray ); break;
		case 'F': r = new token( tk_if_key_farray ); break;
		case 'I': r = new token( tk_if_key_int ); break;

		case 'R': r = new token( tk_if_key_rptr ); break;
		case 'D': r = new token( tk_if_key_dptr ); break;
		case 'N': r = new token( tk_if_key_data ); break;

		default:
			error( "illegal key tag @'%c'", a );
			break;
	}
	return r;
}

// --------------------------------------------------------------------------------
token* parser::get_const( int a )
{
	bool	b = true;
	char*	p = m_buf;
	char*	e = m_buf + LINESIZE - 1;

	while ( p < e ) {
		if	( a < 0 ) break;
		else if ( a == '$' || a == '@' ) { ungetc( a, m_in ); break; }
		else if ( a == '\n' )		 { if ( b ) p = m_buf; *p++ = a; ++m_line; break; }
		else if ( a != ' ' && a != '\t' ) b = false;
		*p++ = a;
		a = getc( m_in );
	}
	*p = '\0';

	token*	r = new token_str( tk_constant, m_buf );

	m_buf[ 0 ] = '\0';

	return r;
}

// --------------------------------------------------------------------------------
token* parser::get_expression()
{
	int	a = getc( m_in );
	token*	r = get_const( a );

	if ( getc( m_in ) != '$' || getc( m_in ) != '"' ) {
		error( "illeal expression" );
		delete r;
		r = nullptr;
	}
	return r;	
}

// --------------------------------------------------------------------------------
const char* parser::get_subtitle()
{
	int	a = getc( m_in );
	int	t = '\n';
	char*	p = m_buf;
	char*	e = m_buf + LINESIZE - 1;

	while ( a == ' ' || a == '\t' ) a = getc( m_in );

	if ( a == '"' || a == '\'' ) { t = a; a = getc( m_in ); } 

	while ( p < e ) {
		if ( a == t ) {
			if ( a == '\n' ) ++m_line;
			while ( int a = getc( m_in )) {
				if ( a == '\n' ) ++m_line;
				if ( a != ' ' && a != '\t' && a != '\n' ) { ungetc( a, m_in ); break; }
			}
			break;
		}
		else if ( a == '$' && t == '\n' ) { ungetc( a, m_in ); break; }
		else if ( a < 0 )		  { error( "unterminated subtitle" ); break; }

		*p++ = a;
		a = getc( m_in );
	}
	*p = '\0';

	return m_buf;
}

// parser - parse
// --------------------------------------------------------------------------------
void parser::parse()
{
	m_units.clear();
	m_line = 1;

	while ( token* p = get_token()) {
		if      ( p->is( tk_unit ))		m_units.enq( parse_unit( ut_test ));
		else if ( p->is( tk_key_unit ))		m_units.enq( parse_unit( ut_key ));
		else if ( p->is( tk_value_unit ))	m_units.enq( parse_unit( ut_value ));
		else if ( p->is( tk_key_value_unit ))	m_units.enq( parse_unit( ut_key_value ));
		else if ( p->is( tk_auxiliary ))	m_units.enq( parse_auxiliary());
		else if ( p->is( tk_constant ))		delete p;
		else error( "parse: unit $U required" );
	}
}

// --------------------------------------------------------------------------------
token_lst* parser::parse_unit( int ut )
{
	token_lst*	r;

	if ( r = dynamic_cast<token_lst*>( get_token( get_subtitle()))) {
		token* p;

		while ( p = get_token()) {
			if      ( p->is( tk_begin )) { r->store( parse_block( p )); }
			else if ( p->is( tk_end )) { r->end( p ); break; }
			else if ( p->is( tk_unit ) || p->is( tk_key_unit ) || p->is( tk_value_unit ) ||
				  p->is( tk_key_value_unit )) { error( "parse_unit: unit in unit" ); delete p; }
			else if ( p->is_conditional()) { r->store( parse_conditional( p )); r->store( parse_block( get_token())); }
			else if ( p->is_duplicate()) { r->store( parse_duplicate( p )); }
			else if ( p->is( tk_conditional_sequence )) { 
				p = get_token();
				if ( !p->is( tk_constant )) unget_token( p );
			}
			else r->store( p );
		}
		if ( p == nullptr ) error( "parse_unit: unterminated unit, $} required" );
	} else {
		error( "parse_unit: ${ required" );
		exit( -1 );
	}
	r->set_type( ut );

	return r;
}

// --------------------------------------------------------------------------------
token_lst* parser::parse_auxiliary()
{
	token_lst*	r;

	if ( r = dynamic_cast<token_lst*>( get_token() )) {
		while ( token* p = get_token()) {
			if      ( p->is( tk_constant )) r->store( p );
			else if ( p->is( tk_end )) { r->end( p ); break; }
			else			   delete p;
		}
	} else {
		error( "parse_auxiliary: ${ required" );
		exit( -1 );
	}
	return r;
}

// --------------------------------------------------------------------------------
token* parser::parse_block( token* r )
{
	if ( r->is( tk_begin )) {
		token* p;

		while ( p = get_token()) {
			if      ( p->is( tk_begin )) { r->store( parse_block( p )); }
			else if ( p->is( tk_end )) { r->end( p ); break; }
			else if ( p->is( tk_unit ) || p->is( tk_key_unit ) || p->is( tk_value_unit ) ||
				  p->is( tk_key_value_unit )) { error( "parse_block: unit in block" ); delete p; }
			else if ( p->is_conditional()) { r->store( parse_conditional( p )); r->store( parse_block( get_token())); }
			else if ( p->is_duplicate()) { r->store( parse_duplicate( p )); }
			else if ( p->is( tk_conditional_sequence )) { 
				p = get_token();
				if ( !p->is( tk_constant )) unget_token( p );
			}
			else r->store( p );
		}
		if ( p == nullptr ) error( "parse_repeat: unterminated repeat, $} required" );
	} else {
		error( "parse_block: ${ required" );
		r->print();
		exit( -1 );
	}
	return r;
}

// --------------------------------------------------------------------------------
token* parser::parse_duplicate( token* r )
{
	if ( token* p = get_token()) {
		if ( p->is( tk_begin )) r->store( parse_block( p )); 
		else			r->store( p );
	}
	return r;
}

// --------------------------------------------------------------------------------
token* parser::parse_conditional( token* p )
{
	int	f = 0;

	while ( p ) {
		if      ( p->is( tk_if_value_rptr ))	f |= st_value_rptr;
		else if ( p->is( tk_if_value_dptr ))	f |= st_value_dptr;
		else if ( p->is( tk_if_value_data ))	f |= st_value_data;

		else if ( p->is( tk_if_key_rptr ))	f |= st_key_rptr;
		else if ( p->is( tk_if_key_dptr ))	f |= st_key_dptr;
		else if ( p->is( tk_if_key_data ))	f |= st_key_data;

		else if ( p->is( tk_if_value_cstring ))	f |= st_value_cstring;
		else if ( p->is( tk_if_value_pointer ))	f |= st_value_pointer; 
		else if ( p->is( tk_if_value_varray ))	f |= st_value_varray; 
		else if ( p->is( tk_if_value_farray ))	f |= st_value_farray;
		else if ( p->is( tk_if_value_int ))	f |= st_value_int;  

		else if ( p->is( tk_if_key_cstring ))	f |= st_key_cstring;
		else if ( p->is( tk_if_key_pointer ))	f |= st_key_pointer;
		else if ( p->is( tk_if_key_varray ))	f |= st_key_varray; 
		else if ( p->is( tk_if_key_farray ))	f |= st_key_farray;
		else if ( p->is( tk_if_key_int ))	f |= st_key_int;

		else { unget_token( p ); break; }

		delete p;
		p = get_token();
	}
	return new token_chr( tk_conditional, f );
}

// --------------------------------------------------------------------------------
void parser::error( const char* msg, ... )
{
	va_list		args;

	va_start( args, msg );
	fprintf( stderr, "Parse Error (%u): ", m_line );
	vfprintf( stderr, msg, args );
	fprintf( stderr, "\n" );
	va_end( args );
}

// writer
// --------------------------------------------------------------------------------
void writer::write_header( const pslist<token_lst*>& us, bool keyed )
{
	char		date[ 16 ];
	time_t		t = time( nullptr );
	unsigned	c = 0;		// test counter

	strftime( date, 16, "%Y/%m/%d", localtime( &t ));

	fprintf( m_out, "//\n" );
	fprintf( m_out, "// %s test\n", m_fn ? m_fn : m_pcontainer );
	fprintf( m_out, "//\n" );
	fprintf( m_out, "//\t\tgenerated by gendptest, %s\n", date );
	fprintf( m_out, "//\n\n" );
	fprintf( m_out, "#include <dptl/%s.hpp>\n",
			strcmp( m_pcontainer, "dp_multimap" ) == 0 ? "dp_map" :
			strcmp( m_pcontainer, "dp_multiset" ) == 0 ? "dp_set" : 
			strcmp( m_pcontainer, "dp_unordered_multimap" ) == 0 ? "dp_unordered_map" : 
			strcmp( m_pcontainer, "dp_unordered_multiset" ) == 0 ? "dp_unordered_set" : m_pcontainer );
	fprintf( m_out, "#include <dptl/utest.hpp>\n" );
	fprintf( m_out, "#include <dptl/udata.hpp>\n" );
	fprintf( m_out, "#include <cstring>\n\n" );
	fprintf( m_out, "using namespace dptl;\n\n" );
	fprintf( m_out, "// Unit Test Classes\n" );
	fprintf( m_out, "// -----------------------------------------------------------------------------\n" );
	fprintf( m_out, "class %stest : public unittest, public basic_testdata\n", m_pcontainer );
	fprintf( m_out, "{\n" );
	fprintf( m_out, "\tint\t\tm_cnt;\n\n" );
	fprintf( m_out, "    public:\n" );

	for ( auto u = us.cbegin(); u != us.cend(); ++u ) {
		unsigned		ts = 0;

		if ((*u)->is_test()) ts += 1;
		if ( keyed ) {
			if ((*u)->value_test() && (*u)->key_test()) ts += 3;
			if ((*u)->value_test()) ts += 7;
			if ((*u)->key_test()) ts += 7;
		} else {
			if ((*u)->value_test()) ts += 8;
		}
		if ( 0 < ts ) {
			if ( const char* sb = (*u)->subtitle()) fprintf( m_out, "\t// --- %s ---\n", sb );
			for ( unsigned j = 0; j < ts; ++j ) fprintf( m_out, "\tvoid test%02.2X();\n", ++c );
			fprintf( m_out, "\n" );
		}
	}

	fprintf( m_out, "\t%stest( int argc, char** argv ) : unittest( argc, argv )\n", m_pcontainer );
	fprintf( m_out, "\t{\n" );
	fprintf( m_out, "\t\tm_cnt = test::getCntr();\n" );
	fprintf( m_out, "\t\tfprintf( stderr, \"*** test::getCntr() : %%d\\n\", m_cnt );\n" );
	fprintf( m_out, "\t}\n\n" );
	fprintf( m_out, "\t~%stest()\n", m_pcontainer );
	fprintf( m_out, "\t{\n" );
	fprintf( m_out, "\t\tthis->set_subtitle( \"test class allocation\" );\n" );
	fprintf( m_out, "\t\tAssertEqual( m_cnt, test::getCntr() );\n" );
	fprintf( m_out, "\t\tfprintf( stderr, \"\\n\" );\n" );
	fprintf( m_out, "\t}\n" );
	fprintf( m_out, "};\n" );
}

// --------------------------------------------------------------------------------
void writer::write_footer()
{
	fprintf( m_out, "\n" );
	fprintf( m_out, "// Main\n" );
	fprintf( m_out, "// -----------------------------------------------------------------------------\n" );
	fprintf( m_out, "int main( int argc, char** argv )\n" );
	fprintf( m_out, "{\n" );
	fprintf( m_out, "\t%stest( argc, argv ).run();\n", m_pcontainer );
	fprintf( m_out, "\treturn 0;\n" );
	fprintf( m_out, "}\n" );
}

// --------------------------------------------------------------------------------
void writer::write_unit( token_lst* ls, mode vp, data vt, mode kp, data kt )
{
	if ( const char* sb = ls->subtitle()) {
		fprintf( m_out, "\n// -----------------------------------------------------------------------------\n" );
		fprintf( m_out, "void %stest::test%02.2X()\n{\n", m_pcontainer, test_number());
		++m_indent;
		indent();
		fprintf( m_out, "this->set_subtitle( \"%s", sb );
		if ( kt ) fprintf( m_out, ", %s(%s) key", mode_literal( kp ), data_literal( kt )); 
		if ( vt ) fprintf( m_out, " - %s(%s) value", mode_literal( vp ), data_literal( vt )); 
		fprintf( m_out, "\" );\n" );
	} else {
		indent();
		fprintf( m_out, "{\n" );
		++m_indent;
	}

	write_block( ls, vp, vt, kp, kt );

	--m_indent;
	indent();
	fprintf( m_out, "}\n" );
}

// --------------------------------------------------------------------------------
void writer::write_auxiliary( token_lst* ls )
{
	ls->init();
	while ( token* tk = ls->next()) {
		if ( tk->is( tk_constant )) {
			write_constant( tk->str() );
		} else {
			fprintf( stderr, "Error: write_auxiliary - token %s is illegal\n", tk->name());
			exit( 1 );
		}
	}
}

// --------------------------------------------------------------------------------
void writer::write_block( token_lst* ls, mode vp, data vt, mode kp, data kt )
{
	ls->init();
	while ( token* tk = ls->next()) {
		if ( tk->is( tk_conditional )) {
			if ( token_lst* cd = dynamic_cast<token_lst*>( ls->next() )) {
				if ( tk->check( vp, vt, kp, kt )) write_block( cd, vp, vt, kp, kt );
			} else {
				fprintf( stderr, "Error: write: illegal conditional block\n" );
			}
		}
		else if ( tk->is( tk_value_duplicate_if )) {
			if ( vp == md_dptr && !tk->get()->is( tk_value_null )) {
				if ( vt == dt_cstring )	fprintf( m_out, "strdup( " );
				else			fprintf( m_out, "dpl( " );
				write_token( tk->get(), vp, vt, kp, kt );
				fprintf( m_out, " )" );
			} else {
				write_token( tk->get(), vp, vt, kp, kt );
			}
		}
		else if ( tk->is( tk_key_duplicate_if )) {
			if ( kp == md_dptr && !tk->get()->is( tk_key_null )) {
				if ( kt == dt_cstring )	fprintf( m_out, "strdup( " );
				else			fprintf( m_out, "dpl( " );
				write_token( tk->get(), vp, vt, kp, kt );
				fprintf( m_out, " )" );
			} else {
				write_token( tk->get(), vp, vt, kp, kt );
			}
		}
		else write_token( tk, vp, vt, kp, kt );
	}
}

// --------------------------------------------------------------------------------
void writer::write_token( token* tk, mode vp, data vt, mode kp, data kt, mode xp, mode yp )
{
	if      ( tk->is( tk_constant )) 		write_constant( tk->str() );

	else if	( tk->is( tk_target_container ))	write_target_container( vp, vt, kp, kt );
	else if ( tk->is( tk_auxiliary_container ))	write_target_container( xp, vt, yp, kt );
	else if	( tk->is( tk_target_template ))		write_target_template( vp, kp );
	else if ( tk->is( tk_auxiliary_template ))	write_target_template( xp, yp );

	// token type (value)
	else if ( tk->is( tk_value_type ))		write_type( vt );
	else if ( tk->is( tk_const_value_type )) 	write_const_type( vt );
	else if ( tk->is( tk_native_value_type ))	write_native_type( vt );
	else if ( tk->is( tk_const_native_value_type ))	write_const_native_type( vt );
	else if ( tk->is( tk_value_data ))		write_data( vt, tk->chr());
	else if ( tk->is( tk_value_null )) 		write_null( vt );
	
	// token type (key)
	else if ( tk->is( tk_key_type ))		write_const_type( kt ); 
	else if ( tk->is( tk_native_key_type ))		write_const_native_type( kt );
	else if ( tk->is( tk_key_data ))		write_data( kt, tk->chr());
	else if ( tk->is( tk_key_null ))		write_null( kt );

	// token mode (value)
	else if	( tk->is( tk_value_mode_char ))		write_mode_char( vp );
	else if ( tk->is( tk_value_auxiliary_mode_char )) write_mode_char( xp );

	// token mode (key)
	else if	( tk->is( tk_key_mode_char ))		write_mode_char( kp );
	else if ( tk->is( tk_key_auxiliary_mode_char )) write_mode_char( yp );

	// expressions
	else if ( tk->is( tk_begin )) {
		token_lst* ls = dynamic_cast<token_lst*>( tk );
		ls->init();
		while( token* ex = ls->next()) {
			if ( ex->is( tk_end )) break;
			write_token( ex, vp, vt, kp, kt );
		}
	}
	else {
		fprintf( stderr, "Error: write_token - token %s is illegal\n", tk->name());
		exit( 1 );
	}
}

// --------------------------------------------------------------------------------
void writer::write_target_container( mode vp, data vt, mode kp, data kt )
{
	if ( kp != md_null )
		fprintf( m_out, "d%s%s_%s<%s,%s>", mode_char( kp ), mode_char( vp ),
						  m_pcontainer + 3, data_ntype( kt ), data_ntype( vt ));
	else if ( vp != md_data )
		fprintf( m_out, "d%s_%s<%s>", mode_char( vp ), m_pcontainer + 3, data_ntype( vt ));
	else
		fprintf( m_out, "std::%s<%s>", m_pcontainer + 3, data_ntype( vt ));
}

// --------------------------------------------------------------------------------
void writer::write_target_template( mode vp, mode kp )
{
	if ( kp != md_null )
		fprintf( m_out, "d%s%s_%s", mode_char( kp ), mode_char( vp ), m_pcontainer + 3 );
	else if ( vp != md_data )
		fprintf( m_out, "d%s_%s", mode_char( vp ), m_pcontainer + 3 );
	else
		fprintf( m_out, "std::%s", m_pcontainer + 3 );
}

// --------------------------------------------------------------------------------
void writer::write_mode_char( mode m )		{ fprintf( m_out, "%s", mode_char( m )); }
void writer::write_constant( const char* s )	{ fprintf( m_out, "%s", s ); }
void writer::write_type( data vt )		{ fprintf( m_out, "%s", data_type( vt )); }
void writer::write_const_type( data vt )
{
	const char*	t = data_type( vt );

	if ( strncmp( t, "const", 5 ) == 0 || strncmp( t, "int", 3 ) == 0 )
		fprintf( m_out, "%s", t );
	else
		fprintf( m_out, "const %s", t );
}
void writer::write_native_type( data vt )	{ fprintf( m_out, "%s", data_ntype( vt )); } 
void writer::write_const_native_type( data vt )
{
	const char*	t = data_ntype( vt );

	if ( strncmp( t, "const", 5 ) == 0 || strncmp( t, "int", 3 ) == 0 )
		fprintf( m_out, "%s", t );
	else
		fprintf( m_out, "const %s", t );
}
void writer::write_data( data vt, int n )
{
	if ( vt == dt_int && n == '6' ) fprintf( m_out, "3" );
	else			      fprintf( m_out, "%s%c", data_char( vt ), n );
}
void writer::write_null( data vt )	   { fprintf( m_out, vt == dt_int ? "0" : "nullptr" ); }

// generator
// --------------------------------------------------------------------------------
void generator::generate()
{
	m_p.parse();

	m_p.init();

	m_w.write_header( m_p.units(), m_keyed );

	if ( m_keyed ) 
		key_value_test();
	else
		value_test();

	m_w.write_footer();
}

// --------------------------------------------------------------------------------
void generator::key_value_test()
{
	while ( token_lst* u = m_p.get()) {

		if ( u->value_test() || u->key_test()) {
			m_w.write_unit( u, md_rptr, dt_pointer, md_rptr, dt_pointer );		// 1 
			m_w.write_unit( u, md_rptr, dt_pointer, md_dptr, dt_pointer );		// 2 
			m_w.write_unit( u, md_dptr, dt_pointer, md_rptr, dt_pointer );		
			m_w.write_unit( u, md_dptr, dt_pointer, md_dptr, dt_pointer );		// 4

			if ( u->value_test()) {
				m_w.write_unit( u, md_rptr, dt_cstring, md_rptr, dt_cstring );	
				m_w.write_unit( u, md_dptr, dt_cstring, md_rptr, dt_cstring );
		
				m_w.write_unit( u, md_rptr, dt_varray, md_rptr, dt_cstring );
				m_w.write_unit( u, md_dptr, dt_varray, md_rptr, dt_cstring );
		
				m_w.write_unit( u, md_rptr, dt_farray, md_rptr, dt_cstring );
				m_w.write_unit( u, md_dptr, dt_farray, md_rptr, dt_cstring );
		
				m_w.write_unit( u, md_data, dt_int, md_rptr, dt_cstring );	// 11
			}	
			if ( u->key_test()) {
				m_w.write_unit( u, md_dptr, dt_pointer, md_rptr, dt_cstring );	// 12
				m_w.write_unit( u, md_dptr, dt_pointer, md_dptr, dt_cstring );	// 13

				m_w.write_unit( u, md_dptr, dt_pointer, md_rptr, dt_varray );
				m_w.write_unit( u, md_dptr, dt_pointer, md_dptr, dt_varray );

				m_w.write_unit( u, md_dptr, dt_pointer, md_rptr, dt_farray );
				m_w.write_unit( u, md_dptr, dt_pointer, md_dptr, dt_farray );
				
				m_w.write_unit( u, md_dptr, dt_pointer, md_data, dt_int );	// 18
			}

		} else if ( u->is_test()) {
			m_w.write_unit( u );
		} else {
			m_w.write_auxiliary( u );
		}
	}
}

// --------------------------------------------------------------------------------
void generator::value_test()
{
	while ( token_lst* u = m_p.get()) {

		if ( u->value_test()) {
			m_w.write_unit( u, md_rptr, dt_pointer );	// 1 
			m_w.write_unit( u, md_dptr, dt_pointer );	// 2 

			m_w.write_unit( u, md_rptr, dt_cstring );	// 3
			m_w.write_unit( u, md_dptr, dt_cstring );
	
			m_w.write_unit( u, md_rptr, dt_varray );
			m_w.write_unit( u, md_dptr, dt_varray );
	
			m_w.write_unit( u, md_rptr, dt_farray );
			m_w.write_unit( u, md_dptr, dt_farray );
	
			m_w.write_unit( u, md_data, dt_int );		// 9

		} else if ( u->is_test()) {
			m_w.write_unit( u );
		} else {
			m_w.write_auxiliary( u );
		}
	}
}

// --------------------------------------------------------------------------------
void usage()
{
	fprintf( stderr, "Usage: gendptest [-d][-s|+s][-p pcontainer] in-file [out-file]\n" );
}

// main
// --------------------------------------------------------------------------------
int main( int argc, char** argv )
{
	const char*	pc = nullptr;
	const char*	fin = nullptr;
	const char*	fout = nullptr;
	const char*	pcontainer[] = { "plist", "phash", "pmultimap", "pmap", "pmultiset", "pset", "pvector", nullptr };
	bool		debug = false;
	int		stl = 0;

	if ( 1 < argc ) {
		for ( int i = 1; i < argc; ++i ) {
			if ( *argv[ i ] == '-' ) {
				switch( argv[ i ][ 1 ] ) {
					case 'p':
						pc = argv[ ++i ];
						break;
					case 'd':
						debug = true;
						break;
					case 's':
						stl = -1;
						break;
					default:
						usage();
						return -1;
				}
			} else if ( *argv[ i ] == '+' ) {
				switch( argv[ i ][ 1 ] ) {
					case 's':
						stl = 1;
						break;
					default:
						usage();
						return -1;
				}
			} else {
				if ( fin ) fout = argv[ i ];
				else	   fin  = argv[ i ];
			}
		}
	}

	if ( fin == nullptr ) { usage(); return -1; }

	if ( debug ) {
		parser	p( fin );

		p.parse();
		p.print();
	} else {
		 if ( pc == nullptr ) {
			const char**	pp = pcontainer;
	
			while ( pc = *pp++ ) if ( strstr( fin, pc )) break;
	
			if ( pc == nullptr ) {
				fprintf( stderr, "Error: command argument '-p pcontainer' required\n" );
				exit( 1 );
			}
		}
		generator( pc, fin, fout, stl ).generate();
	}
	return 0;
}

