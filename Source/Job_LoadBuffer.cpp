/*============================================================================*/
/* Job_LoadBuffer.cpp -- Load Buffer Job                                      */
/*============================================================================*/

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobNote mJobNote_LoadBuffer (
  "Job_LoadBuffer"_Stz,    // name of the job class
  CN_JOB_LOADBUFFER,       // class enum
  JK_ONESHOT,              // job kind
  "table load buffer"_Stz, // comment about the job
  nullptr,                 // the prompt command
  nullptr,                 // the second word
  "LoadBuffer_seed"_Stz,   // the task seed for this job
  false,                   // check resources?
  false,                   // detached job?
  nullptr,                 // prompt pre-process function
  nullptr,                 // prompt post-process function
  false,                   // job is not prompt enabled
  "<help>"_Stz,            // help
  "<example>"_Stz          // example
);

/* example:
 
 ld lineitem "/tpch/lineitem.tbl" ;
 
 */

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_LoadBuffer* JobsModel::_pJob_LoadBuffer(void* aAddress)
{
  Job_LoadBuffer* pJob_LoadBuffer = reinterpret_cast<Job_LoadBuffer*>(aAddress);
  
  //RASSERT( !pJob_LoadBuffer || CN_JOB_LOADBUFFER == pJob_LoadBuffer->eClass());
  
  return pJob_LoadBuffer;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_LoadBuffer::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_LoadBuffer* pJob_LoadBuffer =
    static_cast<Job_LoadBuffer*>(Heap::pAllocate(CN_JOB_LOADBUFFER));
  
  return pJob_LoadBuffer;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_LoadBuffer::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_LoadBuffer::operator delete(void* aAddress)
{
  Job_LoadBuffer* pJob_LoadBuffer = static_cast<Job_LoadBuffer*>(aAddress);
  
  Heap::vFree(pJob_LoadBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_LoadBuffer::Job_LoadBuffer(Interface::SEitf* pSEitf)
: Job(CN_JOB_LOADBUFFER), JobDicPath(pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOADBUFFER,_FOOTER_);
  
  m_sSeed = mJobNote_LoadBuffer.pJobClass()->m_aSeed;
  
  _checkInitialised_(CN_JOB_LOADBUFFER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOADBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_LoadBuffer::~Job_LoadBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOADBUFFER,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_LoadBuffer::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_LOADBUFFER == eClass);
  RASSERT(pObject->eClass() == CN_JOB_LOADBUFFER);
  
  Job_LoadBuffer* pJob_LoadBuffer = _pJob_LoadBuffer(pObject);

  pJob_LoadBuffer->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_LoadBuffer::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOADBUFFER,_FOOTER_);
  
                                                               //! Alka to write
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_LoadBuffer::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOADBUFFER);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
