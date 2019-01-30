/*============================================================================*/
/* CompiledWith.h -- What are we compiling with?                              */
/*============================================================================*/
#pragma once

#define RAPIDSSE_VERSION_STRING "0.4.1"

#define WITH_ACTIVITY_MAP
#define WITH_ENTRY_EXIT
//#define WITH_MUNGED

// toggle behaviors on or off
#ifdef _DEBUG
#define __ADDRESS_CHECK   // check that address is in pool
#define __ASSERT_CHECKS   // Assert() macro
#define __CLASS_CHECK     // known class
#define __DEBUG_CHECKS    // _pre_(), _post_(), _checkInitialised_()
#define __FOOTER_CHECK    // footer marker check
#define __MEMORY_ZERO     // initialise wsp etc to all 0's
#define __MAGIC_CHECK     // magic marker check in header
#else
#endif


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
