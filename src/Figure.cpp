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
#include <QActionEvent>
#include <QActionGroup>
#include <QApplication>
#include <QEvent>
#include <QFrame>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QTimer>
#include <QToolBar>

#include "Canvas.h"
#include "Container.h"
#include "Figure.h"
#include "FigureWindow.h"
#include "MouseModeActionGroup.h"
#include "Utils.h"

//////////////////////////////////////////////////////////////////////////////

namespace QtHandles
{

//////////////////////////////////////////////////////////////////////////////

#define ABOUT_TEXT "<b>QtHandles</b> - a Qt-based toolkit for <a href=\"http://www.octave.org\">Octave</a>.<br><br>Copyright (C) 2011 Michael Goffioul."

//////////////////////////////////////////////////////////////////////////////

static bool hasUiControlChildren (const figure::properties& fp)
{
  Matrix kids = fp.get_all_children ();

  for (int i = 0; i < kids.numel (); i++)
    {
      graphics_object go (gh_manager::get_object (kids(i)));

      if (go && (go.isa ("uicontrol") || go.isa ("uipanel")
		 || go.isa ("uibuttongroup")))
	return true;
    }

  return false;
}

//////////////////////////////////////////////////////////////////////////////

static bool hasUiMenuChildren (const figure::properties& fp)
{
  Matrix kids = fp.get_all_children ();

  for (int i = 0; i < kids.numel (); i++)
    {
      graphics_object go (gh_manager::get_object (kids(i)));

      if (go && go.isa ("uimenu"))
	return true;
    }

  return false;
}

//////////////////////////////////////////////////////////////////////////////

Figure* Figure::create (const graphics_object& go)
{
  return new Figure (go, new FigureWindow ());
}

//////////////////////////////////////////////////////////////////////////////

Figure::Figure (const graphics_object& go, QMainWindow* win)
     : Object (go, win), m_blockUpdates (false), m_mouseMode (NoMode),
       m_lastMouseMode (NoMode), m_figureToolBar (0)
{
  m_container = new Container (win);
  win->setCentralWidget (m_container);

  figure::properties& fp = properties<figure> ();

  createFigureToolBarAndMenuBar ();

  int offset = 0;
  if (fp.toolbar_is ("figure")
      || (fp.toolbar_is ("auto") && ! hasUiControlChildren (fp)))
    offset += m_figureToolBar->sizeHint ().height ();
  else
    m_figureToolBar->hide ();
  if (fp.menubar_is ("figure") || hasUiMenuChildren (fp))
    offset += m_menuBar->sizeHint ().height () + 1;
  else
    m_menuBar->hide ();

  Matrix bb = fp.get_boundingbox (true);
  win->setGeometry (xround (bb(0)), xround (bb(1)) - offset,
		    xround (bb(2)), xround (bb(3)) + offset);
  win->setWindowTitle (Utils::fromStdString (fp.get_title ()));

  if (fp.is_visible ())
    QTimer::singleShot (0, win, SLOT (show ()));
  else
    win->hide ();

  win->installEventFilter (this);
  m_container->installEventFilter (this);
}

//////////////////////////////////////////////////////////////////////////////

Figure::~Figure (void)
{
}

//////////////////////////////////////////////////////////////////////////////

void Figure::createFigureToolBarAndMenuBar (void)
{
  QMainWindow* win = qWidget<QMainWindow> ();

  m_figureToolBar = win->addToolBar (tr ("Figure ToolBar"));
  m_figureToolBar->setMovable (false);
  m_figureToolBar->setFloatable (false);

  MouseModeActionGroup* mouseModeGroup = new MouseModeActionGroup (win);
  connect (mouseModeGroup, SIGNAL (modeChanged (MouseMode)),
	   SLOT (setMouseMode (MouseMode)));
  m_figureToolBar->addActions (mouseModeGroup->actions ());

  m_menuBar = win->menuBar ();

  QMenu* fileMenu = m_menuBar->addMenu (tr ("&File"));
  fileMenu->menuAction ()->setObjectName ("builtinMenu");
  fileMenu->addAction (tr ("&New Figure"), this, SLOT (fileNewFigure (void)));
  fileMenu->addAction (tr ("&Open..."))->setEnabled (false);
  fileMenu->addSeparator ();
  fileMenu->addAction (tr ("&Save"))->setEnabled (false);
  fileMenu->addAction (tr ("Save &As"))->setEnabled (false);
  fileMenu->addSeparator ();
  fileMenu->addAction (tr ("&Close Figure"), this,
		       SLOT (fileCloseFigure (void)), Qt::CTRL|Qt::Key_W);

  QMenu* editMenu = m_menuBar->addMenu (tr ("&Edit"));
  editMenu->menuAction ()->setObjectName ("builtinMenu");
  editMenu->addAction (tr ("Cop&y"), this, SLOT (editCopy (void)),
		       Qt::CTRL|Qt::Key_C)->setEnabled (false);
  editMenu->addAction (tr ("Cu&t"), this, SLOT (editCut (void)),
		       Qt::CTRL|Qt::Key_X)->setEnabled (false);
  editMenu->addAction (tr ("&Paste"), this, SLOT (editPaste(void)),
		       Qt::CTRL|Qt::Key_V)->setEnabled (false);
  editMenu->addSeparator ();
  editMenu->addActions (mouseModeGroup->actions ());

  QMenu* helpMenu = m_menuBar->addMenu (tr ("&Help"));
  helpMenu->menuAction ()->setObjectName ("builtinMenu");
  helpMenu->addAction (tr ("&About QtHandles"), this,
		       SLOT (helpAboutQtHandles (void)));
  helpMenu->addAction (tr ("About &Qt"), qApp, SLOT (aboutQt (void)));

  m_menuBar->installEventFilter (this);
}

//////////////////////////////////////////////////////////////////////////////

Container* Figure::innerContainer (void)
{
  return m_container;
}

//////////////////////////////////////////////////////////////////////////////

void Figure::redraw (void)
{
  Canvas* canvas = m_container->canvas (m_handle);

  if (canvas)
    {
    canvas->redraw ();
    //canvas->setMouseMode (RotateMode);
    }

  foreach (QFrame* frame,
	   qWidget<QWidget> ()->findChildren<QFrame*> ("UIPanel"))
    {
      Object* obj = Object::fromQObject (frame);

      if (obj)
	obj->slotRedraw ();
    }
}

//////////////////////////////////////////////////////////////////////////////

void Figure::beingDeleted (void)
{
  Canvas* canvas = m_container->canvas (m_handle.value (), false);

  if (canvas)
    canvas->blockRedraw (true);
  m_menuBar->removeEventFilter (this);
}

//////////////////////////////////////////////////////////////////////////////

void Figure::update (int pId)
{
  if (m_blockUpdates)
    return;

  figure::properties& fp = properties<figure> ();
  QMainWindow* win = qWidget<QMainWindow> ();

  m_blockUpdates = true;

  switch (pId)
    {
    case figure::properties::ID_POSITION:
	{
	  Matrix bb = fp.get_boundingbox (true);
	  int offset = 0;

	  if (m_figureToolBar->isVisible ())
	    offset = m_figureToolBar->sizeHint ().height ();
	  if (m_menuBar->isVisible ())
	    offset += m_menuBar->sizeHint ().height () + 1;
	  win->setGeometry (xround (bb(0)), xround (bb(1)) - offset,
			    xround (bb(2)), xround (bb(3)) + offset);
	}
      break;
    case figure::properties::ID_NAME:
    case figure::properties::ID_NUMBERTITLE:
      win->setWindowTitle (Utils::fromStdString (fp.get_title ()));
      break;
    case figure::properties::ID_VISIBLE:
      if (fp.is_visible ())
	QTimer::singleShot (0, win, SLOT (show ()));
      else
	win->hide ();
      break;
    case figure::properties::ID_TOOLBAR:
      if (fp.toolbar_is ("none"))
	showFigureToolBar (false);
      else if (fp.toolbar_is ("figure"))
	showFigureToolBar (true);
      else // "auto"
	showFigureToolBar (! hasUiControlChildren (fp));
      break;
    case figure::properties::ID_MENUBAR:
      showMenuBar (fp.menubar_is ("figure"));
      break;
    default:
      break;
    }

  m_blockUpdates = false;
}

//////////////////////////////////////////////////////////////////////////////

void Figure::showFigureToolBar (bool visible)
{
  if (m_figureToolBar->isVisible () != visible)
    {
      int dy = m_figureToolBar->sizeHint ().height ();
      QRect r = qWidget<QWidget> ()->geometry ();

      if (! visible)
	r.adjust (0, dy, 0, 0);
      else
	r.adjust (0, -dy, 0, 0);

      m_blockUpdates = true;
      qWidget<QWidget> ()->setGeometry (r);
      m_figureToolBar->setVisible (visible);
      m_blockUpdates = false;

      updateBoundingBox (false);

      if (visible)
	m_mouseMode = m_lastMouseMode;
      else
	{
	  m_lastMouseMode = m_mouseMode;
	  m_mouseMode = NoMode;
	}
    }
}

//////////////////////////////////////////////////////////////////////////////

void Figure::showMenuBar (bool visible)
{
  int h1 = m_menuBar->sizeHint ().height ();

  foreach (QAction* a, m_menuBar->actions ())
    if (a->objectName () == "builtinMenu")
      a->setVisible (visible);

  int h2 = m_menuBar->sizeHint ().height ();

  if (! visible)
    visible = hasUiMenuChildren (properties<figure> ());

  if (m_menuBar->isVisible () != visible)
    {
      int dy = qMax (h1, h2) + 1;
      QRect r = qWidget<QWidget> ()->geometry ();

      if (! visible)
	r.adjust (0, dy, 0, 0);
      else
	r.adjust (0, -dy, 0, 0);

      m_blockUpdates = true;
      qWidget<QWidget> ()->setGeometry (r);
      m_menuBar->setVisible (visible);
      m_blockUpdates = false;

      updateBoundingBox (false);
    }
}

//////////////////////////////////////////////////////////////////////////////

void Figure::updateMenuBar (void)
{
  gh_manager::auto_lock lock;
  graphics_object go = object ();

  if (go.valid_object ())
    showMenuBar (Utils::properties<figure> (go).menubar_is ("figure"));
}

//////////////////////////////////////////////////////////////////////////////

QWidget* Figure::menu (void)
{
  return qWidget<QMainWindow> ()->menuBar ();
}

//////////////////////////////////////////////////////////////////////////////

struct UpdateBoundingBoxData
{
  Matrix m_bbox;
  bool m_internal;
  graphics_handle m_handle;
};

void Figure::updateBoundingBoxHelper (void* data)
{
  gh_manager::auto_lock lock;

  UpdateBoundingBoxData* d = reinterpret_cast<UpdateBoundingBoxData*> (data);
  graphics_object go = gh_manager::get_object (d->m_handle);

  if (go.valid_object ())
    {
      figure::properties& fp = Utils::properties<figure> (go);

      fp.set_boundingbox (d->m_bbox, d->m_internal, false);
    }

  delete d;
}

//////////////////////////////////////////////////////////////////////////////

void Figure::updateBoundingBox (bool internal)
{
  QWidget* win = qWidget<QWidget> ();
  Matrix bb (1, 4);

  if (internal)
    {
      QPoint pt = win->mapToGlobal (m_container->pos ());

      bb(0) = pt.x ();
      bb(1) = pt.y ();
      bb(2) = m_container->width ();
      bb(3) = m_container->height ();
    }
  else
    {
      bb(0) = win->x ();
      bb(1) = win->y ();
      bb(2) = win->frameGeometry ().width ();
      bb(3) = win->frameGeometry ().height ();
    }

  UpdateBoundingBoxData* d = new UpdateBoundingBoxData ();

  d->m_bbox = bb;
  d->m_internal = internal;
  d->m_handle = m_handle;

  gh_manager::post_function (Figure::updateBoundingBoxHelper, d);
}

//////////////////////////////////////////////////////////////////////////////

bool Figure::eventFilter (QObject* obj, QEvent* event)
{
  if (! m_blockUpdates)
    {
      if (obj == m_container)
	{
	  switch (event->type ())
	    {
	    case QEvent::Resize:
	      updateBoundingBox (true);
	      break;
	    case QEvent::ChildAdded:
	      if (dynamic_cast<QChildEvent*> (event)->child
		  ()->isWidgetType())
		{
		  gh_manager::auto_lock lock;
		  const figure::properties& fp = properties<figure> ();

		  showFigureToolBar (! hasUiControlChildren (fp));
		}
	    default:
	      break;
	    }
	}
      else if (obj == m_menuBar)
	{
	  switch (event->type ())
	    {
	    case QEvent::ActionAdded:
	    case QEvent::ActionRemoved:
		{
		  QAction* a = dynamic_cast<QActionEvent*> (event)->action ();

		  if (! a->isSeparator ()
		      && a->objectName () != "builtinMenu")
		    {
		      // To get the right sizeHint() value in showMenuBar, it
		      // must be executed *after* action addition and *before*
		      // action removal.
		      if (event->type () == QEvent::ActionAdded)
			QTimer::singleShot (0, this,
					    SLOT (updateMenuBar (void)));
		      else
			updateMenuBar ();
		    }
		}
	      break;
	    default:
	      break;
	    }
	}
      else
	{
	  switch (event->type ())
	    {
	    case QEvent::Move:
	      updateBoundingBox (true);
	      updateBoundingBox (false);
	      break;
	    case QEvent::Close:
	      event->ignore ();
	      gh_manager::post_callback (m_handle, "closerequestfcn");
	      return true;
	    case QEvent::Resize:
	      updateBoundingBox (false);
	      break;
	    default:
	      break;
	    }
	}
    }

  return false;
}

//////////////////////////////////////////////////////////////////////////////

void Figure::helpAboutQtHandles (void)
{
  QMessageBox::about (qWidget<QMainWindow> (), tr ("About QtHandles"),
		      ABOUT_TEXT);
}

//////////////////////////////////////////////////////////////////////////////

void Figure::fileNewFigure (void)
{
}

//////////////////////////////////////////////////////////////////////////////

void Figure::fileCloseFigure (void)
{
  qWidget<QMainWindow> ()->close ();
}

//////////////////////////////////////////////////////////////////////////////

void Figure::editCopy (void)
{
}

//////////////////////////////////////////////////////////////////////////////

void Figure::editCut (void)
{
}

//////////////////////////////////////////////////////////////////////////////

void Figure::editPaste (void)
{
}

//////////////////////////////////////////////////////////////////////////////

}; // namespace QtHandles
