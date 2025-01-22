#pragma once

//      credits: Sergey Fursa       //
//        also known as qo0         //
//                       ____       //
//       ______  ____   / __ \      //
//      / __  / / __ \ / / / /      //
//     / /_/ / / /_/ // /_/ /       //
//     \__  /  \____/ \____/        //
//       /_/                        //
//                                  //
//    internal cs:go cheat base     //

// used: user defined behaviour
#include "user.h"

#pragma region common_compiler_specific
#ifdef Q_COMPILER_MSC
// treat "discarding return value of function with 'nodiscard' attribute" warning as error
#pragma warning(error : 4834)
#endif

#ifdef Q_COMPILER_CLANG
#pragma clang diagnostic ignored "-Wunused-private-field"
#endif

#if defined(Q_COMPILER_MSC)
#define Q_NAKED __declspec(naked)
#elif defined(Q_COMPILER_CLANG)
#define Q_NAKED [[gnu::naked]]
#endif

// @todo: platform dependent / but currently we shouldn't give fuck on it
#define Q_CDECL __cdecl
#define Q_STDCALL __stdcall
#define Q_FASTCALL __fastcall
#define Q_THISCALL __thiscall
#define Q_VECTORCALL __vectorcall
#pragma endregion

#pragma region common_implementation_specific
#define _Q_INTERNAL_CONCATENATE(LEFT, RIGHT) LEFT##RIGHT
#define _Q_INTERNAL_STRINGIFY(NAME) #NAME
#define _Q_INTERNAL_UNPARENTHESIZE(...) __VA_ARGS__

// convert plain text to string
#define Q_STRINGIFY(NAME) _Q_INTERNAL_STRINGIFY(NAME)
// concatenate plain text
#define Q_CONCATENATE(LEFT, RIGHT) _Q_INTERNAL_CONCATENATE(LEFT, RIGHT)
// unparenthesize variadic arguments
#define Q_UNPARENTHESIZE(...) _Q_INTERNAL_UNPARENTHESIZE(__VA_ARGS__)

// calculate elements count of fixed-size C array
#define Q_ARRAYSIZE(ARRAY) (sizeof(ARRAY) / sizeof(ARRAY[0]))

// calculate the offset of a struct member variable, in bytes
#define Q_OFFSETOF(STRUCT, MEMBER) __builtin_offsetof(STRUCT, MEMBER)

#ifndef Q_NO_RTTI
#if defined(Q_COMPILER_MSC) && !defined(_CPPRTTI)
#define Q_NO_RTTI
#elif defined(Q_COMPILER_CLANG)
#if !__has_feature(cxx_rtti)
#define Q_NO_RTTI
#endif
#endif
#endif

#ifndef Q_INLINE
#if defined(Q_COMPILER_MSC)
#define Q_INLINE __forceinline
#else
// referenced to clang documentation, this is enough: https://clang.llvm.org/compatibility.html
#define Q_INLINE inline
#endif
#endif

#ifndef Q_RETURN_ADDRESS
#if defined(Q_COMPILER_MSC)
#define Q_RETURN_ADDRESS() _ReturnAddress()
#elif defined(Q_COMPILER_CLANG)
#define Q_RETURN_ADDRESS() __builtin_return_address(0)
#else
#error "it is expected you to define Q_RETURN_ADDRESS() into something that will get the return address off the stack!"
#define Q_RETURN_ADDRESS()
#endif
#endif

#ifndef Q_FRAME_ADDRESS
#if defined(Q_COMPILER_MSC)
#define Q_FRAME_ADDRESS() _AddressOfReturnAddress()
#elif defined(Q_COMPILER_CLANG)
// @note: it isn't always what we're expecting, compiler dependent
#define Q_FRAME_ADDRESS() __builtin_frame_address(0)
#else
#error "it is expected you to define Q_FRAME_ADDRESS() into something that will get the address of the function's stack frame!"
#define Q_FRAME_ADDRESS()
#endif
#endif

#ifndef Q_DEBUG_BREAK
#if defined(Q_COMPILER_MSC)
#define Q_DEBUG_BREAK() __debugbreak()
#elif defined(Q_COMPILER_CLANG)
#define Q_DEBUG_BREAK() __builtin_debugtrap()
#else
// @todo: use #warning instead of static asserts when c++23 comes out
static_assert(false, "it is expected you to define Q_DEBUG_BREAK() into something that will break in a debugger!");
#define Q_DEBUG_BREAK()
#endif
#endif

#ifndef Q_ASSERT
#ifdef _DEBUG
#define Q_ASSERT(EXPRESSION) static_cast<void>(!!(EXPRESSION) || (Q_DEBUG_BREAK(), 0))
#else
// disable assertion for release builds
#define Q_ASSERT(EXPRESSION) static_cast<void>(0)
#endif
#endif
#pragma endregion

/*
 * explicitly delete the following constructors, to prevent attempts on using them:
 * constructor, move-constructor, copy-constructor
 */
#define Q_CLASS_NO_CONSTRUCTOR(CLASS) \
	CLASS() = delete;                 \
	CLASS(CLASS&&) = delete;          \
	CLASS(const CLASS&) = delete;

/*
 * explicitly delete the following assignment operators, to prevent attempts on using them:
 * move-assignment, copy-assignment
 */
#define Q_CLASS_NO_ASSIGNMENT(CLASS)    \
	CLASS& operator=(CLASS&&) = delete; \
	CLASS& operator=(const CLASS&) = delete;

// explicitly delete any class initializer to prevent attempts on using them
#define Q_CLASS_NO_INITIALIZER(CLASS) \
	Q_CLASS_NO_CONSTRUCTOR(CLASS)     \
	Q_CLASS_NO_ASSIGNMENT(CLASS)

// explicitly delete class heap allocator and deallocator, to prevent attempts on using class at heap memory
#define Q_CLASS_NO_ALLOC()                           \
	void* operator new(const size_t nSize) = delete; \
	void operator delete(void* pMemory) = delete;
