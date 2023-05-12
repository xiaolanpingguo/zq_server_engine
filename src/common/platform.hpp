#pragma once


#ifdef _MSC_VER
#define COMPILER_MSC
#elif defined( __INTEL_COMPILER )
#define COMPILER_INTEL
#elif defined( __BORLANDC__ )
#define COMPILER_BORLAND
#elif defined( __GNUC__ )
#define COMPILER_GNU
#elif defined( __clang__ )
#define COMPILER_CLANG
#else
#pragma error "FATAL ERROR: Unknown compiler."
#endif


#if defined( __WIN32__ ) || defined( WIN32 ) || defined( _WIN32 )
#ifndef PLATFORM_WIN
#define PLATFORM_WIN
#endif
#elif defined( __INTEL_COMPILER )
#ifndef PLATFORM_INTEL
#define PLATFORM_INTEL
#endif
#elif defined( __APPLE_CC__ )
#ifndef LATFORM_APPLE
#define LATFORM_APPLE
#endif
#else
#ifndef PLATFORM_LINUX
#define PLATFORM_LINUX
#endif
#endif


#ifndef X64
#if defined( _WIN64 ) || defined( __x86_64__ ) || defined( __amd64 ) || defined( __LP64__ )
#define X64_BITS
#endif
#endif



#if defined(COMPILER_MSC)
	#define DLL_EXPORT __declspec(dllexport)
	#define DLL_IMPORT __declspec(dllimport)
#elif defined(COMPILER_GNU)
	#define DLL_EXPORT __attribute__((visibility("default")))
	#define DLL_IMPORT
#else
	#error compiler not supported!
#endif

