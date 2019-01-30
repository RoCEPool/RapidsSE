/*============================================================================*/
/* CompiledWith.h -- What is the code being compiling with?                   */
/*============================================================================*/
#pragma once

#define RAPIDSSE_VERSION_STRING "0.4.1"

#define WITH_ACTIVITY_MAP        // code for ActivityMap compiled in
#define WITH_ENTRY_EXIT          // code for ENTRY() & EXIT() macros compiled in
//#define WITH_MUNGED_SEN          // use munged sen
#define WITH_TRACE_API           // code for tracing api's compiled in

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// debug behaviours

#ifdef _DEBUG

#define WITH_ADDRESS_CHECKING   // check that address is in pool
#define WITH_ASSERT_MACRO       // Assert() macro
#define WITH_DEBUG_VERSION      // compile as debug version
#define WITH_CORECTNESS_ALGEBRA // known class, magic, footer
#define WITH_PREPOST_CHECKING   // _pre/postCondition_(), _checkInitialised_()

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// RELEASE behaviours

#else
#define WITH_RELEASE_VERSION    // compile as release version
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// platform

#ifdef __APPLE__
#define WITH_APPLE
#endif

#ifdef __LINUX__
#define WITH_LINUX
#endif

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
