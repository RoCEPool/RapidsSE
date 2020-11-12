/*============================================================================*/
/* Correctness.h -- Correctness Algebra namespace                             */
/*============================================================================*/
#pragma once

#include "ClassData.h"

//------------------------------------------------------------------------------

namespace Correctness
{
  void vCheckAssumptions(void);

  bool bTraceWith(Stz* szWith);
  void vTraceWith(void);

  bool bTraceWithout(Stz* szWith);
  void vTraceWithout(void);
}

//------------------------------------------------------------------------------


#include "eFK.h"
#include "Exception.hpp"


#include "Footer.hpp"
#include "Header.hpp"

#include "Correct.hpp"

#include "Fault.hpp"

#include "MemLib.hpp"

#include "AssertMacros.h"

#include "Object.hpp"
#include "StackObject.hpp"
#include "NonProper.hpp"

#include "Addressing.hpp"
#include "SizedHeader.hpp"
#include "Trace.hpp"
#include "Jot.hpp"
#include "IntLib.hpp"

#include "Statistics.hpp"
#include "Focal.hpp"

#include "String.hpp"

#include "PTR.hpp"
#include "SMA.hpp"
#include "REF_t.hpp"
#include "REG_t.hpp"


namespace Tasks {
  typedef void (Step::*StepFp)(void);
}


#include "Cell.hpp"


//typedef void (pDUMPFUN)(eCN eClass, Object* pObject, Stz*);
//extern pDUMPFUN *gpDump[];

typedef void (pOUTLINEFUN)(Focal* pFocal, Stz*);
extern pOUTLINEFUN *gpOutline[];

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
