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
#include "RowBuffer.hpp"

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
  
  //  create a new buffer
  RowBuffer* pRowBuffer = new RowBuffer;
  
  // initialise the row slab inside the new row buffer
  pRowBuffer->vInitPix(pStreamDef,true);
  
  while (iNextKey && pRowBuffer->pAllotRow()) {
  
    i8 iPtnKeyHash = 0;
    
    pRowBuffer->vDepositColVal(iNextKey);
    
    
    
    
    HshLib::vRunningHash(&iPtnKeyHash,iNextKey);
    pRowBuffer->vDepositColVal(iPtnKeyHash);

    if (10001 == ++iNextKey)
      iNextKey = 0;
  }
  
  m_pDataStack->vPushInt(iNextKey);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
