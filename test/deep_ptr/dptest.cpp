// test
// -----------------------------------------------------------------------------
#include <deque>
#include <set>
#include <dptl/deep_ptr.hpp>
#include <dptl/udata.hpp>
#include <dptl/utest.hpp>

using namespace dptl;

class ppadapttest : public unittest, public basic_testdata
{
	int	m_cnt;

    public:
	void test01();
	void test02();
	void test03();
	void test04();
	void test05();
	void test06();
	void test07();
	void test08();
	void test09();

	void test11();
	void test12();
	void test13();
	void test14();

	void test21();
	void test22();
	void test23();
	void test24();

	void test91();
	void test92();

	ppadapttest( int argc, char** argv ) : unittest( argc, argv )
	{
		m_cnt = test::getCntr();
		fprintf( stderr, "*** test::getCntr() : %d\n", m_cnt );
	}
	~ppadapttest()
	{
		this->set_subtitle( "test class allocation" );
		AssertEqual( m_cnt, test::getCntr() );
		fprintf( stderr, "\n" );
	}
};

// -----------------------------------------------------------------------------
void ppadapttest::test01()
{
	this->set_subtitle( "refer pointer" );

	std::deque<test*>		dr;
	test*				p;

	dr.push_front( &t1 );
	dr.push_front( &t2 );
	dr.push_front( &t3 );

	for ( auto i = dr.begin(); i != dr.end(); ++i ) println( *i );

	std::deque<test*>		dq = dr;

	for ( auto i = dq.begin(); i != dq.end(); ++i ) println( *i );
}

// -----------------------------------------------------------------------------
void ppadapttest::test02()
{
	this->set_subtitle( "deep pointer (move)" );

	std::deque<deep_ptr<test*>>	dm;
	test*				p;

	dm.push_front( dpl( &t1 ));
	dm.push_front( dpl( &t2 ));
	dm.push_front( dpl( &t3 ));

	for ( auto i = dm.begin(); i != dm.end(); ++i ) println( static_cast<test*>( *i ));

	std::deque<deep_ptr<test*>>	dq = std::move( dm );

	for ( auto i = dq.begin(); i != dq.end(); ++i ) println( static_cast<test*>( p ));
}

// -----------------------------------------------------------------------------
void ppadapttest::test03()
{
	this->set_subtitle( "deep pointer (copy)" );

	std::deque<deep_ptr<test*>>	dc;
	test*				p;

	dc.push_front( static_cast<test* const&>( &t1 ));
	dc.push_front( static_cast<test* const&>( &t2 ));
	dc.push_front( static_cast<test* const&>( &t3 ));

	for ( auto i = dc.begin(); i != dc.end(); ++i ) {
		p = *i;
		println( p );
	}

	std::deque<deep_ptr<test*>>	dq = dc;

	for ( auto i = dq.begin(); i != dq.end(); ++i ) {
		p = *i;
		println( p );
	}
}

// -----------------------------------------------------------------------------
void ppadapttest::test04()
{
	this->set_subtitle( "refer string" );

	std::deque<const char*>		dr;
	const char*			p;

	dr.push_front( s1 );
	dr.push_front( s2 );
	dr.push_front( s3 );

	for ( auto i = dr.begin(); i != dr.end(); ++i ) {
		p = *i;
		println( p );
	}

	std::deque<const char*>		dq = dr;

	for ( auto i = dq.begin(); i != dq.end(); ++i ) {
		p = *i;
		println( p );
	}
}

// -----------------------------------------------------------------------------
void ppadapttest::test05()
{
	this->set_subtitle( "deep string (move)" );

	std::deque<deep_ptr<char[]>>	ds;
	char*				p;

	ds.push_front( strdup( s1 ));
	ds.push_front( strdup( s2 ));
	ds.push_front( strdup( s3 ));

	for ( auto i = ds.begin(); i != ds.end(); ++i ) {
		p = *i;
		println( p );
	}

	std::deque<deep_ptr<char[]>>	dq = ds;

	for ( auto i = dq.begin(); i != dq.end(); ++i ) {
		p = *i;
		println( p );
	}
}

// -----------------------------------------------------------------------------
void ppadapttest::test06()
{
	this->set_subtitle( "deep string (copy)" );

	std::deque<deep_ptr<char[]>>	dc;
	char*				p;

	dc.push_front( static_cast<char const* const&>( s1 ));
	dc.push_front( static_cast<char const* const&>( s2 ));
	dc.push_front( static_cast<char const* const&>( s3 ));

	for ( auto i = dc.begin(); i != dc.end(); ++i ) {
		p = *i;
		println( p );
	}

	std::deque<deep_ptr<char[]>>	dq = dc;

	for ( auto i = dq.begin(); i != dq.end(); ++i ) {
		p = *i;
		println( p );
	}
}

// -----------------------------------------------------------------------------
void ppadapttest::test07()
{
	this->set_subtitle( "refer array" );

	std::deque<test*>	dr;
	test*			p;

	dr.push_front( w1 );
	dr.push_front( w2 );
	dr.push_front( w3 );

	for ( auto i = dr.begin(); i != dr.end(); ++i ) {
		p = *i;
		printvln( p, 3 );
	}

	std::deque<test*>	dq = dr;

	for ( auto i = dq.begin(); i != dq.end(); ++i ) {
		p = *i;
		printvln( p, 3 );
	}
}

// -----------------------------------------------------------------------------
void ppadapttest::test08()
{
	this->set_subtitle( "deep array (move)" );

	std::deque<deep_ptr<test[3]>>	ds;
	test*				p;

	ds.push_front( dpl( w1 ));
	ds.push_front( dpl( w2 ));
	ds.push_front( dpl( w3 ));

	for ( auto i = ds.begin(); i != ds.end(); ++i ) {
		p = *i;
		printvln( p, 3 );
	}

	std::deque<deep_ptr<test[3]>>	dq = ds;

	for ( auto i = dq.begin(); i != dq.end(); ++i ) {
		p = *i;
		printvln( p, 3 );
	}
}

// -----------------------------------------------------------------------------
void ppadapttest::test09()
{
	this->set_subtitle( "deep array (copy)" );

	std::deque<deep_ptr<test[3]>>	dc;
	test*				p;

	dc.push_front( static_cast<test* const&>( w1 ));
	dc.push_front( static_cast<test* const&>( w2 ));
	dc.push_front( static_cast<test* const&>( w3 ));

	for ( auto i = dc.begin(); i != dc.end(); ++i ) {
		p = *i;
		printvln( p, 3 );
	}

	std::deque<deep_ptr<test[3]>>	dq = dc;

	for ( auto i = dq.begin(); i != dq.end(); ++i ) {
		p = *i;
		printvln( p, 3 );
	}
}

// -----------------------------------------------------------------------------
void ppadapttest::test11()
{
	this->set_subtitle( "cross policy copy/move: deep->deep" );

	deep_ptr<test*>		q1( dpl( &t1 ));		// &t1 is rhs reference, so 'dpl' or 'cast into lhs ref' required
	deep_ptr<test*>		q2( dpl( &t2 )); 
	deep_ptr<test*>		q3( dpl( &t3 )); 
	deep_ptr<test*>		q4( dpl( &t4 )); 
	deep_ptr<test*>		r1( q1 );
	deep_ptr<test*>		r2; 
	deep_ptr<test*>		r3( std::move( q3 ));
	deep_ptr<test*>		r4; 

	r2 = q2;
	r4 = std::move( q4 );
}

// -----------------------------------------------------------------------------
void ppadapttest::test12()
{
	this->set_subtitle( "cross policy copy/move: refer->refer" );

	dref_ptr<test*>		q1( &t1 );
	dref_ptr<test*>		q2( &t2 ); 
	dref_ptr<test*>		q3( &t3 ); 
	dref_ptr<test*>		q4( &t4 ); 
	dref_ptr<test*>		r1( q1 );
	dref_ptr<test*>		r2; 
	dref_ptr<test*>		r3( std::move( q3 ));
	dref_ptr<test*>		r4; 

	r2 = q2;
	r4 = std::move( q4 );
}

// -----------------------------------------------------------------------------
void ppadapttest::test13()
{
	this->set_subtitle( "cross policy copy/move: deep->refer" );

	deep_ptr<test*>		q1( dpl( &t1 ));
	deep_ptr<test*>		q2( dpl( &t2 )); 
	deep_ptr<test*>		q3( dpl( &t3 )); 
	deep_ptr<test*>		q4( dpl( &t4 )); 
	dref_ptr<test*>		r1( q1 );
	dref_ptr<test*>		r2; 
	dref_ptr<test*>		r3( std::move( q3 ));
	dref_ptr<test*>		r4;		 

	r2 = q2;
	r4 = std::move( q4 );
}

// -----------------------------------------------------------------------------
void ppadapttest::test14()
{
	this->set_subtitle( "cross policy copy/move: refer->deep" );

	dref_ptr<test*>		q1( &t1 );
	dref_ptr<test*>		q2( &t2 ); 
	dref_ptr<test*>		q3( dpl( &t3 ));		// mover has ownership
	dref_ptr<test*>		q4( dpl( &t4 ));		// mover has ownership
	deep_ptr<test*>		r1( q1 );
	deep_ptr<test*>		r2; 
	deep_ptr<test*>		r3( std::move( q3 ));		// mover transfers ownership to movee
	deep_ptr<test*>		r4;				 

	r2 = q2;
	r4 = std::move( q4 );					// mover transfers ownership to movee 

	AssertNull( q3.get());
	AssertNull( q4.get());
}

// -----------------------------------------------------------------------------
void ppadapttest::test21()
{
	this->set_subtitle( "cross policy copy/move: non-const->non-const" );

	deep_ptr<test*>		q1( dpl( &t1 ));		// &t1 is rhs reference, so 'dpl' or 'cast into lhs ref' required
	deep_ptr<test*>		q2( dpl( &t2 )); 
	deep_ptr<test*>		q3( dpl( &t3 )); 
	deep_ptr<test*>		q4( dpl( &t4 )); 
	deep_ptr<test*>		r1( q1 );
	deep_ptr<test*>		r2; 
	deep_ptr<test*>		r3( std::move( q3 ));
	deep_ptr<test*>		r4; 

	r2 = q2;
	r4 = std::move( q4 );
}

// -----------------------------------------------------------------------------
void ppadapttest::test22()
{
	this->set_subtitle( "cross policy copy/move: const->const" );

	deep_ptr<const test*>	q1( dpl( &t1 ));
	deep_ptr<const test*>	q2( dpl( &t2 )); 
	deep_ptr<const test*>	q3( dpl( &t3 )); 
	deep_ptr<const test*>	q4( dpl( &t4 )); 
	deep_ptr<const test*>	r1( q1 );
	deep_ptr<const test*>	r2; 
	deep_ptr<const test*>	r3( std::move( q3 ));
	deep_ptr<const test*>	r4; 

	r2 = q2;
	r4 = std::move( q4 );
}

// -----------------------------------------------------------------------------
void ppadapttest::test23()
{
	this->set_subtitle( "cross policy copy/move: non-const->const" );

	deep_ptr<test*>		q1( dpl( &t1 ));
	deep_ptr<test*>		q2( dpl( &t2 )); 
	deep_ptr<test*>		q3( dpl( &t3 )); 
	deep_ptr<test*>		q4( dpl( &t4 )); 
	deep_ptr<const test*>	r1( q1 );
	deep_ptr<const test*>	r2; 
	deep_ptr<const test*>	r3( std::move( q3 ));
	deep_ptr<const test*>	r4;		 

	r2 = q2;
	r4 = std::move( q4 );
}

// -----------------------------------------------------------------------------
void ppadapttest::test24()
{
	this->set_subtitle( "cross policy copy/move: const->non-const" );

	deep_ptr<const test*>	q1( dpl( &t1 ));
	deep_ptr<const test*>	q2( dpl( &t2 )); 
	deep_ptr<const test*>	q3( dpl( &t3 ));
	deep_ptr<const test*>	q4( dpl( &t4 ));
	deep_ptr<test*>		r1( q1 );
	deep_ptr<test*>		r2; 
	//deep_ptr<test*>		r3( std::move( q3 ));
	deep_ptr<test*>		r4;				 

	r2 = q2;
	//r4 = std::move( q4 );			
}

// -----------------------------------------------------------------------------
void ppadapttest::test91()
{
	this->set_subtitle( "dp_less test" );

	std::set<const char*, dp_less<const char[]>>	s;

	s = { "hello", "world", "thanks", "great", "happiness" };

	for ( auto p : s ) printf( "%s\n", p );
}

// -----------------------------------------------------------------------------
void ppadapttest::test92()
{
	this->set_subtitle( "sizeof deep_ptr/dref_ptr" );

	AssertEqual( sizeof( deep_ptr<test*> ), sizeof( test* ));
	AssertEqual( sizeof( dref_ptr<test*> ), sizeof( test* ));
}

// -----------------------------------------------------------------------------
int main( int argc, char** argv )
{
	ppadapttest( argc, argv ).run();

	return 0;
}

