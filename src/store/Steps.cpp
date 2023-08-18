/*============================================================================*/
/* Steps.cpp -- Step Tracker                                                 */
/*============================================================================*/

#include "BASE.h"

#include "StepAccum.hpp"
#include "Steps.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__pCastFunction(STORE,Steps,CN_STEPS)
__aNewFunction(Steps,CN_STEPS)
__vDeleteFunction(Steps,CN_STEPS)
__aFooterFunction(Steps)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Steps::Steps(void)
: Object(CN_STEPS)
, m_okLock(this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STEPS,_FOOTER_);
  
  m_iNext = 0;
  //m_xStepAccum[]
  
  _checkInitialised_(CN_STEPS);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_STEPS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Steps::~Steps(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STEPS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* dump grid                                                                  */
/*----------------------------------------------------------------------------*/
void Steps::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  UNUSED(eClass);
  UNUSED(aTab);
  
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
void Steps::vShowAll(void)
{
  m_okLock.vGrabWrite();

  for (i8 i_Step = 0; i_Step < m_iNext; i_Step++)
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
/* write all steps to given file                                              */
/*----------------------------------------------------------------------------*/
void Steps::vWriteAll(void)
{
  File* pFile = new(C_VIA_MALLOC) File(nullptr);

  Now oNow;

  s256 sFilePath;

  sFilePath.vCatSprint("../tmp/Steps_%s.csv"_Stz,oNow.sz());
  vTrace("%s\n"_Stz, sFilePath.szStz());

  bool bOpened = pFile->bOpenWrite(sFilePath.szStz());
  if (! bOpened) {
    vTrace("Unable to open Steps file: %s\n"_Stz,pFile->sFilePath().szStz());
    return;
  }

  m_okLock.vGrabWrite();

  pFile->vPrint("Step Name, Invoked, Total Cycles, Cycles Per Invoke, "
                "ns Per Invoke, Max ns, Total Misses, Max Misses, Max Allocs, Max Frees, "
                "P_Tiny Alloc, P_Small Alloc, P_Medium Alloc, P_Large Alloc, "
                "T_Tiny Alloc, T_Small Alloc, T_Medium Alloc, T_Large Alloc, "
                "V_Tiny Alloc, V_Small Alloc, V_Medium Alloc, V_Large Alloc, "
                "P_Tiny Frees, P_Small Frees, P_Medium Frees, P_Large Frees, "
                "T_Tiny Frees, T_Small Frees, T_Medium Frees, T_Large Frees, "
                "V_Tiny Frees, V_Small Frees, V_Medium Frees, V_Large Frees \n"_Stz);
  for (i8 i_Step = 0; i_Step < m_iNext; i_Step++)
    m_xStepAccum[i_Step].vPrintAll(pFile);

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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
