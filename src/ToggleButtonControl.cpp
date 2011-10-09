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

#include <QPushButton>

#include "ToggleButtonControl.h"
#include "Container.h"
#include "Utils.h"

//////////////////////////////////////////////////////////////////////////////

namespace QtHandles
{

//////////////////////////////////////////////////////////////////////////////

ToggleButtonControl* ToggleButtonControl::create (const graphics_object& go)
{
  Object* parent = Object::parentObject (go);

  if (parent)
    {
      Container* container = parent->innerContainer ();

      if (container)
	return new ToggleButtonControl (go, new QPushButton (container));
    }

  return 0;
}

//////////////////////////////////////////////////////////////////////////////

ToggleButtonControl::ToggleButtonControl (const graphics_object& go,
					  QPushButton* btn)
    : ButtonControl (go, btn)
{
  btn->setCheckable (true);
  btn->setAutoFillBackground (true);
}

//////////////////////////////////////////////////////////////////////////////

ToggleButtonControl::~ToggleButtonControl (void)
{
}

//////////////////////////////////////////////////////////////////////////////

};
