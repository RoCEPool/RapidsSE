/*============================================================================*/
/* CompiledWith.h -- What is the code being compiling with?                   */
/*============================================================================*/
#pragma once

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// platform

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

#define WITH_LOCK_TRACKING EndDefine         // code for tracking lock activity

#define WITH_OOM_FAULTINJECTION              // oom fault injectition

//#define WITH_STEP_TIMING EndDefine         // code for timing steps

//#define WITH_TRACE_API EndDefine           // trace api's
//#define WITH_TRACE_INDEX_COMPACTION EndDefine    // compaction progress
//#define WITH_TRACE_LOGICUNIT EndDefine       // trace logic unit


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// release behaviours

#else

#define WITH_RELEASE_VERSION EndDefine       // compile as release version

//#define WITH_ASSERT_MACRO EndDefine        // ASSERT() macro
//#define WITH_CHECK_INITIALISED EndDefine   // _checkInitialised_()
//#define WITH_CORRECTNESS_ALGEBRA EndDefine // magic, class, footer, pre, post

#define WITH_LOCK_TRACKING EndDefine       // code for tracking lock activity

//#define WITH_OOM_FAULTINJECTION              // oom fault injectition

//#define WITH_STEP_TIMING EndDefine         // code for timing steps

//#define WITH_TRACE_API EndDefine           // code for tracing api's compiled in
//#define WITH_TRACE_INDEX_COMPACTION EndDefine    // compaction progress
//#define WITH_TRACE_LOGICUNIT EndDefine     // trace logic unit

#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// row coder dev

//#define WITH_ANALYSE_FREE EndDefine        // code analyze free space during load
//#define WITH_VERIFY_TAILS EndDefine        // code verify tails

//#define WITH_TRACE_ENCODING EndDefine      // trace encoding compiled in
//#define WITH_TRACE_INFLATE EndDefine       // trace inflate compiled in

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// WIP

//#define WITH_DEDICATED_SLEEP EndDefine     // only dedicated threads can sleep

//#define WITH_SSN_SKIPLIST EndDefine        // using SsnId

//#define WITH_COMBINEDOP_SE EndDefine       // compile combinedOP generated code with SE

#define WITH_OPT_HASHCAST EndDefine       //

#define WITH_SUBPIPELINE EndDefine       //

#define WITH_STAGESET EndDefine       //

#define WITH_PAGING_DEBUG EndDefine
#define WITH_PAGING EndDefine

//#define WITH_PAGING EndDefine

#define WITH_API_FORK EndDefine       // RAY


#define WITH_LOCALS_OOM EndDefine     // RAY

#define WITH_TSCP_TIMER EndDefine    // Arjun: Use tscp as a timer instead of the GetTimeOfDay() for unload/reload timing

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
