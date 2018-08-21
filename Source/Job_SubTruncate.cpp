/*============================================================================*/
/* Job_SubTruncate.cpp -- Sub Truncate Job                                    */
/*============================================================================*/

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobNote mJobNote_SubTruncate (
  "Job_SubTruncate"_Stz,     // name of the job class
  CN_JOB_SUBTRUNCATE,        // class enum
  JK_ONESHOT,                // job kind
  "table sub-truncate"_Stz,  // comment about the job
  nullptr,                   // the prompt command
  nullptr,                   // the second word
  "subTruncate_seed"_Stz,    // the _req task seed 
  nullptr,                   // the _rcv task seed 
  nullptr,                   // the _exe task seed
  nullptr,                   // the _end task seed
  nullptr,                   // the _can task seed
  false,                     // check resources?
  false,                     // detached job?
  false,                     // buffer attached?
  JT_NONE,                   // job trace
  pJob_SubTruncate_pre,      // prompt pre-process function
  vJob_SubTruncate_post,     // prompt post-process function
  nullptr,                   // job new function
  vJob_SubTruncate_delete,   // job delete function
  nullptr,                   // job pre-send function
  nullptr,                   // job post-receive function
  vJob_SubTruncate_show,     // job show function
  false,                     // job is enabled
  "<help>"_Stz,              // help
  "<example>"_Stz            // example
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_SubTruncate* JobsModel::_pJob_SubTruncate(void* aAddress)
{
  Job_SubTruncate* pJob_TruncateTable = reinterpret_cast<Job_SubTruncate*>(aAddress);
  
  RASSERT( !pJob_TruncateTable || CN_JOB_SUBTRUNCATE == pJob_TruncateTable->eClass());
  
  return pJob_TruncateTable;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_SubTruncate::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_SubTruncate* pJob_SubTruncate =
    static_cast<Job_SubTruncate*>(Heap::pAllocate(CN_JOB_SUBTRUNCATE));
  
  return pJob_SubTruncate;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_SubTruncate::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_SubTruncate::operator delete(void* aAddress)
{
  Job_SubTruncate* pJob_SubTruncate = static_cast<Job_SubTruncate*>(aAddress);
  
  Heap::vFree(pJob_SubTruncate);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_SubTruncate::Job_SubTruncate(Interface::SEitf* pSEitf)
: Job(CN_JOB_SUBTRUNCATE, pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBTRUNCATE,_FOOTER_);
    
  m_sSeed = mJobNote_SubTruncate.pJobClass()->m_aSeedReq;
  m_bCheckResources = false;
  m_iTruncateJobId  = -1;
  m_iSlot           = -1;
  
  _checkInitialised_(CN_JOB_SUBTRUNCATE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBTRUNCATE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_SubTruncate::~Job_SubTruncate(void)
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
  RASSERT(CN_JOB_SUBTRUNCATE == eClass);
  RASSERT(pObject->eClass() == CN_JOB_SUBTRUNCATE);
  
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
Job* JobsModel::pJob_SubTruncate_pre(Interface::SEitf* pSEitf)
{
  Job_SubTruncate* pJob_SubTruncate = new Job_SubTruncate(pSEitf);
  
  return pJob_SubTruncate;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_SubTruncate_post(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_SubTruncate* pJob_SubTruncate = _pJob_SubTruncate(pJob);
  
  if ( pJob_SubTruncate->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("ERROR (NODE%lld): %s\n"_Stz, i_Node, aEC[pJob_SubTruncate->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_SubTruncate_delete(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_SubTruncate* pJob_SubTruncate = _pJob_SubTruncate(pJob);
  
  delete pJob_SubTruncate;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_SubTruncate_show(Job* pJob)
{
  Job_SubTruncate* pJob_SubTruncate = _pJob_SubTruncate(pJob);

  pJob_SubTruncate->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
