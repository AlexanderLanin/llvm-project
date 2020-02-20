// RUN: %clang_cc1 -fsyntax-only -Wdocumentation -Wdocumentation-pedantic -fcomment-block-commands=foobar -verify %s
// RUN  %clang_cc1 -std=c++11 -fsyntax-only -Wdocumentation -Wdocumentation-pedantic -fcomment-block-commands=foobar -fdiagnostics-parseable-fixits %s 2>&1 | FileCheck -DATTRIBUTE="__attribute__((deprecated))" %s
// RUN: %clang_cc1 -std=c++14 -fsyntax-only -Wdocumentation -Wdocumentation-pedantic -fcomment-block-commands=foobar -fdiagnostics-parseable-fixits %s 2>&1 | FileCheck --check-prefixes=CHECK,CHECK14 -DATTRIBUTE="[[deprecated]]" %s

// expected-warning@+2 {{parameter 'ZZZZZZZZZZ' not found in the function declaration}} expected-note@+2 {{did you mean 'a'?}}
// CHECK: fix-it:"{{.*}}":{[[@LINE+1]]:12-[[@LINE+1]]:22}:"a"
/// \param ZZZZZZZZZZ Blah blah.
int test1(int a);

// expected-warning@+2 {{parameter 'aab' not found in the function declaration}} expected-note@+2 {{did you mean 'aaa'?}}
// CHECK: fix-it:"{{.*}}":{[[@LINE+1]]:12-[[@LINE+1]]:15}:"aaa"
/// \param aab Blah blah.
int test2(int aaa, int bbb);

// expected-warning@+3 {{parameter 'aab' not found in the function declaration}}
// CHECK-NOT: fix-it:"{{.*}}":{[[@LINE+2]]:
/// \param aaa Documented
/// \param aab Do not duplicate documentation of aaa.
int test2to1_oneWasOk(int aaa);

// expected-warning@+4 {{parameter 'aaaa' not found in the function declaration}} expected-note@+4 {{did you mean 'aaa'?}}
// expected-warning@+4 {{parameter 'ThisOneHasBeenRemovedButIsStillDocumented' not found in the function declaration}} expected-note-NOT@+4 {{did you mean}}
// CHECK: fix-it:"{{.*}}":{[[@LINE+2]]:
// CHECK-NOT: fix-it:"{{.*}}":{[[@LINE+2]]:
/// \param aaaa Almost there!
/// \param ThisOneHasBeenRemovedButIsStillDocumented Auto fix only for small spelling mistakes.
int test2to1_fixBestMatch(int aaa);

// expected-warning@+4 {{parameter 'aaab' not found in the function declaration}} expected-note-NOT@+4 {{did you mean}}
// expected-warning@+4 {{parameter 'aaac' not found in the function declaration}} expected-note-NOT@+4 {{did you mean}}
// CHECK-NOT: fix-it:"{{.*}}":{[[@LINE+2]]:
// CHECK-NOT: fix-it:"{{.*}}":{[[@LINE+2]]:
/// \param aaab Almost there!
/// \param aaac Me too!
int test2to1_noWinner(int aaa);

// Auto-fixing those two parameters would potentially worthen the situation as information is lost.
// At the moment they at least have different names and a human can resonably pick the correct one by name.
// expected-warning@+4 {{parameter 'arg1' not found in the function declaration}} expected-note-NOT@+4 {{did you mean}}
// expected-warning@+4 {{parameter 'arg2' not found in the function declaration}} expected-note-NOT@+4 {{did you mean}}
// CHECK-NOT: fix-it:"{{.*}}":{[[@LINE+2]]:
// CHECK-NOT: fix-it:"{{.*}}":{[[@LINE+2]]:
/// \param arg1 Number of elements.
/// \param arg2 Array of elements (int for simplicity here).
int test2to2(int argc, int argv);

// expected-warning@+2 {{template parameter 'ZZZZZZZZZZ' not found in the template declaration}} expected-note@+2 {{did you mean 'T'?}}
// CHECK: fix-it:"{{.*}}":{[[@LINE+1]]:13-[[@LINE+1]]:23}:"T"
/// \tparam ZZZZZZZZZZ Aaa
template<typename T>
void test3(T aaa);

// expected-warning@+2 {{template parameter 'SomTy' not found in the template declaration}} expected-note@+2 {{did you mean 'SomeTy'?}}
// CHECK: fix-it:"{{.*}}":{[[@LINE+1]]:13-[[@LINE+1]]:18}:"SomeTy"
/// \tparam SomTy Aaa
/// \tparam OtherTy Bbb
template<typename SomeTy, typename OtherTy>
void test4(SomeTy aaa, OtherTy bbb);

// expected-warning@+2 {{declaration is marked with '\deprecated' command but does not have a deprecation attribute}} expected-note@+3 {{add a deprecation attribute to the declaration to silence this warning}}
// CHECK: fix-it:"{{.*}}":{[[@LINE+2]]:1-[[@LINE+2]]:1}:"[[ATTRIBUTE]] "
/// \deprecated
void test_deprecated_1();

// expected-warning@+2 {{declaration is marked with '\deprecated' command but does not have a deprecation attribute}} expected-note@+3 {{add a deprecation attribute to the declaration to silence this warning}}
// CHECK: fix-it:"{{.*}}":{[[@LINE+2]]:1-[[@LINE+2]]:1}:"[[ATTRIBUTE]] "
/// \deprecated
void test_deprecated_2(int a);

struct test_deprecated_3 {
  // expected-warning@+2 {{declaration is marked with '\deprecated' command but does not have a deprecation attribute}} expected-note@+3 {{add a deprecation attribute to the declaration to silence this warning}}
  // CHECK: fix-it:"{{.*}}":{[[@LINE+2]]:3-[[@LINE+2]]:3}:"[[ATTRIBUTE]] "
  /// \deprecated
  void test_deprecated_4();

  // expected-warning@+2 {{declaration is marked with '\deprecated' command but does not have a deprecation attribute}} expected-note@+3 {{add a deprecation attribute to the declaration to silence this warning}}
  // CHECK: fix-it:"{{.*}}":{[[@LINE+2]]:3-[[@LINE+2]]:3}:"[[ATTRIBUTE]] "
  /// \deprecated
  void test_deprecated_5() {
  }
};

template<typename T>
struct test_deprecated_6 {
  // expected-warning@+2 {{declaration is marked with '\deprecated' command but does not have a deprecation attribute}} expected-note@+3 {{add a deprecation attribute to the declaration to silence this warning}}
  // CHECK: fix-it:"{{.*}}":{[[@LINE+2]]:3-[[@LINE+2]]:3}:"[[ATTRIBUTE]] "
  /// \deprecated
  void test_deprecated_7();

  // expected-warning@+2 {{declaration is marked with '\deprecated' command but does not have a deprecation attribute}} expected-note@+3 {{add a deprecation attribute to the declaration to silence this warning}}
  // CHECK: fix-it:"{{.*}}":{[[@LINE+2]]:3-[[@LINE+2]]:3}:"[[ATTRIBUTE]] "
  /// \deprecated
  void test_deprecated_8() {
  }
};

class PR43753 {
  // expected-warning@+3 {{declaration is marked with '\deprecated' command but does not have a deprecation attribute}}
  // expected-note@+3 {{add a deprecation attribute to the declaration to silence this warning}}
  // CHECK: fix-it:"{{.*}}":{[[@LINE+2]]:3-[[@LINE+2]]:3}:"[[ATTRIBUTE]] "
  /// \deprecated
  static void test_deprecated_static();

  // expected-warning@+3 {{declaration is marked with '\deprecated' command but does not have a deprecation attribute}}
  // expected-note@+3 {{add a deprecation attribute to the declaration to silence this warning}}
  // CHECK: fix-it:"{{.*}}":{[[@LINE+2]]:3-[[@LINE+2]]:3}:"[[ATTRIBUTE]] "
  /// \deprecated
  static auto test_deprecated_static_trailing_return() -> int;

#if __cplusplus >= 201402L
  // expected-warning@+2 {{declaration is marked with '\deprecated' command but does not have a deprecation attribute}}
  // expected-note@+2 {{add a deprecation attribute to the declaration to silence this warning}}
  /// \deprecated
  static decltype(auto) test_deprecated_static_decltype_auto() { return 1; }
#endif

  // expected-warning@+2 {{declaration is marked with '\deprecated' command but does not have a deprecation attribute}}
  // expected-note@+2 {{add a deprecation attribute to the declaration to silence this warning}}
  /// \deprecated
  void test_deprecated_const() const;

  // expected-warning@+2 {{declaration is marked with '\deprecated' command but does not have a deprecation attribute}}
  // expected-note@+2 {{add a deprecation attribute to the declaration to silence this warning}}
  /// \deprecated
  auto test_deprecated_trailing_return() -> int;

#if __cplusplus >= 201402L
  // expected-warning@+3 {{declaration is marked with '\deprecated' command but does not have a deprecation attribute}}
  // expected-note@+3 {{add a deprecation attribute to the declaration to silence this warning}}
  // CHECK14: fix-it:"{{.*}}":{[[@LINE+2]]:3-[[@LINE+2]]:3}:"[[ATTRIBUTE]] "
  /// \deprecated
  decltype(auto) test_deprecated_decltype_auto() const { return a; }

private:
  int a{0};
#endif
};
#define MY_ATTR_DEPRECATED __attribute__((deprecated))

// expected-warning@+2 {{declaration is marked with '\deprecated' command but does not have a deprecation attribute}} expected-note@+3 {{add a deprecation attribute to the declaration to silence this warning}}
// CHECK: fix-it:"{{.*}}":{[[@LINE+2]]:1-[[@LINE+2]]:1}:"MY_ATTR_DEPRECATED "
/// \deprecated
void test_deprecated_9(int a);

#if __cplusplus >= 201402L
#define ATTRIBUTE_DEPRECATED [[deprecated]]

// expected-warning@+2 {{declaration is marked with '\deprecated' command but does not have a deprecation attribute}} expected-note@+3 {{add a deprecation attribute to the declaration to silence this warning}}
// CHECK14: fix-it:"{{.*}}":{[[@LINE+2]]:1-[[@LINE+2]]:1}:"ATTRIBUTE_DEPRECATED "
/// \deprecated
void test_deprecated_10(int a);
#endif

// rdar://12381408
// expected-warning@+3  {{unknown command tag name 'retur'; did you mean 'return'?}}
// CHECK: fix-it:"{{.*}}":{[[@LINE+2]]:6-[[@LINE+2]]:11}:"return"
/// \brief testing fixit
/// \retur int in FooBar
int FooBar();

// expected-warning@+2  {{unknown command tag name 'fooba'; did you mean 'foobar'?}}
// CHECK: fix-it:"{{.*}}":{[[@LINE+1]]:6-[[@LINE+1]]:11}:"foobar"
/// \fooba bbb IS_DOXYGEN_END
int gorf();

// expected-warning@+1 {{unknown command tag name}}
/// \t bbb IS_DOXYGEN_END
int Bar();

// expected-warning@+3  {{unknown command tag name 'encode'; did you mean 'endcode'?}}
// expected-warning@+2  {{'\endcode' command does not terminate a verbatim text block}}
// CHECK: fix-it:"{{.*}}":{[[@LINE+1]]:6-[[@LINE+1]]:12}:"endcode"
/// \encode PR18051
int PR18051();
