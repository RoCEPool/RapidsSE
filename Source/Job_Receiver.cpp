/*============================================================================*/
/* Job_Receiver.cpp -- Socket Receiver Job                                    */
/*============================================================================*/

// Job Short: Receiver

#include "JobsModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_Receiver =
{
  "Job_Receiver"_Stz,    // name of the job class
  CN_JOB_RECEIVER,       // class enum
  JK_SYSTEM,             // job kind
  "socket receiver"_Stz, // comment about the job
  nullptr,               // the prompt command
  nullptr,               // the second word
  "receiver_run"_Stz,    // the _run task seed
  nullptr,               // the _req task seed
  nullptr,               // the _rcv task seed
  nullptr,               // the _exe task seed
  nullptr,               // the _end task seed
  nullptr,               // the _can task seed
  false,                 // check resources?
  false,                 // detached job?
  false,                 // buffer attached?
  JT_NONE,               // job trace
  nullptr,               // prompt pre-process function
  nullptr,               // prompt post-process function
  pJob_Receiver_new,     // job new function
  vJob_Receiver_delete,  // job delete function
  nullptr,               // job pre-send function
  nullptr,               // job post-receive function
  vJob_Receiver_show,    // job show function
  
  // help and example
  false,
  "<none>"_Stz,
  "<none>"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* JobsModel::pJob_Receiver_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_Repeater_class;
  
  return &xJobClass_Receiver;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Receiver* JobsModel::_pJob_Receiver(void* aAddress)
{
  Job_Receiver* pJob_Receiver = reinterpret_cast<Job_Receiver*>(aAddress);
  
  RASSERT_TYPE(pJob_Receiver,CN_JOB_RECEIVER);
  
  return pJob_Receiver;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Receiver::operator new(size_t)
{
  Job_Receiver* pJob_Receiver =
    static_cast<Job_Receiver*>(HeapMem::pAllocate(CN_JOB_RECEIVER));
  
  return pJob_Receiver;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Receiver::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Receiver::operator delete(void* aAddress)
{
  Job_Receiver* pJob_Receiver = static_cast<Job_Receiver*>(aAddress);
  
  HeapMem::vFree(pJob_Receiver);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Receiver::Job_Receiver(Interface::Session* pSession)
: Job(CN_JOB_RECEIVER, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_RECEIVER,_FOOTER_);
    
  m_iConnNo  = -1;
  m_pNode    = nullptr;
  
  _checkInitialised_(CN_JOB_RECEIVER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_RECEIVER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Receiver::~Job_Receiver(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_RECEIVER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Receiver::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_Receiver* pJob_Receiver = _pJob_Receiver(pObject);

  pJob_Receiver->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get connection number                                                      */
/*----------------------------------------------------------------------------*/
i8 Job_Receiver::iConnNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_RECEIVER,_FOOTER_);
  
  return m_iConnNo;
}

/*----------------------------------------------------------------------------*/
/* set connection number                                                      */
/*----------------------------------------------------------------------------*/
void Job_Receiver::vConnNo(i8 iConnNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_RECEIVER,_FOOTER_);
  
  m_iConnNo = iConnNo;
}

/*----------------------------------------------------------------------------*/
/* get node                                                                   */
/*----------------------------------------------------------------------------*/
GridModel::Node* Job_Receiver::pNode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_RECEIVER,_FOOTER_);
  
  return m_pNode;
}

/*----------------------------------------------------------------------------*/
/* set node                                                                   */
/*----------------------------------------------------------------------------*/
void Job_Receiver::vNode(GridModel::Node* pNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_RECEIVER,_FOOTER_);
  
  m_pNode = pNode;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Receiver::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBLOADER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Receiver::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_RECEIVER);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_Receiver_new(Job* pArg)
{
  Job_Receiver* pJob_Receiver = new Job_Receiver(nullptr);
  
  return pJob_Receiver;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_Receiver_delete(Interface::Session* pSession, Job* pJob)
{
  Job_Receiver* pJob_Receiver = _pJob_Receiver(pJob);
  
  delete pJob_Receiver;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_Receiver_show(Job* pJob)
{
  Job_Receiver* pJob_Receiver = _pJob_Receiver(pJob);

  pJob_Receiver->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
