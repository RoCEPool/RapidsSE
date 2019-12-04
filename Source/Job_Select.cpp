/*============================================================================*/
/* Job_Select.cpp -- Select Statement Job                                     */
/*============================================================================*/

#include "Jobs.h"

JobClass xJobClass_Select =
{
  CN_JOB_SELECT,             // class number
  JK_META,                   // job kind
  RS_NONE,                   // response kind
  "select"_Stz,              // comment about the job
  "select"_Stz,              // the prompt command
  nullptr,                   // the second word
  nullptr,                   // the _run task seed
  "select_req"_Stz,          // the _req task seed
  nullptr,                   // the _rcv task seed
  nullptr,                   // the _exe task seed
  nullptr,                   // the _end task seed
  nullptr,                   // the _can task seed
  true,                      // check resources?
  false,                     // detached job?
  false,                     // buffer attached?
  DL_GRAB_READ,              // ddl lock model
  PY_NORMAL,                 // priority
  JF_NONE,                   // flags

  eJob_checkCluster,         // prompt check function
  pJob_Select_pre,           // prompt pre-process function
  vJob_post,                 // prompt post-process function
  nullptr,                   // job new function
  vJob_Select_delete,        // job delete function
  nullptr,                   // job pre-send function
  nullptr,                   // job post-receive function
  vJob_Select_show,          // job show function
  
  // help and example
  true,
  "select <tableNm> <partitionNo>"_Stz,
  "fetch supplier 1;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_Select_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_Start_class;
  
  return &xJobClass_Select;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
Job_Select* Jobs::_pJob_Select(void* aAddress)
{
  Job_Select* pJob_Select = reinterpret_cast<Job_Select*>(aAddress);
  
  RASSERT_TYPE(pJob_Select,CN_JOB_SELECT);
  
  return pJob_Select;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Select::operator new(size_t)
{
  Job_Select* pJob_Select =
    static_cast<Job_Select*>(HeapMem::pAllocate(CN_JOB_SELECT));
  
  return pJob_Select;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* Job_Select::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void Job_Select::operator delete(void* aAddress)
{
  Job_Select* pJob_Select = static_cast<Job_Select*>(aAddress);
  
  HeapMem::vFree(pJob_Select);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Select::Job_Select(JobClass* aJobClass, ReqTuple* aReqTuple)
: Job(aJobClass, aReqTuple)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SELECT,_FOOTER_);
  
  m_pPipeline = new Pipeline(this);
  
  m_pToClause  = new Op_ToClause  (nullptr,m_pPipeline);
  m_pToFilter  = new Op_ToFilter  (nullptr,m_pPipeline);
  m_pToCommand = new Op_ToCommand (nullptr,m_pPipeline);
  m_pToDo      = new Op_ToDo      (nullptr,m_pPipeline);
  
  //! ZEDOP has been removed !!!
  //m_pZed       = new ZedOp        (nullptr,m_pPipeline);
  
  m_pStmt_Select = new Stmt_Select;

  m_pToClause->vOpOut(m_pToFilter);
  m_pToFilter->vOpOut(m_pToCommand);
  m_pToCommand->vOpOut(m_pToDo);
  
  //! ZEDOP has been removed !!!
  //m_pToDo->vOpOut(m_pZed);
  //m_pZed->vOpOut(nullptr);

  _checkInitialised_(CN_JOB_SELECT);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_SELECT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Select::~Job_Select(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SELECT,_FOOTER_);
 
  //delete m_pZed;
  delete m_pToDo;
  delete m_pToCommand;
  delete m_pToFilter;
  delete m_pToClause;
  
  delete m_pPipeline;
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Select::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_Select* pJob_Select = _pJob_Select(pObject);

  pJob_Select->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_Select::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SELECT,_FOOTER_);
  
  ASSERT(pTokenVec->pGet(0)->bSame("select"_Stz));
  
  // get number of items in token vector
  //i8 iNumTokens = pTokenVec->nItems();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pStmt_Select->pTokenVec()->vCopyVector(pTokenVec);
  
  //m_pStmt_Select->pTokenVec()->vDumpTokens();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Select::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SELECT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* query begin                                                                */
/*----------------------------------------------------------------------------*/
void Job_Select::vBegin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SELECT,_FOOTER_);
  
  AM::vSetAcksRmg(pTask(),5);
  
//  Session* pSession = AM::pThisTLSSession();
//  
//  bool bSpawned;
//  
//  bSpawned = AM::bSpawnOp(pSession,m_pZed);        ASSERT(bSpawned);
//  bSpawned = AM::bSpawnOp(pSession,m_pToDo);       ASSERT(bSpawned);
//  bSpawned = AM::bSpawnOp(pSession,m_pToCommand);  ASSERT(bSpawned);
//  bSpawned = AM::bSpawnOp(pSession,m_pToFilter);   ASSERT(bSpawned);
//  bSpawned = AM::bSpawnOp(pSession,m_pToClause);   ASSERT(bSpawned);
}

/*----------------------------------------------------------------------------*/
/* query end                                                                  */
/*----------------------------------------------------------------------------*/
void Job_Select::vEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SELECT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* query execute                                                              */
/*----------------------------------------------------------------------------*/
void Job_Select::vExecute(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SELECT,_FOOTER_);
  
  m_pToClause->vAppendLeft(m_pStmt_Select);
  m_pStmt_Select = nullptr;
  
  EndMarker* pEndMarker = new EndMarker;
  m_pToClause->vAppendLeft(pEndMarker);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Select::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SELECT);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_Select_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession)
{
  Job_Select* pJob_Select = new Job_Select(aJobClass,aReqTuple);
  //pJob->vDicPath(pSession);

  TokenVec* pTokenVec = pSession->m_pInput->pTknVec();
  
  bool bResult = pJob_Select->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    pJob_Select->vAddError(EC_INVALID_COMMAND);
  }
  
  return pJob_Select;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Select_delete(Job* pJob)
{
  Job_Select* pJob_Select = _pJob_Select(pJob);
  
  delete pJob_Select;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Select_show(Job* pJob)
{
  Job_Select* pJob_Select = _pJob_Select(pJob);

  pJob_Select->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
