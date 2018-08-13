/*============================================================================*/
/* Job_Receiver.cpp -- Socket Receiver Job                                    */
/*============================================================================*/

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobNote mJobNote_Receiver (
  "Job_Receiver"_Stz,    // name of the job class
  CN_JOB_RECEIVER,       // class enum
  JK_CHILD,              // job kind
  "Socket Receiver"_Stz, // comment about the job
  nullptr,               // the prompt command
  nullptr,               // the second word
  "Receiver_seed"_Stz,   // the task seed for this job
  false,                 // check resources?
  false,                 // detached job?
  JT_NONE,               // job trace
  nullptr,               // prompt pre-process function
  nullptr,               // prompt post-process function
  nullptr,               // job new function
  vJob_Receiver_delete,  // job delete function
  vJob_Receiver_show,    // job show function
  false,                 // job is not prompt enabled
  "<help>"_Stz,          // help
  "<example>"_Stz        // example
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Receiver* JobsModel::_pJob_Receiver(void* aAddress)
{
  Job_Receiver* pJob_Receiver = reinterpret_cast<Job_Receiver*>(aAddress);
  
  RASSERT( !pJob_Receiver || CN_JOB_RECEIVER == pJob_Receiver->eClass());
  
  return pJob_Receiver;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Receiver::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_Receiver* pJob_Receiver =
    static_cast<Job_Receiver*>(Heap::pAllocate(CN_JOB_RECEIVER));
  
  return pJob_Receiver;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Receiver::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Receiver::operator delete(void* aAddress)
{
  Job_Receiver* pJob_Receiver = static_cast<Job_Receiver*>(aAddress);
  
  Heap::vFree(pJob_Receiver);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Receiver::Job_Receiver(Interface::SEitf* pSEitf)
: Job(CN_JOB_RECEIVER, pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_RECEIVER,_FOOTER_);
  
  m_sSeed = mJobNote_Receiver.pJobClass()->m_aSeed;
  
  _checkInitialised_(CN_JOB_RECEIVER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_RECEIVER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Receiver::~Job_Receiver(void)
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
  RASSERT(CN_JOB_RECEIVER == eClass);
  RASSERT(pObject->eClass() == CN_JOB_RECEIVER);
  
  Job_Receiver* pJob_Receiver = _pJob_Receiver(pObject);

  pJob_Receiver->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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
void JobsModel::vJob_Receiver_delete(Interface::SEitf* pSEitf, Job* pJob)
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
