// Mines-Perfect: a minesweeper clone
// Copyright (C) 1995-2003  Christian Czepluch
// Copyright (C) 2015 James Dearing
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include "linux-compatibility.h"

#if defined wxUSE_UNICODE
  wxString intToStr( int value, wchar_t* str, int base ) {
    size_t length = wcslen( str );
	switch( base ) {
		case 10: {
			swprintf( str, length, L"%d", value );
			break;
		}
		case 16: {
			swprintf( str, length, L"%x", value );
			break;
		}
		case 8: {
			swprintf( str, length, L"%o", value );
			break;
		}
		default: break;
	}
	return str;
  }
#else
  wxString intToStr( int value, char* str, int base ) {
	switch( base ) {
		case 10: {
			sprintf( str, "%d", value );
			break;
		}
		case 16: {
			sprintf( str, "%x", value );
			break;
		}
		case 8: {
			sprintf( str, "%o", value );
			break;
		}
		default: break;
	}
	return str;
  }
#endif

#if defined wxUSE_UNICODE
  #include <cwchar>
  int strToInt( wxString str ) {
  	std::wstring temp = str.c_str();
  	//std::wcout << temp << std::endl;
  	int result = wcstol( temp.c_str(), NULL, 10 );//debugging statements, remove when bug is fixed
    return result;
  }
#else
  #include <cstdlib>
  int strToInt( wxString str ) {
    return atoi( str.c_str() );
  }
#endif

unsigned long long getTime() {
  timespec t;
  clock_gettime( CLOCK_MONOTONIC, &t );
  unsigned long long result = ( t.tv_sec * 1000000 ) + t.tv_nsec;
  return result;
}
