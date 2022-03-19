/*============================================================================*/
/* Steps_LogicCompiler.cpp -- LogicCompiler step group                        */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "Node_I.hpp"
#include "Job.hpp"
#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_LogicCompiler(void)
{
  vAddStep("LogicCompiler[].compile"_Stz,   &Step::vLogicCompilerCompile);
  vAddStep("LogicCompiler[].delete"_Stz,    &Step::vLogicCompilerDelete);
  vAddStep("LogicCompiler[].extract"_Stz,   &Step::vLogicCompilerExtract);
  vAddStep("LogicCompiler[].extractAsm"_Stz, &Step::vLogicCompilerExtractAsm);
  vAddStep("LogicCompiler[].isNull"_Stz,    &Step::vLogicCompilerIsNull);
  vAddStep("LogicCompiler[].new"_Stz,       &Step::vLogicCompilerNew);
}

/*----------------------------------------------------------------------------*/
/* LogicCompiler[].compile -- compile                                         */
/*----------------------------------------------------------------------------*/
void Step::vLogicCompilerCompile(void)
{
  ENTRY();
      
  i8 iResult =
    m_pLogicUnit->m_xpLogicCompiler->iCompile();
  
  UNUSED(iResult);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicCompiler[].delete -- delete logic compiler                            */
/*----------------------------------------------------------------------------*/
void Step::vLogicCompilerDelete(void)
{
  ENTRY();
  
  LogicCompiler* pLogicCompiler = * m_pLogicUnit->m_xpLogicCompiler;
  
  m_pLogicUnit->m_xpLogicCompiler = nullptr;
  
  delete pLogicCompiler;
      
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicCompiler[].extract -- extract the EO_ fragments                       */
/*----------------------------------------------------------------------------*/
void Step::vLogicCompilerExtract(void)
{
  ENTRY();
    
//#ifdef WITH_APPLE
  LogicCompiler::vExtract();
//#endif
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicCompiler[].extractAsm -- extract the AM_ fragments                    */
/*----------------------------------------------------------------------------*/
void Step::vLogicCompilerExtractAsm(void)
{
  ENTRY();

//#ifdef WITH_APPLE
  LogicCompiler::vExtractAsm();
//#endif

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicCompiler[].isNull -- is logicCompiler null ?                          */
/*----------------------------------------------------------------------------*/
void Step::vLogicCompilerIsNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt( (m_pLogicUnit )->m_xpLogicCompiler.bIsNull() ? 1 : 0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicCompiler[].new -- create a new logic compiler                         */
/*----------------------------------------------------------------------------*/
void Step::vLogicCompilerNew(void)
{
  ENTRY();
  
  m_pLogicUnit->m_xpLogicCompiler =
    new LogicCompiler(m_pLogicUnit);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
