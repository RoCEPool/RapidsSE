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
  ASSERT(! gpStepArray);
  
  StepArray* gpStepArray = (StepArray*)gpBufferPool->pAllocBuffer();
  
  return gpStepArray;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StepArray::StepArray(void)
: Buffer(CN_STEPARRAY)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STEPARRAY,_FOOTER_);
  
  //m_zLock
  m_iNext = 0;
  //m_xStepAccum[]
  
  Memory::vCheckInitialised(this,sizeof(StepArray));
  
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