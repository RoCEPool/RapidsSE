/*============================================================================*/
/* Job_DropCatalog.cpp -- Drop Catalog Job                                    */
/*============================================================================*/

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobNote mJobNote_DropCatalog (
  "Job_DropCatalog"_Stz,          // name of the job class
  CN_JOB_DROPCATALOG,             // class enum
  JK_DIST2PC,                     // job kind
  "drop catalog catalogName"_Stz, // comment about the job
  "drop"_Stz,                     // the prompt command
  "catalog"_Stz,                  // the second word
  "dropCatalog"_Stz,              // the task seed for this job
  true,                           // check resources?
  false,                          // detached job?
  pJob_DropCatalog_pre,           // prompt pre-process function
  vJob_DropCatalog_post,          // prompt post-process function
  vJob_DropCatalog_delete,        // job delete function
  true,                           // job is enabled
  "<help>"_Stz,                   // help
  "<example>"_Stz                 // example
);


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_DropCatalog* JobsModel::_pJob_DropCatalog(void* aAddress)
{
  Job_DropCatalog* pJob_DropCatalog = reinterpret_cast<Job_DropCatalog*>(aAddress);
  
  RASSERT( !pJob_DropCatalog || CN_JOB_DROPCATALOG == pJob_DropCatalog->eClass());
  
  return pJob_DropCatalog;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_DropCatalog::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_DropCatalog* pJob_DropSchema =
    static_cast<Job_DropCatalog*>(Heap::pAllocate(CN_JOB_DROPCATALOG));
  
  return pJob_DropSchema;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_DropCatalog::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_DropCatalog::operator delete(void* aAddress)
{
  Job_DropCatalog* pJob_DropCatalog = static_cast<Job_DropCatalog*>(aAddress);
  
  Heap::vFree(pJob_DropCatalog);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_DropCatalog::Job_DropCatalog(Interface::SEitf* pSEitf)
: Job(CN_JOB_DROPCATALOG), JobDicPath(pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPCATALOG,_FOOTER_);
    
  m_sSeed = mJobNote_DropCatalog.pJobClass()->m_aSeed;
  m_bCheckResources   = true;
  
  _checkInitialised_(CN_JOB_DROPCATALOG);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPCATALOG,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_DropCatalog::~Job_DropCatalog(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPCATALOG,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_DropCatalog::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_DROPCATALOG == eClass);
  RASSERT(pObject->eClass() == CN_JOB_DROPCATALOG);
  
  Job_DropCatalog* pJob_DropCatalog = _pJob_DropCatalog(pObject);

  pJob_DropCatalog->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_DropCatalog::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPCATALOG,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("drop"_Stz));
  ASSERT(pTokenVec->pGet(1)->bSame("catalog"_Stz));
  
  // - - - - - - - - - - - - - - - - - - - - -
  // get number of items in token
  iNumTokens = pTokenVec->nItems();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens != 3)
  return false;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // set the table name
  m_sObjectName = pTokenVec->pGet(2)->aText();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_DropCatalog::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPCATALOG,_FOOTER_);
  
                                                               //! Alka to write
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_DropCatalog::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPCATALOG);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_DropCatalog_pre(Interface::SEitf* pSEitf)
{
  Job_DropCatalog* pJob_DropCatalog = new Job_DropCatalog(pSEitf);
  
  TokenVec* pTokenVec = pSEitf->m_pCommandProcessor->pTknVec();
  
  bool bResult = pJob_DropCatalog->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_DropCatalog->m_bHasError = true;
    pJob_DropCatalog->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_DropCatalog;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_DropCatalog_post(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_DropCatalog* pJob_DropCatalog = _pJob_DropCatalog(pJob);
  
  if ( pJob_DropCatalog->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("ERROR (NODE%lld): %s\n"_Stz, i_Node, aEC[pJob_DropCatalog->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_DropCatalog_delete(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_DropCatalog* pJob_DropCatalog = _pJob_DropCatalog(pJob);
  
  // now delete the job
  delete pJob_DropCatalog;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
