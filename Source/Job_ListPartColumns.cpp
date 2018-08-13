/*============================================================================*/
/* Job_ListPartColumns.cpp -- List Partition Columns Job                      */
/*============================================================================*/

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobNote mJobNote_ListPartColumns (
  "Job_ListPartColumns"_Stz,        // name of the job class
  CN_JOB_LISTPARTCOLUMNS,           // class enum
  JK_SINGLENODE,                    // job kind
  "list partition key columns"_Stz, // comment about the job
  "lspartcol"_Stz,                  // the prompt command
  nullptr,                          // the second word
  "listPartColumns"_Stz,            // the task seed for this job
  false,                            // check resources?
  false,                            // detached job?
  JT_NONE,                          // job trace
  pJob_ListPartColumns_pre,         // prompt pre-process function
  vJob_ListPartColumns_post,        // prompt post-process function
  nullptr,                          // job new function
  vJob_ListPartColumns_delete,      // job delete function
  nullptr,                          // job receive function
  vJob_ListPartColumns_show,        // job show function
  true,                             // job is enabled
  "<help>"_Stz,                     // help
  "<example>"_Stz                   // example
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_ListPartColumns* JobsModel::_pJob_ListPartColumns(void* aAddress)
{
  Job_ListPartColumns* pJob_ListPartColumns = reinterpret_cast<Job_ListPartColumns*>(aAddress);
  
  //RASSERT( !pJob_ListPartColumns || CN_JOB_LISTPARTCOLUMNS == pJob_ListPartColumns->eClass());
  
  return pJob_ListPartColumns;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_ListPartColumns::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_ListPartColumns* pJob_ListPartColumns =
    static_cast<Job_ListPartColumns*>(Heap::pAllocate(CN_JOB_LISTPARTCOLUMNS));
  
  return pJob_ListPartColumns;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_ListPartColumns::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_ListPartColumns::operator delete(void* aAddress)
{
  Job_ListPartColumns* pJob_ListPartColumns = static_cast<Job_ListPartColumns*>(aAddress);
  
  Heap::vFree(pJob_ListPartColumns);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_ListPartColumns::Job_ListPartColumns(Interface::SEitf* pSEitf)
: Job(CN_JOB_LISTPARTCOLUMNS, pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTPARTCOLUMNS,_FOOTER_);
  
  m_sSeed    = mJobNote_ListPartColumns.pJobClass()->m_aSeed;

  _checkInitialised_(CN_JOB_LISTPARTCOLUMNS);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTPARTCOLUMNS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_ListPartColumns::~Job_ListPartColumns(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTPARTCOLUMNS,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_ListPartColumns::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_DOCOL == eClass);
  RASSERT(pObject->eClass() == CN_JOB_LISTPARTCOLUMNS);
  
  Job_ListPartColumns* pJob_ListPartColumns = _pJob_ListPartColumns(pObject);

  pJob_ListPartColumns->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_ListPartColumns::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTPARTCOLUMNS,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("lspartcol"_Stz));
  
  // - - - - - - - - - - - - - - - - - - - - -
  // get no of tokens in token vector
  iNumTokens = pTokenVec->nTokens();;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 2 tokens
  if (iNumTokens != 2) {
    return nullptr;
  }
  
  // - - - - - - - - - - - - - - - - - - - - -
  // set the table name
  m_sObjectName = pTokenVec->pGet(1)->aText();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_ListPartColumns::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTPARTCOLUMNS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_ListPartColumns::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTPARTCOLUMNS);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_ListPartColumns_pre(Interface::SEitf* pSEitf)
{
  Job_ListPartColumns* pJob_ListPartColumns = new Job_ListPartColumns(pSEitf);
  
  TokenVec* pTokenVec = pSEitf->m_pCommandProcessor->pTknVec();
  
  bool bResult = pJob_ListPartColumns->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_ListPartColumns->m_bHasError = true;
    pJob_ListPartColumns->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_ListPartColumns;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_ListPartColumns_post(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_ListPartColumns* pJob_ListPartColumns = _pJob_ListPartColumns(pJob);
  
  if ( pJob_ListPartColumns->m_bHasError ) {
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("ERROR (NODE%lld): %s\n"_Stz, i_Node, aEC[pJob_ListPartColumns->m_xNode[i_Node].m_eErrorCode]);
    }
  }
  else {
    if (gpAttachMap->m_bMetadata) {
      vTrace("metadata: %s\n"_Stz, pJob_ListPartColumns->m_sMetadata.aStr());
    }
    
    i8 iRowsProcessed = 0;
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("%s\n"_Stz, pJob_ListPartColumns->m_xNode[i_Node].m_sResponse.aChar());
      iRowsProcessed += pJob_ListPartColumns->m_xNode[i_Node].m_iRowsProcessed;
    }
    if (gpAttachMap->m_bMetadata) {
      vTrace("success: %lld rows affected\n"_Stz, iRowsProcessed);
    }
  }
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_ListPartColumns_delete(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_ListPartColumns* pJob_ListPartColumns = _pJob_ListPartColumns(pJob);
  
  delete pJob_ListPartColumns;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_ListPartColumns_show(Job* pJob)
{
  Job_ListPartColumns* pJob_ListPartColumns = _pJob_ListPartColumns(pJob);

  pJob_ListPartColumns->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
