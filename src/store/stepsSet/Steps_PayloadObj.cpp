/*============================================================================*/
/* Steps_PayloadObj.cpp - PayloadObj step group                               */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "PayloadObj.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_PayloadObj(void)
{

  vAddStep("payloadObj::delete"_Stz, &Step::vPayloadObjDelete);

}

/*----------------------------------------------------------------------------*/
/* payloadObj::delete : delete the payloadObj on the stack                    */
/*----------------------------------------------------------------------------*/
void Step::vPayloadObjDelete(void)
{
  ENTRY();

  WLP::PayloadObj* pPayloadObj = WLP::_pPayloadObj(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_PAYLOADOBJ == pPayloadObj->eGetClassNo());

  delete pPayloadObj;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
