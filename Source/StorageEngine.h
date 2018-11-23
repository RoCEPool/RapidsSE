/*============================================================================*/
/* StorageEngine.h -- StorageEngine namespace                                 */
/*============================================================================*/

#pragma once

#include "Prompt.h"

//-----------------------------------------------------------------------------

namespace StorageEngine
{
  void vCheckAllAssumptions(void);
  void vCheckAssumptions(void);
  void vSetSignalHandlers(void);
  
  void vAddConstant(Stz* aName, i8 iVal);
  void vAddConstant(Stz* aName, Stz* aValue);
  void vAddConstants(void);
}

using namespace StorageEngine;

//-----------------------------------------------------------------------------

#include "ApiState.hpp"
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
