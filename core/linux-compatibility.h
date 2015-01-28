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

#include <stdio.h>
#include <time.h>

#define CLK_TCK CLOCKS_PER_SEC

char* itoa( int value, char* str, int base) {
	switch( base ) {
		case 10: {
			sprintf( str, "%d", value );
			break;
		}
		case 16: {
			sprintf( str, "%x", value );
			break;
		}
		case 8:{
			sprintf( str, "%o", value );
			break;
		}
		default: break;
	}
	return str;
}

#endif
