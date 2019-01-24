/*============================================================================*/
/* Api.cpp - RapidsSE Api funcstions                                          */
/*============================================================================*/

#include "Interface.h"
#include "StorageEngine.h"

extern BufferPool*  gpBufferPool;
extern AttachMap*   gpAttachMap;
extern ActivityMap* gpActivityMap;


////////////////////////////////////////////////////////////////////////////////
//                             System Level Calls                             //
////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* check the library version against given version                            */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_CheckVersion(i8 iTrace, Stz* sVersion)
{
  API_SYSTEM_LEVEL();
  API_ENTRY(AC_CheckVersion);
  API_PARAM_P1(sVersion);
  API_BEGIN();
  
  try {
    
    if ( ! sVersion->bEqual((Stz*)RAPIDSSE_VERSION_STRING)) {
      
      vTrace(">RapidsSE_CheckLibraryVersion: RDP (%s) does not match RapidsSE (%s)"_Stz,
             sVersion, RAPIDSSE_VERSION_STRING);
      
      API_RETURN_EC(EC_VERSION_MISMATCH);
    }
  }
  catch (Error* pError) { API_CATCH(pError); }
  catch (...) { Error* pError = nullptr; API_CATCH(pError); }
  
  API_RETURN();
}

/*----------------------------------------------------------------------------*/
/* delete an se session                                                       */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_DeleteSession(i8 iTrace, Session** ppSession)
{
  Session* pSession = *ppSession;
  
  API_SESSION_LEVEL();
  API_ENTRY(AC_DeleteSession);
  API_BEGIN();
  
  try {
    
    // delete jobs from the job stack of the session
    vDeleteJobs(pSession);
    API_LOC(310);
    
    // delete the session
    RapidsSE::vDeleteSession(pSession);
    API_LOC(320);
    
    // set session to nullptr so macro knows not to look at it
    pSession = nullptr;
    
    // retrun back null session
    *ppSession = nullptr;
    
    API_LOC(330);
  }
  catch (Error* pError) { API_CATCH(pError); }
  catch (...) { Error* pError = nullptr; API_CATCH(pError); }
  
  API_RETURN();
}

/*----------------------------------------------------------------------------*/
/* is the error an EC_INVALID_INTERFACE ?                                     */
/*----------------------------------------------------------------------------*/
//
// can this be: RapidsSE_IsInvalidInterface
//
bool RapidsSE_IsInvalidSession(i8 iTrace, int iError)
{
  API_SYSTEM_LEVEL();
  API_ENTRY(AC_IsInvalidSession);
  API_BEGIN();
  
  API_EXITING();
  if (iError == EC_INVALID_INTERFACE)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* create a new se session                                                    */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_NewSession(i8 iTrace,
                        Stz* sCatalog, Stz* sSchema, i8 iPartition,
                        void** aaSession)
{
  API_SYSTEM_LEVEL();
  API_ENTRY(AC_NewSession);
  API_PARAM_P2(sCatalog, sSchema);
  API_NOTNULL(aaSession);
  API_BEGIN();
  
  try {
    
    VALIDATE_POOL();
    
    pSession = new Session(iPartition);
    
    eEC eError = pSession->eInit(sCatalog,sSchema);
    if (eError != EC_NONE)
      API_RETURN_EC(eError);
    
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
eEC RapidsSE_OpenPool(i8 iTrace, Stz* sRegion, void** aaAttachMap)
{
  API_SYSTEM_LEVEL();
  API_ENTRY(AC_OpenPool);
  API_PARAM_P1(sRegion);
  API_BEGIN();
  
  try {
    
    // open the pool
    s32 sPoolName;
    sPoolName = (Stz*)sRegion;
    
    bool bResult = RapidsSE::bOpenPool(sPoolName);
    if (! bResult)
      API_RETURN_EC(EC_INVALID_INSTANCE);
    
    VALIDATE_POOL();
    
    *aaAttachMap = gpAttachMap;
    
    ActivityMap::vOpenByApi();
    
    if (gpAttachMap)
      gpAttachMap->m_oStatistics.Api.m_nPoolOpen++;
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
eEC RapidsSE_BeginInsert(i8 iTrace, Session* pSession,
                         Stz* sTable, i8 nSubInserters, void** aaJob)
{
  API_SESSION_LEVEL();
  API_ENTRY(AC_BeginInsert);
  API_NOTNULL(aaJob);
  API_BEGIN();
  
  try {
 
    // create Job_InsertTable
    JobClass* pJobClass_InsertTable = JobClass::pFind(CN_JOB_INSERTTABLE);
    ASSERT(pJobClass_InsertTable);
    API_LOC(310);
    
    Job_InsertTable* pJob_InsertTable =
      static_cast<Job_InsertTable*>(pJobClass_InsertTable->pPreJob(pSession));
    API_LOC(320);

    // find the table in the session dictionary stack
    Table* pTable = Table::pFindTable(pSession->pPathStack(), sTable);
    API_LOC(330);

    // set the table in the InsertTable job
    pJob_InsertTable->m_pTable = pTable;
    API_LOC(340);

    // return the insertTable job
    *aaJob = pJob_InsertTable;
    API_LOC(350);

    // set subInserters in the TblPtb
    gpAttachMap->m_nSubInserters = nSubInserters;
    API_LOC(360);

    // push the InsertTable job onto the jobstack
    pSession->m_pJobStack->vPushPointer(pJob_InsertTable);
    API_LOC(370);

    vApiJob(pSession,pJob_InsertTable);
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
  API_ENTRY(AC_EndJobs);
  API_BEGIN();
  
  // delete the jobs from the sessions jobStack
  vDeleteJobs(pSession);
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
                            Stz* sCatalog, Stz* sSchema, Stz* sCommand,
                            void** aaJob)
{
  API_SESSION_LEVEL();
  API_ENTRY(AC_ExecuteCommand);
  API_PARAM_P1(sCommand);
  API_BEGIN();
  
  try {
    
    // set the catalog and schema in the session
    eEC eError = pSession->eInit(sCatalog, sSchema);
    if (eError != EC_NONE)
      API_RETURN_EC(eError);
    API_LOC(310);

    // execute the command
    Job* pJob = pAPI_ExecCmdStr(sCommand, pSession, false, false);
    API_LOC(320);

    vApiJob(pSession,pJob);
    API_LOC(330);

    // check if job had an error
    if (pJob->m_bHasError) {
      for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
        eEC eError = pJob->m_xNode[i_Node].m_eErrorCode;
        if (eError != EC_NONE) {
          pSession->m_pJobStack->vPushPointer(pJob);
          API_RETURN_EC(eError);
        }
      }
      NEVER_REACH;
    }
    API_LOC(340);

    // return job in aaJob
    *aaJob = pJob;
    API_LOC(350);

    // push the job on the sessions job stack
    pSession->m_pJobStack->vPushPointer(pJob);
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
                       Stz* sTableName, Stz* sRecord)
{
  API_SESSION_LEVEL();
  API_ENTRY(AC_InsertRow);
  API_NOTNULL(sRecord);
  API_BEGIN();
  
  try {
    
    // create Job_InsertTable
    JobClass* pJobClass_InsertRow = JobClass::pFind(CN_JOB_INSERTROW);
    ASSERT(pJobClass_InsertRow);
    API_LOC(310);
    
    Job_InsertRow* pJob_InsertRow =
    static_cast<Job_InsertRow*>(pJobClass_InsertRow->pPreJob(pSession));
    API_LOC(320);
    
    // push the InsertTable job onto the jobstack
    pSession->m_pJobStack->vPushPointer(pJob_InsertRow);
    API_LOC(330);
    
    vApiJob(pSession,pJob_InsertRow);
    API_LOC(340);
    
    // get the length of the record
    i8 iRawDataLength = StrLib::iSize(sRecord->aChar());
    API_LOC(350);
    
    // check length of record
    if (iRawDataLength > C_MAX_RECORD)
      API_RETURN_EC(EC_MAX_ROW_LENGTH);
    API_LOC(360);
    
    // set the row in the insertRow job
    pJob_InsertRow->m_sObjectName = sTableName;
    pJob_InsertRow->m_aRawData = sRecord;
    pJob_InsertRow->m_nRawDataBytes = iRawDataLength;
    API_LOC(370);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // spawn and wait for the job
    vSpawnJobAndWait(pSession, pJob_InsertRow);
    API_LOC(380);
    
    // check if job had an error
    if (pJob_InsertRow->m_bHasError) {
      
      for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
        
        eEC eError = pJob_InsertRow->m_xNode[i_Node].m_eErrorCode;
        if (eError != EC_NONE)
          API_RETURN_EC(eError);
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
  API_ENTRY(AC_TranslateError);
  API_NOTNULL(aaError);
  API_BEGIN();
  
  try {
    
    // get the error message
    pSession->m_sResponse = aEC[(eEC)iErrorCode];
    API_LOC(310);

    *aaError = pSession->m_sResponse.aChar();
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
  API_ENTRY(AC_BeginSubInserter);
  API_NOTNULL(aJob);
  API_NOTNULL(aaJob);
  API_BEGIN();
  
  try {
    
    // get the insertTable job
    Job_InsertTable* pJob_InsertTable = _pJob_InsertTable(aJob);
    API_LOC(310);

    // create Job_SubInserter
    JobClass* pJobClass_SubInserter = JobClass::pFind(CN_JOB_SUBINSERTER);
    ASSERT(pJobClass_SubInserter);
    API_LOC(320);

    Job_SubInserter* pJob_SubInserter =
    static_cast<Job_SubInserter*>(pJobClass_SubInserter->pPreJob(pSession));
    API_LOC(330);

    // push the InsertTable job onto the jobstack
    pSession->m_pJobStack->vPushPointer(pJob_SubInserter);
    vApiJob(pSession,pJob_SubInserter);
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
  API_ENTRY(AC_CopyBuffer);
  API_BEGIN();
  
  try {
    
    // get the insertRow job
    Job_SubInserter* pJob_SubInserter = _pJob_SubInserter(aJob);
    API_LOC(310);

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
    static_cast<Job_CopyBuffer*>(pJobClass_CopyBuffer->pPreJob(pSession));
    API_LOC(350);

    // set the pJob_SubInserter in the pJob_CopyBuffer
    pJob_CopyBuffer->m_pJob_SubInserter = pJob_SubInserter;
    API_LOC(360);

    // move the recordBuffer from the subInsert into CopyBuffer
    pJob_CopyBuffer->m_pRecordBuffer = pRecordBuffer;
    API_LOC(370);
    
    ++pJob_SubInserter->m_ziRecordBuffers;
    pJob_SubInserter->m_pRecordBuffer = nullptr;
    API_LOC(380);

    // push the InsertTable job onto the jobstack
    //pSession->m_pJobStack->vPushPointer(pJob_CopyBuffer);
    //vApiJob(pSession,pJob_CopyBuffer);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // spawn the job
    //vTrace("Spawning copyBuffer job\n"_Stz);
    bAPI_SpawnJob(pJob_CopyBuffer, SS_SEED_REQ, pSession);
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
  API_ENTRY(AC_EndInsert);
  API_BEGIN();
  
  try {
    
    // get the insertTable job
    Job_InsertTable* pJob_InsertTable = _pJob_InsertTable(aJob);
    API_LOC(310);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // spawn and wait for job
    vSpawnJobAndWait(pSession, pJob_InsertTable);
    API_LOC(320);

    // check if job had an error
    if (pJob_InsertTable->m_bHasError) {
      
      for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
        eEC eError = pJob_InsertTable->m_xNode[i_Node].m_eErrorCode;
        if (eError != EC_NONE)
          API_RETURN_EC(eError);
      }
      NEVER_REACH;
    }
    API_LOC(330);

    // delete all jobs in the ssession
    vDeleteJobs(pSession);
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
  API_ENTRY(AC_EndSubInserter);
  API_BEGIN();
  
  try {
    
    // get the insertRows job
    Job_SubInserter* pJob_SubInserter = _pJob_SubInserter(aJob);
    API_LOC(310);

    // wait for all record buffers to be copied across
    while (*pJob_SubInserter->m_ziRecordBuffers !=
           *pJob_SubInserter->m_ziCopiedBuffers) {
      //vTrace("Waiting %lld %lld\n"_Stz, *pJob_SubInserter->m_ziRecordBuffers, *pJob_SubInserter->m_ziCopiedBuffers);
    }
    API_LOC(320);

    pJob_SubInserter->vEnd();
    API_LOC(330);

    // delete all jobs in the ssession
    vDeleteJobs(pSession);
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
// this will go
//
eEC RapidsSE_FetchNextBuffer(i8 iTrace, Session* pSession, void* aJob,
                             void** aaFetchBuffer, i8* aiExpectedRows)
{
  API_JOB_LEVEL();
  API_ENTRY(AC_FetchNextBuffer);
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
    
//    // pop an entryBuffer off the queue
//    GenVar xVariable;
//    while (!pJob_FetchRows->m_pBufferQueue->bPop(&xVariable)) {
//      //vTrace("SLEEPING\n"_Stz);
//      Pthread::vSleepMilli(1);
//    }
//
//    //vTrace("Buffers remaining %lld\n"_Stz, pJob_FetchRows->m_pBufferQueue->iCount());
//
//    *piExpectedRows = (i8)(pJob_FetchRows->m_pBufferQueue->iCount() * C_ROWS_PER_BUFFER);
//
//    // get entry buffer
//    PtrVal* aPtrVal = xVariable.aNestedPtrVal();
//    pJob_FetchRows->m_pFetchBuffer = (FetchBuffer*)(aPtrVal->pObj());
//    //vTrace("Expected rows per buffer %lld %lld\n"_Stz, rowsPerBuffer, pJob_FetchRows->m_pFetchBuffer->nFetchedRows());
//    //vTrace("Got buffer %p %lld\n"_Stz, pJob_FetchRows->m_pFetchBuffer, pJob_FetchRows->m_iPartition);
    
    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

    FetchBuffer* pFetchBuffer;
    for (;;) {
      pFetchBuffer = _pFetchBuffer(pJob_FetchRows->m_pBufferQueue->pPop());
      if (pFetchBuffer)
        break;
      //vTrace("SLEEPING\n"_Stz);
      Pthread::vSleepMilli(1,CN_JOB_FETCHROWS);
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
/* get load result                                                            */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetLoadResult(i8 iTrace, Session* pSession, void* aJob,
                           i8* aiValidRows, i8* aiInvalidRows, i8* aiSkippedRows)
{
  API_JOB_LEVEL();
  API_ENTRY(AC_GetLoadResult);
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
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      
      //vTrace("%s"_Stz, pJob_LoadTable->m_xNode[i_Node].m_sResponse.aChar());
      iRowsProcessed += *pJob_LoadTable->m_xNode[i_Node].m_ziRowsProcessed;
      iRowsInvalid   += *pJob_LoadTable->m_xNode[i_Node].m_ziRowsInvalid;
      iRowsSkipped   += *pJob_LoadTable->m_xNode[i_Node].m_ziRowsSkipped;
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
  API_ENTRY(AC_GetRecordBuffer);
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
  API_ENTRY(AC_GetRow);
  API_NOTNULL(aaRow);
  API_BEGIN();
  
  i8 i_Node;
  i8 iMaxRowNo = 0;
  for (i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
    iMaxRowNo = *pJob->m_xNode[i_Node].m_ziRowsProcessed;
    if (iRow < iMaxRowNo)
      break;
    iRow -= iMaxRowNo;
  }
  API_LOC(310);

  // tokenize the response
  TokenVec* pTknVec = Tokeniser::pTokenise(pJob->m_xNode[i_Node].m_sResponse);
  API_LOC(320);

  i8 iRows = 0;
  i8 iToken = 0;
  API_LOC(330);

  while (true) {
    RASSERT(pTknVec->pGet(iToken++)->aText()->bSame("row:"_Stz));
    
    RASSERT(pTknVec->pGet(iToken++)->aText()->bSame("("_Stz));
    
    if (iRows == iRow)
      break;
    
    while ( ! pTknVec->pGet(iToken)->aText()->bSame(")"_Stz)) {
      iToken++;
    }
  
    RASSERT(pTknVec->pGet(iToken++)->aText()->bSame(")"_Stz));
    iRows++;
  }
  API_LOC(340);

  pSession->m_sResponse.vCopy(""_Stz);
  while ( ! pTknVec->pGet(iToken)->aText()->bSame(")"_Stz)) {
    pSession->m_sResponse.vConcat(pTknVec->pGet(iToken++)->aText());
  }
  API_LOC(350);

  // return the row
  *aaRow = pSession->m_sResponse.aChar();
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
  API_ENTRY(AC_GetRowCount);
  API_NOTNULL(aiRowCount);
  API_BEGIN();
  
  i8 iRowsProcessed= 0;
  for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++)
    iRowsProcessed += *pJob->m_xNode[i_Node].m_ziRowsProcessed;
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
  API_ENTRY(AC_GetTableColumnRow);
  API_NOTNULL(aaRow);
  API_BEGIN();
  
  Job_ListColumns* pJob_ListColumns = _pJob_ListColumns(aJob);
  API_LOC(310);
  
  i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
  Table* pTable =
  (Table*)StrLib::iToI8(pJob_ListColumns->m_xNode[iNode].m_sResponse.aChar());
  API_LOC(320);
  
  ColumnVec* pColumnVec = pTable->pColumnVec();
  Column* pColumn = pColumnVec->pGet(i_Col);
  API_LOC(330);
  
  sprintf(pSession->m_sResponse.aChar(),"%lld,%s,%s,%lld,%lld,%s,%s",
          pColumn->iColumnNo(),
          pColumn->sColumnName().aChar(),
          aCT[pColumn->eColumnType()]->aChar(),
          pColumn->iColumnLength(),
          pColumn->iColumnScale(),
          pColumn->bIsNullable() ? "true" : "false",
          pColumn->bIsPartitionKey() ? "true": "false"
          );
  API_LOC(340);
  
  *aaRow = pSession->m_sResponse.aChar();
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
  API_ENTRY(AC_GetTableColumnRowCount);
  API_NOTNULL(aiRowCount);
  API_BEGIN();
  
  Job_ListColumns* pJob_ListColumns = _pJob_ListColumns(aJob);
  API_LOC(310);

  i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
  Table* pTable =
    (Table*)StrLib::iToI8(pJob_ListColumns->m_xNode[iNode].m_sResponse.aChar());
  API_LOC(320);

  ColumnVec* pColumnVec = pTable->pColumnVec();
  API_LOC(330);

  pJob_ListColumns->m_xNode[iNode].m_ziRowsProcessed = pColumnVec->nItems() - 1;
  API_LOC(340);

  *aiRowCount = *(pJob_ListColumns->m_xNode[iNode].m_ziRowsProcessed);
  API_LOC(350);

  //vTrace("RowCount = %lld\n"_Stz, pJob->m_xNode[gpAttachMap->m_pGrid->iThisNode()].m_iRowsProcessed);
  
  API_RETURN();
}

/*----------------------------------------------------------------------------*/
/* fetch next buffer                                                          */
/*----------------------------------------------------------------------------*/
//
// can this be: RapidsSE_NextRow
//
eEC RapidsSE_ScanNextRow(i8 iTrace, Session* pSession, void* aJob,
                         void** aaEntryBuffer, i8* aiExpectedRows,
                         void** aaEntry, void** aaRow
                         )
{
  API_JOB_LEVEL();
  API_ENTRY(AC_ScanNextRow);
  API_NOTNULL(aaRow);
  API_BEGIN();
  
  try {
    
    Job_SelectTable* pJob_SelectTable = _pJob_SelectTable(aJob);
    API_LOC(310);

    // get the table
    Table* pTable = pJob_SelectTable->m_pTable;
    API_LOC(320);

    //*piExpectedRows = (i8)(pJob_FetchRows->m_pBufferQueue->iCount() * C_ROWS_PER_BUFFER);
    
    Entry* aEntry = nullptr;
    EntryBuffer* pEntryBuffer = nullptr;
    API_LOC(330);

    // do we have a buffer ?
    if (! *aaEntryBuffer) {
      // get first entry buffer
      TablePtn* pPartition = pTable->pPartition(pJob_SelectTable->m_iPartition);
      Index* pIndex = pTable->pHiddenIndex();
      IndexTree* pIndexTree = pPartition->pIndexTree(pIndex->iIndexNo());
      pEntryBuffer = pIndexTree->pFirstL0Buffer();
      vTrace("Got first buffer %p\n"_Stz, pEntryBuffer);
      if ( ! pEntryBuffer) {
        *aaRow = nullptr;
        
      } else {
        aEntry = pEntryBuffer->aFirstEntry();
        *aaEntry = aEntry;
        *aaEntryBuffer = pEntryBuffer;
        *aaRow = aEntry->aRow();
      }
      
      API_RETURN();
    }
    API_LOC(340);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // we have a buffer
    pEntryBuffer = *((EntryBuffer**)(aaEntryBuffer));
    API_LOC(350);

    // we should have an entry
    aEntry = *((Entry**)(aaEntry));
    RASSERT(aEntry);
    API_LOC(360);

    // get the next entry
    aEntry = pEntryBuffer->aNextEntry(aEntry, 0);
    API_LOC(370);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // did we find an entry ?
    if ( ! aEntry ) {
      vTrace("didn't get next entry\n"_Stz);
      // get next buffer from the current indexBuffer
      pEntryBuffer = pEntryBuffer->pNextBuffer();
      
      if ( pEntryBuffer ) {
        vTrace("got next buffer %p\n"_Stz, pEntryBuffer);
        aEntry = pEntryBuffer->aFirstEntry();

      }
    }
    API_LOC(380);

    if (aEntry) {
      *aaEntryBuffer = pEntryBuffer;
      *aaEntry = aEntry;
      *aaRow = aEntry->aRow();
      //vTrace("Returning rowEntry %p\n"_Stz, aEntry);
    } else {
      *aaRow = nullptr;
    }
    API_LOC(390);
  }
  catch (Error* pError) { API_CATCH(pError); }
  catch (...) { Error* pError = nullptr; API_CATCH(pError); }
  
  API_RETURN();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* execute command                                                            */
/*----------------------------------------------------------------------------*/
Job* pAPI_ExecCmdStr(Stz* sCommand, Session* pSession,
                          bool bPostJob, bool bFromPrompt)
{
  gpAttachMap->m_oStatistics.Api.m_nExecCmdStr++;
  
  CommandProcessor* pCommandProcessor = pSession->m_pCommandProcessor;
  
  vApiLocation(pSession,101);
  
  pCommandProcessor->vTokenise(sCommand);
  
  bool bFound = pCommandProcessor->bFindJobClass();
  if (! bFound) {
    return nullptr;
  }
  
  pCommandProcessor->vPreJob(pSession);
  
  Job* pJob = pCommandProcessor->pJob();
  if (!pJob) {
    return nullptr;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // spawn and wait for job
  vSpawnJobAndWait(pSession, pJob);
  
  // run post ?
  if (bPostJob)
    pCommandProcessor->vPostJob(pSession);
  
  return pJob;
}

/*----------------------------------------------------------------------------*/
/* spawn job                                                                  */
/*----------------------------------------------------------------------------*/
bool bAPI_SpawnJob(Job* pJob, eSS eSeed, Session* pSession)
{
  //----------------------------------------------------------------------------
  // Before appending request for se, ensure that se is running
  // get the se pid from the attachmap
  i8 iPid = gpAttachMap->m_iPid;
  if (!Process::bTestPid(iPid)) {
    // SE is not running anymore!
    if (pSession)
      vWarn("Cannot find pid %lld\n"_Stz, gpAttachMap->m_iPid);
    vWarn("Cannot find pid %lld\n"_Stz, gpAttachMap->m_iPid);
    return false;
  }
  
  //----------------------------------------------------------------------------
  
  Task* pNewTask = new Task(pJob);
  
  // form the word to execute for the request
  pJob->vSeed(eSeed);
  
  pNewTask->vSession(pSession);
  
  Handlers::vSpawnedTask(pNewTask);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* mark call of an api                                                        */
/*----------------------------------------------------------------------------*/
void vApiCall(Session* pSession, eAC eApiCall)
{
  gpAttachMap->m_pApiState->vApiCall(pSession,eApiCall);
}

/*----------------------------------------------------------------------------*/
/* mark error of an api                                                       */
/*----------------------------------------------------------------------------*/
void vApiError(Session* pSession, eEC eError)
{
  gpAttachMap->m_pApiState->vApiError(pSession,eError);
}

/*----------------------------------------------------------------------------*/
/* mark job of an api                                                         */
/*----------------------------------------------------------------------------*/
void vApiJob(Session* pSession, Job* pJob)
{
  gpAttachMap->m_pApiState->vApiJob(pSession,pJob);
}

/*----------------------------------------------------------------------------*/
/* mark location of an api                                                    */
/*----------------------------------------------------------------------------*/
void vApiLocation(Session* pSession, i8 iLocation)
{
  gpAttachMap->m_pApiState->vSetLocation(pSession,iLocation);
}

/*----------------------------------------------------------------------------*/
/* mark return of an api                                                      */
/*----------------------------------------------------------------------------*/
void vApiReturn(Session* pSession)
{
  gpAttachMap->m_pApiState->vApiReturn(pSession);
}

/*----------------------------------------------------------------------------*/
/* delete jobs                                                                */
/*----------------------------------------------------------------------------*/
void vDeleteJobs(Session* pSession)
{
  Stack* pJobStack = pSession->m_pJobStack;
  Object* pJobObject;

  while (pJobStack->bPop(pJobObject)) {
    
    Job* aJob = (Job*)pJobObject;
    
    // find the job class on this node
    eCN eClass = aJob->eClass();
    JobClass* pJobClass = JobClass::pFind(eClass);
    
    vApiLocation(pSession,200);
    
    //vTrace("API deleting job %p jobId %lld %s\n"_Stz, aJob, aJob->m_iJobId, aCN[eClass]);
    
    // If we come across a JOB_FETCHROWS then we need to empty its queues
    if (eClass == CN_JOB_FETCHROWS) {
      Job_FetchRows* pJob_FetchRows = _pJob_FetchRows(aJob);
  
      //vTrace("%s m_nLastBuffersScanned: %lld %lld\n"_Stz,pJob_FetchRows->m_pTable->sTable(),
      //       pJob_FetchRows->m_nLastBuffersScanned, pJob_FetchRows->m_nLastBuffers );
      
      // if we had an entry buffer delete it
      pJob_FetchRows->vDeleteBuffer();
      
      while (pJob_FetchRows->m_nLastBuffersScanned != pJob_FetchRows->m_nLastBuffers) {
        // pop an entryBuffer off the queue
        vApiLocation(pSession,300);
        
        // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
//        GenVar xVariable;
//        while (!pJob_FetchRows->m_pBufferQueue->bPop(&xVariable)) {
//          Pthread::vSleepMilli(1);
//          vApiLocation(pSession,400);
//          continue;
//        }
        
//        vApiLocation(pSession,500);
//
//        PtrVal* aPtrVal = xVariable.aNestedPtrVal();
//        pJob_FetchRows->m_pFetchBuffer = (FetchBuffer*)(aPtrVal->pObj());
        // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
        FetchBuffer* pFetchBuffer;
        for (;;) {
          pFetchBuffer = _pFetchBuffer(pJob_FetchRows->m_pBufferQueue->pPop());
          if (pFetchBuffer)
            break;
          Pthread::vSleepMilli(1,CN_JOB_FETCHROWS);
          vApiLocation(pSession,400);
          continue;
        }
        vApiLocation(pSession,500);

        pJob_FetchRows->m_pFetchBuffer = pFetchBuffer;
        // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

        
        if (pJob_FetchRows->m_pFetchBuffer->bIsLast()) {
          pJob_FetchRows->m_nLastBuffersScanned++;
          //vTrace("%s m_nLastBuffersScanned: %lld %lld\n"_Stz, pJob_FetchRows->m_pTable->sTable(),
          //       pJob_FetchRows->m_nLastBuffersScanned, pJob_FetchRows->m_nLastBuffers );
        }
        
        // delete the buffer
        pJob_FetchRows->vDeleteBuffer();
      }
    }
    vApiLocation(pSession,600);
    
    // delete the job
    //vTrace("Deleting job %p %s\n"_Stz, aJob, aCN[aJob->eClass()]);
    pJobClass->vDeleteJob(pSession,aJob);
    
    pSession->m_nJobDelete++;
  }
}

/*----------------------------------------------------------------------------*/
/* Spawn amd wait for job to complete                                         */
/*----------------------------------------------------------------------------*/
void vSpawnJobAndWait(Session* pSession, Job* pJob)
{
  i8 iJobSeq = pJob->m_iJobSeq;
  
  // lock this job's mutex
  pJob->vMutexLock();
//  i8 rCode = pthread_mutex_lock(&pJob->m_xThreadMutex);
//  if (0 != rCode)
//    vWarn("pthread_mutex_lock() error; rCode=%lld\n"_Stz,rCode);
  
  vApiLocation(pSession,106);
  
  // set the job state to be locked
  pJob->m_eState = JS_LOCKED;
  
  // spawn the insertTable job
  bAPI_SpawnJob(pJob, SS_SEED_REQ, pSession);
  vApiLocation(pSession,107);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // wait for condition & state
  
  // set the job state to be waiting
  pJob->m_eState = JS_WAITING;
  
  // wait till signalled
  do {
    
    RASSERT( _bIsValidJob(pJob->eClass()) );
    RASSERT( iJobSeq == pJob->m_iJobSeq );
    
    //    vTrace("ThreadCond:\n"_Stz);
    //    MemLib::vHexDump(&pJob->m_xThreadCond,  sizeof(pthread_cond_t));
    //
    //    vTrace("ThreadMutex:\n"_Stz);
    //    MemLib::vHexDump(&pJob->m_xThreadMutex, sizeof(pthread_mutex_t));
    
    pJob->vMutexWait();
//    rCode = pthread_cond_wait(&pJob->m_xThreadCond,
//                              &pJob->m_xThreadMutex);
//    if (0 != rCode)
//      vWarn("pthread_cond_wait() error; rCode=%lld\n"_Stz,rCode);

    RASSERT( _bIsValidJob(pJob->eClass()) );
    RASSERT( iJobSeq == pJob->m_iJobSeq );
    
    
  } while (pJob->m_eState != JS_SIGNALLED);
  
  RASSERT( _bIsValidJob(pJob->eClass()) );
  RASSERT( iJobSeq == pJob->m_iJobSeq );
  
  vApiLocation(pSession,108);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // set the job state back to running
  pJob->m_eState = JS_RUNNING;
  
  // unlock this job's mutex
  pJob->vMutexUnlock();
//  rCode = pthread_mutex_unlock(&pJob->m_xThreadMutex);
//  if (0 != rCode)
//    vWarn("pthread_mutex_unlock() error; rCode=%lld\n"_Stz,rCode);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* monitor portrait                                                           */
/*----------------------------------------------------------------------------*/
void vApiStatistics(eAC eApiCall)
{
  if (! gpAttachMap)
    return;
  
  switch (eApiCall) {
    case AC_BeginInsert:         gpAttachMap->m_oStatistics.Api.m_nBeginInsertTable++;     break;
    case AC_BeginSubInserter:    gpAttachMap->m_oStatistics.Api.m_nBeginSubInserter++;     break;
    case AC_CheckVersion:        gpAttachMap->m_oStatistics.Api.m_nCheckLibraryVersion++;  break;
    case AC_CopyBuffer:          gpAttachMap->m_oStatistics.Api.m_nCopyBuffer++;           break;
    case AC_EndInsert:           gpAttachMap->m_oStatistics.Api.m_nEndInsertJobs++;        break;
    case AC_EndJobs:             gpAttachMap->m_oStatistics.Api.m_nEndJobs++;              break;
    case AC_EndSubInserter:      gpAttachMap->m_oStatistics.Api.m_nEndSubInserter++;       break;
    //case AC_ExecCmdStr:          gpAttachMap->m_oStatistics.Api.m_nExecCmdStr++;           break;
    case AC_ExecuteCommand:      gpAttachMap->m_oStatistics.Api.m_nExecuteCommand++;       break;
    case AC_FetchNextBuffer:     gpAttachMap->m_oStatistics.Api.m_nFetchNextBuffer++;      break;
    case AC_GetRecordBuffer:     gpAttachMap->m_oStatistics.Api.m_nGetRecordBuffer++;      break;
    case AC_TranslateError:      gpAttachMap->m_oStatistics.Api.m_nGetErrorMessage++;      break;
    case AC_GetRow:              gpAttachMap->m_oStatistics.Api.m_nGetRow++;               break;
    case AC_GetRowCount:         gpAttachMap->m_oStatistics.Api.m_nGetRowCount++;          break;
    case AC_GetTableColumnRowCount: gpAttachMap->m_oStatistics.Api.m_nGetRowCountTableColumn++;  break;
    case AC_GetTableColumnRow:   gpAttachMap->m_oStatistics.Api.m_nGetRowTableColumn++;    break;
    case AC_InsertRow:           gpAttachMap->m_oStatistics.Api.m_nInsertRow++;            break;
    case AC_GetLoadResult:       gpAttachMap->m_oStatistics.Api.m_nLoadResult++;           break;
    case AC_OpenPool:            gpAttachMap->m_oStatistics.Api.m_nPoolOpen++;             break;
    case AC_ScanNextRow:         gpAttachMap->m_oStatistics.Api.m_nScanNextRow++;          break;
    case AC_DeleteSession:       gpAttachMap->m_oStatistics.Api.m_nSessionDelete++;        break;
    case AC_IsInvalidSession:    gpAttachMap->m_oStatistics.Api.m_nSessionInvalid++;       break;
    case AC_NewSession:          gpAttachMap->m_oStatistics.Api.m_nSessionNew++;           break;
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* monitor portrait                                                           */
/*----------------------------------------------------------------------------*/
void vAPI_MonitorPortrait(s48K& sPortrait)
{
  s48K s;
  i8 nCats = 0;
  i8 nBreak = 80;
  i8 iPrev = 0;
  Statistics* pStats = &gpAttachMap->m_oStatistics;
  
  if (pStats->Api.m_nBeginInsertTable) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("beginInsertTable %lld "_Stz,pStats->Api.m_nBeginInsertTable);
  }
  if (pStats->Api.m_nBeginSubInserter) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("beginSubInserter %lld "_Stz,pStats->Api.m_nBeginSubInserter);
  }
  if (pStats->Api.m_nCheckLibraryVersion) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("checkVersion %lld "_Stz,pStats->Api.m_nCheckLibraryVersion);
  }
  if (pStats->Api.m_nCopyBuffer) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("copyBuffer %lld "_Stz,pStats->Api.m_nCopyBuffer);
  }
  if (pStats->Api.m_nEndInsertJobs) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("endInsertJobs %lld "_Stz,pStats->Api.m_nEndInsertJobs);
  }
  if (pStats->Api.m_nEndJobs) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("endJobs %lld "_Stz,pStats->Api.m_nEndJobs);
  }
  if (pStats->Api.m_nExecCmdStr) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("execCmdStr %lld "_Stz,pStats->Api.m_nExecCmdStr);
  }
  if (pStats->Api.m_nExecuteCommand) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("executeCommand %lld "_Stz,pStats->Api.m_nExecuteCommand);
  }
  if (pStats->Api.m_nFetchNextBuffer) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("fetchNextBuffer %lld "_Stz,pStats->Api.m_nFetchNextBuffer);
  }
  if (pStats->Api.m_nGetRecordBuffer) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("getRecordBuffer %lld "_Stz,pStats->Api.m_nGetRecordBuffer);
  }
  if (pStats->Api.m_nGetErrorMessage) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("getErrorMessage %lld "_Stz,pStats->Api.m_nGetErrorMessage);
  }
  if (pStats->Api.m_nGetRow) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("getRow %lld "_Stz,pStats->Api.m_nGetRow);
  }
  if (pStats->Api.m_nGetRowCount) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("getRowCount %lld "_Stz,pStats->Api.m_nGetRowCount);
  }
  if (pStats->Api.m_nGetRowCountTableColumn) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("GetRowCountTableColumn %lld "_Stz,pStats->Api.m_nGetRowCountTableColumn);
  }
  if (pStats->Api.m_nGetRowTableColumn) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("getRowTableColumn %lld "_Stz,pStats->Api.m_nGetRowTableColumn);
  }
  if (pStats->Api.m_nInsertRow) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("insertRow %lld "_Stz,pStats->Api.m_nInsertRow);
  }
  if (pStats->Api.m_nInsertRows) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("insertRows %lld "_Stz,pStats->Api.m_nInsertRows);
  }
  if (pStats->Api.m_nLoadResult) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("loadResult %lld "_Stz,pStats->Api.m_nLoadResult);
  }
  if (pStats->Api.m_nPoolOpen) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("poolOpen %lld "_Stz,pStats->Api.m_nPoolOpen);
  }
  if (pStats->Api.m_nScanNextRow) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("scanNextRow %lld "_Stz,pStats->Api.m_nScanNextRow);
  }
  if (pStats->Api.m_nSessionDelete) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("sessionDelete %lld "_Stz,pStats->Api.m_nSessionDelete);
  }
  if (pStats->Api.m_nSessionInvalid) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("sessionInvalid %lld "_Stz,pStats->Api.m_nSessionInvalid);
  }
  if (pStats->Api.m_nSessionNew) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("sessionNew %lld "_Stz,pStats->Api.m_nSessionNew);
  }
  
  if (nCats) {
    sPortrait.vCopy("Api: "_Stz);
    sPortrait.vConcat(s);
    sPortrait.vConcat("\n"_Stz);
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
