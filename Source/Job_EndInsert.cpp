/*============================================================================*/
/* Job_EndInsert.cpp -- Insert Row Job                                        */
/*============================================================================*/

// Job Short: EndInsert

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/* example:
 
 ld lineitem "/tpch/lineitem.tbl" ;
 
 */

JobClass xJobClass_EndInsert =
{
  "Job_EndInsert"_Stz,    // name of the job class
  CN_JOB_ENDINSERT,       // class enum
  JK_DIST2PC,             // job kind
  "end insert"_Stz,       // comment about the job
  nullptr,                // the prompt command
  nullptr,                // the second word
  nullptr,                // the _run task seed
  "endInsert_req"_Stz,    // the _req task seed
  "endInsert_beg"_Stz,    // the _rcv task seed
  "endInsert_exe"_Stz,    // the _exe task seed
  "endInsert_end"_Stz,    // the _end task seed
  "endInsert_can"_Stz,    // the _can task seed
  false,                  // check resources?
  false,                  // detached job?
  false,                  // buffer attached?
  JT_NONE,                // job trace
  pJob_EndInsert_pre,     // prompt pre-process function
  vJob_EndInsert_post,    // prompt post-process function
  pJob_EndInsert_new,     // job new function
  vJob_EndInsert_delete,  // job delete function
  nullptr,                // job pre-send function
  nullptr,                // job post-receive function
  vJob_EndInsert_show,    // job show function
  true,                   // job is enabled
  "<help>"_Stz,           // help
  "<example>"_Stz,        // example

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* JobsModel::pJob_EndInsert_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_FetchRows_class;
  
  return &xJobClass_EndInsert;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_EndInsert* JobsModel::_pJob_EndInsert(void* aAddress)
{
  Job_EndInsert* pJob_EndInsert = reinterpret_cast<Job_EndInsert*>(aAddress);
  
  //RASSERT( !pJob_EndInsert || CN_JOB_ENDINSERT == pJob_EndInsert->eClass());
  
  return pJob_EndInsert;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_EndInsert::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_EndInsert* pJob_EndInsert =
    static_cast<Job_EndInsert*>(HeapMem::pAllocate(CN_JOB_ENDINSERT));
  
  return pJob_EndInsert;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_EndInsert::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_EndInsert::operator delete(void* aAddress)
{
  Job_EndInsert* pJob_EndInsert = static_cast<Job_EndInsert*>(aAddress);
  
  HeapMem::vFree(pJob_EndInsert);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_EndInsert::Job_EndInsert(Interface::SEitf* pSEitf)
: Job(CN_JOB_ENDINSERT, pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_ENDINSERT,_FOOTER_);
  
  m_pTask = nullptr;
  m_iInsertRowJobId = -1;
  
  _checkInitialised_(CN_JOB_ENDINSERT);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_ENDINSERT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_EndInsert::~Job_EndInsert(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_ENDINSERT,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_EndInsert::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_ENDINSERT == eClass);
  RASSERT(pObject->eClass() == CN_JOB_ENDINSERT);
  
  Job_EndInsert* pJob_EndInsert = _pJob_EndInsert(pObject);

  pJob_EndInsert->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_EndInsert::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_ENDINSERT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_EndInsert::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_ENDINSERT);
  
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
Job* JobsModel::pJob_EndInsert_pre(Interface::SEitf* pSEitf)
{
  Job_EndInsert* pJob_EndInsert = new Job_EndInsert(pSEitf);
  
//  TokenVec* pTokenVec = pSEitf->m_pCommandProcessor->pTknVec();
//
//  bool bResult = pJob_EndInsert->bPopulateJob(pTokenVec);
//
//  if ( !bResult) {
//    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
//    pJob_EndInsert->m_bHasError = true;
//    pJob_EndInsert->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
//  }
  
  return pJob_EndInsert;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_EndInsert_post(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_EndInsert* pJob_EndInsert = _pJob_EndInsert(pJob);
  
  if ( pJob_EndInsert->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[pJob_EndInsert->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
}

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_EndInsert_new(Job* pArg)
{
  Job_EndInsert* pJob_EndInsert = new Job_EndInsert(nullptr);
  
  return pJob_EndInsert;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_EndInsert_delete(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_EndInsert* pJob_EndInsert = _pJob_EndInsert(pJob);
  
  vTrace("DELETING ENDINSERT\n"_Stz);
  delete pJob_EndInsert;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_EndInsert_show(Job* pJob)
{
  Job_EndInsert* pJob_EndInsert = _pJob_EndInsert(pJob);

  pJob_EndInsert->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
