/*============================================================================*/
/* Job_ListPartitions.cpp -- List Partitions Job                              */
/*============================================================================*/

// Job Short: ListPartitions

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_ListPartitions =
{
  "Job_ListPartitions"_Stz,// name of the job class
  CN_JOB_LISTPARTITIONS,   // class enum
  JK_LOCAL,                // job kind
  "list partitions"_Stz,   // comment about the job
  "lspart"_Stz,            // the prompt command
  nullptr,                 // the second word
  nullptr,                 // the _run task seed
  "listPartitions_req"_Stz,// the _req task seed
  nullptr,                 // the _exe task seed
  nullptr,                 // the _rcv task seed
  nullptr,                 // the _end task seed
  nullptr,                 // the _can task seed
  false,                   // check resources?
  false,                   // detached job?
  false,                   // buffer attached?
  JT_NONE,                 // job trace
  pJob_ListPartitions_pre, // prompt pre-process function
  vJob_ListPartitions_post,// prompt post-process function
  nullptr,                 // job new function
  vJob_ListPartitions_delete, // job delete function
  nullptr,                 // job pre-send function
  nullptr,                 // job post-receive function
  vJob_ListPartitions_show,// job show function
  true,                    // job is enabled
  "<help>"_Stz,            // help
  "<example>"_Stz,         // example

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* JobsModel::pJob_ListPartitions_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_ListSchemas_class;
  
  return &xJobClass_ListPartitions;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_ListPartitions* JobsModel::_pJob_ListPartitions(void* aAddress)
{
  Job_ListPartitions* pJob_ListPartitions = reinterpret_cast<Job_ListPartitions*>(aAddress);
  
  //RASSERT( !pJob_ListPartitions || CN_JOB_LISTPARTITIONS == pJob_ListPartitions->eClass());
  
  return pJob_ListPartitions;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_ListPartitions::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_ListPartitions* pJob_ListPartitions =
    static_cast<Job_ListPartitions*>(Heap::pAllocate(CN_JOB_LISTPARTITIONS));
  
  return pJob_ListPartitions;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_ListPartitions::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_ListPartitions::operator delete(void* aAddress)
{
  Job_ListPartitions* pJob_ListPartitions = static_cast<Job_ListPartitions*>(aAddress);
  
  Heap::vFree(pJob_ListPartitions);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_ListPartitions::Job_ListPartitions(Interface::SEitf* pSEitf)
: Job(CN_JOB_LISTPARTITIONS, pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTPARTITIONS,_FOOTER_);
  
  _checkInitialised_(CN_JOB_LISTPARTITIONS);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTPARTITIONS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_ListPartitions::~Job_ListPartitions(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTPARTITIONS,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_ListPartitions::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_LISTPARTITIONS == eClass);
  RASSERT(pObject->eClass() == CN_JOB_LISTPARTITIONS);
  
  Job_ListPartitions* pJob_ListPartitions = _pJob_ListPartitions(pObject);

  pJob_ListPartitions->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_ListPartitions::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTPARTITIONS,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("lspart"_Stz));
  
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
void Job_ListPartitions::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTPARTITIONS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_ListPartitions::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTPARTITIONS);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_ListPartitions_pre(Interface::SEitf* pSEitf)
{
  Job_ListPartitions* pJob_ListPartitions = new Job_ListPartitions(pSEitf);
  
  TokenVec* pTokenVec = pSEitf->m_pCommandProcessor->pTknVec();
  
  bool bResult = pJob_ListPartitions->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_ListPartitions->m_bHasError = true;
    pJob_ListPartitions->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_ListPartitions;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_ListPartitions_post(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_ListPartitions* pJob_ListPartitions = _pJob_ListPartitions(pJob);
  
  if ( pJob_ListPartitions->m_bHasError ) {
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("ERROR (NODE%lld): %s\n"_Stz, i_Node, aEC[pJob_ListPartitions->m_xNode[i_Node].m_eErrorCode]);
    }
  }
  else {
    if (gpAttachMap->m_bMetadata) {
      vTrace("metadata: %s\n"_Stz, pJob_ListPartitions->m_sMetadata.aStr());
    }
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("%s\n"_Stz, pJob_ListPartitions->m_xNode[i_Node].m_sResponse.aChar());
    }
    vTrace("success: %lld rows affected\n"_Stz, gpAttachMap->m_pGrid->m_nNodes);
  }
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_ListPartitions_delete(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_ListPartitions* pJob_ListPartitions = _pJob_ListPartitions(pJob);
  
  delete pJob_ListPartitions;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_ListPartitions_show(Job* pJob)
{
  Job_ListPartitions* pJob_ListPartitions = _pJob_ListPartitions(pJob);

  pJob_ListPartitions->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
