/*============================================================================*/
/* Job_Connector.cpp -- Socket/Node Cross Connector Job                       */
/*============================================================================*/

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobNote mJobNote_Connector (
  "Job_Connector"_Stz,    // name of the job class
  CN_JOB_CONNECTOR,       // class enum
  JK_SYSTEM,              // job kind
  "socket connector"_Stz, // comment about the job
  nullptr,                // the prompt command
  nullptr,                // the second word
  "connector_seed"_Stz,   // the _run task seed
  nullptr,                // the _req task seed
  nullptr,                // the _rcv task seed 
  nullptr,                // the _exe task seed
  nullptr,                // the _end task seed
  nullptr,                // the _can task seed
  false,                  // check resources?
  false,                  // detached job?
  false,                  // buffer attached?
  JT_NONE,                // job trace
  nullptr,                // prompt pre-process function
  nullptr,                // prompt post-process function
  pJob_Connector_new,     // job new function
  vJob_Connector_delete,  // job delete function
  nullptr,                // job pre-send function
  nullptr,                // job post-receive function
  vJob_Connector_show,    // job show function
  false,                  // job is not prompt enabled
  "<help>"_Stz,           // help
  "<example>"_Stz         // example
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Connector* JobsModel::_pJob_Connector(void* aAddress)
{
  Job_Connector* pJob_Connector = reinterpret_cast<Job_Connector*>(aAddress);
  
  RASSERT( !pJob_Connector || CN_JOB_CONNECTOR == pJob_Connector->eClass());
  
  return pJob_Connector;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Connector::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_Connector* pJob_Connector =
    static_cast<Job_Connector*>(Heap::pAllocate(CN_JOB_CONNECTOR));
  
  return pJob_Connector;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Connector::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Connector::operator delete(void* aAddress)
{
  Job_Connector* pJob_Connector = static_cast<Job_Connector*>(aAddress);
  
  Heap::vFree(pJob_Connector);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Connector::Job_Connector(Interface::SEitf* pSEitf)
: Job(CN_JOB_CONNECTOR, pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CONNECTOR,_FOOTER_);
  
  //m_sSeed = mJobNote_Connector.pJobClass()->m_aSeedReq;
  
  _checkInitialised_(CN_JOB_CONNECTOR);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_CONNECTOR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Connector::~Job_Connector(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CONNECTOR,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Connector::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_CONNECTOR == eClass);
  RASSERT(pObject->eClass() == CN_JOB_CONNECTOR);
  
  Job_Connector* pJob_Connector = _pJob_Connector(pObject);

  pJob_Connector->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Connector::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBLOADER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Connector::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CONNECTOR);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_Connector_new(Job* pArg)
{
  Job_Connector* pJob_Connector = new Job_Connector(nullptr);
  
  return pJob_Connector;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_Connector_delete(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_Connector* pJob_Connector = _pJob_Connector(pJob);
  
  delete pJob_Connector;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_Connector_show(Job* pJob)
{
  Job_Connector* pJob_Connector = _pJob_Connector(pJob);

  pJob_Connector->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
