/*============================================================================*/
/* Job_InsertRow.cpp -- Insert Row Job                                        */
/*============================================================================*/

// Job Short: InsertRow

#include "JobsModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_InsertRow =
{
  "Job_InsertRow"_Stz,    // name of the job class
  CN_JOB_INSERTROW,       // class enum
  JK_LOCAL,               // job kind
  "insert into tableNm"_Stz, // comment about the job
  "insert"_Stz,           // the prompt command
  "into"_Stz,             // the second word
  nullptr,                // the _run task seed
  "insertRow_req"_Stz,    // the _req task seed
  nullptr,                // the _rcv task seed
  nullptr,                // the _exe task seed
  nullptr,                // the _end task seed
  nullptr,                // the _can task seed
  true,                   // check resources?
  false,                  // detached job?
  false,                  // buffer attached?
  JT_NONE,                // job trace
  pJob_InsertRow_pre,     // prompt pre-process function
  vJob_InsertRow_post,    // prompt post-process function
  pJob_InsertRow_new,     // job new function
  vJob_InsertRow_delete,  // job delete function
  nullptr,                // job pre-send function
  nullptr,                // job post-receive function
  vJob_InsertRow_show,    // job show function
  
  // help and example
  true,
  "insert row \"<values>\""_Stz,
  "insert row \"1,abcdef\";"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* JobsModel::pJob_InsertRow_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_Lf_class;
  
  return &xJobClass_InsertRow;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_InsertRow* JobsModel::_pJob_InsertRow(void* aAddress)
{
  Job_InsertRow* pJob_InsertRow = reinterpret_cast<Job_InsertRow*>(aAddress);
  
  RASSERT_TYPE(pJob_InsertRow,CN_JOB_INSERTROW);
  
  return pJob_InsertRow;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_InsertRow::operator new(size_t)
{
  Job_InsertRow* pJob_LoadTable =
    static_cast<Job_InsertRow*>(HeapMem::pAllocate(CN_JOB_INSERTROW));
  
  return pJob_LoadTable;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_InsertRow::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_InsertRow::operator delete(void* aAddress)
{
  Job_InsertRow* pJob_InsertRow = static_cast<Job_InsertRow*>(aAddress);
  
  HeapMem::vFree(pJob_InsertRow);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_InsertRow::Job_InsertRow(Interface::Session* pSession)
: Job(CN_JOB_INSERTROW, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INSERTROW,_FOOTER_);
  
  m_pTable = nullptr;
  
  // set the enclosined_by to be a '
  m_xFormatters.m_cEnclosedBy_Leading = '\'';
  m_xFormatters.m_cEnclosedBy_Trailing = '\'';
  
  // create Job_InsertTable
  JobClass* pJobClass_InsertTable = JobClass::pFind(CN_JOB_INSERTTABLE);
  ASSERT(pJobClass_InsertTable);
  m_pJob_InsertTable =
  static_cast<Job_InsertTable*>(pJobClass_InsertTable->pPreJob(pSession));
  
  _checkInitialised_(CN_JOB_INSERTROW);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_INSERTROW,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_InsertRow::~Job_InsertRow(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INSERTROW,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_InsertRow::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_InsertRow* pJob_InsertRow = _pJob_InsertRow(pObject);

  pJob_InsertRow->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* fromat a row                                                               */
/*----------------------------------------------------------------------------*/
bool Job_InsertRow::bFormatRow(Stz* aRawData, i8 iRawLength)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INSERTROW,_FOOTER_);
  
  bool bBlankLine;
  
  bool bValid = Job_WriteBehind::bFormatRow(
                  m_pTable, &m_xFormatters, aRawData, iRawLength, bBlankLine);
  
  return bValid;
}

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_InsertRow::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INSERTROW,_FOOTER_);
  
  ASSERT(pTokenVec->pGet(0)->bSame("insert"_Stz));
  ASSERT(pTokenVec->pGet(1)->bSame("into"_Stz));
  
  // get no of tokens in token vector
  i8 nTokens = pTokenVec->nTokens();;
  
  // there should be at least 3 tokens
  if (nTokens != 3)
    return nullptr;
  
  // get the table name
  m_sObjectName = pTokenVec->pGet(2)->aText();

  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_InsertRow::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INSERTROW,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_InsertRow::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INSERTROW);
  
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
Job* JobsModel::pJob_InsertRow_pre(Interface::Session* pSession)
{
  Job_InsertRow* pJob_InsertRow = new Job_InsertRow(pSession);
  
  TokenVec* pTokenVec = pSession->m_pCommandProcessor->pTknVec();
  
  bool bResult = pJob_InsertRow->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_InsertRow->m_bHasError = true;
    pJob_InsertRow->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_InsertRow;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_InsertRow_post(Interface::Session* pSession, Job* pJob)
{
  Job_InsertRow* pJob_InsertRow = _pJob_InsertRow(pJob);
  
  if ( pJob_InsertRow->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[pJob_InsertRow->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
}

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_InsertRow_new(Job* pArg)
{
  Job_InsertRow* pJob_InsertRow = new Job_InsertRow(nullptr);
  
  return pJob_InsertRow;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_InsertRow_delete(Interface::Session* pSession, Job* pJob)
{
  Job_InsertRow* pJob_InsertRow = _pJob_InsertRow(pJob);
  
  //vTrace("DELETING INSERTROW\n"_Stz);
  delete pJob_InsertRow;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_InsertRow_show(Job* pJob)
{
  Job_InsertRow* pJob_InsertRow = _pJob_InsertRow(pJob);

  pJob_InsertRow->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
