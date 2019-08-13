/*============================================================================*/
/* Correctness.h -- Correctness Algebra namespace                             */
/*============================================================================*/
#pragma once

#include "ClassData.h"

//------------------------------------------------------------------------------

namespace Correctness
{
  void vCheckAssumptions(void);
  void vTraceWith(void);
}

//------------------------------------------------------------------------------


#include "LimitsFormats.h"
#include "Footer.hpp"
#include "Header.hpp"
#include "Debug.hpp"

#include "MemLib.hpp"

#include "AssertMacros.hpp"

#include "Object.hpp"

#include "Addressing.hpp"
#include "SizedHeader.hpp"
#include "Report.hpp"
#include "IntLib.hpp"


#include "Statistics.hpp"



#define IGNORE_RETURN(call) call

typedef void (pDUMPFUN)(eCN eClass, Object* pObject, Stz*);
extern pDUMPFUN *gpDump[];

typedef void (pFINALIZE(eCN eClass, Object* pObject));
extern pFINALIZE *gpFinalize[];


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
