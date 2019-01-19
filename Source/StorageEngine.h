/*============================================================================*/
/* StorageEngine.h -- StorageEngine namespace                                 */
/*============================================================================*/

#pragma once

#include "Axioms.h"

//-----------------------------------------------------------------------------

namespace StorageEngine
{
  void vCheckAllAssumptions(void);
  void vCheckAssumptions(void);
  void vSetSignalHandlers(void);
}

using namespace StorageEngine;

//-----------------------------------------------------------------------------

#include "Arguments.hpp"
#include "CarryOver.hpp"
#include "AttachMap.hpp"
#include "Region.hpp"
#include "SizesDumps.hpp"
#include "Finalizers.hpp"

#include "RapidsSE.hpp"

typedef void (pDUMPFUN)(eCN eClass, Object* pObject, Stz*);
extern pDUMPFUN *gpDump[];

typedef void (pFINALIZE(eCN eClass, Object* pObject));
extern pFINALIZE *gpFinalize[];

extern i8 giClassSize[];

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
