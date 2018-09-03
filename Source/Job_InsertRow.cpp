/*============================================================================*/
/* Job_InsertRow.cpp -- Insert Row Job                                        */
/*============================================================================*/

// Job Short: LoadTable

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/* example:
 
 ld lineitem "/tpch/lineitem.tbl" ;
 
 */

JobClass xJobClass_InsertRow =
{
  "Job_InsertRow"_Stz,    // name of the job class
  CN_JOB_INSERTROW,       // class enum
  JK_DIST2PC,             // job kind
  "insert row"_Stz,       // comment about the job
  "insert"_Stz,           // the prompt command
  "row"_Stz,              // the second word
  nullptr,                // the _run task seed
  "insertRow_req"_Stz,    // the _req task seed
  "insertRow_beg"_Stz,    // the _rcv task seed
  "insertRow_exe"_Stz,    // the _exe task seed
  "insertRow_end"_Stz,    // the _end task seed
  "insertRow_can"_Stz,    // the _can task seed
  true,                   // check resources?
  false,                  // detached job?
  false,                  // buffer attached?
  JT_NONE,                // job trace
  pJob_InsertRow_pre,     // prompt pre-process function
  vJob_InsertRow_post,    // prompt post-process function
  pJob_InsertRow_new,     // job new function
  vJob_InsertRow_delete,  // job delete function
  vJob_send,              // job pre-send function
  vJob_recv,              // job post-receive function
  vJob_InsertRow_show,    // job show function
  true,                   // job is enabled
  "<help>"_Stz,           // help
  "<example>"_Stz,        // example

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* JobsModel::pJob_InsertRow_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_Prompt_class;
  
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
  
  //RASSERT( !pJob_InsertRow || CN_JOB_INSERTROW == pJob_InsertRow->eClass());
  
  return pJob_InsertRow;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_InsertRow::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_InsertRow* pJob_LoadTable =
    static_cast<Job_InsertRow*>(Heap::pAllocate(CN_JOB_INSERTROW));
  
  return pJob_LoadTable;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_InsertRow::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_InsertRow::operator delete(void* aAddress)
{
  Job_InsertRow* pJob_InsertRow = static_cast<Job_InsertRow*>(aAddress);
  
  Heap::vFree(pJob_InsertRow);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_InsertRow::Job_InsertRow(Interface::SEitf* pSEitf)
: Job(CN_JOB_INSERTROW, pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INSERTROW,_FOOTER_);
  
  _checkInitialised_(CN_JOB_INSERTROW);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_INSERTROW,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_InsertRow::~Job_InsertRow(void)
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
  RASSERT(CN_JOB_INSERTROW == eClass);
  RASSERT(pObject->eClass() == CN_JOB_INSERTROW);
  
  Job_InsertRow* pJob_InsertRow = _pJob_InsertRow(pObject);

  pJob_InsertRow->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_InsertRow::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INSERTROW,_FOOTER_);
  
  ASSERT(pTokenVec->pGet(0)->bSame("insert"_Stz));
  ASSERT(pTokenVec->pGet(1)->bSame("row"_Stz));
  
  // get no of tokens in token vector
  i8 nTokens = pTokenVec->nTokens();;
  
  // there should be at least 3 tokens
  if (nTokens != 3)
    return nullptr;
  
//  if (!m_pSEitf->pCurrentTable()) {
//    m_pSEitf->vSetLastError(EC_NO_SUCH_TABLE);
//    return false;
//  }
  
  Stz* pRow = pTokenVec->pGet(2)->aText();
  bool bBlankLine = false;
  s48K s;
  i8 iRowLength;
  
  // format the record into row format
  bool bValid = Row::bFormatRow(
                                m_pSEitf->m_pCurrentTable->pColumnVec(),
                                &m_pSEitf->m_xFormatters,
                                pRow, StrLib::iSize(pRow->aChar()),
                                s, iRowLength, bBlankLine,
                                false /* allowNonPrintable */);
  
  if (! bValid) {
    return false;
  }
  
  // get the partition this row hashes to
  //i8 iRowPartition = m_pSEitf->pCurrentTable()->iRowPartition(s);
  
  //bool bResult = m_pSEitf->bInsertRow(s, iRowLength, iRowPartition);
  
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get task                                                                   */
/*----------------------------------------------------------------------------*/
Tasks::Task* Job_InsertRow::pTask(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INSERTROW,_FOOTER_);
  
  return m_pTask;
}

/*----------------------------------------------------------------------------*/
/* set task                                                                   */
/*----------------------------------------------------------------------------*/
void Job_InsertRow::vTask(Tasks::Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INSERTROW,_FOOTER_);
  
  m_pTask = pTask;
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
Job* JobsModel::pJob_InsertRow_pre(Interface::SEitf* pSEitf)
{
  Job_InsertRow* pJob_InsertRow = new Job_InsertRow(pSEitf);
  
  TokenVec* pTokenVec = pSEitf->m_pCommandProcessor->pTknVec();
  
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
void JobsModel::vJob_InsertRow_post(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_InsertRow* pJob_InsertRow = _pJob_InsertRow(pJob);
  
  if ( pJob_InsertRow->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("ERROR (NODE%lld): %s\n"_Stz, i_Node, aEC[pJob_InsertRow->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
  else {

    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("%s\n"_Stz, pJob_InsertRow->m_xNode[i_Node].m_sResponse.aChar());
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
void JobsModel::vJob_InsertRow_delete(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_InsertRow* pJob_InsertRow = _pJob_InsertRow(pJob);
  
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
