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
 * Application
 * Common implementation.
 */

#include "core/application.h++"

#include <cstdlib>
#include <iostream>

namespace skui
{
  namespace core
  {
    namespace implementation
    {
      static application* instance = nullptr;
    }

    application::application(int argc, char* argv[], string name)
      : commandline_arguments{argv+1, argv+argc}
      , name(std::move(name))
    {
      if(implementation::instance != nullptr)
      {
        std::cerr << "skui::core::application: creating a second application instance is not allowed.\n";
        implementation::instance->quit();
      }
      implementation::instance = this;
    }

    application& application::instance()
    {
      if(implementation::instance == nullptr)
      {
        std::cerr << "skui::core::application::instance: application instance not created.\n";
        std::exit(EXIT_FAILURE);
      }
      return *implementation::instance;
    }

    int application::execute()
    {
      return event_loop.execute();
    }

    void application::quit(int exit_code)
    {
      about_to_quit.emit();
      event_loop.stop(exit_code);
    }

    const std::vector<string>& application::arguments() const
    {
      return commandline_arguments;
    }
  }
}
