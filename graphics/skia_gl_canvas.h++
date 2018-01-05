/**
 * The MIT License (MIT)
 *
 * Copyright © 2017-2018 Ruben Van Boxem
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 **/

/*
 * OpenGL-backed graphics canvas for Skia.
 */

#ifndef SKUI_GRAPHICS_SKIA_GL_CANVAS_H
#define SKUI_GRAPHICS_SKIA_GL_CANVAS_H

#include "graphics/skia_canvas.h++"

#include <vector>

class GrGLInterface;
class GrContext;

namespace skui
{
  namespace graphics
  {
    class skia_gl_canvas : public skia_canvas
    {
    public:
      skia_gl_canvas(const pixel_size& size,
                     const GrGLInterface& gl_interface,
                     canvas_flags flags);
      ~skia_gl_canvas() override;

    private:
      std::unique_ptr<GrContext> gr_context;
    };
  }
}

#endif
