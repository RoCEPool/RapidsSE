/*============================================================================*/
/* Api.h - RapidsSE Api                                                       */
/*============================================================================*/

#pragma once

#include "StorageEngine.h"

extern "C" void   vRapidsSE_PoolOpen(void);
extern "C" void   vRapidsSE_SEitfDelete(SEitf* pSEitf);
extern "C" SEitf* pRapidsSE_SEitfNew(void);
extern "C" char*  pRapidsSE_GetCatalogs(SEitf* pSEitf);
  
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/