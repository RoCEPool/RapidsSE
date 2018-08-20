/*============================================================================*/
/* Job_ListTenants.cpp -- List Tenants Job                                    */
/*============================================================================*/

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobNote mJobNote_ListTenants (
  "Job_ListTenants"_Stz,  // name of the job class
  CN_JOB_LISTTENANTS,     // class enum
  JK_SINGLENODE,          // job kind
  "list tenants"_Stz,     // comment about the job
  "lsten"_Stz,            // the prompt command
  nullptr,                // the second word
  "listTenants_req"_Stz,  // the _req task seed 
  nullptr,                // the _rcv task seed 
  nullptr,                // the _exe task seed
  nullptr,                // the _end task seed
  nullptr,                // the _can task seed
  false,                  // check resources?
  false,                  // detached job?
  false,                  // buffer attached?
  JT_NONE,                // job trace
  pJob_ListTenants_pre,   // prompt pre-process function
  vJob_ListTenants_post,  // prompt post-process function
  nullptr,                // job new function
  vJob_ListTenants_delete,// job delete function
  nullptr,                // job pre-send function
  nullptr,                // job post-receive function
  vJob_ListTenants_show,  // job show function
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
Job_ListTenants* JobsModel::_pJob_ListTenants(void* aAddress)
{
  Job_ListTenants* pJob_ListTenants = reinterpret_cast<Job_ListTenants*>(aAddress);
  
  //RASSERT( !pJob_ListTenants || CN_JOB_LISTTENANTS == pJob_ListTenants->eClass());
  
  return pJob_ListTenants;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_ListTenants::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_ListTenants* pJob_ListTenants =
    static_cast<Job_ListTenants*>(Heap::pAllocate(CN_JOB_LISTTENANTS));
  
  return pJob_ListTenants;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_ListTenants::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_ListTenants::operator delete(void* aAddress)
{
  Job_ListTenants* pJob_ListColumns = static_cast<Job_ListTenants*>(aAddress);
  
  Heap::vFree(pJob_ListColumns);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_ListTenants::Job_ListTenants(Interface::SEitf* pSEitf)
: Job(CN_JOB_LISTTENANTS, pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTTENANTS,_FOOTER_);
  
  m_sSeed    = mJobNote_ListTenants.pJobClass()->m_aSeedReq;

  _checkInitialised_(CN_JOB_LISTTENANTS);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTTENANTS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_ListTenants::~Job_ListTenants(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTTENANTS,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_ListTenants::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_LISTTENANTS == eClass);
  RASSERT(pObject->eClass() == CN_JOB_LISTTENANTS);
  
  Job_ListTenants* pJob_ListTenants = _pJob_ListTenants(pObject);

  pJob_ListTenants->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_ListTenants::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTTENANTS,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("lsten"_Stz));
  
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
void Job_ListTenants::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTTENANTS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_ListTenants::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTTENANTS);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_ListTenants_pre(Interface::SEitf* pSEitf)
{
  Job_ListTenants* pJob_ListTenants = new Job_ListTenants(pSEitf);
  
  TokenVec* pTokenVec = pSEitf->m_pCommandProcessor->pTknVec();
  
  bool bResult = pJob_ListTenants->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_ListTenants->m_bHasError = true;
    pJob_ListTenants->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_ListTenants;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_ListTenants_post(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_ListTenants* pJob_ListTenants = _pJob_ListTenants(pJob);
  
  if ( pJob_ListTenants->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("ERROR (NODE%lld): %s\n"_Stz, i_Node, aEC[pJob_ListTenants->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
  else {
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("%s\n"_Stz, pJob_ListTenants->m_xNode[i_Node].m_sResponse.aChar());
    }
  }
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_ListTenants_delete(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_ListTenants* pJob_ListTenants = _pJob_ListTenants(pJob);
  
  delete pJob_ListTenants;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_ListTenants_show(Job* pJob)
{
  Job_ListTenants* pJob_ListTenants = _pJob_ListTenants(pJob);

  pJob_ListTenants->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
