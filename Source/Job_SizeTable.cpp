/*============================================================================*/
/* Job_SizeTable.cpp -- size table Job                                        */
/*============================================================================*/

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobNote mJobNote_SizeTable (
  "Job_SizeTable"_Stz,   // name of the job class
  CN_JOB_SIZETABLE,      // class enum
  JK_DIST2PC,            // job kind
  "size table"_Stz,      // comment about the job
  "sz"_Stz,              // the prompt command
  nullptr,               // the second word
  "sizeTable"_Stz,       // the task seed for this job
  true,                  // check resources?
  false,                 // detached job?
  JT_NONE,               // job trace
  pJob_SizeTable_pre,    // prompt pre-process function
  vJob_SizeTable_post,   // prompt post-process function
  nullptr,               // job new function
  vJob_SizeTable_delete, // job delete function
  nullptr,               // job send function
  nullptr,               // job receive function
  vJob_SizeTable_show,   // job show function
  true,                  // job is enabled
  "<help>"_Stz,          // help
  "<example>"_Stz        // example
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_SizeTable* JobsModel::_pJob_SizeTable(void* aAddress)
{
  Job_SizeTable* pJob_SizeTable = reinterpret_cast<Job_SizeTable*>(aAddress);
  
  RASSERT( !pJob_SizeTable || CN_JOB_SIZETABLE == pJob_SizeTable->eClass());
  
  return pJob_SizeTable;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_SizeTable::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_SizeTable* pJob_SizeTable =
    static_cast<Job_SizeTable*>(Heap::pAllocate(CN_JOB_SIZETABLE));
  
  return pJob_SizeTable;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_SizeTable::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_SizeTable::operator delete(void* aAddress)
{
  Job_SizeTable* pJob_SizeTable = static_cast<Job_SizeTable*>(aAddress);
  
  Heap::vFree(pJob_SizeTable);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_SizeTable::Job_SizeTable(Interface::SEitf* pSEitf)
: Job(CN_JOB_SIZETABLE, pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SIZETABLE,_FOOTER_);
    
  m_sSeed = mJobNote_SizeTable.pJobClass()->m_aSeed;
  m_bCheckResources   = true;
  
  _checkInitialised_(CN_JOB_SIZETABLE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_SIZETABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_SizeTable::~Job_SizeTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SIZETABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_SizeTable::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_SIZETABLE == eClass);
  RASSERT(pObject->eClass() == CN_JOB_SIZETABLE);
  
  Job_SizeTable* pJob_SizeTable = _pJob_SizeTable(pObject);

  pJob_SizeTable->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_SizeTable::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SIZETABLE,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("sz"_Stz));
  
  // get number of items in token vector
  iNumTokens = pTokenVec->nItems();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 2 tokens
  if (iNumTokens != 2)
    return false;
  
  // set the table name
  m_sObjectName = pTokenVec->pGet(1)->aText();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_SizeTable::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SIZETABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_SizeTable::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SIZETABLE);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_SizeTable_pre(Interface::SEitf* pSEitf)
{
  Job_SizeTable* pJob_SizeTable = new Job_SizeTable(pSEitf);
  
  TokenVec* pTokenVec = pSEitf->m_pCommandProcessor->pTknVec();
  
  bool bResult = pJob_SizeTable->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_SizeTable->m_bHasError = true;
    pJob_SizeTable->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_SizeTable;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_SizeTable_post(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_SizeTable* pJob_SizeTable = _pJob_SizeTable(pJob);
  
  if ( pJob_SizeTable->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("ERROR (NODE%lld): %s\n"_Stz, i_Node, aEC[pJob_SizeTable->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
  else {
    if (gpAttachMap->m_bMetadata) {
      vTrace("metadata: %s\n"_Stz, pJob_SizeTable->m_sMetadata.aStr());
    }
    
    i8 iRowsProcessed = 0;
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("%s\n"_Stz, pJob_SizeTable->m_xNode[i_Node].m_sResponse.aChar());
      iRowsProcessed += pJob_SizeTable->m_xNode[i_Node].m_iRowsProcessed;
    }
    if (gpAttachMap->m_bMetadata) {
      vTrace("success: %lld rows affected\n"_Stz, iRowsProcessed);
    }
  }
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_SizeTable_delete(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_SizeTable* pJob_SizeTable = _pJob_SizeTable(pJob);
  
  delete pJob_SizeTable;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_SizeTable_show(Job* pJob)
{
  Job_SizeTable* pJob_SizeTable = _pJob_SizeTable(pJob);

  pJob_SizeTable->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
