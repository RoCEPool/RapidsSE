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
  false,                   // job is not prompt enabled
  "<help>"_Stz,            // help
  "<example>"_Stz,         // example

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
  
  RASSERT( !pJob_LoadBuffer || CN_JOB_LOADBUFFER == pJob_LoadBuffer->eClass());
  
  return pJob_LoadBuffer;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_LoadBuffer::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_LoadBuffer* pJob_LoadBuffer =
    static_cast<Job_LoadBuffer*>(HeapMem::pAllocate(CN_JOB_LOADBUFFER));
  
  return pJob_LoadBuffer;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_LoadBuffer::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
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
    
  m_iJobLoadTableId = -1;
  m_iPartition = -1;
  m_nBuffersFromSubLoader = C_UNKNOWN;

  m_pJob_LoadTable = nullptr;
  
  m_pTable = nullptr;
  m_bEndOfBuffers = false;
  m_nRows = 0;
  m_bLowMem = false;

  _checkInitialised_(CN_JOB_LOADBUFFER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOADBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_LoadBuffer::~Job_LoadBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOADBUFFER,_FOOTER_);
  
  delete m_pWorkingBuffer;
  m_pWorkingBuffer = nullptr;
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_LoadBuffer::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_LOADBUFFER == eClass);
  RASSERT(pObject->eClass() == CN_JOB_LOADBUFFER);
  
  Job_LoadBuffer* pJob_LoadBuffer = _pJob_LoadBuffer(pObject);

  pJob_LoadBuffer->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get number of buffers loadable                                             */
/*----------------------------------------------------------------------------*/
i8 Job_LoadBuffer::nBuffersFromSubLoader(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOADBUFFER,_FOOTER_);
  
  return m_nBuffersFromSubLoader;
}

/*----------------------------------------------------------------------------*/
/* get number of buffers from the subloader which are to loaded               */
/*----------------------------------------------------------------------------*/
void Job_LoadBuffer::vBuffersFromSubLoader(i8 nBuffers)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOADBUFFER,_FOOTER_);
  
  m_nBuffersFromSubLoader = nBuffers;
}

/*----------------------------------------------------------------------------*/
/* get the load table job                                                     */
/*----------------------------------------------------------------------------*/
Job_LoadTable* Job_LoadBuffer::pJobLoadTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOADBUFFER,_FOOTER_);
  
  return m_pJob_LoadTable;
}

/*----------------------------------------------------------------------------*/
/* set the load table job                                                     */
/*----------------------------------------------------------------------------*/
void Job_LoadBuffer::vJobLoadTable(Job_LoadTable* pJobLoadTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOADBUFFER,_FOOTER_);
  
  m_pJob_LoadTable = pJobLoadTable;
}

/*----------------------------------------------------------------------------*/
/* get the job id of the load table job                                       */
/*----------------------------------------------------------------------------*/
i8 Job_LoadBuffer::iJobLoadTableId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOADBUFFER,_FOOTER_);
  
  return m_iJobLoadTableId;
}

/*----------------------------------------------------------------------------*/
/* set the job id of the load table job                                       */
/*----------------------------------------------------------------------------*/
void Job_LoadBuffer::vJobLoadTableId(i8 iJobId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOADBUFFER,_FOOTER_);
  
  m_iJobLoadTableId = iJobId;
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
                m_iJobLoadTableId,
                m_sObjectName.aStr(),
                m_iPartition,
                m_pWorkingBuffer->nWorkingRows());
  
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
