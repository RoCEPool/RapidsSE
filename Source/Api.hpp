/*============================================================================*/
/* Api.h - RapidsSE Api                                                       */
/*============================================================================*/

#pragma once

#include "StorageEngine.h"

// external SE API calls
extern "C" eEC RapidsSE_ExecuteRequest(SEitf* pSEitf, char* pWords[], char** ppResponse);
extern "C" eEC RapidsSE_GetCatalogs(SEitf* pSEitf, void** ppCatalog);
extern "C" eEC RapidsSE_GetColumns(SEitf* pSEitf, char* pCatalog,  char* pSchema, char* pTable, void** ppColumns);
extern "C" eEC RapidsSE_GetErrorCode(SEitf* pSEitf, i8* pError);
extern "C" eEC RapidsSE_GetLastError(SEitf* pSEitf, void** ppError);
extern "C" eEC RapidsSE_GetNextRow(SEitf* pSEitf, void** ppRow);
extern "C" eEC RapidsSE_GetPartitions(SEitf* pSEitf, void** ppParitions);
extern "C" eEC RapidsSE_GetSchemas(SEitf* pSEitf, char* pCatalog, void** ppSchema);
extern "C" eEC RapidsSE_GetTables(SEitf* pSEitf, char* pCatalog, char* pSchema, void** ppTables);
extern "C" eEC RapidsSE_PoolOpen(void);
extern "C" eEC RapidsSE_ScanTable(SEitf* pSEitf, char* pCatalog, char* pSchema, char* pTable);
extern "C" eEC RapidsSE_SEitfDelete(SEitf* pSEitf);
extern "C" eEC RapidsSE_SEitfNew(void** ppSEitf, i8 iPartition);


// private calls
eEC eExecuteRequest(SEitf* pSEitf, const char* pCommand, const char* pParameter);
eEC eExecuteRequest(SEitf* pSEitf, const char* pCommand);
eEC eExecuteRequest(SEitf* pSEitf, char* pWords[]);
eEC eGetResponse(SEitf* pSEitf);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/