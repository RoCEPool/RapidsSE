/*============================================================================*/
/* LlcTest.cpp -- LLC Miss Test                                               */
/*============================================================================*/

#include "StorageEngine.h"

// this is too large an array to fit on thread stack so it's declared global
#define C_ELEMENTS (C_1MB * 16)
i8 xArray[C_ELEMENTS];

/*----------------------------------------------------------------------------*/
/* test                                                                       */
/*----------------------------------------------------------------------------*/
void LlcTest::vTest(MP_Task mpTask)
{
#ifdef __APPLE__
  
  PosixThread* pPosixThread = mpTask->pThread()->m_pPosixThread;
  
  pPosixThread->vEnableRealtime_2();
  
  pPosixThread->iGetInfo();
  
  // create an identity array
  for (i8 i_Element = 0; i_Element < C_ELEMENTS; i_Element++)
    xArray[i_Element] = i_Element;
  
  i8 iIteration = 0;
  

  //           Step   Miss
  //           Length Ratio
  vAnIteration(iIteration++, 1,     1);
  vAnIteration(iIteration++, 1,     3);
  vAnIteration(iIteration++, 1,     5);

  vAnIteration(iIteration++, 3,     1);
  vAnIteration(iIteration++, 3,     3);
  vAnIteration(iIteration++, 3,     5);
  
  vAnIteration(iIteration++, 5,     1);
  vAnIteration(iIteration++, 5,     3);
  vAnIteration(iIteration++, 5,     5);
  
  vAnIteration(iIteration++, 10,    1);
  vAnIteration(iIteration++, 10,    3);
  vAnIteration(iIteration++, 10,    5);
  
  vAnIteration(iIteration++, 30,    1);
  vAnIteration(iIteration++, 30,    3);
  vAnIteration(iIteration++, 30,    5);
  
  vAnIteration(iIteration++, 50,    1);
  vAnIteration(iIteration++, 50,    3);
  vAnIteration(iIteration++, 50,    5);
  
  vAnIteration(iIteration++, 100,   1);
  vAnIteration(iIteration++, 100,   3);
  vAnIteration(iIteration++, 100,   5);
  
  vAnIteration(iIteration++, 300,   1);
  vAnIteration(iIteration++, 300,   3);
  vAnIteration(iIteration++, 300,   5);
  
  vAnIteration(iIteration++, 500,   1);
  vAnIteration(iIteration++, 500,   3);
  vAnIteration(iIteration++, 500,   5);
  
  vAnIteration(iIteration++, 1000,  1);
  vAnIteration(iIteration++, 1000,  3);
  vAnIteration(iIteration++, 1000,  5);
  
  vAnIteration(iIteration++, 3000,  1);
  vAnIteration(iIteration++, 3000,  3);
  vAnIteration(iIteration++, 3000,  5);
  
  vAnIteration(iIteration++, 5000,  1);
  vAnIteration(iIteration++, 5000,  3);
  vAnIteration(iIteration++, 5000,  5);
  
  vAnIteration(iIteration++, 10000, 1);
  vAnIteration(iIteration++, 10000, 3);
  vAnIteration(iIteration++, 10000, 5);
  
  //vAnIteration(iIteration++, 10,    10);

#endif
}

/*----------------------------------------------------------------------------*/
/* test                                                                       */
/*----------------------------------------------------------------------------*/
void LlcTest::vAnIteration(i8 iIteration, i8 nStepLength, i8 nMissRatio)
{
#ifdef __APPLE__

  i8 nMissBoundary = nStepLength * nMissRatio;
  
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
  
  #define C_REPEATED 1
  for (i8 i_Count = 0; i_Count < C_REPEATED; i_Count++) {
    
    for (i8 i_Element = 0; i_Element < C_ELEMENTS; i_Element++) {
      
      nIterations++;
      
      // our pseudo step - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      i8 i_Random = Random::iRandom(C_ELEMENTS);
      iRandomSum += xArray[i_Random];
      if (i_Element % nStepLength != 0)
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
      if (iDeltaMisses > nMissBoundary) {
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
  
  
  vTrace("\n-- Iteration %lld ----------------------------------------------------------------------\n"
         "C_ELEMENTS %lld Array %s StepLength %lld MissBoundary %lld\n"
         "Elapsed %s Iterations %s Misses %s Adjusted %s CacheRatio %lld%% AdjMissRatio %lld%%\n"
         "Steps %s Switches %lld Every %s Zeroes %lld Boundaries %lld\n"
         "MinDelta %lld AvgDelta %lld MaxDelta %lld WorstDelta %lld\n"
         "Sum %lld AvgElement %lld\n"_Stz,
         
         iIteration,
         
         C_ELEMENTS,
         sSizeMag.aChar(),
         nStepLength,
         nMissBoundary,
         
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
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
