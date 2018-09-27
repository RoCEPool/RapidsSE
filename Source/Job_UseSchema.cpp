/*============================================================================*/
/* Job_UseSchema.cpp -- Use Schema Job                                        */
/*============================================================================*/

// Job Short: UseSchema

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_UseSchema =
{
  "Job_UseSchema"_Stz,   // name of the job class
  CN_JOB_USESCHEMA,      // class enum
  JK_LOCAL,              // job kind
  "use schema"_Stz,      // comment about the job
  "use"_Stz,             // the prompt command
  "schema"_Stz,          // the second word
  nullptr,               // the _run task seed
  "useSchema_req"_Stz,   // the _req task seed
  nullptr,               // the _rcv task seed
  nullptr,               // the _exe task seed
  nullptr,               // the _end task seed
  nullptr,               // the _can task seed
  false,                 // check resources?
  false,                 // detached job?
  false,                 // buffer attached?
  JT_NONE,               // job trace
  pJob_UseSchema_pre,    // prompt pre-process function
  vJob_UseSchema_post,   // prompt post-process function
  nullptr,               // job new function
  vJob_UseSchema_delete, // job delete function
  nullptr,               // job pre-send function
  nullptr,               // job post-receive function
  vJob_UseSchema_show,   // job show function
  true,                  // job is enabled
  "<help>"_Stz,          // help
  "<example>"_Stz,       // example

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* JobsModel::pJob_UseSchema_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_VerifyIndex_class;
  
  return &xJobClass_UseSchema;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_UseSchema* JobsModel::_pJob_UseSchema(void* aAddress)
{
  Job_UseSchema* pJob_UseSchema = reinterpret_cast<Job_UseSchema*>(aAddress);
  
  //RASSERT( !pJob_UseSchema || CN_JOB_USESCHEMA == pJob_UseSchema->eClass());
  
  return pJob_UseSchema;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_UseSchema::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_UseSchema* pJob_ListCatalogs =
    static_cast<Job_UseSchema*>(Heap::pAllocate(CN_JOB_USESCHEMA));
  
  return pJob_ListCatalogs;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_UseSchema::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_UseSchema::operator delete(void* aAddress)
{
  Job_UseSchema* pJob_UseSchema = static_cast<Job_UseSchema*>(aAddress);
  
  Heap::vFree(pJob_UseSchema);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_UseSchema::Job_UseSchema(Interface::SEitf* pSEitf)
: Job(CN_JOB_USESCHEMA, pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_USESCHEMA,_FOOTER_);
  
  _checkInitialised_(CN_JOB_USESCHEMA);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_USESCHEMA,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_UseSchema::~Job_UseSchema(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_USESCHEMA,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_UseSchema::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_USESCHEMA == eClass);
  RASSERT(pObject->eClass() == CN_JOB_USESCHEMA);
  
  Job_UseSchema* pJob_UseSchema = _pJob_UseSchema(pObject);

  pJob_UseSchema->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_UseSchema::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_USESCHEMA,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("use"_Stz));
  ASSERT(pTokenVec->pGet(1)->bSame("schema"_Stz));
  
  // - - - - - - - - - - - - - - - - - - - - -
  // get no of tokens in token vector
  iNumTokens = pTokenVec->nTokens();;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens != 3) {
    return nullptr;
  }
  
  // - - - - - - - - - - - - - - - - - - - - -
  // set the table name
  m_sObjectName = pTokenVec->pGet(2)->aText();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_UseSchema::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_USESCHEMA,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_UseSchema::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_USESCHEMA);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_UseSchema_pre(Interface::SEitf* pSEitf)
{
  Job_UseSchema* pJob_UseSchema = new Job_UseSchema(pSEitf);
  
  TokenVec* pTokenVec = pSEitf->m_pCommandProcessor->pTknVec();
  
  bool bResult = pJob_UseSchema->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_UseSchema->m_bHasError = true;
    pJob_UseSchema->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_UseSchema;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_UseSchema_post(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_UseSchema* pJob_UseSchema = _pJob_UseSchema(pJob);
  
  if ( pJob_UseSchema->m_bHasError ) {
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("error: (node%lld)%s\n"_Stz, i_Node, aEC[pJob_UseSchema->m_xNode[i_Node].m_eErrorCode]);
    }
  }
  else {
    vTrace("success: 0 rows affected\n"_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_UseSchema_delete(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_UseSchema* pJob_UseSchema = _pJob_UseSchema(pJob);
  
  delete pJob_UseSchema;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_UseSchema_show(Job* pJob)
{
  Job_UseSchema* pJob_UseSchema = _pJob_UseSchema(pJob);

  pJob_UseSchema->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
