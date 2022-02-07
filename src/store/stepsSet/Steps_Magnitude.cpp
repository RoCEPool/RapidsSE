/*============================================================================*/
/* Steps_Magnitude.cpp - magnitude step group                                 */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Magnitude(void)
{
  vAddStep("toDigitName"_Stz,     &Step::vToDigitName);
  vAddStep("bytes.magnitude"_Stz, &Step::vBytesMagnitude);
  vAddStep("number.magnitude"_Stz, &Step::vNumberMagnitude);
}

/*----------------------------------------------------------------------------*/
/* bytes.magnitude -- return magnitude of bytes                               */
/*----------------------------------------------------------------------------*/
void Step::vBytesMagnitude(void)
{
  ENTRY();
  
  i8 iBytes = m_pDataStack->iPopInt(__func__);
  
  s256 sMag;
  MagLib::vByteMag(iBytes, sMag);
  
  m_pDataStack->vPushStr(sMag);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* number.magnitude -- get magnitude of number                                */
/*----------------------------------------------------------------------------*/
void Step::vNumberMagnitude(void)
{
  ENTRY();
  
  i8 iNumber = m_pDataStack->iPopInt(__func__);
  
  i8 iScale = m_pDataStack->iPopInt(__func__);
  
  s256 s;
  
  MagLib::vByteMag(iNumber, iScale, s);
  
  m_pDataStack->vPushStr(s);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
