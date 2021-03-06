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

#include <QLabel>

#include "Container.h"
#include "TextControl.h"
#include "Utils.h"

//////////////////////////////////////////////////////////////////////////////

namespace QtHandles
{

//////////////////////////////////////////////////////////////////////////////

TextControl* TextControl::create (const graphics_object& go)
{
  Object* parent = Object::parentObject (go);

  if (parent)
    {
      Container* container = parent->innerContainer ();

      if (container)
	return new TextControl (go, new QLabel (container));
    }

  return 0;
}

//////////////////////////////////////////////////////////////////////////////

TextControl::TextControl (const graphics_object& go, QLabel* label)
     : BaseControl (go, label)
{
  uicontrol::properties& up = properties<uicontrol> ();

  label->setAutoFillBackground (true);
  label->setTextFormat (Qt::PlainText);
  label->setWordWrap (false);
  label->setAlignment (Utils::fromHVAlign (up.get_horizontalalignment (),
					   up.get_verticalalignment ()));
  // FIXME: support string_vector
  label->setText (Utils::fromStdString (up.get_string_string ()));
}

//////////////////////////////////////////////////////////////////////////////

TextControl::~TextControl (void)
{
}

//////////////////////////////////////////////////////////////////////////////

void TextControl::update (int pId)
{
  uicontrol::properties& up = properties<uicontrol> ();
  QLabel* label = qWidget<QLabel> ();

  switch (pId)
    {
    case uicontrol::properties::ID_STRING:
      // FIXME: support string_vector
      label->setText (Utils::fromStdString (up.get_string_string ()));
      break;
    case uicontrol::properties::ID_HORIZONTALALIGNMENT:
    case uicontrol::properties::ID_VERTICALALIGNMENT:
      label->setAlignment (Utils::fromHVAlign (up.get_horizontalalignment (),
					       up.get_verticalalignment ()));
      break;
    default:
      BaseControl::update (pId);
      break;
    }
}

//////////////////////////////////////////////////////////////////////////////

}; // namespace QtHandles
