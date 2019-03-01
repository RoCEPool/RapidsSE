/*============================================================================*/
/* Job_SubTruncate.cpp -- Sub Truncate Job                                    */
/*============================================================================*/

// Job Short: SubTruncate

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_SubTruncate =
{
  CN_JOB_SUBTRUNCATE,        // class enum
  JK_CHILD,                  // job kind
  "table sub-truncate"_Stz,  // comment about the job
  nullptr,                   // the prompt command
  nullptr,                   // the second word
  "subTruncate_run"_Stz,     // the _run task seed
  nullptr,                   // the _req task seed
  nullptr,                   // the _rcv task seed
  nullptr,                   // the _exe task seed
  nullptr,                   // the _end task seed
  nullptr,                   // the _can task seed
  false,                     // check resources?
  false,                     // detached job?
  false,                     // buffer attached?
  JT_NONE,                   // job trace
  DL_NONE,                   // ddl lock model
  PY_NORMAL,                 // priority
  JF_NONE,                   // flags

  nullptr,                   // prompt check function
  pJob_SubTruncate_pre,      // prompt pre-process function
  vJob_SubTruncate_post,     // prompt post-process function
  nullptr,                   // job new function
  vJob_SubTruncate_delete,   // job delete function
  nullptr,                   // job pre-send function
  nullptr,                   // job post-receive function
  vJob_SubTruncate_show,     // job show function
  
  // help and example
  false,
  "<none>"_Stz,
  "<none>"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_SubTruncate_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_SubWalker_class;
  
  return &xJobClass_SubTruncate;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_SubTruncate* Jobs::_pJob_SubTruncate(void* aAddress)
{
  Job_SubTruncate* pJob_TruncateTable = reinterpret_cast<Job_SubTruncate*>(aAddress);
  
  RASSERT_TYPE(pJob_TruncateTable,CN_JOB_SUBTRUNCATE);
  
  return pJob_TruncateTable;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_SubTruncate::operator new(size_t)
{
  Job_SubTruncate* pJob_SubTruncate =
    static_cast<Job_SubTruncate*>(HeapMem::pAllocate(CN_JOB_SUBTRUNCATE));
  
  return pJob_SubTruncate;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_SubTruncate::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_SubTruncate::operator delete(void* aAddress)
{
  Job_SubTruncate* pJob_SubTruncate = static_cast<Job_SubTruncate*>(aAddress);
  
  HeapMem::vFree(pJob_SubTruncate);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_SubTruncate::Job_SubTruncate(Interface::Session* pSession)
: Job(CN_JOB_SUBTRUNCATE, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBTRUNCATE,_FOOTER_);
    
  m_iTruncateJobId  = -1;
  
  _checkInitialised_(CN_JOB_SUBTRUNCATE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBTRUNCATE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_SubTruncate::~Job_SubTruncate(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBTRUNCATE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_SubTruncate::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_SubTruncate* pJob_SubTruncate = _pJob_SubTruncate(pObject);

  pJob_SubTruncate->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_SubTruncate::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBTRUNCATE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_SubTruncate::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBTRUNCATE);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_SubTruncate_pre(Interface::Session* pSession)
{
  Job_SubTruncate* pJob_SubTruncate = new Job_SubTruncate(pSession);
  
  return pJob_SubTruncate;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_SubTruncate_post(Interface::Session* pSession, Job* pJob)
{
  Job_SubTruncate* pJob_SubTruncate = _pJob_SubTruncate(pJob);
  
  if ( pJob_SubTruncate->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[pJob_SubTruncate->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_SubTruncate_delete(Interface::Session* pSession, Job* pJob)
{
  Job_SubTruncate* pJob_SubTruncate = _pJob_SubTruncate(pJob);
  
  delete pJob_SubTruncate;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_SubTruncate_show(Job* pJob)
{
  Job_SubTruncate* pJob_SubTruncate = _pJob_SubTruncate(pJob);

  pJob_SubTruncate->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
