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

#ifndef __QtHandles_GLCanvas__
#define __QtHandles_GLCanvas__ 1

#include <QGLWidget>

#include "Canvas.h"

//////////////////////////////////////////////////////////////////////////////

namespace QtHandles
{

//////////////////////////////////////////////////////////////////////////////

class GLCanvas : public QGLWidget, public Canvas
{
public:
  GLCanvas (QWidget* parent, const graphics_handle& handle);
  ~GLCanvas (void);

  void draw (const graphics_handle& handle);
  void drawZoomBox (const QPoint& p1, const QPoint& p2);
  void resize (int /* x */, int /* y */,
	       int /* width */, int /* height */) { }
  QWidget* qWidget (void) { return this; }

protected:
  void paintGL (void);
  void mouseMoveEvent (QMouseEvent* event);
  void mousePressEvent (QMouseEvent* event);
  void mouseReleaseEvent (QMouseEvent* event);
};

//////////////////////////////////////////////////////////////////////////////

}; // namespace QtHandles

//////////////////////////////////////////////////////////////////////////////

#endif
