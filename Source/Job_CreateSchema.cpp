/*============================================================================*/
/* Job_CreateSchema.cpp -- Create Schema Job                                  */
/*============================================================================*/

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobNote mJobNote_CreateSchema (
  "Job_CreateSchema"_Stz,   // name of the job class
  CN_JOB_CREATESCHEMA,      // class enum
  JK_DIST2PC,               // job kind
  "create schema"_Stz,      // comment about the job
  "create"_Stz,             // the prompt command
  "schema"_Stz,             // the second word
  "createSchema"_Stz,       // the task seed for this job
  true,                     // check resources?
  false,                    // detached job?
  JT_NONE,                  // job trace
  pJob_CreateSchema_pre,    // prompt pre-process function
  vJob_CreateSchema_post,   // prompt post-process function
  nullptr,                  // job new function
  vJob_CreateSchema_delete, // job delete function
  nullptr,                  // job receive function
  vJob_CreateSchema_show,   // job show function
  true,                     // job is enabled
  "<help>"_Stz,             // help
  "<example>"_Stz           // example
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_CreateSchema* JobsModel::_pJob_CreateSchema(void* aAddress)
{
  Job_CreateSchema* pJob_CreateSchema = reinterpret_cast<Job_CreateSchema*>(aAddress);
  
  //RASSERT( !pJob_CreateSchema || CN_JOB_CREATESCHEMA == pJob_CreateSchema->eClass());
  
  return pJob_CreateSchema;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_CreateSchema::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_CreateTable* pJob_CreateSchema =
    static_cast<Job_CreateTable*>(Heap::pAllocate(CN_JOB_CREATESCHEMA));
  
  return pJob_CreateSchema;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_CreateSchema::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_CreateSchema::operator delete(void* aAddress)
{
  Job_CreateSchema* pJob_CreateSchema = static_cast<Job_CreateSchema*>(aAddress);
  
  Heap::vFree(pJob_CreateSchema);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_CreateSchema::Job_CreateSchema(Interface::SEitf* pSEitf)
: Job(CN_JOB_CREATESCHEMA, pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATESCHEMA,_FOOTER_);
    
  m_sSeed = mJobNote_CreateSchema.pJobClass()->m_aSeed;
  m_bCheckResources   = true;
  
  _checkInitialised_(CN_JOB_CREATESCHEMA);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATESCHEMA,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_CreateSchema::~Job_CreateSchema(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATESCHEMA,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_CreateSchema::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_CREATESCHEMA == eClass);
  RASSERT(pObject->eClass() == CN_JOB_CREATESCHEMA);
  
  Job_CreateSchema* pJob_CreateSchema = _pJob_CreateSchema(pObject);

  pJob_CreateSchema->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_CreateSchema::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATESCHEMA,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("create"_Stz));
  ASSERT(pTokenVec->pGet(1)->bSame("schema"_Stz));
  
  // get number of items in token vector
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
void Job_CreateSchema::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATESCHEMA,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_CreateSchema::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATESCHEMA);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_CreateSchema_pre(Interface::SEitf* pSEitf)
{
  Job_CreateSchema* pJob_CreateSchema = new Job_CreateSchema(pSEitf);
  
  TokenVec* pTokenVec = pSEitf->m_pCommandProcessor->pTknVec();
  
  bool bResult = pJob_CreateSchema->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_CreateSchema->m_bHasError = true;
    pJob_CreateSchema->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_CreateSchema;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_CreateSchema_post(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_CreateSchema* pJob_CreateSchema = _pJob_CreateSchema(pJob);
  
  if ( pJob_CreateSchema->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("ERROR (NODE%lld): %s\n"_Stz, i_Node, aEC[pJob_CreateSchema->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_CreateSchema_delete(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_CreateSchema* pJob_CreateSchema = _pJob_CreateSchema(pJob);
  
  // now delete the job
  delete pJob_CreateSchema;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_CreateSchema_show(Job* pJob)
{
  Job_CreateSchema* pJob_CreateSchema = _pJob_CreateSchema(pJob);

  pJob_CreateSchema->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
