// RUN: %clang_cc1 -fsyntax-only -Wdocumentation -Wdocumentation-pedantic -fcomment-block-commands=foobar -verify %s
// RUN  %clang_cc1 -std=c++11 -fsyntax-only -Wdocumentation -Wdocumentation-pedantic -fcomment-block-commands=foobar -fdiagnostics-parseable-fixits %s 2>&1 | FileCheck -DATTRIBUTE="__attribute__((deprecated))" %s
// RUN: %clang_cc1 -std=c++14 -fsyntax-only -Wdocumentation -Wdocumentation-pedantic -fcomment-block-commands=foobar -fdiagnostics-parseable-fixits %s 2>&1 | FileCheck --check-prefixes=CHECK,CHECK14 -DATTRIBUTE="[[deprecated]]" %s

// expected-warning@+2 {{parameter 'ZZZZZZZZZZ' not found in the function declaration}} expected-note@+2 {{did you mean 'a'?}}
// CHECK: fix-it:"{{.*}}":{[[@LINE+1]]:12-[[@LINE+1]]:22}:"a"
/// \param ZZZZZZZZZZ Blah blah.
int fixSinceThereIsOnlyOneOption(int a);

// expected-warning@+2 {{parameter 'aab' not found in the function declaration}} expected-note@+2 {{did you mean 'aaa'?}}
// CHECK: fix-it:"{{.*}}":{[[@LINE+1]]:12-[[@LINE+1]]:15}:"aaa"
/// \param aab Blah blah.
int fixParamCommentToBestMatch(int aaa, int bbb);

// expected-warning@+3 {{parameter 'aab' not found in the function declaration}} expected-note-NOT@+3 {{did you mean}}
// CHECK-NOT: fix-it:"{{.*}}":{[[@LINE+2]]:
/// \param aaa Documented
/// \param aab Do not duplicate documentation of aaa.
int doNotDuplicateExistingComments(int aaa);

// expected-warning@+4 {{parameter 'aaaa' not found in the function declaration}} expected-note@+4 {{did you mean 'aaa'?}}
// expected-warning@+4 {{parameter 'ThisOneHasBeenRemovedButIsStillDocumented' not found in the function declaration}} expected-note-NOT@+4 {{did you mean}}
// CHECK: fix-it:"{{.*}}":{[[@LINE+2]]:
// CHECK-NOT: fix-it:"{{.*}}":{[[@LINE+2]]:
/// \param aaaa Almost there.
/// \param ThisOneHasBeenRemovedButIsStillDocumented Widely different name!
int doNotDuplicateCommentsViaFixIt(int aaa);

// Test data and expectations from https://bugs.llvm.org/show_bug.cgi?id=43755.
// Note: there is a an intentional typo in sligtlyMisspelled.
// expected-warning@+4 {{parameter 'sligtlyMisspelled' not found in the function declaration}} expected-note@+4 {{did you mean 'slightlyMisspelled'?}}
// expected-warning@+4 {{parameter 'someParameterWhichWasRemovedLongAgo' not found in the function declaration}} expected-note-NOT@+4 {{did you mean}}
// CHECK: fix-it:"{{.*}}":{[[@LINE+2]]:
// CHECK-NOT: fix-it:"{{.*}}":{[[@LINE+2]]:
/// @param sligtlyMisspelled hello
/// @param someParameterWhichWasRemovedLongAgo world
void doNotDuplicateCommentsViaFixIt_43755(int slightlyMisspelled);

// expected-warning@+6 {{parameter 'aaaa' not found in the function declaration}} expected-note-NOT@+6 {{did you mean}}
// expected-warning@+6 {{parameter 'aaab' not found in the function declaration}} expected-note-NOT@+6 {{did you mean}}
// expected-warning@+6 {{parameter 'aaac' not found in the function declaration}} expected-note-NOT@+6 {{did you mean}}
// CHECK-NOT: fix-it:"{{.*}}":{[[@LINE+3]]:
// CHECK-NOT: fix-it:"{{.*}}":{[[@LINE+3]]:
// CHECK-NOT: fix-it:"{{.*}}":{[[@LINE+3]]:
/// \param aaaa Almost there!
/// \param aaab Me too!
/// \param aaac Me three!
int onlyFixWhenExactlyOneBestParamComment(int aaa);

// expected-warning@+6 {{parameter 'aaab' not found in the function declaration}} expected-note-NOT@+6 {{did you mean}}
// expected-warning@+6 {{parameter 'aabb' not found in the function declaration}} expected-note-NOT@+6 {{did you mean}}
// expected-warning@+6 {{parameter 'abbb' not found in the function declaration}} expected-note-NOT@+6 {{did you mean}}
// CHECK-NOT: fix-it:"{{.*}}":{[[@LINE+3]]:
// CHECK-NOT: fix-it:"{{.*}}":{[[@LINE+3]]:
// CHECK-NOT: fix-it:"{{.*}}":{[[@LINE+3]]:
/// \param aaab Almost there!
/// \param aabb Me too!
/// \param abbb Me three!
int onlyFixWhenExactlyOneBestParamComment_WithUnrelatedParam(int aaa, int NotMe);

// expected-warning@+6 {{parameter 'aaab' not found in the function declaration}} expected-note-NOT@+6 {{did you mean}}
// expected-warning@+6 {{parameter 'aabb' not found in the function declaration}} expected-note-NOT@+6 {{did you mean}}
// expected-warning@+6 {{parameter 'abbb' not found in the function declaration}} expected-note-NOT@+6 {{did you mean}}
// CHECK-NOT: fix-it:"{{.*}}":{[[@LINE+3]]:
// CHECK-NOT: fix-it:"{{.*}}":{[[@LINE+3]]:
// CHECK-NOT: fix-it:"{{.*}}":{[[@LINE+3]]:
/// \param aaab Almost there!
/// \param aabb Me too!
/// \param abbb Me three!
/// \param NotMe Match!
int onlyFixWhenExactlyOneBestParamComment_WithUnrelatedParam2(int aaa, int NotMe);

// Auto-fixing those two parameters would potentially worsen the situation as information is lost.
// At the moment they at least have different names and a human can resonably pick the correct one by name.
// expected-warning@+4 {{parameter 'ArgC' not found in the function declaration}} expected-note-NOT@+4 {{did you mean}}
// expected-warning@+4 {{parameter 'ArgV' not found in the function declaration}} expected-note-NOT@+4 {{did you mean}}
// CHECK-NOT: fix-it:"{{.*}}":{[[@LINE+2]]:
// CHECK-NOT: fix-it:"{{.*}}":{[[@LINE+2]]:
/// \param ArgC Almost matches.
/// \param ArgV Almost matches.
int test2to2_bothAlmost(int argc, int argv);

// Note: first parameter is the same as above, bug gets auto fixed here since
// it's obviously the better choice.
// expected-warning@+4 {{parameter 'ArgC' not found in the function declaration}} expected-note@+4 {{did you mean}}
// expected-warning@+4 {{parameter 'ZZZZZZ' not found in the function declaration}} expected-note-NOT@+4 {{did you mean}}
// CHECK: fix-it:"{{.*}}":{[[@LINE+2]]:
// CHECK-NOT: fix-it:"{{.*}}":{[[@LINE+2]]:
/// \param ArgC Almost matches.
/// \param ZZZZZZ Does not match at all.
int test2to2_oneAlmost(int argc, int argv);

// Note: first parameter is the same as above, bug gets auto fixed here since
// the other is already ok.
// expected-warning@+4 {{parameter 'ArgC' not found in the function declaration}} expected-note@+4 {{did you mean}}
// expected-warning-NOT@+4 {{parameter 'ZZZZZZ' not found in the function declaration}} expected-note-NOT@+4 {{did you mean}}
// CHECK: fix-it:"{{.*}}":{[[@LINE+2]]:
// CHECK-NOT: fix-it:"{{.*}}":{[[@LINE+2]]:
/// \param ArgC Almost matches.
/// \param argv Already matches.
int test2to2_oneAlmost2(int argc, int argv);

// expected-warning@+4 {{parameter 'YYYYYY' not found in the function declaration}} expected-note-NOT@+4 {{did you mean}}
// expected-warning@+4 {{parameter 'ZZZZZZ' not found in the function declaration}} expected-note-NOT@+4 {{did you mean}}
// CHECK-NOT: fix-it:"{{.*}}":{[[@LINE+2]]:
// CHECK-NOT: fix-it:"{{.*}}":{[[@LINE+2]]:
/// \param YYYYYY Does not match at all.
/// \param ZZZZZZ Does not match at all.
int test2to2_noMatch(int argc, int argv);

// expected-warning@+7 {{parameter 'aaab' not found in the function declaration}} expected-note@+7 {{did you mean 'aab'?}}
// expected-warning@+7 {{parameter 'aaac' not found in the function declaration}} expected-note@+7 {{did you mean 'aac'?}}
// expected-warning@+7 {{parameter 'aaad' not found in the function declaration}} expected-note@+7 {{did you mean 'aad'?}}
// CHECK: fix-it:"{{.*}}":{[[@LINE+4]]:
// CHECK: fix-it:"{{.*}}":{[[@LINE+4]]:
// CHECK: fix-it:"{{.*}}":{[[@LINE+4]]:
/**
 * \param aaab Should match aab
 * \param aaac Should match aac
 * \param aaad Should match aad
 */
void test3to3_allAlmost(int aab, int aac, int aad);

// expected-warning@+7 {{parameter 'aaab' not found in the function declaration}} expected-note@+7 {{did you mean 'aab'?}}
// expected-warning@+7 {{parameter 'aaac' not found in the function declaration}} expected-note@+7 {{did you mean 'aac'?}}
// expected-warning@+7 {{parameter 'aaad' not found in the function declaration}} expected-note@+7 {{did you mean 'aad'?}}
// CHECK: fix-it:"{{.*}}":{[[@LINE+4]]:
// CHECK: fix-it:"{{.*}}":{[[@LINE+4]]:
// CHECK: fix-it:"{{.*}}":{[[@LINE+4]]:
/**
 * \param aaab Should match aab
 * \param aaac Should match aac
 * \param aaad Should match aad
 */
void test3to3_allAlmost_rev(int aad, int aac, int aab);




// Test data and expectations from https://bugs.llvm.org/show_bug.cgi?id=43808
// expected-warning@+5 {{parameter 'arg[]' not found in the function declaration}} expected-note@+5 {{did you mean 'arg'?}}
// expected-warning@+5 {{parameter 'cnt' not found in the function declaration}} expected-note-NOT@+5 {{did you mean 'argc'?}}
// CHECK: fix-it:"{{.*}}":{[[@LINE+3]]:{{.*}}:"arg"
// CHECK-NOT: fix-it:"{{.*}}":{[[@LINE+3]]:
/**
  *  @param[in] arg[]       Docu_1
  *  @param[in] cnt         Docu_2
  */
void run(const char *const arg[], int argc);

// 2nd run after the above has been fixed. Now 1 parameter needs fixing and
// there is only one option, so it's matched regardless of how close they are.
// (see also fixSinceThereIsOnlyOneOption test).
// This could be handled in a single pass in the future.
// expected-warning@+4 {{parameter 'cnt' not found in the function declaration}} expected-note@+4 {{did you mean 'argc'?}}
// CHECK: fix-it:"{{.*}}":{[[@LINE+3]]:{{.*}}:"argc"
/**
  *  @param[in] arg         Docu_1
  *  @param[in] cnt         Docu_2
  */
void run(const char *const arg[], int argc);




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

