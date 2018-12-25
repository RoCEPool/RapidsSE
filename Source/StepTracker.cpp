/*============================================================================*/
/* StepTracker.cpp -- Step Tracker                                            */
/*============================================================================*/

#include "Measurement.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

const Stz* C_STEPTRACKER_FILE = "/Rapids/StepTracker.csv"_Stz;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
StepTracker* Measurement::_pStepTracker(void* aAddr)
{
  StepTracker* pStepTracker = reinterpret_cast<StepTracker*>(aAddr);
  
  RASSERT_TYPE(pStepTracker,CN_STEPTRACKER);
  
  return pStepTracker;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* StepTracker::operator new(size_t)
{
  RASSERT(! gpAttachMap->m_pStepTracker);
  
  StepTracker* pStepTracker = (StepTracker*)HeapMem::pAllocate(CN_STEPTRACKER);
  
  return pStepTracker;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StepTracker::StepTracker(void)
: Object(CN_STEPTRACKER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STEPTRACKER,_FOOTER_);
  
  m_zLock.vInitLock(LK_STEP_ARRAY);
  m_iNext = 0;
  //m_xStepAccum[]
  
  _checkInitialised_(CN_STEPTRACKER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_STEPTRACKER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
StepTracker::~StepTracker(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STEPTRACKER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* dump grid                                                                  */
/*----------------------------------------------------------------------------*/
void StepTracker::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  StepTracker* pStepTracker = _pStepTracker(pObject);
  
  vTrace("\n"_Stz, aTab, pStepTracker);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allocate a step accumulator                                                */
/*----------------------------------------------------------------------------*/
StepAccum* StepTracker::pAllocAccum(Stz* aStepNm)
{
  ASSERT(m_zLock.bIsLocked());

  i8 iAccum = m_iNext++;
   
  ASSERT(iAccum < C_STEPS);
  
  m_xStepAccum[iAccum].m_aStepNm = aStepNm;
  
  return &m_xStepAccum[iAccum];
}

/*----------------------------------------------------------------------------*/
/* drop write lock                                                            */
/*----------------------------------------------------------------------------*/
void StepTracker::vDropWrite(void)
{
  m_zLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* grab write lock                                                            */
/*----------------------------------------------------------------------------*/
void StepTracker::vGrabWrite(void)
{
  m_zLock.vGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* reset steps                                                                */
/*----------------------------------------------------------------------------*/
void StepTracker::vReset(void)
{
  m_zLock.vGrabWrite();
  
  for (i8 i_Step = 0; i_Step < m_iNext; i_Step++)
    m_xStepAccum[i_Step].vReset();
  
  m_zLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* write steps to given file                                                  */
/*----------------------------------------------------------------------------*/
void StepTracker::vWrite(void)
{
  File xFile;

  bool bOpened = xFile.bOpen((Stz*)C_STEPTRACKER_FILE,C_OPEN_WRITE);
  if (! bOpened) {
    vTrace("Unable to open StepTracker file: %s\n"_Stz,xFile.sFilePath().aStr());
    return;
  }
  
  m_zLock.vGrabWrite();
  
  xFile.vPrint("Step, Invoked, Step Total, Per Invoke\n"_Stz);
  for (i8 i_Step = 0; i_Step < m_iNext; i_Step++)
    m_xStepAccum[i_Step].vPrint(xFile);
  
  m_zLock.vDropWrite();
  
  vTrace("Written: %s\n"_Stz,xFile.sFilePath().aStr());
  
  xFile.vClose();
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* StepTracker::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STEPTRACKER);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
