/*============================================================================*/
/* Job_ListCatalogs.cpp -- List Catalogs Job                                  */
/*============================================================================*/

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobNote mJobNote_ListCatalogs (
  "Job_ListCatalogs"_Stz,  // name of the job class
  CN_JOB_LISTCATALOGS,     // class enum
  JK_SINGLENODE,           // job kind
  "list catalogs"_Stz,     // comment about the job
  "lscat"_Stz,             // the prompt command
  nullptr,                 // the second word
  "listCatalogs_req"_Stz,  // the _req task seed
  nullptr,                 // the _rcv task seed 
  nullptr,                 // the _exe task seed
  nullptr,                 // the _end task seed
  nullptr,                 // the _can task seed
  false,                   // check resources?
  false,                   // detached job?
  false,                   // buffer attached?
  JT_NONE,                 // job trace
  pJob_ListCatalogs_pre,   // prompt pre-process function
  vJob_ListCatalogs_post,  // prompt post-process function
  nullptr,                 // job new function
  vJob_ListCatalogs_delete,// job delete function
  nullptr,                 // job pre-send function
  nullptr,                 // job post-receive function
  vJob_ListCatalogs_show,  // job show function
  true,                    // job is enabled
  "<help>"_Stz,            // help
  "<example>"_Stz          // example
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_ListCatalogs* JobsModel::_pJob_ListCatalogs(void* aAddress)
{
  Job_ListCatalogs* pJob_ListCatalogs = reinterpret_cast<Job_ListCatalogs*>(aAddress);
  
  //RASSERT( !pJob_ListCatalogs || CN_JOB_LISTCATALOGS == pJob_ListCatalogs->eClass());
  
  return pJob_ListCatalogs;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_ListCatalogs::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_ListCatalogs* pJob_ListCatalogs =
    static_cast<Job_ListCatalogs*>(Heap::pAllocate(CN_JOB_LISTCATALOGS));
  
  return pJob_ListCatalogs;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_ListCatalogs::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_ListCatalogs::operator delete(void* aAddress)
{
  Job_ListCatalogs* pJob_ListCatalogs = static_cast<Job_ListCatalogs*>(aAddress);
  
  Heap::vFree(pJob_ListCatalogs);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_ListCatalogs::Job_ListCatalogs(Interface::SEitf* pSEitf)
: Job(CN_JOB_LISTCATALOGS, pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTCATALOGS,_FOOTER_);
  
  m_sSeed    = mJobNote_ListCatalogs.pJobClass()->m_aSeedReq;

  _checkInitialised_(CN_JOB_LISTCATALOGS);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTCATALOGS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_ListCatalogs::~Job_ListCatalogs(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTCATALOGS,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_ListCatalogs::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_LISTCATALOGS == eClass);
  RASSERT(pObject->eClass() == CN_JOB_LISTCATALOGS);
  
  Job_ListCatalogs* pJob_ListCatalogs = _pJob_ListCatalogs(pObject);

  pJob_ListCatalogs->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_ListCatalogs::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTCATALOGS,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("lscat"_Stz));
  
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
void Job_ListCatalogs::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTCATALOGS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_ListCatalogs::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTCATALOGS);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_ListCatalogs_pre(Interface::SEitf* pSEitf)
{
  Job_ListCatalogs* pJob_ListCatalogs = new Job_ListCatalogs(pSEitf);
  
  TokenVec* pTokenVec = pSEitf->m_pCommandProcessor->pTknVec();
  
  bool bResult = pJob_ListCatalogs->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_ListCatalogs->m_bHasError = true;
    pJob_ListCatalogs->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_ListCatalogs;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_ListCatalogs_post(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_ListCatalogs* pJob_ListCatalogs = _pJob_ListCatalogs(pJob);
  
  if ( pJob_ListCatalogs->m_bHasError ) {
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("ERROR (NODE%lld): %s\n"_Stz, i_Node, aEC[pJob_ListCatalogs->m_xNode[i_Node].m_eErrorCode]);
    }
  }
  else {
    if (gpAttachMap->m_bMetadata) {
      vTrace("metadata: %s\n"_Stz, pJob_ListCatalogs->m_sMetadata.aStr());
    }
    
    i8 iRowsProcessed = 0;
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("%s\n"_Stz, pJob_ListCatalogs->m_xNode[i_Node].m_sResponse.aChar());
      iRowsProcessed += pJob_ListCatalogs->m_xNode[i_Node].m_iRowsProcessed;
    }
    if (gpAttachMap->m_bMetadata) {
      vTrace("success: %lld rows affected\n"_Stz, iRowsProcessed);
    }
  }
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_ListCatalogs_delete(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_ListCatalogs* pJob_ListCatalogs = _pJob_ListCatalogs(pJob);
  
  delete pJob_ListCatalogs;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_ListCatalogs_show(Job* pJob)
{
  Job_ListCatalogs* pJob_ListCatalogs = _pJob_ListCatalogs(pJob);

  pJob_ListCatalogs->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
