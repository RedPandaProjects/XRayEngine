#include "stdafx.h"
#pragma hdrstop

#include "xrdebug.h"
#include "os_clipboard.h"

#include <sal.h>
#include "directx\dxerr.h"

#pragma warning(push)
#pragma warning(disable:4995)
#include <malloc.h>
#include <direct.h>
#pragma warning(pop)

XRCORE_API	xrDebug		Debug;
XRayDebugInterface* GDebugInterface = nullptr;


void xrDebug::gather_info		(const char *expression, const char *description, const char *argument0, const char *argument1, const char *file, int line, const char *function, LPSTR assertion_info, u32 const assertion_info_size)
{
	LPSTR				buffer_base = assertion_info;
	LPSTR				buffer = assertion_info;
	int assertion_size	= (int)assertion_info_size;
	LPCSTR				endline = "\n";
	LPCSTR				prefix = "[error]";
	bool				extended_description = (description && !argument0 && strchr(description,'\n'));
	for (int i=0; i<2; ++i) {
		if (!i)
			buffer		+= xr_sprintf(buffer,assertion_size - u32(buffer - buffer_base),"%sFATAL ERROR%s%s",endline,endline,endline);
		buffer			+= xr_sprintf(buffer,assertion_size - u32(buffer - buffer_base),"%sExpression    : %s%s",prefix,expression,endline);
		buffer			+= xr_sprintf(buffer,assertion_size - u32(buffer - buffer_base),"%sFunction      : %s%s",prefix,function,endline);
		buffer			+= xr_sprintf(buffer,assertion_size - u32(buffer - buffer_base),"%sFile          : %s%s",prefix,file,endline);
		buffer			+= xr_sprintf(buffer,assertion_size - u32(buffer - buffer_base),"%sLine          : %d%s",prefix,line,endline);
		
		if (extended_description) {
			buffer		+= xr_sprintf(buffer,assertion_size - u32(buffer - buffer_base),"%s%s%s",endline,description,endline);
			if (argument0) {
				if (argument1) {
					buffer	+= xr_sprintf(buffer,assertion_size - u32(buffer - buffer_base),"%s%s",argument0,endline);
					buffer	+= xr_sprintf(buffer,assertion_size - u32(buffer - buffer_base),"%s%s",argument1,endline);
				}
				else
					buffer	+= xr_sprintf(buffer,assertion_size - u32(buffer - buffer_base),"%s%s",argument0,endline);
			}
		}
		else {
			buffer		+= xr_sprintf(buffer,assertion_size - u32(buffer - buffer_base),"%sDescription   : %s%s",prefix,description,endline);
			if (argument0) {
				if (argument1) {
					buffer	+= xr_sprintf(buffer,assertion_size - u32(buffer - buffer_base),"%sArgument 0    : %s%s",prefix,argument0,endline);
					buffer	+= xr_sprintf(buffer,assertion_size - u32(buffer - buffer_base),"%sArgument 1    : %s%s",prefix,argument1,endline);
				}
				else
					buffer	+= xr_sprintf(buffer,assertion_size - u32(buffer - buffer_base),"%sArguments     : %s%s",prefix,argument0,endline);
			}
		}

		buffer			+= xr_sprintf(buffer,assertion_size - u32(buffer - buffer_base),"%s",endline);
		if (!i) {
			{
				
			}
			buffer		= assertion_info;
			endline		= "\r\n";
			prefix		= "";
		}
	}

	
}

void xrDebug::do_exit	(const xr_string&message)
{
	MessageBox			(NULL,message.c_str(),"Error",MB_OK|MB_ICONERROR|MB_SYSTEMMODAL);
	TerminateProcess	(GetCurrentProcess(),1);
}

void xrDebug::backend	(const char *expression, const char *description, const char *argument0, const char *argument1, const char *file, int line, const char *function, bool &ignore_always)
{
	if (IsDebuggerPresent())
	{
		DebugBreak();
		return;
	}


	//error_after_dialog	= true;

	string4096			assertion_info;

	gather_info			(expression, description, argument0, argument1, file, line, function, assertion_info, sizeof(assertion_info) );


	if (handler)
		handler			();

	if (get_on_dialog())
		get_on_dialog()	(true);

	GDebugInterface->CriticalError(assertion_info);

	if (get_on_dialog())
		get_on_dialog()	(false);

}

LPCSTR xrDebug::error2string	(long code)
{
	LPCSTR				result	= 0;
	static	string1024	desc_storage;

	result				= DXGetErrorDescription	(code);
	if (0==result)
	{
		FormatMessage	(FORMAT_MESSAGE_FROM_SYSTEM,0,code,0,desc_storage,sizeof(desc_storage)-1,0);
		result			= desc_storage;
	}
	return		result	;
}

void xrDebug::error		(long hr, const char* expr, const char *file, int line, const char *function, bool &ignore_always)
{
	backend		(error2string(hr),expr,0,0,file,line,function,ignore_always);
}

void xrDebug::error		(long hr, const char* expr, const char* e2, const char *file, int line, const char *function, bool &ignore_always)
{
	backend		(error2string(hr),expr,e2,0,file,line,function,ignore_always);
}

void xrDebug::fail		(const char *e1, const char *file, int line, const char *function, bool &ignore_always)
{
	backend		("assertion failed",e1,0,0,file,line,function,ignore_always);
}

void xrDebug::fail		(const char *e1, const xr_string&e2, const char *file, int line, const char *function, bool &ignore_always)
{
	backend		(e1,e2.c_str(),0,0,file,line,function,ignore_always);
}

void xrDebug::fail		(const char *e1, const char *e2, const char *file, int line, const char *function, bool &ignore_always)
{
	backend		(e1,e2,0,0,file,line,function,ignore_always);
}

void xrDebug::fail		(const char *e1, const char *e2, const char *e3, const char *file, int line, const char *function, bool &ignore_always)
{
	backend		(e1,e2,e3,0,file,line,function,ignore_always);
}

void xrDebug::fail		(const char *e1, const char *e2, const char *e3, const char *e4, const char *file, int line, const char *function, bool &ignore_always)
{
	backend		(e1,e2,e3,e4,file,line,function,ignore_always);
}

void __cdecl xrDebug::fatal(const char *file, int line, const char *function, const char* F,...)
{
	string1024	buffer;

	va_list		p;
	va_start	(p,F);
	vsprintf	(buffer,F,p);
	va_end		(p);

	bool		ignore_always = true;

	backend		("fatal error","<no expression>",buffer,0,file,line,function,ignore_always);
}