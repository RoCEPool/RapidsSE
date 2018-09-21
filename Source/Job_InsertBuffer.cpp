/*============================================================================*/
/* Job_InsertBuffer.cpp -- Insert Row Job                                        */
/*============================================================================*/

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/* example:
 
 ld lineitem "/tpch/lineitem.tbl" ;
 
 */

JobClass xJobClass_InsertBuffer =
{
  "Job_InsertRow"_Stz,    // name of the job class
  CN_JOB_INSERTBUFFER,    // class enum
  JK_DIST2PC,             // job kind
  "insert buffer"_Stz,    // comment about the job
  nullptr,                // the prompt command
  nullptr,                // the second word
  "insertBuffer_run"_Stz, // the _run task seed
  nullptr,                // the _req task seed
  "insertBuffer_kid"_Stz, // the _rcv task seed
  nullptr,                // the _exe task seed
  nullptr,                // the _end task seed
  nullptr,                // the _can task seed
  false,                  // check resources?
  false,                  // detached job?
  true,                   // buffer attached?
  JT_ALL,                 // job trace
  pJob_InsertBuffer_pre,  // prompt pre-process function
  nullptr,                // prompt post-process function
  pJob_InsertBuffer_new,   // job new function
  vJob_InsertBuffer_delete,// job delete function
  vJob_send,               // job pre-send function
  vJob_recv,               // job post-receive function
  vJob_InsertBuffer_show,  // job show function
  false,                   // job is not prompt enabled
  "<help>"_Stz,            // help
  "<example>"_Stz,         // example

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* JobsModel::pJob_InsertBuffer_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_InsertRow_class;
  
  return &xJobClass_InsertBuffer;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_InsertBuffer* JobsModel::_pJob_InsertBuffer(void* aAddress)
{
  Job_InsertBuffer* pJob_InsertBuffer = reinterpret_cast<Job_InsertBuffer*>(aAddress);
  
  //RASSERT( !pJob_InsertBuffer || CN_JOB_INSERTBUFFER == pJob_InsertBuffer->eClass());
  
  return pJob_InsertBuffer;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_InsertBuffer::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_InsertBuffer* pJob_InsertBuffer =
    static_cast<Job_InsertBuffer*>(Heap::pAllocate(CN_JOB_INSERTBUFFER));
  
  return pJob_InsertBuffer;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_InsertBuffer::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_InsertBuffer::operator delete(void* aAddress)
{
  Job_InsertBuffer* pJob_InsertBuffer = static_cast<Job_InsertBuffer*>(aAddress);
  
  Heap::vFree(pJob_InsertBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_InsertBuffer::Job_InsertBuffer(Interface::SEitf* pSEitf)
: Job(CN_JOB_INSERTBUFFER, pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INSERTBUFFER,_FOOTER_);
  
  m_pTable          = nullptr;
  m_pTask           = nullptr;
  m_pWorkingBuffer  = nullptr;
  m_iPartitionId    = -1;
  m_iEndInsertJobId = -1;
  m_nBuffersToBeInserted = C_UNKNOWN;
  
  _checkInitialised_(CN_JOB_INSERTBUFFER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_INSERTBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_InsertBuffer::~Job_InsertBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INSERTBUFFER,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_InsertBuffer::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_INSERTBUFFER == eClass);
  RASSERT(pObject->eClass() == CN_JOB_INSERTBUFFER);
  
  Job_InsertBuffer* pJob_InsertBuffer = _pJob_InsertBuffer(pObject);

  pJob_InsertBuffer->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_InsertBuffer::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INSERTBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_InsertBuffer::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INSERTBUFFER);
  
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
Job* JobsModel::pJob_InsertBuffer_pre(Interface::SEitf* pSEitf)
{
  Job_InsertBuffer* pJob_InsertBuffer = new Job_InsertBuffer(pSEitf);
  
  return pJob_InsertBuffer;
}

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_InsertBuffer_new(Job* pArg)
{
  Job_InsertBuffer* pJob_InsertBuffer = new Job_InsertBuffer(nullptr);
  
  return pJob_InsertBuffer;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_InsertBuffer_delete(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_InsertBuffer* pJob_InsertBuffer = _pJob_InsertBuffer(pJob);
  
  vTrace("DELETING INSERTBUFFER\n"_Stz);
  delete pJob_InsertBuffer;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_InsertBuffer_show(Job* pJob)
{
  Job_InsertBuffer* pJob_InsertBuffer = _pJob_InsertBuffer(pJob);

  pJob_InsertBuffer->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
