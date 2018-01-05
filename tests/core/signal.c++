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

#include "core/signal.h++"

namespace
{
  using skui::test::check;

  bool free_slot_called = false;
  void slot() { free_slot_called = true; }

  bool overloaded_slot_called = false;
  void slot(bool) { overloaded_slot_called = true; }

  void test_signal_connect_emit()
  {
    bool lambda_slot_called = false;
    auto lambda_slot = [&lambda_slot_called]() { lambda_slot_called = true; };

    skui::core::signal<> signal;
    signal.connect(static_cast<void(*)()>(slot));
    signal.connect(lambda_slot);

    signal.emit();
    check(free_slot_called, "Free function slot called.");
    check(lambda_slot_called, "Lambda function slot called.");
    check(!overloaded_slot_called, "Overloaded slot not called.");

    skui::core::signal<bool> signal_bool;
    signal_bool.connect(static_cast<void(*)(bool)>(slot));
    signal_bool.emit(true);

    check(overloaded_slot_called, "Overloaded slot called.");

    skui::core::signal<int> signal_int;
    signal_int.connect(lambda_slot);
    lambda_slot_called = false;
    signal_int.emit(5);

    check(lambda_slot_called, "Slot with less arguments called.");
  }

  void test_signal_copy_move()
  {
    bool slot_called = false;
    auto slot = [&slot_called]() { slot_called = true; };
    skui::core::signal<> signal_one;
    skui::core::signal<> signal_two;

    signal_one.connect(slot);

    signal_two = signal_one;

    signal_one.emit();

    check(slot_called, "Copied-from signal still connected.");

    slot_called = false;

    signal_two.emit();

    check(slot_called, "Copied-to signal connected.");

    signal_two = std::move(signal_one);
    slot_called = false;

    signal_one.emit();

    check(!slot_called, "Moved-from signal disconnected.");

    signal_two.emit();

    check(slot_called, "Moved-to signal connected correclty.");
  }

  void test_signal_with_argument()
  {
    bool slot_called = false;
    auto slot = [&slot_called](bool called) { slot_called = called; };

    skui::core::signal<bool> signal;
    signal.connect(slot);

    signal.emit(true);

    check(slot_called, "Argument passed through signal.");
  }

  void test_signal_disconnect()
  {
    free_slot_called = false;
    bool other_slot_called = false;
    auto other_slot = [&other_slot_called]() { other_slot_called = true; };

    skui::core::signal<> signal;
    auto slot_connection = signal.connect(static_cast<void(*)()>(&slot));
    signal.connect(other_slot);
    signal.disconnect(slot_connection);

    signal.emit();
    skui::test::check(!free_slot_called, "Slot disconnected.");
    skui::test::check(other_slot_called, "Other slot still connected.");

    signal.disconnect_all();

    other_slot_called = false;
    signal.emit();

    skui::test::check(!other_slot_called, "Disconnect all slots.");
  }

  struct parent : public skui::core::trackable
  {
    virtual ~parent() = default;
    virtual void f() const {};
    virtual void g(int) const = 0;
    virtual void h() { slot_called = true; }

    mutable bool slot_called = false;
  };
  struct mock : public parent
  {
    mock() = default;
    void f() const override { slot_called = true; }
    void g(int) const override { slot_called = true; }
  };

  void test_member_functions()
  {
    {
      mock object;
      skui::core::signal<> signal;
      signal.connect(&object, &mock::f);

      signal.emit();
      check(object.slot_called, "member function slot called.");
    }
    {
      mock object;
      skui::core::signal<int> signal_int;
      signal_int.connect(&object, &mock::g);

      signal_int.emit(0);
      check(object.slot_called, "member function with argument called");
    }
    {
      mock object;
      skui::core::signal<int> signal_int;
      signal_int.connect(&object, &mock::f);

      signal_int.emit(0);
      check(object.slot_called, "member function with argument less called");
    }
    {
      mock object;
      skui::core::signal<> signal;
      signal.connect(&object, &mock::h);

      signal.emit();
      check(object.slot_called, "not overridden virtual function called.");
    }
    {
      mock object;
      skui::core::signal<> signal;
      signal.connect(&object, &parent::f);

      signal.emit();
      check(object.slot_called, "overridden virtual function called through function pointer to base member function");
    }
    {
      mock object;
      parent& parent_object = object;
      skui::core::signal<> signal;
      signal.connect(&parent_object, &parent::f);

      signal.emit();
      check(object.slot_called, "overridden virtual function called through function pointer to base member function with base class pointer");
    }
    {
      mock object;
      mock other_object;
      skui::core::signal<> signal;
      signal.connect(&object, &mock::f);
      signal.connect(&other_object, &mock::f);
      signal.disconnect(&other_object);

      signal.emit();
      check(object.slot_called, "connected slot called");
      check(!other_object.slot_called, "disconnected slot not called");
    }
  }

  void test_deleted_connected_object()
  {
    mock mock_object;
    auto mock_ptr = std::make_unique<mock>();
    skui::core::signal<> signal;
    signal.connect(&mock_object, &mock::f);
    signal.connect(mock_ptr.get(), &mock::f);

    mock_ptr.reset();
    signal.emit();
    check(mock_object.slot_called, "signal disconnects on object delete");
  }

  void test_relay_signal()
  {
    skui::core::signal<> signal;
    skui::core::signal<> receiving;
    bool receiving_called = false;

    receiving.connect([&receiving_called] { receiving_called = true; });

    signal.relay(receiving);

    signal.emit();

    check(receiving_called, "Signal connected to signal was called.");
  }
}

int main()
{
  test_signal_connect_emit();
  test_signal_copy_move();
  test_signal_with_argument();
  test_signal_disconnect();
  test_member_functions();
  test_deleted_connected_object();
  test_relay_signal();

  return skui::test::exit_code;
}
