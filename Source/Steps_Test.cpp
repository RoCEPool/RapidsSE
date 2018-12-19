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

// this tool large an array to fit on thread stack so it's declared as a global
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

  pPosixThread->iGetInfo();
  
  for (i8 i_Elem = 0; i_Elem < nElems; i_Elem++)
    xArray[i_Elem] = i_Elem;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Timer xElapsedTimer;
  xElapsedTimer.vStart();

  i8 iSumArray = 0;
  i8 nIterations = 0;
  i8 nSwitches = 0;
  i8 nBoundaries = 0;
  
  i8 nSumMisses = 0;
  i8 iMaxDelta = 0;
  
  for (i8 nTimes = 0; nTimes < 10; nTimes++) {
  
    for (i8 i_Elem = 0; i_Elem < nElems; i_Elem++) {
      
      i8 iEnterCore = iPmc_Core();
      i8 iEnterMisses = iPmc_GetLlcMisses();
      
      // our pseudo step - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      nIterations++;
      i8 i_Random = Random::iRandom(nElems-1);
      iSumArray += xArray[i_Random];
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

      i8 iExitCore = iPmc_Core();
      i8 iExitMisses = iPmc_GetLlcMisses();

      // ignore if we switched cores
      if (iExitCore != iEnterCore) {
        nSwitches++;
        continue;
      }
      
      i8 iDeltaMisses = iExitMisses - iEnterMisses;
      
      // ignore if delta misses is out of bounds
      if (iDeltaMisses <= 0 || iDeltaMisses > 1000) {
        nBoundaries++;
        continue;
      }
      
      if (nSumMisses < 0) {
        vTrace("negative at A: %lld\n"_Stz,nSumMisses);
      }
      
      nSumMisses += iDeltaMisses;
      
      if (nSumMisses < 0) {
        vTrace("negative at B: %lld\n"_Stz,nSumMisses);
      }
      
      if (iDeltaMisses > iMaxDelta)
        iMaxDelta = iDeltaMisses;
    }
  }
  
  xElapsedTimer.vUpdate();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  
  s32 sElapsedMagnitude;
  Magnitudes::vTimeMag(xElapsedTimer.iNanoseconds(),sElapsedMagnitude);
  
  i8 iRate = xElapsedTimer.iNanoseconds() / nSwitches;
  s32 sRateMagnitude;
  Magnitudes::vTimeMag(iRate,sRateMagnitude);

  vTrace("Elapsed %s Iterations %lld Switches %lld Boundaries %lld SumMisses %lld Rate %s MaxDelta %lld\n"_Stz,
         sElapsedMagnitude.aChar(),
         nIterations,
         nSwitches,
         nBoundaries,
         nSumMisses,
         sRateMagnitude.aChar(),
         iMaxDelta
         );

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
