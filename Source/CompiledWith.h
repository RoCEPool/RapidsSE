/*============================================================================*/
/* CompiledWith.h -- What is the code being compiling with?                   */
/*============================================================================*/
#pragma once

#define RAPIDSSE_VERSION_STRING "4.0.5"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// platform

#ifdef __APPLE__
#define WITH_APPLE
#endif

#ifdef __LINUX__
#define WITH_LINUX
#define WITH_MUNGED_SEN  // use munged sen
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// debug behaviours

#ifdef _DEBUG

#define WITH_DEBUG_VERSION      // compile as debug version

#define WITH_ACTION_SEQUENCE    // code for ActionSequence compiled in
#define WITH_ACTIVITY_MAP       // code for ActivityMap compiled in
#define WITH_ADDRESS_CHECKING   // check that address is in pool
#define WITH_ASSERT_MACRO       // Assert() macro
#define WITH_CHECK_INITIALISED  // _checkInitialised_()
#define WITH_CORECTNESS_ALGEBRA // known class, magic, footer
#define WITH_ENTRY_EXIT         // code for ENTRY() & EXIT() macros compiled in
#define WITH_PREPOST_CHECKING   // _pre/postCondition_()
#define WITH_TRACE_API          // code for tracing api's compiled in
//#define WITH_TRACE_ENCODING     // code for tracing encoding compiled in
//#define WITH_TRACE_DECODING     // code for tracing decoding compiled in

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// release behaviours

#else

#define WITH_RELEASE_VERSION    // compile as release version

//#define WITH_ACTION_SEQUENCE    // code for ActionSequence compiled in
//#define WITH_ACTIVITY_MAP       // code for ActivityMap compiled in
//#define WITH_CHECK_INITIALISED  // _checkInitialised_()
//#define WITH_CORECTNESS_ALGEBRA // known class, magic, footer
//#define WITH_ENTRY_EXIT         // code for ENTRY() & EXIT() macros compiled in
//#define WITH_PREPOST_CHECKING   // _pre/postCondition_()
//#define WITH_TRACE_API          // code for tracing api's compiled in
//#define WITH_TRACE_ENCODING     // code for tracing encoding compiled in
//#define WITH_TRACE_DECODING     // code for tracing decoding compiled in
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// testing

#define WITH_ENCODING_BCD_DIGIT // encode bcd as digit if possible
#define WITH_ENCODING_INT_DIGIT // encode int as digit if possible
#define WITH_ENCODING_VCH_SMALL // encode vch small
#define WITH_SHOCO_DUPLICATES   // test for duplicate shoco tail lines
#define WITH_SHOCO_FOLDLAST     // test for folding last shoco line
//#define WITH_ENCODING_XCHECK    // encode cross check
//#define WITH_TRACE_DECODING     // code for tracing decoding compiled in
//#define WITH_LARGE_ROWS         // encode cross check
//#define WITH_ANALYSE_FREE       // analyze free space during load
//#define WITH_VERIFY_TAILS       // verify tails
//#define WITH_QUICKER_INFLATE    // quicker inflate test

#define WITH_HEAD_OVERFLOW      // encode cross check

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
