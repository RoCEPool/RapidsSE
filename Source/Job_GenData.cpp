/*============================================================================*/
/* Job_GenData.cpp -- Generate Data Job                                       */
/*============================================================================*/

// Job Short: GenData

#include "JobsModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_GenData =
{
  "Job_GenData"_Stz,     // name of the job class
  CN_JOB_GENDATA,        // class enum
  JK_LOCAL,              // job kind
  "generate data"_Stz,   // comment about the job
  "gendata"_Stz,         // the prompt command
  nullptr,               // the second word
  nullptr,               // the _run task seed
  "genData_req"_Stz,     // the _req task seed
  "genData_rcv"_Stz,     // the _rcv task seed
  nullptr,               // the _exe task seed
  nullptr,               // the _end task seed
  nullptr,               // the _can task seed
  false,                 // check resources?
  false,                 // detached job?
  false,                 // buffer attached?
  JT_NONE,               // job trace
  pJob_GenData_pre,      // prompt pre-process function
  vJob_GenData_post,     // prompt post-process function
  pJob_GenData_new,      // job new function
  vJob_GenData_delete,   // job delete function
  vJob_send,             // job pre-send function
  vJob_recv,             // job post-receive function
  vJob_GenData_show,     // job show function
  
  // help and example
  true,
  "gendata <tableNm>"_Stz,
  "gendata X0_T1_Z0;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* JobsModel::pJob_GenData_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_InsertBuffer_class;
  
  return &xJobClass_GenData;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_GenData* JobsModel::_pJob_GenData(void* aAddress)
{
  Job_GenData* pJob_GenData = reinterpret_cast<Job_GenData*>(aAddress);
  
  RASSERT_TYPE(pJob_GenData,CN_JOB_GENDATA);
  
  return pJob_GenData;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_GenData::operator new(size_t)
{
  Job_GenData* pJob_GenData =
    static_cast<Job_GenData*>(HeapMem::pAllocate(CN_JOB_GENDATA));
  
  return pJob_GenData;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_GenData::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_GenData::operator delete(void* aAddress)
{
  Job_GenData* pJob_GenData = static_cast<Job_GenData*>(aAddress);
  
  HeapMem::vFree(pJob_GenData);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_GenData::Job_GenData(Interface::Session* pSession)
: Job(CN_JOB_GENDATA, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENDATA,_FOOTER_);
  
  _checkInitialised_(CN_JOB_GENDATA);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENDATA,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_GenData::~Job_GenData(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENDATA,_FOOTER_);
  
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_GenData::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_GenData* pJob_GenData = _pJob_GenData(pObject);

  pJob_GenData->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_GenData::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENDATA,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_GenData::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENDATA);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_GenData_pre(Interface::Session* pSession)
{
  i8 nTokens;

  Job_GenData* pJob_GenData = new Job_GenData(pSession);
  
  TokenVec* pTknVec = pSession->m_pCommandProcessor->pTknVec();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // get no of tokens in token vector
  nTokens = pTknVec->nTokens();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 2 tokens
  if (nTokens != 2)
    return nullptr;
  
  // set the table name
  pJob_GenData->m_sObjectName = pTknVec->pGet(1)->aText();

  return pJob_GenData;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_GenData_post(Interface::Session* pSession, Job* pJob)
{
}

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_GenData_new(Job* pArg)
{
  Job_GenData* pJob_GenData = new Job_GenData(nullptr);
  
  return pJob_GenData;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_GenData_delete(Interface::Session* pSession, Job* pJob)
{
  Job_GenData* pJob_GenData = _pJob_GenData(pJob);
  
  delete pJob_GenData;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_GenData_show(Job* pJob)
{
  Job_GenData* pJob_GenData = _pJob_GenData(pJob);

  pJob_GenData->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
