/*============================================================================*/
/* Job_Repeater.cpp -- Repeater Job                                           */
/*============================================================================*/

// Job Short: Repeater

#include "JobsModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_Repeater =
{
  "Job_Repeater"_Stz,    // name of the job class
  CN_JOB_REPEATER,       // class enum
  JK_LOCAL,              // job kind
  "repeat data"_Stz,     // comment about the job
  "repeater"_Stz,        // the prompt command
  nullptr,               // the second word
  nullptr,               // the _run task seed
  "repeater_req"_Stz,    // the _req task seed
  nullptr,               // the _rcv task seed
  nullptr,               // the _exe task seed
  nullptr,               // the _end task seed
  nullptr,               // the _can task seed
  false,                 // check resources?
  false,                 // detached job?
  false,                 // buffer attached?
  JT_NONE,               // job trace
  pJob_Repeater_pre,     // prompt pre-process function
  vJob_Repeater_post,    // prompt post-process function
  pJob_Repeater_new,     // job new function
  vJob_Repeater_delete,  // job delete function
  nullptr,               // job pre-send function
  nullptr,               // job post-receive function
  vJob_Repeater_show,    // job show function
  
  // help and example
  true,
  "repeater <fileNm> <pipeNm>"_Stz,
  "repeater \"/rapids/SensorPipe\" \"/rapids/SensorPipe\";"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* JobsModel::pJob_Repeater_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_Sanity_class;
  
  return &xJobClass_Repeater;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Repeater* JobsModel::_pJob_Repeater(void* aAddress)
{
  Job_Repeater* pJob_Repeater = reinterpret_cast<Job_Repeater*>(aAddress);
  
  RASSERT_TYPE(pJob_Repeater,CN_JOB_REPEATER);
  
  return pJob_Repeater;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Repeater::operator new(size_t)
{
  Job_Repeater* pJob_Repeater =
    static_cast<Job_Repeater*>(HeapMem::pAllocate(CN_JOB_REPEATER));
  
  return pJob_Repeater;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Repeater::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Repeater::operator delete(void* aAddress)
{
  Job_Repeater* pJob_Repeater = static_cast<Job_Repeater*>(aAddress);
  
  HeapMem::vFree(pJob_Repeater);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Repeater::Job_Repeater(Interface::Session* pSession)
: Job(CN_JOB_REPEATER, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_REPEATER,_FOOTER_);
  
  m_sPipeName = "<none>"_Stz;
  //m_oFile
  //m_xPipe
  
  m_xWsp.vFill(0xEF,CN_JOB_REPEATER);

  _checkInitialised_(CN_JOB_REPEATER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_REPEATER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Repeater::~Job_Repeater(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_REPEATER,_FOOTER_);
  
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Repeater::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_Repeater* pJob_Repeater = _pJob_Repeater(pObject);

  pJob_Repeater->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Repeater::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_REPEATER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Repeater::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_REPEATER);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_Repeater_pre(Interface::Session* pSession)
{
  i8 nTokens;

  Job_Repeater* pJob_Repeater = new Job_Repeater(pSession);

  TokenVec* pTknVec = pSession->m_pCommandProcessor->pTknVec();

  // get no of tokens in token vector
  nTokens = pTknVec->nTokens();

  // there should be 3 tokens
  if (nTokens < 3)
    return pJob_Repeater;

  pJob_Repeater->m_sFileName = pTknVec->pGet(1)->aText();
  pJob_Repeater->m_sPipeName = pTknVec->pGet(2)->aText();

  return pJob_Repeater;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_Repeater_post(Interface::Session* pSession, Job* pJob)
{
}

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_Repeater_new(Job* pArg)
{
  Job_Repeater* pJob_Repeater = new Job_Repeater(nullptr);
  
  return pJob_Repeater;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_Repeater_delete(Interface::Session* pSession, Job* pJob)
{
  Job_Repeater* pJob_Repeater = _pJob_Repeater(pJob);
  
  delete pJob_Repeater;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_Repeater_show(Job* pJob)
{
  Job_Repeater* pJob_Repeater = _pJob_Repeater(pJob);

  pJob_Repeater->vShow();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* indefinitely copy file into pipe                                           */
/*----------------------------------------------------------------------------*/
void Job_Repeater::vIndefiniteCopy(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_REPEATER,_FOOTER_);
  
  i8 nBytesRead;
  
  for (;;) {
    
    m_oFile.vSeek(0);
    
    while ( (nBytesRead = m_oFile.iReadBytes(m_xWsp.aArea(),C_REPEATER_WSP)) ) {
      bool bWritten = m_xPipe.bWriteBytes(m_xWsp.aArea(),nBytesRead);
      ASSERT(bWritten);
    }
  }
}

/*----------------------------------------------------------------------------*/
/* repeatedly copy file into pipe                                             */
/*----------------------------------------------------------------------------*/
void Job_Repeater::vRepeat(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_REPEATER,_FOOTER_);
  
  vTrace("Repeating data from %s into %s\n"_Stz,
         m_sFileName.aStr(), m_sPipeName.aStr());
  
  bool bFileOpened = m_oFile.bOpen(m_sFileName,C_OPEN_READ);
  if (! bFileOpened) {
    vTrace("Unable to open file: %s\n"_Stz,m_sFileName.aStr());
    return;
  }
  
  bool bPipeOpened = m_xPipe.bOpen(m_sPipeName,C_OPEN_WRITE);
  if (! bPipeOpened) {
    vTrace("Unable to open pipe: %s\n"_Stz,m_sPipeName.aStr());
    return;
  }

  vIndefiniteCopy();
  
  NEVER_REACH;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
