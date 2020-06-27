/*============================================================================*/
/* CompiledWith.h -- What is the code being compiling with?                   */
/*============================================================================*/
#pragma once

static char* RAPIDSSE_VERSION_STRING = (char*)"3.0.4 alpha";
static long  RAPIDSSE_VERSION_NUMBER = 0x030104;

static char* RAPIDSSE_FORMAT_STRING  = (char*)"3.0.4";
static long  RAPIDSSE_FORMAT_NUMBER  = 0x030104;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// platform

#ifdef __APPLE__
#define WITH_APPLE EndDefine
#define WITH_MUNGED_SEN EndDefine // use munged sen
#endif

#ifdef __LINUX__
#define WITH_LINUX EndDefine
#define WITH_MUNGED_SEN EndDefine // use munged sen
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// debug behaviours

//#ifdef _DEBUG
#if defined (_DEBUG) // || defined (WITH_DEBUG_VERSION)

#define WITH_DEBUG_VERSION EndDefine         // compile as debug version

#define WITH_ACTION_SEQUENCE EndDefine       // code for ActionLog compiled in
#define WITH_ADDRESS_CHECKING EndDefine      // check that address is in pool
#define WITH_ASSERT_MACRO EndDefine          // Assert() macro
#define WITH_CHECK_INITIALISED EndDefine     // _checkInitialised_()
#define WITH_CORRECTNESS_ALGEBRA EndDefine   // known class, magic, footer
//#define WITH_EB_LAZY_CHAIN EndDefine         // build lazy chain on insert
#define WITH_ENTRY_EXIT EndDefine            // code for ENTRY() & EXIT() macros
#define WITH_IT_COMPACT_TRACE EndDefine      //
#define WITH_LOCK_TRACKING EndDefine         // track lock activity
#define WITH_PREPOST_CHECKING EndDefine      // _pre/postCondition_()
#define WITH_SETH_SUPPORT EndDefine          // seth support
#define WITH_SHARED_LOG EndDefine            // code for Log compiled in
//#define WITH_STATISTICS_COUNTERS  EndDefine // statistics
#define WITH_TRACE_API EndDefine             // code for tracing api's compiled in
//#define WITH_TRACE_DYNAMO EndDefine        // tracing compiling dynamic pipeline
//#define WITH_PYTHON_SUPPORT EndDefine      // code for python support
#define WITH_TRACE_PURGING EndDefine         // trace purging
//#define WITH_TRACE_PROCEDURE EndDefine     // trace procedure

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// release behaviours

#else

#define WITH_RELEASE_VERSION EndDefine     // compile as release version

// usually correctness, entry/exit, prepost

//#define WITH_ASSERT_MACRO EndDefine        // Assert() macro
//#define WITH_ACTION_SEQUENCE EndDefine     // code for ActionLog compiled in
//#define WITH_CHECK_INITIALISED EndDefine   // _checkInitialised_()
//#define WITH_CORRECTNESS_ALGEBRA EndDefine // known class, magic, footer
//#define WITH_EB_LAZY_CHAIN EndDefine       // build lazy chain on insert
//#define WITH_ENTRY_EXIT EndDefine          // code for ENTRY() & EXIT() macros
//#define WITH_IT_COMPACT_TRACE EndDefine    // compact trace
//#define WITH_LOCK_TRACKING EndDefine       // track lock activity
//#define WITH_PREPOST_CHECKING EndDefine    // _pre/postCondition_()
//#define WITH_SETH_SUPPORT EndDefine        // seth support
//#define WITH_STATISTICS_COUNTERS EndDefine // statistics
//#define WITH_TRACE_API EndDefine           // code for tracing api's compiled in
//#define WITH_TRACE_DYNAMO EndDefine        // tracing compiling dynamic pipeline
//#define WITH_PYTHON_SUPPORT EndDefine      // code for python support
//#define WITH_TRACE_PURGING EndDefine       // trace purging
//#define WITH_TRACE_PROCEDURE EndDefine     // trace procedure

#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// buckets

//#define WITH_BUCKET_HASHING EndDefine      // code for bucket hashing compiled in


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// row coder dev

//#define WITH_ANALYSE_FREE EndDefine        // analyze free space during load
//#define WITH_ENCODING_TRACE EndDefine      // code for tracing encoding compiled in
//#define WITH_INFLATE_TRACE EndDefine       // code for tracing inflate compiled in
//#define WITH_SHOCO_DUPLICATES EndDefine    // test for duplicate shoco values
//#define WITH_VERIFY_TAILS EndDefine        // verify tails

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// event-based task model

//#define WITH_PULSING EndDefine             // code for pulsing compiled in
//#define WITH_NO_XMIT_OPT EndDefine         // code for no xmit optimization

// temporary

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
