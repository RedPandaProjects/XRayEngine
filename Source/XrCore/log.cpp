#include "stdafx.h"
XRayLogInterface* GLogInterface = nullptr;
static LogCallback			LogCB = 0;
XRCORE_API	BOOL		    LogExecCB = TRUE;
xrCriticalSection* LogCS = nullptr;
void Log				(const char *s) 
{
	LogCS->Enter();
	int		i, j;

	u32			length = xr_strlen(s);
	PSTR split = (PSTR)_alloca((length + 1) * sizeof(char));
	for (i = 0, j = 0; s[i] != 0; i++) 
	{
		if (s[i] == '\n')
		{
			split[j] = 0;	// end of line
			if (split[0] == 0) { split[0] = ' '; split[1] = 0; }
			GLogInterface->Log(split);
			if (LogExecCB && LogCB)LogCB(split);
			j = 0;
		}
		else 
		{
			split[j++] = s[i];
		}
	}
	split[j] = 0;
	GLogInterface->Log(split);
	if (LogExecCB && LogCB)LogCB(split);
	LogCS->Leave();
	
}

void __cdecl Msg		( const char *format, ...)
{
	va_list		mark;
	string2048	buf;
	va_start	(mark, format );
	int sz		= _vsnprintf(buf, sizeof(buf)-1, format, mark ); buf[sizeof(buf)-1]=0;
    va_end		(mark);
	if (sz)		Log(buf);
}

void Log				(const char *msg, const char *dop)
{
	if (!dop) {
		Log		(msg);
		return;
	}

	u32			buffer_size = (xr_strlen(msg) + 1 + xr_strlen(dop) + 1) * sizeof(char);
	PSTR buf	= (PSTR)_alloca( buffer_size );
	strconcat	(buffer_size, buf, msg, " ", dop);
	Log			(buf);
}

void Log				(const char *msg, u32 dop) 
{
	string1024		Buffer;
	xr_sprintf	(Buffer, sizeof(Buffer), "%s %u", msg, dop);
	Log			(Buffer);
}

void Log				(const char *msg, int dop) 
{
	string1024		Buffer;
	xr_sprintf	(Buffer, sizeof(Buffer), "%s %d", msg, dop);
	Log			(Buffer);
}

void Log				(const char *msg, float dop)
{
	string1024		Buffer;
	xr_sprintf	(Buffer, sizeof(Buffer), "%s %f", msg, dop);
	Log			(Buffer);
}

void Log				(const char *msg, const Fvector &dop) 
{
	u32			buffer_size = (xr_strlen(msg) + 2 + 3*(64 + 1) + 1) * sizeof(char);
	PSTR buf	= (PSTR)_alloca( buffer_size );

	xr_sprintf	(buf, buffer_size,"%s (%f,%f,%f)",msg, VPUSH(dop) );
	Log			(buf);
}

void Log				(const char *msg, const Fmatrix &dop)	
{
	u32			buffer_size = (xr_strlen(msg) + 2 + 4*( 4*(64 + 1) + 1 ) + 1) * sizeof(char);
	PSTR buf	= (PSTR)_alloca( buffer_size );

	xr_sprintf	(buf, buffer_size,"%s:\n%f,%f,%f,%f\n%f,%f,%f,%f\n%f,%f,%f,%f\n%f,%f,%f,%f\n",
		msg,
		dop.i.x, dop.i.y, dop.i.z, dop._14_,
		dop.j.x, dop.j.y, dop.j.z, dop._24_,
		dop.k.x, dop.k.y, dop.k.z, dop._34_,
		dop.c.x, dop.c.y, dop.c.z, dop._44_
	);
	Log			(buf);
}

void LogWinErr			(const char *msg, long err_code)	{
	Msg					("%s: %s",msg,Debug.error2string(err_code)	);
}

LogCallback SetLogCB	(LogCallback cb)
{
	LogCallback	result	= LogCB;
	LogCB				= cb;
	return				(result);
}

LPCSTR log_name			()
{
	return				"engine.log";
}
