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

// this is too large an array to fit on thread stack so it's declared global
#define C_ELEMENTS (C_1MB * 64)
i8 xArray[C_ELEMENTS];

/*----------------------------------------------------------------------------*/
/* test::test0 -- analyse llc misses                                          */
/*----------------------------------------------------------------------------*/
void Step::vTestTest0(MP_Task mpTask)
{
  ENTRY("vTestTest0"_Stz);
  
#ifdef __APPLE__
  PosixThread* pPosixThread = mpTask->pThread()->m_pPosixThread;
  
  pPosixThread->vEnableRealtime_2();

  pPosixThread->iGetInfo();
  
  // create an identity array
  for (i8 i_Element = 0; i_Element < C_ELEMENTS; i_Element++)
    xArray[i_Element] = i_Element;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Timer xElapsedTimer;
  xElapsedTimer.vStart();

  i8 iRandomSum = 0;
  i8 nIterations = 0;
  i8 nSwitches = 0;
  i8 nZeroes = 0;
  i8 nBoundaries = 0;
  i8 nSumMisses = 0;
  i8 iMinDelta = C_I8_MAX;
  i8 iMaxDelta = 0;
  i8 iWorstDelta = 0;
  i8 nSteps = 0;
  
  i8 iEnterCore = iPmc_Core();
  i8 iEnterMisses = iPmc_GetLlcMisses();
  
  #define C_REPEATED 10
  for (i8 i_Count = 0; i_Count < C_REPEATED; i_Count++) {
  
    for (i8 i_Element = 0; i_Element < C_ELEMENTS; i_Element++) {

      nIterations++;

      // our pseudo step - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      #define C_STEP_LENGTH 1
      #define C_MISS_BOUNDARY (C_STEP_LENGTH * 3)
      i8 i_Random = Random::iRandom(C_ELEMENTS);
      iRandomSum += xArray[i_Random];
      if (i_Element % C_STEP_LENGTH != 0)
        continue;
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      
      nSteps++;

      i8 iExitCore = iPmc_Core();
      i8 iExitMisses = iPmc_GetLlcMisses();

      // ignore if we switched cores
      if (iExitCore != iEnterCore) {
        nSwitches++;
        iEnterCore = iPmc_Core();
        iEnterMisses = iPmc_GetLlcMisses();
        continue;
      }
      
      i8 iDeltaMisses = iExitMisses - iEnterMisses;
      if (iDeltaMisses > iWorstDelta)
        iWorstDelta = iDeltaMisses;

      // ignore if delta misses is out of bounds
      if (iDeltaMisses <= 0) {
        nZeroes++;
        iEnterCore = iPmc_Core();
        iEnterMisses = iPmc_GetLlcMisses();
        continue;
      }
      if (iDeltaMisses > C_MISS_BOUNDARY) {
        nBoundaries++;
        iEnterCore = iPmc_Core();
        iEnterMisses = iPmc_GetLlcMisses();
        continue;
      }

      nSumMisses += iDeltaMisses;
      
      if (iDeltaMisses > iMaxDelta)
        iMaxDelta = iDeltaMisses;
      if (iDeltaMisses < iMinDelta)
        iMinDelta = iDeltaMisses;

      iEnterCore = iPmc_Core();
      iEnterMisses = iPmc_GetLlcMisses();
    }
  }
  
  xElapsedTimer.vUpdate();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  i8 iEvery = DIV(xElapsedTimer.iNanoseconds(),nSwitches);

  i8 iAvgDelta = nSumMisses / (nSteps - nSwitches - nZeroes - nBoundaries);
  
  i8 iAdjMisses = nSumMisses
                    + (iAvgDelta * (nSwitches))
                    + (iMaxDelta * (nBoundaries))
                    ;
  
  i8 iAdjMissRatio = iAdjMisses * 100 / nIterations;
  
  i8 iCacheRatio = C_1MB * 30 * 100 / sizeof(xArray);

  i8 iAvgElement = iRandomSum / nIterations;
  
  s32 sElapsedMag;
  Magnitudes::vTimeMag(xElapsedTimer.iNanoseconds(),sElapsedMag);
  
  s32 sSizeMag;
  Magnitudes::vByteMag(sizeof(xArray),sSizeMag);
  
  s32 sIterationsMag;
  Magnitudes::vUnitMag(nIterations,sIterationsMag);
  
  s32 sMissesMag;
  Magnitudes::vUnitMag(nSumMisses,sMissesMag);
  
  s32 sAdjustedMag;
  Magnitudes::vUnitMag(iAdjMisses,sAdjustedMag);
  
  s32 sStepsMag;
  Magnitudes::vUnitMag(nSteps,sStepsMag);
  
  s32 sEveryMag;
  Magnitudes::vTimeMag(iEvery,sEveryMag);
  
  
  vTrace("C_ELEMENTS %lld Array %s C_REPEATED %lld C_STEP_LENGTH %lld C_MISS_BOUNDARY %lld\n"
         "Elapsed %s Iterations %s Misses %s Adjusted %s CacheRatio %lld%% AdjMissRatio %lld%%\n"
         "Steps %s Switches %lld Every %s Zeroes %lld Boundaries %lld\n"
         "MinDelta %lld AvgDelta %lld MaxDelta %lld WorstDelta %lld\n"
         "Sum %lld AvgElement %lld\n"_Stz,
         
         C_ELEMENTS,
         sSizeMag.aChar(),
         C_REPEATED,
         C_STEP_LENGTH,
         C_MISS_BOUNDARY,
         
         sElapsedMag.aChar(),
         sIterationsMag.aChar(),
         sMissesMag.aChar(),
         sAdjustedMag.aChar(),
         iCacheRatio,
         iAdjMissRatio,
         
         sStepsMag.aChar(),
         nSwitches,
         sEveryMag.aChar(),
         nZeroes,
         nBoundaries,
         
         iMinDelta,
         iAvgDelta,
         iMaxDelta,
         iWorstDelta,
         
         iRandomSum,
         iAvgElement
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
