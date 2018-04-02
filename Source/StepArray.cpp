/*============================================================================*/
/* StepAccum.cpp -- Step Accumulator                                          */
/*============================================================================*/

#include "StorageEngine.h"
#include "Tasks.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* StepArray::operator new(size_t iSize)
{
  RASSERT(iSize);
  RASSERT(! gpAttachMap->m_pStepArray);
  
  gpAttachMap->m_pStepArray = (StepArray*)Heap::pAllocate(CN_STEPARRAY);
  
  return gpAttachMap->m_pStepArray;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StepArray::StepArray(void)
: Object(CN_STEPARRAY)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STEPARRAY,_FOOTER_);
  
  m_zLock.vInitLock(LK_STEP_ARRAY);
  m_iNext = 0;
  //m_xStepAccum[]
  
  _checkInitialised_(CN_STEPARRAY);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_STEPARRAY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
StepArray::~StepArray(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STEPARRAY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* allocate a step accumulator                                                */
/*----------------------------------------------------------------------------*/
StepAccum* StepArray::pAllocAccum(Stz* aStepNm)
{
  ASSERT(m_zLock.bIsLocked());

  i8 iAccum = m_iNext++;
   
  ASSERT(iAccum < C_STEPS);
  
  m_xStepAccum[iAccum].m_aStepNm = aStepNm;
  
  return &m_xStepAccum[iAccum];
}

/*----------------------------------------------------------------------------*/
/* dump grid                                                                  */
/*----------------------------------------------------------------------------*/
void StepArray::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(eClass == CN_STEPARRAY);
  RASSERT(pObject->eClass() == CN_STEPARRAY);
  
  StepArray* pStepArray = reinterpret_cast<StepArray*>(pObject);
  
  vTrace("\n"_Stz, aTab, pStepArray);
}

/*----------------------------------------------------------------------------*/
/* list steps                                                                 */
/*----------------------------------------------------------------------------*/
void StepArray::vList(File& xFile)
{
  m_zLock.vGrabWrite();
  
  xFile.vPrint("Step, Invoked, StepTotal, Per Invoke\n"_Stz);
  for (i8 i_Step = 0; i_Step < m_iNext; i_Step++)
    m_xStepAccum[i_Step].vPrint(xFile);
  
  m_zLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* reset steps                                                                */
/*----------------------------------------------------------------------------*/
void StepArray::vReset(void)
{
  m_zLock.vGrabWrite();
  
  for (i8 i_Step = 0; i_Step < m_iNext; i_Step++)
    m_xStepAccum[i_Step].vReset();
  
  m_zLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* grab write lock                                                            */
/*----------------------------------------------------------------------------*/
void StepArray::vGrabWrite(void)
{
  m_zLock.vGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* drop write lock                                                            */
/*----------------------------------------------------------------------------*/
void StepArray::vDropWrite(void)
{
  m_zLock.vDropWrite();
}
/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* StepArray::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STEPARRAY);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
