/*============================================================================*/
/* Job_InsertRows.cpp -- Insert Row Job                                        */
/*============================================================================*/

// Job Short: InsertRow

#include "JobsModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_InsertRows =
{
  "Job_InsertRows"_Stz,   // name of the job class
  CN_JOB_INSERTROWS,      // class enum
  JK_LOCAL,               // job kind
  "insert rows into tableNm"_Stz, // comment about the job
  "insert"_Stz,           // the prompt command
  "rows"_Stz,             // the second word
  nullptr,                // the _run task seed
  "insertRows_req"_Stz,   // the _req task seed
  nullptr,                // the _rcv task seed
  nullptr,                // the _exe task seed
  nullptr,                // the _end task seed
  nullptr,                // the _can task seed
  true,                   // check resources?
  false,                  // detached job?
  false,                  // buffer attached?
  JT_NONE,                // job trace
  pJob_InsertRows_pre,    // prompt pre-process function
  vJob_InsertRows_post,    // prompt post-process function
  pJob_InsertRows_new,     // job new function
  vJob_InsertRows_delete,  // job delete function
  nullptr,                // job pre-send function
  nullptr,                // job post-receive function
  vJob_InsertRows_show,    // job show function
  
  // help and example
  true,
  "insert rows \"<values>\""_Stz,
  "insert rows \"1,abcdef\";"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* JobsModel::pJob_InsertRows_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_InsertTable_class;
  
  return &xJobClass_InsertRows;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_InsertRows* JobsModel::_pJob_InsertRows(void* aAddress)
{
  Job_InsertRows* pJob_InsertRows = reinterpret_cast<Job_InsertRows*>(aAddress);
  
  RASSERT_TYPE(pJob_InsertRows,CN_JOB_INSERTROWS);
  
  return pJob_InsertRows;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_InsertRows::operator new(size_t)
{
  Job_InsertRows* pJob_LoadTable =
    static_cast<Job_InsertRows*>(HeapMem::pAllocate(CN_JOB_INSERTROWS));
  
  return pJob_LoadTable;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_InsertRows::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_InsertRows::operator delete(void* aAddress)
{
  Job_InsertRows* pJob_InsertRows = static_cast<Job_InsertRows*>(aAddress);
  
  HeapMem::vFree(pJob_InsertRows);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_InsertRows::Job_InsertRows(Interface::Session* pSession)
: Job(CN_JOB_INSERTROWS, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INSERTROWS,_FOOTER_);
  
  m_pTable = nullptr;
  
  // set the enclosined_by to be a '
  m_xFormatters.m_cEnclosedBy_Leading = '\'';
  m_xFormatters.m_cEnclosedBy_Trailing = '\'';
  
  // create Job_InsertTable
  JobClass* pJobClass_InsertTable = JobClass::pFind(CN_JOB_INSERTTABLE);
  ASSERT(pJobClass_InsertTable);
  m_pJob_InsertTable =
  static_cast<Job_InsertTable*>(pJobClass_InsertTable->pPreJob(pSession));
  
  // set insertRowJob in the insertTable job
  m_pJob_InsertTable->m_pJobInsertRows = this;
  
  _checkInitialised_(CN_JOB_INSERTROWS);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_INSERTROWS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_InsertRows::~Job_InsertRows(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INSERTROWS,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_InsertRows::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_InsertRows* pJob_InsertRows = _pJob_InsertRows(pObject);

  pJob_InsertRows->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* fromat a row                                                               */
/*----------------------------------------------------------------------------*/
bool Job_InsertRows::bFormatRow(Stz* aRawData, i8 iRawLength)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INSERTROWS,_FOOTER_);
  
  bool bBlankLine;
  
  bool bValid = Job_WriteBehind::bFormatRow(
                  m_pTable, &m_xFormatters, aRawData, iRawLength, bBlankLine);
  
  return bValid;
}

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_InsertRows::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INSERTROWS,_FOOTER_);
  
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
void Job_InsertRows::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INSERTROWS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_InsertRows::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INSERTROWS);
  
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
Job* JobsModel::pJob_InsertRows_pre(Interface::Session* pSession)
{
  Job_InsertRows* pJob_InsertRows = new Job_InsertRows(pSession);
  
  TokenVec* pTokenVec = pSession->m_pCommandProcessor->pTknVec();
  
  bool bResult = pJob_InsertRows->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_InsertRows->m_bHasError = true;
    pJob_InsertRows->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_InsertRows;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_InsertRows_post(Interface::Session* pSession, Job* pJob)
{
  Job_InsertRows* pJob_InsertRows = _pJob_InsertRows(pJob);
  
  if ( pJob_InsertRows->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[pJob_InsertRows->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
}

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_InsertRows_new(Job* pArg)
{
  Job_InsertRows* pJob_InsertRows = new Job_InsertRows(nullptr);
  
  return pJob_InsertRows;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_InsertRows_delete(Interface::Session* pSession, Job* pJob)
{
  Job_InsertRows* pJob_InsertRows = _pJob_InsertRows(pJob);
  
  //vTrace("DELETING INSERTROW\n"_Stz);
  delete pJob_InsertRows;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_InsertRows_show(Job* pJob)
{
  Job_InsertRows* pJob_InsertRows = _pJob_InsertRows(pJob);

  pJob_InsertRows->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
