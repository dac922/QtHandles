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

#ifndef __QtHandles_Figure__
#define __QtHandles_Figure__ 1

#include "MenuContainer.h"
#include "Object.h"

class QMainWindow;
class QMenuBar;
class QToolBar;

//////////////////////////////////////////////////////////////////////////////

namespace QtHandles
{

//////////////////////////////////////////////////////////////////////////////

enum MouseMode
{
  NoMode	= 0,
  RotateMode	= 1,
  ZoomMode	= 2,
  PanMode	= 3,
  SelectMode	= 4
};

//////////////////////////////////////////////////////////////////////////////

class Container;

class Figure : public Object, public MenuContainer
{
  Q_OBJECT

public:
  Figure (const graphics_object& go, QMainWindow* win);
  ~Figure (void);

  static Figure* create (const graphics_object& go);

  MouseMode mouseMode (void) { return m_mouseMode; }

  Container* innerContainer (void);
  QWidget* menu (void);

  bool eventFilter (QObject* watched, QEvent* event);

protected:
  void redraw (void);
  void update (int pId);
  void updateBoundingBox (bool internal = false);
  void beingDeleted (void);

private:
  void createFigureToolBarAndMenuBar (void);
  void showFigureToolBar (bool visible);
  void showMenuBar (bool visible);

  static void updateBoundingBoxHelper (void*);

private slots:
  void setMouseMode (MouseMode mode) { m_mouseMode = mode; }
  void fileNewFigure (void);
  void fileCloseFigure (void);
  void editCopy (void);
  void editCut (void);
  void editPaste (void);
  void helpAboutQtHandles (void);
  void updateMenuBar (void);

private:
  Container* m_container;
  bool m_blockUpdates;
  MouseMode m_mouseMode, m_lastMouseMode;
  QToolBar* m_figureToolBar;
  QMenuBar* m_menuBar;
};

//////////////////////////////////////////////////////////////////////////////

}; // namespace QtHandles

//////////////////////////////////////////////////////////////////////////////

#endif
