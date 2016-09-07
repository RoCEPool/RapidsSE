/*============================================================================*/
/* StorageEngine.h - includes for StorageEngine namespace                     */
/*============================================================================*/

#pragma once

#include "GridModel.h"

#include "Arguments.hpp"
#include "RapidsSE.hpp"
#include "AttachMap.hpp"

typedef void (pDUMPFUN)(eCN eClass, Object* pObject, const char*);
extern pDUMPFUN *gpDump[];

//-----------------------------------------------------------------------------
namespace StorageEngine
{
  void vCheckAssumptions(void);
}

using namespace StorageEngine;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
