/*============================================================================*/
/* AM.hpp -- AttachMap helper                                                 */
/*============================================================================*/
#pragma once

namespace StorageEngine
{
  class AM
  {
  public:
    // args
    static bool bDeveloper(void);
    static i8   nDummyLoop(void);
    static i8   nDummySleep(void);
    static i8   nDummyThreads(void);
    static i8   iJumboSize(void);
    static bool bLog(void);
    static i8   nMaxLoadErrors(void);
    static i8   nMaxReadyLP(void);
    static i8   nNodes(void);
    static Stz* szRegionName(void);
    static i8   iRegionSize(void);
    static i8   nSlotsPerNode(void);
    static i8   nSubIngesters(void);
    static i8   nSubInserters(void);
    static i8   nSubLoaders(void);
    static i8   nThreads(void);

    //
    static i8   iElapsedSsec(void);
    static ForthWord* pForthWord(void);
    static i8   nIndexEntries(void);
    static i8   iPid(void);
    static bool bProgress(void);
    static bool bMetadata(void);
    static Statistics* pStatistics(void);
    static StepTiming* pStepTiming(void);
    static PoolBuffer* pTakenBuffers(void);
    static i8   nTakenBytes(void);
    static ForthWord* pTenants(void);
    static void vThreadBegins(void);
    static i8   nThreadsBegun(void);
    static bool bTracepoint(void);
    static TxSet* pTxSet(void);
    static bool bVerbose(void);
  
    //
    static void vForthWord(ForthWord* pForthWord);
    static void vStepTiming(StepTiming* pStepTiming);
    static void vTakenBuffers(PoolBuffer* pTakenBuffers);
    static void vTakeBytes(i8 nTakeBytes);
    static void vTakenBytes(i8 nTakenBytes);
    static void vTenants(ForthWord* pTenants);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
