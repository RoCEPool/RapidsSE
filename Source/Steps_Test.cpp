/*============================================================================*/
/* Steps_Test.cpp - test step group                                           */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Test(void)
{
  vAddStep("test::test0"_Stz,  &Step::vTestTest0);
  vAddStep("test::test1"_Stz,  &Step::vTestTest1);
  vAddStep("test::test2"_Stz,  &Step::vTestTest2);
  vAddStep("test::test3"_Stz,  &Step::vTestTest3);
  vAddStep("test::test4"_Stz,  &Step::vTestTest4);
  vAddStep("test::test5"_Stz,  &Step::vTestTest5);
  vAddStep("test::test6"_Stz,  &Step::vTestTest6);
  vAddStep("test::test7"_Stz,  &Step::vTestTest7);
  vAddStep("test::test8"_Stz,  &Step::vTestTest8);
  vAddStep("test::test9"_Stz,  &Step::vTestTest9);
}

#define nElem (1024*1024*16)
i8 xI8[nElem];

/*----------------------------------------------------------------------------*/
/* test::test0 -- test0                                                       */
/*----------------------------------------------------------------------------*/
void Step::vTestTest0(MP_Task mpTask)
{
  ENTRY("vTestTest0"_Stz);
  
  PosixThread* pPosixThread = mpTask->pThread()->m_pPosixThread;
  pPosixThread->vEnableRealtime();

  i8 iSum = 0;
  i8 iStartCore = 0 ; //iPmc_Core();
  i8 iStartMisses = 0 ; //iPmc_GetLlcMisses();
  
  
  //vTrace("iPmc_Info(%lld) %llx\n"_Stz,iPmc_Core(),iPmc_Info());
  
  //vTrace("Starting(%lld)   iPmc_GetLlcMisses %lld\n"_Stz,
  //       iPmc_Core(),iPmc_GetLlcMisses());
  
  for (i8 i = 0; i < nElem; i++)
    xI8[i] = i;
  //vTrace("Sequential iPmc_GetLlcMisses %lld; iSum %lld\n"_Stz,
  //       iPmc_GetLlcMisses(),iSum);
  for (i8 i = 0; i < nElem; i++) {
    i8 iElem = Random::iRandom(nElem-1);
    //vTrace("iElem %lld; iPmc_GetLlcMisses %llx\n"_Stz,
    //       iElem,iPmc_GetLlcMisses());
    iSum += xI8[iElem];
    
//    if (iPmc_Core() != iStartCore) {
//      iStartCore = iPmc_Core();
//      vTrace("Now on (%lld) at %lld\n"_Stz,iStartCore,i);
//    }
  }
  
//  vTrace("Random(%lld)     LlcMisses %lld; iSum %lld\n"_Stz,
//        iPmc_Core(),iPmc_GetLlcMisses() - iStartMisses,iSum);

  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* test::test1 -- test1                                                       */
/*----------------------------------------------------------------------------*/
void Step::vTestTest1(MP_Task mpTask)
{
  ENTRY("vTestTest1"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* test::test2 -- test2                                                       */
/*----------------------------------------------------------------------------*/
void Step::vTestTest2(MP_Task mpTask)
{
  ENTRY("vTestTest2"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* test::test3 -- test3                                                       */
/*----------------------------------------------------------------------------*/
void Step::vTestTest3(MP_Task mpTask)
{
  ENTRY("vTestTest3"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* test::test4 -- test4                                                       */
/*----------------------------------------------------------------------------*/
void Step::vTestTest4(MP_Task mpTask)
{
  ENTRY("vTestTest4"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* test::test5 -- test5                                                       */
/*----------------------------------------------------------------------------*/
void Step::vTestTest5(MP_Task mpTask)
{
  ENTRY("vTestTest5"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* test::test6 -- test6                                                       */
/*----------------------------------------------------------------------------*/
void Step::vTestTest6(MP_Task mpTask)
{
  ENTRY("vTestTest6"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* test::test7 -- test7                                                       */
/*----------------------------------------------------------------------------*/
void Step::vTestTest7(MP_Task mpTask)
{
  ENTRY("vTestTest7"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* test::test8 -- test8                                                       */
/*----------------------------------------------------------------------------*/
void Step::vTestTest8(MP_Task mpTask)
{
  ENTRY("vTestTest8"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* test::test9 -- test9                                                       */
/*----------------------------------------------------------------------------*/
void Step::vTestTest9(MP_Task mpTask)
{
  ENTRY("vTestTest9 "_Stz);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
