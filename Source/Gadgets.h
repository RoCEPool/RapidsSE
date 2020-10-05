/*============================================================================*/
/* Gadgets.h -- Gadgets namespace                                             */
/*============================================================================*/
#pragma once

#include "Pool.h"

//------------------------------------------------------------------------------

namespace Gadgets
{
  static const i8 C_TPCH_CUSTOMER =  150 * C_1K;
  static const i8 C_TPCH_LINEITEM = 6000 * C_1K;
  static const i8 C_TPCH_ORDERS   = 1500 * C_1K;
  static const i8 C_TPCH_PART     =  200 * C_1K;
  static const i8 C_TPCH_PARTSUPP =  800 * C_1K;
  static const i8 C_TPCH_SUPPLIER =   10 * C_1K;

  static const i8 C_TPCH_NATION   = 25;
  static const i8 C_TPCH_REGION   = 5;


  void vCheckAssumptions(void);
}

//------------------------------------------------------------------------------

// tags
static Stz* C_TAGSET_NAME   = "<aTagSetNm>"_Stz;

static Stz* C_TAG_BYPROMPT  = "<byPrompt>"_Stz;
static Stz* C_TAG_CMD       = "<cmd>"_Stz;
static Stz* C_TAG_CMDFILE   = "<cmdFile>"_Stz;
static Stz* C_TAG_CMDLINE   = "<cmdLine>"_Stz;
static Stz* C_TAG_DIVISION  = "<division>"_Stz;
static Stz* C_TAG_FLOW      = "<flow>"_Stz;
static Stz* C_TAG_FORCELOG  = "<forceLog>"_Stz;
static Stz* C_TAG_FREEPCT   = "<freePct>"_Stz;
static Stz* C_TAG_DISTID    = "<distId>"_Stz;
static Stz* C_TAG_GRID      = "<grid>"_Stz;
static Stz* C_TAG_HOST      = "<host>"_Stz;
static Stz* C_TAG_JOBKIND   = "<jobKind>"_Stz;
static Stz* C_TAG_JOBNM     = "<jobNm>"_Stz;
static Stz* C_TAG_LIFE      = "<life>"_Stz;
static Stz* C_TAG_NEXTOP    = "<nextOp>"_Stz;
static Stz* C_TAG_NEXTSTAGE = "<nextStage>"_Stz;
static Stz* C_TAG_NODE      = "<node>"_Stz;
static Stz* C_TAG_NOTE      = "<note>"_Stz;
static Stz* C_TAG_ORIGIN    = "<origin>"_Stz;
static Stz* C_TAG_PATH      = "<path>"_Stz;
static Stz* C_TAG_REQUEST   = "<request>"_Stz;
static Stz* C_TAG_NOWAIT    = "<noWait>"_Stz;
static Stz* C_TAG_SECTION   = "<section>"_Stz;
static Stz* C_TAG_SESSION   = "<session>"_Stz;
static Stz* C_TAG_SHM       = "<shm>"_Stz;
static Stz* C_TAG_STMT      = "<stmt>"_Stz;
static Stz* C_TAG_SUMMARY   = "<summary>"_Stz;
static Stz* C_TAG_TABLENM   = "<tableNm>"_Stz;
static Stz* C_TAG_TAGS      = "<tags>"_Stz;
static Stz* C_TAG_TIME      = "<time>"_Stz;
static Stz* C_TAG_TOPOLOGY  = "<topology>"_Stz;
static Stz* C_TAG_WHEN      = "<when>"_Stz;
static Stz* C_TAG_XFER      = "<xfer>"_Stz;
static Stz* C_TAG_XMITKIND  = "<xmitKind>"_Stz;
static Stz* C_TAG_XMITS     = "<xmits>"_Stz;

static Stz* C_TAG_DBG_ACKS  = "<dbg_acks>"_Stz;

static Stz* C_TAG_LD_DATE_FORMAT          = "<date_format>"_Stz;
static Stz* C_TAG_LD_ENCLOSED_BY_LEADING  = "<enclosed_by_leading>"_Stz;
static Stz* C_TAG_LD_ENCLOSED_BY_TRAILING = "<enclosed_by_trailing>"_Stz;
static Stz* C_TAG_LD_ESCAPE_CHAR          = "<escape_char>"_Stz;
static Stz* C_TAG_LD_FIELD_DELIMITER      = "<field_delimiter>"_Stz;
static Stz* C_TAG_LD_MAX_ERRORS           = "<max_errors>"_Stz;
static Stz* C_TAG_LD_RECORD_TERMINATOR    = "<record_terminator>"_Stz;
static Stz* C_TAG_LD_RECORD_ENDS          = "<record_ends>"_Stz;

// job info tags
static Stz* C_TAG_SEEDCAN  = "<seedCan>"_Stz;
static Stz* C_TAG_SEEDEND  = "<seedEnd>"_Stz;
static Stz* C_TAG_SEEDEXE  = "<seedExe>"_Stz;
static Stz* C_TAG_SEEDRCV  = "<seedRcv>"_Stz;
static Stz* C_TAG_SEEDREQ  = "<seedReq>"_Stz;
static Stz* C_TAG_SEEDRUN  = "<seedRun>"_Stz;

//------------------------------------------------------------------------------

#include "Birth.hpp"
#include "CRM.hpp"
#include "DecaCnt.hpp"
#include "Decider.hpp"
#include "EnumFlags.hpp"
#include "FreqDist.hpp"
#include "HshLib.hpp"
#include "HV.hpp"
#include "MemTest.hpp"
#include "NextGid.hpp"
#include "Now.hpp"
#include "PtnLib.hpp"
#include "Rand.hpp"
#include "SeeMe.hpp"
#include "Slab.hpp"
#include "SlabAllocator.hpp"
#include "Sleep.hpp"
#include "Scan.hpp"
#include "Sprint.hpp"
#include "TagSet.hpp"
#include "TempName.hpp"
#include "TickingDevice.hpp"
#include "Timer.hpp"
#include "AggTimer.hpp"

#include "ByteRate.hpp"
#include "Peruse.hpp"
#include "Numbat.hpp"

#include "SP_AggTimer.hpp"
#include "SP_Bytes.hpp"
#include "SP_Char.hpp"
#include "SP_Elapsed.hpp"
#include "SP_Footer.hpp"
#include "SP_Header.hpp"
#include "SP_ReqTuple.hpp"
#include "SP_String.hpp"
#include "SP_TagSet.hpp"
#include "SP_Timer.hpp"

typedef void (*FNADDR)(void*);

#define INTRINSIC(FNNAME) \
  \
  FnDesc FnDesc_ ## FNNAME (#FNNAME ## _Stz, (FNADDR)Step::FNNAME); \
  \
  void Step::FNNAME(Forth* pForth) \
EndDefine

#define JOBS_fn(FNNAME) \
  \
  FnDesc FnDesc_ ## FNNAME (#FNNAME ## _Stz, (FNADDR)Jobs::FNNAME); \
  \
  void Jobs::FNNAME(Job* pJob) \
EndDefine

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
