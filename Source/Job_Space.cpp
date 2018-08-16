/*============================================================================*/
/* Job_Space.cpp -- Space Job                                                 */
/*============================================================================*/

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobNote mJobNote_Space (
  "Job_space"_Stz,  // name of the job class
  CN_JOB_SPACE,     // class enum
  JK_DIST2PC,       // job kind
  "space"_Stz,      // comment about the job
  "space"_Stz,      // the prompt command
  nullptr,          // the second word
  "space"_Stz,      // the task seed for this job
  nullptr,          // the second task seed for this job
  false,            // check resources?
  false,            // detached job?
  false,            // buffer attached?
  JT_NONE,          // job trace
  pJob_Space_pre,   // prompt pre-process function
  vJob_Space_post,  // prompt post-process function
  nullptr,          // job new function
  vJob_Space_delete,// job delete function
  nullptr,          // job send function
  nullptr,          // job receive function
  vJob_Space_show,  // job show function
  true,             // job is enabled
  "<help>"_Stz,     // help
  "<example>"_Stz   // example
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Space* JobsModel::_pJob_Space(void* aAddress)
{
  Job_Space* pJob_Space = reinterpret_cast<Job_Space*>(aAddress);
  
  //RASSERT( !Job_Space || CN_JOB_SPACE == Job_Space->eClass());
  
  return pJob_Space;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Space::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_Space* pJob_Space =
    static_cast<Job_Space*>(Heap::pAllocate(CN_JOB_SPACE));
  
  return pJob_Space;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Space::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Space::operator delete(void* aAddress)
{
  Job_Space* pJob_Space = static_cast<Job_Space*>(aAddress);
  
  Heap::vFree(pJob_Space);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Space::Job_Space(Interface::SEitf* pSEitf)
: Job(CN_JOB_SPACE, pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SPACE,_FOOTER_);
  
  m_sSeed    = mJobNote_Space.pJobClass()->m_aSeed1;

  _checkInitialised_(CN_JOB_SPACE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_SPACE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Space::~Job_Space(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SPACE,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Space::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_SPACE == eClass);
  RASSERT(pObject->eClass() == CN_JOB_SPACE);
  
  Job_Space* pJob_Space = _pJob_Space(pObject);

  pJob_Space->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_Space::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SPACE,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("space"_Stz));
  
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
void Job_Space::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SPACE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Space::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SPACE);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_Space_pre(Interface::SEitf* pSEitf)
{
  Job_Space* pJob_Space = new Job_Space(pSEitf);
  
  TokenVec* pTokenVec = pSEitf->m_pCommandProcessor->pTknVec();
  
  bool bResult = pJob_Space->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_Space->m_bHasError = true;
    pJob_Space->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_Space;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_Space_post(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_Space* pJob_Space = _pJob_Space(pJob);

  if ( pJob_Space->m_bHasError ) {
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("ERROR (NODE%lld): %s\n"_Stz, i_Node, aEC[pJob_Space->m_xNode[i_Node].m_eErrorCode]);
    }
  }
  else {
    if (gpAttachMap->m_bMetadata) {
      vTrace("metadata: %s\n"_Stz, pJob_Space->m_sMetadata.aStr());
    }
    
    i8 iRowsProcessed = 0;
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("%s\n"_Stz, pJob_Space->m_xNode[i_Node].m_sResponse.aChar());
      iRowsProcessed += pJob_Space->m_xNode[i_Node].m_iRowsProcessed;
    }
    if (gpAttachMap->m_bMetadata) {
      vTrace("success: %lld rows affected\n"_Stz, iRowsProcessed);
    }
  }
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_Space_delete(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_Space* pJob_Space = _pJob_Space(pJob);
  
  delete pJob_Space;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_Space_show(Job* pJob)
{
  Job_Space* pJob_Space = _pJob_Space(pJob);

  pJob_Space->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
