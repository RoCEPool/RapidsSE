/*============================================================================*/
/* StepsSet.hpp - the set of steps                                            */
/*============================================================================*/
#pragma once

  // access path
  static void vAccessPathTableFetch(Tasks::MP_Task mpTask);
  static void vAccessPathTableStore(Tasks::MP_Task mpTask);

  // accum
  static void vAccumDelete(Tasks::MP_Task mpTask);
  static void vAccumNew(Tasks::MP_Task mpTask);

  static void vAccumAccumBuffer(Tasks::MP_Task mpTask);
  static void vAccumCountFetch(Tasks::MP_Task mpTask);
  static void vAccumCountStore(Tasks::MP_Task mpTask);
  static void vAccumGroupFetch(Tasks::MP_Task mpTask);
  static void vAccumGroupStore(Tasks::MP_Task mpTask);
  static void vAccumMaxFetch(Tasks::MP_Task mpTask);
  static void vAccumMaxStore(Tasks::MP_Task mpTask);
  static void vAccumMinFetch(Tasks::MP_Task mpTask);
  static void vAccumMinStore(Tasks::MP_Task mpTask);
  static void vAccumSumFetch(Tasks::MP_Task mpTask);
  static void vAccumSumStore(Tasks::MP_Task mpTask);

  // activity map
  static void vActivityMapDump(Tasks::MP_Task mpTask);
  static void vActivityMapMark(Tasks::MP_Task mpTask);
  static void vActivityMapMarkEntry(Tasks::MP_Task mpTask);
  static void vActivityMapMarkExit(Tasks::MP_Task mpTask);

  // address
  static void vAddressEqual(Tasks::MP_Task mpTask);
  static void vAddressFetch(Tasks::MP_Task mpTask);
  static void vAddressStore(Tasks::MP_Task mpTask);

  // apiState
  static void vApiStateSetPosition(Tasks::MP_Task mpTask);

  // args
  static void vArgsOscillate(Tasks::MP_Task mpTask);
  static void vArgsPrint(Tasks::MP_Task mpTask);

  // assert
  static void vAssert(Tasks::MP_Task mpTask);

  static void vAssertClass(Tasks::MP_Task mpTask);
  static void vAssertClass_0(Tasks::MP_Task mpTask);
  static void vAssertClass_1(Tasks::MP_Task mpTask);
  static void vAssertClass_2(Tasks::MP_Task mpTask);
  static void vAssertClass_3(Tasks::MP_Task mpTask);
  static void vAssertClass_4(Tasks::MP_Task mpTask);

  static void vAssertStack_0(Tasks::MP_Task mpTask);
  static void vAssertStack_0more(Tasks::MP_Task mpTask);
  static void vAssertStack_1(Tasks::MP_Task mpTask);
  static void vAssertStack_1more(Tasks::MP_Task mpTask);
  static void vAssertStack_2(Tasks::MP_Task mpTask);
  static void vAssertStack_2more(Tasks::MP_Task mpTask);
  static void vAssertStack_3(Tasks::MP_Task mpTask);
  static void vAssertStack_3more(Tasks::MP_Task mpTask);
  static void vAssertStack_4(Tasks::MP_Task mpTask);
  static void vAssertStack_4more(Tasks::MP_Task mpTask);

  // astack
  static void vAstackClass(Tasks::MP_Task mpTask);
  static void vAstackDepth(Tasks::MP_Task mpTask);
  static void vAstackDrop(Tasks::MP_Task mpTask);
  static void vAstackDup(Tasks::MP_Task mpTask);
  static void vAstackFetch(Tasks::MP_Task mpTask);
  static void vAstackNip(Tasks::MP_Task mpTask);
  static void vAstackPick(Tasks::MP_Task mpTask);
  static void vAstackReverse(Tasks::MP_Task mpTask);
  static void vAstackStk(Tasks::MP_Task mpTask);
  static void vAstackSwap(Tasks::MP_Task mpTask);
  static void vAstackToA(Tasks::MP_Task mpTask);
  static void vAstackToP(Tasks::MP_Task mpTask);
  static void vAstackToValSet(Tasks::MP_Task mpTask);
  static void vAstackToValues(Tasks::MP_Task mpTask);
  static void vAstackTransfer(Tasks::MP_Task mpTask);
  static void vAstack0(Tasks::MP_Task mpTask);
  static void vAstack1(Tasks::MP_Task mpTask);
  static void vAstack2(Tasks::MP_Task mpTask);
  static void vAstack3(Tasks::MP_Task mpTask);
  static void vAstack4(Tasks::MP_Task mpTask);
  static void vAstack5(Tasks::MP_Task mpTask);
  static void vAstack6(Tasks::MP_Task mpTask);
  static void vAstack7(Tasks::MP_Task mpTask);
  static void vAstack8(Tasks::MP_Task mpTask);
  static void vAstack9(Tasks::MP_Task mpTask);

  // attachmap
  static void vAttachMapFetch(Tasks::MP_Task mpTask);
  static void vAttachMapStore(Tasks::MP_Task mpTask);
  static void vAttachMapTenantsFetch(Tasks::MP_Task mpTask);

  // bp
  static void vBpAnalyze(Tasks::MP_Task mpTask);
  static void vBpDump(Tasks::MP_Task mpTask);
  static void vBpEpochDump(Tasks::MP_Task mpTask);
  static void vBpEpochTake(Tasks::MP_Task mpTask);
  static void vBpReadBufferPool(Tasks::MP_Task mpTask);
  static void vBpWriteBufferPool(Tasks::MP_Task mpTask);
  static void vFreeBuffers(Tasks::MP_Task mpTask);
  static void vFreePercent(Tasks::MP_Task mpTask);
  static void vFreeSpace(Tasks::MP_Task mpTask);
  static void vInUseSpace(Tasks::MP_Task mpTask);
  static void vSnapshot(Tasks::MP_Task mpTask);
  static void vTotalSpace(Tasks::MP_Task mpTask);

  // cast
  static void vCastInt(Tasks::MP_Task mpTask);
  static void vCastPtr(Tasks::MP_Task mpTask);

  // class
  static void vClassName(Tasks::MP_Task mpTask);

  // column
  static void vColumnIsNullableFetch(Tasks::MP_Task mpTask);
  static void vColumnIsPartitionKeyFetch(Tasks::MP_Task mpTask);
  static void vColumnLengthFetch(Tasks::MP_Task mpTask);
  static void vColumnNameFetch(Tasks::MP_Task mpTask);
  static void vColumnScaleFetch(Tasks::MP_Task mpTask);
  static void vColumnTypeFetch(Tasks::MP_Task mpTask);

  // command processor
  static void vCommandProcessorBlankLine(Tasks::MP_Task mpTask);
  static void vCommandProcessorDeleteJob(Tasks::MP_Task mpTask);
  static void vCommandProcessorExecuteFile(Tasks::MP_Task mpTask);
  static void vCommandProcessorFindJobClass(Tasks::MP_Task mpTask);
  static void vCommandProcessorGetCommand(Tasks::MP_Task mpTask);
  static void vCommandProcessorJobClass(Tasks::MP_Task mpTask);
  static void vCommandProcessorPostJob(Tasks::MP_Task mpTask);
  static void vCommandProcessorPreJob(Tasks::MP_Task mpTask);
  static void vCommandProcessorSpawnJob(Tasks::MP_Task mpTask);

  // comment
  static void vComment(Tasks::MP_Task mpTask);

  // common
  static void vCommonFetch(Tasks::MP_Task mpTask);
  static void vCommonStore(Tasks::MP_Task mpTask);
  static void vCommonSlash(Tasks::MP_Task mpTask);
  static void vCommonToCommon(Tasks::MP_Task mpTask);

  static void vOpFrameDrop(Tasks::MP_Task mpTask);
  static void vOpFramePush(Tasks::MP_Task mpTask);


  // cpu
  static void vCpuRdtscp(Tasks::MP_Task mpTask);

  // cursor
  static void vCursorFetch(Tasks::MP_Task mpTask);
  static void vCursorStore(Tasks::MP_Task mpTask);


  // diclock
  static void vDicLockDropWrite(Tasks::MP_Task mpTask);
  static void vDicLockGrabWrite(Tasks::MP_Task mpTask);

  // ddllock
  static void vDdlLockDropRead(Tasks::MP_Task mpTask);
  static void vDdlLockDropWrite(Tasks::MP_Task mpTask);
  static void vDdlLockGrabRead(Tasks::MP_Task mpTask);
  static void vDdlLockGrabWrite(Tasks::MP_Task mpTask);
  static void vDdlLockTestDrop(Tasks::MP_Task mpTask);

  // debug
  static void vDebugTrap(Tasks::MP_Task mpTask);

  // dic
  static void vDicFrozenGet(Tasks::MP_Task mpTask);
  static void vDicFrozenSet(Tasks::MP_Task mpTask);
  static void vDicInit(Tasks::MP_Task mpTask);
  static void vDicKindGet(Tasks::MP_Task mpTask);
  static void vDicKindSet(Tasks::MP_Task mpTask);
  static void vDicLastWord(Tasks::MP_Task mpTask);
  static void vDicMain(Tasks::MP_Task mpTask);
  static void vDicName(Tasks::MP_Task mpTask);
  static void vDicRef(Tasks::MP_Task mpTask);
  static void vDicWordCount(Tasks::MP_Task mpTask);

  // dicPath
  static void vDicPathDepth(Tasks::MP_Task mpTask);
  static void vDicPathDump(Tasks::MP_Task mpTask);
  static void vDicPathPick(Tasks::MP_Task mpTask);

  // dicStack
  static void vDicStackDepth(Tasks::MP_Task mpTask);
  static void vDicStackDrop(Tasks::MP_Task mpTask);
  static void vDicStackDump(Tasks::MP_Task mpTask);
  static void vDicStackPick(Tasks::MP_Task mpTask);
  static void vDicStackPop(Tasks::MP_Task mpTask);
  static void vDicStackPush(Tasks::MP_Task mpTask);

  // dir
  static void vDirChangeCurrent(Tasks::MP_Task mpTask);
  static void vDirGetCurrent(Tasks::MP_Task mpTask);
  static void vDirExists(Tasks::MP_Task mpTask);
  static void vDirMake(Tasks::MP_Task mpTask);
  static void vDirRemove(Tasks::MP_Task mpTask);

  static void vDirClose(Tasks::MP_Task mpTask);
  static void vDirDelete(Tasks::MP_Task mpTask);
  static void vDirEntryName(Tasks::MP_Task mpTask);
  static void vDirEntryType(Tasks::MP_Task mpTask);
  static void vDirIsClosed(Tasks::MP_Task mpTask);
  static void vDirIsOpen(Tasks::MP_Task mpTask);
  static void vDirNew(Tasks::MP_Task mpTask);
  static void vDirOpen(Tasks::MP_Task mpTask);
  static void vDirPathGet(Tasks::MP_Task mpTask);
  static void vDirRead(Tasks::MP_Task mpTask);
  static void vDirRewind(Tasks::MP_Task mpTask);
  static void vDirSeek(Tasks::MP_Task mpTask);
  static void vDirTell(Tasks::MP_Task mpTask);

  // docol
  static void vDocolCompiling(Tasks::MP_Task mpTask);
  static void vDocolInterpreting(Tasks::MP_Task mpTask);
  static void vDocolIsCompiling(Tasks::MP_Task mpTask);
  static void vDocolIsInterpreting(Tasks::MP_Task mpTask);

  // dot
  static void vDot(Tasks::MP_Task mpTask);
  static void vDotPut(Tasks::MP_Task mpTask);
  static void vDotNewline(Tasks::MP_Task mpTask);
  static void vDotVerbose(Tasks::MP_Task mpTask);

  // dstack
  static void vDstackClass(Tasks::MP_Task mpTask);                 //! DUP OF class@
  static void vDstackDepth(Tasks::MP_Task mpTask);
  static void vDstackDrop(Tasks::MP_Task mpTask);
  static void vDstackDropTwo(Tasks::MP_Task mpTask);
  static void vDstackDump(Tasks::MP_Task mpTask);
  static void vDstackDup(Tasks::MP_Task mpTask);
  static void vDstackDupNonZero(Tasks::MP_Task mpTask);
  static void vDstackNip(Tasks::MP_Task mpTask);
  static void vDstackOver(Tasks::MP_Task mpTask);
  static void vDstackPick(Tasks::MP_Task mpTask);
  static void vDstackReverse(Tasks::MP_Task mpTask);
  static void vDstackRoll(Tasks::MP_Task mpTask);
  static void vDstackRollInv(Tasks::MP_Task mpTask);
  static void vDstackRot(Tasks::MP_Task mpTask);
  static void vDstackRotInv(Tasks::MP_Task mpTask);
  static void vDstackStk(Tasks::MP_Task mpTask);
  static void vDstackSwap(Tasks::MP_Task mpTask);
  static void vDstackToValSet(Tasks::MP_Task mpTask);
  static void vDstackToValues(Tasks::MP_Task mpTask);
  static void vDstackTransfer(Tasks::MP_Task mpTask);
  static void vDstackTuck(Tasks::MP_Task mpTask);

  // encode
  static void vEncodeAgain(Tasks::MP_Task mpTask);
  static void vEncodeAhead(Tasks::MP_Task mpTask);
  static void vEncodeCallPrimitive(Tasks::MP_Task mpTask);
  static void vEncodeCallRoutine(Tasks::MP_Task mpTask);
  static void vEncodeContinue(Tasks::MP_Task mpTask);
  static void vEncodeHere(Tasks::MP_Task mpTask);
  static void vEncodeIf(Tasks::MP_Task mpTask);
  static void vEncodeIfAgain(Tasks::MP_Task mpTask);
  static void vEncodeLoop(Tasks::MP_Task mpTask);
  static void vEncodeLoopPlus(Tasks::MP_Task mpTask);
  static void vEncodeNext(Tasks::MP_Task mpTask);
  static void vEncodePushValue(Tasks::MP_Task mpTask);
  static void vEncodeThen(Tasks::MP_Task mpTask);
  static void vEncodeUntil(Tasks::MP_Task mpTask);

  // enter
  static void vEnterObject(Tasks::MP_Task mpTask);
  static void vEnterRoutine(Tasks::MP_Task mpTask);

  // entry
  static void vEntryMatches(Tasks::MP_Task mpTask);
  static void vEntrySame(Tasks::MP_Task mpTask);
  static void vEntryToDataRow(Tasks::MP_Task mpTask);

  // entry buffer
  static void vEntryBufferCreate(Tasks::MP_Task mpTask);
  static void vEntryBufferDelete(Tasks::MP_Task mpTask);
  static void vEntryBufferDump(Tasks::MP_Task mpTask);
  static void vEntryBufferFirstEntry(Tasks::MP_Task mpTask);
  static void vEntryBufferIndexFetch(Tasks::MP_Task mpTask);
  static void vEntryBufferIndexStore(Tasks::MP_Task mpTask);
  static void vEntryBufferIsLastFetch(Tasks::MP_Task mpTask);
  static void vEntryBufferIsLastStore(Tasks::MP_Task mpTask);
  static void vEntryBufferIterate(Tasks::MP_Task mpTask);
  static void vEntryBufferNextBuffer(Tasks::MP_Task mpTask);
  static void vEntryBufferNextEntry(Tasks::MP_Task mpTask);
  static void vEntryBufferPartitionFetch(Tasks::MP_Task mpTask);
  static void vEntryBufferPartitionStore(Tasks::MP_Task mpTask);
  static void vEntryBufferToEntryBuffer(Tasks::MP_Task mpTask);

  // env
  static void vEnvGet(Tasks::MP_Task mpTask);

  // errors
  static void vErrorMsg(Tasks::MP_Task mpTask);

  // exit
  static void vExit(Tasks::MP_Task mpTask);

  // file
  static void vFileClose(Tasks::MP_Task mpTask);
  static void vFileDelete(Tasks::MP_Task mpTask);
  static void vFileNew(Tasks::MP_Task mpTask);
  static void vFileOpen(Tasks::MP_Task mpTask);
  static void vFilePathGet(Tasks::MP_Task mpTask);
  static void vFileRead(Tasks::MP_Task mpTask);
  static void vFileReadRecord(Tasks::MP_Task mpTask);
  static void vFileWrite(Tasks::MP_Task mpTask);

  // flow
  static void vFlowBegin_(Tasks::MP_Task mpTask);
  static void vFlowExecute(Tasks::MP_Task mpTask);
  static void vFlowHazard(Tasks::MP_Task mpTask);
  static void vFlowJump(Tasks::MP_Task mpTask);
  static void vFlowJumpBegin_(Tasks::MP_Task mpTask);
  static void vFlowJumpFalse(Tasks::MP_Task mpTask);
  static void vFlowJumpTrue(Tasks::MP_Task mpTask);
  static void vFlowNop(Tasks::MP_Task mpTask);
  static void vFlowRetryPoint(Tasks::MP_Task mpTask);
  static void vFlowTest(Tasks::MP_Task mpTask);
  static void vFlowThrow(Tasks::MP_Task mpTask);

  // grid
  static void vGridFindNode(Tasks::MP_Task mpTask);
  static void vGridNode(Tasks::MP_Task mpTask);
  static void vGridNodes(Tasks::MP_Task mpTask);
  static void vGridPartitions(Tasks::MP_Task mpTask);
  static void vGridShow(Tasks::MP_Task mpTask);
  static void vGridSlots(Tasks::MP_Task mpTask);
  static void vGridStartedStore(Tasks::MP_Task mpTask);
  static void vGridStartedFetch(Tasks::MP_Task mpTask);
  static void vGridStartNodeStore(Tasks::MP_Task mpTask);
  static void vGridStartNodeFetch(Tasks::MP_Task mpTask);
  static void vGridThisNodeFetch(Tasks::MP_Task mpTask);
  static void vGridThisNodeNameFetch(Tasks::MP_Task mpTask);
  static void vGridToNode(Tasks::MP_Task mpTask);
  static void vGridToPartition(Tasks::MP_Task mpTask);
  static void vGridToSlot(Tasks::MP_Task mpTask);

  // group by
  static void vGroupByApplyBuffer(Tasks::MP_Task mpTask);
  static void vGroupByApplyEntry(Tasks::MP_Task mpTask);
  static void vGroupByApplyPartial(Tasks::MP_Task mpTask);
  static void vGroupByDelete(Tasks::MP_Task mpTask);
  static void vGroupByEnd(Tasks::MP_Task mpTask);
  static void vGroupByNew(Tasks::MP_Task mpTask);
  static void vGroupByStart(Tasks::MP_Task mpTask);

  // handler
  static void vHandlerServiceSleepQueue(Tasks::MP_Task mpTask);

  // index
  static void vIndexAnalyze(Tasks::MP_Task mpTask);
  static void vIndexColumnFetch(Tasks::MP_Task mpTask);
  static void vIndexColumnCountFetch(Tasks::MP_Task mpTask);
  static void vIndexColumnName(Tasks::MP_Task mpTask);
  static void vIndexColumnType(Tasks::MP_Task mpTask);
  static void vIndexCompact(Tasks::MP_Task mpTask);
  static void vIndexCreateColumnVecHidden(Tasks::MP_Task mpTask);
  static void vIndexCreateColumnVec(Tasks::MP_Task mpTask);
  static void vIndexDump(Tasks::MP_Task mpTask);
  static void vIndexExactKey(Tasks::MP_Task mpTask);
  static void vIndexInit(Tasks::MP_Task mpTask);
  static void vIndexIsHiddenFetch(Tasks::MP_Task mpTask);
  static void vIndexIsHiddenStore(Tasks::MP_Task mpTask);
  static void vIndexIsPrimaryKeyFetch(Tasks::MP_Task mpTask);
  static void vIndexIsPrimaryKeyStore(Tasks::MP_Task mpTask);
  static void vIndexIsUniqueFetch(Tasks::MP_Task mpTask);
  static void vIndexIsUniqueStore(Tasks::MP_Task mpTask);
  static void vIndexIsValidUniqueIndex(Tasks::MP_Task mpTask);
  static void vIndexLowerBound(Tasks::MP_Task mpTask);
  static void vIndexNameFetch(Tasks::MP_Task mpTask);
  static void vIndexPopulate(Tasks::MP_Task mpTask);
  static void vIndexPosition(Tasks::MP_Task mpTask);
  static void vIndexPositionFirst(Tasks::MP_Task mpTask);
  static void vIndexRange(Tasks::MP_Task mpTask);
  static void vIndexScan(Tasks::MP_Task mpTask);
  static void vIndexSetSingleColumn(Tasks::MP_Task mpTask);
  static void vIndexShowColumnVec(Tasks::MP_Task mpTask);
  static void vIndexSpace(Tasks::MP_Task mpTask);
  static void vIndexSubDic(Tasks::MP_Task mpTask);
  static void vIndexTableFetch(Tasks::MP_Task mpTask);
  static void vIndexTableStore(Tasks::MP_Task mpTask);
  static void vIndexTruncate(Tasks::MP_Task mpTask);
  static void vIndexUpper(Tasks::MP_Task mpTask);
  static void vIndexUpperBound(Tasks::MP_Task mpTask);
  static void vIndexVerify(Tasks::MP_Task mpTask);
  static void vIndexVisualize(Tasks::MP_Task mpTask);

  // input
  static void vInputClose(Tasks::MP_Task mpTask);
  static void vInputDepth(Tasks::MP_Task mpTask);
  static void vInputNext(Tasks::MP_Task mpTask);
  static void vInputOpen(Tasks::MP_Task mpTask);
  static void vInputPop(Tasks::MP_Task mpTask);
  static void vInputPush(Tasks::MP_Task mpTask);
  static void vInputStdin(Tasks::MP_Task mpTask);
  static void vInputText(Tasks::MP_Task mpTask);
  static void vInputToken(Tasks::MP_Task mpTask);
  static void vInputType(Tasks::MP_Task mpTask);
  static void vInputUnknown(Tasks::MP_Task mpTask);

  // int
  static void vIntAdd(Tasks::MP_Task mpTask);
  static void vIntDivide(Tasks::MP_Task mpTask);
  static void vIntEqual(Tasks::MP_Task mpTask);
  static void vIntGreater(Tasks::MP_Task mpTask);
  static void vIntGreaterEqual(Tasks::MP_Task mpTask);
  static void vIntHash(Tasks::MP_Task mpTask);
  static void vIntHo16(Tasks::MP_Task mpTask);
  static void vIntLess(Tasks::MP_Task mpTask);
  static void vIntLessEqual(Tasks::MP_Task mpTask);
  static void vIntModulo(Tasks::MP_Task mpTask);
  static void vIntMultiply(Tasks::MP_Task mpTask);
  static void vIntNeg(Tasks::MP_Task mpTask);
  static void vIntNotEqual(Tasks::MP_Task mpTask);
  static void vIntSubtract(Tasks::MP_Task mpTask);

  // jobClass
  static void vJobClassBuffer(Tasks::MP_Task mpTask);
  static void vJobClassFind(Tasks::MP_Task mpTask);
  static void vJobClassName(Tasks::MP_Task mpTask);
  static void vJobClassNew(Tasks::MP_Task mpTask);

  static void vJobClassSeedRun(Tasks::MP_Task mpTask);
  static void vJobClassSeedReq(Tasks::MP_Task mpTask);
  static void vJobClassSeedRcv(Tasks::MP_Task mpTask);
  static void vJobClassSeedExe(Tasks::MP_Task mpTask);
  static void vJobClassSeedEnd(Tasks::MP_Task mpTask);
  static void vJobClassSeedCan(Tasks::MP_Task mpTask);

  // job
  static void vJobAcksRemaining(Tasks::MP_Task mpTask);
  static void vJobAddOriginator(Tasks::MP_Task mpTask);
  static void vJobAddParticipant(Tasks::MP_Task mpTask);
  static void vJobClass(Tasks::MP_Task mpTask);
  static void vJobColumnCountFetch(Tasks::MP_Task mpTask);
  static void vJobColumnNameFetch(Tasks::MP_Task mpTask);
  static void vJobColumnIsPartKey(Tasks::MP_Task mpTask);
  static void vJobColumnTypeFetch(Tasks::MP_Task mpTask);
  static void vJobDecAcksRemaining(Tasks::MP_Task mpTask);
  static void vJobDelete(Tasks::MP_Task mpTask);
  static void vJobDicPath(Tasks::MP_Task mpTask);
  static void vJobExecutingTaskFetch(Tasks::MP_Task mpTask);
  static void vJobFindOriginator(Tasks::MP_Task mpTask);
  static void vJobFindParticipant(Tasks::MP_Task mpTask);
  static void vJobHasError(Tasks::MP_Task mpTask);
  static void vJobHasWorkingBuffer(Tasks::MP_Task mpTask);
  static void vJobIdFetch(Tasks::MP_Task mpTask);
  static void vJobIndexNameFetch(Tasks::MP_Task mpTask);
  static void vJobIsExecuting(Tasks::MP_Task mpTask);
  static void vJobErrorStore(Tasks::MP_Task mpTask);
  static void vJobErrorFetch(Tasks::MP_Task mpTask);
  static void vJobExecuteFetch(Tasks::MP_Task mpTask);
  static void vJobJobClass(Tasks::MP_Task mpTask);
  static void vJobMetadataStore(Tasks::MP_Task mpTask);
  static void vJobNewBegun(Tasks::MP_Task mpTask);
  static void vJobNewCancel(Tasks::MP_Task mpTask);
  static void vJobNewCancelled(Tasks::MP_Task mpTask);
  static void vJobNewEnd(Tasks::MP_Task mpTask);
  static void vJobNewEnded(Tasks::MP_Task mpTask);
  static void vJobNewExecute(Tasks::MP_Task mpTask);
  static void vJobNewExecuted(Tasks::MP_Task mpTask);
  static void vJobObjectNameFetch(Tasks::MP_Task mpTask);
  static void vJobOriginatingTaskFetch(Tasks::MP_Task mpTask);
  static void vJobOriginatingTaskStore(Tasks::MP_Task mpTask);
  static void vJobOriginatorFetch(Tasks::MP_Task mpTask);
  static void vJobParticipantFetch(Tasks::MP_Task mpTask);
  static void vJobParticipantStore(Tasks::MP_Task mpTask);
  static void vJobPostRecv(Tasks::MP_Task mpTask);
  static void vJobPreSend(Tasks::MP_Task mpTask);
  static void vJobRecv(Tasks::MP_Task mpTask);
  static void vJobPartitionsFetch(Tasks::MP_Task mpTask);
  static void vJobRegionSizeFetch(Tasks::MP_Task mpTask);
  static void vJobRemoveOriginator(Tasks::MP_Task mpTask);
  static void vJobRemoveParticipant(Tasks::MP_Task mpTask);
  static void vJobResponseStore(Tasks::MP_Task mpTask);
  static void vJobResponseFetch(Tasks::MP_Task mpTask);
  static void vJobRowsInvalidFetch(Tasks::MP_Task mpTask);
  static void vJobRowsInvalidStore(Tasks::MP_Task mpTask);
  static void vJobRowsProcessedFetch(Tasks::MP_Task mpTask);
  static void vJobRowsProcessedStore(Tasks::MP_Task mpTask);
  static void vJobRowsSkippedFetch(Tasks::MP_Task mpTask);
  static void vJobRowsSkippedStore(Tasks::MP_Task mpTask);
  static void vJobSend(Tasks::MP_Task mpTask);
  static void vJobShow(Tasks::MP_Task mpTask);
  static void vJobSignalConnector(Tasks::MP_Task mpTask);
  static void vJobSpawn(Tasks::MP_Task mpTask);
  static void vJobThis(Tasks::MP_Task mpTask);
  static void vJobTraceStore(Tasks::MP_Task mpTask);
  static void vJobTraceFetch(Tasks::MP_Task mpTask);
  static void vJobWaitParticipant(Tasks::MP_Task mpTask);
  static void vJobWorkingBufferFetch(Tasks::MP_Task mpTask);
  static void vJobWorkingBufferStore(Tasks::MP_Task mpTask);

  // job generic
  static void vJob__BegunErrorFetch(Tasks::MP_Task mpTask);
  static void vJob__BegunErrorStore(Tasks::MP_Task mpTask);
  static void vJob__CancelledErrorFetch(Tasks::MP_Task mpTask);
  static void vJob__CancelledErrorStore(Tasks::MP_Task mpTask);
  static void vJob__ExecutedErrorFetch(Tasks::MP_Task mpTask);
  static void vJob__ExecutedErrorStore(Tasks::MP_Task mpTask);
  static void vJob__ExecutedResponseFetch(Tasks::MP_Task mpTask);
  static void vJob__ExecutedResponseStore(Tasks::MP_Task mpTask);
  static void vJob__ExecutedRowsInvalidFetch(Tasks::MP_Task mpTask);
  static void vJob__ExecutedRowsInvalidStore(Tasks::MP_Task mpTask);
  static void vJob__ExecutedRowsProcessedFetch(Tasks::MP_Task mpTask);
  static void vJob__ExecutedRowsProcessedStore(Tasks::MP_Task mpTask);
  static void vJob__ExecutedRowsSkippedFetch(Tasks::MP_Task mpTask);
  static void vJob__ExecutedRowsSkippedStore(Tasks::MP_Task mpTask);

  // job analyzeIndex
  static void vJob_AnalyzeIndexIsDetailed(Tasks::MP_Task mpTask);
  static void vJob_AnalyzeTableIsDetailed(Tasks::MP_Task mpTask);

  // job cat
  static void vJob_CatBegin(Tasks::MP_Task mpTask);
  static void vJob_CatChunk(Tasks::MP_Task mpTask);
  static void vJob_CatEnd(Tasks::MP_Task mpTask);

  // job CreateIndex
  static void vJob_CreateIndexIsUniqueFetch(Tasks::MP_Task mpTask);

  // job FetchRows
  static void vJob_FetchRowsLastBuffersStore(Tasks::MP_Task mpTask);
  static void vJob_FetchRowsPartitionFetch(Tasks::MP_Task mpTask);
  static void vJob_FetchRowsPlannerStore(Tasks::MP_Task mpTask);
  static void vJob_FetchRowsSpawnSub(Tasks::MP_Task mpTask);
  static void vJob_FetchRowsTableStore(Tasks::MP_Task mpTask);
  static void vJob_FetchRowsWhereFetch(Tasks::MP_Task mpTask);

  // job genData
  static void vJob_GenerateGenerate(Tasks::MP_Task mpTask);
  static void vJob_GenerateTableStore(Tasks::MP_Task mpTask);

  // job ingest
  static void vJob_IngestBegin(Tasks::MP_Task mpTask);
  static void vJob_IngestChunk(Tasks::MP_Task mpTask);
  static void vJob_IngestEnd(Tasks::MP_Task mpTask);
  static void vJob_IngestTableStore(Tasks::MP_Task mpTask);

  // job insertRows
  static void vJob_InsertTableInsertRowsJobFetch(Tasks::MP_Task mpTask);
  static void vJob_InsertRowsEnd(Tasks::MP_Task mpTask);

  static void vJob_InsertRowsTableStore(Tasks::MP_Task mpTask);

  static void vJob_InsertRowPartitionFetch(Tasks::MP_Task mpTask);
  static void vJob_InsertRowNewJobWriteRow(Tasks::MP_Task mpTask);
  static void vJob_InsertRowSpawnJobWriteRow(Tasks::MP_Task mpTask);

  // job LoadTable
  static void vJob_LoadTableBegin(Tasks::MP_Task mpTask);
  static void vJob_LoadTableChunk(Tasks::MP_Task mpTask);
  static void vJob_LoadTableDumpMap(Tasks::MP_Task mpTask);
  static void vJob_LoadTableEnd(Tasks::MP_Task mpTask);
  static void vJob_LoadTableFilePathFetch(Tasks::MP_Task mpTask);
  static void vJob_LoadTableTableStore(Tasks::MP_Task mpTask);

  // job NetHog
  static void vJob_NetHogHogNoFetch(Tasks::MP_Task mpTask);

  // job NodeInfo
  static void vJob_NodeInfoNodeNameFetch(Tasks::MP_Task mpTask);

  // job publish
  static void vJob_PublishPublish(Tasks::MP_Task mpTask);
  static void vJob_PublishSpawnSub(Tasks::MP_Task mpTask);
  static void vJob_PublishTableStore(Tasks::MP_Task mpTask);

  // job purge
  static void vJob_PurgeSpawnSub(Tasks::MP_Task mpTask);
  static void vJob_PurgeTableStore(Tasks::MP_Task mpTask);

  // job receiver
  static void vJob_ReceiverConnNoFetch(Tasks::MP_Task mpTask);
  static void vJob_ReceiverConnNoStore(Tasks::MP_Task mpTask);
  static void vJob_ReceiverNodeFetch(Tasks::MP_Task mpTask);
  static void vJob_ReceiverNodeStore(Tasks::MP_Task mpTask);

  // job repeater
  static void vJob_RepeaterBegin(Tasks::MP_Task mpTask);
  static void vJob_RepeaterChunk(Tasks::MP_Task mpTask);
  static void vJob_RepeaterEnd(Tasks::MP_Task mpTask);

  // job SelectTable
  static void vJob_SelectTablePartitionFetch(Tasks::MP_Task mpTask);
  static void vJob_SelectTableTableStore(Tasks::MP_Task mpTask);

  // job sink
  static void vJob_SinkBegin(Tasks::MP_Task mpTask);
  static void vJob_SinkChunk(Tasks::MP_Task mpTask);
  static void vJob_SinkEnd(Tasks::MP_Task mpTask);

  // Job subFetch
  static void vJob_SubFetchBufferQueueFetch(Tasks::MP_Task mpTask);
  static void vJob_SubFetchPhraseFetch(Tasks::MP_Task mpTask);
  static void vJob_SubFetchPlannerFetch(Tasks::MP_Task mpTask);
  static void vJob_SubFetchSlotFetch(Tasks::MP_Task mpTask);

  // Job subLoader
  static void vJob_SubLoaderBegin(Tasks::MP_Task mpTask);
  static void vJob_SubLoaderChunk(Tasks::MP_Task mpTask);
  static void vJob_SubLoaderEnd(Tasks::MP_Task mpTask);
  static void vJob_SubLoaderSpawn(Tasks::MP_Task mpTask);

  // job subPublish
  static void vJob_SubPublishSlotFetch(Tasks::MP_Task mpTask);
  static void vJob_SubPublishPublishJobIdFetch(Tasks::MP_Task mpTask);

  // job subPurger
  static void vJob_SubPurgerBegin(Tasks::MP_Task mpTask);
  static void vJob_SubPurgerChunk(Tasks::MP_Task mpTask);
  static void vJob_SubPurgerEnd(Tasks::MP_Task mpTask);

  static void vJob_SubPurgerSlotFetch(Tasks::MP_Task mpTask);
  static void vJob_SubPurgerPurgeJobIdFetch(Tasks::MP_Task mpTask);

  // Job subTruncate
  static void vJob_SubTruncateSlotFetch(Tasks::MP_Task mpTask);
  static void vJob_SubTruncateTruncateJobIdFetch(Tasks::MP_Task mpTask);

  // Job truncate
  static void vJob_TruncateTableSpawnSub(Tasks::MP_Task mpTask);

  // Job WriteBuffer
  static void vJob_WriteBufferParentJobIdFetch(Tasks::MP_Task mpTask);
  static void vJob_WriteBufferInsertRows(Tasks::MP_Task mpTask);
  static void vJob_WriteBufferIsIngesting(Tasks::MP_Task mpTask);
  static void vJob_WriteBufferIsInserting(Tasks::MP_Task mpTask);
  static void vJob_WriteBufferIsLoading(Tasks::MP_Task mpTask);
  static void vJob_WriteBufferPartitionFetch(Tasks::MP_Task mpTask);
  static void vJob_WriteBufferTableStore(Tasks::MP_Task mpTask);

  // Job WriteRow
  static void vJob_WriteRowParentJobIdFetch(Tasks::MP_Task mpTask);
  static void vJob_WriteRowHashRow(Tasks::MP_Task mpTask);
  static void vJob_WriteRowInsertRow(Tasks::MP_Task mpTask);
  static void vJob_WriteRowPartitionFetch(Tasks::MP_Task mpTask);
  static void vJob_WriteRowTableStore(Tasks::MP_Task mpTask);

  // load table
  static void vLoadTableChunkFile(Tasks::MP_Task mpTask);
  static void vLoadTableDelete(Tasks::MP_Task mpTask);
  static void vLoadTableDumpMap(Tasks::MP_Task mpTask);
  static void vLoadTableNew(Tasks::MP_Task mpTask);
  static void vLoadTableTableStore(Tasks::MP_Task mpTask);

  // locals
  static void vLocalFetch(Tasks::MP_Task mpTask);
  static void vLocalStore(Tasks::MP_Task mpTask);
  static void vLocalsBar(Tasks::MP_Task mpTask);
  static void vLocalsPop(Tasks::MP_Task mpTask);
  static void vLocalsPush(Tasks::MP_Task mpTask);
  static void vLocalsTo(Tasks::MP_Task mpTask);

  // lock
  static void vLockDropRead(Tasks::MP_Task mpTask);
  static void vLockDropWrite(Tasks::MP_Task mpTask);
  static void vLockGrabRead(Tasks::MP_Task mpTask);
  static void vLockGrabWrite(Tasks::MP_Task mpTask);
  static void vLockInit(Tasks::MP_Task mpTask);

  // locks
  static void vLockTrackerList(Tasks::MP_Task mpTask);
  static void vLockTrackerReset(Tasks::MP_Task mpTask);

  // logic
  static void vLogicAnd(Tasks::MP_Task mpTask);
  static void vLogicNot(Tasks::MP_Task mpTask);
  static void vLogicOr(Tasks::MP_Task mpTask);

  // magnitude
  static void vBytesMagnitude(Tasks::MP_Task mpTask);
  static void vNumberMagnitude(Tasks::MP_Task mpTask);
  static void vToDigitName(Tasks::MP_Task mpTask);

  // memory
  static void vMemoryByteDump(Tasks::MP_Task mpTask);
  static void vMemoryList(Tasks::MP_Task mpTask);
  static void vMemoryReset(Tasks::MP_Task mpTask);
  static void vMemoryWordDump(Tasks::MP_Task mpTask);

  // node (of grid)
  static void vNodeInit(Tasks::MP_Task mpTask);
  static void vNodeIP(Tasks::MP_Task mpTask);
  static void vNodeName(Tasks::MP_Task mpTask);
  static void vNodeNew(Tasks::MP_Task mpTask);
  static void vNodeNo(Tasks::MP_Task mpTask);
  static void vNodePartitions(Tasks::MP_Task mpTask);
  static void vNodePort(Tasks::MP_Task mpTask);
  static void vNodeRecvSocketStore(Tasks::MP_Task mpTask);
  static void vNodeRecvSocketFetch(Tasks::MP_Task mpTask);
  static void vNodeSendSocketStore(Tasks::MP_Task mpTask);
  static void vNodeSendSocketFetch(Tasks::MP_Task mpTask);
  static void vNodeSize(Tasks::MP_Task mpTask);
  static void vNodeStatus(Tasks::MP_Task mpTask);

  // object
  static void vObjectClassFetch(Tasks::MP_Task mpTask);
  static void vObjectClassStore(Tasks::MP_Task mpTask);
  static void vObjectDump(Tasks::MP_Task mpTask);
  static void vObjectFetch(Tasks::MP_Task mpTask);
  static void vObjectMagicMark(Tasks::MP_Task mpTask);
  static void vObjectStore(Tasks::MP_Task mpTask);
  static void vObjectWordBodySet(Tasks::MP_Task mpTask);

  // opFrame
  static void vOpFrameDic(Tasks::MP_Task mpTask);
  static void vOpFrameName(Tasks::MP_Task mpTask);


  // persistence
  static void vPersistenceCheckpoint(Tasks::MP_Task mpTask);
  static void vPersistenceReload(Tasks::MP_Task mpTask);

  // plan
  static void vPlanCompile(Tasks::MP_Task mpTask);
  static void vPlanDelete(Tasks::MP_Task mpTask);
  static void vPlanDic(Tasks::MP_Task mpTask);
  static void vPlanExecutingTaskFetch(Tasks::MP_Task mpTask);
  static void vPlanExecutingTaskStore(Tasks::MP_Task mpTask);
  static void vPlanFinished(Tasks::MP_Task mpTask);
  static void vPlanIdFetch(Tasks::MP_Task mpTask);
  static void vPlanLambdaAppend(Tasks::MP_Task mpTask);
  static void vPlanLambdaFetch(Tasks::MP_Task mpTask);
  static void vPlanLambdaStore(Tasks::MP_Task mpTask);
  static void vPlanNew(Tasks::MP_Task mpTask);
  static void vPlanNameFetch(Tasks::MP_Task mpTask);
  static void vPlanNameStore(Tasks::MP_Task mpTask);
  static void vPlanOpFrameAdd(Tasks::MP_Task mpTask);
  static void vPlanOpFrameFind(Tasks::MP_Task mpTask);
  static void vPlanOpFrameTop(Tasks::MP_Task mpTask);
  static void vPlanShow(Tasks::MP_Task mpTask);

  // plan list
  static void vPlanListAddPlan(Tasks::MP_Task mpTask);
  static void vPlanListFindPlan(Tasks::MP_Task mpTask);
  static void vPlanListRemovePlan(Tasks::MP_Task mpTask);
  static void vPlanListShow(Tasks::MP_Task mpTask);

  // planner
  static void vPlannerChoosePaths(Tasks::MP_Task mpTask);
  static void vPlannerChosenPath(Tasks::MP_Task mpTask);
  static void vPlannerDelete(Tasks::MP_Task mpTask);
  static void vPlannerFetchTechniqueFetch(Tasks::MP_Task mpTask);
  static void vPlannerGenPaths(Tasks::MP_Task mpTask);
  static void vPlannerHighKeyFetch(Tasks::MP_Task mpTask);
  static void vPlannerIndexNmFetch(Tasks::MP_Task mpTask);
  static void vPlannerKeyFetch(Tasks::MP_Task mpTask);
  static void vPlannerLowKeyFetch(Tasks::MP_Task mpTask);
  static void vPlannerMatchPaths(Tasks::MP_Task mpTask);
  static void vPlannerNew(Tasks::MP_Task mpTask);
  static void vPlannerParseWhere(Tasks::MP_Task mpTask);
  static void vPlannerPhrasesFetch(Tasks::MP_Task mpTask);
  static void vPlannerScorePaths(Tasks::MP_Task mpTask);
  static void vPlannerShowPaths(Tasks::MP_Task mpTask);
  static void vPlannerTableFetch(Tasks::MP_Task mpTask);
  static void vPlannerTableStore(Tasks::MP_Task mpTask);

  // process
  static void vProcessGetPid(Tasks::MP_Task mpTask);
  static void vProcessQuit(Tasks::MP_Task mpTask);
  static void vProcessTestPid(Tasks::MP_Task mpTask);

  // prompt
  static void vPromptContinue(Tasks::MP_Task mpTask);
  static void vPromptExecuteFunction(Tasks::MP_Task mpTask);
  static void vPromptGetCommand(Tasks::MP_Task mpTask);
  static void vPromptNew(Tasks::MP_Task mpTask);
  static void vPromptSession(Tasks::MP_Task mpTask);

  // push
  static void vPushI(Tasks::MP_Task mpTask);
  static void vPushJ(Tasks::MP_Task mpTask);
  static void vPushNullPtr(Tasks::MP_Task mpTask);
  static void vPushValue(Tasks::MP_Task mpTask);

  // queue
  static void vQueueAppend(Tasks::MP_Task mpTask);
  static void vQueueDelete(Tasks::MP_Task mpTask);
  static void vQueueDump(Tasks::MP_Task mpTask);
  static void vQueueInit(Tasks::MP_Task mpTask);
  static void vQueueNew(Tasks::MP_Task mpTask);
  static void vQueuePop(Tasks::MP_Task mpTask);

  // queues
  static void vQueuesExecutingLength(Tasks::MP_Task mpTask);
  static void vQueuesReadyLength(Tasks::MP_Task mpTask);
  static void vQueuesReadySlotLength(Tasks::MP_Task mpTask);
  static void vQueuesSleepingLength(Tasks::MP_Task mpTask);
  static void vQueuesWaitingLength(Tasks::MP_Task mpTask);

  // rbCursor
  static void vRbCursorBegin(Tasks::MP_Task mpTask);
  static void vRbCursorDelete(Tasks::MP_Task mpTask);
  static void vRbCursorEnd(Tasks::MP_Task mpTask);
  static void vRbCursorFirst(Tasks::MP_Task mpTask);
  static void vRbCursorGet(Tasks::MP_Task mpTask);
  static void vRbCursorInit(Tasks::MP_Task mpTask);
  static void vRbCursorNew(Tasks::MP_Task mpTask);
  static void vRbCursorNext(Tasks::MP_Task mpTask);
  static void vRbCursorRowBuffer(Tasks::MP_Task mpTask);
  static void vRbCursorTableFetch(Tasks::MP_Task mpTask);
  static void vRbCursorTableStore(Tasks::MP_Task mpTask);

  // row
  static void vRowString(Tasks::MP_Task mpTask);
  static void vRowEscapedString(Tasks::MP_Task mpTask);

  // rowBuffer
  static void vRowBufferDump(Tasks::MP_Task mpTask);
  static void vRowBufferNew(Tasks::MP_Task mpTask);

  // rowCursor
  static void vRowCursorBegin(Tasks::MP_Task mpTask);
  static void vRowCursorDelete(Tasks::MP_Task mpTask);
  static void vRowCursorEnd(Tasks::MP_Task mpTask);
  static void vRowCursorEntryBufferFetch(Tasks::MP_Task mpTask);
  static void vRowCursorEntryBufferStore(Tasks::MP_Task mpTask);
  static void vRowCursorFirst(Tasks::MP_Task mpTask);
  static void vRowCursorInit(Tasks::MP_Task mpTask);
  static void vRowCursorNew(Tasks::MP_Task mpTask);
  static void vRowCursorNext(Tasks::MP_Task mpTask);
  static void vRowCursorRow(Tasks::MP_Task mpTask);
  static void vRowCursorTableFetch(Tasks::MP_Task mpTask);
  static void vRowCursorTableStore(Tasks::MP_Task mpTask);

  // rstack
  static void vRstackCsPick(Tasks::MP_Task mpTask);
  static void vRstackCsRoll(Tasks::MP_Task mpTask);
  static void vRstackDepth(Tasks::MP_Task mpTask);
  static void vRstackDrop(Tasks::MP_Task mpTask);
  static void vRstackFetch(Tasks::MP_Task mpTask);
  static void vRstackFetchTwo(Tasks::MP_Task mpTask);
  static void vRstackPick(Tasks::MP_Task mpTask);
  static void vRstackReverse(Tasks::MP_Task mpTask);
  static void vRstackRoll(Tasks::MP_Task mpTask);
  static void vRstackRollInv(Tasks::MP_Task mpTask);
  static void vRstackStk(Tasks::MP_Task mpTask);
  static void vRstackSwap(Tasks::MP_Task mpTask);
  static void vRstackToP(Tasks::MP_Task mpTask);
  static void vRstackToR(Tasks::MP_Task mpTask);
  static void vRstackToRTwo(Tasks::MP_Task mpTask);
  static void vRstack0(Tasks::MP_Task mpTask);
  static void vRstack1(Tasks::MP_Task mpTask);
  static void vRstack2(Tasks::MP_Task mpTask);
  static void vRstack3(Tasks::MP_Task mpTask);
  static void vRstack4(Tasks::MP_Task mpTask);
  static void vRstack5(Tasks::MP_Task mpTask);
  static void vRstack6(Tasks::MP_Task mpTask);
  static void vRstack7(Tasks::MP_Task mpTask);
  static void vRstack8(Tasks::MP_Task mpTask);
  static void vRstack9(Tasks::MP_Task mpTask);

  // session
  static void vSessionCommandProcessor(Tasks::MP_Task mpTask);
  static void vSessionDicStack(Tasks::MP_Task mpTask);
  static void vSessionDump(Tasks::MP_Task mpTask);
  static void vSessionIsExiting(Tasks::MP_Task mpTask);
  static void vSessionPid(Tasks::MP_Task mpTask);
  static void vSessionSlotFetch(Tasks::MP_Task mpTask);
  static void vSessionTxFetch(Tasks::MP_Task mpTask);
  static void vSessionTxStore(Tasks::MP_Task mpTask);

  // selCursor
  static void vSelCursorBegin(Tasks::MP_Task mpTask);
  static void vSelCursorDelete(Tasks::MP_Task mpTask);
  static void vSelCursorEnd(Tasks::MP_Task mpTask);
  static void vSelCursorFirst(Tasks::MP_Task mpTask);
  static void vSelCursorGet(Tasks::MP_Task mpTask);
  static void vSelCursorHasRow(Tasks::MP_Task mpTask);
  static void vSelCursorInit(Tasks::MP_Task mpTask);
  static void vSelCursorNew(Tasks::MP_Task mpTask);
  static void vSelCursorNext(Tasks::MP_Task mpTask);
  static void vSelCursorRowBuffer(Tasks::MP_Task mpTask);
  static void vSelCursorTableFetch(Tasks::MP_Task mpTask);
  static void vSelCursorTableStore(Tasks::MP_Task mpTask);

  // semaphore
  static void vSemaphoreInit(Tasks::MP_Task mpTask);

  // shell
  static void vShellExecute(Tasks::MP_Task mpTask);

  // slots
  static void vSlotsColumnCount(Tasks::MP_Task mpTask);
  static void vSlotsColumnName(Tasks::MP_Task mpTask);
  static void vSlotsCreateColumnVec(Tasks::MP_Task mpTask);
  static void vSlotsSubDic(Tasks::MP_Task mpTask);

  // socket
  static void vSocketAccept(Tasks::MP_Task mpTask);
  static void vSocketClose(Tasks::MP_Task mpTask);
  static void vSocketConnect(Tasks::MP_Task mpTask);
  static void vSocketDropWrite(Tasks::MP_Task mpTask);
  static void vSocketGrabWrite(Tasks::MP_Task mpTask);
  static void vSocketListen(Tasks::MP_Task mpTask);
  static void vSocketOpen(Tasks::MP_Task mpTask);
  static void vSocketRecvObject(Tasks::MP_Task mpTask);
  static void vSocketRecvObject_LowMem(Tasks::MP_Task mpTask);
  static void vSocketSendObject(Tasks::MP_Task mpTask);

  // steps
  static void vStepTimingReset(Tasks::MP_Task mpTask);
  static void vStepTimingStart(Tasks::MP_Task mpTask);
  static void vStepTimingWrite(Tasks::MP_Task mpTask);

  // string
  static void vStrAlgebra(Tasks::MP_Task mpTask);
  static void vStrCat(Tasks::MP_Task mpTask);
  static void vStrClip(Tasks::MP_Task mpTask);
  static void vStrEqual(Tasks::MP_Task mpTask);
  static void vStrEscape(Tasks::MP_Task mpTask);
  static void vStrFirstCsv(Tasks::MP_Task mpTask);
  static void vStrFormat(Tasks::MP_Task mpTask);
  static void vStrLeft(Tasks::MP_Task mpTask);
  static void vStrLength(Tasks::MP_Task mpTask);
  static void vStrPad(Tasks::MP_Task mpTask);
  static void vStrPrepend(Tasks::MP_Task mpTask);
  static void vStrRandom(Tasks::MP_Task mpTask);
  static void vStrReplace(Tasks::MP_Task mpTask);
  static void vStrRight(Tasks::MP_Task mpTask);
  static void vStrSub(Tasks::MP_Task mpTask);
  static void vStrTailInt(Tasks::MP_Task mpTask);

  static void vS256Init(Tasks::MP_Task mpTask);

  // sub loader
  static void vSubLoaderDelete(Tasks::MP_Task mpTask);

  // table
  static void vTableAddIndex(Tasks::MP_Task mpTask);
  static void vTableAnalyze(Tasks::MP_Task mpTask);
  static void vTableColumnCountFetch(Tasks::MP_Task mpTask);
  static void vTableCreateColumnVec(Tasks::MP_Task mpTask);
  static void vTableDrop(Tasks::MP_Task mpTask);
  static void vTableDump(Tasks::MP_Task mpTask);
  static void vTableFirstBuffer(Tasks::MP_Task mpTask);
  static void vTableGenData(Tasks::MP_Task mpTask);
  static void vTableGetColumnsAsString(Tasks::MP_Task mpTask);
  static void vTableColumnFetch(Tasks::MP_Task mpTask);
  static void vTableGetPtn(Tasks::MP_Task mpTask);
  static void vTableHiddenIndexFetch(Tasks::MP_Task mpTask);
  static void vTableHiddenIndexStore(Tasks::MP_Task mpTask);
  static void vTableIndexCount(Tasks::MP_Task mpTask);
  static void vTableInit(Tasks::MP_Task mpTask);
  static void vTableInsert(Tasks::MP_Task mpTask);
  static void vTableInsertEntries(Tasks::MP_Task mpTask);
  static void vTableIsLoadingStore(Tasks::MP_Task mpTask);
  static void vTableLazyBuildIndexes(Tasks::MP_Task mpTask);
  static void vTableList(Tasks::MP_Task mpTask);
  static void vTableName(Tasks::MP_Task mpTask);
  static void vTablePublish(Tasks::MP_Task mpTask);
  static void vTablePurge(Tasks::MP_Task mpTask);
  static void vTableRemoveIndex(Tasks::MP_Task mpTask);
  static void vTableShowColumnVec(Tasks::MP_Task mpTask);
  static void vTableSlots(Tasks::MP_Task mpTask);
  static void vTableSubDic(Tasks::MP_Task mpTask);
  static void vTableSubLoaders(Tasks::MP_Task mpTask);
  static void vTableTruncate(Tasks::MP_Task mpTask);
  static void vTableIsValidIndexColumns(Tasks::MP_Task mpTask);

  // table partition
  static void vTablePtnAttemptGrabRead(Tasks::MP_Task mpTask);
  static void vTablePtnAttemptGrabWrite(Tasks::MP_Task mpTask);
  static void vTablePtnDropRead(Tasks::MP_Task mpTask);
  static void vTablePtnDropWrite(Tasks::MP_Task mpTask);
  static void vTablePtnDump(Tasks::MP_Task mpTask);
  static void vTablePtnIsLocked(Tasks::MP_Task mpTask);

  // task
  static void vTaskAck(Tasks::MP_Task mpTask);
  static void vTaskAcksStore(Tasks::MP_Task mpTask);
  static void vTaskBreakPoint(Tasks::MP_Task mpTask);
  static void vTaskDestructorTraceFetch(Tasks::MP_Task mpTask);
  static void vTaskDestructorTraceStore(Tasks::MP_Task mpTask);
  static void vTaskDicStack(Tasks::MP_Task mpTask);
  static void vTaskDropLock(Tasks::MP_Task mpTask);
  static void vTaskDump(Tasks::MP_Task mpTask);
  static void vTaskGrabLock(Tasks::MP_Task mpTask);
  static void vTaskIsAckSet(Tasks::MP_Task mpTask);
  static void vTaskJobFetch(Tasks::MP_Task mpTask);
  static void vTaskJobStore(Tasks::MP_Task mpTask);
  static void vTaskNameFetch(Tasks::MP_Task mpTask);
  static void vTaskThis(Tasks::MP_Task mpTask);
  static void vTaskPartitionFetch(Tasks::MP_Task mpTask);
  static void vTaskPartitionStore(Tasks::MP_Task mpTask);
  static void vTaskPlanFetch(Tasks::MP_Task mpTask);
  static void vTaskPlanStore(Tasks::MP_Task mpTask);
  static void vTaskSeed(Tasks::MP_Task mpTask);
  static void vTaskSessionFetch(Tasks::MP_Task mpTask);
  static void vTaskSessionStore(Tasks::MP_Task mpTask);
  static void vTaskSend(Tasks::MP_Task mpTask);
  static void vTaskSendNodeFetch(Tasks::MP_Task mpTask);
  static void vTaskSendNodeStore(Tasks::MP_Task mpTask);
  static void vTaskSleepUs(Tasks::MP_Task mpTask);
  static void vTaskStateFetch(Tasks::MP_Task mpTask);
  static void vTaskStateStore(Tasks::MP_Task mpTask);
  static void vTaskThread(Tasks::MP_Task mpTask);
  static void vTaskWait(Tasks::MP_Task mpTask);
  static void vTaskYield(Tasks::MP_Task mpTask);

  // test
  static void vTestTest0(Tasks::MP_Task mpTask);
  static void vTestTest1(Tasks::MP_Task mpTask);
  static void vTestTest2(Tasks::MP_Task mpTask);
  static void vTestTest3(Tasks::MP_Task mpTask);
  static void vTestTest4(Tasks::MP_Task mpTask);
  static void vTestTest5(Tasks::MP_Task mpTask);
  static void vTestTest6(Tasks::MP_Task mpTask);
  static void vTestTest7(Tasks::MP_Task mpTask);
  static void vTestTest8(Tasks::MP_Task mpTask);
  static void vTestTest9(Tasks::MP_Task mpTask);

  // thread
  static void vThreadBind(Tasks::MP_Task mpTask);
  static void vThreadCondSignal(Tasks::MP_Task mpTask);
  static void vThreadCondWait(Tasks::MP_Task mpTask);
  static void vThreadCpu(Tasks::MP_Task mpTask);
  static void vThreadDedicatedFetch(Tasks::MP_Task mpTask);
  static void vThreadDedicatedStore(Tasks::MP_Task mpTask);
  static void vThreadDisableRealtime(Tasks::MP_Task mpTask);
  static void vThreadDump(Tasks::MP_Task mpTask);
  static void vThreadEnableRealtime(Tasks::MP_Task mpTask);
  static void vThreadId(Tasks::MP_Task mpTask);
  static void vThreadoLockMutexLock(Tasks::MP_Task mpTask);
  static void vThreadMutexUnlock(Tasks::MP_Task mpTask);
  static void vThreadSleep_s(Tasks::MP_Task mpTask);
  static void vThreadSleep_ms(Tasks::MP_Task mpTask);
  static void vThreadSleep_us(Tasks::MP_Task mpTask);
  static void vThreadSleep_ns(Tasks::MP_Task mpTask);

  // timer
  static void vTimerDelete(Tasks::MP_Task mpTask);
  static void vTimerInit(Tasks::MP_Task mpTask);
  static void vTimerMicrosecs(Tasks::MP_Task mpTask);
  static void vTimerMillisecs(Tasks::MP_Task mpTask);
  static void vTimerNanosecs(Tasks::MP_Task mpTask);
  static void vTimerNew(Tasks::MP_Task mpTask);
  static void vTimerReset(Tasks::MP_Task mpTask);
  static void vTimerSecs(Tasks::MP_Task mpTask);
  static void vTimerStart(Tasks::MP_Task mpTask);
  static void vTimerTicks(Tasks::MP_Task mpTask);
  static void vTimerUpdate(Tasks::MP_Task mpTask);

  // to
  static void vToBinVal(Tasks::MP_Task mpTask);
  static void vToBody(Tasks::MP_Task mpTask);
  static void vToInt(Tasks::MP_Task mpTask);
  static void vToObject(Tasks::MP_Task mpTask);
  static void vToPtr(Tasks::MP_Task mpTask);
  static void vToStr(Tasks::MP_Task mpTask);
  static void vToTask(Tasks::MP_Task mpTask);

  // token
  static void vTokenColon(Tasks::MP_Task mpTask);

  // trace
  static void vTraceOff(Tasks::MP_Task mpTask);
  static void vTraceOn(Tasks::MP_Task mpTask);
  static void vTracePoint(Tasks::MP_Task mpTask);
  static void vTraceSet(Tasks::MP_Task mpTask);

  static void vPerformancePoint(Tasks::MP_Task mpTask);  //! wrong place
  static void vPerformancePrint(Tasks::MP_Task mpTask);  //! wrong place

  // tx
  static void vTxBegin(Tasks::MP_Task mpTask);
  static void vTxCommit(Tasks::MP_Task mpTask);
  static void vTxCounterDec(Tasks::MP_Task mpTask);
  static void vTxCounterFetch(Tasks::MP_Task mpTask);
  static void vTxCounterStore(Tasks::MP_Task mpTask);
  static void vTxDistMemberFetch(Tasks::MP_Task mpTask);
  static void vTxDistMemberStore(Tasks::MP_Task mpTask);
  static void vTxDump(Tasks::MP_Task mpTask);
  static void vTxDumpAll(Tasks::MP_Task mpTask);
  static void vTxFree(Tasks::MP_Task mpTask);
  static void vTxId(Tasks::MP_Task mpTask);
  static void vTxPrepare(Tasks::MP_Task mpTask);
  static void vTxRollback(Tasks::MP_Task mpTask);
  static void vTxTestDist(Tasks::MP_Task mpTask);

  // txCollection
  static void vTxCollectionDump(Tasks::MP_Task mpTask);
  static void vTxCollectionInit(Tasks::MP_Task mpTask);

  // txCursor
  static void vTxCursorBegin(Tasks::MP_Task mpTask);
  static void vTxCursorDelete(Tasks::MP_Task mpTask);
  static void vTxCursorEnd(Tasks::MP_Task mpTask);
  static void vTxCursorFirst(Tasks::MP_Task mpTask);
  static void vTxCursorGet(Tasks::MP_Task mpTask);
  static void vTxCursorInit(Tasks::MP_Task mpTask);
  static void vTxCursorNew(Tasks::MP_Task mpTask);
  static void vTxCursorNext(Tasks::MP_Task mpTask);

  // txMap
  static void vTxMapInit(Tasks::MP_Task mpTask);

  // txRef
  static void vTxRefBegin(Tasks::MP_Task mpTask);
  static void vTxRefCheckDistId(Tasks::MP_Task mpTask);
  static void vTxRefCommit(Tasks::MP_Task mpTask);
  static void vTxRefDump(Tasks::MP_Task mpTask);
  static void vTxRefEnd(Tasks::MP_Task mpTask);
  static void vTxRefId(Tasks::MP_Task mpTask);
  static void vTxRefInit(Tasks::MP_Task mpTask);
  static void vTxRefNew(Tasks::MP_Task mpTask);
  static void vTxRefRollback(Tasks::MP_Task mpTask);

  // utility
  static void vUtilityPrefetch(Tasks::MP_Task mpTask);
  static void vUtilitySchemaTest(Tasks::MP_Task mpTask);

  // vector
  static void vVectorDifference(Tasks::MP_Task mpTask);

  // word
  static void vAllot(Tasks::MP_Task mpTask);
  static void vCell(Tasks::MP_Task mpTask);
  static void vColon(Tasks::MP_Task mpTask);
  static void vComma(Tasks::MP_Task mpTask);
  static void vCompileComma(Tasks::MP_Task mpTask);
  static void vCreate(Tasks::MP_Task mpTask);
  static void vCreateFrom(Tasks::MP_Task mpTask);
  static void vCreateFromInMain(Tasks::MP_Task mpTask);
  static void vCreateFromPersistent(Tasks::MP_Task mpTask);
  static void vCreatePersistent(Tasks::MP_Task mpTask);
  static void vCreateTenants(Tasks::MP_Task mpTask);
  static void vDicWhich(Tasks::MP_Task mpTask);
  static void vDoes(Tasks::MP_Task mpTask);
  static void vDoes_RT(Tasks::MP_Task mpTask);
  static void vFind(Tasks::MP_Task mpTask);
  static void vFindInCurrent(Tasks::MP_Task mpTask);
  static void vForget(Tasks::MP_Task mpTask);
  static void vHashHashColon(Tasks::MP_Task mpTask);
  static void vImmediate(Tasks::MP_Task mpTask);
  static void vLocate(Tasks::MP_Task mpTask);
  static void vOpDic(Tasks::MP_Task mpTask);
  static void vPlanDicTask(Tasks::MP_Task mpTask);
  static void vPostpone(Tasks::MP_Task mpTask);
  static void vSee(Tasks::MP_Task mpTask);
  static void vSemicolon(Tasks::MP_Task mpTask);
  static void vSubDic(Tasks::MP_Task mpTask);
  static void vTick(Tasks::MP_Task mpTask);
  static void vWord(Tasks::MP_Task mpTask);
  static void vWordDump(Tasks::MP_Task mpTask);
  static void vWordForget(Tasks::MP_Task mpTask);
  static void vWordImmediateGet(Tasks::MP_Task mpTask);
  static void vWordImmediateSet(Tasks::MP_Task mpTask);
  static void vWordIsImmediate(Tasks::MP_Task mpTask);
  static void vWordIsPrimitive(Tasks::MP_Task mpTask);
  static void vWordKindGet(Tasks::MP_Task mpTask);
  static void vWordKindSet(Tasks::MP_Task mpTask);
  static void vWordName(Tasks::MP_Task mpTask);
  static void vWordPrev(Tasks::MP_Task mpTask);
  static void vWordThis(Tasks::MP_Task mpTask);
  static void vWordTypeGet(Tasks::MP_Task mpTask);
  static void vWordTypeSet(Tasks::MP_Task mpTask);

  // working buffer
  static void vWorkingBufferIsLastFetch(Tasks::MP_Task mpTask);
  static void vWorkingBufferIsLastStore(Tasks::MP_Task mpTask);
  static void vWorkingBufferRows(Tasks::MP_Task mpTask);
  static void vWorkingBufferShow(Tasks::MP_Task mpTask);
  static void vWorkingBufferToWorkingBuffer(Tasks::MP_Task mpTask);

  // zero
  static void vZeroLess(Tasks::MP_Task mpTask);
  static void vZeroLessEqual(Tasks::MP_Task mpTask);
  static void vZeroEqual(Tasks::MP_Task mpTask);
  static void vZeroNotEqual(Tasks::MP_Task mpTask);
  static void vZeroGreaterEqual(Tasks::MP_Task mpTask);
  static void vZeroGreater(Tasks::MP_Task mpTask);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
