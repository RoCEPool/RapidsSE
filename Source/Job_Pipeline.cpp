/*============================================================================*/
/* Job_Pipeline.cpp -- Pipeline Job                                           */
/*============================================================================*/

#include "Jobs.h"

JobClass xJobClass_Pipeline =
{
  CN_JOB_PIPELINE,       // class number
  JK_DIST2PC,            // job kind
  RS_QUEUE_ROWS,         // response kind
  "pipeline"_Stz,        // comment about the job
  "pipeline"_Stz,        // the prompt command
  nullptr,               // the second word
  nullptr,               // the _run task seed
  "pipeline_req"_Stz,    // the _req task seed
  "pipeline_beg"_Stz,    // the _rcv task seed
  "pipeline_exe"_Stz,    // the _exe task seed
  "pipeline_end"_Stz,    // the _end task seed
  "pipeline_can"_Stz,    // the _can task seed
  false,                 // check resources?
  false,                 // detached job?
  false,                 // buffer attached?
  DL_GRAB_READ,          // ddl lock model
  PY_NORMAL,             // priority
  JF_NONE,               // flags

  eJob_checkCluster,     // prompt check function
  pJob_Pipeline_pre,     // prompt pre-process function
  vJob_post,             // prompt post-process function
  nullptr,               // job new function
  vJob_Pipeline_delete,  // job delete function
  vJob_send,             // job pre-send function
  vJob_recv,             // job post-receive function
  vJob_Pipeline_show,    // job show function
  
  // help and example
  true,
  "pipeline"_Stz,
  "pipeline;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_Pipeline_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_Receiver_class;

  return &xJobClass_Pipeline;
}

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Pipeline* Jobs::_pJob_Pipeline(void* aAddress)
{
  Job_Pipeline* pJob_Pipeline = reinterpret_cast<Job_Pipeline*>(aAddress);
  
  RASSERT_TYPE(pJob_Pipeline,CN_JOB_PIPELINE);
  
  return pJob_Pipeline;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* Job_Pipeline::operator new(size_t)
{
  Job_Pipeline* pJob_Pipeline =
    static_cast<Job_Pipeline*>(HeapMem::pAllocate(CN_JOB_PIPELINE));
  
  return pJob_Pipeline;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* Job_Pipeline::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void Job_Pipeline::operator delete(void* aAddress)
{
  Job_Pipeline* pJob_Pipeline = static_cast<Job_Pipeline*>(aAddress);
  
  HeapMem::vFree(pJob_Pipeline);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Pipeline::Job_Pipeline(JobClass* aJobClass, ReqTuple* aReqTuple)
: Job(aJobClass, aReqTuple)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_PIPELINE,_FOOTER_);
  
  //m_oOpVec
  
  _checkInitialised_(CN_JOB_PIPELINE);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_PIPELINE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Pipeline::~Job_Pipeline(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_PIPELINE,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Pipeline::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_Pipeline* pJob_Pipeline = _pJob_Pipeline(pObject);

  pJob_Pipeline->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Pipeline::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_PIPELINE);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_Pipeline_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession)
{
  Job_Pipeline* pJob_Pipeline = new Job_Pipeline(aJobClass,aReqTuple);
  
  return pJob_Pipeline;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Pipeline_delete(Job* pJob)
{
  Job_Pipeline* pJob_Pipeline = _pJob_Pipeline(pJob);
  
  delete pJob_Pipeline;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Pipeline_show(Job* pJob)
{
  Job_Pipeline* pJob_Pipeline = _pJob_Pipeline(pJob);

  pJob_Pipeline->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
