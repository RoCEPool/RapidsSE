/*============================================================================*/
/* CompiledWith.h -- What are we compiling with?                              */
/*============================================================================*/
#pragma once

#define RAPIDSSE_VERSION_STRING "0.4.1"

#define WITH_ACTIVITY_MAP
#define WITH_ENTRY_EXIT
//#define WITH_LW_FOOTER
#define WITH_JOB_TRACE false
//#define WITH_MB_WATCH
//#define WITH_MUNGED
#define WITH_SETH


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// debug behaviours

#ifdef _DEBUG

#define WITH_ADDRESS_CHECK   // check that address is in pool
#define WITH_ASSERT_CHECKS   // Assert() macro
#define WITH_CLASS_CHECK     // known class
#define WITH_DEBUG_CHECKS    // _pre_(), _post_(), _checkInitialised_()
#define WITH_FOOTER_CHECK    // footer marker check
#define WITH_MAGIC_CHECK     // magic marker check in header
#define WITH_MEMORY_ZERO     // initialise wsp etc to all 0's

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// RELEASE behaviours

#else

#endif

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
