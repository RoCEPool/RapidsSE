/*============================================================================*/
/* Job_DropSchema.cpp -- Drop Schema Job                                      */
/*============================================================================*/

// Job Short: DropSchema

#include "Jobs.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_DropSchema =
{
  "Job_DropSchema"_Stz,   // name of the job class
  CN_JOB_DROPSCHEMA,      // class enum
  JK_DIST2PC,             // job kind
  "drop schema"_Stz,      // comment about the job
  "drop"_Stz,             // the prompt command
  "schema"_Stz,           // the second word
  nullptr,                // the _run task seed
  "dropSchema_req"_Stz,   // the _req task seed
  "dropSchema_beg"_Stz,   // the _rcv task seed
  "dropSchema_exe"_Stz,   // the _exe task seed
  "dropSchema_end"_Stz,   // the _end task seed
  "dropSchema_can"_Stz,   // the _can task seed
  true,                   // check resources?
  false,                  // detached job?
  false,                  // buffer attached?
  JT_NONE,                // job trace
  DL_NONE,                // ddl lock model

  eJob_checkCluster,      // prompt check function
  pJob_DropSchema_pre,    // prompt pre-process function
  vJob_DropSchema_post,   // prompt post-process function
  nullptr,                // job new function
  vJob_DropSchema_delete, // job delete function
  vJob_send,              // job pre-send function
  vJob_recv,              // job post-receive function
  vJob_DropSchema_show,   // job show function
  
  // help and example
  true,
  "drop schema <schemaName>"_Stz,
  "create schema tpchSchema;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_DropSchema_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_DropTable_class;
  
  return &xJobClass_DropSchema;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_DropSchema* Jobs::_pJob_DropSchema(void* aAddress)
{
  Job_DropSchema* pJob_DropSchema = reinterpret_cast<Job_DropSchema*>(aAddress);
  
  RASSERT_TYPE(pJob_DropSchema,CN_JOB_DROPSCHEMA);
  
  return pJob_DropSchema;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_DropSchema::operator new(size_t)
{
  Job_DropSchema* pJob_DropSchema =
    static_cast<Job_DropSchema*>(HeapMem::pAllocate(CN_JOB_DROPSCHEMA));
  
  return pJob_DropSchema;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_DropSchema::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_DropSchema::operator delete(void* aAddress)
{
  Job_DropSchema* pJob_DropSchema = static_cast<Job_DropSchema*>(aAddress);
  
  HeapMem::vFree(pJob_DropSchema);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_DropSchema::Job_DropSchema(Interface::Session* pSession)
: Job(CN_JOB_DROPSCHEMA, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPSCHEMA,_FOOTER_);
    
  _checkInitialised_(CN_JOB_DROPSCHEMA);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPSCHEMA,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_DropSchema::~Job_DropSchema(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPSCHEMA,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_DropSchema::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_DropSchema* pJob_DropSchema = _pJob_DropSchema(pObject);

  pJob_DropSchema->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_DropSchema::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPSCHEMA,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("drop"_Stz));
  ASSERT(pTokenVec->pGet(1)->bSame("schema"_Stz));
  
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
void Job_DropSchema::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPSCHEMA,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_DropSchema::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPSCHEMA);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_DropSchema_pre(Interface::Session* pSession)
{
  Job_DropSchema* pJob_DropSchema = new Job_DropSchema(pSession);
  
  TokenVec* pTokenVec = pSession->m_pCommand->pTknVec();
  
  bool bResult = pJob_DropSchema->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_DropSchema->m_bHasError = true;
    pJob_DropSchema->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_DropSchema;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_DropSchema_post(Interface::Session* pSession, Job* pJob)
{
  Job_DropSchema* pJob_DropSchema = _pJob_DropSchema(pJob);
  
  if ( pJob_DropSchema->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[pJob_DropSchema->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
  else {
    vTrace("success: 0 rows affected\n"_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_DropSchema_delete(Interface::Session* pSession, Job* pJob)
{
  Job_DropSchema* pJob_DropSchema = _pJob_DropSchema(pJob);
  
  delete pJob_DropSchema;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_DropSchema_show(Job* pJob)
{
  Job_DropSchema* pJob_DropSchema = _pJob_DropSchema(pJob);

  pJob_DropSchema->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
