/*============================================================================*/
/* Job_Connector.cpp -- Socket/Node Cross Connector Job                       */
/*============================================================================*/

#include "Jobs.h"

JobClass xJobClass_Connector =
{
  CN_JOB_CONNECTOR,       // class number
  JK_SERVICE,              // job kind
  RS_NONE,                // response kind
  "socket connector"_Stz, // comment about the job
  nullptr,                // the prompt command
  nullptr,                // the second word
  "connector_run"_Stz,    // the _run task seed
  nullptr,                // the _req task seed
  nullptr,                // the _rcv task seed
  nullptr,                // the _exe task seed
  nullptr,                // the _end task seed
  nullptr,                // the _can task seed
  false,                  // check resources?
  false,                  // detached job?
  false,                  // buffer attached?
  DL_NONE,                // ddl lock model
  PY_NORMAL,              // priority
  JF_NONE,                // flags

  eJob_checkCluster,      // prompt check function
  nullptr,                // prompt pre-process function
  nullptr,                // prompt post-process function
  pJob_Connector_new,     // job new function
  vJob_Connector_delete,  // job delete function
  nullptr,                // job pre-send function
  nullptr,                // job post-receive function
  vJob_Connector_show,    // job show function
  
  // help and example
  false,
 C_NO_NAME,
  C_NO_NAME,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_Connector_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_CopyBuffer_class;
  
  return &xJobClass_Connector;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Connector* Jobs::_pJob_Connector(void* aAddress)
{
  Job_Connector* pJob_Connector = reinterpret_cast<Job_Connector*>(aAddress);
  
  RASSERT_TYPE(pJob_Connector,CN_JOB_CONNECTOR);
  
  return pJob_Connector;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Connector::operator new(size_t)
{
  Job_Connector* pJob_Connector =
    static_cast<Job_Connector*>(HeapMem::pAllocate(CN_JOB_CONNECTOR));
  
  return pJob_Connector;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Connector::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Connector::operator delete(void* aAddress)
{
  Job_Connector* pJob_Connector = static_cast<Job_Connector*>(aAddress);
  
  HeapMem::vFree(pJob_Connector);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Connector::Job_Connector(JobClass* aJobClass, ReqTuple* aReqTuple)
: Job(aJobClass, aReqTuple)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CONNECTOR,_FOOTER_);
  
  _checkInitialised_(CN_JOB_CONNECTOR);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_CONNECTOR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Connector::~Job_Connector(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CONNECTOR,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Connector::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_Connector* pJob_Connector = _pJob_Connector(pObject);

  pJob_Connector->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Connector::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBLOADER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Connector::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CONNECTOR);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_Connector_new(JobClass* aJobClass, ReqTuple* aReqTuple)
{
  ReqTuple xReqTuple("**CONNECTOR"_Stz,false);
  
  Job_Connector* pJob_Connector = new Job_Connector(aJobClass,&xReqTuple);
  
  return pJob_Connector;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Connector_delete(Job* pJob)
{
  Job_Connector* pJob_Connector = _pJob_Connector(pJob);
  
  delete pJob_Connector;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Connector_show(Job* pJob)
{
  Job_Connector* pJob_Connector = _pJob_Connector(pJob);

  pJob_Connector->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
