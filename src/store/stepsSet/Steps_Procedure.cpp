/*============================================================================*/
/* Steps_Procedure.cpp - procedure step group                                 */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "Procedure.hpp"
#include "TagConstants.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Procedure(void)
{
  vAddStep("procedure::add"_Stz,  &Step::vProcedureAdd);
}

/*----------------------------------------------------------------------------*/
/* process::getPid -- get pid of this process                                  */
/*----------------------------------------------------------------------------*/
//void Step::vProcedureAdd(void)
//{
//  ENTRY();
//
//  s48K sCmd;
//  m_pDataStack->vPopStr(sCmd,__func__);
//
//  s48K sCmdTagStr;
//  m_pDataStack->vPopStr(sCmdTagStr,__func__);
//
//  TokenVec* pTokenVec = TokenVec::pTokeniseSen(sCmd);
//
//  Procedure::vAddProcedure(pTokenVec);
//
//  delete pTokenVec;
//
//  EXIT();
//}
/*----------------------------------------------------------------------------*/
void Step::vProcedureAdd(void)
{
  ENTRY();
  
  s48K sProc;
  m_pDataStack->vPopStr(sProc,__func__);
  
  s48K sWithTagStr;
  m_pDataStack->vPopStr(sWithTagStr,__func__);
  
  if (AM::TRACE::bVerbose(C_VB_BOOT)) {
    vTrace("AddProcedure: %s\n"_Stz,sProc.szStz());
    vTrace("WithTags: %s\n"_Stz,sWithTagStr.szStz());
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  TokenVec* pDmnTknVec = TokenVec::pTokeniseSen(sProc);
  
  pDmnTknVec->vIntegrityCheck();
  
  Token* pTkn0 = pDmnTknVec->pGet(0);
  ASSERT(pTkn0->bSame("procedure"_Stz) || pTkn0->bSame("procedureLocal"_Stz));
  UNUSED(pTkn0);

  //Stz* ssProcNm = pDmnTknVec->pGet(1)->szChars();
  
  Procedure::vAddProcedure(pDmnTknVec);
  
  delete pDmnTknVec;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
