/*============================================================================*/
/* Job_KidExport.cpp -- Drag ExportBuffer to export node and write it         */
/*============================================================================*/

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_WriteRecord =
{
  "Job_KidExport"_Stz,    // name of the job class
  CN_JOB_KIDEXPORT,       // class enum
  JK_GRANDKID,            // job kind
  "kid export"_Stz,       // comment about the job
  nullptr,                // the prompt command
  nullptr,                // the second word
  "kidExport_kid"_Stz,    // the _run task seed
  nullptr,                // the _req task seed
  "kidExport_rcv"_Stz,    // the _rcv task seed
  nullptr,                // the _exe task seed
  nullptr,                // the _end task seed
  nullptr,                // the _can task seed
  false,                  // check resources?
  false,                  // detached job?
  true,                   // buffer attached?
  JT_NONE,                // job trace
  
  nullptr,                // prompt check function
  pJob_KidExport_pre,     // prompt pre-process function
  nullptr,                // prompt post-process function
  pJob_KidExport_new,     // job new function
  vJob_KidExport_delete,  // job delete function
  vJob_send,              // job pre-send function
  vJob_recv,              // job post-receive function
  vJob_KidExport_show,    // job show function
  
  // help and example
  false,
  "<none>"_Stz,
  "<none>"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_KidExport_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_WriteRow_class;
  
  return &xJobClass_WriteRecord;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_KidExport* Jobs::_pJob_KidExport(void* aAddress)
{
  Job_KidExport* pJob_KidExport = reinterpret_cast<Job_KidExport*>(aAddress);
  
  RASSERT_TYPE(pJob_KidExport,CN_JOB_KIDEXPORT);
  
  return pJob_KidExport;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_KidExport::operator new(size_t)
{
  Job_KidExport* pJob_KidExport =
    static_cast<Job_KidExport*>(HeapMem::pAllocate(CN_JOB_KIDEXPORT));
  
  return pJob_KidExport;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_KidExport::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_KidExport::operator delete(void* aAddress)
{
  Job_KidExport* pJob_KidExport = static_cast<Job_KidExport*>(aAddress);
  //vTrace("DELETING INSERTBUFFER\n"_Stz);
  HeapMem::vFree(pJob_KidExport);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_KidExport::Job_KidExport(Interface::Session* pSession)
: Job(CN_JOB_KIDEXPORT, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_KIDEXPORT,_FOOTER_);
  
  m_iExportJobId = C_UNKNOWN;
  
  
  _checkInitialised_(CN_JOB_KIDEXPORT);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_KIDEXPORT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_KidExport::~Job_KidExport(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_KIDEXPORT,_FOOTER_);
  
  delete m_pCurrentBuffer;
  m_pCurrentBuffer = nullptr;
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_KidExport::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_KidExport* pJob_KidExport = _pJob_KidExport(pObject);

  pJob_KidExport->vShow();
}

/*----------------------------------------------------------------------------*/
/* spawn a KidExport job                                                      */
/*----------------------------------------------------------------------------*/
void Job_KidExport::vSpawn(i8 iExportJobId,
                          ExportBuffer* pExportBuffer)
{
  Job_KidExport* pJob_KidExport = new Job_KidExport(nullptr);
  
  pJob_KidExport->m_iExportJobId = iExportJobId;
  pJob_KidExport->m_pSendWith = pExportBuffer;
  
  pJob_KidExport->vSeed(SS_SEED_RUN);
  
  IGNORE_RETURN(Api::bSpawnJob(nullptr,pJob_KidExport,SS_SEED_RUN));
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_KidExport::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_KIDEXPORT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* write                                                                       */
/*----------------------------------------------------------------------------*/
void Job_KidExport::vWriteBuffer(Job_Export* pJob_Export)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_KIDEXPORT,_FOOTER_);
  
  ExportBuffer* pExportBuffer = _pExportBuffer(m_pSendWith);

  pJob_Export->vWriteBuffer(pExportBuffer);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_KidExport::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_KIDEXPORT);
  
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
Job* Jobs::pJob_KidExport_pre(Interface::Session* pSession)
{
  Job_KidExport* pJob_KidExport = new Job_KidExport(pSession);
  
  return pJob_KidExport;
}

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_KidExport_new(Job* pArg)
{
  Job_KidExport* pJob_KidExport = new Job_KidExport(nullptr);
  
  return pJob_KidExport;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_KidExport_delete(Interface::Session* pSession, Job* pJob)
{
  Job_KidExport* pJob_KidExport = _pJob_KidExport(pJob);
  
  //vTrace("DELETING INSERTBUFFER\n"_Stz);
  delete pJob_KidExport;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_KidExport_show(Job* pJob)
{
  Job_KidExport* pJob_KidExport = _pJob_KidExport(pJob);

  pJob_KidExport->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
