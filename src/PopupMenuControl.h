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

#ifndef __QtHandles_PopupMenuControl__
#define __QtHandles_PopupMenuControl__ 1

#include "BaseControl.h"

class QComboBox;

//////////////////////////////////////////////////////////////////////////////

namespace QtHandles
{

//////////////////////////////////////////////////////////////////////////////

class PopupMenuControl : public BaseControl
{
  Q_OBJECT

public:
  PopupMenuControl (const graphics_object& go, QComboBox* box);
  ~PopupMenuControl (void);

  static PopupMenuControl* create (const graphics_object& go);

protected:
  void update (int pId);

private slots:
  void currentIndexChanged (int index);

private:
  bool m_blockUpdate;
};

//////////////////////////////////////////////////////////////////////////////

}; // namespace QtHandles

//////////////////////////////////////////////////////////////////////////////

#endif
