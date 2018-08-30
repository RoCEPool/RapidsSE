/*============================================================================*/
/* Job_ListIndexes.cpp -- List Columns Job                                    */
/*============================================================================*/

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobNote mJobNote_ListIndexes (
  "Job_ListIndexes"_Stz,  // name of the job class
  CN_JOB_LISTINDEXES,     // class enum
  JK_LOCAL,               // job kind
  "list table indexes"_Stz,     // comment about the job
  "lsidx"_Stz,            // the prompt command
  nullptr,                // the second word
  nullptr,                // the _run task seed
  "listIndexes_req"_Stz,  // the _req task seed
  nullptr,                // the _rcv task seed 
  nullptr,                // the _exe task seed
  nullptr,                // the _end task seed
  nullptr,                // the _can task seed
  false,                  // check resources?
  false,                  // detached job?
  false,                  // buffer attached?
  JT_NONE,                // job trace
  pJob_ListIndexes_pre,   // prompt pre-process function
  vJob_ListIndexes_post,  // prompt post-process function
  nullptr,                // job new function
  vJob_ListIndexes_delete,// job delete function
  nullptr,                // job pre-send function
  nullptr,                // job post-receive function
  vJob_ListIndexes_show,  // job show function
  true,                   // job is enabled
  "<help>"_Stz,           // help
  "<example>"_Stz         // example
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_ListIndexes* JobsModel::_pJob_ListIndexes(void* aAddress)
{
  Job_ListIndexes* pJob_ListIndexes = reinterpret_cast<Job_ListIndexes*>(aAddress);
  
  //RASSERT( !pJob_ListIndexes || CN_JOB_LF == pJob_ListIndexes->eClass());
  
  return pJob_ListIndexes;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_ListIndexes::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_ListIndexes* pJob_ListIndexes =
    static_cast<Job_ListIndexes*>(Heap::pAllocate(CN_JOB_LISTINDEXES));
  
  return pJob_ListIndexes;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_ListIndexes::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_ListIndexes::operator delete(void* aAddress)
{
  Job_ListIndexes* pJob_ListIndexes = static_cast<Job_ListIndexes*>(aAddress);
  
  Heap::vFree(pJob_ListIndexes);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_ListIndexes::Job_ListIndexes(Interface::SEitf* pSEitf)
: Job(CN_JOB_LISTINDEXES, pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTINDEXES,_FOOTER_);
  
  _checkInitialised_(CN_JOB_LISTINDEXES);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTINDEXES,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_ListIndexes::~Job_ListIndexes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTINDEXES,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_ListIndexes::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_LISTINDEXES == eClass);
  RASSERT(pObject->eClass() == CN_JOB_LISTINDEXES);
  
  Job_ListIndexes* pJob_ListIndexes = _pJob_ListIndexes(pObject);

  pJob_ListIndexes->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_ListIndexes::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTINDEXES,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("lsidx"_Stz));
  
  // - - - - - - - - - - - - - - - - - - - - -
  // get no of tokens in token vector
  iNumTokens = pTokenVec->nTokens();;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 1 tokens
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
void Job_ListIndexes::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTINDEXES,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_ListIndexes::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTINDEXES);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_ListIndexes_pre(Interface::SEitf* pSEitf)
{
  Job_ListIndexes* pJob_ListIndexes = new Job_ListIndexes(pSEitf);
  
  TokenVec* pTokenVec = pSEitf->m_pCommandProcessor->pTknVec();
  
  bool bResult = pJob_ListIndexes->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_ListIndexes->m_bHasError = true;
    pJob_ListIndexes->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_ListIndexes;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_ListIndexes_post(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_ListIndexes* pJob_ListIndexes = _pJob_ListIndexes(pJob);
  
  if ( pJob_ListIndexes->m_bHasError ) {
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("ERROR (NODE%lld): %s\n"_Stz, i_Node, aEC[pJob_ListIndexes->m_xNode[i_Node].m_eErrorCode]);
    }
  }
  else {
    if (gpAttachMap->m_bMetadata) {
      vTrace("metadata: %s\n"_Stz, pJob_ListIndexes->m_sMetadata.aStr());
    }
    
    i8 iRowsProcessed = 0;
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("%s\n"_Stz, pJob_ListIndexes->m_xNode[i_Node].m_sResponse.aChar());
      iRowsProcessed += pJob_ListIndexes->m_xNode[i_Node].m_iRowsProcessed;
    }
    if (gpAttachMap->m_bMetadata) {
      vTrace("success: %lld rows affected\n"_Stz, iRowsProcessed);
    }
  }
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_ListIndexes_delete(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_ListIndexes* pJob_ListIndexes = _pJob_ListIndexes(pJob);
  
  // now delete the job
  delete pJob_ListIndexes;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_ListIndexes_show(Job* pJob)
{
  Job_ListIndexes* pJob_ListIndexes = _pJob_ListIndexes(pJob);

  pJob_ListIndexes->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
