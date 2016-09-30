/*============================================================================*/
/* StorageEngine.h - includes for StorageEngine namespace                     */
/*============================================================================*/

#pragma once

#include "GridModel.h"

#include "Arguments.hpp"
#include "AttachMap.hpp"
#include "Region.hpp"
#include "SizesDumps.hpp"

#include "RapidsSE.hpp"

typedef void (pDUMPFUN)(eCN eClass, Object* pObject, const char*);
extern pDUMPFUN *gpDump[];
extern i8 giClassSize[];

//-----------------------------------------------------------------------------
namespace StorageEngine
{
  void vCheckAssumptions(void);
}

using namespace StorageEngine;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
