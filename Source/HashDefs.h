/*============================================================================*/
/* HashDefs.h -- Various #definitions                                         */
/*============================================================================*/

#pragma once

#define REPORT_INFO
#define REPORT_TRACE
#define REPORT_WARNING
#define REPORT_ERROR
#define REPORT_FATAL

#ifdef _DEBUG
  #define ADDRESS_CHECK   // address is in pool
  #define ASSERT_CHECKS   // assert macro
  #define CLASS_CHECK     // known class
  #define DEBUG_CHECKS    // _pre_(), _post_(), _checkInitialised_()
  #define ENTRY_EXIT      // entry exit macros
  #define FOOTER_CHECK    // footer marker check
  #define MAGIC_CHECK     // magic marker check in header
#endif

#define VERBOSE_TRACE
#define HIDDEN_INDEX

//#define STEP_BOARD

#define DIV(a,b) (b ? (a / b) : 0)    // checked divide
#define MAX(a,b) (((a)>(b))?(a):(b))  // which is smaller
#define MIN(a,b) (((a)<(b))?(a):(b))  // which is bigger
#define MOD(a,b) (b ? (a % b) : 0)    // checked modulo
#define NIL(a,b) (a ? a : b)          // null replacement

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
