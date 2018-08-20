/*============================================================================*/
/* Job_DumpIndex.cpp -- dump index Job                                        */
/*============================================================================*/

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobNote mJobNote_DumpIndex (
  "Job_DumpIndex"_Stz,   // name of the job class
  CN_JOB_DUMPINDEX,      // class enum
  JK_DIST2PC,            // job kind
  "dump index"_Stz,      // comment about the job
  "dumpidx"_Stz,         // the prompt command
  nullptr,               // the second word
  "dumpIndex_req"_Stz,   // the _req task seed
  nullptr,               // the _rcv task seed
  "dumpIndex_node"_Stz,  // the _exe task seed
  nullptr,               // the _end task seed
  nullptr,               // the _can task seed
  true,                  // check resources?
  false,                 // detached job?
  false,                 // buffer attached?
  JT_NONE,               // job trace
  pJob_DumpIndex_pre,    // prompt pre-process function
  vJob_DumpIndex_post,   // prompt post-process function
  nullptr,               // job new function
  vJob_DumpIndex_delete, // job delete function
  nullptr,               // job pre-send function
  nullptr,               // job post-receive function
  vJob_DumpIndex_show,   // job show function
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
Job_DumpIndex* JobsModel::_pJob_DumpIndex(void* aAddress)
{
  Job_DumpIndex* pJob_DumpIndex = reinterpret_cast<Job_DumpIndex*>(aAddress);
  
  RASSERT( !pJob_DumpIndex || CN_JOB_DUMPINDEX == pJob_DumpIndex->eClass());
  
  return pJob_DumpIndex;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_DumpIndex::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_DumpIndex* pJob_DumpIndex =
    static_cast<Job_DumpIndex*>(Heap::pAllocate(CN_JOB_DUMPINDEX));
  
  return pJob_DumpIndex;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_DumpIndex::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_DumpIndex::operator delete(void* aAddress)
{
  Job_DumpIndex* pJob_DumpIndex = static_cast<Job_DumpIndex*>(aAddress);
  
  Heap::vFree(pJob_DumpIndex);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_DumpIndex::Job_DumpIndex(Interface::SEitf* pSEitf)
: Job(CN_JOB_DUMPINDEX, pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DUMPINDEX,_FOOTER_);
    
  m_sSeed = mJobNote_DumpIndex.pJobClass()->m_aSeedReq;
  m_bCheckResources = true;
  
  _checkInitialised_(CN_JOB_DUMPINDEX);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_DUMPINDEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_DumpIndex::~Job_DumpIndex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DUMPINDEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_DumpIndex::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_DUMPINDEX == eClass);
  RASSERT(pObject->eClass() == CN_JOB_DUMPINDEX);
  
  Job_DumpIndex* pJob_DumpIndex = _pJob_DumpIndex(pObject);

  pJob_DumpIndex->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_DumpIndex::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DUMPINDEX,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("dumpidx"_Stz));
  
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
void Job_DumpIndex::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DUMPINDEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_DumpIndex::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DUMPINDEX);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_DumpIndex_pre(Interface::SEitf* pSEitf)
{
  Job_DumpIndex* pJob_DumpIndex = new Job_DumpIndex(pSEitf);
  
  TokenVec* pTokenVec = pSEitf->m_pCommandProcessor->pTknVec();
  
  bool bResult = pJob_DumpIndex->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_DumpIndex->m_bHasError = true;
    pJob_DumpIndex->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_DumpIndex;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_DumpIndex_post(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_DumpIndex* pJob_DumpIndex = _pJob_DumpIndex(pJob);
  
  if ( pJob_DumpIndex->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("ERROR (NODE%lld): %s\n"_Stz, i_Node, aEC[pJob_DumpIndex->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_DumpIndex_delete(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_DumpIndex* pJob_DumpIndex = _pJob_DumpIndex(pJob);
  
  delete pJob_DumpIndex;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_DumpIndex_show(Job* pJob)
{
  Job_DumpIndex* pJob_DumpIndex = _pJob_DumpIndex(pJob);

  pJob_DumpIndex->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
