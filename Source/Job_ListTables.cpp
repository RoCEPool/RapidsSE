/*============================================================================*/
/* Job_ListTables.cpp -- List Tables Job                                      */
/*============================================================================*/

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobNote mJobNote_ListTables (
  "Job_ListTables"_Stz,   // name of the job class
  CN_JOB_LISTTABLES,      // class enum
  JK_SINGLENODE,          // job kind
  "list tables"_Stz,      // comment about the job
  "lstbl"_Stz,            // the prompt command
  nullptr,                // the second word
  "listTables_req"_Stz,   // the _req task seed 
  nullptr,                // the _rcv task seed 
  nullptr,                // the _exe task seed
  false,                  // check resources?
  false,                  // detached job?
  false,                  // buffer attached?
  JT_NONE,                // job trace
  pJob_ListTables_pre,    // prompt pre-process function
  vJob_ListTables_post,   // prompt post-process function
  nullptr,                // job new function
  vJob_ListTables_delete, // job delete function
  nullptr,                // job pre-send function
  nullptr,                // job post-receive function
  vJob_ListTables_show,   // job show function
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
Job_ListTables* JobsModel::_pJob_ListTables(void* aAddress)
{
  Job_ListTables* pJob_ListTables = reinterpret_cast<Job_ListTables*>(aAddress);
  
  //RASSERT( !pJob_ListTables || CN_JOB_LISTTABLES == pJob_ListTables->eClass());
  
  return pJob_ListTables;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_ListTables::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_ListTables* pJob_ListTables =
    static_cast<Job_ListTables*>(Heap::pAllocate(CN_JOB_LISTTABLES));
  
  return pJob_ListTables;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_ListTables::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_ListTables::operator delete(void* aAddress)
{
  Job_ListTables* pJob_ListTables = static_cast<Job_ListTables*>(aAddress);
  
  Heap::vFree(pJob_ListTables);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_ListTables::Job_ListTables(Interface::SEitf* pSEitf)
: Job(CN_JOB_LISTTABLES, pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTTABLES,_FOOTER_);
  
  m_sSeed    = mJobNote_ListTables.pJobClass()->m_aSeed1;

  _checkInitialised_(CN_JOB_LF);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTTABLES,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_ListTables::~Job_ListTables(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTTABLES,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_ListTables::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_LISTTABLES == eClass);
  RASSERT(pObject->eClass() == CN_JOB_LISTTABLES);
  
  Job_ListTables* pJob_ListTables = _pJob_ListTables(pObject);

  pJob_ListTables->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_ListTables::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTTABLES,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("lstbl"_Stz));
  
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
void Job_ListTables::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTTABLES,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_ListTables::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTTABLES);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_ListTables_pre(Interface::SEitf* pSEitf)
{
  Job_ListTables* pJob_ListTables = new Job_ListTables(pSEitf);
  
  TokenVec* pTokenVec = pSEitf->m_pCommandProcessor->pTknVec();
  
  bool bResult = pJob_ListTables->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_ListTables->m_bHasError = true;
    pJob_ListTables->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_ListTables;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_ListTables_post(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_ListTables* pJob_ListTables = _pJob_ListTables(pJob);
  
  if ( pJob_ListTables->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("ERROR (NODE%lld): %s\n"_Stz, i_Node, aEC[pJob_ListTables->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
  else {
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("%s\n"_Stz, pJob_ListTables->m_xNode[i_Node].m_sResponse.aChar());
    }
  }
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_ListTables_delete(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_ListTables* pJob_ListTables = _pJob_ListTables(pJob);
  
  delete pJob_ListTables;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_ListTables_show(Job* pJob)
{
  Job_ListTables* pJob_ListTables = _pJob_ListTables(pJob);

  pJob_ListTables->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
