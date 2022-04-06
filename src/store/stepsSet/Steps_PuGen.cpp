/*============================================================================*/
/* Steps_PuGen.cpp - Pu Gen group                                             */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"


#include "StreamDef.hpp"
#include "ToKeyHshRow.hpp"
#include "RowBuffer.hpp"

#include "PuGen.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_PuGen(void)
{
  vAddStep("PuGen::x"_Stz,  &Step::vPuGenX);
}


/*----------------------------------------------------------------------------*/
/* PuGen::x -- x                                                              */
/*----------------------------------------------------------------------------*/
void Step::vPuGenX(void)
{
  ENTRY();
  
  ToKeyHshRow* pToRow = _pToKeyHshRow(m_pDataStack->pPopPtr(__func__));

  StreamDef* pStreamDef = _pStreamDef(m_pDataStack->pPopPtr(__func__));

  i8 iNextKey = m_pDataStack->iPopInt(__func__);

  PuGen::vGen(iNextKey,pToRow,pStreamDef);
  
  m_pDataStack->vPushInt(iNextKey);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
