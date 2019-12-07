/*============================================================================*/
/* RapidsSE.cpp -- RapidsSE External Api                                      */
/*============================================================================*/

#include "Interface.h"
#include "StorageEngine.h"

extern BufferPool*  gpBufferPool;
extern AttachMap*   gpAttachMap;
extern Log* gpLog;


////////////////////////////////////////////////////////////////////////////////
//                             Library Level Calls                             //
////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* check the library version against given version                            */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_CheckVersion(i8 iTrace, Stz* szVersion)
{
  API_SYSTEM_LEVEL();
  API_ENTER(AC_CheckVersion);
  API_PARAM(szVersion);
  API_BEGIN();
  
  try {
    
    if ( ! szVersion->bEqual((Stz*)RAPIDSSE_VERSION_STRING)) {
      
      vTrace(">RapidsSE_CheckLibraryVersion: RDP (%s) does not match Engine (%s)"_Stz,
             szVersion, RAPIDSSE_VERSION_STRING);
      
      API_RETURN_ERROR_NO_SESSION(EC_VERSION_MISMATCH);
    }
  }
  catch (Error* pError) { API_CATCH_NO_SESSION(pError); }
  catch (...) { Error* pError = nullptr; API_CATCH_NO_SESSION(pError); }
  
  API_RETURN();
}

/*----------------------------------------------------------------------------*/
/* delete an se session                                                       */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_DeleteSession(i8 iTrace, Session** ppSession)
{
  Session* pSession = *ppSession;
  
  API_SESSION_LEVEL();
  API_ENTER(AC_DeleteSession);
  API_BEGIN();
  
  try {
    
    // delete jobs from the job stack of the session
    Api::vDeleteJobs(pSession);
    API_LOC(310);
    
    // delete the session
    Engine::vDeleteSession(pSession);
    //API_LOC(320);
    
    // set session to nullptr so macro knows not to look at it
    pSession = nullptr;
    
    // retrun back null session
    *ppSession = nullptr;
    
    //API_LOC(330);
  }
  catch (Error* pError) { API_CATCH(pError); }
  catch (...) { Error* pError = nullptr; API_CATCH(pError); }
  
  API_RETURN();
}

/*----------------------------------------------------------------------------*/
/* is the error an EC_INVALID_INTERFACE ?                                     */
/*----------------------------------------------------------------------------*/
int RapidsSE_IsInvalidSession(i8 iTrace, int iError)
{
  API_SYSTEM_LEVEL();
  API_ENTER(AC_IsInvalidSession);
  API_BEGIN();
  API_EXITING();
  
  if (iError == EC_INVALID_INTERFACE) {
    return 1;
  }
  else
    return 0;
}

/*----------------------------------------------------------------------------*/
/* create a new se session                                                    */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_NewSession(i8 iTrace,
                        Stz* szCatalog, Stz* szSchema, i8 iPartition,
                        void** aaSession)
{
  API_SYSTEM_LEVEL();
  API_ENTER(AC_NewSession);
  API_PARAM(szCatalog);
  API_PARAM(szSchema);
  API_NOTNULL(aaSession);
  API_BEGIN();
  try {
    
    API_VALIDATE_POOL();
   
    //pSession = new Session(iPartition);
    pSession = AM::pSessionArray()->pAllocSession();
    pSession->vPartition(iPartition);
    
    eEC eError = pSession->eInit(szCatalog,szSchema);

    
    if (eError != EC_NONE)
      API_RETURN_ERROR(eError);
    
    // return the newly created session
    *aaSession = pSession;
    pSession = nullptr;
  }
  catch (Error* pError) { API_CATCH(pError); }
  catch (...) { Error* pError = nullptr; API_CATCH(pError); }

  API_RETURN();
}

/*----------------------------------------------------------------------------*/
/* open the shared memory buffer pool                                         */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_OpenPool(i8 iTrace, Stz* szRegion, void** aaAttachMap)
{
  API_SYSTEM_LEVEL();
  API_ENTER(AC_OpenPool);
  API_PARAM(szRegion);
  API_BEGIN();
  
  try {
    
    // open the pool
    s32 sPoolName;
    sPoolName = (Stz*)szRegion;
    
    bool bResult = Engine::bOpenPool(sPoolName);
    if (! bResult)
      API_RETURN_ERROR(EC_INVALID_INSTANCE);
    
    API_VALIDATE_POOL();
    
    *aaAttachMap = gpAttachMap;
    
    AM::vLogOpenByApi();
    
#ifdef WITH_STATISTICS_COUNTERS
    if (gpAttachMap)
      gpAttachMap->m_oStatistics.Api.m_nPoolOpen++;
#endif
      
  }
  catch (Error* pError) { API_CATCH(pError); }
  catch (...) { Error* pError = nullptr; API_CATCH(pError); }
  
  API_RETURN();
}


////////////////////////////////////////////////////////////////////////////////
//                             Session Level Calls                            //
////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* begin the insert table job                                                 */
/*----------------------------------------------------------------------------*/
eEC  RapidsSE_BeginFetch(i8 iTrace, Session* pSession,
                         Stz* szTable, void** aaTable)
{
  API_SESSION_LEVEL();
  API_ENTER(AC_BeginFetch);
  API_PARAM(szTable);
  API_NOTNULL(aaTable);
  API_BEGIN();
  
  try {
    
    // find the table in the session dictionary stack
    Table* pTable = Table::pFindTable(pSession->pPathStack(), szTable);
    API_LOC(310);
    
    *aaTable = pTable;
    API_LOC(320);
  }
  catch (Error* pError) { API_CATCH(pError); }
  catch (...) { Error* pError = nullptr; API_CATCH(pError); }
  
  API_RETURN();
}

/*----------------------------------------------------------------------------*/
/* begin the insert table job                                                 */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_BeginInsert(i8 iTrace, Session* pSession,
                         Stz* szTable, i8 nSubInserters, void** aaJob)
{
  API_SESSION_LEVEL();
  API_ENTER(AC_BeginInsert);
  API_NOTNULL(aaJob);
  API_BEGIN();
  
  try {
 
    // create Job_InsertTable
    JobClass* pJobClass_InsertTable = JobClass::pFind(CN_JOB_INSERTTABLE);
    ASSERT(pJobClass_InsertTable);
    API_LOC(310);
    
    ReqTuple xReqTuple("!!BEGIN_INSERT"_Stz,false);
    
    Job_InsertTable* pJob_InsertTable =
      static_cast<Job_InsertTable*>
        (pJobClass_InsertTable->pPreJob(&xReqTuple,pSession));
    API_LOC(320);

    // find the table in the session dictionary stack
    Table* pTable = Table::pFindTable(pSession->pPathStack(), szTable);
    API_LOC(330);

    // set the table in the InsertTable job
    pJob_InsertTable->m_pTable = pTable;
    API_LOC(340);

    // return the insertTable job
    *aaJob = pJob_InsertTable;
    API_LOC(350);

    // set subInserters in the TblPtb
    AM::vSubInserters(nSubInserters);   //! Is this right? (ray)
    API_LOC(360);

    // push the InsertTable job onto the jobstack
    pSession->m_pJobStack->vPushPtr(pJob_InsertTable);
    API_LOC(370);

    SessionArray::vMarkJob(pSession,pJob_InsertTable);
    API_LOC(380);
  }
  catch (Error* pError) { API_CATCH(pError); }
  catch (...) { Error* pError = nullptr; API_CATCH(pError); }
  
  API_RETURN();
}

/*----------------------------------------------------------------------------*/
/* end all jobs of this session                                               */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_EndJobs(i8 iTrace, Session* pSession)
{
  API_SESSION_LEVEL();
  API_ENTER(AC_EndJobs);
  API_BEGIN();
  
  // delete the jobs from the sessions jobStack
  Api::vDeleteJobs(pSession);
  API_LOC(310);

  // mark session as exiting (any fetches still running will stop)
  pSession->m_bSessionExiting = true;
  API_LOC(320);

  API_RETURN();
}

/*----------------------------------------------------------------------------*/
/* execute command                                                            */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_ExecuteCommand(i8 iTrace, Session* pSession,
                            Stz* szCatalog, Stz* szSchema, Stz* szCommand,
                            void** aaJob)
{
  API_SESSION_LEVEL();
  API_ENTER(AC_ExecuteCommand);
  API_PARAM(szCommand);
  API_BEGIN();
  
  try {
    
    // set the catalog and schema in the session
    eEC eError = pSession->eInit(szCatalog, szSchema);
    if (eError != EC_NONE)
      API_RETURN_ERROR(eError);
    API_LOC(310);

    // execute the command
    Job* pJob = Api::pExecuteCommand(pSession, szCommand, false, false);
    API_LOC(320);

    SessionArray::vMarkJob(pSession,pJob);
    API_LOC(330);

    // check if job had an error
    if (pJob->bHasError()) {
      i8 iResponses = pJob->m_pResponseVec->nItems();
      
      for (i8 i_Node = 0; i_Node < iResponses; i_Node++) {
        eEC eError = pJob->m_pResponseVec->pGet(i_Node)->m_eErrorCode;
        if (eError != EC_NONE) {
          pSession->m_pJobStack->vPushPtr(pJob);
          API_RETURN_ERROR(eError);
        }
      }
      NEVER_REACH;
    }
    API_LOC(340);

    // return job in aaJob
    *aaJob = pJob;
    API_LOC(350);

    // push the job on the sessions job stack
    pSession->m_pJobStack->vPushPtr(pJob);
    API_LOC(360);
  }
  catch (Error* pError) { API_CATCH(pError); }
  catch (...) { Error* pError = nullptr; API_CATCH(pError); }
  
  API_RETURN();
}

/*----------------------------------------------------------------------------*/
/* insert the given row into a table                                          */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_InsertRow(i8 iTrace, Session* pSession,
                       Stz* szTableNm, Stz* szRecord)
{
  API_SESSION_LEVEL();
  API_ENTER(AC_InsertRow);
  API_NOTNULL(szRecord);
  API_BEGIN();
  
  try {
    
    // create Job_InsertTable
    JobClass* pJobClass_InsertRow = JobClass::pFind(CN_JOB_INSERTROW);
    ASSERT(pJobClass_InsertRow);
    API_LOC(310);
    
    ReqTuple xReqTuple("!!INSERT_ROW"_Stz,false);
    
    Job_InsertRow* pJob_InsertRow =
      static_cast<Job_InsertRow*>
        (pJobClass_InsertRow->pPreJob(&xReqTuple,pSession));
    API_LOC(320);
    
    // push the InsertTable job onto the jobstack
    pSession->m_pJobStack->vPushPtr(pJob_InsertRow);
    API_LOC(330);
    
    SessionArray::vMarkJob(pSession,pJob_InsertRow);
    API_LOC(340);
    
    // get the length of the record
    i8 iRawDataLength = StrLib::iSize(szRecord->azChar());
    API_LOC(350);
    
    // check length of record
    if (iRawDataLength > C_MAX_RECORD)
      API_RETURN_ERROR(EC_MAX_ROW_LENGTH);
    API_LOC(360);
    
    // set the row in the insertRow job
    pJob_InsertRow->pEnameSet()->vAdd("<tableNm>"_Stz, szTableNm);
    pJob_InsertRow->m_pRecord->vRecStr(szRecord);
    
    //pJob_InsertRow->m_nRawDataBytes = iRawDataLength;
    API_LOC(370);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // spawn and wait for the job
    Api::vSpawnJobAndWait(pJob_InsertRow);
    API_LOC(380);
    
    // check if job had an error
    if (pJob_InsertRow->bHasError()) {
      
      i8 iResponses = pJob->m_pResponseVec->nItems();
      
      for (i8 i_Node = 0; i_Node < iResponses; i_Node++) {
        eEC eError = pJob->m_pResponseVec->pGet(i_Node)->m_eErrorCode;
        
        if (eError != EC_NONE)
          API_RETURN_ERROR(eError);
      }
      NEVER_REACH;
    }
    API_LOC(390);
  }
  catch (Error* pError) { API_CATCH(pError); }
  catch (...) { Error* pError = nullptr; API_CATCH(pError); }
  
  API_RETURN();
}

/*----------------------------------------------------------------------------*/
/* translate error code                                                       */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_TranslateError(i8 iTrace, Session* pSession,
                            int iErrorCode, void** aaError)
{
  API_SESSION_LEVEL();
  API_ENTER(AC_TranslateError);
  API_NOTNULL(aaError);
  API_BEGIN();
  
  try {
    
    // get the error message
    pSession->m_sResponse = aEC[(eEC)iErrorCode];
    API_LOC(310);

    *aaError = pSession->m_sResponse.azChar();
    API_LOC(320);
  }
  catch (Error* pError) { API_CATCH(pError); }
  catch (...) { Error* pError = nullptr; API_CATCH(pError); }
  
  API_RETURN();
}


////////////////////////////////////////////////////////////////////////////////
//                               Job Level Calls                              //
////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* begin the subInserter job                                                  */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_BeginSubInserter(i8 iTrace, Session* pSession, void* aJob,
                              void** aaJob)
{
  API_JOB_LEVEL();
  API_ENTER(AC_BeginSubInserter);
  API_NOTNULL(aJob);
  API_NOTNULL(aaJob);
  API_BEGIN();
  
  try {
    
    // get the insertTable job
    Job_InsertTable* pJob_InsertTable = _pJob_InsertTable(aJob);
    API_LOC(310);

    ReqTuple* aReqTuple = &pJob_InsertTable->m_xReqTuple;
    
    // create Job_SubInserter
    JobClass* pJobClass_SubInserter = JobClass::pFind(CN_JOB_SUBINSERTER);
    ASSERT(pJobClass_SubInserter);
    API_LOC(320);

    Job_SubInserter* pJob_SubInserter =
      static_cast<Job_SubInserter*>
        (pJobClass_SubInserter->pPreJob(aReqTuple,pSession));
    API_LOC(330);

    // push the InsertTable job onto the jobstack
    pSession->m_pJobStack->vPushPtr(pJob_SubInserter);
    SessionArray::vMarkJob(pSession,pJob_SubInserter);
    API_LOC(340);

    // set the insertTable in the subInserter
    pJob_SubInserter->m_pJob_InsertTable = pJob_InsertTable;
    pJob_SubInserter->m_pParentJob = pJob_InsertTable;
    API_LOC(350);

    // set the table pointer (THIS CAN GO ????)
    pJob_SubInserter->m_pTable = pJob_InsertTable->m_pTable;
    API_LOC(360);

    // return the sub_inserter job
    *aaJob = pJob_SubInserter;
    API_LOC(370);
  }
  catch (Error* pError) { API_CATCH(pError); }
  catch (...) { Error* pError = nullptr; API_CATCH(pError); }
  
  API_RETURN();
}

/*----------------------------------------------------------------------------*/
/* insert the record buffer into a table                                      */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_CopyBuffer(i8 iTrace, Session* pSession, void* aJob)
{
  API_JOB_LEVEL();
  API_ENTER(AC_CopyBuffer);
  API_BEGIN();
  
  try {
    
    // get the insertRow job
    Job_SubInserter* pJob_SubInserter = _pJob_SubInserter(aJob);
    API_LOC(310);

    ReqTuple* aReqTuple = &pJob_SubInserter->m_xReqTuple;
    
    // get the record buffer
    RecordBuffer* pRecordBuffer = pJob_SubInserter->m_pRecordBuffer;
    API_LOC(320);

    //pJob_SubInserter->m_pRecordBuffer->vShow();
    
    if (! pRecordBuffer)
      API_RETURN();
    API_LOC(330);

    // create Job_CopyBuffer
    JobClass* pJobClass_CopyBuffer = JobClass::pFind(CN_JOB_COPYBUFFER);
    ASSERT(pJobClass_CopyBuffer);
    API_LOC(340);

    Job_CopyBuffer* pJob_CopyBuffer =
      static_cast<Job_CopyBuffer*>
        (pJobClass_CopyBuffer->pPreJob(aReqTuple,pSession));
    API_LOC(350);

    // set the pJob_SubInserter and table in the pJob_CopyBuffer
    pJob_CopyBuffer->m_pJob_SubInserter = pJob_SubInserter;
    pJob_CopyBuffer->m_pTable = pJob_SubInserter->m_pTable;
    API_LOC(360);

    // move the recordBuffer from the subInsert into CopyBuffer
    pJob_CopyBuffer->m_pRecordBuffer = pRecordBuffer;
    API_LOC(370);
    
    ++pJob_SubInserter->m_ixRecordBuffers;
    pJob_SubInserter->m_pRecordBuffer = nullptr;
    API_LOC(380);

    // push the InsertTable job onto the jobstack
    //pSession->m_pJobStack->vPushPointer(pJob_CopyBuffer);
    //ApiState::vMarkJob(pSession,pJob_CopyBuffer);
    
    pJob_CopyBuffer->vSeed(SS_SEED_REQ);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // spawn the job
    //vTrace("Spawning copyBuffer job\n"_Stz);
    IGNORE_RETURN(Api::bSpawnJob(pJob_CopyBuffer));
    API_LOC(390);
  }
  catch (Error* pError) { API_CATCH(pError); }
  catch (...) { Error* pError = nullptr; API_CATCH(pError); }
  
  API_RETURN();
}

/*----------------------------------------------------------------------------*/
/* end insert and all session jobs                                            */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_EndInsert(i8 iTrace, Session* pSession, void* aJob)
{
  API_JOB_LEVEL();
  API_ENTER(AC_EndInsert);
  API_BEGIN();
  
  try {
    
    // get the insertTable job
    Job_InsertTable* pJob_InsertTable = _pJob_InsertTable(aJob);
    API_LOC(310);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // spawn and wait for job
    Api::vSpawnJobAndWait(pJob_InsertTable);
    API_LOC(320);

    // check if job had an error
    if (pJob_InsertTable->bHasError()) {
      i8 iResponses = pJob->m_pResponseVec->nItems();
      
      for (i8 i_Node = 0; i_Node < iResponses; i_Node++) {
        eEC eError = pJob->m_pResponseVec->pGet(i_Node)->m_eErrorCode;
        if (eError != EC_NONE)
          API_RETURN_ERROR(eError);
      }
      NEVER_REACH;
    }
    API_LOC(330);

    // delete all jobs in the ssession
    Api::vDeleteJobs(pSession);
    API_LOC(340);
  }
  catch (Error* pError) { API_CATCH(pError); }
  catch (...) { Error* pError = nullptr; API_CATCH(pError); }
  
  API_RETURN();
}

/*----------------------------------------------------------------------------*/
/* end subinserter and all session jobs                                       */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_EndSubInserter(i8 iTrace, Session* pSession, void* aJob)
{
  API_JOB_LEVEL();
  API_ENTER(AC_EndSubInserter);
  API_BEGIN();
  
  try {
    
    // get the insertRows job
    Job_SubInserter* pJob_SubInserter = _pJob_SubInserter(aJob);
    API_LOC(310);

    // wait for all record buffers to be copied across
    while (*pJob_SubInserter->m_ixRecordBuffers !=
           *pJob_SubInserter->m_ixCopiedBuffers) {
      //vTrace("Waiting %lld %lld\n"_Stz, *pJob_SubInserter->m_ixRecordBuffers, *pJob_SubInserter->m_ixCopiedBuffers);
    }
    API_LOC(320);

    pJob_SubInserter->vEnd();
    API_LOC(330);

    // delete all jobs in the ssession
    Api::vDeleteJobs(pSession);
    API_LOC(340);
  }
  catch (Error* pError) { API_CATCH(pError); }
  catch (...) { Error* pError = nullptr; API_CATCH(pError); }
  
  API_RETURN();
}

/*----------------------------------------------------------------------------*/
/* fetch next buffer                                                          */
/*----------------------------------------------------------------------------*/
//
// THIS SHOULD GO
//

static const i8 C_ROWS_PER_BUFFER
  = (C_SIZE_LARGE_OBJECT - C_1KB*4)/sizeof(CacheLine*);

eEC RapidsSE_FetchNextBuffer(i8 iTrace, Session* pSession, void* aJob,
                             void** aaFetchBuffer, i8* aiExpectedRows)
{
  API_JOB_LEVEL();
  API_ENTER(AC_FetchNextBuffer);
  API_NOTNULL(aaFetchBuffer);
  API_BEGIN();
  
  try {
    
    Job_FetchRows* pJob_FetchRows = _pJob_FetchRows(aJob);
    API_LOC(310);

    // if we had an entry buffer delete it
    pJob_FetchRows->vDeleteBuffer();
    API_LOC(320);

    // are we done ?
    if (pJob_FetchRows->m_nLastBuffersScanned ==
        pJob_FetchRows->m_nLastBuffers) {
      *aaFetchBuffer = nullptr;
      API_RETURN();
    }
    API_LOC(330);

    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
    
    FetchBuffer* pFetchBuffer;
    for (;;) {
      pFetchBuffer = _pFetchBuffer(pJob_FetchRows->m_pBufferQueue->pPop());
      if (pFetchBuffer)
        break;
      //vTrace("SLEEPING\n"_Stz);
      AM::vNapUsec(250,CN_JOB_FETCHROWS);
    }
    pJob_FetchRows->m_pFetchBuffer = pFetchBuffer;
    API_LOC(340);

    *aiExpectedRows = (i8)(pJob_FetchRows->m_pBufferQueue->iCount()
                           * C_ROWS_PER_BUFFER);
    API_LOC(350);

    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

    // is it a last buffer ?
    if (pJob_FetchRows->m_pFetchBuffer->bIsLast()) {
      pJob_FetchRows->m_nLastBuffersScanned++;
      API_LOC(360);
      
//      vTrace("FNB: LAST %p %s m_nLastBuffersScanned: %lld %lld\n"_Stz, pJob_FetchRows->m_pFetchBuffer,
//             pJob_FetchRows->m_pTable->sTable(),
//             pJob_FetchRows->m_nLastBuffersScanned, pJob_FetchRows->m_nLastBuffers );
    }
    
//    vTrace("FNB: NOT LAST %p %s m_nLastBuffersScanned: %lld %lld\n"_Stz,pJob_FetchRows->m_pFetchBuffer,
//           pJob_FetchRows->m_pTable->sTable(),
//           pJob_FetchRows->m_nLastBuffersScanned, pJob_FetchRows->m_nLastBuffers );
    
    // return the fetched buffer
    *aaFetchBuffer = pJob_FetchRows->m_pFetchBuffer;
    API_LOC(370);
  }
  catch (Error* pError) { API_CATCH(pError); }
  catch (...) { Error* pError = nullptr; API_CATCH(pError); }
  
  API_RETURN();
}

/*----------------------------------------------------------------------------*/
/* fetch table                                                                */
/*----------------------------------------------------------------------------*/
eEC  RapidsSE_FetchTable (i8 iTrace, Session* pSession, Stz* szTable,
                          i8 iPartition, void** aaCrwBuffer, i8* aiRowCount)
{

  API_SESSION_LEVEL();
  API_ENTER(AC_FetchTable);
  API_NOTNULL(aaCrwBuffer);
  //API_NOTNULL(aiBufferNo);
  API_NOTNULL(aiRowCount);
  API_BEGIN();
  
  
  try {
    
    // find the table in the session dictionary stack
    Table* pTable = Table::pFindTable(pSession->pPathStack(), szTable);
    API_LOC(310);
    
    // get first entry buffer
    TablePtn* pPartition = pTable->pPartition(TablePtn::iPartitionToSlot(iPartition));
    API_LOC(330);
    
    CrwBuffer* pCrwBuffer = pPartition->pRootCrwBuffer();
    API_LOC(340);
    
    // return the first buffer
    *aaCrwBuffer = pCrwBuffer;
    
    i8 iRowCount = 0;
    while (pCrwBuffer) {
      
      iRowCount += pCrwBuffer->nRowCount();
      
      pCrwBuffer = pCrwBuffer->pNextBuffer();
    }
    
    // return table row count
    *aiRowCount = iRowCount;
    
    API_RETURN();
    
  }
  catch (Error* pError) { API_CATCH(pError);
    
  }
  catch (...) { Error* pError = nullptr; API_CATCH(pError);
    
  }

  API_RETURN();
   
}

/*----------------------------------------------------------------------------*/
/* get load result                                                            */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetLoadResult(i8 iTrace, Session* pSession, void* aJob,
                           i8* aiValidRows, i8* aiInvalidRows, i8* aiSkippedRows)
{
  API_JOB_LEVEL();
  API_ENTER(AC_GetLoadResult);
  API_NOTNULL(aiValidRows);
  API_NOTNULL(aiInvalidRows);
  API_BEGIN();
  
  try {
    
    Job_LoadTable* pJob_LoadTable = _pJob_LoadTable(aJob);
    API_LOC(310);
    //    vTrace("L %lld IN %lld\n"_Stz,
    //           pJob_LoadTable->m_iValidRows,
    //           pJob_LoadTable->m_iInvalidRows);
    
    i8 iRowsProcessed = 0;
    i8 iRowsInvalid   = 0;
    i8 iRowsSkipped   = 0;
    i8 iResponses = pJob->m_pResponseVec->nItems();
    
    for (i8 i_Node = 0; i_Node < iResponses; i_Node++) {
      Response* pResponse = pJob->m_pResponseVec->pGet(i_Node);
      iRowsProcessed += *pResponse->m_ixRowsProcessed;
      iRowsInvalid   += *pResponse->m_ixRowsInvalid;
      iRowsSkipped   += *pResponse->m_ixRowsSkipped;
    }
    API_LOC(320);
    
    *aiValidRows   = iRowsProcessed;
    *aiInvalidRows = iRowsInvalid;
    *aiSkippedRows = iRowsSkipped;
    API_LOC(330);
  }
  catch (Error* pError) { API_CATCH(pError); }
  catch (...) { Error* pError = nullptr; API_CATCH(pError); }
  
  API_RETURN();
}

/*----------------------------------------------------------------------------*/
/* get record buffer from the job                                             */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetRecordBuffer(i8 iTrace, Session* pSession, void* aJob,
                             void** aaRecordBuffer)
{
  API_JOB_LEVEL();
  API_ENTER(AC_GetRecordBuffer);
  API_NOTNULL(aaRecordBuffer);
  API_BEGIN();
  
  Job_SubInserter* pJob_SubInserter = _pJob_SubInserter(aJob);
  API_LOC(310);

  RASSERT(! pJob_SubInserter->m_pRecordBuffer);
  pJob_SubInserter->m_pRecordBuffer = new RecordBuffer();
  API_LOC(320);

  *aaRecordBuffer = pJob_SubInserter->m_pRecordBuffer;
  API_LOC(330);

  API_RETURN();
}

/*----------------------------------------------------------------------------*/
/* get row                                                                    */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetRow(i8 iTrace, Session* pSession, void* aJob,
                    i8 iRow, void** aaRow)
{
  API_JOB_LEVEL();
  API_ENTER(AC_GetRow);
  API_NOTNULL(aaRow);
  API_BEGIN();
  
  i8 iMaxRowNo = 0;
  
  eCN eClass = pJob->eClass();
  JobClass* pJobClass = JobClass::pFind(eClass);
  pJob->m_aJobClass = pJobClass;
  eRS eResponseKind = pJob->m_aJobClass->m_eResponse;
  Response* pResponse = nullptr;
  
  if (eResponseKind == RS_LOCAL_ROWS) {
    pResponse = pJob->pResponse();
  }
  else {
    RASSERT(eResponseKind == RS_NODE_ROWS);
  
    i8 iResponses = pJob->m_pResponseVec->nItems();
    
    for (i8 i_Node = 0; i_Node < iResponses; i_Node++) {
      pResponse = pJob->m_pResponseVec->pGet(i_Node);
      iMaxRowNo = *pResponse->m_ixRowsProcessed;
      if (iRow < iMaxRowNo)
        break;
      iRow -= iMaxRowNo;
    }
  }
  
  API_LOC(310);

  // tokenize the response
  TokenVec* pTknVec = Tokeniser::pTokenise(pResponse->m_sResponse);
  API_LOC(320);

  i8 iRows = 0;
  i8 iToken = 0;
  API_LOC(330);

  while (true) {
    RASSERT(pTknVec->pGet(iToken++)->szText()->bSame("row:"_Stz));
    
    RASSERT(pTknVec->pGet(iToken++)->szText()->bSame("("_Stz));
    
    if (iRows == iRow)
      break;
    
    while ( ! pTknVec->pGet(iToken)->szText()->bSame(")"_Stz)) {
      iToken++;
    }
  
    RASSERT(pTknVec->pGet(iToken++)->szText()->bSame(")"_Stz));
    iRows++;
  }
  API_LOC(340);

  pSession->m_sResponse.vCopy(""_Stz);
  while ( ! pTknVec->pGet(iToken)->szText()->bSame(")"_Stz)) {
    pSession->m_sResponse.vConcat(pTknVec->pGet(iToken++)->szText());
  }
  API_LOC(350);

  // return the row
  *aaRow = pSession->m_sResponse.azChar();
  API_LOC(360);

  // delete the token vector
  pTknVec->vSetAllTokens();
  delete pTknVec;
  API_LOC(370);

  API_RETURN();
}

/*----------------------------------------------------------------------------*/
/* get row count                                                              */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetRowCount(i8 iTrace, Session* pSession, void* aJob,
                         i8 *aiRowCount)
{
  API_JOB_LEVEL();
  API_ENTER(AC_GetRowCount);
  API_NOTNULL(aiRowCount);
  API_BEGIN();
  
  eCN eClass = pJob->eClass();
  JobClass* pJobClass = JobClass::pFind(eClass);
  pJob->m_aJobClass = pJobClass;
  eRS eResponseKind = pJob->m_aJobClass->m_eResponse;
  
  i8 iRowsProcessed= 0;
  
  if (eResponseKind == RS_LOCAL_ROWS) {
    iRowsProcessed = pJob->iRowsProcessed();
  }
  else {
    RASSERT(eResponseKind == RS_NODE_ROWS);
    
    i8 iResponses = pJob->m_pResponseVec->nItems();
    
    for (i8 i_Node = 0; i_Node < iResponses; i_Node++) {
      iRowsProcessed += *pJob->m_pResponseVec->pGet(i_Node)->m_ixRowsProcessed;
    }
  }
  
  API_LOC(310);

  *aiRowCount = iRowsProcessed;
  API_LOC(320);

  //vTrace("RowCount = %lld\n"_Stz, *aRowCount);
  
  API_RETURN();
}

/*----------------------------------------------------------------------------*/
/* get row table column                                                       */
/*----------------------------------------------------------------------------*/
//
// should this be: RapidsSE_GetColumnRecord ?
//
eEC RapidsSE_GetTableColumnRow(i8 iTrace, Session* pSession, void* aJob,
                               i8 i_Col, void** aaRow)
{
  API_JOB_LEVEL();
  API_ENTER(AC_GetTableColumnRow);
  API_NOTNULL(aaRow);
  API_BEGIN();
  
  Job_Universal* pJob_ListColumns = _pJob_Universal(aJob);
  API_LOC(310);
  
  //i8 iNode = AM::pGrid()->m_iThisNode;
  Table* pTable =
  (Table*)StrLib::iToI8(pJob_ListColumns->szResponse()->azChar());
  API_LOC(320);
  
  StreamDef* pStreamDef = pTable->pStreamDef();
  Column* pColumn = pStreamDef->pGetColumn(i_Col);
  API_LOC(330);
  
  sprintf(pSession->m_sResponse.azChar(),"%lld,%s,%s,%lld,%lld,%s,%s",
          pColumn->iColumnNo(),
          pColumn->szColNm()->azChar(),
          aCT[pColumn->eColumnType()]->azChar(),
          pColumn->iColumnLength(),
          pColumn->iColumnScale(),
          pColumn->bIsNullable() ? "true" : "false",
          pColumn->bIsPtnKey() ? "true": "false"
          );
  API_LOC(340);
  
  *aaRow = pSession->m_sResponse.azChar();
  API_LOC(350);
  
  API_RETURN();
}

/*----------------------------------------------------------------------------*/
/* get row count table column                                                 */
/*----------------------------------------------------------------------------*/
//
// should this be: RapidsSE_GetColumnCount ?
//
eEC RapidsSE_GetTableColumnRowCount(i8 iTrace, Session* pSession, void* aJob,
                                    i8 *aiRowCount)
{
  API_JOB_LEVEL();
  API_ENTER(AC_GetTableColumnRowCount);
  API_NOTNULL(aiRowCount);
  API_BEGIN();
  
  Job_Universal* pJob_ListColumns = _pJob_Universal(aJob);
  API_LOC(310);

  //i8 iNode = AM::pGrid()->m_iThisNode;
  Table* pTable =
    (Table*)StrLib::iToI8(pJob_ListColumns->szResponse()->azChar());
  API_LOC(320);

  StreamDef* pStreamDef = pTable->pStreamDef();
  API_LOC(330);

  pJob_ListColumns->vRowsProcessed(pStreamDef->nColumns(C_ALL));
  API_LOC(340);

  *aiRowCount = pJob_ListColumns->iRowsProcessed();
  API_LOC(350);
  
  API_RETURN();
}

/*----------------------------------------------------------------------------*/
/* fetch next row                                                             */
/*----------------------------------------------------------------------------*/
//
// THIS SHOULD GO
//
eEC RapidsSE_NextRow(i8 iTrace, Session* pSession, void* aTable, i8 iPartition,
                     i8* aiExpectedRows,
                     void** aaEntBuffer,
                     void** aaItem,
                     void** aaRow
                     )
{
  API_SESSION_LEVEL();
  API_ENTER(AC_NextRow);
  API_NOTNULL(aiExpectedRows);
  API_NOTNULL(aaEntBuffer);
  API_NOTNULL(aaItem);
  API_NOTNULL(aaRow);
  API_BEGIN();
  
  RASSERT(0);
  
  /*
  try {

    // get the table
    Table* pTable = _pTable(aTable);
    RASSERT(pTable->eClass() == CN_TABLE);
    API_LOC(320);
    
    // *piExpectedRows = (i8)(pJob_FetchRows->m_pBufferQueue->iCount() * C_ROWS_PER_BUFFER);
    
    Ent* aItem = nullptr;
    EntBuffer* pEntBuffer = nullptr;
    API_LOC(330);

    // do we have a buffer ?
    if (! *aaEntBuffer) {
      // get first entry buffer
      TablePtn* pPartition = pTable->pPartition(iPartition);
      Index* pIndex = pTable->pHiddenIndex();
      IndexTree* pIndexTree = pPartition->pIndexTree(pIndex->iIndexNo());
      pEntBuffer = pIndexTree->pFirstH0Buffer();
      //vTrace("Got first buffer %p\n"_Stz, pEntBuffer);
      if ( ! pEntBuffer) {
        *aaRow = nullptr;
        
      } else {
        aItem = pEntBuffer->aLeadEntry();
        *aaItem = aItem;
        RASSERT(aItem != nullptr);
        RASSERT(aItem->eClass() == CN_ENT);
        *aaEntBuffer = pEntBuffer;
        *aaRow = _aRowData(aItem->aDataArea());
        //vTrace("returning %p %p\n"_Stz, *aaEntBuffer, *aaItem);
      }
      
      API_RETURN();
    }
    API_LOC(340);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // we have a buffer
    pEntBuffer = *((EntBuffer**)(aaEntBuffer));
    RASSERT(pEntBuffer->eClass() == CN_ENTBUFFER);
    API_LOC(350);

    // we should have an entry
    aItem = *((Ent**)(aaItem));
    RASSERT(aItem != nullptr);
    //vTrace("got %p %p\n"_Stz, pEntBuffer, aItem);
    RASSERT(aItem->eClass() == CN_ENT);
    API_LOC(360);

    // get the next entry
    aItem = pEntBuffer->aNextEntry(aItem, 0);
    API_LOC(370);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // did we find an entry ?
    if ( ! aItem ) {
      //vTrace("didn't get next entry\n"_Stz);
      // get next buffer from the current indexBuffer
      pEntBuffer = pEntBuffer->pNextBuffer();
      
      if ( pEntBuffer ) {
        //vTrace("got next buffer %p\n"_Stz, pEntBuffer);
        aItem = pEntBuffer->aLeadEntry();

      }
    }
    API_LOC(380);

    if (aItem) {
      *aaEntBuffer = pEntBuffer;
      *aaItem = aItem;
      *aaRow = _aRowData(aItem->aDataArea());
      //vTrace("Returning rowEntry %p\n"_Stz, aItem);
    } else {
      *aaRow = nullptr;
    }
    API_LOC(390);
  }
  catch (Error* pError) { API_CATCH(pError); }
  catch (...) { Error* pError = nullptr; API_CATCH(pError); }
  */
  
  API_RETURN();
}

/*----------------------------------------------------------------------------*/
/* get first buffer                                                           */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetFirstBuffer(i8 iTrace, Session* pSession, void* aTable,
                            i8 iPartition, void** aaEntBuffer, i8* aiBufferNo, i8* aiRowCount
                           )
{
  API_SESSION_LEVEL();
  API_ENTER(AC_GetFirstBuffer);
  API_NOTNULL(aaEntBuffer);
  API_NOTNULL(aiBufferNo);
  API_NOTNULL(aiRowCount);
  API_BEGIN();
  
  RASSERT(0);
  
 /*
  try {
    
    // get the table
    Table* pTable = _pTable(aTable);
    RASSERT(pTable->eClass() == CN_TABLE);
    API_LOC(320);
    
    // get first entry buffer
    TablePtn* pPartition = pTable->pPartition(iPartition);
    Index* pIndex = pTable->pHiddenIndex();
    IndexTree* pIndexTree = pPartition->pIndexTree(pIndex->iIndexNo());
    API_LOC(330);
    
    EntBuffer* pEntBuffer = pIndexTree->pFirstH0Buffer();
    API_LOC(340);
    
    // return the first buffer
    *aaEntBuffer = pEntBuffer;
    
    // return no of rows
    if (pEntBuffer) {
      //vTrace("Returning buffer %p %lld %lld \n"_Stz, pEntBuffer, *pEntBuffer->nBufferNo(), pEntBuffer->nItems());
      *aiRowCount = pEntBuffer->nItems();
      *aiBufferNo = *pEntBuffer->nPoolBufferNo();
    }
    else {
      *aiRowCount = 0;
      *aiBufferNo = -1;
    }
    
    API_RETURN();
  }
  catch (Error* pError) { API_CATCH(pError); }
  catch (...) { Error* pError = nullptr; API_CATCH(pError); }
  */
  
  API_RETURN();
}

/*----------------------------------------------------------------------------*/
/* get next buffer                                                            */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetNextBuffer(i8 iTrace, Session* pSession, void** aaEntBuffer, i8* aiBufferNo, i8* aiRowCount)
{
  API_SESSION_LEVEL();
  API_ENTER(AC_GetNextBuffer);
  API_NOTNULL(aaEntBuffer);
  API_NOTNULL(aiBufferNo);
  API_NOTNULL(aiRowCount);
  API_BEGIN();
  
  try {
    
    //vTrace("last buffer %p\n"_Stz, *aaEntBuffer);
    EntBuffer* pEntBuffer = _pEntBuffer(*aaEntBuffer);
    API_LOC(320);
    
    EntBuffer* pNextBuffer = pEntBuffer->pNextBuffer();
    API_LOC(330);
    
    // return the next buffer back
    *aaEntBuffer = pNextBuffer;
    
    // return no of rows
    // return no of rows
    if (pNextBuffer) {
      //vTrace("Returning buffer %p %lld %lld \n"_Stz, pNextBuffer, *pNextBuffer->nBufferNo(), pNextBuffer->nItems());
      *aiRowCount = pNextBuffer->nItems();
      *aiBufferNo = *pNextBuffer->nPoolBufferNo();
    }
    else {
      *aiRowCount = 0;
      *aiBufferNo = -1;
    }
    
    API_RETURN();
  }
  catch (Error* pError) { API_CATCH(pError); }
  catch (...) { Error* pError = nullptr; API_CATCH(pError); }
  
  API_RETURN();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
