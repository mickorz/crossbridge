// RUN: %clang_cc1 -triple i386-pc-linux-gnu -emit-llvm -o - %s | FileCheck %s

// CHECK: @foo

// Make sure we mangle overloadable, even in C system headers.
# 1 "somesystemheader.h" 1 3 4
// CHECK: @_Z2f0i
void __attribute__((__overloadable__)) f0(int a) {}
// CHECK: @_Z2f0l
void __attribute__((__overloadable__)) f0(long b) {}

// CHECK: @bar

// These should get merged.
void foo() __asm__("bar");
void foo2() __asm__("bar");

int nux __asm__("foo");
extern float nux2 __asm__("foo");

int test() { 
  foo();
  foo2();
  
  return nux + nux2;
}


// Function becomes a variable.
void foo3() __asm__("var");

void test2() {
  foo3();
}
int foo4 __asm__("var") = 4;


// Variable becomes a function
extern int foo5 __asm__("var2");

void test3() {
  foo5 = 1;
}

void foo6() __asm__("var2");
void foo6() {
}



int foo7 __asm__("foo7") __attribute__((used));
float foo8 __asm__("foo7") = 42;

// PR4412
int func(void);
extern int func (void) __asm__ ("FUNC");

// CHECK: @FUNC
int func(void) {
  return 42;
}

// CHECK: @_Z4foo9Dv4_f
typedef __attribute__(( vector_size(16) )) float float4;
void __attribute__((__overloadable__)) foo9(float4 f) {}
