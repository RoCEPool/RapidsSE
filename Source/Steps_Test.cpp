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

#define nElems (C_1M * 16)
i8 xArray[nElems];

/*----------------------------------------------------------------------------*/
/* test::test0 -- test0                                                       */
/*----------------------------------------------------------------------------*/
void Step::vTestTest0(MP_Task mpTask)
{
  ENTRY("vTestTest0"_Stz);
  
#ifdef __APPLE__
  PosixThread* pPosixThread = mpTask->pThread()->m_pPosixThread;
  pPosixThread->vEnableRealtime_2();

  Timer xTimer;
  xTimer.vStart();

  i8 iStartCore = iPmc_Core();
  i8 iStartMisses = iPmc_GetLlcMisses();
  
  pPosixThread->iGetInfo();
  
  for (i8 i_Elem = 0; i_Elem < nElems; i_Elem++)
    xArray[i_Elem] = i_Elem;
  
  i8 iSum = 0;
  i8 nIterations = 0;
  i8 nSwitches = 0;
  
  for (i8 nTimes = 0; nTimes < 10; nTimes++) {
  
    for (i8 i_Elem = 0; i_Elem < nElems; i_Elem++) {
      
      nIterations++;
      
      i8 i_Random = Random::iRandom(nElems-1);
      //vTrace("iSum %lld xArray %lld\n"_Stz,iSum,xArray[i_Random]);
      iSum += xArray[i_Random];
      //vTrace("iSum %lld\n"_Stz,iSum);
      
      if (iPmc_Core() != iStartCore) {
        nSwitches++;
        iStartCore = iPmc_Core();
      }
      
      //if (nIterations % 100000 == 0)
      //  pPosixThread->vEnableRealtime_2();
    }
  }
  
  xTimer.vUpdate();
  
  s32 sElapsedMagnitude;
  Magnitudes::vTimeMag(xTimer.iNanoseconds(),sElapsedMagnitude);
  
  i8 iRate = xTimer.iNanoseconds() / nSwitches;
  s32 sRateMagnitude;
  Magnitudes::vTimeMag(iRate,sRateMagnitude);

  vTrace("LlcMisses %lld; iSum %lld; Elapsed %s; Iterations %lld; Switches %lld; Rate %s\n"_Stz,
         iPmc_GetLlcMisses() - iStartMisses,
         iSum,
         sElapsedMagnitude.aChar(),
         nIterations,
         nSwitches,
         sRateMagnitude.aChar());

#endif
  
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
