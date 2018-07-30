/*============================================================================*/
/* Job_DropIndex.cpp -- drop index Job                                        */
/*============================================================================*/

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobNote mJobNote_DropIndex (
  "Job_DropIndex"_Stz,        // name of the job class
  CN_JOB_DROPINDEX,           // class enum
  "drop index indexName"_Stz, // comment about the job
  "drop"_Stz,                 // the prompt command
  "index"_Stz,                // the second word
  "dropIndex"_Stz,            // the task seed for this job
  false,                      // detached job?
  pJob_DropIndex_pre,         // prompt pre-process function
  vJob_DropIndex_post,        // prompt post-process function
  true
);


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_DropIndex* JobsModel::_pJob_DropIndex(void* aAddress)
{
  Job_DropIndex* pJob_DropIndex = reinterpret_cast<Job_DropIndex*>(aAddress);
  
  RASSERT( !pJob_DropIndex || CN_JOB_DROPINDEX == pJob_DropIndex->eClass());
  
  return pJob_DropIndex;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_DropIndex::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_DropIndex* pJob_DropIndex =
    static_cast<Job_DropIndex*>(Heap::pAllocate(CN_JOB_DROPINDEX));
  
  return pJob_DropIndex;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_DropIndex::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_DropIndex::operator delete(void* aAddress)
{
  Job_DropIndex* pJob_DropIndex = static_cast<Job_DropIndex*>(aAddress);
  
  Heap::vFree(pJob_DropIndex);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_DropIndex::Job_DropIndex(Interface::SEitf* pSEitf)
: Job(CN_JOB_DROPINDEX), JobDicPath(pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPINDEX,_FOOTER_);
    
  //m_sExecute = "Job_DropTable.execute"_Stz;
  
  _checkInitialised_(CN_JOB_DROPINDEX);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPINDEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_DropIndex::~Job_DropIndex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPINDEX,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_DropIndex::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_DROPINDEX == eClass);
  RASSERT(pObject->eClass() == CN_JOB_DROPINDEX);
  
  Job_DropIndex* pJob_DropIndex = _pJob_DropIndex(pObject);

  pJob_DropIndex->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_DropIndex::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPINDEX,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("drop"_Stz));
  ASSERT(pTokenVec->pGet(1)->bSame("index"_Stz));
  
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
void Job_DropIndex::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPINDEX,_FOOTER_);
  
                                                               //! Alka to write
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_DropIndex::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPINDEX);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_DropIndex_pre(Interface::SEitf* pSEitf)
{
  Job_DropIndex* pJob_DropIndex = new Job_DropIndex(pSEitf);
  
  TokenVec* pTokenVec = pSEitf->m_pCommandProcessor->pTknVec();
  
  bool bResult = pJob_DropIndex->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_DropIndex->m_bHasError = true;
    pJob_DropIndex->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_DropIndex;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_DropIndex_post(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_DropIndex* pJob_DropIndex = _pJob_DropIndex(pJob);
  
  if ( pJob_DropIndex->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("ERROR (NODE%lld): %s\n"_Stz, i_Node, aEC[pJob_DropIndex->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
  
  // now delete the job
  delete pJob_DropIndex;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
