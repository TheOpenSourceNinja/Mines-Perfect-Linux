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

#ifndef LINUX_COMPATIBILITY_H_INCLUDED
#define LINUX_COMPATIBILITY_H_INCLUDED

#include <stdio.h> //for the itoa() replacement
#include <time.h> //for getTime()

#include <wx/string.h>

//replacement for itoa()
wxString intToStr( int value, wchar_t* str, int base );

//replacement for atoi()
int strToInt( wxString str );

//replacement for clock()
//Returns time in nanoseconds
unsigned long long getTime(); //Needs at least the same precision as clock_t, which is implementation-defined

#endif
