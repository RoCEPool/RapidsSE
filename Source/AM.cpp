/*============================================================================*/
/* AM.cpp -- AttachMap helper                                                 */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap*  gpAttachMap;
extern BufferPool* gpBufferPool;
extern File*       gpTraceFile;


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
ApiState* AM::pApiState(void) { return gpAttachMap->m_pApiState;              }
bool AM::bBootError(void)     { return gpAttachMap->m_bBootError;             }
BufferPool* AM::pBufferPool(void) { return gpAttachMap->m_pBufferPool;        }
CarryOver* AM::pCarryOver(void) { return gpAttachMap->m_pCarryOver;           }
Lock* AM::pDdlLock(void)      { return & gpAttachMap->m_okDdlLock;            }
Lock* AM::pDicLock(void)      { return & gpAttachMap->m_okDicLock;            }
i8   AM::iElapsedSsec(void)   { return gpAttachMap->m_oBirth.iElapsedSsec();  }
i8   AM::iEpoch(void)         { return gpAttachMap->m_iEpoch;                 }
Formatters* AM::pFormatters(void) { return & gpAttachMap->m_oFormatters;      }
ForthWord* AM::pForthWord(void) { return gpAttachMap->m_pForthWord;           }
Grid* AM::pGrid(void)         { return gpAttachMap->m_pGrid;                  }
HeapMem* AM::pHeapMem(void)   { return & gpAttachMap->m_xHeapMem;             }
i8   AM::nIndexEntries(void)  { return gpAttachMap->m_nIndexEntries;          }
LockTracker* AM::pLockTracker(void) { return & gpAttachMap->m_oLockTracker;   }
EnumFlags* AM::pMarkAction(void) { return & gpAttachMap->m_oMarkAction;       }
EnumFlags* AM::pMarkApi(void) { return & gpAttachMap->m_oMarkApi;             }
EnumFlags* AM::pMarkJob(void) { return & gpAttachMap->m_oMarkJob;             }
EnumFlags* AM::pMarkResource(void) { return & gpAttachMap->m_oMarkResource;   }
EnumFlags* AM::pMarkTrace(void) { return & gpAttachMap->m_oMarkTrace;         }
i8   AM::iNextEpoch(void)     { return ++ gpAttachMap->m_iEpoch;              }
JobList* AM::pOriginatingJobs(void) { return gpAttachMap->m_pOriginatingJobs; }
JobList* AM::pParticipatingJobs(void) { return gpAttachMap->m_pParticipatingJobs; }
i8   AM::iPid(void)           { return gpAttachMap->m_iPid;                   }
bool AM::bProgress(void)      { return gpAttachMap->m_bProgress;              }
bool AM::bMetadata(void)      { return gpAttachMap->m_bMetadata;              }
i8   AM::iNextDiagram(void)   { return ++ gpAttachMap->m_ixNextDiagram;       }
i8   AM::iNextJobId(void)     { return ++ gpAttachMap->m_ixNextJobId;         }
i8   AM::iNextPlanId(void)    { return ++ gpAttachMap->m_ixNextPlanId;        }
i8*  AM::iaObjectsInPool(void){ return gpAttachMap->m_nObjectsInPool;         }
Queues* AM::pQueues(void)     { return gpAttachMap->m_pQueues;                }
ResourceTracker* AM::pResourceTracker(void) { return & gpAttachMap->m_oResourceTracker; }
Dic* AM::pRootDic(void)       { return & gpAttachMap->m_oRootDic;             }
Statistics* AM::pStatistics(void) { return & gpAttachMap->m_oStatistics;      }
StepTiming* AM::pStepTiming(void)  { return gpAttachMap->m_pStepTiming;       }
PoolBuffer* AM::pTakenBuffers(void) { return gpAttachMap->m_pTakenBuffers;    }
i8   AM::nTakenBytes(void)    { return gpAttachMap->m_nTakenBytes;            }
ForthWord* AM::pTenants(void) { return gpAttachMap->m_pTenants;               }
void AM::vThreadBegins(void)  { ++ gpAttachMap->m_ixThreads;                  }
i8   AM::nThreadsBegun(void)  { return * gpAttachMap->m_ixThreads;            }
bool AM::bTracepoint(void)    { return gpAttachMap->m_bTracepoint;            }
TxSet* AM::pTxSet(void)       { return gpAttachMap->m_pTxSet;                 }
bool AM::bVerbose(void)       { return gpAttachMap->m_bVerbose;               }

//
void AM::vBootError(bool bBootError) {
  gpAttachMap->m_bBootError = bBootError;
}
void AM::vForthWord(ForthWord *pWord) {
  gpAttachMap->m_pForthWord = pWord;
}
void AM::vSubInserters(i8 nSubInserters) {
  gpAttachMap->Arg.m_nSubInserters = nSubInserters;
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
//
bool AM::bAttached(void) {
  if (gpAttachMap)
    return true;
  else
    return false;
}
void AM::vDumpApi(void) {
  pApiState()->vDump();
}
void AM::vWriteLog(void) {
  Log::vWriteCsv(false);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* write to trace file etc                                                    */
/*----------------------------------------------------------------------------*/
void AM::vWriteTrace(Stz* aFormat, va_list xArgs)
{
  static Lock oLock(LK_TRACE);
  static char caFormatted[C_1MB] = {'\0'};
  
  bool bUseLock = (gpAttachMap ? true : false);
  bool bLowMem  = gpBufferPool ? gpBufferPool->bLowMemory() : false;
  
  if (bUseLock)
    oLock.vGrabWrite();
  
  // produce formatted line
  vsnprintf(caFormatted,sizeof(caFormatted) - 3,aFormat->aChar(),xArgs);
  
  // print formatted line
  fprintf(stdout,"%s%s",bLowMem ? "LOW-MEMORY! " : "",caFormatted);
  fflush(stdout);
  
  if (gpTraceFile) {
    gpTraceFile->vPrint("%s%s"_Stz,bLowMem ? "LOW-MEMORY! " : "",caFormatted);
    gpTraceFile->vFlush();
  }
  
  if (bUseLock)
    oLock.vDropWrite();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
