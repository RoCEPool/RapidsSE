/*============================================================================*/
/* Steps_String.cpp - string[] step group                                     */
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
void Step::vAddSteps_StringA(void)
{
  vAddStep("stringA[]"_Stz,      &Step::vStringAGet);
  vAddStep("stringA[].get"_Stz,  &Step::vStringAGet);
  vAddStep("stringA[].set"_Stz,  &Step::vStringASet);

  vAddStep("string0[]"_Stz,      &Step::vString0Get);
  vAddStep("string0[].get"_Stz,  &Step::vString0Get);
  vAddStep("string0[].set"_Stz,  &Step::vString0Set);

  vAddStep("string1[]"_Stz,      &Step::vString1Get);
  vAddStep("string1[].get"_Stz,  &Step::vString1Get);
  vAddStep("string1[].set"_Stz,  &Step::vString1Set);

  vAddStep("string2[]"_Stz,      &Step::vString2Get);
  vAddStep("string2[].get"_Stz,  &Step::vString2Get);
  vAddStep("string2[].set"_Stz,  &Step::vString2Set);

  vAddStep("string3[]"_Stz,      &Step::vString3Get);
  vAddStep("string3[].get"_Stz,  &Step::vString3Get);
  vAddStep("string3[].set"_Stz,  &Step::vString3Set);

  vAddStep("string4[]"_Stz,      &Step::vString4Get);
  vAddStep("string4[].get"_Stz,  &Step::vString4Get);
  vAddStep("string4[].set"_Stz,  &Step::vString4Set);

  vAddStep("string5[]"_Stz,      &Step::vString5Get);
  vAddStep("string5[].get"_Stz,  &Step::vString5Get);
  vAddStep("string5[].set"_Stz,  &Step::vString5Set);

  vAddStep("string6[]"_Stz,      &Step::vString6Get);
  vAddStep("string6[].get"_Stz,  &Step::vString6Get);
  vAddStep("string6[].set"_Stz,  &Step::vString6Set);

  vAddStep("string7[]"_Stz,      &Step::vString7Get);
  vAddStep("string7[].get"_Stz,  &Step::vString7Get);
  vAddStep("string7[].set"_Stz,  &Step::vString7Set);
}

/*----------------------------------------------------------------------------*/
/* stringA[].get -- get stringA[]                                             */
/*----------------------------------------------------------------------------*/
void Step::vStringAGet(void)
{
  ENTRY();
    
  m_pDataStack->vPushStr(m_pLogicUnit->m_sStringA);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* stringA[].set -- set stringA[]                                             */
/*----------------------------------------------------------------------------*/
void Step::vStringASet(void)
{
  ENTRY();
    
  m_pDataStack->vPopStr(m_pLogicUnit->m_sStringA, __func__);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* String0[].get -- get String0[]                                             */
/*----------------------------------------------------------------------------*/
void Step::vString0Get(void)
{
  ENTRY();
    
  m_pDataStack->vPushStr(m_pLogicUnit->m_sString0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* String0[].set -- set String0[]                                             */
/*----------------------------------------------------------------------------*/
void Step::vString0Set(void)
{
  ENTRY();
    
  m_pDataStack->vPopStr(m_pLogicUnit->m_sString0, __func__);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* String1[].get -- get String1[]                                             */
/*----------------------------------------------------------------------------*/
void Step::vString1Get(void)
{
  ENTRY();
    
  m_pDataStack->vPushStr(m_pLogicUnit->m_sString1);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* String1[].set -- set String1[]                                             */
/*----------------------------------------------------------------------------*/
void Step::vString1Set(void)
{
  ENTRY();
    
  m_pDataStack->vPopStr(m_pLogicUnit->m_sString1, __func__);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* String2[].get -- get String2[]                                             */
/*----------------------------------------------------------------------------*/
void Step::vString2Get(void)
{
  ENTRY();
    
  m_pDataStack->vPushStr(m_pLogicUnit->m_sString2);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* String2[].set -- set String2[]                                             */
/*----------------------------------------------------------------------------*/
void Step::vString2Set(void)
{
  ENTRY();
    
  m_pDataStack->vPopStr(m_pLogicUnit->m_sString2, __func__);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* String3[].get -- get String3[]                                             */
/*----------------------------------------------------------------------------*/
void Step::vString3Get(void)
{
  ENTRY();
    
  m_pDataStack->vPushStr(m_pLogicUnit->m_sString3);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* String3[].set -- set String3[]                                             */
/*----------------------------------------------------------------------------*/
void Step::vString3Set(void)
{
  ENTRY();
    
  m_pDataStack->vPopStr(m_pLogicUnit->m_sString3, __func__);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* String4[].get -- get String4[]                                             */
/*----------------------------------------------------------------------------*/
void Step::vString4Get(void)
{
  ENTRY();
    
  m_pDataStack->vPushStr(m_pLogicUnit->m_sString4);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* String4[].set -- set String4[]                                             */
/*----------------------------------------------------------------------------*/
void Step::vString4Set(void)
{
  ENTRY();
    
  m_pDataStack->vPopStr(m_pLogicUnit->m_sString4, __func__);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* String5[].get -- get String5[]                                             */
/*----------------------------------------------------------------------------*/
void Step::vString5Get(void)
{
  ENTRY();
    
  m_pDataStack->vPushStr(m_pLogicUnit->m_sString5);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* String5[].set -- set String5[]                                             */
/*----------------------------------------------------------------------------*/
void Step::vString5Set(void)
{
  ENTRY();
    
  m_pDataStack->vPopStr(m_pLogicUnit->m_sString5, __func__);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* String6[].get -- get String6[]                                             */
/*----------------------------------------------------------------------------*/
void Step::vString6Get(void)
{
  ENTRY();
    
  m_pDataStack->vPushStr(m_pLogicUnit->m_sString6);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* String6[].set -- set String6[]                                             */
/*----------------------------------------------------------------------------*/
void Step::vString6Set(void)
{
  ENTRY();
    
  m_pDataStack->vPopStr(m_pLogicUnit->m_sString6, __func__);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* String7[].get -- get String7[]                                             */
/*----------------------------------------------------------------------------*/
void Step::vString7Get(void)
{
  ENTRY();
    
  m_pDataStack->vPushStr(m_pLogicUnit->m_sString7);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* String7[].set -- set String7[]                                             */
/*----------------------------------------------------------------------------*/
void Step::vString7Set(void)
{
  ENTRY();
    
  m_pDataStack->vPopStr(m_pLogicUnit->m_sString7, __func__);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
