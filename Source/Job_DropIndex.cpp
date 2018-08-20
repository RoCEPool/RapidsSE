/*============================================================================*/
/* Job_DropIndex.cpp -- drop index Job                                        */
/*============================================================================*/

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobNote mJobNote_DropIndex (
  "Job_DropIndex"_Stz,   // name of the job class
  CN_JOB_DROPINDEX,      // class enum
  JK_DIST2PC,            // job kind
  "drop index"_Stz,      // comment about the job
  "drop"_Stz,            // the prompt command
  "index"_Stz,           // the second word
  "dropIndex_req"_Stz,   // the _req task seed
  nullptr,               // the _rcv task seed
  "dropIndex_node"_Stz,  // the _exe task seed
  true,                  // check resources?
  false,                 // detached job?
  false,                 // buffer attached?
  JT_NONE,               // job trace
  pJob_DropIndex_pre,    // prompt pre-process function
  vJob_DropIndex_post,   // prompt post-process function
  nullptr,               // job new function
  vJob_DropIndex_delete, // job delete function
  nullptr,               // job pre-send function
  nullptr,               // job post-receive function
  vJob_DropIndex_show,   // job show function
  true,                  // job is enabled
  "<help>"_Stz,          // help
  "<example>"_Stz        // example
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_DropIndex* JobsModel::_pJob_DropIndex(void* aAddress)
{
  Job_DropIndex* pJob_DropIndex = reinterpret_cast<Job_DropIndex*>(aAddress);
  
  RASSERT( !pJob_DropIndex || CN_JOB_DROPINDEX == pJob_DropIndex->eClass());
  
  return pJob_DropIndex;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_DropIndex::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_DropIndex* pJob_DropIndex =
    static_cast<Job_DropIndex*>(Heap::pAllocate(CN_JOB_DROPINDEX));
  
  return pJob_DropIndex;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_DropIndex::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_DropIndex::operator delete(void* aAddress)
{
  Job_DropIndex* pJob_DropIndex = static_cast<Job_DropIndex*>(aAddress);
  
  Heap::vFree(pJob_DropIndex);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_DropIndex::Job_DropIndex(Interface::SEitf* pSEitf)
: Job(CN_JOB_DROPINDEX, pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPINDEX,_FOOTER_);
    
  m_sSeed           = mJobNote_DropIndex.pJobClass()->m_aSeed1;
  m_bCheckResources = true;
  
  _checkInitialised_(CN_JOB_DROPINDEX);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPINDEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_DropIndex::~Job_DropIndex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPINDEX,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_DropIndex::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_DROPINDEX == eClass);
  RASSERT(pObject->eClass() == CN_JOB_DROPINDEX);
  
  Job_DropIndex* pJob_DropIndex = _pJob_DropIndex(pObject);

  pJob_DropIndex->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_DropIndex::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPINDEX,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("drop"_Stz));
  ASSERT(pTokenVec->pGet(1)->bSame("index"_Stz));
  
  // - - - - - - - - - - - - - - - - - - - - -
  // get number of items in token
  iNumTokens = pTokenVec->nItems();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 5 tokens
  if (iNumTokens != 5)
    return false;
  
  // 4th token should be '.'
  if (!pTokenVec->pGet(3)->bSame("."_Stz)) {
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - -
  // set the table name
  m_sObjectName = pTokenVec->pGet(2)->aText();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // set the index name
  m_sIndexName = pTokenVec->pGet(4)->aText();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_DropIndex::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPINDEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_DropIndex::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPINDEX);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_DropIndex_pre(Interface::SEitf* pSEitf)
{
  Job_DropIndex* pJob_DropIndex = new Job_DropIndex(pSEitf);
  
  TokenVec* pTokenVec = pSEitf->m_pCommandProcessor->pTknVec();
  
  bool bResult = pJob_DropIndex->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_DropIndex->m_bHasError = true;
    pJob_DropIndex->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_DropIndex;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_DropIndex_post(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_DropIndex* pJob_DropIndex = _pJob_DropIndex(pJob);
  
  if ( pJob_DropIndex->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("ERROR (NODE%lld): %s\n"_Stz, i_Node, aEC[pJob_DropIndex->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_DropIndex_delete(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_DropIndex* pJob_DropIndex = _pJob_DropIndex(pJob);
  
  delete pJob_DropIndex;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_DropIndex_show(Job* pJob)
{
  Job_DropIndex* pJob_DropIndex = _pJob_DropIndex(pJob);

  pJob_DropIndex->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
