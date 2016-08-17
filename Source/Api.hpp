/*============================================================================*/
/* Api.h - RapidsSE Api                                                       */
/*============================================================================*/

#pragma once

#include "StorageEngine.h"

extern "C" char*  pRapidsSE_GetCatalogs(SEitf* pSEitf);
extern "C" char*  pRapidsSE_GetSchemas(SEitf* pSEitf, char* pCatalog);
extern "C" char*  pRapidsSE_GetTables(SEitf* pSEitf, char* pCatalog, char* pSchema);
extern "C" void   vRapidsSE_PoolOpen(void);
extern "C" void   vRapidsSE_SEitfDelete(SEitf* pSEitf);
extern "C" SEitf* pRapidsSE_SEitfNew(void);
  
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/