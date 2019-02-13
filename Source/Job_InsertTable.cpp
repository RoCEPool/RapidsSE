/*============================================================================*/
/* Job_InsertTable.cpp -- Insert Row Job                                        */
/*============================================================================*/

// Job Short: EndInsert

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/* example:
 
 ld lineitem "/tpch/SF1/lineitem.csv" ;
 
 */

JobClass xJobClass_EndInsert =
{
  "Job_InsertTable"_Stz,    // name of the job class
  CN_JOB_INSERTTABLE,       // class enum
  JK_DIST2PC,               // job kind
  "insert table"_Stz,       // comment about the job
  nullptr,                  // the prompt command
  nullptr,                  // the second word
  nullptr,                  // the _run task seed
  "insertTable_req"_Stz,    // the _req task seed
  "insertTable_beg"_Stz,    // the _rcv task seed
  "insertTable_exe"_Stz,    // the _exe task seed
  "insertTable_end"_Stz,    // the _end task seed
  "insertTable_can"_Stz,    // the _can task seed
  false,                    // check resources?
  false,                    // detached job?
  false,                    // buffer attached?
  JT_NONE,                  // job trace
  
  eJob_checkCluster,        // prompt check function
  pJob_InsertTable_pre,     // prompt pre-process function
  vJob_InsertTable_post,    // prompt post-process function
  pJob_InsertTable_new,     // job new function
  vJob_InsertTable_delete,  // job delete function
  vJob_send,                // job pre-send function
  vJob_recv,                // job post-receive function
  vJob_InsertTable_show,    // job show function
  
  // help and example
  false,
  "<none>"_Stz,
  "<none>"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_InsertTable_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_Lf_class;
  
  return &xJobClass_EndInsert;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_InsertTable* Jobs::_pJob_InsertTable(void* aAddress)
{
  Job_InsertTable* pJob_InsertTable = reinterpret_cast<Job_InsertTable*>(aAddress);
  
  RASSERT_TYPE(pJob_InsertTable,CN_JOB_INSERTTABLE);
  
  return pJob_InsertTable;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_InsertTable::operator new(size_t)
{
  Job_InsertTable* pJob_InsertTable =
    static_cast<Job_InsertTable*>(HeapMem::pAllocate(CN_JOB_INSERTTABLE));
  
  return pJob_InsertTable;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_InsertTable::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_InsertTable::operator delete(void* aAddress)
{
  Job_InsertTable* pJob_InsertTable = static_cast<Job_InsertTable*>(aAddress);
  
  HeapMem::vFree(pJob_InsertTable);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_InsertTable::Job_InsertTable(Interface::Session* pSession)
: Job(CN_JOB_INSERTTABLE, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INSERTTABLE,_FOOTER_);
  
  m_pTable = nullptr;
  m_pJobSubInserter = nullptr;
  
  _checkInitialised_(CN_JOB_INSERTTABLE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_INSERTTABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_InsertTable::~Job_InsertTable(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INSERTTABLE,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_InsertTable::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_InsertTable* pJob_InsertTable = _pJob_InsertTable(pObject);

  pJob_InsertTable->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_InsertTable::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INSERTTABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_InsertTable::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INSERTTABLE);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_InsertTable_pre(Interface::Session* pSession)
{
  Job_InsertTable* pJob_InsertTable = new Job_InsertTable(pSession);
  
  // get a jobId for the insertTable job
//  i8 iJobId = gpAttachMap->m_pOriginatingJobs->iNextJobId();
//  pJob_InsertTable->m_iJobId = iJobId;
  
  return pJob_InsertTable;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_InsertTable_post(Interface::Session* pSession, Job* pJob)
{
  Job_InsertTable* pJob_InsertTable = _pJob_InsertTable(pJob);
  
  if ( pJob_InsertTable->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[pJob_InsertTable->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
}

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_InsertTable_new(Job* pArg)
{
  Job_InsertTable* pJob_InsertTable = new Job_InsertTable(nullptr);
  
  return pJob_InsertTable;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_InsertTable_delete(Interface::Session* pSession, Job* pJob)
{
  Job_InsertTable* pJob_InsertTable = _pJob_InsertTable(pJob);
  
  //vTrace("DELETING ENDINSERT\n"_Stz);
  delete pJob_InsertTable;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_InsertTable_show(Job* pJob)
{
  Job_InsertTable* pJob_InsertTable = _pJob_InsertTable(pJob);

  pJob_InsertTable->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
