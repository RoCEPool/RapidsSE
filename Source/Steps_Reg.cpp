/*============================================================================*/
/* Steps_Reg.cpp - reg step group                                             */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern Arguments       gxArguments;

extern AttachMap*      gpAttachMap;
extern BufferPool*     gpBufferPool;
extern Heap*           gpMediumHeap;
extern BPA*            gpBPA;
extern Dic*            gpMainDic;
extern Word*           gpWord;

extern Grid*           gpGrid;
extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;
extern TxCollection*   gpTxCollection;

extern NP_Task         gnpRootTask;
extern StepBoard*      gpStepBoard;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// registers: //////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Reg(void)
{
  vAddPrimitive("reg.pop",          &vRegPop,          false);
  vAddPrimitive("reg.push",         &vRegPush,         false);
  vAddPrimitive("reg0",             &vRegReg0,         false);
  vAddPrimitive("reg1",             &vRegReg1,         false);
  vAddPrimitive("reg2",             &vRegReg2,         false);
  vAddPrimitive("reg3",             &vRegReg3,         false);
  vAddPrimitive("reg4",             &vRegReg4,         false);
  vAddPrimitive("reg5",             &vRegReg5,         false);
  vAddPrimitive("reg6",             &vRegReg6,         false);
  vAddPrimitive("reg7",             &vRegReg7,         false);
  vAddPrimitive("reg8",             &vRegReg8,         false);
  vAddPrimitive("reg9",             &vRegReg9,         false);
  vAddPrimitive("regA",             &vRegRegA,         false);
  vAddPrimitive("regB",             &vRegRegB,         false);
  vAddPrimitive("regC",             &vRegRegC,         false);
  vAddPrimitive("regD",             &vRegRegD,         false);
  vAddPrimitive("regE",             &vRegRegE,         false);
  vAddPrimitive("regF",             &vRegRegF,         false);
  vAddPrimitive("regN",             &vRegRegNth,       false);
  vAddPrimitive(">reg0",            &vRegToReg0,       false);
  vAddPrimitive(">reg1",            &vRegToReg1,       false);
  vAddPrimitive(">reg2",            &vRegToReg2,       false);
  vAddPrimitive(">reg3",            &vRegToReg3,       false);
  vAddPrimitive(">reg4",            &vRegToReg4,       false);
  vAddPrimitive(">reg5",            &vRegToReg5,       false);
  vAddPrimitive(">reg6",            &vRegToReg6,       false);
  vAddPrimitive(">reg7",            &vRegToReg7,       false);
  vAddPrimitive(">reg8",            &vRegToReg8,       false);
  vAddPrimitive(">reg9",            &vRegToReg9,       false);
  vAddPrimitive(">regA",            &vRegToRegA,       false);
  vAddPrimitive(">regB",            &vRegToRegB,       false);
  vAddPrimitive(">regC",            &vRegToRegC,       false);
  vAddPrimitive(">regD",            &vRegToRegD,       false);
  vAddPrimitive(">regE",            &vRegToRegE,       false);
  vAddPrimitive(">regF",            &vRegToRegF,       false);
  vAddPrimitive(">regN",            &vRegToRegNth,     false);
}

/*----------------------------------------------------------------------------*/
/* reg.pop -- pop register set                                                */
/*----------------------------------------------------------------------------*/
void Task::vRegPop(NNP_Task nnpTask)
{
  ENTRY("vRegPop");
  
  RegFrame* pRegFrame = nnpTask->m_pRegFrame;
  
  if ( ! pRegFrame )
    THROW_INVALID_DSTACK("reg.pop");
  
  nnpTask->m_pRegFrame = nnpTask->m_pRegFrame->pNext();
  
  //vTrace("Deleting frame %s\n",pRegFrame->m_sName.a());
  
  delete pRegFrame;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* reg.push -- push a register set                                            */
/*----------------------------------------------------------------------------*/
void Task::vRegPush(NNP_Task nnpTask)
{
ENTRY("vRegPush");
  
  RegFrame* pRegFrame = new RegFrame;
  
  pRegFrame->vNext(nnpTask->m_pRegFrame);
  
  nnpTask->m_pRegFrame = pRegFrame;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* reg0 -- push reg0 onto dstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegReg0(NNP_Task nnpTask)
{
ENTRY("vRegReg0");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK("reg0");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(0);
  
  nnpTask->m_pDataStack->vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* reg1 -- push reg1 onto dstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegReg1(NNP_Task nnpTask)
{
  ENTRY("vRegReg1");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK("reg1");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(1);
  
  nnpTask->m_pDataStack->vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* reg2 -- push reg2 onto dstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegReg2(NNP_Task nnpTask)
{
  ENTRY("vRegReg2");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK("reg2");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(2);
  
  nnpTask->m_pDataStack->vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* reg3 -- push reg3 onto dstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegReg3(NNP_Task nnpTask)
{
  ENTRY("vRegReg3");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK("reg3");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(3);
  
  nnpTask->m_pDataStack->vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* reg4 -- push reg4 onto dstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegReg4(NNP_Task nnpTask)
{
  ENTRY("vRegReg4");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK("reg4");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(4);
  
  nnpTask->m_pDataStack->vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* reg5 -- push reg5 onto dstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegReg5(NNP_Task nnpTask)
{
  ENTRY("vRegReg5");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK("reg5");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(5);
  
  nnpTask->m_pDataStack->vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* reg6 -- push reg6 onto dstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegReg6(NNP_Task nnpTask)
{
  ENTRY("vRegReg6");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK("reg6");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(6);
  
  nnpTask->m_pDataStack->vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* reg7 -- push reg7 onto dstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegReg7(NNP_Task nnpTask)
{
  ENTRY("vRegReg7");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK("reg7");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(7);
  
  nnpTask->m_pDataStack->vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* reg8 -- push reg8 onto dstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegReg8(NNP_Task nnpTask)
{
  ENTRY("vRegReg8");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK("reg8");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(8);
  
  nnpTask->m_pDataStack->vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* reg9 -- push reg9 onto dstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegReg9(NNP_Task nnpTask)
{
  ENTRY("vRegReg9");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK("reg9");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(9);
  
  nnpTask->m_pDataStack->vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* regA -- push regA onto dstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegRegA(NNP_Task nnpTask)
{
  ENTRY("vRegRegA");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK("regA");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(10);
  
  nnpTask->m_pDataStack->vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* regB -- push regB onto dstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegRegB(NNP_Task nnpTask)
{
  ENTRY("vRegRegB");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK("regB");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(11);
  
  nnpTask->m_pDataStack->vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* regC -- push regC onto dstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegRegC(NNP_Task nnpTask)
{
  ENTRY("vRegRegC");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK("regC");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(12);
  
  nnpTask->m_pDataStack->vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* regD -- push regD onto dstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegRegD(NNP_Task nnpTask)
{
  ENTRY("vRegRegD");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK("regD");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(13);
  
  nnpTask->m_pDataStack->vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* regE -- push regE onto dstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegRegE(NNP_Task nnpTask)
{
  ENTRY("vRegRegE");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK("regE");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(14);
  
  nnpTask->m_pDataStack->vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* regF -- push regF onto dstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegRegF(NNP_Task nnpTask)
{
  ENTRY("vRegRegF");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK("regF");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(15);
  
  nnpTask->m_pDataStack->vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* regN -- push reg[n] onto dstack                                            */
/*----------------------------------------------------------------------------*/
void Task::vRegRegNth(NNP_Task nnpTask)
{
  ENTRY("vRegRegNth");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK("regN");
  
  i8 iReg;
  if ( ! nnpTask->m_pDataStack->bPop(iReg) )
    THROW_INVALID_DSTACK("regN");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(iReg);
  
  nnpTask->m_pDataStack->vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >reg0 -- pop dstack into reg0                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToReg0(NNP_Task nnpTask)
{
ENTRY("vRegToReg0");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK(">reg0");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(0);
  
  nnpTask->m_pDataStack->bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >reg1 -- pop dstack into reg1                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToReg1(NNP_Task nnpTask)
{
  ENTRY("vRegToReg1");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK(">reg1");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(1);
  
  nnpTask->m_pDataStack->bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >reg2 -- pop dstack into reg2                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToReg2(NNP_Task nnpTask)
{
  ENTRY("vRegToReg2");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK(">reg2");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(2);
  
  nnpTask->m_pDataStack->bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >reg3 -- pop dstack into reg3                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToReg3(NNP_Task nnpTask)
{
  ENTRY("vRegToReg3");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK(">reg3");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(3);
  
  nnpTask->m_pDataStack->bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >reg4 -- pop dstack into reg4                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToReg4(NNP_Task nnpTask)
{
  ENTRY("vRegToReg4");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK(">reg4");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(4);
  
  nnpTask->m_pDataStack->bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >reg5 -- pop dstack into reg5                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToReg5(NNP_Task nnpTask)
{
  ENTRY("vRegToReg5");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK(">reg5");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(5);
  
  nnpTask->m_pDataStack->bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >reg6 -- pop dstack into reg6                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToReg6(NNP_Task nnpTask)
{
  ENTRY("vRegToReg6");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK(">reg6");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(6);
  
  nnpTask->m_pDataStack->bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >reg7 -- pop dstack into reg7                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToReg7(NNP_Task nnpTask)
{
  ENTRY("vRegToReg7");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK(">reg7");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(7);
  
  nnpTask->m_pDataStack->bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >reg8 -- pop dstack into reg8                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToReg8(NNP_Task nnpTask)
{
  ENTRY("vRegToReg8");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK(">reg8");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(8);
  
  nnpTask->m_pDataStack->bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >reg9 -- pop dstack into reg9                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToReg9(NNP_Task nnpTask)
{
  ENTRY("vRegToReg9");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK(">reg9");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(9);
  
  nnpTask->m_pDataStack->bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >regA -- pop dstack into regA                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToRegA(NNP_Task nnpTask)
{
  ENTRY("vRegToRegA");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK(">regA");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(10);
  
  nnpTask->m_pDataStack->bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >regB -- pop dstack into regB                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToRegB(NNP_Task nnpTask)
{
  ENTRY("vRegToRegB");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK(">regB");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(11);
  
  nnpTask->m_pDataStack->bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >regC -- pop dstack into regC                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToRegC(NNP_Task nnpTask)
{
  ENTRY("vRegToRegC");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK(">regC");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(12);
  
  nnpTask->m_pDataStack->bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >regD -- pop dstack into regD                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToRegD(NNP_Task nnpTask)
{
  ENTRY("vRegToRegD");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK(">regD");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(13);
  
  nnpTask->m_pDataStack->bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >regE -- pop dstack into regE                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToRegE(NNP_Task nnpTask)
{
  ENTRY("vRegToRegE");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK(">regE");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(14);
  
  nnpTask->m_pDataStack->bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >regF -- pop dstack into regF                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToRegF(NNP_Task nnpTask)
{
  ENTRY("vRegToRegF");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK(">regF");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(15);
  
  nnpTask->m_pDataStack->bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >regN -- pop dstack into regN                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToRegNth(NNP_Task nnpTask)
{
  ENTRY("vRegToRegNth");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK(">regN");
  
  i8 iReg;
  if ( ! nnpTask->m_pDataStack->bPop(iReg) )
    THROW_INVALID_DSTACK(">regNth");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(iReg);
  
  nnpTask->m_pDataStack->bPop(pValReg);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
