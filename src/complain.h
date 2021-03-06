/* Declaration for error-reporting function for Bison.
   Copyright 2000, 2001 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
   later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA.  */

#ifndef COMPLAIN_H_
#define COMPLAIN_H_ 1

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef __STDC__

/* Informative messages, but we proceed. */

extern void warn (const char *format, ...)
     __attribute__ ((__format__ (__printf__, 1, 2)));

/* Something bad happen, but let's continue and die later. */

extern void complain (const char *format, ...)
     __attribute__ ((__format__ (__printf__, 1, 2)));

/* Something bad happen and we die now. */

extern void fatal (const char *format, ...)
     __attribute__ ((__format__ (__printf__, 1, 2)));

#else
void warn ();
void complain ();
void fatal ();
#endif

/* Position in the current input file. */
extern char *infile;
extern int lineno;

/* This variable is incremented each time `warn' is called.  */
extern unsigned int warn_message_count;

/* This variable is incremented each time `complain' is called.  */
extern unsigned int complain_message_count;

#ifdef	__cplusplus
}
#endif

#endif /* !COMPLAIN_H_ */
