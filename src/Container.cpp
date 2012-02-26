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

#include <QVBoxLayout>

#include <octave/oct.h>
#include <octave/graphics.h>

#include "Canvas.h"
#include "Container.h"
#include "Object.h"
#include "Utils.h"

//////////////////////////////////////////////////////////////////////////////

namespace QtHandles
{

//////////////////////////////////////////////////////////////////////////////

Container::Container (QWidget* parent)
  : ContainerBase (parent), m_canvas (0)
{
  setFocusPolicy (Qt::ClickFocus);
}

//////////////////////////////////////////////////////////////////////////////

Container::~Container (void)
{
}

//////////////////////////////////////////////////////////////////////////////

Canvas* Container::canvas (const graphics_handle& handle, bool create)
{
  if (! m_canvas && create)
    {
      graphics_object go = gh_manager::get_object (handle);

      if (go)
	{
	  graphics_object fig = go.get_ancestor ("figure");

	  m_canvas = Canvas::create (fig.get("renderer").string_value (),
				     this, handle);

	  QWidget* canvasWidget = m_canvas->qWidget ();

	  canvasWidget->lower ();
	  canvasWidget->show ();
	  canvasWidget->setGeometry (0, 0, width (), height ());
	}
    }

  return m_canvas;
}

//////////////////////////////////////////////////////////////////////////////

void Container::resizeEvent (QResizeEvent* /* event */)
{
  if (m_canvas)
    m_canvas->qWidget ()->setGeometry (0, 0, width (), height ());

  gh_manager::auto_lock lock;

  foreach (QObject* qObj, children ())
    {
      if (qObj->isWidgetType ())
	{
	  Object* obj = Object::fromQObject (qObj);

	  if (obj)
	    {
	      Matrix bb = obj->properties ().get_boundingbox (false);

	      obj->qWidget<QWidget> ()
		->setGeometry (xround (bb(0)), xround (bb(1)),
			       xround (bb(2)), xround (bb(3)));
	    }
	}
    }
}

//////////////////////////////////////////////////////////////////////////////

}; // namespace QtHandles
