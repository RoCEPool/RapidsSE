/*============================================================================*/
/* Job_SubExport.cpp -- Sub Export Job                                        */
/*============================================================================*/

// Job Short: SubExport

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_SubExport =
{
  "Job_SubExporter"_Stz,     // name of the job class
  CN_JOB_SUBEXPORT,          // class enum
  JK_CHILD,                  // job kind
  "sub-export rows"_Stz,     // comment about the job
  "subExporter"_Stz,         // the prompt command
  nullptr,                   // the second word
  "subExporter_run"_Stz,     // the _run task seed
  nullptr,                   // the _req task seed
  nullptr,                   // the _rcv task seed
  nullptr,                   // the _exe task seed
  nullptr,                   // the _end task seed
  nullptr,                   // the _can task seed
  false,                     // check resources?
  false,                     // detached job?
  false,                     // buffer attached?
  JT_NONE,                   // job trace
  pJob_SubExport_pre,        // prompt pre-process function
  vJob_SubExport_post,       // prompt post-process function
  nullptr,                   // job new function
  vJob_SubExport_delete,     // job delete function
  nullptr,                   // job pre-send function
  nullptr,                   // job post-receive function
  vJob_SubExport_show,       // job show function
  
  // help and example
  true,
  "<none>"_Stz,
  "<none>"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_SubExport_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_SubFetch_class;
  
  return &xJobClass_SubExport;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_SubExport* Jobs::_pJob_SubExport(void* aAddress)
{
  Job_SubExport* pJob_SubExport = reinterpret_cast<Job_SubExport*>(aAddress);
  
  RASSERT_TYPE(pJob_SubExport,CN_JOB_SUBEXPORT);
  
  return pJob_SubExport;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_SubExport::operator new(size_t)
{
  Job_SubExport* pJob_SubExport =
    static_cast<Job_SubExport*>(HeapMem::pAllocate(CN_JOB_SUBEXPORT));
  
  return pJob_SubExport;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_SubExport::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_SubExport::operator delete(void* aAddress)
{
  Job_SubExport* pJob_SubExport = static_cast<Job_SubExport*>(aAddress);
  
  HeapMem::vFree(pJob_SubExport);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_SubExport::Job_SubExport(Interface::Session* pSession)
: Job(CN_JOB_SUBEXPORT, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBEXPORT,_FOOTER_);
    
  m_pJobExport  = nullptr;
  m_iSlot       = -1;
  
  _checkInitialised_(CN_JOB_SUBEXPORT);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBEXPORT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_SubExport::~Job_SubExport(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBEXPORT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_SubExport::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_SubExport* pJob_SubExport = _pJob_SubExport(pObject);

  pJob_SubExport->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_SubExport::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBEXPORT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_SubExport::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBEXPORT);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_SubExport_pre(Interface::Session* pSession)
{
  Job_SubExport* pJob_SubExport = new Job_SubExport(pSession);
  
  return pJob_SubExport;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_SubExport_post(Interface::Session* pSession, Job* pJob)
{
  Job_SubExport* pJob_SubExport = _pJob_SubExport(pJob);
  
  if ( pJob_SubExport->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[pJob_SubExport->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_SubExport_delete(Interface::Session* pSession, Job* pJob)
{
  Job_SubExport* pJob_SubExport = _pJob_SubExport(pJob);
  
  delete pJob_SubExport;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_SubExport_show(Job* pJob)
{
  Job_SubExport* pJob_SubExport = _pJob_SubExport(pJob);

  pJob_SubExport->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
