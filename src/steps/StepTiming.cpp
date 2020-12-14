/*============================================================================*/
/* StepTiming.cpp -- Step Tracker                                            */
/*============================================================================*/

#include "Steps.h"

#include "File.hpp" //?

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
StepTiming* Steps::_pStepTiming(void* aAddr)
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
  RASSERT(! AM::pStepTiming());
  
  StepTiming* pStepTiming = (StepTiming*)HeapMem::pAllocate(CN_STEPTIMING);
  
  return pStepTiming;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StepTiming::StepTiming(void)
: Object(CN_STEPTIMING)
, m_okLock(LK_STEP_ARRAY,this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STEPTIMING,_FOOTER_);
  
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
  ASSERT(m_okLock.bIsLocked());

  i8 iAccum = m_iNext++;
   
  ASSERT(iAccum < C_STEPS);
  
  m_xStepAccum[iAccum].m_szStepNm = aStepNm;
  
  return &m_xStepAccum[iAccum];
}

/*----------------------------------------------------------------------------*/
/* drop write lock                                                            */
/*----------------------------------------------------------------------------*/
void StepTiming::vDropWrite(void)
{
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* grab write lock                                                            */
/*----------------------------------------------------------------------------*/
void StepTiming::vGrabWrite(void)
{
  m_okLock.vGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* reset steps                                                                */
/*----------------------------------------------------------------------------*/
void StepTiming::vReset(void)
{
  m_okLock.vGrabWrite();
  
  for (i8 i_Step = 0; i_Step < m_iNext; i_Step++)
    m_xStepAccum[i_Step].vReset();
  
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* write steps to given file                                                  */
/*----------------------------------------------------------------------------*/
void StepTiming::vShow(Stz* szStepNm)
{
  m_okLock.vGrabWrite();
  
  for (i8 i_Step = 0; i_Step < m_iNext; i_Step++)
    if (m_xStepAccum[i_Step].m_szStepNm->bSame(szStepNm))
      m_xStepAccum[i_Step].vShow();
  
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* write steps to given file                                                  */
/*----------------------------------------------------------------------------*/
void StepTiming::vWrite(Stz* szPath)
{
  File* pFile = new File(nullptr);
  
  Now oNow;
  
  s256 sFilePath = szPath;
  
  //sFilePath.vCatSprint("/StepTiming_%s.csv"_Stz,oNow.sz());

  bool bOpened = pFile->bOpenWrite(sFilePath.szStz());
  if (! bOpened) {
    vTrace("Unable to open StepTiming file: %s\n"_Stz,pFile->sFilePath().szStz());
    return;
  }
  
  m_okLock.vGrabWrite();
  
  pFile->vPrint("Step, Invoked, Step Total, Per Invoke\n"_Stz);
  for (i8 i_Step = 0; i_Step < m_iNext; i_Step++)
    m_xStepAccum[i_Step].vPrint(pFile);
  
  m_okLock.vDropWrite();
  
  vTrace("Written: %s\n"_Stz,pFile->sFilePath().szStz());
  
  pFile->vClose();
  
  delete pFile;
}

/*----------------------------------------------------------------------------*/
/* write steps to given file                                                  */
/*----------------------------------------------------------------------------*/
void StepTiming::vWriteSingleStep(Stz* szPath, Stz* szStepNm)
{
  File* pFile = new File(nullptr);
  
  Now oNow;
  
  s256 sFilePath = szPath;
  
  //sFilePath.vCatSprint("/StepTiming_%s.csv"_Stz,oNow.sz());

  bool bOpened = pFile->bOpenWrite(sFilePath.szStz());
  if (! bOpened) {
    vTrace("Unable to open StepTiming file: %s\n"_Stz,pFile->sFilePath().szStz());
    return;
  }
  
  m_okLock.vGrabWrite();
  
  pFile->vPrint("Step, Invoked, Step Total, Per Invoke\n"_Stz);
  for (i8 i_Step = 0; i_Step < m_iNext; i_Step++)
    if (m_xStepAccum[i_Step].m_szStepNm->bSame(szStepNm))
      m_xStepAccum[i_Step].vPrint(pFile);
  
  m_okLock.vDropWrite();
  
  vTrace("Written: %s\n"_Stz,pFile->sFilePath().szStz());
  
  pFile->vClose();
  
  delete pFile;
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
