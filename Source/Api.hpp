/*============================================================================*/
/* Api.h - RapidsSE Api                                                       */
/*============================================================================*/
#pragma once

// external SE API calls
extern "C" eEC RapidsSE_AddColumn(SEitf* pSEitf, Str* pColumnName, Str* pColumnType);
extern "C" eEC RapidsSE_BeginInsert(SEitf* pSEitf, Str* pCatalog, Str* pSchema, Str* pTable);
extern "C" eEC RapidsSE_CheckLibraryVersion(Str* pVersion);
extern "C" eEC RapidsSE_CreateTable(SEitf* seitf, Str* catalog, Str* pSchema, Str* table);
extern "C" eEC RapidsSE_DropTable(SEitf* pSEitf, Str* pCatalog, Str* pSchema, Str* pTable);
extern "C" eEC RapidsSE_EndCreate(SEitf* pSEitf);
extern "C" eEC RapidsSE_EndInsert(SEitf* pSEitf);
extern "C" eEC RapidsSE_EndScan(SEitf* pSEitf);
extern "C" eEC RapidsSE_GetCatalogs(SEitf* pSEitf, void** ppCatalog);
extern "C" eEC RapidsSE_GetColumns(SEitf* pSEitf, Str* pCatalog,  Str* pSchema, Str* pTable, void** ppColumns);
extern "C" eEC RapidsSE_GetLastError(SEitf* pSEitf, void** ppError);
extern "C" eEC RapidsSE_GetNextRow(SEitf* pSEitf, void** ppRow);
extern "C" eEC RapidsSE_GetPartitions(SEitf* pSEitf, void** ppParitions, void** ppCount);
extern "C" eEC RapidsSE_GetSchemas(SEitf* pSEitf, Str* pCatalog, void** ppSchema);
extern "C" eEC RapidsSE_GetEngineVersion(SEitf* pSEitf, void** ppVersion);
extern "C" eEC RapidsSE_GetTables(SEitf* pSEitf, Str* pCatalog, Str* pSchema, void** ppTables);
extern "C" eEC RapidsSE_HashRowPartition(SEitf* pSEitf, Str* pRow, void** ppPartition);
extern "C" eEC RapidsSE_InsertRow(SEitf* pSEitf, Str* pRow);
extern "C" eEC RapidsSE_PoolOpen(Str* pInstanceName, void** ppInstance);
extern "C" eEC RapidsSE_ScanTable(SEitf* pSEitf, Str* pCatalog, Str* pSchema, Str* pTable);
extern "C" eEC RapidsSE_SEitfDelete(void* pAttachMap, SEitf* pSEitf);
extern "C" eEC RapidsSE_SEitfNew(void* pAttachMap, void** ppSEitf, i8 iPartition);

extern "C" bool RapidsSE_SEitfInvalid(int eError);

bool RapidsSE_ExecuteRequest(SEitf* pSEitf, Str* aWords[], i8 iWords, Str** ppResponse);

// private calls
eEC eExecuteRequest(SEitf* pSEitf, Str* aCommand);
eEC eExecuteRequest(SEitf* pSEitf, Str* aCommand, Str* aParameter);
eEC eExecuteRequest(SEitf* pSEitf, Str* aCommand, Str* aParameter1,
                    Str* aParameter2, Str* aParameter3);

bool bExecuteRequest(SEitf* pSEitf, Str* aWords[], i8 iWords);
eEC eGetResponse(SEitf* pSEitf);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
