/*============================================================================*/
/* CompiledWith.h -- What are we compiling with?                              */
/*============================================================================*/
#pragma once

#define RAPIDSSE_VERSION_STRING "0.4.1"

#define WITH_ACTIVITY_MAP
#define WITH_ENTRY_EXIT
//#define WITH_LW_FOOTER
//#define WITH_MB_WATCH
//#define WITH_MUNGED
#define WITH_SETH
#define WITH_TRACE_API
#define WITH_VLQ_DATUMS

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// debug behaviours

#ifdef _DEBUG

#define WITH_ADDRESS_CHECKING    // check that address is in pool
#define WITH_ASSERT_MACRO        // Assert() macro
#define WITH_DEBUG               //
#define WITH_CORECTNESS_ALGEBRA  // known class, magic, footer
#define WITH_PREPOST_CHECKING    // _pre_(), _post_(), _checkInitialised_()

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// RELEASE behaviours

#else

#endif

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
