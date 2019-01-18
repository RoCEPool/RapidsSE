/*============================================================================*/
/* Job_CreateSchema.cpp -- Create Schema Job                                  */
/*============================================================================*/

// Job Short: CreateSchema

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_CreateSchema =
{
  "Job_CreateSchema"_Stz,   // name of the job class
  CN_JOB_CREATESCHEMA,      // class enum
  JK_DIST2PC,               // job kind
  "create schema"_Stz,      // comment about the job
  "create"_Stz,             // the prompt command
  "schema"_Stz,             // the second word
  nullptr,                  // the _run task seed
  "createSchema_req"_Stz,   // the _req task seed
  "createSchema_beg"_Stz,   // the _rcv task seed
  "createSchema_exe"_Stz,   // the _exe task seed
  "createSchema_end"_Stz,   // the _end task seed
  "createSchema_can"_Stz,   // the _can task seed
  true,                     // check resources?
  false,                    // detached job?
  false,                    // buffer attached?
  JT_NONE,                  // job trace
  pJob_CreateSchema_pre,    // prompt pre-process function
  vJob_CreateSchema_post,   // prompt post-process function
  nullptr,                  // job new function
  vJob_CreateSchema_delete, // job delete function
  vJob_send,                // job pre-send function
  vJob_recv,                // job post-receive function
  vJob_CreateSchema_show,   // job show function
  
  // help and example
  true,
  "create schema <schemaName>"_Stz,
  "create schema tpchSchema;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_CreateSchema_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_CreateTable_class;
  
  return &xJobClass_CreateSchema;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_CreateSchema* Jobs::_pJob_CreateSchema(void* aAddress)
{
  Job_CreateSchema* pJob_CreateSchema = reinterpret_cast<Job_CreateSchema*>(aAddress);
  
  RASSERT_TYPE(pJob_CreateSchema,CN_JOB_CREATESCHEMA);
  
  return pJob_CreateSchema;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_CreateSchema::operator new(size_t)
{
  Job_CreateTable* pJob_CreateSchema =
    static_cast<Job_CreateTable*>(HeapMem::pAllocate(CN_JOB_CREATESCHEMA));
  
  return pJob_CreateSchema;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_CreateSchema::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_CreateSchema::operator delete(void* aAddress)
{
  Job_CreateSchema* pJob_CreateSchema = static_cast<Job_CreateSchema*>(aAddress);
  
  HeapMem::vFree(pJob_CreateSchema);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_CreateSchema::Job_CreateSchema(Interface::Session* pSession)
: Job(CN_JOB_CREATESCHEMA, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATESCHEMA,_FOOTER_);
    
  _checkInitialised_(CN_JOB_CREATESCHEMA);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATESCHEMA,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_CreateSchema::~Job_CreateSchema(void) noexcept(false)
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
Job* Jobs::pJob_CreateSchema_pre(Interface::Session* pSession)
{
  Job_CreateSchema* pJob_CreateSchema = new Job_CreateSchema(pSession);
  
  TokenVec* pTokenVec = pSession->m_pCommandProcessor->pTknVec();
  
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
void Jobs::vJob_CreateSchema_post(Interface::Session* pSession, Job* pJob)
{
  Job_CreateSchema* pJob_CreateSchema = _pJob_CreateSchema(pJob);
  
  if ( pJob_CreateSchema->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[pJob_CreateSchema->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
  else {
    vTrace("success: 0 rows affected\n"_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_CreateSchema_delete(Interface::Session* pSession, Job* pJob)
{
  Job_CreateSchema* pJob_CreateSchema = _pJob_CreateSchema(pJob);
  
  // now delete the job
  delete pJob_CreateSchema;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_CreateSchema_show(Job* pJob)
{
  Job_CreateSchema* pJob_CreateSchema = _pJob_CreateSchema(pJob);

  pJob_CreateSchema->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
