/*============================================================================*/
/* Job_WriteBuffer.cpp -- write buffer Job                                    */
/*============================================================================*/

#include "JobsModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/* example:
 
 ld lineitem "/tpch/lineitem.tbl" ;
 
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
JobClass* JobsModel::pJob_WriteBuffer_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)nullptr;
  
  return &xJobClass_WriteBuffer;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_WriteBuffer* JobsModel::_pJob_WriteBuffer(void* aAddress)
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
  m_iPartitionId = -1;
  m_iParentJobId = -1;
  m_nWriteBehindBuffers = C_UNKNOWN;
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOADBUFFER,_FOOTER_);

  // convert partition to node slot
  i8 iSlot = TablePtn::iPartitionToSlot(m_iPartitionId);

  // get table, partition and working buffer
  RASSERT_TYPE(m_pTable,CN_TABLE);

  TablePtn* pPartition = m_pTable->pPartition(iSlot);
  RASSERT_TYPE(pPartition,CN_TABLEPTN);

  RASSERT_TYPE(m_pCurrentBuffer,CN_WORKINGBUFFER);

  i8 iTxId = 0;

  // insert the working buffer
  m_pTable->vInsertEntries(iTxId, iSlot, m_pCurrentBuffer);

  // we inserted one additional buffer
  pPartition->vIncBuffersWritten();

  // does this job have information about the number of buffers loadable
  // from a subloader for this partition?
  if (C_UNKNOWN != m_nWriteBehindBuffers) {
    pPartition->vWriteBehindBuffers(m_nWriteBehindBuffers);
    pPartition->vParentJobId(m_iParentJobId);
    
    if (m_bMark) {
      ActivityMap::vMark(m_iJobId,"Setting m_nWriteBehindBuffers in partition %lld slot %lld to %lld"_Stz,
                         m_iPartitionId, iSlot, m_nWriteBehindBuffers);
      
      ActivityMap::vMark(m_iJobId,"Setting ParentJobId in partition %lld to %lld"_Stz,
                         m_iPartitionId, m_iParentJobId);
    }
  }

  // get the parentJobId
  m_iParentJobId = pPartition->iParentJobId();
  
  bool bAllDone = pPartition->bAllBuffersLoaded(m_eDataSource);
  
  if (bAllDone) {
    if (m_bMark) {
      ActivityMap::vMark(m_iJobId,"All buffers written for partition %lld"_Stz, m_iPartitionId);
    }
  }
  
  return bAllDone;
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
Job* JobsModel::pJob_WriteBuffer_pre(Interface::Session* pSession)
{
  Job_WriteBuffer* pJob_WriteBuffer = new Job_WriteBuffer(pSession);
  
  return pJob_WriteBuffer;
}

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_WriteBuffer_new(Job* pArg)
{
  Job_WriteBuffer* pJob_WriteBuffer = new Job_WriteBuffer(nullptr);
  
  return pJob_WriteBuffer;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_WriteBuffer_delete(Interface::Session* pSession, Job* pJob)
{
  Job_WriteBuffer* pJob_WriteBuffer = _pJob_WriteBuffer(pJob);
  
  //vTrace("DELETING INSERTBUFFER\n"_Stz);
  delete pJob_WriteBuffer;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_WriteBuffer_show(Job* pJob)
{
  Job_WriteBuffer* pJob_WriteBuffer = _pJob_WriteBuffer(pJob);

  pJob_WriteBuffer->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
