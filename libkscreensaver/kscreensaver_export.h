/*  This file is part of the KDE project
    Copyright (C) 2012 Andrius da Costa Ribas <andriusmao@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#ifndef KSCREENSAVER_EXPORT_H
#define KSCREENSAVER_EXPORT_H

#ifndef KSCREENSAVER_EXPORT
# if defined(MAKE_KSCREENSAVER_LIB)
   /* We are building this library */ 
#  define KSCREENSAVER_EXPORT Q_DECL_EXPORT
# else
   /* We are using this library */ 
#  define KSCREENSAVER_EXPORT Q_DECL_IMPORT
# endif
#endif

# ifndef KSCREENSAVER_EXPORT_DEPRECATED
#  define KSCREENSAVER_EXPORT_DEPRECATED KDE_DEPRECATED KSCREENSAVER_EXPORT
# endif

#endif
