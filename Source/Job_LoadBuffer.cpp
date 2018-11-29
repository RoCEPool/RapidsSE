/*============================================================================*/
/* Job_LoadBuffer.cpp -- Load Buffer Job                                      */
/*============================================================================*/

// Job Short: LoadBuffer

#include "JobsModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_LoadBuffer =
{
  "Job_LoadBuffer"_Stz,    // name of the job class
  CN_JOB_LOADBUFFER,       // class enum
  JK_GRANDKID,             // job kind
  "table load buffer"_Stz, // comment about the job
  nullptr,                 // the prompt command
  nullptr,                 // the second word
  "loadBuffer_run"_Stz,    // the _run task seed
  nullptr,                 // the _req task seed
  "loadBuffer_kid"_Stz,    // the _rcv task seed
  nullptr,                 // the _exe task seed
  nullptr,                 // the _end task seed
  nullptr,                 // the _can task seed
  false,                   // check resources?
  false,                   // detached job?
  true,                    // buffer attached?
  JT_NONE,                 // job trace
  vJob_LoadBuffer_pre,     // prompt pre-process function
  nullptr,                 // prompt post-process function
  pJob_LoadBuffer_new,     // job new function
  vJob_LoadBuffer_delete,  // job delete function
  vJob_send,               // job pre-send function
  vJob_recv,               // job post-receive function
  vJob_LoadBuffer_show,    // job show function
  
  false,
  "<none>"_Stz,
  "<none>"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* JobsModel::pJob_LoadBuffer_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_LoadTable_class;
  
  return &xJobClass_LoadBuffer;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_LoadBuffer* JobsModel::_pJob_LoadBuffer(void* aAddress)
{
  Job_LoadBuffer* pJob_LoadBuffer = reinterpret_cast<Job_LoadBuffer*>(aAddress);
  
  RASSERT_TYPE(pJob_LoadBuffer,CN_JOB_LOADBUFFER);
  
  return pJob_LoadBuffer;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_LoadBuffer::operator new(size_t)
{
  Job_LoadBuffer* pJob_LoadBuffer =
    static_cast<Job_LoadBuffer*>(HeapMem::pAllocate(CN_JOB_LOADBUFFER));
  
  return pJob_LoadBuffer;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_LoadBuffer::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_LoadBuffer::operator delete(void* aAddress)
{
  Job_LoadBuffer* pJob_LoadBuffer = static_cast<Job_LoadBuffer*>(aAddress);
  
  HeapMem::vFree(pJob_LoadBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_LoadBuffer::Job_LoadBuffer(Interface::Session* pSession)
: Job(CN_JOB_LOADBUFFER, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOADBUFFER,_FOOTER_);
    
  m_iParentJobId = -1;
  m_iPartition   = -1;
  m_nWriteBehindBuffers = C_UNKNOWN;
  
  m_pTable = nullptr;
  m_nRows = 0;
  m_bLowMem = false;

  _checkInitialised_(CN_JOB_LOADBUFFER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOADBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_LoadBuffer::~Job_LoadBuffer(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOADBUFFER,_FOOTER_);
  
  delete m_pCurrentBuffer;
  m_pCurrentBuffer = nullptr;
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_LoadBuffer::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_LoadBuffer* pJob_LoadBuffer = _pJob_LoadBuffer(pObject);

  pJob_LoadBuffer->vShow();
}

/*----------------------------------------------------------------------------*/
/* get partition being loaded                                                 */
/*----------------------------------------------------------------------------*/
Job_LoadBuffer* Job_LoadBuffer::pNewJobLoadBuffer(Job_SubLoader* pJob_SubLoader, bool bFinal)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get info

  Job_LoadTable* pJob_LoadTable = pJob_SubLoader->pJob_LoadTable();

  i8 iJobLoadTableId = pJob_LoadTable->iJobId();
  i8 iPartition = pJob_SubLoader->m_iPartition;
  WorkingBuffer* pWorkingBuffer = pJob_SubLoader->m_pWorkingBuffer[iPartition];
  pJob_SubLoader->m_pWorkingBuffer[iPartition] = nullptr;

  Session* pSession = pJob_SubLoader->m_pSession;

  //ASSERT( (  pWorkingBuffer->bIsLast() && 0 == pWorkingBuffer->nWorkingRows()) ||
  //        ( !pWorkingBuffer->bIsLast() && pWorkingBuffer->nWorkingRows()));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a load buffer job

  //! this needs to be optimised
  JobClass* pJobClass_LoadBuffer = JobClass::pFind(CN_JOB_LOADBUFFER);

  Job_LoadBuffer* pJob_LoadBuffer =
  static_cast<Job_LoadBuffer*>(pJobClass_LoadBuffer->pPreJob(pSession));

  pJob_LoadBuffer->vSeed(SS_SEED_RUN);

  pJob_LoadBuffer->m_iParentJobId = iJobLoadTableId;
  pJob_LoadBuffer->m_sObjectName = pJob_SubLoader->m_sObjectName;
  pJob_LoadBuffer->vPartition(iPartition);
  pJob_LoadBuffer->m_pCurrentBuffer = pWorkingBuffer;
  pJob_LoadBuffer->m_nRows = pWorkingBuffer->nWorkingRows();

  if (bFinal) {
    i8 nWriteBehindBuffers = pJob_SubLoader->m_nWriteBehindBuffers[iPartition];
    
    ASSERT(C_UNKNOWN != nWriteBehindBuffers);
    pJob_LoadBuffer->vWriteBehindBuffers(nWriteBehindBuffers);
  }

  return pJob_LoadBuffer;
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get number of buffers loadable                                             */
/*----------------------------------------------------------------------------*/
i8 Job_LoadBuffer::nWriteBehindBuffers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOADBUFFER,_FOOTER_);
  
  return m_nWriteBehindBuffers;
}

/*----------------------------------------------------------------------------*/
/* get number of buffers from the subloader which are to loaded               */
/*----------------------------------------------------------------------------*/
void Job_LoadBuffer::vWriteBehindBuffers(i8 nBuffers)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOADBUFFER,_FOOTER_);
  
  m_nWriteBehindBuffers = nBuffers;
}

/*----------------------------------------------------------------------------*/
/* insert rows from working buffer into table                                 */
/*----------------------------------------------------------------------------*/
bool Job_LoadBuffer::bInsertRows(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOADBUFFER,_FOOTER_);
  
  // ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! !
  
  if (! m_pCurrentBuffer) {
    bool bAllBuffersLoaded = bInsertRows_LowMem();
    return bAllBuffersLoaded;
  }
  
  // ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! !

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // find the load table job
  
  Job__Find* pFindJob = new Job__Find;
  pFindJob->m_iJobId = m_iParentJobId;
  
  gpAttachMap->m_pParticipatingJobs->vGrabLock();
  Job* pJob = gpAttachMap->m_pParticipatingJobs->pFindJob(pFindJob);
  gpAttachMap->m_pParticipatingJobs->vDropLock();
  
  delete pFindJob;
  
  Job_LoadTable* pJob_LoadTable = static_cast<Job_LoadTable*>(pJob);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // convert partition to node slot
  i8 iSlot = TablePtn::iPartitionToSlot(m_iPartition);
  
  // get table, partition and working buffer
  TablePtn* pPartition = m_pTable->pPartition(iSlot);
  
  if (gpAttachMap->m_bVerbose)
    vTrace("Job_LoadBuffer %lld %lld %s\n"_Stz,
           m_pCurrentBuffer->nWorkingRows(),
           m_nRows,
           m_pCurrentBuffer->bIsLast() ? "IsLast" : "");
  
  i8 iTxId = 0;
  
  // insert the working buffer
  m_pTable->vInsertEntries(iTxId, iSlot, m_pCurrentBuffer);
  
  // we loaded one additional buffer from a subloader
  pPartition->vIncBuffersWritten();
  
  // add the number of rows into rows processed
  i8 iThisNode = gpAttachMap->m_pGrid->m_iThisNode;
  pJob_LoadTable->m_xNode[iThisNode].m_ziRowsProcessed += m_nRows;
  
  // does this job have information about the number of buffers loadable
  // from a subloader for this partition?
  if (C_UNKNOWN != m_nWriteBehindBuffers)
    pPartition->vAddWriteBehindBuffers(m_nWriteBehindBuffers);
  
  // have all buffers been loaded?
  bool bAllBuffersLoaded = pPartition->bAllBuffersLoaded();
  //if (bAllBuffersLoaded)
  //  vTrace("Buffers loaded\n"_Stz);
  
  return bAllBuffersLoaded;
}

/*----------------------------------------------------------------------------*/
/* insert rows while under low memory condition                               */
/*----------------------------------------------------------------------------*/
bool Job_LoadBuffer::bInsertRows_LowMem(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOADBUFFER,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (gpAttachMap->m_bVerbose)
    vTrace("Job_LoadBuffer (LowMem) \n"_Stz);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // find the load table job
  
  Job__Find* pFindJob = new Job__Find;
  pFindJob->m_iJobId = m_iParentJobId;
  
  gpAttachMap->m_pParticipatingJobs->vGrabLock();
  Job* pJob = gpAttachMap->m_pParticipatingJobs->pFindJob(pFindJob);
  gpAttachMap->m_pParticipatingJobs->vDropLock();
  
  delete pFindJob;

  Job_LoadTable* pJob_LoadTable = static_cast<Job_LoadTable*>(pJob);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


  // convert partition to node slot
  i8 iSlot = TablePtn::iPartitionToSlot(m_iPartition);
  
  // get table, partition and working buffer
  TablePtn* pPartition = m_pTable->pPartition(iSlot);
  
  // add the number of rows into rows skipped
  i8 iThisNode = gpAttachMap->m_pGrid->m_iThisNode;
  pJob_LoadTable->m_xNode[iThisNode].m_ziRowsSkipped += m_nRows;
  
  if (gpAttachMap->m_bVerbose)
    vTrace("m_iRowsSkipped %lld m_nRows %lld\n"_Stz,
           *pJob_LoadTable->m_xNode[iThisNode].m_ziRowsSkipped,
           m_nRows);

  // we loaded one additional buffer from a subloader
  pPartition->vIncBuffersWritten();
  
  // does this job have information about the number of buffers loadable
  // from a subloader for this partition?
  if (C_UNKNOWN != m_nWriteBehindBuffers)
    pPartition->vAddWriteBehindBuffers(m_nWriteBehindBuffers);
  
  // have all buffers been loaded?
  bool bAllBuffersLoaded = pPartition->bAllBuffersLoaded();
  //if (bAllBuffersLoaded)
  //  vTrace("Buffers loaded\n"_Stz);
  
  return bAllBuffersLoaded;
}

/*----------------------------------------------------------------------------*/
/* get partition being loaded                                                 */
/*----------------------------------------------------------------------------*/
i8 Job_LoadBuffer::iPartition(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOADBUFFER,_FOOTER_);
  
  return m_iPartition;
}

/*----------------------------------------------------------------------------*/
/* set partition being loaded                                                 */
/*----------------------------------------------------------------------------*/
void Job_LoadBuffer::vPartition(i8 iPartition)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOADBUFFER,_FOOTER_);
  
  m_iPartition = iPartition;
}

/*----------------------------------------------------------------------------*/
/* get table being loaded                                                     */
/*----------------------------------------------------------------------------*/
Table* Job_LoadBuffer::pTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOADBUFFER,_FOOTER_);
  
  return m_pTable;
}

/*----------------------------------------------------------------------------*/
/* set table being loaded                                                     */
/*----------------------------------------------------------------------------*/
void Job_LoadBuffer::vTable(Table* pTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOADBUFFER,_FOOTER_);
  
  m_pTable = pTable;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_LoadBuffer::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOADBUFFER,_FOOTER_);
  
  s48K sShow;
  
  sShow.vSprint("JobLoadTableId %lld; Table %s; Partition %lld; Rows %lld\n"_Stz,
                m_iParentJobId,
                m_sObjectName.aStr(),
                m_iPartition,
                m_pCurrentBuffer->nWorkingRows());
  
  Job::vShow(sShow);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_LoadBuffer::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOADBUFFER);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobsModel::vJob_LoadBuffer_pre(Interface::Session* pSession)
{
  Job_LoadBuffer* pJob_LoadBuffer = new Job_LoadBuffer(pSession);
  
  return pJob_LoadBuffer;
}

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_LoadBuffer_new(Job* pArg)
{
  Job_LoadBuffer* pJob_LoadBuffer = new Job_LoadBuffer(nullptr);
  
  return pJob_LoadBuffer;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_LoadBuffer_delete(Interface::Session* pSession, Job* pJob)
{
  Job_LoadBuffer* pJob_LoadBuffer = _pJob_LoadBuffer(pJob);

  delete pJob_LoadBuffer;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_LoadBuffer_show(Job* pJob)
{
  Job_LoadBuffer* pJob_LoadBuffer = _pJob_LoadBuffer(pJob);

  pJob_LoadBuffer->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
