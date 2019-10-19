/*============================================================================*/
/* StepsSet.hpp - the set of steps                                            */
/*============================================================================*/
#pragma once

public:

  // access path
  void vAccessPathTableFetch(Forth* pForth);
  void vAccessPathTableStore(Forth* pForth);

  // accum
  void vAccumDelete(Forth* pForth);
  void vAccumNew(Forth* pForth);

  void vAccumAccumBuffer(Forth* pForth);
  void vAccumCountFetch(Forth* pForth);
  void vAccumCountStore(Forth* pForth);
  void vAccumGroupFetch(Forth* pForth);
  void vAccumGroupStore(Forth* pForth);
  void vAccumMaxFetch(Forth* pForth);
  void vAccumMaxStore(Forth* pForth);
  void vAccumMinFetch(Forth* pForth);
  void vAccumMinStore(Forth* pForth);
  void vAccumSumFetch(Forth* pForth);
  void vAccumSumStore(Forth* pForth);

  // address
  void vAddressEqual(Forth* pForth);
  void vAddressFetch(Forth* pForth);
  void vAddressStore(Forth* pForth);

  // apiState
  void vApiStateIncBegun(Forth* pForth);
  void vApiStateIncCancelled(Forth* pForth);
  void vApiStateIncEnded(Forth* pForth);
  void vApiStateIncExecuted(Forth* pForth);
  void vApiStateSenLoc(Forth* pForth);
  void vApiStateTraceSession(Forth* pForth);

  // args
  void vArgsOscillate(Forth* pForth);
  void vArgsPrint(Forth* pForth);

  // article
  void vArticleDump(Forth* pForth);

  // assert
  void vAssert(Forth* pForth);

  void vAssertClass(Forth* pForth);
  void vAssertClass_0(Forth* pForth);
  void vAssertClass_1(Forth* pForth);
  void vAssertClass_2(Forth* pForth);
  void vAssertClass_3(Forth* pForth);
  void vAssertClass_4(Forth* pForth);

  void vAssertStack_0(Forth* pForth);
  void vAssertStack_0more(Forth* pForth);
  void vAssertStack_1(Forth* pForth);
  void vAssertStack_1more(Forth* pForth);
  void vAssertStack_2(Forth* pForth);
  void vAssertStack_2more(Forth* pForth);
  void vAssertStack_3(Forth* pForth);
  void vAssertStack_3more(Forth* pForth);
  void vAssertStack_4(Forth* pForth);
  void vAssertStack_4more(Forth* pForth);

  // astack
  void vAstackClass(Forth* pForth);
  void vAstackDepth(Forth* pForth);
  void vAstackDrop(Forth* pForth);
  void vAstackDup(Forth* pForth);
  void vAstackFetch(Forth* pForth);
  void vAstackNip(Forth* pForth);
  void vAstackPick(Forth* pForth);
  void vAstackReverse(Forth* pForth);
  void vAstackStk(Forth* pForth);
  void vAstackSwap(Forth* pForth);
  void vAstackToA(Forth* pForth);
  void vAstackToD(Forth* pForth);
  void vAstackTransfer(Forth* pForth);
  void vAstack0(Forth* pForth);
  void vAstack1(Forth* pForth);
  void vAstack2(Forth* pForth);
  void vAstack3(Forth* pForth);
  void vAstack4(Forth* pForth);
  void vAstack5(Forth* pForth);
  void vAstack6(Forth* pForth);
  void vAstack7(Forth* pForth);
  void vAstack8(Forth* pForth);
  void vAstack9(Forth* pForth);

  // attachmap
  void vAttachMapTenantsFetch(Forth* pForth);

  // bp
  void vBpAnalyze(Forth* pForth);
  void vBpDump(Forth* pForth);
  void vBpEpochDump(Forth* pForth);
  void vBpEpochTake(Forth* pForth);
  void vBpReadBufferPool(Forth* pForth);
  void vBpWriteBufferPool(Forth* pForth);
  void vBpFreeBuffers(Forth* pForth);
  void vBpFreePercent(Forth* pForth);
  void vBpFreeSpace(Forth* pForth);
  void vBpInUseSpace(Forth* pForth);
  void vBpTotalSpace(Forth* pForth);

  // cast
  void vCastInt(Forth* pForth);
  void vCastPtr(Forth* pForth);

  // class
  void vClassName(Forth* pForth);

  // column
  void vColumnIsNullableFetch(Forth* pForth);
  void vColumnIsPartitionKeyFetch(Forth* pForth);
  void vColumnLengthFetch(Forth* pForth);
  void vColumnNameFetch(Forth* pForth);
  void vColumnScaleFetch(Forth* pForth);
  void vColumnTypeFetch(Forth* pForth);

  // command processor
  void vCommandBlankLine(Forth* pForth);
  void vCommandDeleteJob(Forth* pForth);
  void vCommandExecuteFile(Forth* pForth);
  void vCommandFindJobClass(Forth* pForth);
  void vCommandGetCommand(Forth* pForth);
  void vCommandJobClass(Forth* pForth);
  void vCommandPostJob(Forth* pForth);
  void vCommandPreJob(Forth* pForth);
  void vCommandSpawnJob(Forth* pForth);

  // comment
  void vComment(Forth* pForth);

  // comment
  void vCompilerCurrent(Forth* pForth);

  // cpu
  void vCpuRdtscp(Forth* pForth);

  // cursor
  void vCursorFetch(Forth* pForth);
  void vCursorStore(Forth* pForth);

  // ddllock
  void vDdlLockDropRead(Forth* pForth);
  void vDdlLockDropWrite(Forth* pForth);
  void vDdlLockGrabRead(Forth* pForth);
  void vDdlLockGrabWrite(Forth* pForth);
  void vDdlLockTestDrop(Forth* pForth);

  // debug
  void vDebugTrap(Forth* pForth);

  // dic
  void vDicInit(Forth* pForth);
  void vDicKindGet(Forth* pForth);
  void vDicKindSet(Forth* pForth);
  void vDicLastWord(Forth* pForth);
  void vDicName(Forth* pForth);
  void vDicRef(Forth* pForth);
  void vDicRoot(Forth* pForth);
  void vDicWordCount(Forth* pForth);

  // dicLock
  void vDicLockDropWrite(Forth* pForth);
  void vDicLockGrabWrite(Forth* pForth);

  // dicPath
  void vDicPathDepth(Forth* pForth);
  void vDicPathDump(Forth* pForth);
  void vDicPathPick(Forth* pForth);

  // dicStack
  void vDicStackDepth(Forth* pForth);
  void vDicStackDrop(Forth* pForth);
  void vDicStackDump(Forth* pForth);
  void vDicStackPick(Forth* pForth);
  void vDicStackPop(Forth* pForth);
  void vDicStackPush(Forth* pForth);

  // dir
  void vDirChangeCurrent(Forth* pForth);
  void vDirGetCurrent(Forth* pForth);
  void vDirExists(Forth* pForth);
  void vDirMake(Forth* pForth);
  void vDirRemove(Forth* pForth);

  void vDirClose(Forth* pForth);
  void vDirDelete(Forth* pForth);
  void vDirEntryName(Forth* pForth);
  void vDirEntryType(Forth* pForth);
  void vDirIsClosed(Forth* pForth);
  void vDirIsOpen(Forth* pForth);
  void vDirNew(Forth* pForth);
  void vDirOpen(Forth* pForth);
  void vDirPathGet(Forth* pForth);
  void vDirRead(Forth* pForth);
  void vDirRewind(Forth* pForth);
  void vDirSeek(Forth* pForth);
  void vDirTell(Forth* pForth);

  // docol
  void vDocolCompiling(Forth* pForth);
  void vDocolInterpreting(Forth* pForth);
  void vDocolIsCompiling(Forth* pForth);
  void vDocolIsInterpreting(Forth* pForth);

  // dot
  void vDot(Forth* pForth);
  void vDotPut(Forth* pForth);
  void vDotNewline(Forth* pForth);
  void vDotVerbose(Forth* pForth);

  // dstack
  void vDstackClass(Forth* pForth);                 //! DUP OF class@
  void vDstackDepth(Forth* pForth);
  void vDstackDrop(Forth* pForth);
  void vDstackDropTwo(Forth* pForth);
  void vDstackDup(Forth* pForth);
  void vDstackDupNonZero(Forth* pForth);
  void vDstackNip(Forth* pForth);
  void vDstackOver(Forth* pForth);
  void vDstackPick(Forth* pForth);
  void vDstackReverse(Forth* pForth);
  void vDstackRoll(Forth* pForth);
  void vDstackRollInv(Forth* pForth);
  void vDstackRot(Forth* pForth);
  void vDstackRotInv(Forth* pForth);
  void vDstackStk(Forth* pForth);
  void vDstackSwap(Forth* pForth);
  void vDstackTransfer(Forth* pForth);
  void vDstackTuck(Forth* pForth);

  // encode
  void vEncodeAgain(Forth* pForth);
  void vEncodeAhead(Forth* pForth);
  void vEncodeContinue(Forth* pForth);
  void vEncodeHere(Forth* pForth);
  void vEncodeIf(Forth* pForth);
  void vEncodeIfAgain(Forth* pForth);
  void vEncodeLoop(Forth* pForth);
  void vEncodeLoopPlus(Forth* pForth);
  void vEncodeNext(Forth* pForth);
  void vEncodeThen(Forth* pForth);
  void vEncodeUntil(Forth* pForth);

  // ename-set
  void vEnmSetAddInt(Forth* pForth);
  void vEnmSetAddStr(Forth* pForth);
  void vEnmSetAsStr(Forth* pForth);
  void vEnmSetGetStr(Forth* pForth);
  void vEnmSetHasName(Forth* pForth);
  void vEnmSetNew(Forth* pForth);

  // enter
  void vEnterObject(Forth* pForth);
  void vEnterRoutine(Forth* pForth);

  // entry
  void vEntryMatches(Forth* pForth);
  void vEntrySame(Forth* pForth);
  void vEntryToDataRow(Forth* pForth);

  // entry buffer
  void vEntBufferDump(Forth* pForth);
  void vEntBufferIndexFetch(Forth* pForth);
  void vEntBufferIndexStore(Forth* pForth);
  void vEntBufferIsLastFetch(Forth* pForth);
  void vEntBufferIsLastStore(Forth* pForth);
  void vEntBufferNextBuffer(Forth* pForth);
  void vEntBufferPartitionFetch(Forth* pForth);
  void vEntBufferPartitionStore(Forth* pForth);
  void vEntBufferToEntBuffer(Forth* pForth);

  // env
  void vEnvGet(Forth* pForth);

  // errors
  void vErrorMsg(Forth* pForth);

  // file
  void vFileClose(Forth* pForth);
  void vFileDelete(Forth* pForth);
  void vFileNew(Forth* pForth);
  void vFileOpen(Forth* pForth);
  void vFilePathGet(Forth* pForth);
  void vFileRead(Forth* pForth);
  void vFileReadRecord(Forth* pForth);
  void vFileWrite(Forth* pForth);

  // flow
  void vFlowBegin_(Forth* pForth);
  void vFlowExecute(Forth* pForth);
  void vFlowHazard(Forth* pForth);
  void vFlowJump(Forth* pForth);
  void vFlowJumpBegin_(Forth* pForth);
  void vFlowJumpFalse(Forth* pForth);
  void vFlowJumpTrue(Forth* pForth);
  void vFlowNop(Forth* pForth);
  void vFlowRetryPoint(Forth* pForth);
  void vFlowTest(Forth* pForth);
  void vFlowThrow(Forth* pForth);

  // forth
  void vAllot(Forth* pForth);
  void vCell(Forth* pForth);
  void vColon(Forth* pForth);
  void vComma(Forth* pForth);
  void vCompileComma(Forth* pForth);
  void vCreate(Forth* pForth);
  void vCreateFrom(Forth* pForth);
  void vCreateFromInRoot(Forth* pForth);
  void vCreateFromPersistent(Forth* pForth);
  void vCreatePersistent(Forth* pForth);
  void vCreateTenants(Forth* pForth);
  void vDoes(Forth* pForth);
  void vDoes_RT(Forth* pForth);
  void vExit_RT(Forth* pForth);
  void vFind(Forth* pForth);
  void vFindInCurrent(Forth* pForth);
  void vForget(Forth* pForth);
  void vImmediate(Forth* pForth);
  void vLocate(Forth* pForth);
  void vPostpone(Forth* pForth);
  void vPrompt(Forth* pForth);
  void vSee(Forth* pForth);
  void vSemicolon(Forth* pForth);
  void vSeriesName(Forth* pForth);
  void vSubDic(Forth* pForth);
  void vTick(Forth* pForth);

  // grid
  void vGridConnectedNodes(Forth* pForth);
  void vGridDefinedNodes(Forth* pForth);
  void vGridFindNode(Forth* pForth);
  void vGridIncConnectedNodes(Forth* pForth);
  void vGridNode(Forth* pForth);
  void vGridPartitions(Forth* pForth);
  void vGridShow(Forth* pForth);
  void vGridSlots(Forth* pForth);
  void vGridStartedStore(Forth* pForth);
  void vGridStartedFetch(Forth* pForth);
  void vGridStartNodeStore(Forth* pForth);
  void vGridStartNodeFetch(Forth* pForth);
  void vGridThisNodeFetch(Forth* pForth);
  void vGridThisNodeNameFetch(Forth* pForth);
  void vGridToNode(Forth* pForth);
  void vGridToPartition(Forth* pForth);
  void vGridToSlot(Forth* pForth);

  // group by
  void vGroupByApplyBuffer(Forth* pForth);
  void vGroupByApplyEntry(Forth* pForth);
  void vGroupByApplyPartial(Forth* pForth);
  void vGroupByDelete(Forth* pForth);
  void vGroupByEnd(Forth* pForth);
  void vGroupByNew(Forth* pForth);
  void vGroupByStart(Forth* pForth);

  // handler
  void vHandlerServiceSleepQueue(Forth* pForth);

  // index
  void vIndexAnalyze(Forth* pForth);
  void vIndexColumnFetch(Forth* pForth);
  void vIndexColumnCountFetch(Forth* pForth);
  void vIndexColumnName(Forth* pForth);
  void vIndexColumnType(Forth* pForth);
  void vIndexCompact(Forth* pForth);
  void vIndexCreateColumnVec(Forth* pForth);
  void vIndexDump(Forth* pForth);
  void vIndexInit(Forth* pForth);
  void vIndexIsPrimaryKeyFetch(Forth* pForth);
  void vIndexIsPrimaryKeyStore(Forth* pForth);
  void vIndexIsUniqueFetch(Forth* pForth);
  void vIndexIsUniqueStore(Forth* pForth);
  void vIndexIsValidUniqueIndex(Forth* pForth);
  void vIndexLowerBound(Forth* pForth);
  void vIndexNameFetch(Forth* pForth);
  void vIndexPopulate(Forth* pForth);
  void vIndexRange(Forth* pForth);
  void vIndexShowColumnVec(Forth* pForth);
  void vIndexSpace(Forth* pForth);
  void vIndexSubDic(Forth* pForth);
  void vIndexTableFetch(Forth* pForth);
  void vIndexTableStore(Forth* pForth);
  void vIndexTruncate(Forth* pForth);
  void vIndexUpperBound(Forth* pForth);
  void vIndexVerify(Forth* pForth);
  void vIndexVisualize(Forth* pForth);

  // input
  void vInputClose(Forth* pForth);
  void vInputDepth(Forth* pForth);
  void vInputNext(Forth* pForth);
  void vInputOpen(Forth* pForth);
  void vInputPop(Forth* pForth);
  void vInputPush(Forth* pForth);
  void vInputStdin(Forth* pForth);
  void vInputText(Forth* pForth);
  void vInputToken(Forth* pForth);
  void vInputType(Forth* pForth);
  void vInputUnknown(Forth* pForth);

  // int
  void vIntAdd(Forth* pForth);
  void vIntDivide(Forth* pForth);
  void vIntEqual(Forth* pForth);
  void vIntGreater(Forth* pForth);
  void vIntGreaterEqual(Forth* pForth);
  void vIntHash(Forth* pForth);
  void vIntHo16(Forth* pForth);
  void vIntLess(Forth* pForth);
  void vIntLessEqual(Forth* pForth);
  void vIntModulo(Forth* pForth);
  void vIntMultiply(Forth* pForth);
  void vIntNeg(Forth* pForth);
  void vIntNotEqual(Forth* pForth);
  void vIntRandom(Forth* pForth);
  void vIntSubtract(Forth* pForth);
  void vUnsignedIntRandom(Forth* pForth);

  // jobClass
  void vJobClassBuffer(Forth* pForth);
  void vJobClassFind(Forth* pForth);
  void vJobClassName(Forth* pForth);
  void vJobClassNew(Forth* pForth);

  void vJobClassSeedRun(Forth* pForth);
  void vJobClassSeedReq(Forth* pForth);
  void vJobClassSeedRcv(Forth* pForth);
  void vJobClassSeedExe(Forth* pForth);
  void vJobClassSeedEnd(Forth* pForth);
  void vJobClassSeedCan(Forth* pForth);

  // job
  void vJobAcksRemaining(Forth* pForth);
  void vJobAddOriginator(Forth* pForth);
  void vJobAddParticipant(Forth* pForth);
  void vJobAssertBegin(Forth* pForth);
  void vJobAssertNeutral(Forth* pForth);
  void vJobAssertOriginator(Forth* pForth);
  void vJobAssertParticipant(Forth* pForth);
  void vJobAssertReceived(Forth* pForth);

  // job mark 2
  void vJobMark2IndexStore(Forth* pForth);
  void vJobMark2TableNameFetch(Forth* pForth);
  void vJobMark2TableStore(Forth* pForth);

  void vJobCatalogNameFetch(Forth* pForth);
  void vJobClass(Forth* pForth);
  void vJobColumnCountFetch(Forth* pForth);
  void vJobColumnNameFetch(Forth* pForth);
  void vJobColumnIsPartKey(Forth* pForth);
  void vJobColumnTypeFetch(Forth* pForth);
  void vJobCompleted(Forth* pForth);
  void vJobCreateResponseVec(Forth* pForth);
  void vJobOperandFetch(Forth* pForth);
  void vJobOperandStore(Forth* pForth);

  void vJobDecAcksRemaining(Forth* pForth);
  void vJobDelete(Forth* pForth);
  void vJobDicPath(Forth* pForth);

  void vJobEnameFetch(Forth* pForth);
  void vJobEnameStore(Forth* pForth);
  void vJobErrorStore(Forth* pForth);
  void vJobErrorFetch(Forth* pForth);
  void vJobExecuteFetch(Forth* pForth);

  void vJobFindOriginator(Forth* pForth);
  void vJobFindParticipant(Forth* pForth);
  void vJobFolderNameFetch(Forth* pForth);

  void vJobHasError(Forth* pForth);
  void vJobHasOperand(Forth* pForth);

  void vJobIdFetch(Forth* pForth);
  void vJobIndexNameFetch(Forth* pForth);
  void vJobIsExecuting(Forth* pForth);

  void vJobJobClass(Forth* pForth);
  void vJobJobListFetch(Forth* pForth);

  void vJobMetadataStore(Forth* pForth);

  void vJobNodeNameFetch(Forth* pForth);
  void vJobNoWaitFetch(Forth* pForth);

  void vJobObjectNameFetch(Forth* pForth);
  void vJobOriginatingTaskFetch(Forth* pForth);
  void vJobOriginatingTaskStore(Forth* pForth);
  void vJobOriginatorFetch(Forth* pForth);

  void vJobParentIdFetch(Forth* pForth);
  void vJobParticipantFetch(Forth* pForth);
  void vJobParticipantStore(Forth* pForth);
  void vJobParticipantResponseStore(Forth* pForth);
  void vJobParticipantHasError(Forth* pForth);
  void vJobPartitionsFetch(Forth* pForth);
  void vJobPhaseFetch(Forth* pForth);
  void vJobPhaseStore(Forth* pForth);
  void vJobPostRecv(Forth* pForth);
  void vJobPreSend(Forth* pForth);

  void vJobRecv(Forth* pForth);
  void vJobRegionSizeFetch(Forth* pForth);
  void vJobRemoveOriginator(Forth* pForth);
  void vJobRemoveParticipant(Forth* pForth);
  void vJobReseed(Forth* pForth);
  void vJobResponseStore(Forth* pForth);
  void vJobResponseFetch(Forth* pForth);
  void vJobRoutineNameFetch(Forth* pForth);
  void vJobRowsInvalidFetch(Forth* pForth);
  void vJobRowsInvalidStore(Forth* pForth);
  void vJobRowsProcessedFetch(Forth* pForth);
  void vJobRowsProcessedStore(Forth* pForth);
  void vJobRowsSkippedFetch(Forth* pForth);
  void vJobRowsSkippedStore(Forth* pForth);

  void vJobSchemaNameFetch(Forth* pForth);
  void vJobSeedFetch(Forth* pForth);
  void vJobSend(Forth* pForth);
  void vJobSessionStateFetch(Forth* pForth);
  void vJobShow(Forth* pForth);
  void vJobSignalWaiter(Forth* pForth);
  void vJobSlotFetch(Forth* pForth);
  void vJobSpawn(Forth* pForth);
  void vJobSpawnDedicated(Forth* pForth);
  void vJobStateDiagram(Forth* pForth);

  void vJobTaskFetch(Forth* pForth);
  void vJobTaskStore(Forth* pForth);
  void vJobTenantNameFetch(Forth* pForth);
  void vJobThis(Forth* pForth);

  void vJobWaitParticipant(Forth* pForth);

  // job generic
  void vJob__BegunNew(Forth* pForth);
  void vJob__CancelNew(Forth* pForth);
  void vJob__CancelledNew(Forth* pForth);
  void vJob__EndNew(Forth* pForth);
  void vJob__EndedNew(Forth* pForth);
  void vJob__ExecuteNew(Forth* pForth);
  void vJob__ExecutedNew(Forth* pForth);

  // job analyzeIndex
  void vJob_AnalyzeIndexIsDetailed(Forth* pForth);
  void vJob_AnalyzeTableIsDetailed(Forth* pForth);

  // job Cat
  void vJob_CatBegin(Forth* pForth);
  void vJob_CatChunk(Forth* pForth);
  void vJob_CatEnd(Forth* pForth);

  // job CreateIndex
  void vJob_CreateIndexIsUniqueFetch(Forth* pForth);

  // job Dynamic
  void vJob_DynamicBegin(Forth* pForth);
  void vJob_DynamicEnd(Forth* pForth);
  void vJob_DynamicExecute(Forth* pForth);

  // job Export
  void vJob_ExportAuxAckDecidedFetch(Forth* pForth);
  void vJob_ExportAuxAckDecidedStore(Forth* pForth);
  void vJob_ExportBegin(Forth* pForth);
  void vJob_ExportEnd(Forth* pForth);
  void vJob_ExportTableStore(Forth* pForth);

  // job FetchRows
  void vJob_FetchRowsColumnNameFetch(Forth* pForth);
  void vJob_FetchRowsColumnStore(Forth* pForth);
  void vJob_FetchRowsLastBuffersStore(Forth* pForth);
  void vJob_FetchRowsPartitionFetch(Forth* pForth);
  void vJob_FetchRowsPlannerStore(Forth* pForth);
  void vJob_FetchRowsSpawnSub(Forth* pForth);
  void vJob_FetchRowsSpawnSubInflate(Forth* pForth);
  void vJob_FetchRowsTableStore(Forth* pForth);
  void vJob_FetchRowsWhereFetch(Forth* pForth);

  // job GenData
  void vJob_GenerateGenerate(Forth* pForth);
  void vJob_GenerateTableStore(Forth* pForth);

  // job IndexJoin
  void vJob_IndexJoinLastBuffersStore(Forth* pForth);
  void vJob_IndexJoinLeftIndexStore(Forth* pForth);
  void vJob_IndexJoinLeftTableStore(Forth* pForth);
  void vJob_IndexJoinRightIndexStore(Forth* pForth);
  void vJob_IndexJoinRightIndexNameFetch(Forth* pForth);
  void vJob_IndexJoinRightTableStore(Forth* pForth);
  void vJob_IndexJoinRightTableNameFetch(Forth* pForth);
  void vJob_IndexJoinSpawnSub(Forth* pForth);
  void vJob_SubIndexJoinIndexJoin(Forth* pForth);

  // job inflate
  void vJob_InflateRowsColumnNameFetch(Forth* pForth);
  void vJob_InflateRowsColumnStore(Forth* pForth);
  void vJob_InflateRowsSpawnSub(Forth* pForth);
  void vJob_InflateRowsSpawnSubInflate(Forth* pForth);
  void vJob_InflateRowsSubInfTasksFetch(Forth* pForth);
  void vJob_InflateRowsTableStore(Forth* pForth);
  void vJob_InflateRowsWhereFetch(Forth* pForth);

  // job Ingest
  void vJob_IngestBegin(Forth* pForth);
  void vJob_IngestChunk(Forth* pForth);
  void vJob_IngestEnd(Forth* pForth);
  void vJob_IngestTableStore(Forth* pForth);

  // job InsertRows
  void vJob_CopyBufferCopy(Forth* pForth);

  void vJob_InsertRowPartitionFetch(Forth* pForth);
  void vJob_InsertRowRecordStore(Forth* pForth);
  void vJob_InsertRowNewJobWriteRow(Forth* pForth);
  void vJob_InsertRowSpawnJobWriteRow(Forth* pForth);

  void vJob_InsertTableSubInserterJobFetch(Forth* pForth);

  void vJob_SubInserterBegin(Forth* pForth);
  void vJob_SubInserterEnd(Forth* pForth);

  // job KidExport
  void vJob_KidExportExportOrigin(Forth* pForth);
  void vJob_KidExportExportJobId(Forth* pForth);
  void vJob_KidExportWriteExportBuffer(Forth* pForth);

  // job LoadTable
  void vJob_LoadTableBegin(Forth* pForth);
  void vJob_LoadTableChunk(Forth* pForth);
  void vJob_LoadTableDumpMap(Forth* pForth);
  void vJob_LoadTableEnd(Forth* pForth);
  void vJob_LoadTableFilePathFetch(Forth* pForth);
  void vJob_LoadTableTableStore(Forth* pForth);

  // job Monitor
  void vJob_MonitorBegin(Forth* pForth);
  void vJob_MonitorTake(Forth* pForth);
  void vJob_MonitorEnd(Forth* pForth);

  // job NetHog
  void vJob_NetHogHogNoFetch(Forth* pForth);

  // job NodeInfo
  void vJob_NodeInfoNodeNameFetch(Forth* pForth);

  // job publish
  void vJob_PublishSpawnSub(Forth* pForth);
  void vJob_PublishTableStore(Forth* pForth);

  // job Purge
  void vJob_PurgeSpawnSub(Forth* pForth);
  void vJob_PurgeTableStore(Forth* pForth);

  // job Query
  void vJob_QueryBegin(Forth* pForth);
  void vJob_QueryEnd(Forth* pForth);
  void vJob_QueryExecute(Forth* pForth);

  // job Receiver
  void vJob_ReceiverConnNoFetch(Forth* pForth);
  void vJob_ReceiverConnNoStore(Forth* pForth);
  void vJob_ReceiverJobReceived(Forth* pForth);
  void vJob_ReceiverNodeFetch(Forth* pForth);
  void vJob_ReceiverNodeStore(Forth* pForth);



  // job Repeater
  void vJob_RepeaterBegin(Forth* pForth);
  void vJob_RepeaterChunk(Forth* pForth);
  void vJob_RepeaterEnd(Forth* pForth);

  // job Report
  void vJob_ReportBegin(Forth* pForth);
  void vJob_ReportTake(Forth* pForth);
  void vJob_ReportEnd(Forth* pForth);

  // job Select
  void vJob_SelectBegin(Forth* pForth);
  void vJob_SelectEnd(Forth* pForth);
  void vJob_SelectExecute(Forth* pForth);

  // job SelectTable
  void vJob_SelectTablePartitionFetch(Forth* pForth);
  void vJob_SelectTableTableStore(Forth* pForth);

  // job Sink
  void vJob_SinkBegin(Forth* pForth);
  void vJob_SinkChunk(Forth* pForth);
  void vJob_SinkEnd(Forth* pForth);

  // job Sort
  void vJob_SortBegin(Forth* pForth);
  void vJob_SortEnd(Forth* pForth);
  void vJob_SortExecute(Forth* pForth);

  // job SubExporter
  void vJob_SubExporterBegin(Forth* pForth);
  void vJob_SubExporterChunk(Forth* pForth);
  void vJob_SubExporterEnd(Forth* pForth);
  void vJob_SubExporterSpawn(Forth* pForth);
  void vJob_SubExporterExportJobFetch(Forth* pForth);

  // job SubFetch
  void vJob_SubFetchBufferQueueFetch(Forth* pForth);
  void vJob_SubFetchPhraseFetch(Forth* pForth);
  void vJob_SubFetchPlannerFetch(Forth* pForth);
  void vJob_SubFetchSlotFetch(Forth* pForth);

  // job SubInflate
  void vJob_SubInflateJobInflateRowsFetch(Forth* pForth);
  void vJob_SubInflateInflateRows(Forth* pForth);

  // job SubIngester
  void vJob_SubIngesterBegin(Forth* pForth);
  void vJob_SubIngesterChunk(Forth* pForth);
  void vJob_SubIngesterEnd(Forth* pForth);
  void vJob_SubIngesterJobIngest(Forth* pForth);
  void vJob_SubIngesterSpawn(Forth* pForth);

  // job SubLoader
  void vJob_SubLoaderBegin(Forth* pForth);
  void vJob_SubLoaderChunk(Forth* pForth);
  void vJob_SubLoaderEnd(Forth* pForth);
  void vJob_SubLoaderSpawn(Forth* pForth);
  void vJob_SubLoaderTerminate(Forth* pForth);

  // job SubPublish
  void vJob_SubPublishBegin(Forth* pForth);
  void vJob_SubPublishChunk(Forth* pForth);
  void vJob_SubPublishEnd(Forth* pForth);
  void vJob_SubPublishSlotFetch(Forth* pForth);
  void vJob_SubPublishTableStore(Forth* pForth);
  void vJob_SubPublishPublishJobIdFetch(Forth* pForth);

  // job SubPurger
  void vJob_SubPurgerBegin(Forth* pForth);
  void vJob_SubPurgerChunk(Forth* pForth);
  void vJob_SubPurgerEnd(Forth* pForth);

  void vJob_SubPurgerSlotFetch(Forth* pForth);
  void vJob_SubPurgerPurgeJobIdFetch(Forth* pForth);

  // job SubScan
  void vJob_SubScanBufferQueueFetch(Forth* pForth);
  void vJob_SubScanSlotFetch(Forth* pForth);

  // Job SubTruncate
  void vJob_SubTruncateSlotFetch(Forth* pForth);
  void vJob_SubTruncateTruncateJobIdFetch(Forth* pForth);

  // job SubWalker
  void vJob_SubWalkerBegin(Forth* pForth);
  void vJob_SubWalkerChunk(Forth* pForth);
  void vJob_SubWalkerEnd(Forth* pForth);
  void vJob_SubWalkerWalkJobFetch(Forth* pForth);
  void vJob_SubWalkerSpawn(Forth* pForth);

  // Job Truncate
  void vJob_TruncateTableSpawnSub(Forth* pForth);

  // job Walk
  void vJob_WalkBegin(Forth* pForth);
  void vJob_WalkEnd(Forth* pForth);
  void vJob_WalkIndexStore(Forth* pForth);
  void vJob_WalkTableStore(Forth* pForth);

  // Job WriteBuffer
  void vJob_WriteBufferParentJobIdFetch(Forth* pForth);
  void vJob_WriteBufferInsertRows(Forth* pForth);
  void vJob_WriteBufferIsIngesting(Forth* pForth);
  void vJob_WriteBufferIsInserting(Forth* pForth);
  void vJob_WriteBufferIsLoading(Forth* pForth);
  void vJob_WriteBufferPartitionFetch(Forth* pForth);
  void vJob_WriteBufferTableStore(Forth* pForth);

  // Job WriteRow
  void vJob_WriteRowParentJobIdFetch(Forth* pForth);
  void vJob_WriteRowEncodeRow(Forth* pForth);
  void vJob_WriteRowInsertRow(Forth* pForth);
  void vJob_WriteRowPartitionFetch(Forth* pForth);
  void vJob_WriteRowTableStore(Forth* pForth);

  // Job Operand
  void vJob_OperandPipeOut(Forth* pForth);

  // load table
  void vLoadTableChunkFile(Forth* pForth);
  void vLoadTableDelete(Forth* pForth);
  void vLoadTableDumpMap(Forth* pForth);
  void vLoadTableNew(Forth* pForth);
  void vLoadTableTableStore(Forth* pForth);

  // locals
  void vLocalFetch(Forth* pForth);
  void vLocalStore(Forth* pForth);
  void vLocalsBar(Forth* pForth);
  void vLocalsPop(Forth* pForth);
  void vLocalsPush(Forth* pForth);
  void vLocalsTo(Forth* pForth);

  // lock
  void vLockDropRead(Forth* pForth);
  void vLockDropWrite(Forth* pForth);
  void vLockGrabRead(Forth* pForth);
  void vLockGrabWrite(Forth* pForth);
  void vLockInit(Forth* pForth);

  // locks
  void vLockTrackerList(Forth* pForth);
  void vLockTrackerReset(Forth* pForth);

  // log
  void vLogDump(Forth* pForth);
  void vLogMark(Forth* pForth);
  void vLogMarkEntry(Forth* pForth);
  void vLogMarkExit(Forth* pForth);
  void vLogMarkPhase(Forth* pForth);

  void vLogMarkWord(Forth* pForth);
  void vLogMarkSeed(Forth* pForth);

  void vLogJobSeed(Forth* pForth);

  // logic
  void vLogicAnd(Forth* pForth);
  void vLogicNot(Forth* pForth);
  void vLogicOr(Forth* pForth);

  // magnitude
  void vBytesMagnitude(Forth* pForth);
  void vNumberMagnitude(Forth* pForth);
  void vToDigitName(Forth* pForth);

  // memory
  void vMemoryByteDump(Forth* pForth);
  void vMemoryReset(Forth* pForth);
  void vMemoryStatistics(Forth* pForth);
  void vMemoryWordDump(Forth* pForth);

  // node (of grid)
  void vNodeForget(Forth* pForth);
  void vNodeIP(Forth* pForth);
  void vNodeName(Forth* pForth);
  void vNodeNew(Forth* pForth);
  void vNodeNo(Forth* pForth);
  void vNodePartitions(Forth* pForth);
  void vNodePort(Forth* pForth);
  void vNodeRecvSocketStore(Forth* pForth);
  void vNodeRecvSocketFetch(Forth* pForth);
  void vNodeSendSocketStore(Forth* pForth);
  void vNodeSendSocketFetch(Forth* pForth);
  void vNodeSize(Forth* pForth);
  void vNodeStartNode(Forth* pForth);
  void vNodeStateStore(Forth* pForth);
  void vNodeStateFetch(Forth* pForth);
  void vNodeStatus(Forth* pForth);

  // object
  void vObjectClassFetch(Forth* pForth);
  void vObjectClassStore(Forth* pForth);
  void vObjectDump(Forth* pForth);
  void vObjectFetch(Forth* pForth);
  void vObjectMagicMark(Forth* pForth);
  void vObjectStore(Forth* pForth);
  void vObjectWordBodySet(Forth* pForth);

  // persistence
  void vPersistenceCheckpoint(Forth* pForth);
  void vPersistenceReload(Forth* pForth);

  // plan
  void vPlanCompile(Forth* pForth);
  void vPlanDelete(Forth* pForth);
  void vPlanDic(Forth* pForth);
  void vPlanExecutingTaskFetch(Forth* pForth);
  void vPlanExecutingTaskStore(Forth* pForth);
  void vPlanFinished(Forth* pForth);
  void vPlanIdFetch(Forth* pForth);
  void vPlanLambdaAppend(Forth* pForth);
  void vPlanLambdaFetch(Forth* pForth);
  void vPlanLambdaStore(Forth* pForth);
  void vPlanNew(Forth* pForth);
  void vPlanNameFetch(Forth* pForth);
  void vPlanNameStore(Forth* pForth);
  void vPlanShow(Forth* pForth);

  // planner
  void vPlannerChoosePaths(Forth* pForth);
  void vPlannerChosenPath(Forth* pForth);
  void vPlannerDelete(Forth* pForth);
  void vPlannerFetchTechniqueFetch(Forth* pForth);
  void vPlannerGenPaths(Forth* pForth);
  void vPlannerHighKeyFetch(Forth* pForth);
  void vPlannerIndexNmFetch(Forth* pForth);
  void vPlannerKeyFetch(Forth* pForth);
  void vPlannerLowKeyFetch(Forth* pForth);
  void vPlannerMatchPaths(Forth* pForth);
  void vPlannerNew(Forth* pForth);
  void vPlannerParseWhere(Forth* pForth);
  void vPlannerPhrasesFetch(Forth* pForth);
  void vPlannerScorePaths(Forth* pForth);
  void vPlannerShowPaths(Forth* pForth);
  void vPlannerTableFetch(Forth* pForth);
  void vPlannerTableStore(Forth* pForth);

  // process
  void vProcessGetPid(Forth* pForth);
  void vProcessQuit(Forth* pForth);
  void vProcessTestPid(Forth* pForth);

  // prompt
  void vPromptContinue(Forth* pForth);
  void vPromptExecuteFunction(Forth* pForth);
  void vPromptGetCommand(Forth* pForth);
  void vPromptNew(Forth* pForth);
  void vPromptSession(Forth* pForth);

  // push
  void vPushI(Forth* pForth);
  void vPushJ(Forth* pForth);
  void vPushNullPtr(Forth* pForth);
  void vPushValue(Forth* pForth);

  // queue
  void vQueueAppend(Forth* pForth);
  void vQueueDelete(Forth* pForth);
  void vQueueDump(Forth* pForth);
  void vQueueInit(Forth* pForth);
  void vQueueNew(Forth* pForth);
  void vQueuePop(Forth* pForth);

  // queues
  void vQueuesCheckSleeping(Forth* pForth);
  void vQueuesExecutingLength(Forth* pForth);
  void vQueuesReadyLength(Forth* pForth);
  void vQueuesSleepingLength(Forth* pForth);
  void vQueuesWaitingLength(Forth* pForth);

  // rbCursor
  void vRbCursorBegin(Forth* pForth);
  void vRbCursorDelete(Forth* pForth);
  void vRbCursorEnd(Forth* pForth);
  void vRbCursorFirst(Forth* pForth);
  void vRbCursorGet(Forth* pForth);
  void vRbCursorInit(Forth* pForth);
  void vRbCursorNew(Forth* pForth);
  void vRbCursorNext(Forth* pForth);
  void vRbCursorTableFetch(Forth* pForth);
  void vRbCursorTableStore(Forth* pForth);

  // rowCursor
  void vRowCursorBegin(Forth* pForth);
  void vRowCursorDelete(Forth* pForth);
  void vRowCursorEnd(Forth* pForth);
  void vRowCursorEntBufferFetch(Forth* pForth);
  void vRowCursorEntBufferStore(Forth* pForth);
  void vRowCursorFirst(Forth* pForth);
  void vRowCursorInit(Forth* pForth);
  void vRowCursorNew(Forth* pForth);
  void vRowCursorNext(Forth* pForth);
  void vRowCursorRow(Forth* pForth);
  void vRowCursorTableFetch(Forth* pForth);
  void vRowCursorTableStore(Forth* pForth);

  // rstack
  void vRstackCsPick(Forth* pForth);
  void vRstackCsRoll(Forth* pForth);
  void vRstackDepth(Forth* pForth);
  void vRstackDrop(Forth* pForth);
  void vRstackFetch(Forth* pForth);
  void vRstackFetchTwo(Forth* pForth);
  void vRstackPick(Forth* pForth);
  void vRstackReverse(Forth* pForth);
  void vRstackRoll(Forth* pForth);
  void vRstackRollInv(Forth* pForth);
  void vRstackStk(Forth* pForth);
  void vRstackSwap(Forth* pForth);
  void vRstackToP(Forth* pForth);
  void vRstackToR(Forth* pForth);
  void vRstackToRTwo(Forth* pForth);
  void vRstack0(Forth* pForth);
  void vRstack1(Forth* pForth);
  void vRstack2(Forth* pForth);
  void vRstack3(Forth* pForth);
  void vRstack4(Forth* pForth);
  void vRstack5(Forth* pForth);
  void vRstack6(Forth* pForth);
  void vRstack7(Forth* pForth);
  void vRstack8(Forth* pForth);
  void vRstack9(Forth* pForth);

  // schLock
  void vSchLockDropWrite(Forth* pForth);
  void vSchLockGrabWrite(Forth* pForth);

  // session
  void vSessionCommand(Forth* pForth);
  void vSessionDicStack(Forth* pForth);
  void vSessionDump(Forth* pForth);
  void vSessionIsExiting(Forth* pForth);
  void vSessionPid(Forth* pForth);
  void vSessionSlotFetch(Forth* pForth);
  void vSessionTxFetch(Forth* pForth);
  void vSessionTxStore(Forth* pForth);

  // shell
  void vShellExecute(Forth* pForth);

  // slots
  void vSlotsColumnCount(Forth* pForth);
  void vSlotsColumnName(Forth* pForth);
  void vSlotsCreateColumnVec(Forth* pForth);
  void vSlotsSubDic(Forth* pForth);

  // socket
  void vSocketAccept(Forth* pForth);
  void vSocketClose(Forth* pForth);
  void vSocketConnect(Forth* pForth);
  void vSocketDropWrite(Forth* pForth);
  void vSocketGrabWrite(Forth* pForth);
  void vSocketListen(Forth* pForth);
  void vSocketOpen(Forth* pForth);
  void vSocketRecvObject(Forth* pForth);
  void vSocketRecvObject_LowMem(Forth* pForth);
  void vSocketSendObject(Forth* pForth);

  // steps
  void vStepTimingReset(Forth* pForth);
  void vStepTimingStart(Forth* pForth);
  void vStepTimingWrite(Forth* pForth);

  // string
  void vStrAlgebra(Forth* pForth);
  void vStrCat(Forth* pForth);
  void vStrClip(Forth* pForth);
  void vStrEqual(Forth* pForth);
  void vStrEscape(Forth* pForth);
  void vStrFirstCsv(Forth* pForth);
  void vStrFormat(Forth* pForth);
  void vStrLeft(Forth* pForth);
  void vStrLength(Forth* pForth);
  void vStrPrepend(Forth* pForth);
  void vStrRandom(Forth* pForth);
  void vStrRemovePrefix(Forth* pForth);
  void vStrRemoveSuffix(Forth* pForth);
  void vStrReplace(Forth* pForth);
  void vStrRight(Forth* pForth);
  void vStrSub(Forth* pForth);
  void vStrTailInt(Forth* pForth);

  // sub loader
  void vSubLoaderDelete(Forth* pForth);

  // table
  void vTableAddIndex(Forth* pForth);
  void vTableAnalyze(Forth* pForth);
  void vTableColumnCountFetch(Forth* pForth);
  void vTableCreateColumnVec(Forth* pForth);
  void vTableDrop(Forth* pForth);
  void vTableDump(Forth* pForth);
  void vTableFindColumn(Forth* pForth);
  void vTableFirstBuffer(Forth* pForth);
  void vTableGenData(Forth* pForth);
  void vTableGetColumnsAsString(Forth* pForth);
  void vTableColumnFetch(Forth* pForth);
  void vTableGetPtn(Forth* pForth);
  void vTableInflateScan(Forth* pForth);
  void vTableIndexCount(Forth* pForth);
  void vTableInit(Forth* pForth);
  void vTableIsColumn(Forth* pForth);
  void vTableIsLoadingStore(Forth* pForth);
  void vTableLazyBuildIndexes(Forth* pForth);
  void vTableList(Forth* pForth);
  void vTableName(Forth* pForth);
  void vTablePublish(Forth* pForth);
  void vTablePurge(Forth* pForth);
  void vTableRemoveIndex(Forth* pForth);
  void vTableScan(Forth* pForth);
  void vTableShowColumnVec(Forth* pForth);
  void vTableSlots(Forth* pForth);
  void vTableSpace(Forth* pForth);
  void vTableSubDic(Forth* pForth);
  void vTableSubLoaders(Forth* pForth);
  void vTableTruncate(Forth* pForth);
  void vTableIsValidIndexColumns(Forth* pForth);
  void vTableVerify(Forth* pForth);
  void vTableVisualize(Forth* pForth);

  // table partition
  void vTablePtnAttemptGrabRead(Forth* pForth);
  void vTablePtnAttemptGrabWrite(Forth* pForth);
  void vTablePtnDropRead(Forth* pForth);
  void vTablePtnDropWrite(Forth* pForth);
  void vTablePtnDump(Forth* pForth);
  void vTablePtnIsLocked(Forth* pForth);

  // task
  void vTaskAck(Forth* pForth);
  void vTaskAcksStore(Forth* pForth);
  void vTaskBreakPoint(Forth* pForth);
  void vTaskDestructorTraceFetch(Forth* pForth);
  void vTaskDestructorTraceStore(Forth* pForth);
  void vTaskDicStack(Forth* pForth);
  void vTaskDropLock(Forth* pForth);
  void vTaskDump(Forth* pForth);
  void vTaskDelete(Forth* pForth);
  void vTaskGrabLock(Forth* pForth);
  void vTaskIsAckSet(Forth* pForth);
  void vTaskJobFetch(Forth* pForth);
  void vTaskJobStore(Forth* pForth);
  void vTaskNameFetch(Forth* pForth);
  void vTaskThis(Forth* pForth);
  void vTaskSeed(Forth* pForth);
  void vTaskSessionFetch(Forth* pForth);
  void vTaskSessionStore(Forth* pForth);
  void vTaskSendNodeFetch(Forth* pForth);
  void vTaskSendNodeStore(Forth* pForth);
  void vTaskSleepMsec(Forth* pForth);
  void vTaskSleepNsec(Forth* pForth);
  void vTaskSleepSsec(Forth* pForth);
  void vTaskSleepUsec(Forth* pForth);
  void vTaskStateFetch(Forth* pForth);
  void vTaskThread(Forth* pForth);
  void vTaskWait(Forth* pForth);
  void vTaskYield(Forth* pForth);

  // test
  void vTestTest0(Forth* pForth);
  void vTestTest1(Forth* pForth);
  void vTestTest2(Forth* pForth);
  void vTestTest3(Forth* pForth);
  void vTestTest4(Forth* pForth);
  void vTestTest5(Forth* pForth);
  void vTestTest6(Forth* pForth);
  void vTestTest7(Forth* pForth);
  void vTestTest8(Forth* pForth);
  void vTestTest9(Forth* pForth);
  void vTestTest10(Forth* pForth);
  void vTestTest11(Forth* pForth);
  void vTestTest12(Forth* pForth);
  void vTestTest13(Forth* pForth);
  void vTestTest14(Forth* pForth);
  void vTestTest15(Forth* pForth);
  void vTestTest16(Forth* pForth);
  void vTestTest17(Forth* pForth);
  void vTestTest18(Forth* pForth);
  void vTestTest19(Forth* pForth);
  void vTestTest20(Forth* pForth);
  void vTestTest21(Forth* pForth);
  void vTestTest22(Forth* pForth);
  void vTestTest23(Forth* pForth);
  void vTestTest24(Forth* pForth);
  void vTestTest25(Forth* pForth);

  // thread
  void vThreadBind(Forth* pForth);
  void vThreadCpu(Forth* pForth);
  void vThreadDedicatedFetch(Forth* pForth);
  void vThreadDedicatedStore(Forth* pForth);
  void vThreadDisableRealtime(Forth* pForth);
  void vThreadDump(Forth* pForth);
  void vThreadEnableRealtime(Forth* pForth);
  void vThreadId(Forth* pForth);
  void vThreadSleepMsec(Forth* pForth);
  void vThreadSleepNsec(Forth* pForth);
  void vThreadSleepSsec(Forth* pForth);
  void vThreadSleepUsec(Forth* pForth);

  // timer
  void vTimerDelete(Forth* pForth);
  void vTimerInit(Forth* pForth);
  void vTimerMicrosecs(Forth* pForth);
  void vTimerMillisecs(Forth* pForth);
  void vTimerNanosecs(Forth* pForth);
  void vTimerNew(Forth* pForth);
  void vTimerReset(Forth* pForth);
  void vTimerSecs(Forth* pForth);
  void vTimerStart(Forth* pForth);
  void vTimerTicks(Forth* pForth);
  void vTimerUpdate(Forth* pForth);

  void vTimeCurrent(Forth* pForth);

  // to
  void vToBody(Forth* pForth);
  void vToInt(Forth* pForth);
  void vToPtr(Forth* pForth);
  void vToStr(Forth* pForth);
  void vToTask(Forth* pForth);

  // token
  void vTokenColon(Forth* pForth);

  // trace
  void vTraceOff(Forth* pForth);
  void vTraceOn(Forth* pForth);
  void vTracePoint(Forth* pForth);
  void vTraceSet(Forth* pForth);

  // tx
  void vTxBegin(Forth* pForth);
  void vTxCommit(Forth* pForth);
  void vTxCounterDec(Forth* pForth);
  void vTxCounterFetch(Forth* pForth);
  void vTxCounterStore(Forth* pForth);
  void vTxDistMemberFetch(Forth* pForth);
  void vTxDistMemberStore(Forth* pForth);
  void vTxDump(Forth* pForth);
  void vTxDumpAll(Forth* pForth);
  void vTxFree(Forth* pForth);
  void vTxId(Forth* pForth);
  void vTxPrepare(Forth* pForth);
  void vTxRollback(Forth* pForth);
  void vTxTestDist(Forth* pForth);

  // txCollection
  void vTxCollectionDump(Forth* pForth);
  void vTxCollectionInit(Forth* pForth);

  // txCursor
  void vTxCursorBegin(Forth* pForth);
  void vTxCursorDelete(Forth* pForth);
  void vTxCursorEnd(Forth* pForth);
  void vTxCursorFirst(Forth* pForth);
  void vTxCursorGet(Forth* pForth);
  void vTxCursorInit(Forth* pForth);
  void vTxCursorNew(Forth* pForth);
  void vTxCursorNext(Forth* pForth);

  // txMap
  void vTxMapInit(Forth* pForth);

  // txRef
  void vTxRefBegin(Forth* pForth);
  void vTxRefCheckDistId(Forth* pForth);
  void vTxRefCommit(Forth* pForth);
  void vTxRefDump(Forth* pForth);
  void vTxRefEnd(Forth* pForth);
  void vTxRefId(Forth* pForth);
  void vTxRefInit(Forth* pForth);
  void vTxRefNew(Forth* pForth);
  void vTxRefRollback(Forth* pForth);

  // utility
  void vUtilityPrefetch(Forth* pForth);
  void vUtilitySchemaTest(Forth* pForth);

  // vector
  void vVectorDifference(Forth* pForth);

  // word
  void vWord(Forth* pForth);
  void vWordCategoryFetch(Forth* pForth);
  void vWordCategoryStore(Forth* pForth);
  void vWordDump(Forth* pForth);
  void vWordEnmSetFetch(Forth* pForth);
  void vWordEnmSetStore(Forth* pForth);
  void vWordExitHookGet(Forth* pForth);
  void vWordExitHookSet(Forth* pForth);
  void vWordForget(Forth* pForth);
  void vWordImmediateGet(Forth* pForth);
  void vWordImmediateSet(Forth* pForth);
  void vWordIsImmediate(Forth* pForth);
  void vWordIsIntrinsic(Forth* pForth);
  void vWordKindFetch(Forth* pForth);
  void vWordKindStore(Forth* pForth);
  void vWordNameFetch(Forth* pForth);
  void vWordNameStore(Forth* pForth);
  void vWordPrev(Forth* pForth);

  // zero
  void vZeroLess(Forth* pForth);
  void vZeroLessEqual(Forth* pForth);
  void vZeroEqual(Forth* pForth);
  void vZeroNotEqual(Forth* pForth);
  void vZeroGreaterEqual(Forth* pForth);
  void vZeroGreater(Forth* pForth);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
