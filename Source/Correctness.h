/*============================================================================*/
/* Correctness.h -- Correctness Algebra namespace                             */
/*============================================================================*/
#pragma once

#include "Strings.h"

//------------------------------------------------------------------------------

namespace Correctness
{
  void vCheckAssumptions(void);
  void vTraceWith(void);
}

//------------------------------------------------------------------------------


#include "LimitsFormats.h"
#include "ClassNumbers.h"
#include "Footer.hpp"
#include "Header.hpp"
#include "FreeChunk.hpp"
#include "Debug.hpp"

#include "MemLib.hpp"

#include "AssertMacros.hpp"

#include "Object.hpp"
#include "SeeMe.hpp"

#include "Addressing.hpp"
#include "MP.hpp"
#include "HeapVar.hpp"
#include "SizedHeader.hpp"
#include "Report.hpp"
#include "IntLib.hpp"

#include "String_t.hpp"

typedef String_t<32>     s32;
typedef String_t<256>    s256;
typedef String_t<C_4K>   s4K;   // note this is 4K not 4KB
typedef String_t<C_16KB> s16KB; // note this is 16KB
typedef String_t<C_17KB> s17KB; // note this is 17KB
typedef String_t<C_48K>  s48K;  // note this is 48K not 48KB
typedef String_t<C_1M>   s1M;   // note this is 1M not 1MB
typedef String_t<C_MAX_VCH> sVCH;


#include "MagLib.hpp"
#include "Statistics.hpp"
#include "EnumFlags.hpp"

#include "SysCall.hpp"


#define IGNORE_RETURN(call) call

typedef void (pDUMPFUN)(eCN eClass, Object* pObject, Stz*);
extern pDUMPFUN *gpDump[];

typedef void (pFINALIZE(eCN eClass, Object* pObject));
extern pFINALIZE *gpFinalize[];

namespace Tasks { typedef MP<Task,CN_TASK> MP_Task;
                  typedef void (*StepFp)(Tasks::MP_Task); }

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
