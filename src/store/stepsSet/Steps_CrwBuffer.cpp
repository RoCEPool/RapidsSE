/*============================================================================*/
/* Steps_CrwBuffer.cpp - CrwBuffer steps group                                */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "RowCoder.hpp"
#include "HeadLine.hpp"
#include "Crw.hpp"
#include "CrwPacker.hpp"
#include "CrwBuffer.hpp"
#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"
#include "CrvBuffer.hpp"
#include "TablePtn.hpp"
#include "Table.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_CrwBuffer(void)
{
  vAddStep("crwBuffer::new"_Stz,          &Step::vCrwBufferNew);
  
  vAddStep("crwBuffer.delete"_Stz,        &Step::vCrwBufferDelete);
  vAddStep("crwBuffer.operandDump"_Stz,   &Step::vCrwBufferOperandDump);
}

/*----------------------------------------------------------------------------*/
/* crwBuffer.delete -- delete a crw buffer                                    */
/*----------------------------------------------------------------------------*/
void Step::vCrwBufferDelete(void)
{
  ENTRY();

  CrwBuffer* pCrwBuffer = _pCrwBuffer(m_pDataStack->pPopPtr(__func__));

  delete pCrwBuffer;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* crwBuffer::new -- new a crw buffer                                         */
/*----------------------------------------------------------------------------*/
void Step::vCrwBufferNew(void)
{
  ENTRY();

  CrwBuffer* pCrwBuffer = new CrwBuffer;

  m_pDataStack->vPushPtr(pCrwBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* crwBuffer.operandDump -- dump information about crw buffer                 */
/*----------------------------------------------------------------------------*/
void Step::vCrwBufferOperandDump(void)
{
  ENTRY();

  CrwBuffer* pCrwBuffer = _pCrwBuffer(m_pDataStack->pPopPtr(__func__));

  i8 iDetail = m_pDataStack->iPopInt(__func__);
  
  pCrwBuffer->vOperandDump(iDetail);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
