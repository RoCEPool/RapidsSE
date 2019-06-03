/*============================================================================*/
/* Job_Cat.cpp -- Cat Job                                                     */
/*============================================================================*/

// Job Short: Cat

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_Cat =
{
  CN_JOB_CAT,         // class enum
  JK_LOCAL,           // job kind
  RK_NONE,            // response kind
  "cat data"_Stz,     // comment about the job
  "cat"_Stz,          // the prompt command
  nullptr,            // the second word
  nullptr,            // the _run task seed
  "cat_req"_Stz,      // the _req task seed
  nullptr,             // the _rcv task seed
  nullptr,             // the _exe task seed
  nullptr,             // the _end task seed
  nullptr,             // the _can task seed
  false,               // check resources?
  false,               // detached job?
  false,               // buffer attached?
  JT_NONE,             // job trace
  DL_NONE,             // ddl lock model
  PY_NORMAL,           // priority
  JF_NONE,             // flags

  eJob_checkCluster,   // prompt check function
  pJob_Cat_pre,        // prompt pre-process function
  vJob_post,           // prompt post-process function
  pJob_Cat_new,        // job new function
  vJob_Cat_delete,     // job delete function
  nullptr,             // job pre-send function
  nullptr,             // job post-receive function
  vJob_Cat_show,       // job show function
  
  // help and example
  true,
  "cat <inPipeNm> <outPipeNm>"_Stz,
  "cat \"/rapids/SensorPipe\" \"/rapids/Stage1Pipe\";"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_Cat_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_Cf_class;
  
  return &xJobClass_Cat;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Cat* Jobs::_pJob_Cat(void* aAddress)
{
  Job_Cat* pJob_Cat = reinterpret_cast<Job_Cat*>(aAddress);
  
  RASSERT_TYPE(pJob_Cat,CN_JOB_CAT);
  
  return pJob_Cat;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Cat::operator new(size_t)
{
  Job_Cat* pJob_Cat =
    static_cast<Job_Cat*>(HeapMem::pAllocate(CN_JOB_CAT));
  
  return pJob_Cat;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Cat::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Cat::operator delete(void* aAddress)
{
  Job_Cat* pJob_Cat = static_cast<Job_Cat*>(aAddress);
  
  HeapMem::vFree(pJob_Cat);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Cat::Job_Cat(Interface::Session* pSession)
: Job(CN_JOB_CAT, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CAT,_FOOTER_);
  
  m_pIoCopy     = new IoCopy;
  m_pByteRate   = nullptr;
  m_bStatistics = false;
  
  _checkInitialised_(CN_JOB_CAT);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_CAT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Cat::~Job_Cat(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CAT,_FOOTER_);
  
  delete m_pIoCopy;
  delete m_pByteRate;
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Cat::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_Cat* pJob_Cat = _pJob_Cat(pObject);

  pJob_Cat->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Cat::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CAT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Cat::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CAT);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_Cat_pre(Interface::Session* pSession)
{
  i8 nTokens;

  Job_Cat* pJob_Cat = new Job_Cat(pSession);

  TokenVec* pTknVec = pSession->m_pCommand->pTknVec();

  // get no of tokens in token vector
  nTokens = pTknVec->nTokens();

  // there should be at least 3 tokens
  if (nTokens < 3)
    return pJob_Cat;

  // set the input file name
  pJob_Cat->m_sFilePath = pTknVec->pGet(1)->sText();
  
  // set the output file name
  pJob_Cat->m_sOutputPath = pTknVec->pGet(2)->sText();
  
  // is the last token the print option
  i8 i_Last = nTokens - 1;
  if (pTknVec->pGet(i_Last)->sText().bSame("stats"_Stz) ||
      pTknVec->pGet(i_Last)->sText().bSame("statistics"_Stz))
    pJob_Cat->m_bStatistics = true;
  
  return pJob_Cat;
}

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_Cat_new(Job* pArg)
{
  Job_Cat* pJob_Cat = new Job_Cat(nullptr);
  
  return pJob_Cat;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Cat_delete(Interface::Session* pSession, Job* pJob)
{
  Job_Cat* pJob_Cat = _pJob_Cat(pJob);
  
  delete pJob_Cat;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Cat_show(Job* pJob)
{
  Job_Cat* pJob_Cat = _pJob_Cat(pJob);

  pJob_Cat->vShow();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* begin cat                                                                  */
/*----------------------------------------------------------------------------*/
bool Job_Cat::bBegin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CAT,_FOOTER_);
  
  if (! m_pIoCopy->bOpenRead(m_sFilePath))
    return false;
  
  if (! m_pIoCopy->bOpenWrite(m_sOutputPath))
    return false;
  
  if (m_bStatistics) {
    m_pByteRate = new ByteRate;
    m_pByteRate->vSetReporting("  cat : "_Stz, C_1GB * 1);
    m_pByteRate->vBeginTiming();

  }

  vTrace("Cat data from %s to %s\n"_Stz,m_sFilePath.aStz(),m_sOutputPath.aStz());

  return true;
}

/*----------------------------------------------------------------------------*/
/* copy from input to output                                                  */
/*----------------------------------------------------------------------------*/
bool Job_Cat::bChunk(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CAT,_FOOTER_);

  i8 nBytesCopied = m_pIoCopy->nCopyChunk();

  if (nBytesCopied) {
    if (m_pByteRate)
      m_pByteRate->vAddBytes(nBytesCopied);
    return true;
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* end cat                                                                    */
/*----------------------------------------------------------------------------*/
void Job_Cat::vEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CAT,_FOOTER_);
  
  if (m_pByteRate)
    m_pByteRate->vEndTiming();

   m_pIoCopy->vCloseBoth();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
