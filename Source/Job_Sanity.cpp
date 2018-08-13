/*============================================================================*/
/* Job_Sanity.cpp -- Sanity Job                                               */
/*============================================================================*/
#include "StorageEngine.h"
#include "JobModel.h"

extern AttachMap*      gpAttachMap;

JobNote mJobNote_Sanity (
  "Job_Sanity"_Stz,  // name of the job class
  CN_JOB_SANITY,     // class enum
  JK_DIST2PC,        // job kind
  "sanity"_Stz,      // comment about the job
  "sanity"_Stz,      // the prompt command
  nullptr,           // the second word
  "sanity"_Stz,      // the task seed for this job
  false,             // check resources?
  false,             // detached job?
  JT_NONE,           // job trace
  pJob_Sanity_pre,   // prompt pre-process function
  vJob_Sanity_post,  // prompt post-process function
  nullptr,           // job new function
  vJob_Sanity_delete,// job delete function
  vJob_Sanity_show,  // job show function
  true,              // job is enabled
  "<help>"_Stz,      // help
  "<example>"_Stz    // example
);


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Sanity* JobsModel::_pJob_Sanity(void* aAddress)
{
  Job_Sanity* pJob_Sanity = reinterpret_cast<Job_Sanity*>(aAddress);
  
  //RASSERT( !pJob_Status || CN_JOB_SANITY == Job_Space->eClass());
  
  return pJob_Sanity;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Sanity::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_Sanity* pJob_Sanity =
    static_cast<Job_Sanity*>(Heap::pAllocate(CN_JOB_SANITY));
  
  return pJob_Sanity;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Sanity::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Sanity::operator delete(void* aAddress)
{
  Job_Sanity* pJob_Status = static_cast<Job_Sanity*>(aAddress);
  
  Heap::vFree(pJob_Status);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Sanity::Job_Sanity(Interface::SEitf* pSEitf)
: Job(CN_JOB_SANITY, pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SANITY,_FOOTER_);
    
  m_sSeed    = mJobNote_Sanity.pJobClass()->m_aSeed;
  
  m_iRegionSize = gpAttachMap->m_iRegionSize;
  m_iPartitions = gpAttachMap->m_pGrid->m_nSlots;
  
  _checkInitialised_(CN_JOB_STATUS);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_SANITY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Sanity::~Job_Sanity(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SANITY,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Sanity::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_SANITY == eClass);
  RASSERT(pObject->eClass() == CN_JOB_SANITY);
  
  Job_Sanity* pJob_Sanity = _pJob_Sanity(pObject);

  pJob_Sanity->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_Sanity::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SANITY,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("sanity"_Stz));
  
  // - - - - - - - - - - - - - - - - - - - - -
  // get number of items in token
  iNumTokens = pTokenVec->nItems();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 1 token
  if (iNumTokens != 1)
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Sanity::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SANITY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Sanity::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SANITY);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_Sanity_pre(Interface::SEitf* pSEitf)
{
  Job_Sanity* pJob_Sanity = new Job_Sanity(pSEitf);
  
  TokenVec* pTokenVec = pSEitf->m_pCommandProcessor->pTknVec();
  
  bool bResult = pJob_Sanity->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_Sanity->m_bHasError = true;
    pJob_Sanity->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_Sanity;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_Sanity_post(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_Sanity* pJob_Sanity = _pJob_Sanity(pJob);
  
  if ( pJob_Sanity->m_bHasError ) {
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("ERROR (NODE%lld): %s\n"_Stz, i_Node, aEC[pJob_Sanity->m_xNode[i_Node].m_eErrorCode]);
    }
  }
  else {
    if (gpAttachMap->m_bMetadata) {
      vTrace("metadata: %s\n"_Stz, pJob_Sanity->m_sMetadata.aStr());
    }
    
    i8 iRowsProcessed = 0;
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("%s\n"_Stz, pJob_Sanity->m_xNode[i_Node].m_sResponse.aChar());
      iRowsProcessed += pJob_Sanity->m_xNode[i_Node].m_iRowsProcessed;
    }
    if (gpAttachMap->m_bMetadata) {
      vTrace("success: %lld rows affected\n"_Stz, iRowsProcessed);
    }
  }
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_Sanity_delete(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_Sanity* pJob_Sanity = _pJob_Sanity(pJob);
  
  delete pJob_Sanity;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_Sanity_show(Job* pJob)
{
  Job_Sanity* pJob_Sanity = _pJob_Sanity(pJob);

  pJob_Sanity->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
