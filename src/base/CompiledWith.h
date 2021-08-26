/*============================================================================*/
/* CompiledWith.h -- What is the code being compiling with?                   */
/*============================================================================*/
#pragma once

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// platform

#define __TINYC

#ifdef __APPLE__
#define WITH_APPLE EndDefine
#define WITH_MUNGED_SEN EndDefine // use munged sen APPLE

#endif

#ifdef __LINUX__
#define WITH_LINUX EndDefine
#define WITH_MUNGED_SEN EndDefine // use munged sen LINUX
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// debug behaviours

#if defined (WITH_DEBUG_VERSION)

#define WITH_ASSERT_MACRO EndDefine          // ASSERT() macro
#define WITH_CHECK_INITIALISED EndDefine     // _checkInitialised_()
#define WITH_CORRECTNESS_ALGEBRA EndDefine   // magic, class, footer, pre, post

//#define WITH_IT_COMPACT_TRACE EndDefine    // compaction progress
//#define WITH_LOCK_TRACKING EndDefine       // code for tracking lock activity
//#define WITH_PYTHON_SUPPORT EndDefine      // code for python support
//#define WITH_STATISTICS_COUNTERS EndDefine // code for statistics
//#define WITH_STEP_TIMING EndDefine         // code for timing steps
//#define WITH_TRACE_API EndDefine           // trace api's
//#define WITH_TRACE_LOGICUNIT EndDefine       // trace logic unit
//#define WITH_TRACE_PROCEDURE EndDefine     // trace procedure
//#define WITH_TRACE_PURGING EndDefine       // trace purging


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// release behaviours

#else

#define WITH_RELEASE_VERSION EndDefine       // compile as release version

//#define WITH_ASSERT_MACRO EndDefine        // ASSERT() macro
//#define WITH_CHECK_INITIALISED EndDefine   // _checkInitialised_()
//#define WITH_CORRECTNESS_ALGEBRA EndDefine // magic, class, footer, pre, post

//#define WITH_IT_COMPACT_TRACE EndDefine    // compaction progress
//#define WITH_LOCK_TRACKING EndDefine       // code for tracking lock activity
//#define WITH_PYTHON_SUPPORT EndDefine      // code for python support
//#define WITH_STATISTICS_COUNTERS EndDefine // code for statistics
//#define WITH_STEP_TIMING EndDefine         // code for timing steps
//#define WITH_TRACE_API EndDefine           // code for tracing api's compiled in
//#define WITH_TRACE_LOGICUNIT EndDefine     // trace logic unit
//#define WITH_TRACE_PROCEDURE EndDefine     // trace procedure
//#define WITH_TRACE_PURGING EndDefine       // trace purging

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

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// WIP

#define WITH_NESTED_PIPELINES EndDefine    // code for nested pipelines

#define WITH_KEEP_ROW_HASH EndDefine       // keep row hash in row of rowBuffer
                                           //   instead of partition no

//#define WITH_BKTMAP_2N4P EndDefine         // code for buckets up to 2N4P

//#define WITH_DEDICATED_SLEEP EndDefine     // only dedicated threads can sleep

//#define WITH_JOB_REAPING EndDefine         // reaper job lifecycle

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
