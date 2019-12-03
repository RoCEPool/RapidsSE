/*============================================================================*/
/* StepsSet.hpp - the set of steps                                            */
/*============================================================================*/
#pragma once

public:

  // access path
  void vAccessPathTableFetch(void);
  void vAccessPathTableStore(void);

  // accum
  void vAccumDelete(void);
  void vAccumNew(void);

  void vAccumAccumBuffer(void);
  void vAccumCountFetch(void);
  void vAccumCountStore(void);
  void vAccumGroupFetch(void);
  void vAccumGroupStore(void);
  void vAccumMaxFetch(void);
  void vAccumMaxStore(void);
  void vAccumMinFetch(void);
  void vAccumMinStore(void);
  void vAccumSumFetch(void);
  void vAccumSumStore(void);

  // address
  void vAddressEqual(void);
  void vAddressFetch(void);
  void vAddressStore(void);

  // args
  void vArgsOscillate(void);
  void vArgsPrint(void);

  // article
  void vArticleDump(void);

  // assert
  void vAssert(void);

  void vAssertClass(void);
  void vAssertClass_0(void);
  void vAssertClass_1(void);
  void vAssertClass_2(void);
  void vAssertClass_3(void);
  void vAssertClass_4(void);

  void vAssertStack_0(void);
  void vAssertStack_0more(void);
  void vAssertStack_1(void);
  void vAssertStack_1more(void);
  void vAssertStack_2(void);
  void vAssertStack_2more(void);
  void vAssertStack_3(void);
  void vAssertStack_3more(void);
  void vAssertStack_4(void);
  void vAssertStack_4more(void);

  // astack
  void vAstackClass(void);
  void vAstackDepth(void);
  void vAstackDrop(void);
  void vAstackDup(void);
  void vAstackFetch(void);
  void vAstackNip(void);
  void vAstackPick(void);
  void vAstackReverse(void);
  void vAstackStk(void);
  void vAstackSwap(void);
  void vAstackToA(void);
  void vAstackToD(void);
  void vAstackTransfer(void);
  void vAstack0(void);
  void vAstack1(void);
  void vAstack2(void);
  void vAstack3(void);
  void vAstack4(void);
  void vAstack5(void);
  void vAstack6(void);
  void vAstack7(void);
  void vAstack8(void);
  void vAstack9(void);

  // attachmap
  void vAttachMapTenantsFetch(void);

  // bp
  void vBpAnalyze(void);
  void vBpDump(void);
  void vBpEpochDump(void);
  void vBpEpochTake(void);
  void vBpReadBufferPool(void);
  void vBpWriteBufferPool(void);
  void vBpFreeBuffers(void);
  void vBpFreePercent(void);
  void vBpFreeSpace(void);
  void vBpInUseSpace(void);
  void vBpTotalSpace(void);

  // cast
  void vCastInt(void);
  void vCastPtr(void);

  // class
  void vClassName(void);

  // column
  void vColumnIsNullableFetch(void);
  void vColumnIsPartitionKeyFetch(void);
  void vColumnLengthFetch(void);
  void vColumnNameFetch(void);
  void vColumnScaleFetch(void);
  void vColumnTypeFetch(void);

  // command processor
  void vCommandBlankLine(void);
  void vCommandDeleteJob(void);
  void vCommandExecuteFile(void);
  void vCommandFindJobClass(void);
  void vCommandGetCommand(void);
  void vCommandJobClass(void);
  void vCommandPostJob(void);
  void vCommandPreJob(void);
  void vCommandSpawnJob(void);

  // comment
  void vComment(void);

  // comment
  void vCompilerCurrent(void);
  void vCompilerPop(void);
  void vCompilerPush(void);

  // cpu
  void vCpuRdtscp(void);

  // cursor
  void vCursorFetch(void);
  void vCursorStore(void);

  // ddllock
  void vDdlLockDropRead(void);
  void vDdlLockDropWrite(void);
  void vDdlLockGrabRead(void);
  void vDdlLockGrabWrite(void);
  void vDdlLockNone(void);
  void vDdlLockTestDrop(void);

  // debug
  void vDebugTrap(void);

  // dic
  void vDicInit(void);
  void vDicKindGet(void);
  void vDicKindSet(void);
  void vDicLastWord(void);
  void vDicName(void);
  void vDicRef(void);
  void vDicRoot(void);
  void vDicWordCount(void);

  // dicLock
  void vDicLockDropWrite(void);
  void vDicLockGrabWrite(void);

  // dicPath
  void vDicPathDepth(void);
  void vDicPathDump(void);
  void vDicPathPick(void);

  // dicStack
  void vDicStackDepth(void);
  void vDicStackDrop(void);
  void vDicStackDump(void);
  void vDicStackPick(void);
  void vDicStackPop(void);
  void vDicStackPush(void);
  void vDicStackToPathStr(void);

  // dir
  void vDirChangeCurrent(void);
  void vDirGetCurrent(void);
  void vDirExists(void);
  void vDirMake(void);
  void vDirRemove(void);

  void vDirClose(void);
  void vDirDelete(void);
  void vDirEntryName(void);
  void vDirEntryType(void);
  void vDirIsClosed(void);
  void vDirIsOpen(void);
  void vDirNew(void);
  void vDirOpen(void);
  void vDirPathGet(void);
  void vDirRead(void);
  void vDirRewind(void);
  void vDirSeek(void);
  void vDirTell(void);

  // docol
  void vDocolCompiling(void);
  void vDocolInterpreting(void);
  void vDocolIsCompiling(void);
  void vDocolIsInterpreting(void);

  // dot
  void vDot(void);
  void vDotPut(void);
  void vDotNewline(void);
  void vDotVerbose(void);

  // dstack
  void vDstackClass(void);                 //! DUP OF class@
  void vDstackDepth(void);
  void vDstackDrop(void);
  void vDstackDropTwo(void);
  void vDstackDup(void);
  void vDstackDupNonZero(void);
  void vDstackNip(void);
  void vDstackOver(void);
  void vDstackPick(void);
  void vDstackReverse(void);
  void vDstackRoll(void);
  void vDstackRollInv(void);
  void vDstackRot(void);
  void vDstackRotInv(void);
  void vDstackStk(void);
  void vDstackSwap(void);
  void vDstackTransfer(void);
  void vDstackTuck(void);

  // encode
  void vEncodeAgain(void);
  void vEncodeAhead(void);
  void vEncodeContinue(void);
  void vEncodeHere(void);
  void vEncodeIf(void);
  void vEncodeIfAgain(void);
  void vEncodeLoop(void);
  void vEncodeLoopPlus(void);
  void vEncodeNext(void);
  void vEncodeThen(void);
  void vEncodeUntil(void);

  // ename-set
  void vEnameSetAddInt(void);
  void vEnameSetAddStr(void);
  void vEnameSetAsStr(void);
  void vEnameSetGetInt(void);
  void vEnameSetGetStr(void);
  void vEnameSetHasName(void);
  void vEnameSetUpsertStr(void);
  void vEnameSetNew(void);

  // enter
  void vEnterObject(void);
  void vEnterRoutine(void);

  // entry
  void vEntryMatches(void);
  void vEntrySame(void);
  void vEntryToDataRow(void);

  // entry buffer
  void vEntBufferDump(void);
  void vEntBufferIndexFetch(void);
  void vEntBufferIndexStore(void);
  void vEntBufferIsLastFetch(void);
  void vEntBufferIsLastStore(void);
  void vEntBufferNextBuffer(void);
  void vEntBufferPartitionFetch(void);
  void vEntBufferPartitionStore(void);
  void vEntBufferToEntBuffer(void);

  // env
  void vEnvGet(void);

  // errors
  void vErrorMsg(void);

  // file
  void vFile0Get(void);
  void vFile0Set(void);
  void vFileClose(void);
  void vFileDelete(void);
  void vFileNew(void);
  void vFileOpen(void);
  void vFilePathGet(void);
  void vFileRead(void);
  void vFileReadRecord(void);
  void vFileWrite(void);

  // flow
  void vFlowBegin_(void);
  void vFlowExecute(void);
  void vFlowHazard(void);
  void vFlowJump(void);
  void vFlowJumpBegin_(void);
  void vFlowJumpFalse(void);
  void vFlowJumpTrue(void);
  void vFlowNop(void);
  void vFlowRetryPoint(void);
  void vFlowTest(void);
  void vFlowThrow(void);

  // forth
  void vAllot(void);
  void vCell(void);
  void vColon(void);
  void vComma(void);
  void vCompileComma(void);
  void vCreate(void);
  void vCreateFrom(void);
  void vCreateFromInRoot(void);
  void vCreateFromPersistent(void);
  void vCreatePersistent(void);
  void vCreateTenants(void);
  void vDoes(void);
  void vDoes_RT(void);
  void vExit_RT(void);
  void vFind(void);
  void vFindInCurrent(void);
  void vForget(void);
  void vImmediate(void);
  void vLocate(void);
  void vPostpone(void);
  void vPrompt(void);
  void vSee(void);
  void vSemicolon(void);
  void vSeriesName(void);
  void vSubDic(void);
  void vTick(void);

  // grid
  void vGridConnectedNodes(void);
  void vGridDefinedNodes(void);
  void vGridFindNode(void);
  void vGridIncConnectedNodes(void);
  void vGridNode(void);
  void vGridPartitions(void);
  void vGridShow(void);
  void vGridSlots(void);
  void vGridStartedStore(void);
  void vGridStartedFetch(void);
  void vGridStartNodeStore(void);
  void vGridStartNodeFetch(void);
  void vGridThisNodeFetch(void);
  void vGridThisNodeNameFetch(void);
  void vGridToNode(void);
  void vGridToPartition(void);
  void vGridToSlot(void);

  // group by
  void vGroupByApplyBuffer(void);
  void vGroupByApplyEntry(void);
  void vGroupByApplyPartial(void);
  void vGroupByDelete(void);
  void vGroupByEnd(void);
  void vGroupByNew(void);
  void vGroupByStart(void);

  // handler
  void vHandlerServiceSleepQueue(void);

  // index
  void vIndexAnalyze(void);
  void vIndexColumnFetch(void);
  void vIndexColumnCountFetch(void);
  void vIndexColumnName(void);
  void vIndexColumnType(void);
  void vIndexCompact(void);
  void vIndexDump(void);
  void vIndexIndexDefFetch(void);
  void vIndexIndexDefStore(void);
  void vIndexInit(void);
  void vIndexIsPrimaryKeyFetch(void);
  void vIndexIsPrimaryKeyStore(void);
  void vIndexIsUniqueFetch(void);
  void vIndexIsUniqueStore(void);
  void vIndexIsValidUniqueIndex(void);
  void vIndexLowerBound(void);
  void vIndexNameFetch(void);
  void vIndexPopulate(void);
  void vIndexRange(void);
  void vIndexShowColumnVec(void);
  void vIndexSpace(void);
  void vIndexSubDic(void);
  void vIndexTableFetch(void);
  void vIndexTableStore(void);
  void vIndexTruncate(void);
  void vIndexUpperBound(void);
  void vIndexVerify(void);
  void vIndexVisualize(void);

  // index definition
  void vIndexDefParse(void);

  // input
  void vInputClose(void);
  void vInputDepth(void);
  void vInputNext(void);
  void vInputOpen(void);
  void vInputPop(void);
  void vInputPush(void);
  void vInputStdin(void);
  void vInputText(void);
  void vInputToken(void);
  void vInputType(void);
  void vInputUnknown(void);

  // int
  void vIntAdd(void);
  void vIntDivide(void);
  void vIntEqual(void);
  void vIntGreater(void);
  void vIntGreaterEqual(void);
  void vIntHash(void);
  void vIntHo16(void);
  void vIntLess(void);
  void vIntLessEqual(void);
  void vIntModulo(void);
  void vIntMultiply(void);
  void vIntNeg(void);
  void vIntNotEqual(void);
  void vIntRandom(void);
  void vIntSubtract(void);
  void vUnsignedIntRandom(void);

  // jobClass
  void vJobClassBuffer(void);
  void vJobClassFind(void);
  void vJobClassName(void);
  void vJobClassNew(void);

  void vJobClassSeedRun(void);
  void vJobClassSeedReq(void);
  void vJobClassSeedRcv(void);
  void vJobClassSeedExe(void);
  void vJobClassSeedEnd(void);
  void vJobClassSeedCan(void);

  // job
  void vJobAcksRemaining(void);
  void vJobAddOriginator(void);
  void vJobAddParticipant(void);
  void vJobAssertBegin(void);
  void vJobAssertCorrectness(void);
  void vJobAssertNeutral(void);
  void vJobAssertOriginator(void);
  void vJobAssertParticipant(void);
  void vJobAssertReceived(void);

  void vJobBideOperand(void);

  void vJobClass(void);
  void vJobCompleted(void);
  void vJobCreateResponseVec(void);

  void vJobDec2pcAcksRmg(void);
  void vJobDelete(void);

  void vJobEnameSetFetch(void);
  void vJobErrorStore(void);
  void vJobErrorFetch(void);
  void vJobExecuteFetch(void);

  void vJobFindOriginator(void);
  void vJobFindParticipant(void);

  void vJobHasError(void);
  void vJobHasOperand(void);

  void vJobIdFetch(void);
  void vJobIsExecuting(void);

  void vJobJobClass(void);
  void vJobJobListFetch(void);

  void vJobKindFetch(void);

  void vJobLoopOutputOp(void);

  void vJobMetadataStore(void);

  void vJobNextOperand(void);
  void vJobNoWaitFetch(void);

  void vJobOpenToFile(void);
  void vJobOperandFetch(void);
  void vJobOperandStore(void);
  void vJobOpTableStore(void);
  void vJobOriginatingIdFetch(void);
  void vJobOriginatingIdStore(void);
  void vJobOriginatingTaskFetch(void);
  void vJobOriginatingTaskStore(void);
  void vJobOriginatorFetch(void);

  void vJobParentIdFetch(void);
  void vJobParticipantFetch(void);
  void vJobParticipantStore(void);
  void vJobParticipantResponseStore(void);
  void vJobParticipantHasError(void);
  void vJobPartitionsFetch(void);
  void vJobPhaseFetch(void);
  void vJobPhaseStore(void);
  void vJobPostRecv(void);
  void vJobPreSend(void);

  void vJobRecv(void);
  void vJobRegionSizeFetch(void);
  void vJobRemoveOriginator(void);
  void vJobRemoveParticipant(void);
  void vJobReseed(void);
  void vJobResponseStore(void);
  void vJobResponseFetch(void);
  void vJobRowsInvalidFetch(void);
  void vJobRowsInvalidStore(void);
  void vJobRowsProcessedFetch(void);
  void vJobRowsProcessedStore(void);
  void vJobRowsSkippedFetch(void);
  void vJobRowsSkippedStore(void);

  void vJobSeedDecode(void);
  void vJobSeedFetch(void);
  void vJobSeedStore(void);
  void vJobSend(void);
  void vJobSessionFetch(void);
  void vJobSessionStateFetch(void);
  void vJobShow(void);
  void vJobSignalWaiter(void);
  void vJobSlotFetch(void);
  void vJobSpawn(void);
  void vJobSpawnDedicated(void);
  void vJobSpawnUniversal(void);
  void vJobStateDiagram(void);

  void vJobTaskFetch(void);
  void vJobTaskStore(void);
  void vJobThis(void);

  void vJobWaitParticipant(void);

  // job ref
  void vJobRefPointer(void);
  void vJobRefNone(void);
  void vJobRefReturn(void);



  // job generic
  void vJob__BegunNew(void);
  void vJob__CancelNew(void);
  void vJob__CancelledNew(void);
  void vJob__EndNew(void);
  void vJob__EndedNew(void);
  void vJob__ExecuteNew(void);
  void vJob__ExecutedNew(void);

  // job analyzeIndex
  void vJob_AnalyzeIndexIsDetailed(void);
  void vJob_AnalyzeTableIsDetailed(void);

  // job Cat
  void vJob_CatBegin(void);
  void vJob_CatChunk(void);
  void vJob_CatEnd(void);

  // job CreateIndex
  void vJob_CreateIndexIsUniqueFetch(void);

  // job Export
  void vJob_ExportAuxAckDecidedFetch(void);
  void vJob_ExportAuxAckDecidedStore(void);
  void vJob_ExportBegin(void);
  void vJob_ExportEnd(void);
  void vJob_ExportTableStore(void);

  // job FetchRows
  void vJob_FetchRowsColumnNameFetch(void);
  void vJob_FetchRowsColumnStore(void);
  void vJob_FetchRowsLastBuffersStore(void);
  void vJob_FetchRowsPartitionFetch(void);
  void vJob_FetchRowsPlannerStore(void);
  void vJob_FetchRowsSpawnSub(void);
  void vJob_FetchRowsSpawnSubInflate(void);
  void vJob_FetchRowsTableStore(void);
  void vJob_FetchRowsWhereFetch(void);

  // job GenData
  void vJob_GenerateGenerate(void);
  void vJob_GenerateTableStore(void);

  // job IndexJoin
  void vJob_IndexJoinLastBuffersStore(void);
  void vJob_IndexJoinLeftIndexStore(void);
  void vJob_IndexJoinLeftTableStore(void);
  void vJob_IndexJoinRightIndexStore(void);
  void vJob_IndexJoinRightIndexNameFetch(void);
  void vJob_IndexJoinRightTableStore(void);
  void vJob_IndexJoinRightTableNameFetch(void);
  void vJob_IndexJoinSpawnSub(void);
  void vJob_SubIndexJoinIndexJoin(void);

  // job Ingest
  void vJob_IngestBegin(void);
  void vJob_IngestChunk(void);
  void vJob_IngestEnd(void);
  void vJob_IngestTableStore(void);

  // job InsertRows
  void vJob_CopyBufferCopy(void);

  // insert row
  void vJob_InsertRowPartitionFetch(void);
  void vJob_InsertRowRecordStore(void);
  void vJob_InsertRowNewJobWriteRow(void);
  void vJob_InsertRowSpawnJobWriteRow(void);

  // insert table
  void vJob_InsertTableSubInserterJobFetch(void);

  void vJob_SubInserterBegin(void);
  void vJob_SubInserterEnd(void);

  // job KidExport
  void vJob_KidExportExportOrigin(void);
  void vJob_KidExportExportJobId(void);
  void vJob_KidExportWriteExportBuffer(void);

  // job LoadTable
  void vJob_LoadTableBegin(void);
  void vJob_LoadTableChunk(void);
  void vJob_LoadTableDumpMap(void);
  void vJob_LoadTableEnd(void);
  void vJob_LoadTableFilePathFetch(void);
  void vJob_LoadTableTableStore(void);

  // job Monitor
  void vJob_MonitorBegin(void);
  void vJob_MonitorTake(void);
  void vJob_MonitorEnd(void);

  // job NetHog
  void vJob_NetHogHogNoFetch(void);

  // job NodeInfo
  void vJob_NodeInfoNodeNameFetch(void);

  // Job Operand
  void vJob_OperandPipeOut(void);

  // job Pipeline
  void vJob_PipelineBegin(void);
  void vJob_PipelineCancel(void);
  void vJob_PipelineEnd(void);
  void vJob_PipelineExecute(void);
  void vJob_PipelineCompile(void);

  // job publish
  void vJob_PublishSpawnSub(void);
  void vJob_PublishTableStore(void);

  // job Purge
  void vJob_PurgeSpawnSub(void);
  void vJob_PurgeTableStore(void);

  // job Query
  void vJob_QueryBegin(void);
  void vJob_QueryEnd(void);
  void vJob_QueryExecute(void);

  // job Receiver
  void vJob_ReceiverConnNoFetch(void);
  void vJob_ReceiverConnNoStore(void);
  void vJob_ReceiverJobReceived(void);
  void vJob_ReceiverNodeFetch(void);
  void vJob_ReceiverNodeStore(void);

 // job Repeater
  void vJob_RepeaterBegin(void);
  void vJob_RepeaterChunk(void);
  void vJob_RepeaterEnd(void);

  // job Report
  void vJob_ReportBegin(void);
  void vJob_ReportTake(void);
  void vJob_ReportEnd(void);

  // job Select
  void vJob_SelectBegin(void);
  void vJob_SelectEnd(void);
  void vJob_SelectExecute(void);

  // job SelectTable
  void vJob_SelectTablePartitionFetch(void);
  void vJob_SelectTableTableStore(void);

  // job Sink
  void vJob_SinkBegin(void);
  void vJob_SinkChunk(void);
  void vJob_SinkEnd(void);

  // job Sort
  void vJob_SortBegin(void);
  void vJob_SortEnd(void);
  void vJob_SortExecute(void);

  // job SubExporter
  void vJob_SubExporterBegin(void);
  void vJob_SubExporterChunk(void);
  void vJob_SubExporterEnd(void);
  void vJob_SubExporterSpawn(void);
  void vJob_SubExporterExportJobFetch(void);

  // job SubFetch
  void vJob_SubFetchBufferQueueFetch(void);
  void vJob_SubFetchPhraseFetch(void);
  void vJob_SubFetchPlannerFetch(void);
  void vJob_SubFetchSlotFetch(void);

  // job SubLoader
  void vJob_SubLoaderBegin(void);
  void vJob_SubLoaderChunk(void);
  void vJob_SubLoaderEnd(void);
  void vJob_SubLoaderSpawn(void);
  void vJob_SubLoaderTerminate(void);

  // job SubPublish
  void vJob_SubPublishBegin(void);
  void vJob_SubPublishChunk(void);
  void vJob_SubPublishEnd(void);
  void vJob_SubPublishSlotFetch(void);
  void vJob_SubPublishTableStore(void);
  void vJob_SubPublishPublishJobIdFetch(void);

  // job SubPurger
  void vJob_SubPurgerBegin(void);
  void vJob_SubPurgerChunk(void);
  void vJob_SubPurgerEnd(void);

  void vJob_SubPurgerSlotFetch(void);
  void vJob_SubPurgerPurgeJobIdFetch(void);

  // Job SubTruncate
  void vJob_SubTruncateSlotFetch(void);
  void vJob_SubTruncateTruncateJobIdFetch(void);

  // job SubWalker
  void vJob_SubWalkerBegin(void);
  void vJob_SubWalkerChunk(void);
  void vJob_SubWalkerEnd(void);
  void vJob_SubWalkerWalkJobFetch(void);
  void vJob_SubWalkerSpawn(void);

  // Job Truncate
  void vJob_TruncateTableSpawnSub(void);

  // job Walk
  void vJob_WalkBegin(void);
  void vJob_WalkEnd(void);
  void vJob_WalkIndexStore(void);
  void vJob_WalkTableStore(void);

  // Job WriteBuffer
  void vJob_WriteBufferParentJobIdFetch(void);
  void vJob_WriteBufferInsertRows(void);
  void vJob_WriteBufferIsIngesting(void);
  void vJob_WriteBufferIsInserting(void);
  void vJob_WriteBufferIsLoading(void);
  void vJob_WriteBufferPartitionFetch(void);
  void vJob_WriteBufferTableStore(void);

  // Job WriteRow
  void vJob_WriteRowParentJobIdFetch(void);
  void vJob_WriteRowEncodeRow(void);
  void vJob_WriteRowInsertRow(void);
  void vJob_WriteRowPartitionFetch(void);
  void vJob_WriteRowTableStore(void);

  // load table
  void vLoadTableChunkFile(void);
  void vLoadTableDelete(void);
  void vLoadTableDumpMap(void);
  void vLoadTableNew(void);
  void vLoadTableTableStore(void);

  // locals
  void vLocalFetch(void);
  void vLocalStore(void);
  void vLocalsBar(void);
  void vLocalsPop(void);
  void vLocalsPush(void);
  void vLocalsTo(void);

  // lock
  void vLockDropRead(void);
  void vLockDropWrite(void);
  void vLockGrabRead(void);
  void vLockGrabWrite(void);
  void vLockInit(void);

  // locks
  void vLockTrackerList(void);
  void vLockTrackerReset(void);

  // log
  void vLogDump(void);
  void vLogMark(void);
  void vLogMarkEntry(void);
  void vLogMarkExit(void);
  void vLogMarkPhase(void);

  void vLogMarkWord(void);
  void vLogMarkSeed(void);

  void vLogJobSeed(void);

  // logic
  void vLogicAnd(void);
  void vLogicNot(void);
  void vLogicOr(void);

  // magnitude
  void vBytesMagnitude(void);
  void vNumberMagnitude(void);
  void vToDigitName(void);

  // memory
  void vMemoryByteDump(void);
  void vMemoryReset(void);
  void vMemoryStatistics(void);
  void vMemoryWordDump(void);

  // node (of grid)
  void vNodeForget(void);
  void vNodeIP(void);
  void vNodeName(void);
  void vNodeNew(void);
  void vNodeNo(void);
  void vNodePartitions(void);
  void vNodePort(void);
  void vNodeRecvSocketStore(void);
  void vNodeRecvSocketFetch(void);
  void vNodeSendSocketStore(void);
  void vNodeSendSocketFetch(void);
  void vNodeSize(void);
  void vNodeStartNode(void);
  void vNodeStateStore(void);
  void vNodeStateFetch(void);
  void vNodeStatus(void);

  // object
  void vObjectClassFetch(void);
  void vObjectClassStore(void);
  void vObjectDump(void);
  void vObjectFetch(void);
  void vObjectMagicMark(void);
  void vObjectStore(void);
  void vObjectWordBodySet(void);

  // olist (originator list)
  void vOlistFindJob(void);

  // operand
  void vOperandBide(void);
  void vOperandDelete(void);
  void vOperandFirstRowFetch(void);
  void vOperandGet(void);
  void vOperandHandleAggEnd(void);
  void vOperandHandleEnd(void);
  void vOperandIsNull(void);
  void vOperandItemCountFetch(void);
  void vOperandKind(void);
  void vOperandNewEndMarker(void);
  void vOperandNewStreamDef(void);
  void vOperandNext(void);
  void vOperandPipeOut(void);
  void vOperandPipeToJob(void);
  void vOperandRowCountFetch(void);
  void vOperandSet(void);
  void vOperandToCsv(void);
  void vOperandWorkDone(void);
  void vOperandWriteToFile0(void);

  // persistence
  void vPersistenceCheckpoint(void);
  void vPersistenceReload(void);

  // plan
  void vPlanCompile(void);
  void vPlanDelete(void);
  void vPlanDic(void);
  void vPlanExecutingTaskFetch(void);
  void vPlanExecutingTaskStore(void);
  void vPlanFinished(void);
  void vPlanIdFetch(void);
  void vPlanLambdaAppend(void);
  void vPlanLambdaFetch(void);
  void vPlanLambdaStore(void);
  void vPlanNew(void);
  void vPlanNameFetch(void);
  void vPlanNameStore(void);
  void vPlanShow(void);

  // planner
  void vPlannerChoosePaths(void);
  void vPlannerChosenPath(void);
  void vPlannerDelete(void);
  void vPlannerFetchTechniqueFetch(void);
  void vPlannerGenPaths(void);
  void vPlannerHighKeyFetch(void);
  void vPlannerIndexNmFetch(void);
  void vPlannerKeyFetch(void);
  void vPlannerLowKeyFetch(void);
  void vPlannerMatchPaths(void);
  void vPlannerNew(void);
  void vPlannerParseWhere(void);
  void vPlannerPhrasesFetch(void);
  void vPlannerScorePaths(void);
  void vPlannerShowPaths(void);
  void vPlannerTableFetch(void);
  void vPlannerTableStore(void);

  // plist (participant list)
  void vPlistFindJob(void);

  // process
  void vProcessGetPid(void);
  void vProcessQuit(void);
  void vProcessTestPid(void);

  // prompt
  void vPromptContinue(void);
  void vPromptExecuteFunction(void);
  void vPromptGetCommand(void);
  void vPromptNew(void);
  void vPromptSession(void);

  // push
  void vPushI(void);
  void vPushJ(void);
  void vPushNullPtr(void);
  void vPushValue(void);

  // queue
  void vQueueAppend(void);
  void vQueueDelete(void);
  void vQueueDump(void);
  void vQueueInit(void);
  void vQueueNew(void);
  void vQueuePop(void);

  // queues
  void vQueuesCheckSleeping(void);
  void vQueuesExecutingLength(void);
  void vQueuesReadyLength(void);
  void vQueuesSleepingLength(void);
  void vQueuesWaitingLength(void);

  // rbCursor
  void vRbCursorBegin(void);
  void vRbCursorDelete(void);
  void vRbCursorEnd(void);
  void vRbCursorFirst(void);
  void vRbCursorGet(void);
  void vRbCursorInit(void);
  void vRbCursorNew(void);
  void vRbCursorNext(void);
  void vRbCursorTableFetch(void);
  void vRbCursorTableStore(void);

  // row
  void vRowIntColumnFetch(void);

  // rowCursor
  void vRowCursorBegin(void);
  void vRowCursorDelete(void);
  void vRowCursorEnd(void);
  void vRowCursorEntBufferFetch(void);
  void vRowCursorEntBufferStore(void);
  void vRowCursorFirst(void);
  void vRowCursorInit(void);
  void vRowCursorNew(void);
  void vRowCursorNext(void);
  void vRowCursorRow(void);
  void vRowCursorTableFetch(void);
  void vRowCursorTableStore(void);

  // regset
  void vRegSetFun(void);

  // rstack
  void vRstackCsPick(void);
  void vRstackCsRoll(void);
  void vRstackDepth(void);
  void vRstackDrop(void);
  void vRstackFetch(void);
  void vRstackFetchTwo(void);
  void vRstackPick(void);
  void vRstackReverse(void);
  void vRstackRoll(void);
  void vRstackRollInv(void);
  void vRstackStk(void);
  void vRstackSwap(void);
  void vRstackToP(void);
  void vRstackToR(void);
  void vRstackToRTwo(void);
  void vRstack0(void);
  void vRstack1(void);
  void vRstack2(void);
  void vRstack3(void);
  void vRstack4(void);
  void vRstack5(void);
  void vRstack6(void);
  void vRstack7(void);
  void vRstack8(void);
  void vRstack9(void);

  // schLock
  void vSchLockDropWrite(void);
  void vSchLockGrabWrite(void);

  // session
  void vSessionCommand(void);
  void vSessionDicStackFetch(void);
  void vSessionDump(void);
  void vSessionEnameSetFetch(void);
  void vSessionIsExiting(void);
  void vSessionPid(void);
  void vSessionSlotFetch(void);
  void vSessionTxFetch(void);
  void vSessionTxStore(void);

  // shell
  void vShellExecute(void);

  // slots
  void vSlotsColumnCount(void);
  void vSlotsColumnName(void);
  void vSlotsCreateColumnVec(void);
  void vSlotsSubDic(void);

  // socket
  void vSocketAccept(void);
  void vSocketClose(void);
  void vSocketConnect(void);
  void vSocketDropWrite(void);
  void vSocketGrabWrite(void);
  void vSocketListen(void);
  void vSocketOpen(void);
  void vSocketRecvObject(void);
  void vSocketRecvObject_LowMem(void);
  void vSocketSendObject(void);

  // ssn
  void vSsnIncBegun(void);
  void vSsnIncCancelled(void);
  void vSsnIncEnded(void);
  void vSsnIncExecuted(void);
  void vSsnSenLoc(void);
  void vSsnTraceSession(void);

  // steps
  void vStepTimingReset(void);
  void vStepTimingStart(void);
  void vStepTimingWrite(void);

  // string
  void vStrAlgebra(void);
  void vStrCat(void);
  void vStrClip(void);
  void vStrEqual(void);
  void vStrEscape(void);
  void vStrFirstCsv(void);
  void vStrFormat(void);
  void vStrLeft(void);
  void vStrLength(void);
  void vStrPrepend(void);
  void vStrRandom(void);
  void vStrRemovePrefix(void);
  void vStrRemoveSuffix(void);
  void vStrReplace(void);
  void vStrRight(void);
  void vStrSub(void);
  void vStrTailInt(void);
  void vStrToStrVec(void);

  // stream def
  void vStreamDefGet(void);
  void vStreamDefNewCount(void);
  void vStreamDefNewToCsv(void);
  void vStreamDefPipeOut(void);
  void vStreamDefSet(void);

  // sub loader
  void vSubLoaderDelete(void);

  // subset
  void vSubsetFetch(void);
  void vSubsetNext(void);
  void vSubsetNextBuffer(void);
  void vSubsetSet(void);

  // table
  void vTableAddIndex(void);
  void vTableAnalyze(void);
  void vTableColumnCountFetch(void);
  void vTableDrop(void);
  void vTableDump(void);
  void vTableFindColumn(void);
  void vTableFirstBuffer(void);
  void vTableGenData(void);
  void vTableGetColumnsAsString(void);
  void vTableColumnFetch(void);
  void vTableGetPtn(void);
  void vTableIndexCount(void);
  void vTableInit(void);
  void vTableIsColumn(void);
  void vTableIsLoadingStore(void);
  void vTableLazyBuildIndexes(void);
  void vTableList(void);
  void vTableName(void);
  void vTablePublish(void);
  void vTablePurge(void);
  void vTableRemoveIndex(void);
  void vTableScan(void);
  void vTableShowColumnVec(void);
  void vTableSlots(void);
  void vTableSpace(void);
  void vTableSubDic(void);
  void vTableSubLoaders(void);
  void vTableTableDefFetch(void);
  void vTableTableDefStore(void);
  void vTableTruncate(void);
  void vTableIsValidIndexColumns(void);
  void vTableVerify(void);
  void vTableVisualize(void);

  // table2
  void vTableSet(void);
  void vTablePtnFetch(void);

  // table definition
  void vTableDefParse(void);

  // table partition
  void vTablePtnAttemptGrabRead(void);
  void vTablePtnAttemptGrabWrite(void);
  void vTablePtnDropRead(void);
  void vTablePtnDropWrite(void);
  void vTablePtnDump(void);
  void vTablePtnIsLocked(void);

  // table partition
  void vTablePtnRoot(void);
  void vTablePtnSet(void);

  // task
  void vTaskAck(void);
  void vTaskAcksStore(void);
  void vTaskBreakPoint(void);
  void vTaskDestructorTraceFetch(void);
  void vTaskDestructorTraceStore(void);
  void vTaskDicStack(void);
  void vTaskDoneFinally(void);
  void vTaskDropLock(void);
  void vTaskDump(void);
  void vTaskDelete(void);
  void vTaskGrabLock(void);
  void vTaskIsAckSet(void);
  void vTaskJobFetch(void);
  void vTaskJobStore(void);
  void vTaskNameFetch(void);
  void vTaskThis(void);
  void vTaskSeedInitially(void);
  void vTaskSendNodeFetch(void);
  void vTaskSendNodeStore(void);
  void vTaskSleepMsec(void);
  void vTaskSleepNsec(void);
  void vTaskSleepSsec(void);
  void vTaskSleepUsec(void);
  void vTaskStateFetch(void);
  void vTaskThread(void);
  void vTaskWait(void);
  void vTaskYield(void);

  // test
  void vTestTest0(void);
  void vTestTest1(void);
  void vTestTest2(void);
  void vTestTest3(void);
  void vTestTest4(void);
  void vTestTest5(void);
  void vTestTest6(void);
  void vTestTest7(void);
  void vTestTest8(void);
  void vTestTest9(void);
  void vTestTest10(void);
  void vTestTest11(void);
  void vTestTest12(void);
  void vTestTest13(void);
  void vTestTest14(void);
  void vTestTest15(void);
  void vTestTest16(void);
  void vTestTest17(void);
  void vTestTest18(void);
  void vTestTest19(void);
  void vTestTest20(void);
  void vTestTest21(void);
  void vTestTest22(void);
  void vTestTest23(void);
  void vTestTest24(void);
  void vTestTest25(void);

  // thread
  void vThreadBind(void);
  void vThreadCpu(void);
  void vThreadDedicatedFetch(void);
  void vThreadDedicatedStore(void);
  void vThreadDisableRealtime(void);
  void vThreadDump(void);
  void vThreadEnableRealtime(void);
  void vThreadId(void);
  void vThreadSleepMsec(void);
  void vThreadSleepNsec(void);
  void vThreadSleepSsec(void);
  void vThreadSleepUsec(void);

  // timer
  void vTimerDelete(void);
  void vTimerInit(void);
  void vTimerMicrosecs(void);
  void vTimerMillisecs(void);
  void vTimerNanosecs(void);
  void vTimerNew(void);
  void vTimerReset(void);
  void vTimerSecs(void);
  void vTimerStart(void);
  void vTimerTicks(void);
  void vTimerUpdate(void);

  void vTimeCurrent(void);

  // to
  void vToBody(void);
  void vToInt(void);
  void vToPtr(void);
  void vToStr(void);
  void vToTask(void);

  // token
  void vTokenColon(void);

  // trace
  void vTraceOff(void);
  void vTraceOn(void);
  void vTracePoint(void);
  void vTraceSet(void);

  // tx
  void vTxBegin(void);
  void vTxCommit(void);
  void vTxCounterDec(void);
  void vTxCounterFetch(void);
  void vTxCounterStore(void);
  void vTxDistMemberFetch(void);
  void vTxDistMemberStore(void);
  void vTxDump(void);
  void vTxDumpAll(void);
  void vTxFree(void);
  void vTxId(void);
  void vTxPrepare(void);
  void vTxRollback(void);
  void vTxTestDist(void);

  // txCollection
  void vTxCollectionDump(void);
  void vTxCollectionInit(void);

  // txCursor
  void vTxCursorBegin(void);
  void vTxCursorDelete(void);
  void vTxCursorEnd(void);
  void vTxCursorFirst(void);
  void vTxCursorGet(void);
  void vTxCursorInit(void);
  void vTxCursorNew(void);
  void vTxCursorNext(void);

  // txMap
  void vTxMapInit(void);

  // txRef
  void vTxRefBegin(void);
  void vTxRefCheckDistId(void);
  void vTxRefCommit(void);
  void vTxRefDump(void);
  void vTxRefEnd(void);
  void vTxRefId(void);
  void vTxRefInit(void);
  void vTxRefNew(void);
  void vTxRefRollback(void);

  // utility
  void vUtilityPrefetch(void);
  void vUtilitySchemaTest(void);

  // vector
  void vVectorDifference(void);

  // word
  void vWord(void);
  void vWordCategoryFetch(void);
  void vWordCategoryStore(void);
  void vWordDump(void);
  void vWordEnmSetFetch(void);
  void vWordEnmSetStore(void);
  void vWordExitHookGet(void);
  void vWordExitHookSet(void);
  void vWordFind(void);
  void vWordForget(void);
  void vWordImmediateGet(void);
  void vWordImmediateSet(void);
  void vWordIsImmediate(void);
  void vWordIsIntrinsic(void);
  void vWordKindFetch(void);
  void vWordKindStore(void);
  void vWordNameFetch(void);
  void vWordNameStore(void);
  void vWordNew(void);
  void vWordPrev(void);
  void vWordResetRoutine(void);
  void vWordSee(void);

  // zero
  void vZeroLess(void);
  void vZeroLessEqual(void);
  void vZeroEqual(void);
  void vZeroNotEqual(void);
  void vZeroGreaterEqual(void);
  void vZeroGreater(void);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
