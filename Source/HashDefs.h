/*============================================================================*/
/* hashdefs.h - various #definitions                                          */
/*============================================================================*/

#pragma once

#define VERSION_STRING  "0.0.1"

#define TRACE_FILE "/tmp/se_trace.txt"

#define REPORT_INFO
#define REPORT_TRACE
#define REPORT_WARNING
#define REPORT_ERROR
#define REPORT_FATAL

#define DEBUG_CHECKS
#define ADDRESS_CHECK
#define CLASS_CHECK
#define MAGIC_CHECK

#define DIV(a,b) (b ? (a / b) : 0)    // checked divide
#define MAX(a,b) (((a)>(b))?(a):(b))  // which is smaller
#define MIN(a,b) (((a)<(b))?(a):(b))  // which is bigger
#define MOD(a,b) (b ? (a % b) : 0)    // checked modulo
#define NIL(a,b) (a ? a : b)          // null replacement

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/