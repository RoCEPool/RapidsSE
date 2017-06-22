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
  #define ADDRESS_CHECK
  #define CLASS_CHECK
  #define DEBUG_CHECKS
  #define MAGIC_CHECK
  #define FOOTER_CHECK
  #define ASSERT_CHECKS
  #define ENTRY_EXIT
#endif

#define VERBOSE_TRACE
#define HIDDEN_INDEX

//#define STEP_BOARD

#define RPN_PLANNER

#define DIV(a,b) (b ? (a / b) : 0)    // checked divide
#define MAX(a,b) (((a)>(b))?(a):(b))  // which is smaller
#define MIN(a,b) (((a)<(b))?(a):(b))  // which is bigger
#define MOD(a,b) (b ? (a % b) : 0)    // checked modulo
#define NIL(a,b) (a ? a : b)          // null replacement

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
