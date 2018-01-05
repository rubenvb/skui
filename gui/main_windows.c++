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
 * Windows wrapper function providing WinMain so that users can use a cross-platform int main entry point.
 */

#ifndef SKUI_GUI_MAIN_WINDOWS_H
#define SKUI_GUI_MAIN_WINDOWS_H

#include <codecvt>
#include <cstddef>
#include <locale>
#include <memory>
#include <vector>

#include <core/string.h++>

#define WIN32_MEAN_AND_LEAN
#include <windows.h>
#undef WIN32_MEAN_AND_LEAN

// Hackish solution that works
#ifdef __MINGW32__
int skui_main(int, char*[]);
#define main skui_main
#else
extern "C" int main(int, char*[]);
#endif

int WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
  std::vector<skui::core::string> arg_vector;

  int argc;
  std::unique_ptr<wchar_t*, decltype(&LocalFree)> argz(CommandLineToArgvW(GetCommandLineW(), &argc), &LocalFree);
  if(argz == nullptr)
    std::exit(EXIT_FAILURE);

  arg_vector.reserve(static_cast<std::size_t>(argc));

  // convert to UTF-8
  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> converter;

  for(int i = 1; i<argc; ++i)
  {
    arg_vector.emplace_back(converter.to_bytes(argz.get()[i]));
  }
  std::vector<char*> argv;
  argv.reserve(arg_vector.size()+1);
  for(auto&& arg : arg_vector)
  {
    argv.push_back(&arg[0]);
  }
  argv.push_back(nullptr);

  return main(argc, argv.data());
}

#endif
