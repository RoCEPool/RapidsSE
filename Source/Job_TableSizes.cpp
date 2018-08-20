/*============================================================================*/
/* Job_TableSizes.cpp -- Table Sizes Job                                      */
/*============================================================================*/

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobNote mJobNote_TableSizes (
  "Job_TableSizes"_Stz,  // name of the job class
  CN_JOB_TABLESIZES,     // class enum
  JK_DIST2PC,            // job kind
  "table sizes"_Stz,     // comment about the job
  "tableSizes"_Stz,      // the prompt command
  nullptr,               // the second word
  "tableSizes_req"_Stz,  // the _req task seed
  nullptr,               // the _rcv task seed
  "tableSizes_node"_Stz, // the _exe task seed
  false,                 // check resources?
  false,                 // detached job?
  false,                 // buffer attached?
  JT_NONE,               // job trace
  pJob_TableSizes_pre,   // prompt pre-process function
  vJob_TableSizes_post,  // prompt post-process function
  nullptr,               // job new function
  vJob_TableSizes_delete,// job delete function
  nullptr,               // job pre-send function
  nullptr,               // job pre-receive function
  vJob_TableSizes_show,  // job show function
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
Job_TableSizes* JobsModel::_pJob_TableSizes(void* aAddress)
{
  Job_TableSizes* pJob_TableSizes = reinterpret_cast<Job_TableSizes*>(aAddress);
  
  RASSERT( !pJob_TableSizes || CN_JOB_TABLESIZES == pJob_TableSizes->eClass());
  
  return pJob_TableSizes;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_TableSizes::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_TableSizes* pJob_TableSizes =
    static_cast<Job_TableSizes*>(Heap::pAllocate(CN_JOB_TABLESIZES));
  
  return pJob_TableSizes;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_TableSizes::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_TableSizes::operator delete(void* aAddress)
{
  Job_TableSizes* pJob_TableSizes = static_cast<Job_TableSizes*>(aAddress);
  
  Heap::vFree(pJob_TableSizes);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_TableSizes::Job_TableSizes(Interface::SEitf* pSEitf)
: Job(CN_JOB_TABLESIZES, pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_TABLESIZES,_FOOTER_);
    
  m_sSeed = mJobNote_TableSizes.pJobClass()->m_aSeed1;
  m_bCheckResources   = true;
  
  _checkInitialised_(CN_JOB_TABLESIZES);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_TABLESIZES,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_TableSizes::~Job_TableSizes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_TABLESIZES,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_TableSizes::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_TABLESIZES == eClass);
  RASSERT(pObject->eClass() == CN_JOB_TABLESIZES);
  
  Job_TableSizes* pJob_TableSizes = _pJob_TableSizes(pObject);

  pJob_TableSizes->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_TableSizes::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_TABLESIZES,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("tableSizes"_Stz));
  
  // get number of items in token vector
  iNumTokens = pTokenVec->nItems();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 1 tokens
  if (iNumTokens != 1)
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_TableSizes::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_TABLESIZES,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_TableSizes::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_TABLESIZES);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_TableSizes_pre(Interface::SEitf* pSEitf)
{
  Job_TableSizes* pJob_TableSizes = new Job_TableSizes(pSEitf);
  
  TokenVec* pTokenVec = pSEitf->m_pCommandProcessor->pTknVec();
  
  bool bResult = pJob_TableSizes->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_TableSizes->m_bHasError = true;
    pJob_TableSizes->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_TableSizes;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_TableSizes_post(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_TableSizes* pJob_TableSizes = _pJob_TableSizes(pJob);
  
  if ( pJob_TableSizes->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("ERROR (NODE%lld): %s\n"_Stz, i_Node, aEC[pJob_TableSizes->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
  else {
    if (gpAttachMap->m_bMetadata) {
      vTrace("metadata: %s\n"_Stz, pJob_TableSizes->m_sMetadata.aStr());
    }
    
    i8 iRowsProcessed = 0;
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("%s\n"_Stz, pJob_TableSizes->m_xNode[i_Node].m_sResponse.aChar());
      iRowsProcessed += pJob_TableSizes->m_xNode[i_Node].m_iRowsProcessed;
    }
    if (gpAttachMap->m_bMetadata) {
      vTrace("success: %lld rows affected\n"_Stz, iRowsProcessed);
    }
  }
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_TableSizes_delete(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_TableSizes* pJob_TableSizes = _pJob_TableSizes(pJob);
  
  delete pJob_TableSizes;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_TableSizes_show(Job* pJob)
{
  Job_TableSizes* pJob_TableSizes = _pJob_TableSizes(pJob);

  pJob_TableSizes->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
