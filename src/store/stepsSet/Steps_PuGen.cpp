/*============================================================================*/
/* Steps_PuGen.cpp - Pu Gen group                                             */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"


#include "StreamDef.hpp"
#include "ToRow.hpp"

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
  
  ToRow* pToRow = _pToRow(m_pDataStack->pPopPtr(__func__));

  StreamDef* pStreamDef = _pStreamDef(m_pDataStack->pPopPtr(__func__));

  Stz* szStreamDefNm = pStreamDef->szStreamDefNm();

  i8 iNextKey = m_pDataStack->iPopInt(__func__);

  vTrace("vPuGenX: %p ; %s ; %lld\n"_Stz
         ,pToRow,szStreamDefNm,iNextKey);
  
  if (10 == ++iNextKey)
    iNextKey = 0;
  
  m_pDataStack->vPushInt(iNextKey);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
