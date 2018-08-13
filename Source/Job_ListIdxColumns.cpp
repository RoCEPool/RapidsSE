/*============================================================================*/
/* Job_ListIdxColumns.cpp -- List Index Columns Job                           */
/*============================================================================*/

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobNote mJobNote_ListIdxColumns (
  "Job_ListIdxColumns"_Stz,  // name of the job class
  CN_JOB_LISTIDXCOLUMNS,     // class enum
  JK_SINGLENODE,             // job kind
  "list index columns"_Stz,  // comment about the job
  "lsidxcol"_Stz,            // the prompt command
  nullptr,                   // the second word
  "listIdxColumns"_Stz,      // the task seed for this job
  false,                     // check resources?
  false,                     // detached job?
  JT_NONE,                   // job trace
  pJob_ListIdxColumns_pre,   // prompt pre-process function
  vJob_ListIdxColumns_post,  // prompt post-process function
  nullptr,                   // job new function
  vJob_ListIdxColumns_delete,// job delete function
  nullptr,                   // job receive function
  vJob_ListIdxColumns_show,  // job show function
  true,                      // job is enabled
  "<help>"_Stz,              // help
  "<example>"_Stz            // example
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_ListIdxColumns* JobsModel::_pJob_ListIdxColumns(void* aAddress)
{
  Job_ListIdxColumns* pJob_ListIdxColumns = reinterpret_cast<Job_ListIdxColumns*>(aAddress);
  
  //RASSERT( !pJob_ListIdxColumns || CN_JOB_LISTIDXCOLUMNS == pJob_ListIdxColumns->eClass());
  
  return pJob_ListIdxColumns;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_ListIdxColumns::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_ListIdxColumns* pJob_ListIdxColumns =
    static_cast<Job_ListIdxColumns*>(Heap::pAllocate(CN_JOB_LISTIDXCOLUMNS));
  
  return pJob_ListIdxColumns;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_ListIdxColumns::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_ListIdxColumns::operator delete(void* aAddress)
{
  Job_ListIdxColumns* pJob_ListIdxColumns = static_cast<Job_ListIdxColumns*>(aAddress);
  
  Heap::vFree(pJob_ListIdxColumns);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_ListIdxColumns::Job_ListIdxColumns(Interface::SEitf* pSEitf)
: Job(CN_JOB_LISTIDXCOLUMNS, pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTIDXCOLUMNS,_FOOTER_);
  
  m_sSeed    = mJobNote_ListIdxColumns.pJobClass()->m_aSeed;

  _checkInitialised_(CN_JOB_LISTIDXCOLUMNS);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTIDXCOLUMNS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_ListIdxColumns::~Job_ListIdxColumns(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTIDXCOLUMNS,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_ListIdxColumns::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_LISTIDXCOLUMNS == eClass);
  RASSERT(pObject->eClass() == CN_JOB_LISTIDXCOLUMNS);
  
  Job_ListIdxColumns* pJob_ListIdxColumns = _pJob_ListIdxColumns(pObject);

  pJob_ListIdxColumns->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_ListIdxColumns::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTIDXCOLUMNS,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("lsidxcol"_Stz));
  
  // - - - - - - - - - - - - - - - - - - - - -
  // get no of tokens in token vector
  iNumTokens = pTokenVec->nTokens();;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens != 3) {
    return nullptr;
  }
  
  // - - - - - - - - - - - - - - - - - - - - -
  // set the table name
  m_sTableName = pTokenVec->pGet(1)->aText();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // set the index name
  m_sObjectName = pTokenVec->pGet(2)->aText();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_ListIdxColumns::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTIDXCOLUMNS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_ListIdxColumns::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTIDXCOLUMNS);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_ListIdxColumns_pre(Interface::SEitf* pSEitf)
{
  Job_ListIdxColumns* pJob_ListIdxColumns = new Job_ListIdxColumns(pSEitf);
  
  TokenVec* pTokenVec = pSEitf->m_pCommandProcessor->pTknVec();
  
  bool bResult = pJob_ListIdxColumns->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_ListIdxColumns->m_bHasError = true;
    pJob_ListIdxColumns->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_ListIdxColumns;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_ListIdxColumns_post(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_ListIdxColumns* pJob_ListIdxColumns = _pJob_ListIdxColumns(pJob);
  
  if ( pJob_ListIdxColumns->m_bHasError ) {
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("ERROR (NODE%lld): %s\n"_Stz, i_Node, aEC[pJob_ListIdxColumns->m_xNode[i_Node].m_eErrorCode]);
    }
  }
  else {
    if (gpAttachMap->m_bMetadata) {
      vTrace("metadata: %s\n"_Stz, pJob_ListIdxColumns->m_sMetadata.aStr());
    }
    
    i8 iRowsProcessed = 0;
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("%s\n"_Stz, pJob_ListIdxColumns->m_xNode[i_Node].m_sResponse.aChar());
      iRowsProcessed += pJob_ListIdxColumns->m_xNode[i_Node].m_iRowsProcessed;
    }
    if (gpAttachMap->m_bMetadata) {
      vTrace("success: %lld rows affected\n"_Stz, iRowsProcessed);
    }
  }
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_ListIdxColumns_delete(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_ListIdxColumns* pJob_ListIdxColumns = _pJob_ListIdxColumns(pJob);
  
  delete pJob_ListIdxColumns;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_ListIdxColumns_show(Job* pJob)
{
  Job_ListIdxColumns* pJob_ListIdxColumns = _pJob_ListIdxColumns(pJob);

  pJob_ListIdxColumns->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
