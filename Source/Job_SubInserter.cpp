/*============================================================================*/
/* Job_SubInserter.cpp -- SubInserter Job                                     */
/*============================================================================*/

// Job Short: SubInserter

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_SubInserter =
{
  "Job_SubInserter"_Stz,    // name of the job class
  CN_JOB_SUBINSERTER,       // class enum
  JK_LOCAL,               // job kind
  "insert rows into tableNm"_Stz, // comment about the job
  "insert"_Stz,           // the prompt command
  "rows"_Stz,             // the second word
  nullptr,                // the _run task seed
  "SubInserter_req"_Stz,    // the _req task seed
  nullptr,                // the _rcv task seed
  nullptr,                // the _exe task seed
  nullptr,                // the _end task seed
  nullptr,                // the _can task seed
  true,                   // check resources?
  false,                  // detached job?
  false,                  // buffer attached?
  JT_NONE,                // job trace
  pJob_SubInserter_pre,    // prompt pre-process function
  vJob_SubInserter_post,    // prompt post-process function
  pJob_SubInserter_new,     // job new function
  vJob_SubInserter_delete,  // job delete function
  nullptr,                // job pre-send function
  nullptr,                // job post-receive function
  vJob_SubInserter_show,    // job show function
  
  // help and example
  true,
  "insert rows \"<values>\""_Stz,
  "insert rows \"1,abcdef\";"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* JobModel::pJob_SubInserter_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_SubLoader_class;
  
  return &xJobClass_SubInserter;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_SubInserter* JobModel::_pJob_SubInserter(void* aAddress)
{
  Job_SubInserter* pJob_SubInserter = reinterpret_cast<Job_SubInserter*>(aAddress);
  
  RASSERT_TYPE(pJob_SubInserter,CN_JOB_SUBINSERTER);
  
  return pJob_SubInserter;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_SubInserter::operator new(size_t)
{
  Job_SubInserter* pJob_LoadTable =
    static_cast<Job_SubInserter*>(HeapMem::pAllocate(CN_JOB_SUBINSERTER));
  
  return pJob_LoadTable;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_SubInserter::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_SubInserter::operator delete(void* aAddress)
{
  Job_SubInserter* pJob_SubInserter = static_cast<Job_SubInserter*>(aAddress);
  
  HeapMem::vFree(pJob_SubInserter);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_SubInserter::Job_SubInserter(Interface::Session* pSession)
: Job(CN_JOB_SUBINSERTER, pSession), JobWriteBehind(DS_INSERT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBINSERTER,_FOOTER_);
  
  // set the enclosined_by to be a '
  m_xFormatters.m_bDefault = false;
  m_xFormatters.m_cEnclosedBy_Leading = '\'';
  m_xFormatters.m_cEnclosedBy_Trailing = '\'';
  
  // create Job_InsertTable
  JobClass* pJobClass_InsertTable = JobClass::pFind(CN_JOB_INSERTTABLE);
  ASSERT(pJobClass_InsertTable);
  m_pJob_InsertTable =
  static_cast<Job_InsertTable*>(pJobClass_InsertTable->pPreJob(pSession));
  
  // set insertRowJob in the insertTable job
  m_pJob_InsertTable->m_pJobSubInserter = this;
  
  _checkInitialised_(CN_JOB_SUBINSERTER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBINSERTER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_SubInserter::~Job_SubInserter(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBINSERTER,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_SubInserter::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_SubInserter* pJob_SubInserter = _pJob_SubInserter(pObject);

  pJob_SubInserter->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* begin inserting                                                            */
/*----------------------------------------------------------------------------*/
void Job_SubInserter::vBegin(Table* pTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBINSERTER,_FOOTER_);
  
  // set the table and parentJob in the writeBehind Job
  //RASSERT(pTable == this->m_pTable);
  JobWriteBehind::m_pTable = pTable;
  JobWriteBehind::m_pParentJob = (Job*)m_pJob_InsertTable;
}

/*----------------------------------------------------------------------------*/
/* end inserting                                                              */
/*----------------------------------------------------------------------------*/
void Job_SubInserter::vEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBINSERTER,_FOOTER_);
  
  vHandleDanglingBuffers();
}

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_SubInserter::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBINSERTER,_FOOTER_);
  
  ASSERT(pTokenVec->pGet(0)->bSame("insert"_Stz));
  ASSERT(pTokenVec->pGet(1)->bSame("rows"_Stz));
  
  // get no of tokens in token vector
  i8 nTokens = pTokenVec->nTokens();;
  
  // there should be at least 4 tokens
  if (nTokens != 4)
    return nullptr;
  
  // get the table name
  m_sObjectName = pTokenVec->pGet(3)->aText();

  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_SubInserter::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBINSERTER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_SubInserter::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBINSERTER);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobModel::pJob_SubInserter_pre(Interface::Session* pSession)
{
  Job_SubInserter* pJob_SubInserter = new Job_SubInserter(pSession);
  
  TokenVec* pTokenVec = pSession->m_pCommandProcessor->pTknVec();
  
  bool bResult = pJob_SubInserter->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_SubInserter->m_bHasError = true;
    pJob_SubInserter->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_SubInserter;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobModel::vJob_SubInserter_post(Interface::Session* pSession, Job* pJob)
{
  Job_SubInserter* pJob_SubInserter = _pJob_SubInserter(pJob);
  
  if ( pJob_SubInserter->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[pJob_SubInserter->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
}

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* JobModel::pJob_SubInserter_new(Job* pArg)
{
  Job_SubInserter* pJob_SubInserter = new Job_SubInserter(nullptr);
  
  return pJob_SubInserter;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobModel::vJob_SubInserter_delete(Interface::Session* pSession, Job* pJob)
{
  Job_SubInserter* pJob_SubInserter = _pJob_SubInserter(pJob);
  
  //vTrace("DELETING INSERTROW\n"_Stz);
  delete pJob_SubInserter;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobModel::vJob_SubInserter_show(Job* pJob)
{
  Job_SubInserter* pJob_SubInserter = _pJob_SubInserter(pJob);

  pJob_SubInserter->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
