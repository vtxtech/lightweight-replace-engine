/*
 * Lightweight Replace Engine
 * Copyright (C) 2012-2013 Johannes Scholz. All rights reserved.
 *
 * This file is licensed under the GNU Lesser General Public License 3 (LGPLv3),
 * but distributed WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * @brief Copyleft Vincent Godin. Originally from:
 * @brief http://www.codeproject.com/Articles/16431/Add-color-to-your-std-cout
 *
 */

#include "ConsoleColor.h"

// Current color implementation is Windows-only.
#ifdef WIN32
	#include <windows.h>
#endif

std::ostream& blue(std::ostream &s)
{
#ifdef WIN32
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
    SetConsoleTextAttribute(hStdout, FOREGROUND_BLUE
              |FOREGROUND_GREEN|FOREGROUND_INTENSITY);
#endif
    return s;
}

std::ostream& red(std::ostream &s)
{
#ifdef WIN32
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
    SetConsoleTextAttribute(hStdout, 
                FOREGROUND_RED|FOREGROUND_INTENSITY);
#endif
    return s;
}

std::ostream& green(std::ostream &s)
{
#ifdef WIN32
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
    SetConsoleTextAttribute(hStdout, 
              FOREGROUND_GREEN|FOREGROUND_INTENSITY);
#endif
    return s;
}

std::ostream& yellow(std::ostream &s)
{
#ifdef WIN32
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
    SetConsoleTextAttribute(hStdout, 
         FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
#endif
    return s;
}

std::ostream& white(std::ostream &s)
{
#ifdef WIN32
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
    SetConsoleTextAttribute(hStdout, 
       FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);
#endif
    return s;
}

#ifdef WIN32
	struct color {
		color(WORD attribute):m_color(attribute){};
		WORD m_color;
	};

	template <class _Elem, class _Traits>
	std::basic_ostream<_Elem,_Traits>& 
		  operator<<(std::basic_ostream<_Elem,_Traits>& i, color& c)
	{
		HANDLE hStdout=GetStdHandle(STD_OUTPUT_HANDLE); 
		SetConsoleTextAttribute(hStdout,c.m_color);
		return i;
	}
#endif

