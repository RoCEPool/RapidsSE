/*============================================================================*/
/* Job_WriteBuffer.cpp -- write buffer Job                                    */
/*============================================================================*/

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/* example:
 
 ld lineitem "/tpch/SF1/lineitem.csv" ;
 
 */

JobClass xJobClass_WriteBuffer =
{
  "Job_WriteBuffer"_Stz,  // name of the job class
  CN_JOB_WRITEBUFFER,     // class enum
  JK_DIST2PC,             // job kind
  "write buffer"_Stz,     // comment about the job
  nullptr,                // the prompt command
  nullptr,                // the second word
  "writeBuffer_run"_Stz,  // the _run task seed
  nullptr,                // the _req task seed
  "writeBuffer_kid"_Stz,  // the _rcv task seed
  nullptr,                // the _exe task seed
  nullptr,                // the _end task seed
  nullptr,                // the _can task seed
  false,                  // check resources?
  false,                  // detached job?
  true,                   // buffer attached?
  JT_NONE,                // job trace
  pJob_WriteBuffer_pre,   // prompt pre-process function
  nullptr,                // prompt post-process function
  pJob_WriteBuffer_new,   // job new function
  vJob_WriteBuffer_delete,// job delete function
  vJob_send,              // job pre-send function
  vJob_recv,              // job post-receive function
  vJob_WriteBuffer_show,  // job show function
  
  // help and example
  false,
  "<none>"_Stz,
  "<none>"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* JobModel::pJob_WriteBuffer_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_WriteRow_class;
  
  return &xJobClass_WriteBuffer;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_WriteBuffer* JobModel::_pJob_WriteBuffer(void* aAddress)
{
  Job_WriteBuffer* pJob_WriteBuffer = reinterpret_cast<Job_WriteBuffer*>(aAddress);
  
  RASSERT_TYPE(pJob_WriteBuffer,CN_JOB_WRITEBUFFER);
  
  return pJob_WriteBuffer;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_WriteBuffer::operator new(size_t)
{
  Job_WriteBuffer* pJob_WriteBuffer =
    static_cast<Job_WriteBuffer*>(HeapMem::pAllocate(CN_JOB_WRITEBUFFER));
  
  return pJob_WriteBuffer;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_WriteBuffer::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_WriteBuffer::operator delete(void* aAddress)
{
  Job_WriteBuffer* pJob_WriteBuffer = static_cast<Job_WriteBuffer*>(aAddress);
  //vTrace("DELETING INSERTBUFFER\n"_Stz);
  HeapMem::vFree(pJob_WriteBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_WriteBuffer::Job_WriteBuffer(Interface::Session* pSession)
: Job(CN_JOB_WRITEBUFFER, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_WRITEBUFFER,_FOOTER_);
  
  m_eDataSource  = DS_NONE;
  m_pTable       = nullptr;
  m_iPartitionId = C_UNKNOWN;
  m_iParentJobId = C_UNKNOWN;
  m_nRows        = 0;
  m_nWriteBehindBuffers = C_UNKNOWN;
  
  m_bLowMem      = false;
  
  _checkInitialised_(CN_JOB_WRITEBUFFER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_WRITEBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_WriteBuffer::~Job_WriteBuffer(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_WRITEBUFFER,_FOOTER_);
  
  delete m_pCurrentBuffer;
  m_pCurrentBuffer = nullptr;
}

/*----------------------------------------------------------------------------*/
/* insert rows from working buffer into table                                 */
/*----------------------------------------------------------------------------*/
bool Job_WriteBuffer::bInsertRows(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_WRITEBUFFER,_FOOTER_);

  // ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! !
  
  if (! m_pCurrentBuffer) {
    bool bAllBuffersWritten = bInsertRows_LowMem();
    return bAllBuffersWritten;
  }
  
  // ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! !
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // convert partition to node slot
  i8 iSlot = TablePtn::iPartitionToSlot(m_iPartitionId);

  // get table, partition and working buffer
  RASSERT_TYPE(m_pTable,CN_TABLE);

  TablePtn* pPartition = m_pTable->pPartition(iSlot);
  RASSERT_TYPE(pPartition,CN_TABLEPTN);

  RASSERT_TYPE(m_pCurrentBuffer,CN_WORKINGBUFFER);

//  if (gpAttachMap->m_bVerbose)
//    vTrace("Job_WriteBuffer %lld %lld %s\n"_Stz,
//           m_pCurrentBuffer->nWorkingRows(),
//           m_nRows,
//           m_pCurrentBuffer->bIsLast() ? "IsLast" : "");
  i8 iTxId = 0;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // insert the working buffer
  m_pTable->vInsertEntries(iTxId, iSlot, m_pCurrentBuffer);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // we inserted one additional buffer
  pPartition->vIncBuffersWritten();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Loading ?
  if (m_eDataSource == DS_LOAD) {
//    Job__Find* pFindJob = new Job__Find;
//    pFindJob->m_iJobId = m_iParentJobId;
//
//    gpAttachMap->m_pParticipatingJobs->vGrabLock();
//    Job* pJob = gpAttachMap->m_pParticipatingJobs->pFindJob(pFindJob);
//    gpAttachMap->m_pParticipatingJobs->vDropLock();
//
//    delete pFindJob;
//
//    Job_LoadTable* pJob_LoadTable = static_cast<Job_LoadTable*>(pJob);
//
//    // add the number of rows into rows processed
//    i8 iThisNode = gpAttachMap->m_pGrid->m_iThisNode;
//    pJob_LoadTable->m_xNode[iThisNode].m_ziRowsProcessed += m_nRows;
    
    if (C_UNKNOWN != m_nWriteBehindBuffers)
      pPartition->vAddWriteBehindBuffers(m_nWriteBehindBuffers);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Insering ?
  else if (m_eDataSource == DS_INSERT) {
    // does this job have information about the number of buffers to be written
    // from a subloader for this partition?
    if (C_UNKNOWN != m_nWriteBehindBuffers) {
      pPartition->vWriteBehindBuffers(m_nWriteBehindBuffers);
    }
    
    if (m_bMark) {
      ActivityMap::vMark(m_iJobId,"Setting m_nWriteBehindBuffers in partition %lld slot %lld to %lld"_Stz,
                         m_iPartitionId, iSlot, m_nWriteBehindBuffers);
    }
  }
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Ingesting
  else {
    RASSERT(m_eDataSource == DS_INGEST);
    
    // this runs forever
    return false;
  }
  
  bool bAllBuffersWritten = pPartition->bAllBuffersWritten(m_eDataSource);
  
  if (bAllBuffersWritten) {
    if (m_bMark) {
      ActivityMap::vMark(m_iJobId,"All buffers written for partition %lld"_Stz, m_iPartitionId);
    }
  }
  
  return bAllBuffersWritten;
}

/*----------------------------------------------------------------------------*/
/* insert rows while under low memory condition                               */
/*----------------------------------------------------------------------------*/
bool Job_WriteBuffer::bInsertRows_LowMem(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_WRITEBUFFER,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (gpAttachMap->m_bVerbose)
    vTrace("Job_WriteBuffer (LowMem) \n"_Stz);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // convert partition to node slot
  i8 iSlot = TablePtn::iPartitionToSlot(m_iPartitionId);
  
  // get table, partition and working buffer
  TablePtn* pPartition = m_pTable->pPartition(iSlot);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // find the load table job
  if (m_eDataSource == DS_LOAD) {
//    Job__Find* pFindJob = new Job__Find;
//    pFindJob->m_iJobId = m_iParentJobId;
//
//    gpAttachMap->m_pParticipatingJobs->vGrabLock();
//    Job* pJob = gpAttachMap->m_pParticipatingJobs->pFindJob(pFindJob);
//    gpAttachMap->m_pParticipatingJobs->vDropLock();
//
//    delete pFindJob;
    
    Job* pJobFound = gpAttachMap->m_pParticipatingJobs->pFindJob(m_iParentJobId);

    Job_LoadTable* pJob_LoadTable = static_cast<Job_LoadTable*>(pJobFound);

    // add the number of rows into rows skipped
    i8 iThisNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_LoadTable->m_xNode[iThisNode].m_ziRowsSkipped += m_nRows;
  
    if (gpAttachMap->m_bVerbose)
      vTrace("m_iRowsSkipped %lld m_nRows %lld\n"_Stz,
             *pJob_LoadTable->m_xNode[iThisNode].m_ziRowsSkipped,
             m_nRows);
  }
  
  // we wrote one additional buffer
  pPartition->vIncBuffersWritten();
  
  // does this job have information about the number of buffers loadable
  // from a subloader for this partition?
  if (C_UNKNOWN != m_nWriteBehindBuffers)
    pPartition->vAddWriteBehindBuffers(m_nWriteBehindBuffers);
  
  // have all buffers been loaded?
  bool bAllBuffersWritten = pPartition->bAllBuffersWritten(DS_LOAD);
  //if (bAllBuffersLoaded)
  //  vTrace("Buffers loaded\n"_Stz);
  
  return bAllBuffersWritten;
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_WriteBuffer::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_WriteBuffer* pJob_WriteBuffer = _pJob_WriteBuffer(pObject);

  pJob_WriteBuffer->vShow();
}

/*----------------------------------------------------------------------------*/
/* get partition being loaded                                                 */
/*----------------------------------------------------------------------------*/
void Job_WriteBuffer::vSpawnJobWriteBuffer(Session* pSession,
                                                     Stz* sTableName,
                                                     eDS eDataSource,
                                                     WorkingBuffer* pWorkingBuffer,
                                                     i8 iParentJobId,
                                                     i8 iPartition,
                                                     bool bFinal,
                                                     i8 nWriteBehindBuffers)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a write buffer job
  
  //! this needs to be optimised
  JobClass* pJobClass_WriteBuffer = JobClass::pFind(CN_JOB_WRITEBUFFER);
  
  Job_WriteBuffer* pJob_WriteBuffer =
  static_cast<Job_WriteBuffer*>(pJobClass_WriteBuffer->pPreJob(pSession));
  
  pJob_WriteBuffer->vSeed(SS_SEED_RUN);
  
  pJob_WriteBuffer->m_eDataSource  = eDataSource;
  
  // DS_INSERT/DS_LOAD should have a known parent job id
  RASSERT(DS_INGEST == eDataSource || C_UNKNOWN != iParentJobId);
  
  pJob_WriteBuffer->m_iParentJobId  = iParentJobId;
  pJob_WriteBuffer->m_sObjectName   = sTableName;
  pJob_WriteBuffer->m_iPartitionId  = iPartition;
  pJob_WriteBuffer->m_pCurrentBuffer = pWorkingBuffer;
  pJob_WriteBuffer->m_nRows = pWorkingBuffer->nWorkingRows();
  
  if (bFinal) {
    ASSERT(C_UNKNOWN != nWriteBehindBuffers);
    pJob_WriteBuffer->m_nWriteBehindBuffers = nWriteBehindBuffers;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // spawn the write buffer job
  
  RapidsSE_bSpawnJob(pJob_WriteBuffer,SS_SEED_RUN,pSession);
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_WriteBuffer::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_WRITEBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_WriteBuffer::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_WRITEBUFFER);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobModel::pJob_WriteBuffer_pre(Interface::Session* pSession)
{
  Job_WriteBuffer* pJob_WriteBuffer = new Job_WriteBuffer(pSession);
  
  return pJob_WriteBuffer;
}

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* JobModel::pJob_WriteBuffer_new(Job* pArg)
{
  Job_WriteBuffer* pJob_WriteBuffer = new Job_WriteBuffer(nullptr);
  
  return pJob_WriteBuffer;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobModel::vJob_WriteBuffer_delete(Interface::Session* pSession, Job* pJob)
{
  Job_WriteBuffer* pJob_WriteBuffer = _pJob_WriteBuffer(pJob);
  
  //vTrace("DELETING INSERTBUFFER\n"_Stz);
  delete pJob_WriteBuffer;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobModel::vJob_WriteBuffer_show(Job* pJob)
{
  Job_WriteBuffer* pJob_WriteBuffer = _pJob_WriteBuffer(pJob);

  pJob_WriteBuffer->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
