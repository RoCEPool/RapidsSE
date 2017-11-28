/*============================================================================*/
/* Steps_Reg.cpp - reg step group                                             */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;

// registers: //////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Reg(void)
{
  vAddPrimitive("reg.pop"_Stz,          &vRegPop,          false);
  vAddPrimitive("reg.push"_Stz,         &vRegPush,         false);
  vAddPrimitive("reg0"_Stz,             &vRegReg0,         false);
  vAddPrimitive("reg1"_Stz,             &vRegReg1,         false);
  vAddPrimitive("reg2"_Stz,             &vRegReg2,         false);
  vAddPrimitive("reg3"_Stz,             &vRegReg3,         false);
  vAddPrimitive("reg4"_Stz,             &vRegReg4,         false);
  vAddPrimitive("reg5"_Stz,             &vRegReg5,         false);
  vAddPrimitive("reg6"_Stz,             &vRegReg6,         false);
  vAddPrimitive("reg7"_Stz,             &vRegReg7,         false);
  vAddPrimitive("reg8"_Stz,             &vRegReg8,         false);
  vAddPrimitive("reg9"_Stz,             &vRegReg9,         false);
  vAddPrimitive("regA"_Stz,             &vRegRegA,         false);
  vAddPrimitive("regB"_Stz,             &vRegRegB,         false);
  vAddPrimitive("regC"_Stz,             &vRegRegC,         false);
  vAddPrimitive("regD"_Stz,             &vRegRegD,         false);
  vAddPrimitive("regE"_Stz,             &vRegRegE,         false);
  vAddPrimitive("regF"_Stz,             &vRegRegF,         false);
  vAddPrimitive("regN"_Stz,             &vRegRegNth,       false);
  vAddPrimitive(">reg0"_Stz,            &vRegToReg0,       false);
  vAddPrimitive(">reg1"_Stz,            &vRegToReg1,       false);
  vAddPrimitive(">reg2"_Stz,            &vRegToReg2,       false);
  vAddPrimitive(">reg3"_Stz,            &vRegToReg3,       false);
  vAddPrimitive(">reg4"_Stz,            &vRegToReg4,       false);
  vAddPrimitive(">reg5"_Stz,            &vRegToReg5,       false);
  vAddPrimitive(">reg6"_Stz,            &vRegToReg6,       false);
  vAddPrimitive(">reg7"_Stz,            &vRegToReg7,       false);
  vAddPrimitive(">reg8"_Stz,            &vRegToReg8,       false);
  vAddPrimitive(">reg9"_Stz,            &vRegToReg9,       false);
  vAddPrimitive(">regA"_Stz,            &vRegToRegA,       false);
  vAddPrimitive(">regB"_Stz,            &vRegToRegB,       false);
  vAddPrimitive(">regC"_Stz,            &vRegToRegC,       false);
  vAddPrimitive(">regD"_Stz,            &vRegToRegD,       false);
  vAddPrimitive(">regE"_Stz,            &vRegToRegE,       false);
  vAddPrimitive(">regF"_Stz,            &vRegToRegF,       false);
  vAddPrimitive(">regN"_Stz,            &vRegToRegNth,     false);
}

/*----------------------------------------------------------------------------*/
/* reg.pop -- pop register set                                                */
/*----------------------------------------------------------------------------*/
void Task::vRegPop(MP_Task mpTask)
{
  ENTRY("vRegPop"_Stz);
  
  RegFrame* pRegFrame = mpTask->m_pRegFrame;
  
  if ( ! pRegFrame )
    THROW_INVALID_DSTACK("reg.pop"_Stz);
  
  mpTask->m_pRegFrame = mpTask->m_pRegFrame->pNext();
  
  //vTrace("Deleting frame %s\n"_Stz,pRegFrame->m_sName.s());
  
  delete pRegFrame;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* reg.push -- push a register set                                            */
/*----------------------------------------------------------------------------*/
void Task::vRegPush(MP_Task mpTask)
{
ENTRY("vRegPush"_Stz);
  
  RegFrame* pRegFrame = new RegFrame;
  
  pRegFrame->vNext(mpTask->m_pRegFrame);
  
  mpTask->m_pRegFrame = pRegFrame;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* reg0 -- push reg0 onto dstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegReg0(MP_Task mpTask)
{
ENTRY("vRegReg0"_Stz);
  
  if ( ! mpTask->m_pRegFrame )
    THROW_INVALID_DSTACK("reg0"_Stz);
  
  ValReg* pValReg = mpTask->m_pRegFrame->pValReg(0);
  
  mpTask->m_pDataStack->vPushValue(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* reg1 -- push reg1 onto dstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegReg1(MP_Task mpTask)
{
  ENTRY("vRegReg1"_Stz);
  
  if ( ! mpTask->m_pRegFrame )
    THROW_INVALID_DSTACK("reg1"_Stz);
  
  ValReg* pValReg = mpTask->m_pRegFrame->pValReg(1);
  
  mpTask->m_pDataStack->vPushValue(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* reg2 -- push reg2 onto dstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegReg2(MP_Task mpTask)
{
  ENTRY("vRegReg2"_Stz);
  
  if ( ! mpTask->m_pRegFrame )
    THROW_INVALID_DSTACK("reg2"_Stz);
  
  ValReg* pValReg = mpTask->m_pRegFrame->pValReg(2);
  
  mpTask->m_pDataStack->vPushValue(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* reg3 -- push reg3 onto dstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegReg3(MP_Task mpTask)
{
  ENTRY("vRegReg3"_Stz);
  
  if ( ! mpTask->m_pRegFrame )
    THROW_INVALID_DSTACK("reg3"_Stz);
  
  ValReg* pValReg = mpTask->m_pRegFrame->pValReg(3);
  
  mpTask->m_pDataStack->vPushValue(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* reg4 -- push reg4 onto dstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegReg4(MP_Task mpTask)
{
  ENTRY("vRegReg4"_Stz);
  
  if ( ! mpTask->m_pRegFrame )
    THROW_INVALID_DSTACK("reg4"_Stz);
  
  ValReg* pValReg = mpTask->m_pRegFrame->pValReg(4);
  
  mpTask->m_pDataStack->vPushValue(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* reg5 -- push reg5 onto dstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegReg5(MP_Task mpTask)
{
  ENTRY("vRegReg5"_Stz);
  
  if ( ! mpTask->m_pRegFrame )
    THROW_INVALID_DSTACK("reg5"_Stz);
  
  ValReg* pValReg = mpTask->m_pRegFrame->pValReg(5);
  
  mpTask->m_pDataStack->vPushValue(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* reg6 -- push reg6 onto dstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegReg6(MP_Task mpTask)
{
  ENTRY("vRegReg6"_Stz);
  
  if ( ! mpTask->m_pRegFrame )
    THROW_INVALID_DSTACK("reg6"_Stz);
  
  ValReg* pValReg = mpTask->m_pRegFrame->pValReg(6);
  
  mpTask->m_pDataStack->vPushValue(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* reg7 -- push reg7 onto dstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegReg7(MP_Task mpTask)
{
  ENTRY("vRegReg7"_Stz);
  
  if ( ! mpTask->m_pRegFrame )
    THROW_INVALID_DSTACK("reg7"_Stz);
  
  ValReg* pValReg = mpTask->m_pRegFrame->pValReg(7);
  
  mpTask->m_pDataStack->vPushValue(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* reg8 -- push reg8 onto dstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegReg8(MP_Task mpTask)
{
  ENTRY("vRegReg8"_Stz);
  
  if ( ! mpTask->m_pRegFrame )
    THROW_INVALID_DSTACK("reg8"_Stz);
  
  ValReg* pValReg = mpTask->m_pRegFrame->pValReg(8);
  
  mpTask->m_pDataStack->vPushValue(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* reg9 -- push reg9 onto dstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegReg9(MP_Task mpTask)
{
  ENTRY("vRegReg9"_Stz);
  
  if ( ! mpTask->m_pRegFrame )
    THROW_INVALID_DSTACK("reg9"_Stz);
  
  ValReg* pValReg = mpTask->m_pRegFrame->pValReg(9);
  
  mpTask->m_pDataStack->vPushValue(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* regA -- push regA onto dstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegRegA(MP_Task mpTask)
{
  ENTRY("vRegRegA"_Stz);
  
  if ( ! mpTask->m_pRegFrame )
    THROW_INVALID_DSTACK("regA"_Stz);
  
  ValReg* pValReg = mpTask->m_pRegFrame->pValReg(10);
  
  mpTask->m_pDataStack->vPushValue(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* regB -- push regB onto dstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegRegB(MP_Task mpTask)
{
  ENTRY("vRegRegB"_Stz);
  
  if ( ! mpTask->m_pRegFrame )
    THROW_INVALID_DSTACK("regB"_Stz);
  
  ValReg* pValReg = mpTask->m_pRegFrame->pValReg(11);
  
  mpTask->m_pDataStack->vPushValue(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* regC -- push regC onto dstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegRegC(MP_Task mpTask)
{
  ENTRY("vRegRegC"_Stz);
  
  if ( ! mpTask->m_pRegFrame )
    THROW_INVALID_DSTACK("regC"_Stz);
  
  ValReg* pValReg = mpTask->m_pRegFrame->pValReg(12);
  
  mpTask->m_pDataStack->vPushValue(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* regD -- push regD onto dstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegRegD(MP_Task mpTask)
{
  ENTRY("vRegRegD"_Stz);
  
  if ( ! mpTask->m_pRegFrame )
    THROW_INVALID_DSTACK("regD"_Stz);
  
  ValReg* pValReg = mpTask->m_pRegFrame->pValReg(13);
  
  mpTask->m_pDataStack->vPushValue(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* regE -- push regE onto dstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegRegE(MP_Task mpTask)
{
  ENTRY("vRegRegE"_Stz);
  
  if ( ! mpTask->m_pRegFrame )
    THROW_INVALID_DSTACK("regE"_Stz);
  
  ValReg* pValReg = mpTask->m_pRegFrame->pValReg(14);
  
  mpTask->m_pDataStack->vPushValue(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* regF -- push regF onto dstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegRegF(MP_Task mpTask)
{
  ENTRY("vRegRegF"_Stz);
  
  if ( ! mpTask->m_pRegFrame )
    THROW_INVALID_DSTACK("regF"_Stz);
  
  ValReg* pValReg = mpTask->m_pRegFrame->pValReg(15);
  
  mpTask->m_pDataStack->vPushValue(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* regN -- push reg[n] onto dstack                                            */
/*----------------------------------------------------------------------------*/
void Task::vRegRegNth(MP_Task mpTask)
{
  ENTRY("vRegRegNth"_Stz);
  
  if ( ! mpTask->m_pRegFrame )
    THROW_INVALID_DSTACK("regN"_Stz);
  
  i8 iReg;
  if ( ! mpTask->m_pDataStack->bPop(iReg) )
    THROW_INVALID_DSTACK("regN"_Stz);
  
  ValReg* pValReg = mpTask->m_pRegFrame->pValReg(iReg);
  
  mpTask->m_pDataStack->vPushValue(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >reg0 -- pop dstack into reg0                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToReg0(MP_Task mpTask)
{
ENTRY("vRegToReg0"_Stz);
  
  if ( ! mpTask->m_pRegFrame )
    THROW_INVALID_DSTACK(">reg0"_Stz);
  
  ValReg* pValReg = mpTask->m_pRegFrame->pValReg(0);
  
  mpTask->m_pDataStack->bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >reg1 -- pop dstack into reg1                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToReg1(MP_Task mpTask)
{
  ENTRY("vRegToReg1"_Stz);
  
  if ( ! mpTask->m_pRegFrame )
    THROW_INVALID_DSTACK(">reg1"_Stz);
  
  ValReg* pValReg = mpTask->m_pRegFrame->pValReg(1);
  
  mpTask->m_pDataStack->bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >reg2 -- pop dstack into reg2                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToReg2(MP_Task mpTask)
{
  ENTRY("vRegToReg2"_Stz);
  
  if ( ! mpTask->m_pRegFrame )
    THROW_INVALID_DSTACK(">reg2"_Stz);
  
  ValReg* pValReg = mpTask->m_pRegFrame->pValReg(2);
  
  mpTask->m_pDataStack->bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >reg3 -- pop dstack into reg3                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToReg3(MP_Task mpTask)
{
  ENTRY("vRegToReg3"_Stz);
  
  if ( ! mpTask->m_pRegFrame )
    THROW_INVALID_DSTACK(">reg3"_Stz);
  
  ValReg* pValReg = mpTask->m_pRegFrame->pValReg(3);
  
  mpTask->m_pDataStack->bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >reg4 -- pop dstack into reg4                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToReg4(MP_Task mpTask)
{
  ENTRY("vRegToReg4"_Stz);
  
  if ( ! mpTask->m_pRegFrame )
    THROW_INVALID_DSTACK(">reg4"_Stz);
  
  ValReg* pValReg = mpTask->m_pRegFrame->pValReg(4);
  
  mpTask->m_pDataStack->bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >reg5 -- pop dstack into reg5                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToReg5(MP_Task mpTask)
{
  ENTRY("vRegToReg5"_Stz);
  
  if ( ! mpTask->m_pRegFrame )
    THROW_INVALID_DSTACK(">reg5"_Stz);
  
  ValReg* pValReg = mpTask->m_pRegFrame->pValReg(5);
  
  mpTask->m_pDataStack->bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >reg6 -- pop dstack into reg6                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToReg6(MP_Task mpTask)
{
  ENTRY("vRegToReg6"_Stz);
  
  if ( ! mpTask->m_pRegFrame )
    THROW_INVALID_DSTACK(">reg6"_Stz);
  
  ValReg* pValReg = mpTask->m_pRegFrame->pValReg(6);
  
  mpTask->m_pDataStack->bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >reg7 -- pop dstack into reg7                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToReg7(MP_Task mpTask)
{
  ENTRY("vRegToReg7"_Stz);
  
  if ( ! mpTask->m_pRegFrame )
    THROW_INVALID_DSTACK(">reg7"_Stz);
  
  ValReg* pValReg = mpTask->m_pRegFrame->pValReg(7);
  
  mpTask->m_pDataStack->bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >reg8 -- pop dstack into reg8                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToReg8(MP_Task mpTask)
{
  ENTRY("vRegToReg8"_Stz);
  
  if ( ! mpTask->m_pRegFrame )
    THROW_INVALID_DSTACK(">reg8"_Stz);
  
  ValReg* pValReg = mpTask->m_pRegFrame->pValReg(8);
  
  mpTask->m_pDataStack->bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >reg9 -- pop dstack into reg9                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToReg9(MP_Task mpTask)
{
  ENTRY("vRegToReg9"_Stz);
  
  if ( ! mpTask->m_pRegFrame )
    THROW_INVALID_DSTACK(">reg9"_Stz);
  
  ValReg* pValReg = mpTask->m_pRegFrame->pValReg(9);
  
  mpTask->m_pDataStack->bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >regA -- pop dstack into regA                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToRegA(MP_Task mpTask)
{
  ENTRY("vRegToRegA"_Stz);
  
  if ( ! mpTask->m_pRegFrame )
    THROW_INVALID_DSTACK(">regA"_Stz);
  
  ValReg* pValReg = mpTask->m_pRegFrame->pValReg(10);
  
  mpTask->m_pDataStack->bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >regB -- pop dstack into regB                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToRegB(MP_Task mpTask)
{
  ENTRY("vRegToRegB"_Stz);
  
  if ( ! mpTask->m_pRegFrame )
    THROW_INVALID_DSTACK(">regB"_Stz);
  
  ValReg* pValReg = mpTask->m_pRegFrame->pValReg(11);
  
  mpTask->m_pDataStack->bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >regC -- pop dstack into regC                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToRegC(MP_Task mpTask)
{
  ENTRY("vRegToRegC"_Stz);
  
  if ( ! mpTask->m_pRegFrame )
    THROW_INVALID_DSTACK(">regC"_Stz);
  
  ValReg* pValReg = mpTask->m_pRegFrame->pValReg(12);
  
  mpTask->m_pDataStack->bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >regD -- pop dstack into regD                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToRegD(MP_Task mpTask)
{
  ENTRY("vRegToRegD"_Stz);
  
  if ( ! mpTask->m_pRegFrame )
    THROW_INVALID_DSTACK(">regD"_Stz);
  
  ValReg* pValReg = mpTask->m_pRegFrame->pValReg(13);
  
  mpTask->m_pDataStack->bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >regE -- pop dstack into regE                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToRegE(MP_Task mpTask)
{
  ENTRY("vRegToRegE"_Stz);
  
  if ( ! mpTask->m_pRegFrame )
    THROW_INVALID_DSTACK(">regE"_Stz);
  
  ValReg* pValReg = mpTask->m_pRegFrame->pValReg(14);
  
  mpTask->m_pDataStack->bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >regF -- pop dstack into regF                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToRegF(MP_Task mpTask)
{
  ENTRY("vRegToRegF"_Stz);
  
  if ( ! mpTask->m_pRegFrame )
    THROW_INVALID_DSTACK(">regF"_Stz);
  
  ValReg* pValReg = mpTask->m_pRegFrame->pValReg(15);
  
  mpTask->m_pDataStack->bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >regN -- pop dstack into regN                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToRegNth(MP_Task mpTask)
{
  ENTRY("vRegToRegNth"_Stz);
  
  if ( ! mpTask->m_pRegFrame )
    THROW_INVALID_DSTACK(">regN"_Stz);
  
  i8 iReg;
  if ( ! mpTask->m_pDataStack->bPop(iReg) )
    THROW_INVALID_DSTACK(">regNth"_Stz);
  
  ValReg* pValReg = mpTask->m_pRegFrame->pValReg(iReg);
  
  mpTask->m_pDataStack->bPop(pValReg);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
