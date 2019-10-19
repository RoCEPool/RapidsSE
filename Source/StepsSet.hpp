/*============================================================================*/
/* StepsSet.hpp - the set of steps                                            */
/*============================================================================*/
#pragma once

public:

  // access path
  static void vAccessPathTableFetch(Forth* pForth);
  static void vAccessPathTableStore(Forth* pForth);

  // accum
  static void vAccumDelete(Forth* pForth);
  static void vAccumNew(Forth* pForth);

  static void vAccumAccumBuffer(Forth* pForth);
  static void vAccumCountFetch(Forth* pForth);
  static void vAccumCountStore(Forth* pForth);
  static void vAccumGroupFetch(Forth* pForth);
  static void vAccumGroupStore(Forth* pForth);
  static void vAccumMaxFetch(Forth* pForth);
  static void vAccumMaxStore(Forth* pForth);
  static void vAccumMinFetch(Forth* pForth);
  static void vAccumMinStore(Forth* pForth);
  static void vAccumSumFetch(Forth* pForth);
  static void vAccumSumStore(Forth* pForth);

  // address
  static void vAddressEqual(Forth* pForth);
  static void vAddressFetch(Forth* pForth);
  static void vAddressStore(Forth* pForth);

  // apiState
  static void vApiStateIncBegun(Forth* pForth);
  static void vApiStateIncCancelled(Forth* pForth);
  static void vApiStateIncEnded(Forth* pForth);
  static void vApiStateIncExecuted(Forth* pForth);
  static void vApiStateSenLoc(Forth* pForth);
  static void vApiStateTraceSession(Forth* pForth);

  // args
  static void vArgsOscillate(Forth* pForth);
  static void vArgsPrint(Forth* pForth);

  // article
  static void vArticleDump(Forth* pForth);

  // assert
  static void vAssert(Forth* pForth);

  static void vAssertClass(Forth* pForth);
  static void vAssertClass_0(Forth* pForth);
  static void vAssertClass_1(Forth* pForth);
  static void vAssertClass_2(Forth* pForth);
  static void vAssertClass_3(Forth* pForth);
  static void vAssertClass_4(Forth* pForth);

  static void vAssertStack_0(Forth* pForth);
  static void vAssertStack_0more(Forth* pForth);
  static void vAssertStack_1(Forth* pForth);
  static void vAssertStack_1more(Forth* pForth);
  static void vAssertStack_2(Forth* pForth);
  static void vAssertStack_2more(Forth* pForth);
  static void vAssertStack_3(Forth* pForth);
  static void vAssertStack_3more(Forth* pForth);
  static void vAssertStack_4(Forth* pForth);
  static void vAssertStack_4more(Forth* pForth);

  // astack
  static void vAstackClass(Forth* pForth);
  static void vAstackDepth(Forth* pForth);
  static void vAstackDrop(Forth* pForth);
  static void vAstackDup(Forth* pForth);
  static void vAstackFetch(Forth* pForth);
  static void vAstackNip(Forth* pForth);
  static void vAstackPick(Forth* pForth);
  static void vAstackReverse(Forth* pForth);
  static void vAstackStk(Forth* pForth);
  static void vAstackSwap(Forth* pForth);
  static void vAstackToA(Forth* pForth);
  static void vAstackToD(Forth* pForth);
  static void vAstackTransfer(Forth* pForth);
  static void vAstack0(Forth* pForth);
  static void vAstack1(Forth* pForth);
  static void vAstack2(Forth* pForth);
  static void vAstack3(Forth* pForth);
  static void vAstack4(Forth* pForth);
  static void vAstack5(Forth* pForth);
  static void vAstack6(Forth* pForth);
  static void vAstack7(Forth* pForth);
  static void vAstack8(Forth* pForth);
  static void vAstack9(Forth* pForth);

  // attachmap
  static void vAttachMapTenantsFetch(Forth* pForth);

  // bp
  static void vBpAnalyze(Forth* pForth);
  static void vBpDump(Forth* pForth);
  static void vBpEpochDump(Forth* pForth);
  static void vBpEpochTake(Forth* pForth);
  static void vBpReadBufferPool(Forth* pForth);
  static void vBpWriteBufferPool(Forth* pForth);
  static void vBpFreeBuffers(Forth* pForth);
  static void vBpFreePercent(Forth* pForth);
  static void vBpFreeSpace(Forth* pForth);
  static void vBpInUseSpace(Forth* pForth);
  static void vBpTotalSpace(Forth* pForth);

  // cast
  static void vCastInt(Forth* pForth);
  static void vCastPtr(Forth* pForth);

  // class
  static void vClassName(Forth* pForth);

  // column
  static void vColumnIsNullableFetch(Forth* pForth);
  static void vColumnIsPartitionKeyFetch(Forth* pForth);
  static void vColumnLengthFetch(Forth* pForth);
  static void vColumnNameFetch(Forth* pForth);
  static void vColumnScaleFetch(Forth* pForth);
  static void vColumnTypeFetch(Forth* pForth);

  // command processor
  static void vCommandBlankLine(Forth* pForth);
  static void vCommandDeleteJob(Forth* pForth);
  static void vCommandExecuteFile(Forth* pForth);
  static void vCommandFindJobClass(Forth* pForth);
  static void vCommandGetCommand(Forth* pForth);
  static void vCommandJobClass(Forth* pForth);
  static void vCommandPostJob(Forth* pForth);
  static void vCommandPreJob(Forth* pForth);
  static void vCommandSpawnJob(Forth* pForth);

  // comment
  static void vComment(Forth* pForth);

  // comment
  static void vCompilerCurrent(Forth* pForth);

  // cpu
  static void vCpuRdtscp(Forth* pForth);

  // cursor
  static void vCursorFetch(Forth* pForth);
  static void vCursorStore(Forth* pForth);

  // ddllock
  static void vDdlLockDropRead(Forth* pForth);
  static void vDdlLockDropWrite(Forth* pForth);
  static void vDdlLockGrabRead(Forth* pForth);
  static void vDdlLockGrabWrite(Forth* pForth);
  static void vDdlLockTestDrop(Forth* pForth);

  // debug
  static void vDebugTrap(Forth* pForth);

  // dic
  static void vDicInit(Forth* pForth);
  static void vDicKindGet(Forth* pForth);
  static void vDicKindSet(Forth* pForth);
  static void vDicLastWord(Forth* pForth);
  static void vDicName(Forth* pForth);
  static void vDicRef(Forth* pForth);
  static void vDicRoot(Forth* pForth);
  static void vDicWordCount(Forth* pForth);

  // dicLock
  static void vDicLockDropWrite(Forth* pForth);
  static void vDicLockGrabWrite(Forth* pForth);

  // dicPath
  static void vDicPathDepth(Forth* pForth);
  static void vDicPathDump(Forth* pForth);
  static void vDicPathPick(Forth* pForth);

  // dicStack
  static void vDicStackDepth(Forth* pForth);
  static void vDicStackDrop(Forth* pForth);
  static void vDicStackDump(Forth* pForth);
  static void vDicStackPick(Forth* pForth);
  static void vDicStackPop(Forth* pForth);
  static void vDicStackPush(Forth* pForth);

  // dir
  static void vDirChangeCurrent(Forth* pForth);
  static void vDirGetCurrent(Forth* pForth);
  static void vDirExists(Forth* pForth);
  static void vDirMake(Forth* pForth);
  static void vDirRemove(Forth* pForth);

  static void vDirClose(Forth* pForth);
  static void vDirDelete(Forth* pForth);
  static void vDirEntryName(Forth* pForth);
  static void vDirEntryType(Forth* pForth);
  static void vDirIsClosed(Forth* pForth);
  static void vDirIsOpen(Forth* pForth);
  static void vDirNew(Forth* pForth);
  static void vDirOpen(Forth* pForth);
  static void vDirPathGet(Forth* pForth);
  static void vDirRead(Forth* pForth);
  static void vDirRewind(Forth* pForth);
  static void vDirSeek(Forth* pForth);
  static void vDirTell(Forth* pForth);

  // docol
  static void vDocolCompiling(Forth* pForth);
  static void vDocolInterpreting(Forth* pForth);
  static void vDocolIsCompiling(Forth* pForth);
  static void vDocolIsInterpreting(Forth* pForth);

  // dot
  static void vDot(Forth* pForth);
  static void vDotPut(Forth* pForth);
  static void vDotNewline(Forth* pForth);
  static void vDotVerbose(Forth* pForth);

  // dstack
  static void vDstackClass(Forth* pForth);                 //! DUP OF class@
  static void vDstackDepth(Forth* pForth);
  static void vDstackDrop(Forth* pForth);
  static void vDstackDropTwo(Forth* pForth);
  static void vDstackDup(Forth* pForth);
  static void vDstackDupNonZero(Forth* pForth);
  static void vDstackNip(Forth* pForth);
  static void vDstackOver(Forth* pForth);
  static void vDstackPick(Forth* pForth);
  static void vDstackReverse(Forth* pForth);
  static void vDstackRoll(Forth* pForth);
  static void vDstackRollInv(Forth* pForth);
  static void vDstackRot(Forth* pForth);
  static void vDstackRotInv(Forth* pForth);
  static void vDstackStk(Forth* pForth);
  static void vDstackSwap(Forth* pForth);
  static void vDstackTransfer(Forth* pForth);
  static void vDstackTuck(Forth* pForth);

  // encode
  static void vEncodeAgain(Forth* pForth);
  static void vEncodeAhead(Forth* pForth);
  static void vEncodeContinue(Forth* pForth);
  static void vEncodeHere(Forth* pForth);
  static void vEncodeIf(Forth* pForth);
  static void vEncodeIfAgain(Forth* pForth);
  static void vEncodeLoop(Forth* pForth);
  static void vEncodeLoopPlus(Forth* pForth);
  static void vEncodeNext(Forth* pForth);
  static void vEncodeThen(Forth* pForth);
  static void vEncodeUntil(Forth* pForth);

  // ename-set
  static void vEnmSetAddInt(Forth* pForth);
  static void vEnmSetAddStr(Forth* pForth);
  static void vEnmSetAsStr(Forth* pForth);
  static void vEnmSetGetStr(Forth* pForth);
  static void vEnmSetHasName(Forth* pForth);
  static void vEnmSetNew(Forth* pForth);

  // enter
  static void vEnterObject(Forth* pForth);
  static void vEnterRoutine(Forth* pForth);

  // entry
  static void vEntryMatches(Forth* pForth);
  static void vEntrySame(Forth* pForth);
  static void vEntryToDataRow(Forth* pForth);

  // entry buffer
  static void vEntBufferDump(Forth* pForth);
  static void vEntBufferIndexFetch(Forth* pForth);
  static void vEntBufferIndexStore(Forth* pForth);
  static void vEntBufferIsLastFetch(Forth* pForth);
  static void vEntBufferIsLastStore(Forth* pForth);
  static void vEntBufferNextBuffer(Forth* pForth);
  static void vEntBufferPartitionFetch(Forth* pForth);
  static void vEntBufferPartitionStore(Forth* pForth);
  static void vEntBufferToEntBuffer(Forth* pForth);

  // env
  static void vEnvGet(Forth* pForth);

  // errors
  static void vErrorMsg(Forth* pForth);

  // file
  static void vFileClose(Forth* pForth);
  static void vFileDelete(Forth* pForth);
  static void vFileNew(Forth* pForth);
  static void vFileOpen(Forth* pForth);
  static void vFilePathGet(Forth* pForth);
  static void vFileRead(Forth* pForth);
  static void vFileReadRecord(Forth* pForth);
  static void vFileWrite(Forth* pForth);

  // flow
  static void vFlowBegin_(Forth* pForth);
  static void vFlowExecute(Forth* pForth);
  static void vFlowHazard(Forth* pForth);
  static void vFlowJump(Forth* pForth);
  static void vFlowJumpBegin_(Forth* pForth);
  static void vFlowJumpFalse(Forth* pForth);
  static void vFlowJumpTrue(Forth* pForth);
  static void vFlowNop(Forth* pForth);
  static void vFlowRetryPoint(Forth* pForth);
  static void vFlowTest(Forth* pForth);
  static void vFlowThrow(Forth* pForth);

  // forth
  static void vAllot(Forth* pForth);
  static void vCell(Forth* pForth);
  static void vColon(Forth* pForth);
  static void vComma(Forth* pForth);
  static void vCompileComma(Forth* pForth);
  static void vCreate(Forth* pForth);
  static void vCreateFrom(Forth* pForth);
  static void vCreateFromInRoot(Forth* pForth);
  static void vCreateFromPersistent(Forth* pForth);
  static void vCreatePersistent(Forth* pForth);
  static void vCreateTenants(Forth* pForth);
  static void vDoes(Forth* pForth);
  static void vDoes_RT(Forth* pForth);
  static void vExit_RT(Forth* pForth);
  static void vFind(Forth* pForth);
  static void vFindInCurrent(Forth* pForth);
  static void vForget(Forth* pForth);
  static void vImmediate(Forth* pForth);
  static void vLocate(Forth* pForth);
  static void vPostpone(Forth* pForth);
  static void vPrompt(Forth* pForth);
  static void vSee(Forth* pForth);
  static void vSemicolon(Forth* pForth);
  static void vSeriesName(Forth* pForth);
  static void vSubDic(Forth* pForth);
  static void vTick(Forth* pForth);

  // grid
  static void vGridConnectedNodes(Forth* pForth);
  static void vGridDefinedNodes(Forth* pForth);
  static void vGridFindNode(Forth* pForth);
  static void vGridIncConnectedNodes(Forth* pForth);
  static void vGridNode(Forth* pForth);
  static void vGridPartitions(Forth* pForth);
  static void vGridShow(Forth* pForth);
  static void vGridSlots(Forth* pForth);
  static void vGridStartedStore(Forth* pForth);
  static void vGridStartedFetch(Forth* pForth);
  static void vGridStartNodeStore(Forth* pForth);
  static void vGridStartNodeFetch(Forth* pForth);
  static void vGridThisNodeFetch(Forth* pForth);
  static void vGridThisNodeNameFetch(Forth* pForth);
  static void vGridToNode(Forth* pForth);
  static void vGridToPartition(Forth* pForth);
  static void vGridToSlot(Forth* pForth);

  // group by
  static void vGroupByApplyBuffer(Forth* pForth);
  static void vGroupByApplyEntry(Forth* pForth);
  static void vGroupByApplyPartial(Forth* pForth);
  static void vGroupByDelete(Forth* pForth);
  static void vGroupByEnd(Forth* pForth);
  static void vGroupByNew(Forth* pForth);
  static void vGroupByStart(Forth* pForth);

  // handler
  static void vHandlerServiceSleepQueue(Forth* pForth);

  // index
  static void vIndexAnalyze(Forth* pForth);
  static void vIndexColumnFetch(Forth* pForth);
  static void vIndexColumnCountFetch(Forth* pForth);
  static void vIndexColumnName(Forth* pForth);
  static void vIndexColumnType(Forth* pForth);
  static void vIndexCompact(Forth* pForth);
  static void vIndexCreateColumnVec(Forth* pForth);
  static void vIndexDump(Forth* pForth);
  static void vIndexInit(Forth* pForth);
  static void vIndexIsPrimaryKeyFetch(Forth* pForth);
  static void vIndexIsPrimaryKeyStore(Forth* pForth);
  static void vIndexIsUniqueFetch(Forth* pForth);
  static void vIndexIsUniqueStore(Forth* pForth);
  static void vIndexIsValidUniqueIndex(Forth* pForth);
  static void vIndexLowerBound(Forth* pForth);
  static void vIndexNameFetch(Forth* pForth);
  static void vIndexPopulate(Forth* pForth);
  static void vIndexRange(Forth* pForth);
  static void vIndexShowColumnVec(Forth* pForth);
  static void vIndexSpace(Forth* pForth);
  static void vIndexSubDic(Forth* pForth);
  static void vIndexTableFetch(Forth* pForth);
  static void vIndexTableStore(Forth* pForth);
  static void vIndexTruncate(Forth* pForth);
  static void vIndexUpperBound(Forth* pForth);
  static void vIndexVerify(Forth* pForth);
  static void vIndexVisualize(Forth* pForth);

  // input
  static void vInputClose(Forth* pForth);
  static void vInputDepth(Forth* pForth);
  static void vInputNext(Forth* pForth);
  static void vInputOpen(Forth* pForth);
  static void vInputPop(Forth* pForth);
  static void vInputPush(Forth* pForth);
  static void vInputStdin(Forth* pForth);
  static void vInputText(Forth* pForth);
  static void vInputToken(Forth* pForth);
  static void vInputType(Forth* pForth);
  static void vInputUnknown(Forth* pForth);

  // int
  static void vIntAdd(Forth* pForth);
  static void vIntDivide(Forth* pForth);
  static void vIntEqual(Forth* pForth);
  static void vIntGreater(Forth* pForth);
  static void vIntGreaterEqual(Forth* pForth);
  static void vIntHash(Forth* pForth);
  static void vIntHo16(Forth* pForth);
  static void vIntLess(Forth* pForth);
  static void vIntLessEqual(Forth* pForth);
  static void vIntModulo(Forth* pForth);
  static void vIntMultiply(Forth* pForth);
  static void vIntNeg(Forth* pForth);
  static void vIntNotEqual(Forth* pForth);
  static void vIntRandom(Forth* pForth);
  static void vIntSubtract(Forth* pForth);
  static void vUnsignedIntRandom(Forth* pForth);

  // jobClass
  static void vJobClassBuffer(Forth* pForth);
  static void vJobClassFind(Forth* pForth);
  static void vJobClassName(Forth* pForth);
  static void vJobClassNew(Forth* pForth);

  static void vJobClassSeedRun(Forth* pForth);
  static void vJobClassSeedReq(Forth* pForth);
  static void vJobClassSeedRcv(Forth* pForth);
  static void vJobClassSeedExe(Forth* pForth);
  static void vJobClassSeedEnd(Forth* pForth);
  static void vJobClassSeedCan(Forth* pForth);

  // job
  static void vJobAcksRemaining(Forth* pForth);
  static void vJobAddOriginator(Forth* pForth);
  static void vJobAddParticipant(Forth* pForth);
  static void vJobAssertBegin(Forth* pForth);
  static void vJobAssertNeutral(Forth* pForth);
  static void vJobAssertOriginator(Forth* pForth);
  static void vJobAssertParticipant(Forth* pForth);
  static void vJobAssertReceived(Forth* pForth);

  // job mark 2
  static void vJobMark2IndexStore(Forth* pForth);
  static void vJobMark2TableNameFetch(Forth* pForth);
  static void vJobMark2TableStore(Forth* pForth);

  static void vJobCatalogNameFetch(Forth* pForth);
  static void vJobClass(Forth* pForth);
  static void vJobColumnCountFetch(Forth* pForth);
  static void vJobColumnNameFetch(Forth* pForth);
  static void vJobColumnIsPartKey(Forth* pForth);
  static void vJobColumnTypeFetch(Forth* pForth);
  static void vJobCompleted(Forth* pForth);
  static void vJobCreateResponseVec(Forth* pForth);
  static void vJobOperandFetch(Forth* pForth);
  static void vJobOperandStore(Forth* pForth);

  static void vJobDecAcksRemaining(Forth* pForth);
  static void vJobDelete(Forth* pForth);
  static void vJobDicPath(Forth* pForth);

  static void vJobEnameFetch(Forth* pForth);
  static void vJobEnameStore(Forth* pForth);
  static void vJobErrorStore(Forth* pForth);
  static void vJobErrorFetch(Forth* pForth);
  static void vJobExecuteFetch(Forth* pForth);

  static void vJobFindOriginator(Forth* pForth);
  static void vJobFindParticipant(Forth* pForth);
  static void vJobFolderNameFetch(Forth* pForth);

  static void vJobHasError(Forth* pForth);
  static void vJobHasOperand(Forth* pForth);

  static void vJobIdFetch(Forth* pForth);
  static void vJobIndexNameFetch(Forth* pForth);
  static void vJobIsExecuting(Forth* pForth);

  static void vJobJobClass(Forth* pForth);
  static void vJobJobListFetch(Forth* pForth);

  static void vJobMetadataStore(Forth* pForth);

  static void vJobNodeNameFetch(Forth* pForth);
  static void vJobNoWaitFetch(Forth* pForth);

  static void vJobObjectNameFetch(Forth* pForth);
  static void vJobOriginatingTaskFetch(Forth* pForth);
  static void vJobOriginatingTaskStore(Forth* pForth);
  static void vJobOriginatorFetch(Forth* pForth);

  static void vJobParentIdFetch(Forth* pForth);
  static void vJobParticipantFetch(Forth* pForth);
  static void vJobParticipantStore(Forth* pForth);
  static void vJobParticipantResponseStore(Forth* pForth);
  static void vJobParticipantHasError(Forth* pForth);
  static void vJobPartitionsFetch(Forth* pForth);
  static void vJobPhaseFetch(Forth* pForth);
  static void vJobPhaseStore(Forth* pForth);
  static void vJobPostRecv(Forth* pForth);
  static void vJobPreSend(Forth* pForth);

  static void vJobRecv(Forth* pForth);
  static void vJobRegionSizeFetch(Forth* pForth);
  static void vJobRemoveOriginator(Forth* pForth);
  static void vJobRemoveParticipant(Forth* pForth);
  static void vJobReseed(Forth* pForth);
  static void vJobResponseStore(Forth* pForth);
  static void vJobResponseFetch(Forth* pForth);
  static void vJobRoutineNameFetch(Forth* pForth);
  static void vJobRowsInvalidFetch(Forth* pForth);
  static void vJobRowsInvalidStore(Forth* pForth);
  static void vJobRowsProcessedFetch(Forth* pForth);
  static void vJobRowsProcessedStore(Forth* pForth);
  static void vJobRowsSkippedFetch(Forth* pForth);
  static void vJobRowsSkippedStore(Forth* pForth);

  static void vJobSchemaNameFetch(Forth* pForth);
  static void vJobSeedFetch(Forth* pForth);
  static void vJobSend(Forth* pForth);
  static void vJobSessionStateFetch(Forth* pForth);
  static void vJobShow(Forth* pForth);
  static void vJobSignalWaiter(Forth* pForth);
  static void vJobSlotFetch(Forth* pForth);
  static void vJobSpawn(Forth* pForth);
  static void vJobSpawnDedicated(Forth* pForth);
  static void vJobStateDiagram(Forth* pForth);

  static void vJobTaskFetch(Forth* pForth);
  static void vJobTaskStore(Forth* pForth);
  static void vJobTenantNameFetch(Forth* pForth);
  static void vJobThis(Forth* pForth);

  static void vJobWaitParticipant(Forth* pForth);

  // job generic
  static void vJob__BegunNew(Forth* pForth);
  static void vJob__CancelNew(Forth* pForth);
  static void vJob__CancelledNew(Forth* pForth);
  static void vJob__EndNew(Forth* pForth);
  static void vJob__EndedNew(Forth* pForth);
  static void vJob__ExecuteNew(Forth* pForth);
  static void vJob__ExecutedNew(Forth* pForth);

  // job analyzeIndex
  static void vJob_AnalyzeIndexIsDetailed(Forth* pForth);
  static void vJob_AnalyzeTableIsDetailed(Forth* pForth);

  // job Cat
  static void vJob_CatBegin(Forth* pForth);
  static void vJob_CatChunk(Forth* pForth);
  static void vJob_CatEnd(Forth* pForth);

  // job CreateIndex
  static void vJob_CreateIndexIsUniqueFetch(Forth* pForth);

  // job Dynamic
  static void vJob_DynamicBegin(Forth* pForth);
  static void vJob_DynamicEnd(Forth* pForth);
  static void vJob_DynamicExecute(Forth* pForth);

  // job Export
  static void vJob_ExportAuxAckDecidedFetch(Forth* pForth);
  static void vJob_ExportAuxAckDecidedStore(Forth* pForth);
  static void vJob_ExportBegin(Forth* pForth);
  static void vJob_ExportEnd(Forth* pForth);
  static void vJob_ExportTableStore(Forth* pForth);

  // job FetchRows
  static void vJob_FetchRowsColumnNameFetch(Forth* pForth);
  static void vJob_FetchRowsColumnStore(Forth* pForth);
  static void vJob_FetchRowsLastBuffersStore(Forth* pForth);
  static void vJob_FetchRowsPartitionFetch(Forth* pForth);
  static void vJob_FetchRowsPlannerStore(Forth* pForth);
  static void vJob_FetchRowsSpawnSub(Forth* pForth);
  static void vJob_FetchRowsSpawnSubInflate(Forth* pForth);
  static void vJob_FetchRowsTableStore(Forth* pForth);
  static void vJob_FetchRowsWhereFetch(Forth* pForth);

  // job GenData
  static void vJob_GenerateGenerate(Forth* pForth);
  static void vJob_GenerateTableStore(Forth* pForth);

  // job IndexJoin
  static void vJob_IndexJoinLastBuffersStore(Forth* pForth);
  static void vJob_IndexJoinLeftIndexStore(Forth* pForth);
  static void vJob_IndexJoinLeftTableStore(Forth* pForth);
  static void vJob_IndexJoinRightIndexStore(Forth* pForth);
  static void vJob_IndexJoinRightIndexNameFetch(Forth* pForth);
  static void vJob_IndexJoinRightTableStore(Forth* pForth);
  static void vJob_IndexJoinRightTableNameFetch(Forth* pForth);
  static void vJob_IndexJoinSpawnSub(Forth* pForth);
  static void vJob_SubIndexJoinIndexJoin(Forth* pForth);

  // job inflate
  static void vJob_InflateRowsColumnNameFetch(Forth* pForth);
  static void vJob_InflateRowsColumnStore(Forth* pForth);
  static void vJob_InflateRowsSpawnSub(Forth* pForth);
  static void vJob_InflateRowsSpawnSubInflate(Forth* pForth);
  static void vJob_InflateRowsSubInfTasksFetch(Forth* pForth);
  static void vJob_InflateRowsTableStore(Forth* pForth);
  static void vJob_InflateRowsWhereFetch(Forth* pForth);

  // job Ingest
  static void vJob_IngestBegin(Forth* pForth);
  static void vJob_IngestChunk(Forth* pForth);
  static void vJob_IngestEnd(Forth* pForth);
  static void vJob_IngestTableStore(Forth* pForth);

  // job InsertRows
  static void vJob_CopyBufferCopy(Forth* pForth);

  static void vJob_InsertRowPartitionFetch(Forth* pForth);
  static void vJob_InsertRowRecordStore(Forth* pForth);
  static void vJob_InsertRowNewJobWriteRow(Forth* pForth);
  static void vJob_InsertRowSpawnJobWriteRow(Forth* pForth);

  static void vJob_InsertTableSubInserterJobFetch(Forth* pForth);

  static void vJob_SubInserterBegin(Forth* pForth);
  static void vJob_SubInserterEnd(Forth* pForth);

  // job KidExport
  static void vJob_KidExportExportOrigin(Forth* pForth);
  static void vJob_KidExportExportJobId(Forth* pForth);
  static void vJob_KidExportWriteExportBuffer(Forth* pForth);

  // job LoadTable
  static void vJob_LoadTableBegin(Forth* pForth);
  static void vJob_LoadTableChunk(Forth* pForth);
  static void vJob_LoadTableDumpMap(Forth* pForth);
  static void vJob_LoadTableEnd(Forth* pForth);
  static void vJob_LoadTableFilePathFetch(Forth* pForth);
  static void vJob_LoadTableTableStore(Forth* pForth);

  // job Monitor
  static void vJob_MonitorBegin(Forth* pForth);
  static void vJob_MonitorTake(Forth* pForth);
  static void vJob_MonitorEnd(Forth* pForth);

  // job NetHog
  static void vJob_NetHogHogNoFetch(Forth* pForth);

  // job NodeInfo
  static void vJob_NodeInfoNodeNameFetch(Forth* pForth);

  // job publish
  static void vJob_PublishSpawnSub(Forth* pForth);
  static void vJob_PublishTableStore(Forth* pForth);

  // job Purge
  static void vJob_PurgeSpawnSub(Forth* pForth);
  static void vJob_PurgeTableStore(Forth* pForth);

  // job Query
  static void vJob_QueryBegin(Forth* pForth);
  static void vJob_QueryEnd(Forth* pForth);
  static void vJob_QueryExecute(Forth* pForth);

  // job Receiver
  static void vJob_ReceiverConnNoFetch(Forth* pForth);
  static void vJob_ReceiverConnNoStore(Forth* pForth);
  static void vJob_ReceiverJobReceived(Forth* pForth);
  static void vJob_ReceiverNodeFetch(Forth* pForth);
  static void vJob_ReceiverNodeStore(Forth* pForth);



  // job Repeater
  static void vJob_RepeaterBegin(Forth* pForth);
  static void vJob_RepeaterChunk(Forth* pForth);
  static void vJob_RepeaterEnd(Forth* pForth);

  // job Report
  static void vJob_ReportBegin(Forth* pForth);
  static void vJob_ReportTake(Forth* pForth);
  static void vJob_ReportEnd(Forth* pForth);

  // job Select
  static void vJob_SelectBegin(Forth* pForth);
  static void vJob_SelectEnd(Forth* pForth);
  static void vJob_SelectExecute(Forth* pForth);

  // job SelectTable
  static void vJob_SelectTablePartitionFetch(Forth* pForth);
  static void vJob_SelectTableTableStore(Forth* pForth);

  // job Sink
  static void vJob_SinkBegin(Forth* pForth);
  static void vJob_SinkChunk(Forth* pForth);
  static void vJob_SinkEnd(Forth* pForth);

  // job Sort
  static void vJob_SortBegin(Forth* pForth);
  static void vJob_SortEnd(Forth* pForth);
  static void vJob_SortExecute(Forth* pForth);

  // job SubExporter
  static void vJob_SubExporterBegin(Forth* pForth);
  static void vJob_SubExporterChunk(Forth* pForth);
  static void vJob_SubExporterEnd(Forth* pForth);
  static void vJob_SubExporterSpawn(Forth* pForth);
  static void vJob_SubExporterExportJobFetch(Forth* pForth);

  // job SubFetch
  static void vJob_SubFetchBufferQueueFetch(Forth* pForth);
  static void vJob_SubFetchPhraseFetch(Forth* pForth);
  static void vJob_SubFetchPlannerFetch(Forth* pForth);
  static void vJob_SubFetchSlotFetch(Forth* pForth);

  // job SubInflate
  static void vJob_SubInflateJobInflateRowsFetch(Forth* pForth);
  static void vJob_SubInflateInflateRows(Forth* pForth);

  // job SubIngester
  static void vJob_SubIngesterBegin(Forth* pForth);
  static void vJob_SubIngesterChunk(Forth* pForth);
  static void vJob_SubIngesterEnd(Forth* pForth);
  static void vJob_SubIngesterJobIngest(Forth* pForth);
  static void vJob_SubIngesterSpawn(Forth* pForth);

  // job SubLoader
  static void vJob_SubLoaderBegin(Forth* pForth);
  static void vJob_SubLoaderChunk(Forth* pForth);
  static void vJob_SubLoaderEnd(Forth* pForth);
  static void vJob_SubLoaderSpawn(Forth* pForth);
  static void vJob_SubLoaderTerminate(Forth* pForth);

  // job SubPublish
  static void vJob_SubPublishBegin(Forth* pForth);
  static void vJob_SubPublishChunk(Forth* pForth);
  static void vJob_SubPublishEnd(Forth* pForth);
  static void vJob_SubPublishSlotFetch(Forth* pForth);
  static void vJob_SubPublishTableStore(Forth* pForth);
  static void vJob_SubPublishPublishJobIdFetch(Forth* pForth);

  // job SubPurger
  static void vJob_SubPurgerBegin(Forth* pForth);
  static void vJob_SubPurgerChunk(Forth* pForth);
  static void vJob_SubPurgerEnd(Forth* pForth);

  static void vJob_SubPurgerSlotFetch(Forth* pForth);
  static void vJob_SubPurgerPurgeJobIdFetch(Forth* pForth);

  // job SubScan
  static void vJob_SubScanBufferQueueFetch(Forth* pForth);
  static void vJob_SubScanSlotFetch(Forth* pForth);

  // Job SubTruncate
  static void vJob_SubTruncateSlotFetch(Forth* pForth);
  static void vJob_SubTruncateTruncateJobIdFetch(Forth* pForth);

  // job SubWalker
  static void vJob_SubWalkerBegin(Forth* pForth);
  static void vJob_SubWalkerChunk(Forth* pForth);
  static void vJob_SubWalkerEnd(Forth* pForth);
  static void vJob_SubWalkerWalkJobFetch(Forth* pForth);
  static void vJob_SubWalkerSpawn(Forth* pForth);

  // Job Truncate
  static void vJob_TruncateTableSpawnSub(Forth* pForth);

  // job Walk
  static void vJob_WalkBegin(Forth* pForth);
  static void vJob_WalkEnd(Forth* pForth);
  static void vJob_WalkIndexStore(Forth* pForth);
  static void vJob_WalkTableStore(Forth* pForth);

  // Job WriteBuffer
  static void vJob_WriteBufferParentJobIdFetch(Forth* pForth);
  static void vJob_WriteBufferInsertRows(Forth* pForth);
  static void vJob_WriteBufferIsIngesting(Forth* pForth);
  static void vJob_WriteBufferIsInserting(Forth* pForth);
  static void vJob_WriteBufferIsLoading(Forth* pForth);
  static void vJob_WriteBufferPartitionFetch(Forth* pForth);
  static void vJob_WriteBufferTableStore(Forth* pForth);

  // Job WriteRow
  static void vJob_WriteRowParentJobIdFetch(Forth* pForth);
  static void vJob_WriteRowEncodeRow(Forth* pForth);
  static void vJob_WriteRowInsertRow(Forth* pForth);
  static void vJob_WriteRowPartitionFetch(Forth* pForth);
  static void vJob_WriteRowTableStore(Forth* pForth);

  // Job Operand
  static void vJob_OperandPipeOut(Forth* pForth);

  // load table
  static void vLoadTableChunkFile(Forth* pForth);
  static void vLoadTableDelete(Forth* pForth);
  static void vLoadTableDumpMap(Forth* pForth);
  static void vLoadTableNew(Forth* pForth);
  static void vLoadTableTableStore(Forth* pForth);

  // locals
  static void vLocalFetch(Forth* pForth);
  static void vLocalStore(Forth* pForth);
  static void vLocalsBar(Forth* pForth);
  static void vLocalsPop(Forth* pForth);
  static void vLocalsPush(Forth* pForth);
  static void vLocalsTo(Forth* pForth);

  // lock
  static void vLockDropRead(Forth* pForth);
  static void vLockDropWrite(Forth* pForth);
  static void vLockGrabRead(Forth* pForth);
  static void vLockGrabWrite(Forth* pForth);
  static void vLockInit(Forth* pForth);

  // locks
  static void vLockTrackerList(Forth* pForth);
  static void vLockTrackerReset(Forth* pForth);

  // log
  static void vLogDump(Forth* pForth);
  static void vLogMark(Forth* pForth);
  static void vLogMarkEntry(Forth* pForth);
  static void vLogMarkExit(Forth* pForth);
  static void vLogMarkPhase(Forth* pForth);

  static void vLogMarkWord(Forth* pForth);
  static void vLogMarkSeed(Forth* pForth);

  static void vLogJobSeed(Forth* pForth);

  // logic
  static void vLogicAnd(Forth* pForth);
  static void vLogicNot(Forth* pForth);
  static void vLogicOr(Forth* pForth);

  // magnitude
  static void vBytesMagnitude(Forth* pForth);
  static void vNumberMagnitude(Forth* pForth);
  static void vToDigitName(Forth* pForth);

  // memory
  static void vMemoryByteDump(Forth* pForth);
  static void vMemoryReset(Forth* pForth);
  static void vMemoryStatistics(Forth* pForth);
  static void vMemoryWordDump(Forth* pForth);

  // node (of grid)
  static void vNodeForget(Forth* pForth);
  static void vNodeIP(Forth* pForth);
  static void vNodeName(Forth* pForth);
  static void vNodeNew(Forth* pForth);
  static void vNodeNo(Forth* pForth);
  static void vNodePartitions(Forth* pForth);
  static void vNodePort(Forth* pForth);
  static void vNodeRecvSocketStore(Forth* pForth);
  static void vNodeRecvSocketFetch(Forth* pForth);
  static void vNodeSendSocketStore(Forth* pForth);
  static void vNodeSendSocketFetch(Forth* pForth);
  static void vNodeSize(Forth* pForth);
  static void vNodeStartNode(Forth* pForth);
  static void vNodeStateStore(Forth* pForth);
  static void vNodeStateFetch(Forth* pForth);
  static void vNodeStatus(Forth* pForth);

  // object
  static void vObjectClassFetch(Forth* pForth);
  static void vObjectClassStore(Forth* pForth);
  static void vObjectDump(Forth* pForth);
  static void vObjectFetch(Forth* pForth);
  static void vObjectMagicMark(Forth* pForth);
  static void vObjectStore(Forth* pForth);
  static void vObjectWordBodySet(Forth* pForth);

  // persistence
  static void vPersistenceCheckpoint(Forth* pForth);
  static void vPersistenceReload(Forth* pForth);

  // plan
  static void vPlanCompile(Forth* pForth);
  static void vPlanDelete(Forth* pForth);
  static void vPlanDic(Forth* pForth);
  static void vPlanExecutingTaskFetch(Forth* pForth);
  static void vPlanExecutingTaskStore(Forth* pForth);
  static void vPlanFinished(Forth* pForth);
  static void vPlanIdFetch(Forth* pForth);
  static void vPlanLambdaAppend(Forth* pForth);
  static void vPlanLambdaFetch(Forth* pForth);
  static void vPlanLambdaStore(Forth* pForth);
  static void vPlanNew(Forth* pForth);
  static void vPlanNameFetch(Forth* pForth);
  static void vPlanNameStore(Forth* pForth);
  static void vPlanShow(Forth* pForth);

  // planner
  static void vPlannerChoosePaths(Forth* pForth);
  static void vPlannerChosenPath(Forth* pForth);
  static void vPlannerDelete(Forth* pForth);
  static void vPlannerFetchTechniqueFetch(Forth* pForth);
  static void vPlannerGenPaths(Forth* pForth);
  static void vPlannerHighKeyFetch(Forth* pForth);
  static void vPlannerIndexNmFetch(Forth* pForth);
  static void vPlannerKeyFetch(Forth* pForth);
  static void vPlannerLowKeyFetch(Forth* pForth);
  static void vPlannerMatchPaths(Forth* pForth);
  static void vPlannerNew(Forth* pForth);
  static void vPlannerParseWhere(Forth* pForth);
  static void vPlannerPhrasesFetch(Forth* pForth);
  static void vPlannerScorePaths(Forth* pForth);
  static void vPlannerShowPaths(Forth* pForth);
  static void vPlannerTableFetch(Forth* pForth);
  static void vPlannerTableStore(Forth* pForth);

  // process
  static void vProcessGetPid(Forth* pForth);
  static void vProcessQuit(Forth* pForth);
  static void vProcessTestPid(Forth* pForth);

  // prompt
  static void vPromptContinue(Forth* pForth);
  static void vPromptExecuteFunction(Forth* pForth);
  static void vPromptGetCommand(Forth* pForth);
  static void vPromptNew(Forth* pForth);
  static void vPromptSession(Forth* pForth);

  // push
  static void vPushI(Forth* pForth);
  static void vPushJ(Forth* pForth);
  static void vPushNullPtr(Forth* pForth);
  static void vPushValue(Forth* pForth);

  // queue
  static void vQueueAppend(Forth* pForth);
  static void vQueueDelete(Forth* pForth);
  static void vQueueDump(Forth* pForth);
  static void vQueueInit(Forth* pForth);
  static void vQueueNew(Forth* pForth);
  static void vQueuePop(Forth* pForth);

  // queues
  static void vQueuesCheckSleeping(Forth* pForth);
  static void vQueuesExecutingLength(Forth* pForth);
  static void vQueuesReadyLength(Forth* pForth);
  static void vQueuesSleepingLength(Forth* pForth);
  static void vQueuesWaitingLength(Forth* pForth);

  // rbCursor
  static void vRbCursorBegin(Forth* pForth);
  static void vRbCursorDelete(Forth* pForth);
  static void vRbCursorEnd(Forth* pForth);
  static void vRbCursorFirst(Forth* pForth);
  static void vRbCursorGet(Forth* pForth);
  static void vRbCursorInit(Forth* pForth);
  static void vRbCursorNew(Forth* pForth);
  static void vRbCursorNext(Forth* pForth);
  static void vRbCursorTableFetch(Forth* pForth);
  static void vRbCursorTableStore(Forth* pForth);

  // rowCursor
  static void vRowCursorBegin(Forth* pForth);
  static void vRowCursorDelete(Forth* pForth);
  static void vRowCursorEnd(Forth* pForth);
  static void vRowCursorEntBufferFetch(Forth* pForth);
  static void vRowCursorEntBufferStore(Forth* pForth);
  static void vRowCursorFirst(Forth* pForth);
  static void vRowCursorInit(Forth* pForth);
  static void vRowCursorNew(Forth* pForth);
  static void vRowCursorNext(Forth* pForth);
  static void vRowCursorRow(Forth* pForth);
  static void vRowCursorTableFetch(Forth* pForth);
  static void vRowCursorTableStore(Forth* pForth);

  // rstack
  static void vRstackCsPick(Forth* pForth);
  static void vRstackCsRoll(Forth* pForth);
  static void vRstackDepth(Forth* pForth);
  static void vRstackDrop(Forth* pForth);
  static void vRstackFetch(Forth* pForth);
  static void vRstackFetchTwo(Forth* pForth);
  static void vRstackPick(Forth* pForth);
  static void vRstackReverse(Forth* pForth);
  static void vRstackRoll(Forth* pForth);
  static void vRstackRollInv(Forth* pForth);
  static void vRstackStk(Forth* pForth);
  static void vRstackSwap(Forth* pForth);
  static void vRstackToP(Forth* pForth);
  static void vRstackToR(Forth* pForth);
  static void vRstackToRTwo(Forth* pForth);
  static void vRstack0(Forth* pForth);
  static void vRstack1(Forth* pForth);
  static void vRstack2(Forth* pForth);
  static void vRstack3(Forth* pForth);
  static void vRstack4(Forth* pForth);
  static void vRstack5(Forth* pForth);
  static void vRstack6(Forth* pForth);
  static void vRstack7(Forth* pForth);
  static void vRstack8(Forth* pForth);
  static void vRstack9(Forth* pForth);

  // schLock
  static void vSchLockDropWrite(Forth* pForth);
  static void vSchLockGrabWrite(Forth* pForth);

  // session
  static void vSessionCommand(Forth* pForth);
  static void vSessionDicStack(Forth* pForth);
  static void vSessionDump(Forth* pForth);
  static void vSessionIsExiting(Forth* pForth);
  static void vSessionPid(Forth* pForth);
  static void vSessionSlotFetch(Forth* pForth);
  static void vSessionTxFetch(Forth* pForth);
  static void vSessionTxStore(Forth* pForth);

  // shell
  static void vShellExecute(Forth* pForth);

  // slots
  static void vSlotsColumnCount(Forth* pForth);
  static void vSlotsColumnName(Forth* pForth);
  static void vSlotsCreateColumnVec(Forth* pForth);
  static void vSlotsSubDic(Forth* pForth);

  // socket
  static void vSocketAccept(Forth* pForth);
  static void vSocketClose(Forth* pForth);
  static void vSocketConnect(Forth* pForth);
  static void vSocketDropWrite(Forth* pForth);
  static void vSocketGrabWrite(Forth* pForth);
  static void vSocketListen(Forth* pForth);
  static void vSocketOpen(Forth* pForth);
  static void vSocketRecvObject(Forth* pForth);
  static void vSocketRecvObject_LowMem(Forth* pForth);
  static void vSocketSendObject(Forth* pForth);

  // steps
  static void vStepTimingReset(Forth* pForth);
  static void vStepTimingStart(Forth* pForth);
  static void vStepTimingWrite(Forth* pForth);

  // string
  static void vStrAlgebra(Forth* pForth);
  static void vStrCat(Forth* pForth);
  static void vStrClip(Forth* pForth);
  static void vStrEqual(Forth* pForth);
  static void vStrEscape(Forth* pForth);
  static void vStrFirstCsv(Forth* pForth);
  static void vStrFormat(Forth* pForth);
  static void vStrLeft(Forth* pForth);
  static void vStrLength(Forth* pForth);
  static void vStrPrepend(Forth* pForth);
  static void vStrRandom(Forth* pForth);
  static void vStrRemovePrefix(Forth* pForth);
  static void vStrRemoveSuffix(Forth* pForth);
  static void vStrReplace(Forth* pForth);
  static void vStrRight(Forth* pForth);
  static void vStrSub(Forth* pForth);
  static void vStrTailInt(Forth* pForth);

  // sub loader
  static void vSubLoaderDelete(Forth* pForth);

  // table
  static void vTableAddIndex(Forth* pForth);
  static void vTableAnalyze(Forth* pForth);
  static void vTableColumnCountFetch(Forth* pForth);
  static void vTableCreateColumnVec(Forth* pForth);
  static void vTableDrop(Forth* pForth);
  static void vTableDump(Forth* pForth);
  static void vTableFindColumn(Forth* pForth);
  static void vTableFirstBuffer(Forth* pForth);
  static void vTableGenData(Forth* pForth);
  static void vTableGetColumnsAsString(Forth* pForth);
  static void vTableColumnFetch(Forth* pForth);
  static void vTableGetPtn(Forth* pForth);
  static void vTableInflateScan(Forth* pForth);
  static void vTableIndexCount(Forth* pForth);
  static void vTableInit(Forth* pForth);
  static void vTableIsColumn(Forth* pForth);
  static void vTableIsLoadingStore(Forth* pForth);
  static void vTableLazyBuildIndexes(Forth* pForth);
  static void vTableList(Forth* pForth);
  static void vTableName(Forth* pForth);
  static void vTablePublish(Forth* pForth);
  static void vTablePurge(Forth* pForth);
  static void vTableRemoveIndex(Forth* pForth);
  static void vTableScan(Forth* pForth);
  static void vTableShowColumnVec(Forth* pForth);
  static void vTableSlots(Forth* pForth);
  static void vTableSpace(Forth* pForth);
  static void vTableSubDic(Forth* pForth);
  static void vTableSubLoaders(Forth* pForth);
  static void vTableTruncate(Forth* pForth);
  static void vTableIsValidIndexColumns(Forth* pForth);
  static void vTableVerify(Forth* pForth);
  static void vTableVisualize(Forth* pForth);

  // table partition
  static void vTablePtnAttemptGrabRead(Forth* pForth);
  static void vTablePtnAttemptGrabWrite(Forth* pForth);
  static void vTablePtnDropRead(Forth* pForth);
  static void vTablePtnDropWrite(Forth* pForth);
  static void vTablePtnDump(Forth* pForth);
  static void vTablePtnIsLocked(Forth* pForth);

  // task
  static void vTaskAck(Forth* pForth);
  static void vTaskAcksStore(Forth* pForth);
  static void vTaskBreakPoint(Forth* pForth);
  static void vTaskDestructorTraceFetch(Forth* pForth);
  static void vTaskDestructorTraceStore(Forth* pForth);
  static void vTaskDicStack(Forth* pForth);
  static void vTaskDropLock(Forth* pForth);
  static void vTaskDump(Forth* pForth);
  static void vTaskDelete(Forth* pForth);
  static void vTaskGrabLock(Forth* pForth);
  static void vTaskIsAckSet(Forth* pForth);
  static void vTaskJobFetch(Forth* pForth);
  static void vTaskJobStore(Forth* pForth);
  static void vTaskNameFetch(Forth* pForth);
  static void vTaskThis(Forth* pForth);
  static void vTaskSeed(Forth* pForth);
  static void vTaskSessionFetch(Forth* pForth);
  static void vTaskSessionStore(Forth* pForth);
  static void vTaskSendNodeFetch(Forth* pForth);
  static void vTaskSendNodeStore(Forth* pForth);
  static void vTaskSleepMsec(Forth* pForth);
  static void vTaskSleepNsec(Forth* pForth);
  static void vTaskSleepSsec(Forth* pForth);
  static void vTaskSleepUsec(Forth* pForth);
  static void vTaskStateFetch(Forth* pForth);
  static void vTaskThread(Forth* pForth);
  static void vTaskWait(Forth* pForth);
  static void vTaskYield(Forth* pForth);

  // test
  static void vTestTest0(Forth* pForth);
  static void vTestTest1(Forth* pForth);
  static void vTestTest2(Forth* pForth);
  static void vTestTest3(Forth* pForth);
  static void vTestTest4(Forth* pForth);
  static void vTestTest5(Forth* pForth);
  static void vTestTest6(Forth* pForth);
  static void vTestTest7(Forth* pForth);
  static void vTestTest8(Forth* pForth);
  static void vTestTest9(Forth* pForth);
  static void vTestTest10(Forth* pForth);
  static void vTestTest11(Forth* pForth);
  static void vTestTest12(Forth* pForth);
  static void vTestTest13(Forth* pForth);
  static void vTestTest14(Forth* pForth);
  static void vTestTest15(Forth* pForth);
  static void vTestTest16(Forth* pForth);
  static void vTestTest17(Forth* pForth);
  static void vTestTest18(Forth* pForth);
  static void vTestTest19(Forth* pForth);
  static void vTestTest20(Forth* pForth);
  static void vTestTest21(Forth* pForth);
  static void vTestTest22(Forth* pForth);
  static void vTestTest23(Forth* pForth);
  static void vTestTest24(Forth* pForth);
  static void vTestTest25(Forth* pForth);

  // thread
  static void vThreadBind(Forth* pForth);
  static void vThreadCpu(Forth* pForth);
  static void vThreadDedicatedFetch(Forth* pForth);
  static void vThreadDedicatedStore(Forth* pForth);
  static void vThreadDisableRealtime(Forth* pForth);
  static void vThreadDump(Forth* pForth);
  static void vThreadEnableRealtime(Forth* pForth);
  static void vThreadId(Forth* pForth);
  static void vThreadSleepMsec(Forth* pForth);
  static void vThreadSleepNsec(Forth* pForth);
  static void vThreadSleepSsec(Forth* pForth);
  static void vThreadSleepUsec(Forth* pForth);

  // timer
  static void vTimerDelete(Forth* pForth);
  static void vTimerInit(Forth* pForth);
  static void vTimerMicrosecs(Forth* pForth);
  static void vTimerMillisecs(Forth* pForth);
  static void vTimerNanosecs(Forth* pForth);
  static void vTimerNew(Forth* pForth);
  static void vTimerReset(Forth* pForth);
  static void vTimerSecs(Forth* pForth);
  static void vTimerStart(Forth* pForth);
  static void vTimerTicks(Forth* pForth);
  static void vTimerUpdate(Forth* pForth);

  static void vTimeCurrent(Forth* pForth);

  // to
  static void vToBody(Forth* pForth);
  static void vToInt(Forth* pForth);
  static void vToPtr(Forth* pForth);
  static void vToStr(Forth* pForth);
  static void vToTask(Forth* pForth);

  // token
  static void vTokenColon(Forth* pForth);

  // trace
  static void vTraceOff(Forth* pForth);
  static void vTraceOn(Forth* pForth);
  static void vTracePoint(Forth* pForth);
  static void vTraceSet(Forth* pForth);

  // tx
  static void vTxBegin(Forth* pForth);
  static void vTxCommit(Forth* pForth);
  static void vTxCounterDec(Forth* pForth);
  static void vTxCounterFetch(Forth* pForth);
  static void vTxCounterStore(Forth* pForth);
  static void vTxDistMemberFetch(Forth* pForth);
  static void vTxDistMemberStore(Forth* pForth);
  static void vTxDump(Forth* pForth);
  static void vTxDumpAll(Forth* pForth);
  static void vTxFree(Forth* pForth);
  static void vTxId(Forth* pForth);
  static void vTxPrepare(Forth* pForth);
  static void vTxRollback(Forth* pForth);
  static void vTxTestDist(Forth* pForth);

  // txCollection
  static void vTxCollectionDump(Forth* pForth);
  static void vTxCollectionInit(Forth* pForth);

  // txCursor
  static void vTxCursorBegin(Forth* pForth);
  static void vTxCursorDelete(Forth* pForth);
  static void vTxCursorEnd(Forth* pForth);
  static void vTxCursorFirst(Forth* pForth);
  static void vTxCursorGet(Forth* pForth);
  static void vTxCursorInit(Forth* pForth);
  static void vTxCursorNew(Forth* pForth);
  static void vTxCursorNext(Forth* pForth);

  // txMap
  static void vTxMapInit(Forth* pForth);

  // txRef
  static void vTxRefBegin(Forth* pForth);
  static void vTxRefCheckDistId(Forth* pForth);
  static void vTxRefCommit(Forth* pForth);
  static void vTxRefDump(Forth* pForth);
  static void vTxRefEnd(Forth* pForth);
  static void vTxRefId(Forth* pForth);
  static void vTxRefInit(Forth* pForth);
  static void vTxRefNew(Forth* pForth);
  static void vTxRefRollback(Forth* pForth);

  // utility
  static void vUtilityPrefetch(Forth* pForth);
  static void vUtilitySchemaTest(Forth* pForth);

  // vector
  static void vVectorDifference(Forth* pForth);

  // word
  static void vWord(Forth* pForth);
  static void vWordCategoryFetch(Forth* pForth);
  static void vWordCategoryStore(Forth* pForth);
  static void vWordDump(Forth* pForth);
  static void vWordEnmSetFetch(Forth* pForth);
  static void vWordEnmSetStore(Forth* pForth);
  static void vWordExitHookGet(Forth* pForth);
  static void vWordExitHookSet(Forth* pForth);
  static void vWordForget(Forth* pForth);
  static void vWordImmediateGet(Forth* pForth);
  static void vWordImmediateSet(Forth* pForth);
  static void vWordIsImmediate(Forth* pForth);
  static void vWordIsIntrinsic(Forth* pForth);
  static void vWordKindFetch(Forth* pForth);
  static void vWordKindStore(Forth* pForth);
  static void vWordNameFetch(Forth* pForth);
  static void vWordNameStore(Forth* pForth);
  static void vWordPrev(Forth* pForth);

  // zero
  static void vZeroLess(Forth* pForth);
  static void vZeroLessEqual(Forth* pForth);
  static void vZeroEqual(Forth* pForth);
  static void vZeroNotEqual(Forth* pForth);
  static void vZeroGreaterEqual(Forth* pForth);
  static void vZeroGreater(Forth* pForth);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
