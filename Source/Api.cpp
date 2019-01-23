/*============================================================================*/
/* Api.cpp - RapidsSE Api funcstions                                          */
/*============================================================================*/

#include "Interface.h"
#include "StorageEngine.h"

extern BufferPool*  gpBufferPool;
extern AttachMap*   gpAttachMap;
extern ActivityMap* gpActivityMap;



////////////////////////////////////////////////////////////////////////////////
//                            RapidsSE level calls                            //
////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* check the library version against given version                            */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_CheckLibraryVersion(i8 iTrace, Stz* sVersion)
{
  Session* pSession = nullptr;
  
  API_ENTRY(AC_CheckLibraryVersion);
  
  try {
    
    TRACE_ENTRY_P1(sVersion);
    
    // validate the parameter
    VALIDATE_PARAM("a"_Stz, sVersion);
    
    if ( ! sVersion->bEqual((Stz*)RAPIDSSE_VERSION_STRING)) {
      
      vTrace(">RapidsSE_CheckLibraryVersion: RDP (%s) does not match RapidsSE (%s)"_Stz,
             sVersion, RAPIDSSE_VERSION_STRING);
      
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
/* open the shared memory buffer pool                                         */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_PoolOpen(i8 iTrace, Stz* sRegion, void** aaAttachMap)
{
  Session* pSession = nullptr;
  
  API_ENTRY(AC_PoolOpen);
  
  try {
    
    // trace entry
    TRACE_ENTRY_P1(sRegion);
    
    // open the pool
    s32 sPoolName;
    
    sPoolName = (Stz*)sRegion;
    bool bResult = RapidsSE::bOpenPool(sPoolName);
    
    if (!bResult) {
      TRACE_ERROR("a"_Stz, EC_INVALID_INSTANCE);
      return EC_INVALID_INSTANCE;
    }
    
    VALIDATE_POOL();
    
    *aaAttachMap = gpAttachMap;
    
    ActivityMap::vOpenByApi();
    
    // trace exit
    TRACE_EXIT("ea"_Stz);
    
    if (gpAttachMap)
      gpAttachMap->m_oStatistics.Api.m_nPoolOpen++;
    
    //vApiReturn(pSession);
    return EC_NONE;
    
  }
  catch (Error* pError) {
    RapidsSE::vErrorHandler(pError);
    TRACE_ERROR("!", pError->eErrorCode());
    eEC eError = pError->eErrorCode();
    //vApiError(pSession,eError);
    return eError;
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    //vApiError(pSession,EC_SE_UNKNOWN_ERROR);
    return EC_SE_UNKNOWN_ERROR;
  }
  
}
/*----------------------------------------------------------------------------*/
/* is the error an EC_INVALID_SessionE ?                                      */
/*----------------------------------------------------------------------------*/
bool RapidsSE_SessionInvalid(i8 iTrace, int iError)
{
  Session* pSession = nullptr;
  
  API_ENTRY(AC_SessionInvalid);
  
  try {
    
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
eEC RapidsSE_SessionNew(i8 iTrace, void** aaSession,
                        Stz* sCatalog, Stz* sSchema, i8 iPartition)
{
  Session* pSession = nullptr;
  
  API_ENTRY(AC_SessionNew);
  
  try {
    
    // trace entry
    TRACE_ENTRY_P2(sCatalog, sSchema);
    //TRACE_ENTRY_P2(sCatalog, sSchema);
    
    // valide reference pointer
    VALIDATE_REFPOINTER("a"_Stz, aaSession);
    
    VALIDATE_POOL();
    
    Session* pSession = new Session(iPartition);
    
    eEC eError = pSession->eInit(sCatalog, sSchema);
    
    if (eError != EC_NONE) {
      TRACE_EXIT("ea"_Stz);
      ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
      return eError;
    }
        
    // return the newly created session
    *aaSession = pSession;
    
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


////////////////////////////////////////////////////////////////////////////////
//                             Session level calls                            //
////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* begin the insert table job                                                 */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_BeginInsertTable(i8 iTrace, Session* pSession,
                              Stz* sTable, void** aaJob, i8 nSubInserters)
{
  API_ENTRY(AC_BeginInsertTable);
  
  try {
    
    // validate reference pointer
    VALIDATE_REFPOINTER("a"_Stz, aaJob);
    
    // create Job_InsertTable
    JobClass* pJobClass_InsertTable = JobClass::pFind(CN_JOB_INSERTTABLE);
    ASSERT(pJobClass_InsertTable);
    Job_InsertTable* pJob_InsertTable =
    static_cast<Job_InsertTable*>(pJobClass_InsertTable->pPreJob(pSession));
    
    // find the table in the session dictionary stack
    Table* pTable = Table::pFindTable(pSession->pPathStack(), sTable);
    
    // set the table in the InsertTable job
    pJob_InsertTable->m_pTable = pTable;
    
    // return the insertTable job
    *aaJob = pJob_InsertTable;
    
    // set subInserters in the TblPtb
    gpAttachMap->m_nSubInserters = nSubInserters;
    
    // push the InsertTable job onto the jobstack
    pSession->m_pJobStack->vPushPointer(pJob_InsertTable);
    vApiJob(pSession,pJob_InsertTable);
    
    TRACE_EXIT("ea"_Stz);
    
    vApiReturn(pSession);
    vApiLocation(pSession,900);
    
    ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
    
    return EC_NONE;
  }
  catch (Error* pError) {
    RapidsSE::vErrorHandler(pError);
    TRACE_ERROR("!", pError->eErrorCode());
    eEC eError = pError->eErrorCode();
    vApiError(pSession,eError);
    vApiLocation(pSession,800);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return pError->eErrorCode();
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    vApiError(pSession,EC_SE_UNKNOWN_ERROR);
    vApiLocation(pSession,801);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return EC_SE_UNKNOWN_ERROR;
  }
}

/*----------------------------------------------------------------------------*/
/* end all jobs of this session                                               */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_EndJobs(i8 iTrace, Session* pSession)
{
  API_ENTRY(AC_EndJobs);
  
  // delete the jobs from the sessions jobStack
  vDeleteJobs(pSession);
  
  // mark session as exiting (any fetches still running will stop)
  pSession->m_bSessionExiting = true;
  
  vApiReturn(pSession);
  vApiLocation(pSession,900);
  
  ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* execute command                                                            */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_ExecuteCommand(i8 iTrace, Session* pSession,
                            Stz* sCommand, Stz* sCatalog, Stz* sSchema,
                            void** aaJob)
{
  API_ENTRY(AC_ExecuteCommand);
  
  try {
    // trace entry
    TRACE_ENTRY_P1(sCommand);
    
    vApiLocation(pSession,200);
    
    // set the catalog and schema in the session
    eEC eError = pSession->eInit(sCatalog, sSchema);
    
    if (eError != EC_NONE) {
      TRACE_EXIT("ea"_Stz);
      vApiError(pSession,eError);
      vApiLocation(pSession,811);
      ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
      return eError;
    }
    
    // execute the command
    vApiLocation(pSession,210);
    Job* pJob = RapidsSE_pExecCmdStr(sCommand, pSession, false, false);
    vApiLocation(pSession,220);
    
    vApiJob(pSession,pJob);                                             // API JOB
    
    // check if job had an error
    if (pJob->m_bHasError) {
      for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
        eEC eError = pJob->m_xNode[i_Node].m_eErrorCode;
        if ( eError != EC_NONE) {
          //vTrace("ERROR*** (NODE%lld): %s\n"_Stz, i_Node, aEC[pJob->m_xNode[i_Node].m_eErrorCode]);
          TRACE_ERROR("eb"_Stz, eError);
          vApiError(pSession,pJob->m_xNode[i_Node].m_eErrorCode);
          vApiLocation(pSession,812);
          pSession->m_pJobStack->vPushPointer(pJob);
          ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
          return eError;
        }
      }
      ASSERT(0);
    }
    
    vApiLocation(pSession,230);
    
    // return job in aaJob
    *aaJob = pJob;
    
    vApiLocation(pSession,240);
    
    // push the job on the sessions job stack
    pSession->m_pJobStack->vPushPointer(pJob);
    
    TRACE_EXIT("ec"_Stz);
    
    vApiLocation(pSession,250);
    
    vApiReturn(pSession);
    vApiLocation(pSession,900);
    ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
    return EC_NONE;
  }
  catch (Error* pError) {
    RapidsSE::vErrorHandler(pError);
    TRACE_ERROR("!", pError->eErrorCode());
    eEC eError = pError->eErrorCode();
    vApiError(pSession,eError);
    vApiLocation(pSession,801);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return pError->eErrorCode();
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    vApiError(pSession,EC_SE_UNKNOWN_ERROR);
    vApiLocation(pSession,802);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return EC_SE_UNKNOWN_ERROR;
  }
}

/*----------------------------------------------------------------------------*/
/* get last error                                                             */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetErrorMessage(i8 iTrace, Session* pSession,
                             int iErrorCode, void** aaError)
{
  API_ENTRY(AC_GetErrorMessage);
  
  try {
    
    // validate the reference pointer
    VALIDATE_REFPOINTER("b"_Stz, aaError);
    
    // get the error message
    pSession->m_sResponse = aEC[(eEC)iErrorCode];
    *aaError = pSession->m_sResponse.aChar();
    
    // trace exit
    TRACE_EXIT("ea"_Stz);
    
    vApiReturn(pSession);
    vApiLocation(pSession,900);
    
    ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
    return EC_NONE;
  }
  catch (Error* pError) {
    RapidsSE::vErrorHandler(pError);
    TRACE_ERROR("!", pError->eErrorCode());
    eEC eError = pError->eErrorCode();
    vApiError(pSession,eError);
    vApiLocation(pSession,801);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return pError->eErrorCode();
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    vApiError(pSession,EC_SE_UNKNOWN_ERROR);
    vApiLocation(pSession,802);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return EC_SE_UNKNOWN_ERROR;
  }
}

/*----------------------------------------------------------------------------*/
/* insert the given row into a table                                          */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_InsertRow(i8 iTrace, Session* pSession,
                       Stz* sTableName, Stz* sRecord)
{
  API_ENTRY(AC_InsertRow);
  
  try {
    
    //TRACE_ROW();
    
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
      vApiReturn(pSession);
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
    vApiReturn(pSession);
    vApiLocation(pSession,900);
    ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
    return EC_NONE;
    
  }
  catch (Error* pError) {
    RapidsSE::vErrorHandler(pError);
    TRACE_ERROR("!", pError->eErrorCode());
    eEC eError = pError->eErrorCode();
    vApiError(pSession,eError);
    vApiLocation(pSession,901);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return pError->eErrorCode();
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    vApiError(pSession,EC_SE_UNKNOWN_ERROR);
    vApiLocation(pSession,902);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return EC_SE_UNKNOWN_ERROR;
  }
}

/*----------------------------------------------------------------------------*/
/* delete an se session                                                       */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_SessionDelete(i8 iTrace, Session* pSession)
{
  API_ENTRY(AC_SessionDelete);
  
  try {
    
    // delete jobs from the job stack of the session
    vDeleteJobs(pSession);
    
    vApiReturn(pSession);
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
    //vApiError(pSession,eError);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return eError;
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    //vApiError(pSession,EC_SE_UNKNOWN_ERROR);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return EC_SE_UNKNOWN_ERROR;
  }
  
}


////////////////////////////////////////////////////////////////////////////////
//                               Job level calls                              //
////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* begin the subInserter job                                                  */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_BeginSubInserter(i8 iTrace, Session* pSession, void* aJob,
                              void** aaJob)
{
  API_ENTRY(AC_BeginSubInserter);
  
  try {
    
    // validate the parameter
    VALIDATE_PARAM("a"_Stz, aJob);
    
    // validate reference pointer
    VALIDATE_REFPOINTER("a"_Stz, aaJob);
    
    // get the insertTable job
    Job_InsertTable* pJob_InsertTable = _pJob_InsertTable(aJob);
    
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
    *aaJob = pJob_SubInserter;
    
    TRACE_EXIT("ea"_Stz);
    
    vApiReturn(pSession);
    vApiLocation(pSession,900);
    
    ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
    
    return EC_NONE;
  }
  catch (Error* pError) {
    RapidsSE::vErrorHandler(pError);
    TRACE_ERROR("!", pError->eErrorCode());
    eEC eError = pError->eErrorCode();
    vApiError(pSession,eError);
    vApiLocation(pSession,800);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return pError->eErrorCode();
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    vApiError(pSession,EC_SE_UNKNOWN_ERROR);
    vApiLocation(pSession,801);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return EC_SE_UNKNOWN_ERROR;
  }
}

/*----------------------------------------------------------------------------*/
/* insert the record buffer into a table                                      */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_CopyBuffer(i8 iTrace, Session* pSession, void* aJob)
{
  API_ENTRY(AC_CopyBuffer);
  
  try {
    
    //TRACE_ROW();
    
    // validate the job
    VALIDATE_JOB("b"_Stz);
    
    // get the insertRow job
    Job_SubInserter* pJob_SubInserter = _pJob_SubInserter(aJob);
    
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
    vApiReturn(pSession);
    vApiLocation(pSession,900);
    ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
    return EC_NONE;
    
  }
  catch (Error* pError) {
    RapidsSE::vErrorHandler(pError);
    TRACE_ERROR("!", pError->eErrorCode());
    eEC eError = pError->eErrorCode();
    vApiError(pSession,eError);
    vApiLocation(pSession,800);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return pError->eErrorCode();
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    vApiError(pSession,EC_SE_UNKNOWN_ERROR);
    vApiLocation(pSession,801);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return EC_SE_UNKNOWN_ERROR;
  }
}

/*----------------------------------------------------------------------------*/
/* end insert and all session jobs                                            */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_EndInsertTable(i8 iTrace, Session* pSession, void* aJob)
{
  API_ENTRY(AC_EndInsertTable);
  
  try {
    
    // validate the job
    VALIDATE_JOB("b"_Stz);
    
    // get the insertTable job
    Job_InsertTable* pJob_InsertTable = _pJob_InsertTable(aJob);
    
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
    
    vApiReturn(pSession);
    vApiLocation(pSession,900);
    
    ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);

    return EC_NONE;
  }
  catch (Error* pError) {
    RapidsSE::vErrorHandler(pError);
    TRACE_ERROR("!", pError->eErrorCode());
    eEC eError = pError->eErrorCode();
    vApiError(pSession,eError);
    vApiLocation(pSession,800);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return pError->eErrorCode();
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    vApiError(pSession,EC_SE_UNKNOWN_ERROR);
    vApiLocation(pSession,801);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
   return EC_SE_UNKNOWN_ERROR;
  }
}

/*----------------------------------------------------------------------------*/
/* end insert and all session jobs                                            */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_EndSubInserter(i8 iTrace, Session* pSession, void* aJob)
{
  API_ENTRY(AC_EndSubInserter);
  
  try {
    
    // validate the job
    VALIDATE_JOB("b"_Stz);
    
    // get the insertRows job
    Job_SubInserter* pJob_SubInserter = _pJob_SubInserter(aJob);
    
    // wait for all record buffers to be copied across
    while (*pJob_SubInserter->m_ziRecordBuffers != *pJob_SubInserter->m_ziCopiedBuffers) {
      //vTrace("Waiting %lld %lld\n"_Stz, *pJob_SubInserter->m_ziRecordBuffers, *pJob_SubInserter->m_ziCopiedBuffers);
    }
    
    pJob_SubInserter->vEnd();
    
    // delete all jobs in the ssession
    vDeleteJobs(pSession);
    
    TRACE_EXIT("ea"_Stz);
    
    vApiReturn(pSession);
    vApiLocation(pSession,900);
    
    ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
    
    return EC_NONE;
  }
  catch (Error* pError) {
    RapidsSE::vErrorHandler(pError);
    TRACE_ERROR("!", pError->eErrorCode());
    eEC eError = pError->eErrorCode();
    vApiError(pSession,eError);
    vApiLocation(pSession,800);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return pError->eErrorCode();
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    vApiError(pSession,EC_SE_UNKNOWN_ERROR);
    vApiLocation(pSession,801);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return EC_SE_UNKNOWN_ERROR;
  }
}

/*----------------------------------------------------------------------------*/
/* fetch next buffer                                                          */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_FetchNextBuffer(i8 iTrace, Session* pSession, void* aJob,
                             void** aaFetchBuffer, i8* piExpectedRows)
{
  API_ENTRY(AC_FetchNextBuffer);
  
  try {
    
    // Validate job
    VALIDATE_JOB("b"_Stz);
    
    // validate the reference pointer
    VALIDATE_REFPOINTER("c"_Stz, aaFetchBuffer);
    
    Job_FetchRows* pJob_FetchRows = _pJob_FetchRows(aJob);
    
    // if we had an entry buffer delete it
    pJob_FetchRows->vDeleteBuffer();
    
    // are we done ?
    if (pJob_FetchRows->m_nLastBuffersScanned == pJob_FetchRows->m_nLastBuffers) {
      *aaFetchBuffer = nullptr;
  
      // trace exit
      TRACE_EXIT("ea"_Stz);
      vApiReturn(pSession);
      vApiLocation(pSession,910);
      ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
      return EC_NONE;
    }
    
    
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
      Pthread::vSleepMilli(1);
    }
    pJob_FetchRows->m_pFetchBuffer = pFetchBuffer;
    
    *piExpectedRows = (i8)(pJob_FetchRows->m_pBufferQueue->iCount()
                           * C_ROWS_PER_BUFFER);

    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

    
    
    
    // is it a last buffer ?
    if (pJob_FetchRows->m_pFetchBuffer->bIsLast()) {
      pJob_FetchRows->m_nLastBuffersScanned++;
//      vTrace("FNB: LAST %p %s m_nLastBuffersScanned: %lld %lld\n"_Stz, pJob_FetchRows->m_pFetchBuffer,
//             pJob_FetchRows->m_pTable->sTable(),
//             pJob_FetchRows->m_nLastBuffersScanned, pJob_FetchRows->m_nLastBuffers );
    }
    
//    vTrace("FNB: NOT LAST %p %s m_nLastBuffersScanned: %lld %lld\n"_Stz,pJob_FetchRows->m_pFetchBuffer,
//           pJob_FetchRows->m_pTable->sTable(),
//           pJob_FetchRows->m_nLastBuffersScanned, pJob_FetchRows->m_nLastBuffers );
    
    // return the fetched buffer
    *aaFetchBuffer = pJob_FetchRows->m_pFetchBuffer;
    
    // trace exit
    TRACE_EXIT("eb"_Stz);
    
    vApiReturn(pSession);
    vApiLocation(pSession,900);
    ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
    return EC_NONE;
  }
  catch (Error* pError) {
    RapidsSE::vErrorHandler(pError);
    TRACE_ERROR("!", pError->eErrorCode());
    eEC eError = pError->eErrorCode();
    vApiError(pSession,eError);
    vApiLocation(pSession,801);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return pError->eErrorCode();
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    vApiError(pSession,EC_SE_UNKNOWN_ERROR);
    vApiLocation(pSession,802);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return EC_SE_UNKNOWN_ERROR;
  }
}

/*----------------------------------------------------------------------------*/
/* get record buffer from the job                                             */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetRecordBuffer(i8 iTrace, Session* pSession, void* aJob,
                             void** aaBuffer)
{
  API_ENTRY(AC_GetRecordBuffer);
  
  // validate the job
  VALIDATE_JOB("b"_Stz);
  
  // validate the reference pointer
  VALIDATE_REFPOINTER("c"_Stz, aaBuffer);
  
  Job_SubInserter* pJob_SubInserter = _pJob_SubInserter(aJob);
  
  RASSERT( ! pJob_SubInserter->m_pRecordBuffer );
  pJob_SubInserter->m_pRecordBuffer = new RecordBuffer();
  
  *aaBuffer = pJob_SubInserter->m_pRecordBuffer;
  
  // trace exit
  TRACE_EXIT("ea"_Stz);
  
  vApiReturn(pSession);
  vApiLocation(pSession,900);
  
  ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* get row                                                                    */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetRow(i8 iTrace, Session* pSession, void* aJob,
                    i8 iRow, void** aaRow)
{
  API_ENTRY(AC_GetRow);
  
  // get the jobptr
  Job* pJob = (Job*)aJob;
  
  // validate the job
  VALIDATE_JOB("b"_Stz);
 
  // validate the reference pointer
  VALIDATE_REFPOINTER("c"_Stz, aaRow);
  
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
  *aaRow = pSession->m_sResponse.aChar();
  
  // delete the token vector
  pTknVec->vSetAllTokens();
  delete pTknVec;
  
  // trace exit
  TRACE_EXIT("ea"_Stz);
  
  vApiReturn(pSession);
  vApiLocation(pSession,900);
  
  ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* get row count                                                              */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetRowCount(i8 iTrace, Session* pSession, void* aJob,
                         i8 *aRowCount)
{
  API_ENTRY(AC_GetRowCount);
  
  Job* pJob = (Job*)aJob;
  
  // validate the job
  VALIDATE_JOB("b"_Stz);
  
  // validate the pointer
  VALIDATE_PARAM("c"_Stz, aRowCount);
  
  i8 iRowsProcessed= 0;
  for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
    iRowsProcessed += *pJob->m_xNode[i_Node].m_ziRowsProcessed;
  }
  *aRowCount = iRowsProcessed;
  
  //vTrace("RowCount = %lld\n"_Stz, *aRowCount);
  
  // trace exit
  TRACE_EXIT("ea"_Stz);
  
  vApiReturn(pSession);
  vApiLocation(pSession,900);
  
  ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* get row count table column                                                 */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetRowCountTableColumn(i8 iTrace, Session* pSession, void* aJob,
                                    i8 *aRowCount)
{
  API_ENTRY(AC_GetRowCountTableColumn);
  
  // validate the job
  VALIDATE_JOB("b"_Stz);
  
  // validate the pointer
  VALIDATE_PARAM("c"_Stz, aRowCount);
  
  Job_ListColumns* pJob_ListColumns = _pJob_ListColumns(aJob);
  Table* pTable = (Table*)StrLib::iToI8(pJob_ListColumns->m_xNode[gpAttachMap->m_pGrid->m_iThisNode].m_sResponse.aChar());
  ColumnVec* pColumnVec = pTable->pColumnVec();
  pJob_ListColumns->m_xNode[gpAttachMap->m_pGrid->iThisNode()].m_ziRowsProcessed =  pColumnVec->nItems() - 1;
  *aRowCount = *(pJob_ListColumns->m_xNode[gpAttachMap->m_pGrid->iThisNode()].m_ziRowsProcessed);
  
  //vTrace("RowCount = %lld\n"_Stz, pJob->m_xNode[gpAttachMap->m_pGrid->iThisNode()].m_iRowsProcessed);
  
  // trace exit
  TRACE_EXIT("ea"_Stz);
  
  vApiReturn(pSession);
  vApiLocation(pSession,900);
  
  ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* get row table column                                                       */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetRowTableColumn(i8 iTrace, Session* pSession, void* aJob,
                               i8 iRow, void** aaRow)
{
  API_ENTRY(AC_GetRowTableColumn);
  
  // validate the job
  VALIDATE_JOB("b"_Stz);
  
  // validate the reference pointer
  VALIDATE_REFPOINTER("c"_Stz, aaRow);
  
  Job_ListColumns* pJob_ListColumns = _pJob_ListColumns(aJob);
  
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
    
  
  *aaRow = pSession->m_sResponse.aChar();
  
  // trace exit
  TRACE_EXIT("ea"_Stz);
  
  vApiReturn(pSession);
  vApiLocation(pSession,900);
  
  ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* fetch next buffer                                                          */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_LoadResult(i8 iTrace, Session* pSession, void* aJob,
                        i8* pValidRows, i8* pInvalidRows, i8* pSkippedRows)
{
  API_ENTRY(AC_LoadResult);
  
  try {
    
    // validate the job
    VALIDATE_JOB("b"_Stz);
    
    // validate the reference pointers
    VALIDATE_REFPOINTER("b"_Stz, pValidRows);
    VALIDATE_REFPOINTER("b"_Stz, pInvalidRows);
    
    Job_LoadTable* pJob_LoadTable = _pJob_LoadTable(aJob);
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
    
    vApiReturn(pSession);
    vApiLocation(pSession,900);
    ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
    return EC_NONE;
  }
  catch (Error* pError) {
    RapidsSE::vErrorHandler(pError);
    TRACE_ERROR("!", pError->eErrorCode());
    eEC eError = pError->eErrorCode();
    vApiError(pSession,eError);
    vApiLocation(pSession,800);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return pError->eErrorCode();
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    vApiError(pSession,EC_SE_UNKNOWN_ERROR);
    vApiLocation(pSession,801);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return EC_SE_UNKNOWN_ERROR;
  }
}

/*----------------------------------------------------------------------------*/
/* fetch next buffer                                                          */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_ScanNextRow(i8 iTrace, Session* pSession, void* aJob,
                         void** aaEntryBuffer, void** aaEntry, void** aaRow,
                         i8* piExpectedRows)
{
  API_ENTRY(AC_ScanNextRow);
  
  try {
    
    // Validate job
    VALIDATE_JOB("b"_Stz);
    
    // validate the reference pointer
    VALIDATE_REFPOINTER("c"_Stz, aaRow);
    
    Job_SelectTable* pJob_SelectTable = _pJob_SelectTable(aJob);
    
    // get the table
    Table* pTable = pJob_SelectTable->m_pTable;
    
    //*piExpectedRows = (i8)(pJob_FetchRows->m_pBufferQueue->iCount() * C_ROWS_PER_BUFFER);
    
    Entry* aEntry = nullptr;
    EntryBuffer* pEntryBuffer = nullptr;
    
    // do we have a buffer ?
    if ( ! *aaEntryBuffer) {
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
      
      TRACE_EXIT("ea"_Stz);
      return EC_NONE;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // we have a buffer
    pEntryBuffer = *((EntryBuffer**)(aaEntryBuffer));
    
    // we should have an entry
    aEntry = *((Entry**)(aaEntry));
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
      *aaEntryBuffer = pEntryBuffer;
      *aaEntry = aEntry;
      *aaRow = aEntry->aRow();
      //vTrace("Returning rowEntry %p\n"_Stz, aEntry);
    }
    else {
      TRACE_EXIT("ec"_Stz);
      *aaRow = nullptr;
    }
    
    vApiReturn(pSession);
    vApiLocation(pSession,900);
    ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
    return EC_NONE;
  }
  catch (Error* pError) {
    RapidsSE::vErrorHandler(pError);
    TRACE_ERROR("!", pError->eErrorCode());
    eEC eError = pError->eErrorCode();
    vApiError(pSession,eError);
    vApiLocation(pSession,801);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return pError->eErrorCode();
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    vApiError(pSession,EC_SE_UNKNOWN_ERROR);
    vApiLocation(pSession,802);
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
Job* RapidsSE_pExecCmdStr(Stz* sCommand, Session* pSession,
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
bool RapidsSE_bSpawnJob(Job* pJob, eSS eSeed, Session* pSession)
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
          Pthread::vSleepMilli(1);
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
void vApiStatistics(eAC eApiCall)
{
  if (! gpAttachMap)
    return;
  
  switch (eApiCall) {
    case AC_BeginInsertTable:    gpAttachMap->m_oStatistics.Api.m_nBeginInsertTable++;     break;
    case AC_BeginSubInserter:    gpAttachMap->m_oStatistics.Api.m_nBeginSubInserter++;     break;
    case AC_CheckLibraryVersion: gpAttachMap->m_oStatistics.Api.m_nCheckLibraryVersion++;  break;
    case AC_CopyBuffer:          gpAttachMap->m_oStatistics.Api.m_nCopyBuffer++;           break;
    case AC_EndInsertTable:      gpAttachMap->m_oStatistics.Api.m_nEndInsertJobs++;        break;
    case AC_EndJobs:             gpAttachMap->m_oStatistics.Api.m_nEndJobs++;              break;
    case AC_EndSubInserter:      gpAttachMap->m_oStatistics.Api.m_nEndSubInserter++;       break;
    case AC_ExecCmdStr:          gpAttachMap->m_oStatistics.Api.m_nExecCmdStr++;           break;
    case AC_ExecuteCommand:      gpAttachMap->m_oStatistics.Api.m_nExecuteCommand++;       break;
    case AC_FetchNextBuffer:     gpAttachMap->m_oStatistics.Api.m_nFetchNextBuffer++;      break;
    case AC_GetRecordBuffer:     gpAttachMap->m_oStatistics.Api.m_nGetRecordBuffer++;      break;
    case AC_GetErrorMessage:     gpAttachMap->m_oStatistics.Api.m_nGetErrorMessage++;      break;
    case AC_GetRow:              gpAttachMap->m_oStatistics.Api.m_nGetRow++;               break;
    case AC_GetRowCount:         gpAttachMap->m_oStatistics.Api.m_nGetRowCount++;          break;
    case AC_GetRowCountTableColumn: gpAttachMap->m_oStatistics.Api.m_nGetRowCountTableColumn++;  break;
    case AC_GetRowTableColumn:   gpAttachMap->m_oStatistics.Api.m_nGetRowTableColumn++;    break;
    case AC_InsertRow:           gpAttachMap->m_oStatistics.Api.m_nInsertRow++;            break;
    case AC_LoadResult:          gpAttachMap->m_oStatistics.Api.m_nLoadResult++;           break;
    case AC_PoolOpen:            gpAttachMap->m_oStatistics.Api.m_nPoolOpen++;             break;
    case AC_ScanNextRow:         gpAttachMap->m_oStatistics.Api.m_nScanNextRow++;          break;
    case AC_SessionDelete:       gpAttachMap->m_oStatistics.Api.m_nSessionDelete++;        break;
    case AC_SessionInvalid:      gpAttachMap->m_oStatistics.Api.m_nSessionInvalid++;       break;
    case AC_SessionNew:          gpAttachMap->m_oStatistics.Api.m_nSessionNew++;           break;
    default:
      NEVER_REACH;
  }
}

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
