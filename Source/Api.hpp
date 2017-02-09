/*============================================================================*/
/* Api.h - RapidsSE Api                                                       */
/*============================================================================*/

#pragma once

#include "StorageEngine.h"

// external SE API calls
extern "C" eEC RapidsSE_AddColumn(SEitf* pSEitf, char* pColumnName, char* pColumnType);
extern "C" eEC RapidsSE_BeginInsert(SEitf* pSEitf, char* pCatalog, char* pSchema, char* pTable);
extern "C" eEC RapidsSE_CheckLibraryVersion(char* pVersion);
extern "C" eEC RapidsSE_CreateTable(SEitf* seitf, char* catalog, char* pSchema, char* table);
extern "C" eEC RapidsSE_DropTable(SEitf* pSEitf, char* pCatalog, char* pSchema, char* pTable);
extern "C" eEC RapidsSE_EndCreate(SEitf* pSEitf);
extern "C" eEC RapidsSE_EndInsert(SEitf* pSEitf);
extern "C" eEC RapidsSE_GetCatalogs(SEitf* pSEitf, void** ppCatalog);
extern "C" eEC RapidsSE_GetColumns(SEitf* pSEitf, char* pCatalog,  char* pSchema, char* pTable, void** ppColumns);
extern "C" eEC RapidsSE_GetLastError(SEitf* pSEitf, void** ppError);
extern "C" eEC RapidsSE_GetNextRow(SEitf* pSEitf, void** ppRow);
extern "C" eEC RapidsSE_GetPartitions(SEitf* pSEitf, void** ppParitions, void** ppCount);
extern "C" eEC RapidsSE_GetSchemas(SEitf* pSEitf, char* pCatalog, void** ppSchema);
extern "C" eEC RapidsSE_GetEngineVersion(SEitf* pSEitf, void** ppVersion);
extern "C" eEC RapidsSE_GetTables(SEitf* pSEitf, char* pCatalog, char* pSchema, void** ppTables);
extern "C" eEC RapidsSE_HashRowPartition(SEitf* pSEitf, char* pRow, void** ppPartition);
extern "C" eEC RapidsSE_InsertRow(SEitf* pSEitf, char* pRow);
extern "C" eEC RapidsSE_PoolOpen(char* pInstanceName, void** ppInstance);
extern "C" eEC RapidsSE_ScanTable(SEitf* pSEitf, char* pCatalog, char* pSchema, char* pTable);
extern "C" eEC RapidsSE_SEitfDelete(void* pAttachMap, SEitf* pSEitf);
extern "C" eEC RapidsSE_SEitfNew(void* pAttachMap, void** ppSEitf, i8 iPartition);

extern "C" bool RapidsSE_SEitfInvalid(int eError);

bool RapidsSE_ExecuteRequest(SEitf* pSEitf, char* pWords[], i8 iWords, char** ppResponse);

// private calls
eEC eExecuteRequest(SEitf* pSEitf, const char* pCommand, const char* pParameter);
eEC eExecuteRequest(SEitf* pSEitf, const char* pCommand,
    const char* pParameter1, const char* pParameter2, const char* pParameter3);

eEC eExecuteRequest(SEitf* pSEitf, const char* pCommand);
bool bExecuteRequest(SEitf* pSEitf, char* pWords[], i8 iWords);
eEC eGetResponse(SEitf* pSEitf);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
