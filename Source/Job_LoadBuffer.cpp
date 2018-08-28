/*============================================================================*/
/* Job_LoadBuffer.cpp -- Load Buffer Job                                      */
/*============================================================================*/

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobNote mJobNote_LoadBuffer (
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
  JT_ALL,                  // job trace
  vJob_LoadBuffer_pre,     // prompt pre-process function
  nullptr,                 // prompt post-process function
  pJob_LoadBuffer_new,     // job new function
  vJob_LoadBuffer_delete,  // job delete function
  vJob_send,               // job pre-send function
  vJob_recv,               // job post-receive function
  vJob_LoadBuffer_show,    // job show function
  false,                   // job is not prompt enabled
  "<help>"_Stz,            // help
  "<example>"_Stz          // example
);

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
    static_cast<Job_LoadBuffer*>(Heap::pAllocate(CN_JOB_LOADBUFFER));
  
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
  
  Heap::vFree(pJob_LoadBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_LoadBuffer::Job_LoadBuffer(Interface::SEitf* pSEitf)
: Job(CN_JOB_LOADBUFFER, pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOADBUFFER,_FOOTER_);
  
  //m_sSeed = mJobNote_LoadBuffer.pJobClass()->m_aSeedReq;
  
  m_nBuffersLoadable = C_UNKNOWN;
  m_iLoadJobId = -1;
  m_iPartition = -1;
  m_pTable = nullptr;
  
  _checkInitialised_(CN_JOB_LOADBUFFER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOADBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_LoadBuffer::~Job_LoadBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOADBUFFER,_FOOTER_);
  
  //delete m_pWorkingBuffer;
  //m_pWorkingBuffer = nullptr;
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
i8 Job_LoadBuffer::nBuffersLoadable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOADBUFFER,_FOOTER_);
  
  return m_nBuffersLoadable;
}

/*----------------------------------------------------------------------------*/
/* get number of buffers loadable                                             */
/*----------------------------------------------------------------------------*/
void Job_LoadBuffer::vBuffersLoadable(i8 nBuffersLoadable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOADBUFFER,_FOOTER_);
  
  m_nBuffersLoadable = nBuffersLoadable;
}

/*----------------------------------------------------------------------------*/
/* get the job id of the load table job                                       */
/*----------------------------------------------------------------------------*/
i8 Job_LoadBuffer::iLoadJobId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOADBUFFER,_FOOTER_);
  
  return m_iLoadJobId;
}

/*----------------------------------------------------------------------------*/
/* set the job id of the load table job                                       */
/*----------------------------------------------------------------------------*/
void Job_LoadBuffer::vLoadJobId(i8 iLoadJobId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOADBUFFER,_FOOTER_);
  
  m_iLoadJobId = iLoadJobId;
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
  
  sShow.vSprint("LoadJobId %lld; Table %s; Partition %lld; Rows %lld\n"_Stz,
                m_iLoadJobId,
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
Job* JobsModel::vJob_LoadBuffer_pre(Interface::SEitf* pSEitf)
{
  Job_LoadBuffer* pJob_LoadBuffer = new Job_LoadBuffer(pSEitf);
  
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
void JobsModel::vJob_LoadBuffer_delete(Interface::SEitf* pSEitf, Job* pJob)
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
