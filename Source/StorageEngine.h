/*============================================================================*/
/* StorageEngine.h -- StorageEngine namespace                                 */
/*============================================================================*/

#pragma once

#include "GridModel.h"

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

typedef void (pDUMPFUN)(eCN eClass, Object* pObject, const char*);
extern pDUMPFUN *gpDump[];
extern i8 giClassSize[];

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
