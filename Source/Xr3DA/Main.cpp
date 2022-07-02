#include "..\XrEngine\stdafx.h"


ENGINE_API int EngineLaunch(EGamePath Game);
int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	char* lpCmdLine,
	int       nCmdShow)
{
	//FILE* file				= 0;
	//fopen_s					( &file, "z:\\development\\call_of_prypiat\\resources\\gamedata\\shaders\\r3\\objects\\r4\\accum_sun_near_msaa_minmax.ps\\2048__1___________4_11141_", "rb" );
	//u32 const file_size		= 29544;
	//char* buffer			= (char*)malloc(file_size);
	//fread					( buffer, file_size, 1, file );
	//fclose					( file );

	//u32 const& crc			= *(u32*)buffer;

	//boost::crc_32_type		processor;
	//processor.process_block	( buffer + 4, buffer + file_size );
	//u32 const new_crc		= processor.checksum( );
	//VERIFY					( new_crc == crc );

	//free					(buffer);

	EngineLaunch(EGamePath::NONE);



	return					(0);
}