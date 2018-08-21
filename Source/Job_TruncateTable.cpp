/*============================================================================*/
/* Job_TruncateTable.cpp -- Truncate Table Job                                */
/*============================================================================*/

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobNote mJobNote_TruncateTable (
  "Job_TruncateTable"_Stz,   // name of the job class
  CN_JOB_TRUNCATETABLE,      // class enum
  JK_DIST2PC,                // job kind
  "truncate table"_Stz,      // comment about the job
  "tr"_Stz,                  // the prompt command
  nullptr,                   // the second word
  "truncateTable_req"_Stz,   // the _req task seed
  "truncateTable_beg"_Stz,   // the _rcv task seed
  "truncateTable_node"_Stz,  // the _exe task seed
  "truncateTabl_end"_Stz,    // the _end task seed
  "truncateTabl_can"_Stz,    // the _can task seed
  true,                      // check resources?
  false,                     // detached job?
  false,                     // buffer attached?
  JT_NONE,                   // job trace
  pJob_TruncateTable_pre,    // prompt pre-process function
  vJob_TruncateTable_post,   // prompt post-process function
  nullptr,                   // job new function
  vJob_TruncateTable_delete, // job delete function
  nullptr,                   // job pre-send function
  nullptr,                   // job post-receive function
  vJob_TruncateTable_show,   // job show function
  true,                      // job is enabled
  "tr <tableNm>"_Stz,        // help
  "<example>"_Stz            // example
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_TruncateTable* JobsModel::_pJob_TruncateTable(void* aAddress)
{
  Job_TruncateTable* pJob_TruncateTable = reinterpret_cast<Job_TruncateTable*>(aAddress);
  
  RASSERT( !pJob_TruncateTable || CN_JOB_TRUNCATETABLE == pJob_TruncateTable->eClass());
  
  return pJob_TruncateTable;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_TruncateTable::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_TruncateTable* pJob_TruncateTable =
    static_cast<Job_TruncateTable*>(Heap::pAllocate(CN_JOB_TRUNCATETABLE));
  
  return pJob_TruncateTable;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_TruncateTable::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_TruncateTable::operator delete(void* aAddress)
{
  Job_TruncateTable* pJob_TruncateTable = static_cast<Job_TruncateTable*>(aAddress);
  
  Heap::vFree(pJob_TruncateTable);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_TruncateTable::Job_TruncateTable(Interface::SEitf* pSEitf)
: Job(CN_JOB_TRUNCATETABLE, pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_TRUNCATETABLE,_FOOTER_);
    
  m_sSeed = mJobNote_TruncateTable.pJobClass()->m_aSeedReq;
  m_bCheckResources = true;
  m_pTask           = nullptr;
  
  _checkInitialised_(CN_JOB_TRUNCATETABLE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_TRUNCATETABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_TruncateTable::~Job_TruncateTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_TRUNCATETABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_TruncateTable::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_TRUNCATETABLE == eClass);
  RASSERT(pObject->eClass() == CN_JOB_TRUNCATETABLE);
  
  Job_TruncateTable* pJob_TruncateTable = _pJob_TruncateTable(pObject);

  pJob_TruncateTable->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_TruncateTable::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_TRUNCATETABLE,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("tr"_Stz));
  
  // get number of items in token vector
  iNumTokens = pTokenVec->nItems();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 2 tokens
  if (iNumTokens != 2)
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // set the table name
  m_sObjectName = pTokenVec->pGet(1)->aText();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_TruncateTable::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_TRUNCATETABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_TruncateTable::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_TRUNCATETABLE);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_TruncateTable_pre(Interface::SEitf* pSEitf)
{
  Job_TruncateTable* pJob_TruncateTable = new Job_TruncateTable(pSEitf);
  
  TokenVec* pTokenVec = pSEitf->m_pCommandProcessor->pTknVec();
  
  bool bResult = pJob_TruncateTable->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_TruncateTable->m_bHasError = true;
    pJob_TruncateTable->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_TruncateTable;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_TruncateTable_post(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_TruncateTable* pJob_TruncateTable = _pJob_TruncateTable(pJob);
  
  if ( pJob_TruncateTable->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("ERROR (NODE%lld): %s\n"_Stz, i_Node, aEC[pJob_TruncateTable->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_TruncateTable_delete(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_TruncateTable* pJob_TruncateTable = _pJob_TruncateTable(pJob);
  
  delete pJob_TruncateTable;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_TruncateTable_show(Job* pJob)
{
  Job_TruncateTable* pJob_TruncateTable = _pJob_TruncateTable(pJob);

  pJob_TruncateTable->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
