/*============================================================================*/
/* StepAccum.cpp -- Step Accumulator                                          */
/*============================================================================*/

#include "Tasks.h"

extern BufferPool* gpBufferPool;

extern StepArray* gpStepArray ;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* StepArray::operator new(size_t iSize)
{
  RASSERT(iSize);
  RASSERT(! gpStepArray);
  
  gpStepArray = (StepArray*)Heap::pAllocate(CN_STEPARRAY);
  
  return gpStepArray;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StepArray::StepArray(void)
: Buffer(CN_STEPARRAY)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STEPARRAY,_FOOTER_);
  
  m_zLock.vInit("StepArray::m_zLock");
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
StepAccum* StepArray::pAllocAccum(const char* pFnName)
{
  m_zLock.vGrabWrite();

  i8 iAccum = m_iNext++;
  
  m_zLock.vDropWrite();
 
  ASSERT(iAccum < C_STEPS);
  
  m_xStepAccum[iAccum].m_aFnName = pFnName;
  
  return &m_xStepAccum[iAccum];
}

/*----------------------------------------------------------------------------*/
/* dump grid                                                                  */
/*----------------------------------------------------------------------------*/
void StepArray::vDump(eCN eClass, Object* pObject, const char* aTab)
{
  ASSERT(eClass == CN_STEPARRAY);
  ASSERT(pObject->eClass() == CN_STEPARRAY);
  
  StepArray* pStepArray = reinterpret_cast<StepArray*>(pObject);
  
  vTrace("\n"_Str, aTab, pStepArray);
}

/*----------------------------------------------------------------------------*/
/* list steps                                                                 */
/*----------------------------------------------------------------------------*/
void StepArray::vList(void)
{
  m_zLock.vGrabWrite();
  
  for (i8 iStep = 0; iStep < m_iNext; iStep++)
    m_xStepAccum[iStep].vPrint();
  
  m_zLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* reset steps                                                                */
/*----------------------------------------------------------------------------*/
void StepArray::vReset(void)
{
  m_zLock.vGrabWrite();
  
  for (i8 iStep = 0; iStep < m_iNext; iStep++)
    m_xStepAccum[iStep].vReset();
  
  m_zLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* StepArray::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STEPARRAY);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
