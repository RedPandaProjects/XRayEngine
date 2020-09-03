#include "stdafx.h"
#ifdef	DEBUG
std::string get_string( const Fvector& v )
{
	return make_string	("( %f, %f, %f )", v.x, v.y, v.z );
}
std::string get_string( const Fmatrix& dop )
{
	Fvector4 Matrix[4];
	Matrix[0] = dop.get_row(0);
	Matrix[1] = dop.get_row(1);
	Matrix[2] = dop.get_row(2);
	Matrix[3] = dop.get_row(3);
	return make_string	("\n%f,%f,%f,%f\n%f,%f,%f,%f\n%f,%f,%f,%f\n%f,%f,%f,%f\n",
		Matrix[0].x, Matrix[0].y, Matrix[0].z, Matrix[0].w,
		Matrix[1].x, Matrix[1].y, Matrix[1].z, Matrix[1].w,
		Matrix[2].x, Matrix[2].y, Matrix[2].z, Matrix[2].w,
		Matrix[3].x, Matrix[3].y, Matrix[3].z, Matrix[3].w
	);
}
std::string get_string(const Fbox &box)
{
	return make_string( "[ min: %s - max: %s ]", get_string( box.min ).c_str(), get_string( box.max ).c_str() );
}
std::string get_string( bool v )
{
	return v ? std::string( "true" ) : std::string( "false" );
}



std::string dump_string( LPCSTR name, const Fvector &v )
{
	return make_string( "%s : (%f,%f,%f) ", name, v.x, v.y, v.z );
}

void dump( LPCSTR name, const Fvector &v )
{
	Msg( "%s", dump_string( name, v ).c_str() );
}

std::string dump_string( LPCSTR name, const Fmatrix &dop )
{
	
	Fvector4 Matrix[4];
	Matrix[0] = dop.get_row(0);
	Matrix[1] = dop.get_row(1);
	Matrix[2] = dop.get_row(2);
	Matrix[3] = dop.get_row(3);
	return make_string("\n%s[%f,%f,%f,%f]\n%s[%f,%f,%f,%f]\n%s[%f,%f,%f,%f]\n%s[%f,%f,%f,%f]\n",
		name,Matrix[0].x, Matrix[0].y, Matrix[0].z, Matrix[0].w,
		name, Matrix[1].x, Matrix[1].y, Matrix[1].z, Matrix[1].w,
		name, Matrix[2].x, Matrix[2].y, Matrix[2].z, Matrix[2].w,
		name, Matrix[3].x, Matrix[3].y, Matrix[3].z, Matrix[3].w
	);
}

void dump( LPCSTR name, const Fmatrix &form )
{
	Msg( "%s", dump_string( name, form ) );
	//Msg( "%s, _14_=%f ", dump_string( make_string( "%s.i, ", name ).c_str(), form.i ).c_str( ) , form._14_ );  
	//Msg( "%s, _24_=%f ", dump_string( make_string( "%s.j, ", name ).c_str(), form.j ).c_str( ) , form._24_ );  
	//Msg( "%s, _34_=%f ", dump_string( make_string( "%s.k, ", name ).c_str(), form.k ).c_str( ) , form._34_  );  
	//Msg( "%s, _44_=%f ", dump_string( make_string( "%s.c, ", name ).c_str(), form.c ).c_str( ) , form._44_ );  
}

#endif