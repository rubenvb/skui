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
 * Test
 * Skui testing helpers.
 */

#include <cstdlib>
#include <iostream>

namespace skui
{
  namespace test
  {
    template<typename... ArgTypes>
    void print_error(ArgTypes... args)
    {
      std::cerr << "Test failed: ";
      using expand_variadic_pack = int[];
      (void)expand_variadic_pack{0, ((std::cerr << args), void(), 0)... };
    }

    static int exit_code = EXIT_SUCCESS;
    template<typename... ArgTypes>
    void check(bool passed, ArgTypes... message)
    {
      if(!passed)
      {
        print_error(message..., '\n');
        exit_code = EXIT_FAILURE;
      }
    }
    template<typename... ArgTypes>
    void require(bool passed, ArgTypes... message)
    {
      if(!passed)
      {
        print_error(message..., '\n');
        std::exit(EXIT_FAILURE);
      }
    }
  }
}
