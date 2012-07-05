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

#ifndef __QtHandles_gl_selector__
#define __QtHandles_gl_selector__ 1

#include <octave/gl-render.h>

#include <map>

enum select_flags
{
  select_ignore_hittest  = 0x01,
  select_last            = 0x02
};

class opengl_selector : public opengl_renderer
{
public:
  opengl_selector (void) : size (5) { }

  virtual ~opengl_selector (void) { }

  graphics_object select (const graphics_object& ax, int x, int y,
                          int flags = 0);

  virtual void draw (const graphics_object& go, bool toplevel = true);

protected:
  virtual void draw_text (const text::properties& props);

  virtual void setup_opengl_transformation (const axes::properties& props);

  virtual void init_marker (const std::string& m, double size, float width);

private:
  void apply_pick_matrix (void);

private:
  // The mouse coordinate of the selection/picking point
  int xp, yp;

  // The size (in pixels) of the picking window
  int size;

  // The OpenGL name mapping
  std::map<GLuint, graphics_object> object_map;
};

#endif // __QtHandles_gl_selector__
