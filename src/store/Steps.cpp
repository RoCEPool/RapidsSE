/*============================================================================*/
/* Steps.cpp -- Step Tracker                                                 */
/*============================================================================*/

#include "BASE.h"

#include "StepAccum.hpp"
#include "Steps.hpp"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Steps* STORE::_pSteps(void* aAddr)
{
  Steps* pSteps = reinterpret_cast<Steps*>(aAddr);
  
  RASSERT_TYPE(pSteps,CN_STEPTIMING);
  
  return pSteps;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Steps::operator new(size_t)
{
  RASSERT(! AM::pSteps());
  
  Steps* pSteps = (Steps*)Heaps::pAllocate(CN_STEPTIMING);
  
  return pSteps;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Steps::Steps(void)
: Object(CN_STEPTIMING)
, m_okLock(CN_STEPTIMING,LK_STEP_ARRAY,this)
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
Steps::~Steps(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STEPTIMING,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* dump grid                                                                  */
/*----------------------------------------------------------------------------*/
void Steps::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Steps* pSteps = _pSteps(pObject);
  
  vTrace("\n"_Stz, aTab, pSteps);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allocate a step accumulator                                                */
/*----------------------------------------------------------------------------*/
StepAccum* Steps::pAllocAccum(Stz* aStepNm)
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
void Steps::vDropWrite(void)
{
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* grab write lock                                                            */
/*----------------------------------------------------------------------------*/
void Steps::vGrabWrite(void)
{
  m_okLock.vGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* reset steps                                                                */
/*----------------------------------------------------------------------------*/
void Steps::vReset(void)
{
  m_okLock.vGrabWrite();
  
  for (i8 i_Step = 0; i_Step < m_iNext; i_Step++)
    m_xStepAccum[i_Step].vReset();
  
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* write steps to given file                                                  */
/*----------------------------------------------------------------------------*/
void Steps::vShow(Stz* szStepNm)
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
void Steps::vWrite(Stz* szPath)
{
  File* pFile = new File(nullptr);
  
  Now oNow;
  
  s256 sFilePath = szPath;
  
  //sFilePath.vCatSprint("/Steps_%s.csv"_Stz,oNow.sz());

  bool bOpened = pFile->bOpenWrite(sFilePath.szStz());
  if (! bOpened) {
    vTrace("Unable to open Steps file: %s\n"_Stz,pFile->sFilePath().szStz());
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
void Steps::vWriteSingleStep(Stz* szPath, Stz* szStepNm)
{
  File* pFile = new File(nullptr);
  
  Now oNow;
  
  s256 sFilePath = szPath;
  
  //sFilePath.vCatSprint("/Steps_%s.csv"_Stz,oNow.sz());

  bool bOpened = pFile->bOpenWrite(sFilePath.szStz());
  if (! bOpened) {
    vTrace("Unable to open Steps file: %s\n"_Stz,pFile->sFilePath().szStz());
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
Footer* Steps::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STEPTIMING);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
