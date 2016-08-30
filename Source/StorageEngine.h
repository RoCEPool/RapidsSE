/*============================================================================*/
/* StorageEngine.h - includes for StorageEngine namespace                     */
/*============================================================================*/

#pragma once

#include "GridModel.h"

#include "InProcess.hpp"
#include "OnHeap.hpp"

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
