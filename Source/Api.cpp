/*============================================================================*/
/* Api.cpp - RapidsSE Api funcstions                                          */
/*============================================================================*/

#include "Interface.h"
#include "StorageEngine.h"

extern BufferPool*  gpBufferPool;
extern AttachMap*   gpAttachMap;
extern ActivityMap* gpActivityMap;

/*----------------------------------------------------------------------------*/
/* begin the insert table job                                                 */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_BeginInsertTable(Session* pSession, Stz* aTableName, void** ppJobPtr,
                              i8 iSubInserters, i8 iTraceLevel)
{
  gpAttachMap->m_oStatistics.Api.m_nBeginInsertTable++;
  
  Stz* aApiCall = "RapidsSE_BeginInsertTable"_Stz;
  eAC  eApiCall = AC_BeginInsertTable;
  ActivityMap::vMarkApi(eApiCall,0,">Api %s"_Stz,aApiCall);
  
  vApiCall(pSession,AC_BeginInsertTable);                               // API CALL
  vApiLocation(pSession,100);
  
  try {
    
    // trace entry
    TRACE_ENTRY_P0();
    
    // validate the session
    VALIDATE_SESSION("a"_Stz);
    
    // validate reference pointer
    VALIDATE_REFPOINTER("a"_Stz, ppJobPtr);
    
    // create Job_InsertTable
    JobClass* pJobClass_InsertTable = JobClass::pFind(CN_JOB_INSERTTABLE);
    ASSERT(pJobClass_InsertTable);
    Job_InsertTable* pJob_InsertTable =
    static_cast<Job_InsertTable*>(pJobClass_InsertTable->pPreJob(pSession));
    
    // find the table in the session dictionary stack
    Table* pTable = Table::pFindTable(pSession->pPathStack(), aTableName);
    
    // set the table in the InsertTable job
    pJob_InsertTable->m_pTable = pTable;
    
    // return the insertTable job
    *ppJobPtr = pJob_InsertTable;
    
    // set subInserters in the TblPtb
    gpAttachMap->m_nSubInserters = iSubInserters;
    
    // push the InsertTable job onto the jobstack
    pSession->m_pJobStack->vPushPointer(pJob_InsertTable);
    vApiJob(pSession,pJob_InsertTable);
    
    TRACE_EXIT("ea"_Stz);
    
    vApiReturn(pSession);                                          // API RETURN
    vApiLocation(pSession,900);
    
    ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
    
    return EC_NONE;
  }
  catch (Error* pError) {
    RapidsSE::vErrorHandler(pError);
    TRACE_ERROR("!", pError->eErrorCode());
    eEC eError = pError->eErrorCode();
    vApiError(pSession,eError);                                     // API ERROR
    vApiLocation(pSession,800);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return pError->eErrorCode();
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    vApiError(pSession,EC_SE_UNKNOWN_ERROR);                        // API ERROR
    vApiLocation(pSession,801);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return EC_SE_UNKNOWN_ERROR;
  }
}

/*----------------------------------------------------------------------------*/
/* begin the subInserter job                                                  */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_BeginSubInserter(Session* pSession, void* pJobPtr, void** ppJobPtr, i8 iTraceLevel)
{
  gpAttachMap->m_oStatistics.Api.m_nBeginSubInserter++;
  
  Stz* aApiCall = "RapidsSE_BeginSubInserter"_Stz;
  eAC  eApiCall = AC_BeginSubInserter;
  ActivityMap::vMarkApi(eApiCall,0,">Api %s"_Stz,aApiCall);
  
  vApiCall(pSession,AC_BeginInsertTable);                               // API CALL
  vApiLocation(pSession,100);
  
  try {
    
    // trace entry
    TRACE_ENTRY_P0();
    
    // validate the session
    VALIDATE_SESSION("a"_Stz);
    
    // validate the parameter
    VALIDATE_PARAM("a"_Stz, pJobPtr);
    
    // validate reference pointer
    VALIDATE_REFPOINTER("a"_Stz, ppJobPtr);
    
    // get the insertTable job
    Job_InsertTable* pJob_InsertTable = _pJob_InsertTable(pJobPtr);
    
    // create Job_SubInserter
    JobClass* pJobClass_SubInserter = JobClass::pFind(CN_JOB_SUBINSERTER);
    ASSERT(pJobClass_SubInserter);
    Job_SubInserter* pJob_SubInserter =
    static_cast<Job_SubInserter*>(pJobClass_SubInserter->pPreJob(pSession));
    
    // push the InsertTable job onto the jobstack
    pSession->m_pJobStack->vPushPointer(pJob_SubInserter);
    vApiJob(pSession,pJob_SubInserter);
    
    // set the insertTable in the subInserter
    pJob_SubInserter->m_pJob_InsertTable = pJob_InsertTable;
    pJob_SubInserter->m_pParentJob = pJob_InsertTable;
    
    // set the table pointer (THIS CAN GO ????)
    pJob_SubInserter->m_pTable = pJob_InsertTable->m_pTable;
    
    // return the sub_inserter job
    *ppJobPtr = pJob_SubInserter;
    
    TRACE_EXIT("ea"_Stz);
    
    vApiReturn(pSession);                                          // API RETURN
    vApiLocation(pSession,900);
    
    ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
    
    return EC_NONE;
  }
  catch (Error* pError) {
    RapidsSE::vErrorHandler(pError);
    TRACE_ERROR("!", pError->eErrorCode());
    eEC eError = pError->eErrorCode();
    vApiError(pSession,eError);                                     // API ERROR
    vApiLocation(pSession,800);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return pError->eErrorCode();
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    vApiError(pSession,EC_SE_UNKNOWN_ERROR);                        // API ERROR
    vApiLocation(pSession,801);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return EC_SE_UNKNOWN_ERROR;
  }
}

/*----------------------------------------------------------------------------*/
/* check the library version against version passed                           */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_CheckLibraryVersion(Stz* pVersion, i8 iTraceLevel)
{
  //gpAttachMap->m_oStatistics.Api.m_nCheckLibraryVersion++;
  
  Stz* aApiCall = "RapidsSE_CheckLibraryVersion"_Stz;
  eAC  eApiCall = AC_CheckLibraryVersion;
  ActivityMap::vMarkApi(eApiCall,0,">Api %s"_Stz,aApiCall);

  Session* pSession = nullptr;
  
  try {
    TRACE_ENTRY_P1(pVersion);
    
    // validate the parameter
    VALIDATE_PARAM("a"_Stz, pVersion);
    
    if ( ! pVersion->bEqual((Stz*)RAPIDSSE_VERSION_STRING)) {
      
      vTrace(">RapidsSE_CheckLibraryVersion: RDP (%s) does not match RapidsSE (%s)"_Stz,
          pVersion, RAPIDSSE_VERSION_STRING);
      
      TRACE_ERROR("b"_Stz, EC_VERSION_MISMATCH);
      ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
      return EC_VERSION_MISMATCH;
    }
    
    TRACE_EXIT("ea"_Stz);
    
    ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
    return EC_NONE;
  }
  catch (Error* pError) {
    RapidsSE::vErrorHandler(pError);
    TRACE_ERROR("!", pError->eErrorCode());
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return pError->eErrorCode();
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return EC_SE_UNKNOWN_ERROR;
  }
}

/*----------------------------------------------------------------------------*/
/* insert the record buffer into a table                                      */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_CopyBuffer(Session* pSession, void* pJobPtr, i8 iTraceLevel)
{
  gpAttachMap->m_oStatistics.Api.m_nCopyBuffer++;
  
  Stz* aApiCall = "RapidsSE_CopyBuffer"_Stz;
  eAC  eApiCall = AC_CopyBuffer;
  ActivityMap::vMarkApi(eApiCall,0,">Api %s"_Stz,aApiCall);
  
  vApiCall(pSession,AC_CopyBuffer);                                   // API CALL
  vApiLocation(pSession,100);
  
  try {
    
    // trace entry
    TRACE_ENTRY_P0();
    
    //TRACE_ROW();
    
    // validate the session
    VALIDATE_SESSION("a"_Stz);
    
    // validate the job
    VALIDATE_JOB("b"_Stz);
    
    // get the insertRow job
    Job_SubInserter* pJob_SubInserter = _pJob_SubInserter(pJobPtr);
    
    // get the record buffer
    RecordBuffer* pRecordBuffer = pJob_SubInserter->m_pRecordBuffer;
    
    //pJob_SubInserter->m_pRecordBuffer->vShow();
    
    if ( ! pRecordBuffer ) {
      TRACE_EXIT("ea"_Stz);
      return EC_NONE;
    }
    
    // create Job_CopyBuffer
    JobClass* pJobClass_CopyBuffer = JobClass::pFind(CN_JOB_COPYBUFFER);
    ASSERT(pJobClass_CopyBuffer);
    Job_CopyBuffer* pJob_CopyBuffer =
    static_cast<Job_CopyBuffer*>(pJobClass_CopyBuffer->pPreJob(pSession));
    
    // set the pJob_SubInserter in the pJob_CopyBuffer
    pJob_CopyBuffer->m_pJob_SubInserter = pJob_SubInserter;
    
    // move the recordBuffer from the subInsert into CopyBuffer
    pJob_CopyBuffer->m_pRecordBuffer = pRecordBuffer;
    ++pJob_SubInserter->m_ziRecordBuffers;
    pJob_SubInserter->m_pRecordBuffer = nullptr;
    
    // push the InsertTable job onto the jobstack
    //pSession->m_pJobStack->vPushPointer(pJob_CopyBuffer);
    //vApiJob(pSession,pJob_CopyBuffer);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // spawn the job
    //vTrace("Spawning copyBuffer job\n"_Stz);
    RapidsSE_bSpawnJob(pJob_CopyBuffer, SS_SEED_REQ, pSession);
    
    // trace exit
    TRACE_EXIT("eb"_Stz);
    vApiReturn(pSession);                                          // API RETURN
    vApiLocation(pSession,900);
    ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
    return EC_NONE;
    
  }
  catch (Error* pError) {
    RapidsSE::vErrorHandler(pError);
    TRACE_ERROR("!", pError->eErrorCode());
    eEC eError = pError->eErrorCode();
    vApiError(pSession,eError);                                     // API ERROR
    vApiLocation(pSession,800);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return pError->eErrorCode();
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    vApiError(pSession,EC_SE_UNKNOWN_ERROR);                        // API ERROR
    vApiLocation(pSession,801);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return EC_SE_UNKNOWN_ERROR;
  }
}

/*----------------------------------------------------------------------------*/
/* end insert and all session jobs                                            */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_EndInsertTable(Session* pSession, void* pJobPtr, i8 iTraceLevel)
{
  gpAttachMap->m_oStatistics.Api.m_nEndInsertJobs++;

  Stz* aApiCall = "RapidsSE_EndInsertJobs"_Stz;
  eAC  eApiCall = AC_EndInsertTable;
  ActivityMap::vMarkApi(eApiCall,0,">Api %s"_Stz,aApiCall);
  
  vApiCall(pSession,AC_EndInsertTable);                               // API CALL
  vApiLocation(pSession,100);
  
  try {
    
    // trace entry
    TRACE_ENTRY_P0();
    
    // validate the session
    VALIDATE_SESSION("a"_Stz);
    
    // validate the job
    VALIDATE_JOB("b"_Stz);
    
    // get the insertTable job
    Job_InsertTable* pJob_InsertTable = _pJob_InsertTable(pJobPtr);
    
    //vApiJob(pSession,pJob_InsertTable);
    vApiLocation(pSession,105);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // spawn and wait for job
    vSpawnJobAndWait(pSession, pJob_InsertTable);
 
    // check if job had an error
    if (pJob_InsertTable->m_bHasError) {
      
      for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
        
        eEC eError = pJob_InsertTable->m_xNode[i_Node].m_eErrorCode;
        
        if (eError != EC_NONE) {
          TRACE_ERROR("eb"_Stz, eError);
          vApiError(pSession,eError);
          vApiLocation(pSession,800);
          ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
          return pJob_InsertTable->m_xNode[i_Node].m_eErrorCode;
        }
      }
      ASSERT(0);
    }
    
    // delete all jobs in the ssession
    vDeleteJobs(pSession);
    
    TRACE_EXIT("ea"_Stz);
    
    vApiReturn(pSession);                                          // API RETURN
    vApiLocation(pSession,900);
    
    ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);

    return EC_NONE;
  }
  catch (Error* pError) {
    RapidsSE::vErrorHandler(pError);
    TRACE_ERROR("!", pError->eErrorCode());
    eEC eError = pError->eErrorCode();
    vApiError(pSession,eError);                                     // API ERROR
    vApiLocation(pSession,800);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return pError->eErrorCode();
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    vApiError(pSession,EC_SE_UNKNOWN_ERROR);                        // API ERROR
    vApiLocation(pSession,801);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
   return EC_SE_UNKNOWN_ERROR;
  }
}

/*----------------------------------------------------------------------------*/
/* end insert and all session jobs                                            */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_EndSubInserter(Session* pSession, void* pJobPtr, i8 iTraceLevel)
{
  gpAttachMap->m_oStatistics.Api.m_nEndInsertJobs++;
  
  Stz* aApiCall = "RapidsSE_EndSubInserter"_Stz;
  eAC  eApiCall = AC_EndSubInserter;
  ActivityMap::vMarkApi(eApiCall,0,">Api %s"_Stz,aApiCall);
  
  vApiCall(pSession,AC_EndSubInserter);                               // API CALL
  vApiLocation(pSession,100);
  
  try {
    
    // trace entry
    TRACE_ENTRY_P0();
    
    // validate the session
    VALIDATE_SESSION("a"_Stz);
    
    // validate the job
    VALIDATE_JOB("b"_Stz);
    
    // get the insertRows job
    Job_SubInserter* pJob_SubInserter = _pJob_SubInserter(pJobPtr);
    
    // wait for all record buffers to be copied across
    while (*pJob_SubInserter->m_ziRecordBuffers != *pJob_SubInserter->m_ziCopiedBuffers) {
      //vTrace("Waiting %lld %lld\n"_Stz, *pJob_SubInserter->m_ziRecordBuffers, *pJob_SubInserter->m_ziCopiedBuffers);
    }
    
    pJob_SubInserter->vEnd();
    
    // delete all jobs in the ssession
    vDeleteJobs(pSession);
    
    TRACE_EXIT("ea"_Stz);
    
    vApiReturn(pSession);                                          // API RETURN
    vApiLocation(pSession,900);
    
    ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
    
    return EC_NONE;
  }
  catch (Error* pError) {
    RapidsSE::vErrorHandler(pError);
    TRACE_ERROR("!", pError->eErrorCode());
    eEC eError = pError->eErrorCode();
    vApiError(pSession,eError);                                     // API ERROR
    vApiLocation(pSession,800);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return pError->eErrorCode();
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    vApiError(pSession,EC_SE_UNKNOWN_ERROR);                        // API ERROR
    vApiLocation(pSession,801);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return EC_SE_UNKNOWN_ERROR;
  }
}

/*----------------------------------------------------------------------------*/
/* end all jobs of this session                                               */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_EndJobs(Session* pSession, i8 iTraceLevel)
{
  gpAttachMap->m_oStatistics.Api.m_nEndJobs++;

  Stz* aApiCall = "RapidsSE_EndJobs"_Stz;
  eAC  eApiCall = AC_EndJobs;
  ActivityMap::vMarkApi(eApiCall,0,">Api %s"_Stz,aApiCall);
  
  vApiCall(pSession,AC_EndJobs);                                     // API CALL
  vApiLocation(pSession,100);
  
  // trace entry
  TRACE_ENTRY_P0();
  
  // validate the session
  VALIDATE_SESSION("a"_Stz);
  
  // delete the jobs from the sessions jobStack
  vDeleteJobs(pSession);
  
  // mark session as exiting (any fetches still running will stop)
  pSession->m_bSessionExiting = true;
  
  vApiReturn(pSession);                                            // API RETURN
  vApiLocation(pSession,900);
  
  ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* execute command                                                            */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_ExecuteCommand(Session* pSession, Stz* aCommand,
                            Stz* sCatalogName, Stz* sSchemaName,
                            void** ppJob, i8 iTraceLevel)
{
  gpAttachMap->m_oStatistics.Api.m_nExecuteCommand++;

  Stz* aApiCall = "RapidsSE_ExecuteCommand"_Stz;
  eAC  eApiCall = AC_ExecuteCommand;
  ActivityMap::vMarkApi(eApiCall,0,">Api %s"_Stz,aApiCall);

  vApiCall(pSession,AC_ExecuteCommand);                              // API CALL
  vApiLocation(pSession,100);
  
  try {
    // trace entry
    TRACE_ENTRY_P1(aCommand);

    vApiLocation(pSession,200);

    // validate the session
    VALIDATE_SESSION("a"_Stz);
    
    // set the catalog and schema in the session
    eEC eError = pSession->eInit(sCatalogName, sSchemaName);

    if (eError != EC_NONE) {
      TRACE_EXIT("ea"_Stz);
      vApiError(pSession,eError);                                     // API ERROR
      vApiLocation(pSession,811);
      ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
      return eError;
    }
    
    // execute the command
    vApiLocation(pSession,210);
    Job* pJob = RapidsSE_pExecCmdStr(aCommand, pSession, false, false);
    vApiLocation(pSession,220);

    vApiJob(pSession,pJob);                                             // API JOB

    // check if job had an error
    if (pJob->m_bHasError) {
      for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
        eEC eError = pJob->m_xNode[i_Node].m_eErrorCode;
        if ( eError != EC_NONE) {
          //vTrace("ERROR*** (NODE%lld): %s\n"_Stz, i_Node, aEC[pJob->m_xNode[i_Node].m_eErrorCode]);
          TRACE_ERROR("eb"_Stz, eError);
          vApiError(pSession,pJob->m_xNode[i_Node].m_eErrorCode);                                 // API ERROR
          vApiLocation(pSession,812);
          pSession->m_pJobStack->vPushPointer(pJob);
          ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
          return eError;
        }
      }
      ASSERT(0);
    }
    
    vApiLocation(pSession,230);

    // return job in ppJob
    *ppJob = pJob;
    
    vApiLocation(pSession,240);

    // push the job on the sessions job stack
    pSession->m_pJobStack->vPushPointer(pJob);
    
    TRACE_EXIT("ec"_Stz);

    vApiLocation(pSession,250);

    vApiReturn(pSession);                                            // API RETURN
    vApiLocation(pSession,900);
    ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
    return EC_NONE;
  }
  catch (Error* pError) {
    RapidsSE::vErrorHandler(pError);
    TRACE_ERROR("!", pError->eErrorCode());
    eEC eError = pError->eErrorCode();
    vApiError(pSession,eError);                                     // API ERROR
    vApiLocation(pSession,801);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return pError->eErrorCode();
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    vApiError(pSession,EC_SE_UNKNOWN_ERROR);                        // API ERROR
    vApiLocation(pSession,802);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return EC_SE_UNKNOWN_ERROR;
  }
}

/*----------------------------------------------------------------------------*/
/* fetch next buffer                                                          */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_FetchNextBuffer(Session* pSession, void* pJobPtr, void** ppFetchBuffer, i8* piExpectedRows, i8 iTraceLevel)
{
  gpAttachMap->m_oStatistics.Api.m_nFetchNextBuffer++;

  Stz* aApiCall = "RapidsSE_FetchNextBuffer"_Stz;
  eAC  eApiCall = AC_FetchNextBuffer;
  ActivityMap::vMarkApi(eApiCall,0,">Api %s"_Stz,aApiCall);

  vApiCall(pSession,AC_FetchNextBuffer);                             // API CALL
  vApiLocation(pSession,100);
  
  try {
           
    // trace entry
    TRACE_ENTRY_P0();
    
    // validate the session
    VALIDATE_SESSION("a"_Stz);
    
    // Validate job
    VALIDATE_JOB("b"_Stz);
    
    // validate the reference pointer
    VALIDATE_REFPOINTER("c"_Stz, ppFetchBuffer);
    
    Job_FetchRows* pJob_FetchRows = _pJob_FetchRows(pJobPtr);
    
    // if we had an entry buffer delete it
    pJob_FetchRows->vDeleteBuffer();
    
    // are we done ?
    if (pJob_FetchRows->m_nLastBuffersScanned == pJob_FetchRows->m_nLastBuffers) {
      *ppFetchBuffer = nullptr;
  
      // trace exit
      TRACE_EXIT("ea"_Stz);
      vApiReturn(pSession);                                        // API RETURN
      vApiLocation(pSession,910);
      ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
      return EC_NONE;
    }
    
    
    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
    
//    // pop an entryBuffer off the queue
//    GenVar xVariable;
//    while (!pJob_FetchRows->m_pBufferQueue->bPop(&xVariable)) {
//      //vTrace("SLEEPING\n"_Stz);
//      PosixThread::vSleepMilli(1);
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
      PosixThread::vSleepMilli(1);
    }
    pJob_FetchRows->m_pFetchBuffer = pFetchBuffer;
    
    *piExpectedRows = (i8)(pJob_FetchRows->m_pBufferQueue->iCount()
                           * C_ROWS_PER_BUFFER);

    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

    
    
    
    // is it a last buffer ?
    if (pJob_FetchRows->m_pFetchBuffer->bIsLast()) {
      pJob_FetchRows->m_nLastBuffersScanned++;
//      vTrace("FNB: LAST %p %s m_nLastBuffersScanned: %lld %lld\n"_Stz, pJob_FetchRows->m_pFetchBuffer,
//             pJob_FetchRows->m_pTable->aTableName(),
//             pJob_FetchRows->m_nLastBuffersScanned, pJob_FetchRows->m_nLastBuffers );
    }
    
//    vTrace("FNB: NOT LAST %p %s m_nLastBuffersScanned: %lld %lld\n"_Stz,pJob_FetchRows->m_pFetchBuffer,
//           pJob_FetchRows->m_pTable->aTableName(),
//           pJob_FetchRows->m_nLastBuffersScanned, pJob_FetchRows->m_nLastBuffers );
    
    // return the fetched buffer
    *ppFetchBuffer = pJob_FetchRows->m_pFetchBuffer;
    
    // trace exit
    TRACE_EXIT("eb"_Stz);
    
    vApiReturn(pSession);                                          // API RETURN
    vApiLocation(pSession,900);
    ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
    return EC_NONE;
  }
  catch (Error* pError) {
    RapidsSE::vErrorHandler(pError);
    TRACE_ERROR("!", pError->eErrorCode());
    eEC eError = pError->eErrorCode();
    vApiError(pSession,eError);                                     // API ERROR
    vApiLocation(pSession,801);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return pError->eErrorCode();
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    vApiError(pSession,EC_SE_UNKNOWN_ERROR);                        // API ERROR
    vApiLocation(pSession,802);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return EC_SE_UNKNOWN_ERROR;
  }
}

/*----------------------------------------------------------------------------*/
/* get last error                                                             */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetErrorMessage(Session* pSession, int iErrorCode, void** ppError, i8 iTraceLevel)
{
  gpAttachMap->m_oStatistics.Api.m_nGetErrorMessage++;

  Stz* aApiCall = "RapidsSE_GetErrorMessage"_Stz;
  eAC  eApiCall = AC_GetErrorMessage;
  ActivityMap::vMarkApi(eApiCall,0,">Api %s"_Stz,aApiCall);

  vApiCall(pSession,AC_GetErrorMessage);                             // API CALL
  vApiLocation(pSession,100);
  
  try {
    // trace entry
    TRACE_ENTRY_P0();
    
    // validate the session
    VALIDATE_SESSION("a"_Stz);
    
    // validate the reference pointer
    VALIDATE_REFPOINTER("b"_Stz, ppError);
    
    // get the error message
    pSession->m_sResponse = aEC[(eEC)iErrorCode];
    *ppError = pSession->m_sResponse.aChar();
    
    // trace exit
    TRACE_EXIT("ea"_Stz);
    
    vApiReturn(pSession);                                          // API RETURN
    vApiLocation(pSession,900);

    ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
    return EC_NONE;
  }
  catch (Error* pError) {
    RapidsSE::vErrorHandler(pError);
    TRACE_ERROR("!", pError->eErrorCode());
    eEC eError = pError->eErrorCode();
    vApiError(pSession,eError);                                     // API ERROR
    vApiLocation(pSession,801);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return pError->eErrorCode();
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    vApiError(pSession,EC_SE_UNKNOWN_ERROR);                        // API ERROR
    vApiLocation(pSession,802);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return EC_SE_UNKNOWN_ERROR;
  }
}

/*----------------------------------------------------------------------------*/
/* get record buffer from the job                                             */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetRecordBuffer(Session* pSession, void* pJobPtr, void** ppBuffer, i8 iTraceLevel)
{
  gpAttachMap->m_oStatistics.Api.m_nGetRecordBuffer++;

  Stz* aApiCall = "RapidsSE_GetRecordBuffer"_Stz;
  eAC  eApiCall = AC_GetRecordBuffer;
  ActivityMap::vMarkApi(eApiCall,0,">Api %s"_Stz,aApiCall);
  
  vApiCall(pSession,AC_GetRecordBuffer);                                      // API CALL
  vApiLocation(pSession,100);
  
  // trace entry
  TRACE_ENTRY_P0();
  
  // validate session
  VALIDATE_SESSION("a"_Stz);
  
  // validate the job
  VALIDATE_JOB("b"_Stz);
  
  // validate the reference pointer
  VALIDATE_REFPOINTER("c"_Stz, ppBuffer);
  
  Job_SubInserter* pJob_SubInserter = _pJob_SubInserter(pJobPtr);
  
  RASSERT( ! pJob_SubInserter->m_pRecordBuffer );
  pJob_SubInserter->m_pRecordBuffer = new RecordBuffer();
  
  *ppBuffer = pJob_SubInserter->m_pRecordBuffer;
  
  // trace exit
  TRACE_EXIT("ea"_Stz);
  
  vApiReturn(pSession);                                            // API RETURN
  vApiLocation(pSession,900);
  
  ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* get row                                                                    */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetRow(Session* pSession, void* pJobPtr, i8 iRow, void** ppRow, i8 iTraceLevel)
{
  gpAttachMap->m_oStatistics.Api.m_nGetRow++;

  Stz* aApiCall = "RapidsSE_GetRow"_Stz;
  eAC  eApiCall = AC_GetRow;
  ActivityMap::vMarkApi(eApiCall,0,">Api %s"_Stz,aApiCall);

  vApiCall(pSession,AC_GetRow);                                      // API CALL
  vApiLocation(pSession,100);
  
  // trace entry
  TRACE_ENTRY_P0();

  // validate session
  VALIDATE_SESSION("a"_Stz);
  
  // get the jobptr
  Job* pJob = (Job*)pJobPtr;
  
  // validate the job
  VALIDATE_JOB("b"_Stz);
 
  // validate the reference pointer
  VALIDATE_REFPOINTER("c"_Stz, ppRow);
  
  vTrace("Requesting row %lld\n"_Stz, iRow);
  
  i8 i_Node;
  i8 iMaxRowNo = 0;
  for (i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
    iMaxRowNo = *pJob->m_xNode[i_Node].m_ziRowsProcessed;
    if (iRow < iMaxRowNo)
      break;
    iRow -= iMaxRowNo;
  }
    
  // tokenize the response
  TokenVec* pTknVec = Tokeniser::pTokenise(pJob->m_xNode[i_Node].m_sResponse);
  
  i8 iRows = 0;
  i8 iToken = 0;
  
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
  
  pSession->m_sResponse.vCopy(""_Stz);
  while ( ! pTknVec->pGet(iToken)->aText()->bSame(")"_Stz)) {
    pSession->m_sResponse.vConcat(pTknVec->pGet(iToken++)->aText());
  }
  
  // return the row
  *ppRow = pSession->m_sResponse.aChar();
  vTrace("Returning row %lld %s\n"_Stz, iRow, pSession->m_sResponse.aChar());
  
  // delete the token vector
  pTknVec->vSetAllTokens();
  delete pTknVec;
  
  // trace exit
  TRACE_EXIT("ea"_Stz);
  
  vApiReturn(pSession);                                            // API RETURN
  vApiLocation(pSession,900);
  
  ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* get row count                                                              */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetRowCount(Session* pSession, void* pJobPtr, i8 *pRowCount, i8 iTraceLevel)
{
  gpAttachMap->m_oStatistics.Api.m_nGetRowCount++;

  Stz* aApiCall = "RapidsSE_GetRowCount"_Stz;
  eAC  eApiCall = AC_GetRowCount;
  ActivityMap::vMarkApi(eApiCall,0,">Api %s"_Stz,aApiCall);

  vApiCall(pSession,AC_GetRowCount);                                 // API CALL
  vApiLocation(pSession,100);
  
  // trace entry
  TRACE_ENTRY_P0();
  
  // validate session
  VALIDATE_SESSION("a"_Stz);
  
  Job* pJob = (Job*)pJobPtr;
  
  // validate the job
  VALIDATE_JOB("b"_Stz);
  
  // validate the pointer
  VALIDATE_PARAM("c"_Stz, pRowCount);
  
  i8 iRowsProcessed= 0;
  for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
    iRowsProcessed += *pJob->m_xNode[i_Node].m_ziRowsProcessed;
  }
  *pRowCount = iRowsProcessed;
  
  //vTrace("RowCount = %lld\n"_Stz, *pRowCount);
  
  // trace exit
  TRACE_EXIT("ea"_Stz);
  
  vApiReturn(pSession);                                            // API RETURN
  vApiLocation(pSession,900);
  
  ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* get row count table column                                                 */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetRowCountTableColumn(Session* pSession, void* pJobPtr, i8 *pRowCount, i8 iTraceLevel)
{
  gpAttachMap->m_oStatistics.Api.m_nCheckLibraryVersion++;

  Stz* aApiCall = "RapidsSE_GetRowCountTableColumn"_Stz;
  eAC  eApiCall = AC_GetRowCountTableColumn;
  ActivityMap::vMarkApi(eApiCall,0,">Api %s"_Stz,aApiCall);

  vApiCall(pSession,AC_GetRowCountTableColumn);                      // API CALL
  vApiLocation(pSession,100);
  
  // trace entry
  TRACE_ENTRY_P0();
  
  // validate session
  VALIDATE_SESSION("a"_Stz);
  
  // validate the job
  VALIDATE_JOB("b"_Stz);
  
  // validate the pointer
  VALIDATE_PARAM("c"_Stz, pRowCount);
  
  Job_ListColumns* pJob_ListColumns = _pJob_ListColumns(pJobPtr);
  Table* pTable = (Table*)StrLib::iToI8(pJob_ListColumns->m_xNode[gpAttachMap->m_pGrid->m_iThisNode].m_sResponse.aChar());
  ColumnVec* pColumnVec = pTable->pColumnVec();
  pJob_ListColumns->m_xNode[gpAttachMap->m_pGrid->iThisNode()].m_ziRowsProcessed =  pColumnVec->nItems() - 1;
  *pRowCount = *(pJob_ListColumns->m_xNode[gpAttachMap->m_pGrid->iThisNode()].m_ziRowsProcessed);
  
  //vTrace("RowCount = %lld\n"_Stz, pJob->m_xNode[gpAttachMap->m_pGrid->iThisNode()].m_iRowsProcessed);
  
  // trace exit
  TRACE_EXIT("ea"_Stz);
  
  vApiReturn(pSession);                                            // API RETURN
  vApiLocation(pSession,900);
  
  ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* get row table column                                                       */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetRowTableColumn(Session* pSession, void* pJobPtr, i8 iRow, void** ppRow, i8 iTraceLevel)
{
  gpAttachMap->m_oStatistics.Api.m_nGetRowTableColumn++;

  Stz* aApiCall = "RapidsSE_GetRowTableColumn"_Stz;
  eAC  eApiCall = AC_GetRowTableColumn;
  ActivityMap::vMarkApi(eApiCall,0,">Api %s"_Stz,aApiCall);

  vApiCall(pSession,AC_GetRowTableColumn);                           // API CALL
  vApiLocation(pSession,100);
  
  // trace entry
  TRACE_ENTRY_P0();
  
  // validate session
  VALIDATE_SESSION("a"_Stz);
  
  // validate the job
  VALIDATE_JOB("b"_Stz);
  
  // validate the reference pointer
  VALIDATE_REFPOINTER("c"_Stz, ppRow);
  
  Job_ListColumns* pJob_ListColumns = _pJob_ListColumns(pJobPtr);
  
  Table* pTable = (Table*)StrLib::iToI8(pJob_ListColumns->m_xNode[gpAttachMap->m_pGrid->m_iThisNode].m_sResponse.aChar());
  ColumnVec* pColumnVec = pTable->pColumnVec();
  
  Column* pColumn = pColumnVec->pGet(iRow);
  
  sprintf(pSession->m_sResponse.aChar(),"%lld,%s,%s,%lld,%lld,%s,%s",
           pColumn->iColumnNo(),
           pColumn->sColumnName().aChar(),
           aCT[pColumn->eColumnType()]->aChar(),
           pColumn->iColumnLength(),
           pColumn->iColumnScale(),
           pColumn->bIsNullable() ? "true" : "false",
           pColumn->bIsPartitionKey() ? "true": "false"
  );
    
  
  *ppRow = pSession->m_sResponse.aChar();
  
  // trace exit
  TRACE_EXIT("ea"_Stz);
  
  vApiReturn(pSession);                                            // API RETURN
  vApiLocation(pSession,900);
  
  ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* insert the given row into a table                                          */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_InsertRow(Session* pSession, Stz* sTableName, Stz* sRecord, i8 iTraceLevel)
{
  gpAttachMap->m_oStatistics.Api.m_nInsertRow++;

  Stz* aApiCall = "RapidsSE_InsertRow"_Stz;
  eAC  eApiCall = AC_InsertRow;
  ActivityMap::vMarkApi(eApiCall,0,">Api %s"_Stz,aApiCall);
  
  vApiCall(pSession,AC_InsertRow);                                   // API CALL
  vApiLocation(pSession,100);
  
  try {
    
    // trace entry
    TRACE_ENTRY_P0();
    
    //TRACE_ROW();
    
    // validate the session
    VALIDATE_SESSION("a"_Stz);
    
    // validate parameter
    VALIDATE_PARAM("b"_Stz, sRecord);
    
    // create Job_InsertTable
    JobClass* pJobClass_InsertRow = JobClass::pFind(CN_JOB_INSERTROW);
    ASSERT(pJobClass_InsertRow);
    Job_InsertRow* pJob_InsertRow =
      static_cast<Job_InsertRow*>(pJobClass_InsertRow->pPreJob(pSession));
    
    // push the InsertTable job onto the jobstack
    pSession->m_pJobStack->vPushPointer(pJob_InsertRow);
    vApiJob(pSession,pJob_InsertRow);
    
    // get the length of the record
    i8 iRawDataLength = StrLib::iSize(sRecord->aChar());
    
    // check length of record
    if (iRawDataLength > C_MAX_RECORD) {
      eEC eErrorCode = EC_MAX_ROW_LENGTH;
      TRACE_ERROR("ea"_Stz, eErrorCode);
      vApiReturn(pSession);                                        // API RETURN
      vApiLocation(pSession,910);
      ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
      return eErrorCode;
    }
    
    // set the row in the insertRow job
    pJob_InsertRow->m_sObjectName = sTableName;
    pJob_InsertRow->m_aRawData = sRecord;
    pJob_InsertRow->m_nRawDataBytes = iRawDataLength;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // spawn and wait for the job
    vSpawnJobAndWait(pSession, pJob_InsertRow);
    
    // check if job had an error
    if (pJob_InsertRow->m_bHasError) {
     
      for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
   
        eEC eError = pJob_InsertRow->m_xNode[i_Node].m_eErrorCode;

        if (eError != EC_NONE) {
          TRACE_ERROR("eb"_Stz, eError);
          vApiError(pSession,eError);
          vApiLocation(pSession,800);
          ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
          return pJob_InsertRow->m_xNode[i_Node].m_eErrorCode;
        }
      }
      ASSERT(0);
    }
    
    // trace exit
    TRACE_EXIT("ec"_Stz);
    vApiReturn(pSession);                                          // API RETURN
    vApiLocation(pSession,900);
    ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
    return EC_NONE;
    
  }
  catch (Error* pError) {
    RapidsSE::vErrorHandler(pError);
    TRACE_ERROR("!", pError->eErrorCode());
    eEC eError = pError->eErrorCode();
    vApiError(pSession,eError);                                     // API ERROR
    vApiLocation(pSession,901);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return pError->eErrorCode();
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    vApiError(pSession,EC_SE_UNKNOWN_ERROR);                        // API ERROR
    vApiLocation(pSession,902);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return EC_SE_UNKNOWN_ERROR;
  }
}

/*----------------------------------------------------------------------------*/
/* fetch next buffer                                                          */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_LoadResult(Session* pSession, void* pJobPtr, i8* pValidRows, i8* pInvalidRows, i8* pSkippedRows, i8 iTraceLevel)
{
  gpAttachMap->m_oStatistics.Api.m_nCheckLibraryVersion++;

  Stz* aApiCall = "RapidsSE_LoadResult"_Stz;
  eAC  eApiCall = AC_LoadResult;
  ActivityMap::vMarkApi(eApiCall,0,">Api %s"_Stz,aApiCall);

  vApiCall(pSession,AC_LoadResult);                                  // API CALL
  vApiLocation(pSession,100);
  
  try {
    // trace entry
    TRACE_ENTRY_P0();
    
    // validate the session
    VALIDATE_SESSION("a"_Stz);
    
    // validate the job
    VALIDATE_JOB("b"_Stz);
    
    // validate the reference pointers
    VALIDATE_REFPOINTER("b"_Stz, pValidRows);
    VALIDATE_REFPOINTER("b"_Stz, pInvalidRows);
    
    Job_LoadTable* pJob_LoadTable = _pJob_LoadTable(pJobPtr);
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
    
    *pValidRows   = iRowsProcessed;
    *pInvalidRows = iRowsInvalid;
    *pSkippedRows = iRowsSkipped;
    
    // trace exit
    TRACE_EXIT("ea"_Stz);
    
    vApiReturn(pSession);                                          // API RETURN
    vApiLocation(pSession,900);
    ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
    return EC_NONE;
  }
  catch (Error* pError) {
    RapidsSE::vErrorHandler(pError);
    TRACE_ERROR("!", pError->eErrorCode());
    eEC eError = pError->eErrorCode();
    vApiError(pSession,eError);                                     // API ERROR
    vApiLocation(pSession,800);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return pError->eErrorCode();
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    vApiError(pSession,EC_SE_UNKNOWN_ERROR);                        // API ERROR
    vApiLocation(pSession,801);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return EC_SE_UNKNOWN_ERROR;
  }
}

/*----------------------------------------------------------------------------*/
/* open the shared memory buffer pool                                         */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_PoolOpen(Stz* pInstanceName, void** ppInstance, i8 iTraceLevel)
{
  Stz* aApiCall = "RapidsSE_PoolOpen"_Stz;
                                                                      // API ???
  Session* pSession = nullptr;
  
  try {
    // trace entry
    TRACE_ENTRY_P1(pInstanceName);
    
    // open the pool
    s32 sPoolName;
    
    sPoolName = (Stz*)pInstanceName;
    bool bResult = RapidsSE::bOpenPool(sPoolName);
    
    if (!bResult) {
      TRACE_ERROR("a"_Stz, EC_INVALID_INSTANCE);
      return EC_INVALID_INSTANCE;
    }
    
    VALIDATE_POOL();
    
    *ppInstance = gpAttachMap;
    
    ActivityMap::vOpenByApi();
    
    // trace exit
    TRACE_EXIT("ea"_Stz);
    
    if (gpAttachMap)
      gpAttachMap->m_oStatistics.Api.m_nPoolOpen++;
    
    //vApiReturn(pSession);                                          // API RETURN
    return EC_NONE;
    
  }
  catch (Error* pError) {
    RapidsSE::vErrorHandler(pError);
    TRACE_ERROR("!", pError->eErrorCode());
    eEC eError = pError->eErrorCode();
    //vApiError(pSession,eError);                                     // API ERROR
    return eError;
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    //vApiError(pSession,EC_SE_UNKNOWN_ERROR);                        // API ERROR
    return EC_SE_UNKNOWN_ERROR;
  }

}

/*----------------------------------------------------------------------------*/
/* fetch next buffer                                                          */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_ScanNextRow(Session* pSession, void* pJobPtr, void** ppEntryBuffer, void** ppEntry, void** ppRow, i8* piExpectedRows, i8 iTraceLevel)
{
  gpAttachMap->m_oStatistics.Api.m_nScanNextRow++;

  Stz* aApiCall = "RapidsSE_ScanNextRow"_Stz;
  eAC  eApiCall = AC_ScanNextRow;
  ActivityMap::vMarkApi(eApiCall,0,">Api %s"_Stz,aApiCall);
  
  vApiCall(pSession,AC_ScanNextRow);
  vApiLocation(pSession,100);
  
  try {
    
    // trace entry
    TRACE_ENTRY_P0();
    
    // validate the session
    VALIDATE_SESSION("a"_Stz);
    
    // Validate job
    VALIDATE_JOB("b"_Stz);
    
    // validate the reference pointer
    VALIDATE_REFPOINTER("c"_Stz, ppRow);
    
    Job_SelectTable* pJob_SelectTable = _pJob_SelectTable(pJobPtr);
    
    // get the table
    Table* pTable = pJob_SelectTable->m_pTable;
    
    //*piExpectedRows = (i8)(pJob_FetchRows->m_pBufferQueue->iCount() * C_ROWS_PER_BUFFER);
    
    Entry* aEntry = nullptr;
    EntryBuffer* pEntryBuffer = nullptr;
    
    // do we have a buffer ?
    if ( ! *ppEntryBuffer) {
      // get first entry buffer
      TablePtn* pPartition = pTable->pPartition(pJob_SelectTable->m_iPartition);
      Index* pIndex = pTable->pHiddenIndex();
      IndexTree* pIndexTree = pPartition->pIndexTree(pIndex->iIndexNo());
      pEntryBuffer = pIndexTree->pFirstL0Buffer();
      vTrace("Got first buffer %p\n"_Stz, pEntryBuffer);
      if ( ! pEntryBuffer) {
        *ppRow = nullptr;
        
      } else {
        aEntry = pEntryBuffer->aFirstEntry();
        *ppEntry = aEntry;
        *ppEntryBuffer = pEntryBuffer;
        *ppRow = aEntry->aRow();
      }
      
      TRACE_EXIT("ea"_Stz);
      return EC_NONE;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // we have a buffer
    pEntryBuffer = *((EntryBuffer**)(ppEntryBuffer));
    
    // we should have an entry
    aEntry = *((Entry**)(ppEntry));
    RASSERT(aEntry);
    
    // get the next entry
    aEntry = pEntryBuffer->aNextEntry(aEntry, 0);
    
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
    
    if (aEntry) {
      TRACE_EXIT("eb"_Stz);
      *ppEntryBuffer = pEntryBuffer;
      *ppEntry = aEntry;
      *ppRow = aEntry->aRow();
      //vTrace("Returning rowEntry %p\n"_Stz, aEntry);
    }
    else {
      TRACE_EXIT("ec"_Stz);
      *ppRow = nullptr;
    }
    
    vApiReturn(pSession);                                          // API RETURN
    vApiLocation(pSession,900);
    ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
    return EC_NONE;
  }
  catch (Error* pError) {
    RapidsSE::vErrorHandler(pError);
    TRACE_ERROR("!", pError->eErrorCode());
    eEC eError = pError->eErrorCode();
    vApiError(pSession,eError);                                     // API ERROR
    vApiLocation(pSession,801);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return pError->eErrorCode();
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    vApiError(pSession,EC_SE_UNKNOWN_ERROR);                        // API ERROR
    vApiLocation(pSession,802);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return EC_SE_UNKNOWN_ERROR;
  }
}

/*----------------------------------------------------------------------------*/
/* delete an se session                                                       */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_SessionDelete(Session* pSession, i8 iTraceLevel)
{
  gpAttachMap->m_oStatistics.Api.m_nSessionDelete++;

  Stz* aApiCall = "RapidsSE_SessionDelete"_Stz;
  eAC  eApiCall = AC_SessionDelete;
  ActivityMap::vMarkApi(eApiCall,0,">Api %s"_Stz,aApiCall);

  vApiCall(pSession,AC_SessionDelete);                               // API CALL
  vApiLocation(pSession,100);
  
  try {
    // trace entry
    TRACE_ENTRY_P0();
    
    // validate the session
    VALIDATE_SESSION("a"_Stz);
    
    // delete jobs from the job stack of the session
    vDeleteJobs(pSession);
    
    vApiReturn(pSession);                                          // API RETURN
    vApiLocation(pSession,900);

    // delete the session
    RapidsSE::vDeleteSession(pSession);
    
    // set session to nullptr so trace knows not to look at it
    
    pSession = nullptr;
    
    // trace exit
    TRACE_EXIT("ea"_Stz);
    ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
    return EC_NONE;
  }
  catch (Error* pError) {
    RapidsSE::vErrorHandler(pError);
    TRACE_ERROR("!", pError->eErrorCode());
    eEC eError = pError->eErrorCode();
    //vApiError(pSession,eError);                                     // API ERROR
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return eError;
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    //vApiError(pSession,EC_SE_UNKNOWN_ERROR);                        // API ERROR
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return EC_SE_UNKNOWN_ERROR;
  }

}

/*----------------------------------------------------------------------------*/
/* is the error an EC_INVALID_SessionE ?                                      */
/*----------------------------------------------------------------------------*/
bool RapidsSE_SessionInvalid(int iError, i8 iTraceLevel)
{
  gpAttachMap->m_oStatistics.Api.m_nSessionInvalid++;

  Stz* aApiCall = "RapidsSE_SessionInvalid"_Stz;
  eAC  eApiCall = AC_SessionInvalid;
  ActivityMap::vMarkApi(eApiCall,0,">Api %s"_Stz,aApiCall);
                                                                      // API ???
  Session* pSession = nullptr;
  
  try {
    // trace entry
    TRACE_ENTRY_P0();
    
    if (iError == EC_INVALID_INTERFACE) {
      // trace exit
      TRACE_EXIT("ea"_Stz);

      ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
      return true;
    }
    else {
      // trace exit
      TRACE_EXIT("eb"_Stz);

      ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
      return false;
    }
    
  }
  catch (Error* pError) {
    RapidsSE::vErrorHandler(pError);
    TRACE_ERROR("!", pError->eErrorCode());
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return pError->eErrorCode();
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return EC_SE_UNKNOWN_ERROR;
  }
}

/*----------------------------------------------------------------------------*/
/* create a new se session                                                    */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_SessionNew(void** ppSession, Stz* sCatalogName, Stz* sSchemaName, i8 iPartition, i8 iTraceLevel)
{
  gpAttachMap->m_oStatistics.Api.m_nSessionNew++;

  Stz* aApiCall = "RapidsSE_SessionNew"_Stz;
  eAC  eApiCall = AC_SessionNew;
  ActivityMap::vMarkApi(eApiCall,0,">Api %s"_Stz,aApiCall);
                                                                      // API ???
  Session* pSession = nullptr;
  
  try {
    
    TRACE_ENTRY_P0();
    
    // trace entry
    TRACE_ENTRY_P2(sCatalogName, sSchemaName);
    //TRACE_ENTRY_P2(sCatalogName, sSchemaName);
    
    // valide reference pointer
    VALIDATE_REFPOINTER("a"_Stz, ppSession);
    
    VALIDATE_POOL();
    
    Session* pSession = new Session(iPartition);
    
    eEC eError = pSession->eInit(sCatalogName, sSchemaName);
    
    if (eError != EC_NONE) {
      TRACE_EXIT("ea"_Stz);
      ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
      return eError;
    }
    
    // validate the session just created
    VALIDATE_SESSION("b"_Stz);
    
    // return the newly created session
    *ppSession = pSession;
    
    // trace exit
    TRACE_EXIT("eb"_Stz);

    ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
    return EC_NONE;
    
  }
  catch (Error* pError) {
    RapidsSE::vErrorHandler(pError);
    TRACE_ERROR("!", pError->eErrorCode());
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return pError->eErrorCode();
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return EC_SE_UNKNOWN_ERROR;
  }
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* execute command                                                            */
/*----------------------------------------------------------------------------*/
Job* RapidsSE_pExecCmdStr(Stz* aCommand, Session* pSession,
                          bool bPostJob, bool bFromPrompt)
{
  gpAttachMap->m_oStatistics.Api.m_nExecCmdStr++;
  
  CommandProcessor* pCommandProcessor = pSession->m_pCommandProcessor;
  
  vApiLocation(pSession,101);
  
  pCommandProcessor->vTokenise(aCommand);
  
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
bool RapidsSE_bSpawnJob(Job* pJob, eSS eSeed, Session* pSession)
{
  //----------------------------------------------------------------------------
  // Before appending request for se, ensure that se is running
  // get the se pid from the attachmap
  i8 iPid = gpAttachMap->m_iPid;
  if (!Process::bTestPid(iPid)) {
    // SE is not running anymore!
    if (pSession)
      vError("Cannot find pid %lld\n"_Stz, gpAttachMap->m_iPid);
    vError("Cannot find pid %lld\n"_Stz, gpAttachMap->m_iPid);
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
    
    Job* pJobPtr = (Job*)pJobObject;
    
    // find the job class on this node
    eCN eClass = pJobPtr->eClass();
    JobClass* pJobClass = JobClass::pFind(eClass);
    
    vApiLocation(pSession,200);
    
    //vTrace("API deleting job %p jobId %lld %s\n"_Stz, pJobPtr, pJobPtr->m_iJobId, aCN[eClass]);
    
    // If we come across a JOB_FETCHROWS then we need to empty its queues
    if (eClass == CN_JOB_FETCHROWS) {
      Job_FetchRows* pJob_FetchRows = _pJob_FetchRows(pJobPtr);
  
      //vTrace("%s m_nLastBuffersScanned: %lld %lld\n"_Stz,pJob_FetchRows->m_pTable->aTableName(),
      //       pJob_FetchRows->m_nLastBuffersScanned, pJob_FetchRows->m_nLastBuffers );
      
      // if we had an entry buffer delete it
      pJob_FetchRows->vDeleteBuffer();
      
      while (pJob_FetchRows->m_nLastBuffersScanned != pJob_FetchRows->m_nLastBuffers) {
        // pop an entryBuffer off the queue
        vApiLocation(pSession,300);
        
        // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
//        GenVar xVariable;
//        while (!pJob_FetchRows->m_pBufferQueue->bPop(&xVariable)) {
//          PosixThread::vSleepMilli(1);
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
          PosixThread::vSleepMilli(1);
          vApiLocation(pSession,400);
          continue;
        }
        vApiLocation(pSession,500);

        pJob_FetchRows->m_pFetchBuffer = pFetchBuffer;
        // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

        
        if (pJob_FetchRows->m_pFetchBuffer->bIsLast()) {
          pJob_FetchRows->m_nLastBuffersScanned++;
          //vTrace("%s m_nLastBuffersScanned: %lld %lld\n"_Stz, pJob_FetchRows->m_pTable->aTableName(),
          //       pJob_FetchRows->m_nLastBuffersScanned, pJob_FetchRows->m_nLastBuffers );
        }
        
        // delete the buffer
        pJob_FetchRows->vDeleteBuffer();
      }
    }
    vApiLocation(pSession,600);
    
    // delete the job
    //vTrace("Deleting job %p %s\n"_Stz, pJobPtr, aCN[pJobPtr->eClass()]);
    pJobClass->vDeleteJob(pSession,pJobPtr);
    
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
  RapidsSE_bSpawnJob(pJob, SS_SEED_REQ, pSession);
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
void RapidsSE_vMonitorPortrait(s48K& sPortrait)
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
