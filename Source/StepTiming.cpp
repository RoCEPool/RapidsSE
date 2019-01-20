/*============================================================================*/
/* StepTiming.cpp -- Step Tracker                                            */
/*============================================================================*/

#include "Measurement.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

const Stz* C_StepTiming_FILE = "../StepTimings/StepTiming %s.csv"_Stz;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
StepTiming* Measurement::_pStepTiming(void* aAddr)
{
  StepTiming* pStepTiming = reinterpret_cast<StepTiming*>(aAddr);
  
  RASSERT_TYPE(pStepTiming,CN_STEPTIMING);
  
  return pStepTiming;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* StepTiming::operator new(size_t)
{
  RASSERT(! gpAttachMap->m_pStepTiming);
  
  StepTiming* pStepTiming = (StepTiming*)HeapMem::pAllocate(CN_STEPTIMING);
  
  return pStepTiming;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StepTiming::StepTiming(void)
: Object(CN_STEPTIMING)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STEPTIMING,_FOOTER_);
  
  m_oLock.vInitLock(LK_STEP_ARRAY);
  m_iNext = 0;
  //m_xStepAccum[]
  
  _checkInitialised_(CN_STEPTIMING);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_STEPTIMING,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
StepTiming::~StepTiming(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STEPTIMING,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* dump grid                                                                  */
/*----------------------------------------------------------------------------*/
void StepTiming::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  StepTiming* pStepTiming = _pStepTiming(pObject);
  
  vTrace("\n"_Stz, aTab, pStepTiming);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allocate a step accumulator                                                */
/*----------------------------------------------------------------------------*/
StepAccum* StepTiming::pAllocAccum(Stz* aStepNm)
{
  ASSERT(m_oLock.bIsLocked());

  i8 iAccum = m_iNext++;
   
  ASSERT(iAccum < C_STEPS);
  
  m_xStepAccum[iAccum].m_aStepNm = aStepNm;
  
  return &m_xStepAccum[iAccum];
}

/*----------------------------------------------------------------------------*/
/* drop write lock                                                            */
/*----------------------------------------------------------------------------*/
void StepTiming::vDropWrite(void)
{
  m_oLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* grab write lock                                                            */
/*----------------------------------------------------------------------------*/
void StepTiming::vGrabWrite(void)
{
  m_oLock.vGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* reset steps                                                                */
/*----------------------------------------------------------------------------*/
void StepTiming::vReset(void)
{
  m_oLock.vGrabWrite();
  
  for (i8 i_Step = 0; i_Step < m_iNext; i_Step++)
    m_xStepAccum[i_Step].vReset();
  
  m_oLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* write steps to given file                                                  */
/*----------------------------------------------------------------------------*/
void StepTiming::vWrite(void)
{
  File oFile;
  
  Now oNow;
  
  s256 sFilePath;
  
  sFilePath.vSprint((Stz*)C_StepTiming_FILE,oNow.sFormatted()->aChar());

  bool bOpened = oFile.bOpenWrite(sFilePath.aStz());
  if (! bOpened) {
    vTrace("Unable to open StepTiming file: %s\n"_Stz,oFile.sFilePath().aStz());
    return;
  }
  
  m_oLock.vGrabWrite();
  
  oFile.vPrint("Step, Invoked, Step Total, Per Invoke\n"_Stz);
  for (i8 i_Step = 0; i_Step < m_iNext; i_Step++)
    m_xStepAccum[i_Step].vPrint(oFile);
  
  m_oLock.vDropWrite();
  
  vTrace("Written: %s\n"_Stz,oFile.sFilePath().aStz());
  
  oFile.vClose();
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* StepTiming::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STEPTIMING);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
