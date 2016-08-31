/*============================================================================*/
/* Api.h - RapidsSE Api                                                       */
/*============================================================================*/

#pragma once

#include "StorageEngine.h"

// external SE API calls
extern "C" eIE RapidsSE_ExecuteCommand(SEitf* pSEitf, char* pWords[], char** ppResponse);
extern "C" eIE RapidsSE_GetCatalogs(SEitf* pSEitf, void** ppCatalog);
extern "C" eIE RapidsSE_GetColumns(SEitf* pSEitf, char* pCatalog,  char* pSchema, char* pTable, void** ppColumns);
extern "C" eIE RapidsSE_GetErrorCode(SEitf* pSEitf, i8* pError);
extern "C" eIE RapidsSE_GetLastError(SEitf* pSEitf, void** ppError);
extern "C" eIE RapidsSE_GetNextRow(SEitf* pSEitf, void** ppRow);
extern "C" eIE RapidsSE_GetPartitions(SEitf* pSEitf, void** ppParitions);
extern "C" eIE RapidsSE_GetSchemas(SEitf* pSEitf, char* pCatalog, void** ppSchema);
extern "C" eIE RapidsSE_GetTables(SEitf* pSEitf, char* pCatalog, char* pSchema, void** ppTables);
extern "C" eIE RapidsSE_PoolOpen(void);
extern "C" eIE RapidsSE_ScanTable(SEitf* pSEitf, char* pCatalog, char* pSchema, char* pTable);
extern "C" eIE RapidsSE_SEitfDelete(SEitf* pSEitf);
extern "C" eIE RapidsSE_SEitfNew(void** ppSEitf, i8 iPartition);


// private calls
eIE eExecuteCommand(SEitf* pSEitf, const char* pCommand, const char* pParameter);
eIE eExecuteCommand(SEitf* pSEitf, const char* pCommand);
eIE eExecuteCommand(SEitf* pSEitf, char* pWords[]);
eIE eGetResponse(SEitf* pSEitf);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/