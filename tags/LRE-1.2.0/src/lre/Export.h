/*
 * Lightweight Replace Engine
 * Copyright (C) 2012-2013 Johannes Scholz. All rights reserved.
 *
 * This file is licensed under the GNU Lesser General Public License 3 (LGPLv3),
 * but distributed WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#ifndef LRE_EXPORT_H
#define LRE_EXPORT_H 1

#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__)  || defined( __MWERKS__)
    #  if defined( STATIC_LRE_LIBRARIES )
    #    define LRE_EXPORT
    #  elif defined( LRE_LIBRARY )
    #    define LRE_EXPORT   __declspec(dllexport)
    #  else
    #    define LRE_EXPORT   __declspec(dllimport)
    #  endif
#else
    #  define LRE_EXPORT
#endif

#endif
