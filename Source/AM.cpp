/*============================================================================*/
/* AM.cpp -- AttachMap helper                                                 */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

// Arg
bool AM::bDeveloper(void)     { return gpAttachMap->Arg.m_bDeveloper;         }
i8   AM::nDummyLoop(void)     { return gpAttachMap->Arg.m_nDummyLoop;         }
i8   AM::nDummySleep(void)    { return gpAttachMap->Arg.m_nDummySleep;        }
i8   AM::nDummyThreads(void)  { return gpAttachMap->Arg.m_nDummyThreads;      }
i8   AM::iJumboSize(void)     { return gpAttachMap->Arg.m_iJumboSize;         }
bool AM::bLog(void)           { return gpAttachMap->Arg.m_bLog;               }
i8   AM::nMaxLoadErrors(void) { return gpAttachMap->Arg.m_nMaxLoadErrors;     }
i8   AM::nMaxReadyLP(void)    { return gpAttachMap->Arg.m_nMaxReadyLP;        }
i8   AM::nNodes(void)         { return gpAttachMap->Arg.m_nNodes;             }
Stz* AM::szRegionName(void)   { return gpAttachMap->Arg.m_sRegionName.aStz(); }
i8   AM::iRegionSize(void)    { return gpAttachMap->Arg.m_iRegionSize;        }
i8   AM::nSlotsPerNode(void)  { return gpAttachMap->Arg.m_nSlotsPerNode;      }
i8   AM::nSubIngesters(void)  { return gpAttachMap->Arg.m_nSubIngesters;      }
i8   AM::nSubInserters(void)  { return gpAttachMap->Arg.m_nSubInserters;      }
i8   AM::nSubLoaders(void)    { return gpAttachMap->Arg.m_nSubLoaders;        }
i8   AM::nThreads(void)       { return gpAttachMap->Arg.m_nThreads;           }

//
i8   AM::iElapsedSsec(void)   { return gpAttachMap->m_oBirth.iElapsedSsec();  }
ForthWord* AM::pForthWord(void)     { return gpAttachMap->m_pForthWord;       }
i8   AM::nIndexEntries(void)  { return gpAttachMap->m_nIndexEntries;          }
i8   AM::iPid(void)           { return gpAttachMap->m_iPid;                   }
bool AM::bProgress(void)      { return gpAttachMap->m_bProgress;              }
bool AM::bMetadata(void)      { return gpAttachMap->m_bMetadata;              }
Statistics* AM::pStatistics(void)   { return & gpAttachMap->m_oStatistics;    }
StepTiming* AM::pStepTiming(void)   { return gpAttachMap->m_pStepTiming;      }
PoolBuffer* AM::pTakenBuffers(void) { return gpAttachMap->m_pTakenBuffers;    }
i8   AM::nTakenBytes(void)    { return gpAttachMap->m_nTakenBytes;            }
ForthWord* AM::pTenants(void) { return gpAttachMap->m_pTenants;               }
void AM::vThreadBegins(void)  { ++ gpAttachMap->m_ixThreads;                  }
i8   AM::nThreadsBegun(void)  { return * gpAttachMap->m_ixThreads;            }
bool AM::bTracepoint(void)    { return gpAttachMap->m_bTracepoint;            }
TxSet* AM::pTxSet(void)       { return gpAttachMap->m_pTxSet;                 }
bool AM::bVerbose(void)       { return gpAttachMap->m_bVerbose;               }

//
void AM::vForthWord(ForthWord *pWord)  {
  gpAttachMap->m_pForthWord = pWord;
}
void AM::vStepTiming(StepTiming* pStepTiming) {
  gpAttachMap->m_pStepTiming = pStepTiming;
}
void AM::vTakenBuffers(PoolBuffer* pTakenBuffers) {
  gpAttachMap->m_pTakenBuffers = pTakenBuffers;
}
void AM::vTakeBytes(i8 nTakeBytes) {
  gpAttachMap->m_nTakenBytes = nTakeBytes;
}
void AM::vTakenBytes(i8 nTakenBytes) {
  gpAttachMap->m_nTakenBytes += nTakenBytes;
}
void AM::vTenants(ForthWord *pTenants) {
  gpAttachMap->m_pTenants = pTenants;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
