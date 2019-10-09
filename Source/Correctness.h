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
#include "Focal.hpp"




#define IGNORE_RETURN(call) call

typedef void (pARTFUN)(Object* pObject);

typedef void (pDUMPFUN)(eCN eClass, Object* pObject, Stz*);
extern pDUMPFUN *gpDump[];

typedef void (pFINALIZE(eCN eClass, Object* pObject));
extern pFINALIZE *gpFinalize[];

typedef void (pOUTLINEFUN)(Focal* pFocal, Stz*);
extern pOUTLINEFUN *gpOutline[];

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
