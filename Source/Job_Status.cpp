/*============================================================================*/
/* Job_Status.cpp -- Status Job                                               */
/*============================================================================*/

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobNote mJobNote_Status (
  "status"_Stz,     // name of the job class
  CN_JOB_STATUS,    // class enum
  "status"_Stz,     // comment about the job
  "status"_Stz,     // the prompt command
  nullptr,          // the second word
  "status"_Stz,     // the task seed for this job
  false,            // detached job?
  pJob_Status_pre,  // prompt pre-process function
  vJob_Status_post, // prompt post-process function
  true              // job is enabled
  );

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Status* JobsModel::_pJob_Status(void* aAddress)
{
  Job_Status* pJob_Status = reinterpret_cast<Job_Status*>(aAddress);
  
  //RASSERT( !pJob_Status || CN_JOB_STATUS == Job_Space->eClass());
  
  return pJob_Status;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Status::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_Status* pJob_Status =
    static_cast<Job_Status*>(Heap::pAllocate(CN_JOB_STATUS));
  
  return pJob_Status;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Status::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Status::operator delete(void* aAddress)
{
  Job_Status* pJob_Status = static_cast<Job_Status*>(aAddress);
  
  Heap::vFree(pJob_Status);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Status::Job_Status(Interface::SEitf* pSEitf)
: Job(CN_JOB_STATUS), JobDicPath(pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_STATUS,_FOOTER_);
    
   m_sSeed    = mJobNote_Status.pJobClass()->m_aSeed;
  
  _checkInitialised_(CN_JOB_STATUS);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_STATUS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Status::~Job_Status(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_STATUS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Status::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_STATUS == eClass);
  RASSERT(pObject->eClass() == CN_JOB_STATUS);
  
  Job_Status* pJob_Status = _pJob_Status(pObject);

  pJob_Status->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_Status::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_STATUS,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("status"_Stz));
  
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
void Job_Status::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_STATUS,_FOOTER_);
  
                                                               //! Alka to write
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Status::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_STATUS);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_Status_pre(Interface::SEitf* pSEitf)
{
  Job_Status* pJob_Status = new Job_Status(pSEitf);
  
  TokenVec* pTokenVec = pSEitf->m_pCommandProcessor->pTknVec();
  
  bool bResult = pJob_Status->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_Status->m_bHasError = true;
    pJob_Status->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_Status;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_Status_post(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_Status* pJob_Status = _pJob_Status(pJob);
  
  if ( pJob_Status->m_bHasError ) {
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("ERROR (NODE%lld): %s\n"_Stz, i_Node, aEC[pJob_Status->m_xNode[i_Node].m_eErrorCode]);
    }
  }
  else {
    if (gpAttachMap->m_bMetadata) {
      vTrace("metadata: %s\n"_Stz, pJob_Status->m_sMetadata.aStr());
    }
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("%s\n"_Stz, pJob_Status->m_xNode[i_Node].m_sResponse.aChar());
    }
    if (gpAttachMap->m_bMetadata) {
      vTrace("success: %lld rows affected\n"_Stz, gpAttachMap->m_pGrid->m_nNodes);
    }
  }
  
  // now delete the job
  delete pJob_Status;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
