/*============================================================================*/
/* Job_SubPublish.cpp -- Sub-Publish Job                                      */
/*============================================================================*/

// Job Short: SubPublish

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_SubPublish =
{
  "Job_SubPublish"_Stz,   // name of the job class
  CN_JOB_SUBPUBLISH,      // class enum
  JK_CHILD,               // job kind
  "table sub-publish"_Stz,// comment about the job
  nullptr,                // the prompt command
  nullptr,                // the second word
  "subPublish_run"_Stz,   // the _run task seed
  nullptr,                // the _req task seed
  nullptr,                // the _rcv task seed
  nullptr,                // the _exe task seed
  nullptr,                // the _end task seed
  nullptr,                // the _can task seed
  false,                  // check resources?
  false,                  // detached job?
  false,                  // buffer attached?
  JT_NONE,                // job trace
  pJob_SubPublish_pre,    // prompt pre-process function
  vJob_SubPublish_post,   // prompt post-process function
  nullptr,                // job new function
  vJob_SubPublish_delete, // job delete function
  nullptr,                // job pre-send function
  nullptr,                // job post-receive function
  vJob_SubPublish_show,   // job show function
  
  // help and example
  false,
  "<none>"_Stz,
  "<none>"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* JobModel::pJob_SubPublish_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_SubPurger_class;
  
  return &xJobClass_SubPublish;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_SubPublish* JobModel::_pJob_SubPublish(void* aAddress)
{
  Job_SubPublish* pJob_SubPublish = reinterpret_cast<Job_SubPublish*>(aAddress);
  
  RASSERT_TYPE(pJob_SubPublish,CN_JOB_SUBPUBLISH);
  
  return pJob_SubPublish;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_SubPublish::operator new(size_t)
{
  Job_SubPublish* pJob_SubPublish =
    static_cast<Job_SubPublish*>(HeapMem::pAllocate(CN_JOB_SUBPUBLISH));
  
  return pJob_SubPublish;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_SubPublish::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_SubPublish::operator delete(void* aAddress)
{
  Job_SubPublish* pJob_SubPublish = static_cast<Job_SubPublish*>(aAddress);
  
  HeapMem::vFree(pJob_SubPublish);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_SubPublish::Job_SubPublish(Interface::Session* pSession)
: Job(CN_JOB_SUBPUBLISH, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBPUBLISH,_FOOTER_);
    
  m_iPublishJobId = -1;
  m_iSlot         = -1;
  
  m_pTable        = nullptr;
  m_pEntryBuffer  = nullptr;
  m_pRowAgg       = new RowAgg;
  
  _checkInitialised_(CN_JOB_SUBPUBLISH);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBPUBLISH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_SubPublish::~Job_SubPublish(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBPUBLISH,_FOOTER_);
  
  delete m_pRowAgg;
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_SubPublish::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_SubPublish* pJob_SubPublish = _pJob_SubPublish(pObject);

  pJob_SubPublish->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* begin sub-publish                                                          */
/*----------------------------------------------------------------------------*/
bool Job_SubPublish::bBegin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBPUBLISH,_FOOTER_);
  
  m_pRowAgg->vTable(m_pTable);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* chunk rows                                                                 */
/*----------------------------------------------------------------------------*/
bool Job_SubPublish::bChunk(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBPUBLISH,_FOOTER_);
  
  if (! m_pEntryBuffer) {
    m_pEntryBuffer = m_pTable->pRootBuffer(m_iSlot,0);
    if (! m_pEntryBuffer)
      return false;
  }
  else {
    if (! m_pEntryBuffer->pNextBuffer())
      return false;
    m_pEntryBuffer = m_pEntryBuffer->pNextBuffer();
  }
 
  m_pRowAgg->vAggregate(m_iSlot, m_pEntryBuffer);
  
  m_pRowAgg->vOutput();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* end sub-publish                                                            */
/*----------------------------------------------------------------------------*/
void Job_SubPublish::vEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBPUBLISH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_SubPublish::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBPUBLISH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* get table                                                                  */
/*----------------------------------------------------------------------------*/
Table* Job_SubPublish::pTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBPUBLISH,_FOOTER_);
  
  return m_pTable;
}

/*----------------------------------------------------------------------------*/
/* set table                                                                  */
/*----------------------------------------------------------------------------*/
void Job_SubPublish::vTable(Table* pTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBPUBLISH,_FOOTER_);
  
  m_pTable = pTable;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_SubPublish::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBPUBLISH);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobModel::pJob_SubPublish_pre(Interface::Session* pSession)
{
  Job_SubPublish* pJob_SubPublish = new Job_SubPublish(pSession);
  
  return pJob_SubPublish;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobModel::vJob_SubPublish_post(Interface::Session* pSession, Job* pJob)
{
  Job_SubPublish* pJob_SubPublish = _pJob_SubPublish(pJob);
  
  if ( pJob_SubPublish->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[pJob_SubPublish->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobModel::vJob_SubPublish_delete(Interface::Session* pSession, Job* pJob)
{
  Job_SubPublish* pJob_SubPublish = _pJob_SubPublish(pJob);
  
  delete pJob_SubPublish;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobModel::vJob_SubPublish_show(Job* pJob)
{
  Job_SubPublish* pJob_SubPublish = _pJob_SubPublish(pJob);

  pJob_SubPublish->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
