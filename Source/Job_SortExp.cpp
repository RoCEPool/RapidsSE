/*============================================================================*/
/* Job_SortExp.cpp -- Query Job                                                 */
/*============================================================================*/

#include "Jobs.h"

JobClass xJobClass_Sort =
{
  CN_JOB_SORTEXP,        // class number
  JK_LOCAL,              // job kind
  RK_LOCAL_ROWS,         // response kind
  "sort"_Stz,            // comment about the job
  "sort"_Stz,            // the prompt command
  nullptr,               // the second word
  nullptr,               // the _run task seed
  "sortExp_req"_Stz,     // the _req task seed
  nullptr,               // the _rcv task seed
  nullptr,               // the _exe task seed
  nullptr,               // the _end task seed
  nullptr,               // the _can task seed
  false,                 // check resources?
  false,                 // detached job?
  false,                 // buffer attached?
  JT_NONE,               // job trace
  DL_GRAB_READ,          // ddl lock model
  PY_NORMAL,             // priority
  JF_NONE,               // flags

  nullptr,               // prompt check function
  pJob_SortExp_pre,      // prompt pre-process function
  vJob_post,             // prompt post-process function
  nullptr,               // job new function
  vJob_SortExp_delete,   // job delete function
  nullptr,               // job pre-send function
  nullptr,               // job post-receive function
  vJob_SortExp_show,     // job show function
  
  // help and example
  true,
  "sort"_Stz,
  "sort;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_SortExp_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_Space_class;
  
  return &xJobClass_Sort;
}

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_SortExp* Jobs::_pJob_SortExp(void* aAddress)
{
  Job_SortExp* pJob_SortExp = reinterpret_cast<Job_SortExp*>(aAddress);
  
  RASSERT_TYPE(pJob_SortExp,CN_JOB_SORTEXP);
  
  return pJob_SortExp;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_SortExp::operator new(size_t)
{
  Job_SortExp* pJob_SortExp =
    static_cast<Job_SortExp*>(HeapMem::pAllocate(CN_JOB_SORTEXP));
  
  return pJob_SortExp;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_SortExp::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_SortExp::operator delete(void* aAddress)
{
  Job_SortExp* pJob_SortExp = static_cast<Job_SortExp*>(aAddress);
  
  HeapMem::vFree(pJob_SortExp);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_SortExp::Job_SortExp(Session* pSession)
: JobMark2(CN_JOB_SORTEXP, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SORTEXP,_FOOTER_);
  
  m_iBuffers = 0;
  m_pDigest = new Digest(this);
  
  m_pGenerateOp = nullptr;
  m_pSortOp     = nullptr;
  m_pMergeOp    = nullptr;
  m_pOutputOp   = nullptr;
  m_pZedOp      = nullptr;

  _checkInitialised_(CN_JOB_SORTEXP);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_SORTEXP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_SortExp::~Job_SortExp(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SORTEXP,_FOOTER_);
  
  delete m_pGenerateOp;
  delete m_pSortOp;
  delete m_pMergeOp;
  delete m_pOutputOp;
  delete m_pZedOp;
  
  delete m_pDigest;
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_SortExp::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_SortExp* pJob_SortExp = _pJob_SortExp(pObject);

  pJob_SortExp->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_SortExp::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SORTEXP,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("sort"_Stz));
  
  // - - - - - - - - - - - - - - - - - - - - -
  // get no of tokens in token vector
  iNumTokens = pTokenVec->nTokens();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be only 2 tokens
  if (iNumTokens != 2) {
    return nullptr;
  }
  
  // - - - - - - - - - - - - - - - - - - - - -
  // set the number of buffers to generate/sort
  m_iBuffers = pTokenVec->pGet(1)->iToI8();

  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_SortExp::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SORTEXP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* sort begin                                                                 */
/*----------------------------------------------------------------------------*/
void Job_SortExp::vBegin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SORTEXP,_FOOTER_);
  
  AM::vSetAcksRmg(m_pExecutingTask,5);

  m_pGenerateOp->vOpOut(m_pSortOp);
  m_pSortOp->vOpOut(m_pMergeOp);
  m_pMergeOp->vOpOut(m_pOutputOp);
  m_pOutputOp->vOpOut(m_pZedOp);
  m_pZedOp->vOpOut(nullptr);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  //TablePtn* pTablePtn = m_pTable->pPartition(0);
  //m_pSortOp->vTablePtn(pTablePtn);
}

/*----------------------------------------------------------------------------*/
/* sort end                                                                   */
/*----------------------------------------------------------------------------*/
void Job_SortExp::vEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SORTEXP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* sort execute                                                               */
/*----------------------------------------------------------------------------*/
void Job_SortExp::vExecute(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SORTEXP,_FOOTER_);
  
  Session* pSession = AM::pThisTLSSession();
  
  bool bSpawned;
  
  bSpawned = AM::bSpawnOp(pSession,m_pZedOp);
  ASSERT(bSpawned);
  bSpawned = AM::bSpawnOp(pSession,m_pOutputOp);
  ASSERT(bSpawned);
  bSpawned = AM::bSpawnOp(pSession,m_pMergeOp);
  ASSERT(bSpawned);
  bSpawned = AM::bSpawnOp(pSession,m_pSortOp);
  ASSERT(bSpawned);
  bSpawned = AM::bSpawnOp(pSession,m_pGenerateOp);
  ASSERT(bSpawned);
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_SortExp::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SORTEXP);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_SortExp_pre(Session* pSession)
{
  Job_SortExp* pJob_SortExp = new Job_SortExp(pSession);
  
  TokenVec* pTokenVec = pSession->m_pCommand->pTknVec();
  
  bool bResult = pJob_SortExp->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    pJob_SortExp->vAddError(EC_INVALID_COMMAND);
  }
  
  pJob_SortExp->m_pGenerateOp = new GenerateOp("G_0"_Stz,pJob_SortExp->m_pDigest, pJob_SortExp->m_iBuffers);
  pJob_SortExp->m_pSortOp     = new SortOp    ("S_0"_Stz,pJob_SortExp->m_pDigest);
  pJob_SortExp->m_pMergeOp    = new MergeOp   ("M_0"_Stz,pJob_SortExp->m_pDigest);
  pJob_SortExp->m_pOutputOp   = new OutputOp  ("O_0"_Stz,pJob_SortExp->m_pDigest,nullptr,false);
  pJob_SortExp->m_pZedOp      = new ZedOp     ("Z_0"_Stz,pJob_SortExp->m_pDigest,nullptr,false);
  
  return pJob_SortExp;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_SortExp_delete(Session* pSession, Job* pJob)
{
  Job_SortExp* pJob_SortExp = _pJob_SortExp(pJob);
  
  delete pJob_SortExp;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_SortExp_show(Job* pJob)
{
  Job_SortExp* pJob_SortExp = _pJob_SortExp(pJob);

  pJob_SortExp->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
