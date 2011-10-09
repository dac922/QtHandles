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

#ifndef __QtHandles_Panel__
#define __QtHandles_Panel__ 1

#include "Object.h"

class QFrame;
class QLabel;

//////////////////////////////////////////////////////////////////////////////

namespace QtHandles
{

//////////////////////////////////////////////////////////////////////////////

class Container;

class Panel : public Object
{
public:
  Panel (const graphics_object& go, QFrame* frame);
  ~Panel (void);

  Container* innerContainer (void) { return m_container; }

  bool eventFilter (QObject* watched, QEvent* event);

  static Panel* create (const graphics_object& go);

protected:
  void update (int pId);
  void redraw (void);

private:
  void updateLayout (void);

private:
  Container* m_container;
  QLabel* m_title;
  bool m_blockUpdates;
};

//////////////////////////////////////////////////////////////////////////////

}; // namespace QtHandles

//////////////////////////////////////////////////////////////////////////////

#endif
