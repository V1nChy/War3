// GEFile.cpp: implementation of the GEFile class.
//
//////////////////////////////////////////////////////////////////////

#include "GEFile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GEFile::GEFile()
{

}

GEFile::~GEFile()
{

}


bool GEFile::IsFileExist(string strPath)
{
	HANDLE h = CreateFile( strPath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, 
                       OPEN_EXISTING, 0, NULL );
    if( INVALID_HANDLE_VALUE != h )
    {
        CloseHandle( h );
        return true;
    }
	else 
	{
		CloseHandle( h );
		return false;
	}
}
