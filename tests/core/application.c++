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

#include "test.h++"

#include "core/application.h++"

#include <thread>

namespace
{
  using skui::test::check;
  using skui::test::require;

  void test_arguments()
  {
    const auto& arguments = skui::core::application::instance().commandline_arguments;

    require(arguments.size() == 1, "correct number of arguments.");
    check(arguments[0] == "skui is awesome", "arguments passed correctly.");
  }

  void test_execute_and_quit()
  {
    auto app = &skui::core::application::instance();
    constexpr int exit_code = 1;
    int return_value = 0;
    std::thread t([app, &return_value] { return_value = app->execute(); });

    app->quit(exit_code);

    t.join();

    check(return_value == exit_code, "application::execute returns exit code passed to quit().");
  }
}

int main(int argc, char* argv[])
{
  const skui::core::application app(argc, argv);

  test_arguments();
  test_execute_and_quit();

  return skui::test::exit_code;
}
