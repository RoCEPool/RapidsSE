/*============================================================================*/
/* StorageEngine.h -- StorageEngine namespace                                 */
/*============================================================================*/

#pragma once

#include "Interface.h"

//-----------------------------------------------------------------------------

namespace StorageEngine
{
  void vCheckAllAssumptions(void);
  void vCheckAssumptions(void);
}

using namespace StorageEngine;

//-----------------------------------------------------------------------------

#include "Arguments.hpp"
#include "AttachMap.hpp"
#include "Region.hpp"
#include "SizesDumps.hpp"

#include "RapidsSE.hpp"

typedef void (pDUMPFUN)(eCN eClass, Object* pObject, Str*);
extern pDUMPFUN *gpDump[];
extern i8 giClassSize[];

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
