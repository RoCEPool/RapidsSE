/*============================================================================*/
/* RapidsSE.hpp -- RapidsSE External Api                                      */
/*============================================================================*/
#pragma once

////////////////////////////////////////////////////////////////////////////////
//                             System Level Calls                             //
////////////////////////////////////////////////////////////////////////////////

extern "C" eEC  RapidsSE_CheckVersion     (i8 iTrace,
                                           Stz* sVersion);
extern "C" eEC  RapidsSE_DeleteSession    (i8 iTrace,
                                           Session** ppSession);
extern "C" int  RapidsSE_IsInvalidSession (i8 iTrace,
                                           int eError);
extern "C" eEC  RapidsSE_NewSession       (i8 iTrace,
                                           Stz* sCatalog, Stz* sSchema, i8 iPartition,
                                           void** aaSession);
extern "C" eEC  RapidsSE_OpenPool         (i8 iTrace,
                                           Stz* sRegion, void** aaAttachMap);

////////////////////////////////////////////////////////////////////////////////
//                             Session Level Calls                            //
////////////////////////////////////////////////////////////////////////////////
extern "C" eEC  RapidsSE_BeginFetch      (i8 iTrace, Session* pSession,
                                          Stz* aTable, void** aaTable);
extern "C" eEC  RapidsSE_BeginInsert     (i8 iTrace, Session* pSession,
                                          Stz* sTable, i8 nSubInserters, void** aaJob);
extern "C" eEC  RapidsSE_EndJobs         (i8 iTrace, Session* pSession);
extern "C" eEC  RapidsSE_ExecuteCommand  (i8 iTrace, Session* pSession,
                                          Stz* sCatalog, Stz* sSchema, Stz* sCommand,
                                          void** aaJob);
extern "C" eEC  RapidsSE_InsertRow       (i8 iTrace, Session* pSession,
                                          Stz* pTable, Stz* sRecord);
extern "C" eEC  RapidsSE_NextRow         (i8 iTrace, Session* pSession, void* aTable,
                                          i8 iPartition, i8* aiExpectedRows, void** aaEntryBuffer,
                                          void** aaItem, void** aaRow);
extern "C" eEC  RapidsSE_TranslateError  (i8 iTrace, Session* pSession,
                                          int iErrorCode, void** aaError);

////////////////////////////////////////////////////////////////////////////////
//                               Job Level Calls                              //
////////////////////////////////////////////////////////////////////////////////

extern "C" eEC  RapidsSE_BeginSubInserter(i8 iTrace, Session* pSession, void* aJob,
                                          void** aaJob);
extern "C" eEC  RapidsSE_CopyBuffer      (i8 iTrace, Session* pSession, void* aJob);
extern "C" eEC  RapidsSE_EndInsert       (i8 iTrace, Session* pSession, void* aJob);
extern "C" eEC  RapidsSE_EndSubInserter  (i8 iTrace, Session* pSession, void* aJob);
extern "C" eEC  RapidsSE_FetchNextBuffer (i8 iTrace, Session* pSession, void* aJob,
                                          void** aaFetchBuffer, i8* aiExepectedRows);
extern "C" eEC  RapidsSE_GetLoadResult   (i8 iTrace, Session* pSession, void* aJob,
                                          i8* aiValidRows, i8* aiInvalidRows, i8* aiSkippedRows);
extern "C" eEC  RapidsSE_GetFirstBuffer  (i8 iTrace, Session* pSession, void* aTable,
                                          i8 iPartition, void** aaEntryBuffer, i8* aiBufferNo, i8* aiRowCount);
extern "C" eEC  RapidsSE_GetNextBuffer   (i8 iTrace, Session* pSession, void** aaEntryBuffer, i8* aiBufferNo, i8* aiRowCount);
extern "C" eEC  RapidsSE_GetRecordBuffer (i8 iTrace, Session* pSession, void* aJob,
                                          void** aaBuffer);
extern "C" eEC  RapidsSE_GetRow          (i8 iTrace, Session* pSession, void* aJob,
                                          i8 iRow, void** aaRow);
extern "C" eEC  RapidsSE_GetRowCount     (i8 iTrace, Session* pSession, void* aJob,
                                          i8 *aiRowCount);
extern "C" eEC  RapidsSE_GetTableColumnRow(i8 iTrace, Session* pSession, void* aJob,
                                           i8 i_Col, void** aaRow);
extern "C" eEC  RapidsSE_GetTableColumnRowCount(i8 iTrace, Session* pSession, void* aJob,
                                                i8* aRowCount);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
