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

#include <QAction>
#include <QWidget>

#include "ToolBarButton.h"
#include "Utils.h"

//////////////////////////////////////////////////////////////////////////////

namespace QtHandles
{

//////////////////////////////////////////////////////////////////////////////

template <class T>
ToolBarButton<T>::ToolBarButton (const graphics_object& go, QAction* action)
    : Object (go, action), m_separator (0)
{
  typename T::properties& tp = properties<T> ();

  action->setToolTip (Utils::fromStdString (tp.get_tooltipstring ()));
  action->setVisible (tp.is_visible ());
  QImage img = Utils::makeImageFromCData (tp.get_cdata (), 16, 16);
  action->setIcon (QIcon (QPixmap::fromImage (img)));
  if (tp.is_separator ())
    {
      m_separator = new QAction (action);
      m_separator->setSeparator (true);
      m_separator->setVisible (tp.is_visible ());
    }
  action->setEnabled (tp.is_enable ());

  QWidget* w = qobject_cast<QWidget*> (action->parent ());

  w->insertAction (w->actions ().back (), action);
  if (m_separator)
    w->insertAction (action, m_separator);
}

//////////////////////////////////////////////////////////////////////////////

template <class T>
ToolBarButton<T>::~ToolBarButton (void)
{
}

//////////////////////////////////////////////////////////////////////////////

template <class T>
void ToolBarButton<T>::update (int pId)
{
  typename T::properties& tp = properties<T> ();
  QAction* action = qWidget<QAction> ();

  switch (pId)
    {
    case base_properties::ID_VISIBLE:
      action->setVisible (tp.is_visible ());
      if (m_separator)
	m_separator->setVisible (tp.is_visible ());
      break;
    case T::properties::ID_TOOLTIPSTRING:
      action->setToolTip (Utils::fromStdString (tp.get_tooltipstring ()));
      break;
    case T::properties::ID_CDATA:
	{
	  QImage img = Utils::makeImageFromCData (tp.get_cdata (), 16, 16);

	  action->setIcon (QIcon (QPixmap::fromImage (img)));
	}
      break;
    case T::properties::ID_SEPARATOR:
      if (tp.is_separator ())
	{
	  if (! m_separator)
	    {
	      m_separator = new QAction (action);
	      m_separator->setSeparator (true);
	      m_separator->setVisible (tp.is_visible ());

	      QWidget* w = qobject_cast<QWidget*> (action->parent ());

	      w->insertAction (action, m_separator);
	    }
	}
      else
	{
	  if (m_separator)
	    delete m_separator;
	  m_separator = 0;
	}
      break;
    case T::properties::ID_ENABLE:
      action->setEnabled (tp.is_enable ());
      break;
    default:
      Object::update (pId);
      break;
    }
}

//////////////////////////////////////////////////////////////////////////////

}; // namespace QtHandles
