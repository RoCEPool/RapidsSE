/*============================================================================*/
/* Job_ListSchemas.cpp -- List Schemas Job                                    */
/*============================================================================*/

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobNote mJobNote_ListSchemas (
  "Job_ListSchemas"_Stz,  // name of the job class
  CN_JOB_LISTSCHEMAS,     // class enum
  JK_SINGLENODE,          // job kind
  "list schemas"_Stz,     // comment about the job
  "lssch"_Stz,            // the prompt command
  nullptr,                // the second word
  "listSchemas"_Stz,      // the task seed for this job
  false,                  // check resources?
  false,                  // detached job?
  pJob_ListSchemas_pre,   // prompt pre-process function
  vJob_ListSchemas_post,  // prompt post-process function
  true                    // job is enabled
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_ListSchemas* JobsModel::_pJob_ListSchemas(void* aAddress)
{
  Job_ListSchemas* pJob_ListSchemas = reinterpret_cast<Job_ListSchemas*>(aAddress);
  
  //RASSERT( !pJob_ListSchemas || CN_JOB_LISTSCHEMAS == pJob_ListSchemas->eClass());
  
  return pJob_ListSchemas;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_ListSchemas::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_ListSchemas* pJob_ListSchemas =
    static_cast<Job_ListSchemas*>(Heap::pAllocate(CN_JOB_LISTSCHEMAS));
  
  return pJob_ListSchemas;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_ListSchemas::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_ListSchemas::operator delete(void* aAddress)
{
  Job_ListSchemas* pJob_ListSchemas = static_cast<Job_ListSchemas*>(aAddress);
  
  Heap::vFree(pJob_ListSchemas);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_ListSchemas::Job_ListSchemas(Interface::SEitf* pSEitf)
: Job(CN_JOB_LISTSCHEMAS), JobDicPath(pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTSCHEMAS,_FOOTER_);
  
  m_sSeed    = mJobNote_ListSchemas.pJobClass()->m_aSeed;

  _checkInitialised_(CN_JOB_LISTSCHEMAS);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTSCHEMAS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_ListSchemas::~Job_ListSchemas(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTSCHEMAS,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_ListSchemas::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_LISTSCHEMAS == eClass);
  RASSERT(pObject->eClass() == CN_JOB_LISTSCHEMAS);
  
  Job_ListSchemas* pJob_ListSchemas = _pJob_ListSchemas(pObject);

  pJob_ListSchemas->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_ListSchemas::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTSCHEMAS,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("lssch"_Stz));
  
  // - - - - - - - - - - - - - - - - - - - - -
  // get no of tokens in token vector
  iNumTokens = pTokenVec->nTokens();;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 1 tokens
  if (iNumTokens != 1) {
    return nullptr;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_ListSchemas::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTSCHEMAS,_FOOTER_);
  
                                                               //! Alka to write
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_ListSchemas::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTSCHEMAS);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_ListSchemas_pre(Interface::SEitf* pSEitf)
{
  Job_ListSchemas* pJob_ListSchemas = new Job_ListSchemas(pSEitf);
  
  TokenVec* pTokenVec = pSEitf->m_pCommandProcessor->pTknVec();
  
  bool bResult = pJob_ListSchemas->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_ListSchemas->m_bHasError = true;
    pJob_ListSchemas->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_ListSchemas;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_ListSchemas_post(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_ListSchemas* pJob_ListSchemas = _pJob_ListSchemas(pJob);
  
  if ( pJob_ListSchemas->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("ERROR (NODE%lld): %s\n"_Stz, i_Node, aEC[pJob_ListSchemas->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
  else {
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("%s\n"_Stz, pJob_ListSchemas->m_xNode[i_Node].m_sResponse.aChar());
    }
  }
  // now delete the job
  delete pJob_ListSchemas;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
