/*============================================================================*/
/* Job_NodeInfo.cpp -- Node Information Job                                   */
/*============================================================================*/

#include "Jobs.h"

JobClass xJobClass_NodeInfo =
{
  CN_JOB_NODEINFO,   // class number
  JK_INFO,           // job kind
  "node info"_Stz,   // comment about the job
  nullptr,           // the prompt command
  nullptr,           // the second word
  nullptr,           // the _run task seed
  nullptr,           // the _req task seed
  nullptr,           // the _exe task seed
  nullptr,           // the _rcv task seed
  nullptr,           // the _end task seed
  nullptr,           // the _can task seed
  false,             // check resources?
  false,             // detached job?
  false,             // buffer attached?
  DL_NONE,           // ddl lock model
  PY_NORMAL,         // priority
  JF_NONE,           // flags
  
  nullptr,           // prompt check function
  nullptr,           // prompt pre-process function
  nullptr,           // prompt post-process function
  pJob_NodeInfo_new,     // job new function
  vJob_NodeInfo_delete,  // job delete function
  vJob_send,         // job pre-send function
  vJob_recv,         // job post-receive function
  vJob_NodeInfo_show,    // job show function
  
  // help and example
  false,
  C_NO_NAME,
  C_NO_NAME,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_NodeInfo_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_Operand_class;
  
  return &xJobClass_NodeInfo;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_NodeInfo* Jobs::_pJob_NodeInfo(void* aAddress)
{
  Job_NodeInfo* pJob_NodeInfo = reinterpret_cast<Job_NodeInfo*>(aAddress);
  
  RASSERT_TYPE(pJob_NodeInfo,CN_JOB_NODEINFO);
  
  return pJob_NodeInfo;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_NodeInfo::operator new(size_t)
{
  Job_NodeInfo* pJob_NodeInfo =
    static_cast<Job_NodeInfo*>(HeapMem::pAllocate(CN_JOB_NODEINFO));
  
  return pJob_NodeInfo;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_NodeInfo::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_NodeInfo::operator delete(void* aAddress)
{
  Job_NodeInfo* pJob_NodeInfo = static_cast<Job_NodeInfo*>(aAddress);
  
  HeapMem::vFree(pJob_NodeInfo);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_NodeInfo::Job_NodeInfo(JobClass* aJobClass, ReqTuple* aReqTuple)
: Job(aJobClass, aReqTuple)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_NODEINFO,_FOOTER_);
  
  m_sNodeName = AM::szNodeNm();
  
  _checkInitialised_(CN_JOB_NODEINFO);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_NODEINFO,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_NodeInfo::~Job_NodeInfo(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_NODEINFO,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_NodeInfo::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_NodeInfo* pJob_NodeInfo = _pJob_NodeInfo(pObject);

  pJob_NodeInfo->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_NodeInfo::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_NODEINFO,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_NodeInfo::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_NODEINFO);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_NodeInfo_new(JobClass* aJobClass, ReqTuple* aReqTuple)
{
  ReqTuple xReqTuple("**NODE_INFO"_Stz,false);
  
  Job_NodeInfo* pJob_NodeInfo = new Job_NodeInfo(aJobClass,&xReqTuple);
  
  return pJob_NodeInfo;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_NodeInfo_delete(Job* pJob)
{
  Job_NodeInfo* pJob_NodeInfo = _pJob_NodeInfo(pJob);
  
  delete pJob_NodeInfo;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_NodeInfo_show(Job* pJob)
{
  Job_NodeInfo* pJob_NodeInfo= _pJob_NodeInfo(pJob);

  pJob_NodeInfo->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
