/*============================================================================*/
/* Job_CompactIndex.cpp -- size index Job                                        */
/*============================================================================*/

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobNote mJobNote_CompactIndex (
  "Job_CompactIndex"_Stz, // name of the job class
  CN_JOB_COMPACTINDEX,    // class enum
  JK_DIST2PC,             // job kind
  "compact index"_Stz,    // comment about the job
  "compactidx"_Stz,       // the prompt command
  nullptr,                // the second word
  nullptr,                // the _run task seed
  "compactIndex_req"_Stz, // the _req task seed
  "compactIndex_beg"_Stz, // the _rcv task seed
  "compactIndex_exe"_Stz, // the _exe task seed
  "compactIndex_end"_Stz, // the _end task seed
  "compactIndex_can"_Stz, // the _can task seed
  true,                   // check resources?
  false,                  // detached job?
  false,                  // buffer attached?
  JT_NONE,                // job trace
  pJob_CompactIndex_pre,  // prompt pre-process function
  vJob_CompactIndex_post, // prompt post-process function
  nullptr,                // job new function
  vJob_CompactIndex_delete, // job delete function
  vJob_send,              // job pre-send function
  vJob_recv,              // job post-receive function
  vJob_CompactIndex_show, // job show function
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
Job_CompactIndex* JobsModel::_pJob_CompactIndex(void* aAddress)
{
  Job_CompactIndex* pJob_CompactIndex = reinterpret_cast<Job_CompactIndex*>(aAddress);
  
  RASSERT( !pJob_CompactIndex || CN_JOB_COMPACTINDEX == pJob_CompactIndex->eClass());
  
  return pJob_CompactIndex;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_CompactIndex::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_CompactIndex* pJob_CompactIndex =
    static_cast<Job_CompactIndex*>(Heap::pAllocate(CN_JOB_COMPACTINDEX));
  
  return pJob_CompactIndex;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_CompactIndex::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_CompactIndex::operator delete(void* aAddress)
{
  Job_CompactIndex* pJob_CompactIndex = static_cast<Job_CompactIndex*>(aAddress);
  
  Heap::vFree(pJob_CompactIndex);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_CompactIndex::Job_CompactIndex(Interface::SEitf* pSEitf)
: Job(CN_JOB_COMPACTINDEX, pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_COMPACTINDEX,_FOOTER_);
    
  _checkInitialised_(CN_JOB_COMPACTINDEX);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_COMPACTINDEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_CompactIndex::~Job_CompactIndex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_COMPACTINDEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_CompactIndex::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_COMPACTINDEX == eClass);
  RASSERT(pObject->eClass() == CN_JOB_COMPACTINDEX);
  
  Job_CompactIndex* pJob_CompactIndex = _pJob_CompactIndex(pObject);

  pJob_CompactIndex->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_CompactIndex::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_COMPACTINDEX,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("compactidx"_Stz));
  
  // get number of items in token vector
  iNumTokens = pTokenVec->nItems();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens != 3)
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // set the index name
  m_sIndexName = pTokenVec->pGet(2)->aText();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // set the table name
  m_sObjectName = pTokenVec->pGet(1)->aText();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_CompactIndex::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_COMPACTINDEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_CompactIndex::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_COMPACTINDEX);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_CompactIndex_pre(Interface::SEitf* pSEitf)
{
  Job_CompactIndex* pJob_CompactIndex = new Job_CompactIndex(pSEitf);
  
  TokenVec* pTokenVec = pSEitf->m_pCommandProcessor->pTknVec();
  
  bool bResult = pJob_CompactIndex->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_CompactIndex->m_bHasError = true;
    pJob_CompactIndex->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_CompactIndex;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_CompactIndex_post(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_CompactIndex* pJob_CompactIndex = _pJob_CompactIndex(pJob);
  
  if ( pJob_CompactIndex->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("ERROR (NODE%lld): %s\n"_Stz, i_Node, aEC[pJob_CompactIndex->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_CompactIndex_delete(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_CompactIndex* pJob_CompactIndex = _pJob_CompactIndex(pJob);
  
  delete pJob_CompactIndex;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_CompactIndex_show(Job* pJob)
{
  Job_CompactIndex* pJob_CompactIndex = _pJob_CompactIndex(pJob);

  pJob_CompactIndex->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
