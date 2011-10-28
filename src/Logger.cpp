/*

Copyright (C) 2011 Michael Goffioul.

This file is part of QtHandles.

Foobar is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

QtHandles is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <QProcessEnvironment>

#include <cstdio>

#include "Logger.h"

//////////////////////////////////////////////////////////////////////////////

namespace QtHandles
{

//////////////////////////////////////////////////////////////////////////////

Logger* Logger::s_instance = 0;

//////////////////////////////////////////////////////////////////////////////

Logger::Logger (void)
    : m_debugEnabled (false)
{
  QProcessEnvironment pe (QProcessEnvironment::systemEnvironment ());

  if (pe.value ("QTHANDLES_DEBUG", "0") != "0")
    m_debugEnabled = true;
}

//////////////////////////////////////////////////////////////////////////////

Logger::~Logger (void)
{
}

//////////////////////////////////////////////////////////////////////////////

Logger* Logger::instance (void)
{
  if (! s_instance)
    s_instance = new Logger ();

  return s_instance;
}

//////////////////////////////////////////////////////////////////////////////

#define STATIC_LOGGER(fun) \
void Logger::fun (const char* fmt, ...) \
{ \
  va_list vl; \
  va_start (vl, fmt); \
  instance ()->fun ## V (fmt, vl); \
  va_end (vl); \
}

STATIC_LOGGER (debug)

//////////////////////////////////////////////////////////////////////////////

void Logger::debugV (const char* fmt, va_list arg)
{
  if (m_debugEnabled)
    {
      vfprintf (stderr, fmt, arg);
      fprintf (stderr, "\n");
    }
}

//////////////////////////////////////////////////////////////////////////////

}; // namespace QtHandles
