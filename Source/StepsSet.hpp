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

  // aggregate
  void vAggregateRsbAggregateCpy(void);
  void vAggregateRsbAggregateGby(void);
  void vAggregateRsbDelete(void);
  void vAggregateRsbGet(void);
  void vAggregateRsbNew(void);

  // agg timer
  void vAggTimerAdd(void);
  void vAggTimerDelete(void);
  void vAggTimerMicrosecs(void);
  void vAggTimerMillisecs(void);
  void vAggTimerNanosecs(void);
  void vAggTimerNew(void);
  void vAggTimerReportStr(void);
  void vAggTimerReset(void);
  void vAggTimerSecs(void);
  void vAggTimerTicks(void);

  // api
  void vApiAddProcedure(void);
  void vApiExecuteRequest(void);
  void vApiSpawnDemon(void);

  // args
  void vArgsOscillate(void);
  void vArgsPrint(void);

  // arrange
  void vArrangeBegin(void);
  void vArrangeDelete(void);
  void vArrangeEnd(void);
  void vArrangeFirst(void);
  void vArrangeInsert(void);
  void vArrangeName(void);
  void vArrangeNew(void);
  void vArrangeNext(void);
  void vArrangeObject(void);
  void vArrangeSeqNo(void);
  void vArrangeSequence(void);

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
  void vAstackDiscard(void);
  void vAstackDup(void);
  void vAstackFetch(void);
  void vAstackNip(void);
  void vAstackPick(void);
  void vAstackReverse(void);
  void vAstackStk(void);
  void vAstackSwap(void);
  void vAstackToA(void);
  void vAstackToD(void);
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
  void vAttachMapTenantsStore(void);
  void vAttachMapRegionSizeFetch(void);

  // bp
  void vBpAnalyze(void);
  void vBpEpochDump(void);
  void vBpEpochTake(void);
  void vBpReadBufferPool(void);
  void vBpWriteBufferPool(void);
  void vBpFreeBuffers(void);
  void vBpFreePercent(void);
  void vBpFreeSpace(void);
  void vBpInUseSpace(void);
  void vBpTotalSpace(void);

  // build
  void vBuildBranch(void);
  void vBuildDate(void);
  void vBuildGitHash(void);
  void vBuildHostName(void);
  void vBuildUserName(void);

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
  void vCompilerCompiling(void);
  void vCompilerCondBegin(void);
  void vCompilerCondEnd(void);
  void vCompilerCondElse(void);
  void vCompilerCondTest(void);
  void vCompilerCurrentFetch(void);
  void vCompilerCurrentStore(void);
  void vCompilerDiscarding(void);
  void vCompilerInterpreting(void);
  void vCompilerIsCompiling(void);
  void vCompilerIsDiscarding(void);
  void vCompilerIsInterpreting(void);
  void vCompilerIsPostponing(void);
  void vCompilerNestedFetch(void);
  void vCompilerNestedStore(void);
  void vCompilerPostponing(void);


  // crwBuffer
  void vCrwBufferDelete(void);
  void vCrwBufferOperandDump(void);
  void vCrwBufferNew(void);
  void vCrwBufferSnapshot(void);

  // csv buffer
  void vCsvBufferDelete(void);
  void vCsvBufferNew(void);
  void vCsvBufferOperandDump(void);
  void vCsvBufferPrint(void);

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
  void vDebugFlagFetch(void);
  void vDebugFlagStore(void);

  // dic
  void vDicDelete(void);
  void vDicKindGet(void);
  void vDicKindSet(void);
  void vDicLastWord(void);
  void vDicNameFetch(void);
  void vDicNameStore(void);
  void vDicNew(void);
  void vDicRef(void);
  void vDicRoot(void);
  void vDicWordCount(void);

  // dicLock
  void vDicLockDropWrite(void);
  void vDicLockGrabWrite(void);


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

  // dot
  void vDot(void);
  void vDotPut(void);
  void vDotNewline(void);
  void vDotVerbose(void);

  // dstack
  void vDstackClass(void);                 //! DUP OF class@
  void vDstackDepth(void);
  void vDstackDiscard(void);
  void vDstackDiscardTwo(void);
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
  void vDstackTuck(void);

  // encode
  void vEncodeCurrentCall(void);
  void vEncodeCurrentPush(void);

  void vEncodeCall(void);
  void vEncodePush(void);

  void vEncodeAgain(void);
  void vEncodeAhead(void);
  void vEncodeContinue(void);
  void vEncodeHere(void);
  void vEncodeIf(void);
  void vEncodeIfAgain(void);
  void vEncodeLoop(void);
  void vEncodeLoopPlus(void);
  void vEncodeNext(void);
  void vEncodeNextPlus(void);
  void vEncodeThen(void);
  void vEncodeUntil(void);


  // ename-set
  void vTagSetAppendInt(void);
  void vTagSetAppendStr(void);
  void vTagSetAsStr(void);
  void vTagSetCopy(void);
  void vTagSetGetInt(void);
  void vTagSetGetStr(void);
  void vTagSetHasName(void);
  void vTagSetInsertInt(void);
  void vTagSetInsertStr(void);
  void vTagSetReplaceInt(void);
  void vTagSetReplaceStr(void);
  void vTagSetUpsertStr(void);
  void vTagSetNew(void);

  // enter
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

  // extractRow
  void vExtractRowCreate(void);
  void vExtractRowDelete(void);

  // executable
  void vExecutableIsDebug(void);
  void vExecutableIsRelease(void);

  // expression evaluation vector
  void vExprVecDelete(void);
  void vExprVecEvalAll(void);
  void vExprVecEvalOne(void);
  void vExprVecGet(void);
  void vExprVecNew(void);

  // fact
  void vFactAtrOf(void);
  void vFactAttributeAppend(void);
  void vFactDelete(void);
  void vFactElementAppend(void);
  void vFactElmOf(void);
  void vFactElmOfSet(void);
  void vFactGet(void);
  void vFactNew(void);
  void vFactNewAtr(void);
  void vFactNewSet(void);
  void vFactNewTpl(void);
  void vFactSet(void);
  void vFactSetInt(void);
  void vFactSetText(void);
  void vFactSubOf(void);
 
  // file
  void vFile0Get(void);
  void vFile0GetEofLocation(void);
  void vFile0IsNull(void);
  void vFile0Set(void);
  void vFile0Size(void);
  void vFileClose(void);
  void vFileDelete(void);
  void vFileModTime(void);
  void vFileNew(void);
  void vFileOpen(void);
  void vFilePathGet(void);
  void vFilePrint(void);
  void vFileReadChunk(void);
  void vFileReadCrwBlock(void);
  void vFileReadLine(void);
  void vFileReadText(void);
  void vFileSeek(void);
  void vFileSeekRndCsv(void);
  void vFileSize(void);
  void vFileSizeNm(void);
  void vFileTell(void);
  void vFileWriteLine(void);

  // flow
  void vFlowExecute(void);
  void vFlowHazard(void);
  void vFlowJump(void);
  void vFlowJumpFalse(void);
  void vFlowJumpTrue(void);
  void vFlowNop(void);
  void vFlowReenter(void);
  void vFlowTest(void);
  void vFlowThrow(void);

  // forth
  void vColon(void);
  void vCreateFromInRoot(void);
  void vExit_RT(void);
  void vFind(void);
  void vImmediate(void);
  void vMungedText(void);
  void vPostpone(void);
  void vSee(void);
  void vSemicolon(void);
  void vSeriesName(void);

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
  void vIndexColumnFetch(void);
  void vIndexColumnCountFetch(void);
  void vIndexCompact(void);
  void vIndexCompactPtn(void);
  void vIndexDump(void);
  void vIndexGenEntBuffer(void);
  void vIndexGenEntBuffers(void);
  void vIndexSize(void);
  void vIndexVerify(void);
  void vIndexVisualize(void);

  // index steps - to go
  void vIndexDelete(void);
  void vIndexDeleteNext(void);
  void vIndexIndexDefFetch(void);
  void vIndexIndexDefStore(void);
  void vIndexIndexTree(void);
  void vIndexIsPrimaryKeyFetch(void);
  void vIndexIsPrimaryKeyStore(void);
  void vIndexIsUniqueFetch(void);
  void vIndexIsUniqueStore(void);
  void vIndexIsValidUniqueIndex(void);
  void vIndexNameFetch(void);
  void vIndexNew(void);
  void vIndexPopulate(void);
  void vIndexTableFetch(void);
  void vIndexTableStore(void);


  // index definition
  void vIndexDefAddColumn(void);
  void vIndexDefDelete(void);
  void vIndexDefGet(void);
  void vIndexDefIsNull(void);
  void vIndexDefNameStore(void);
  void vIndexDefNew(void);
  void vIndexDefParse(void);
  void vIndexDefParseOLD(void);
  void vIndexDefSet(void);
  void vIndexDefSyntax(void);
  void vIndexDefParseTableName(void);
  void vIndexDefPipeOut(void);
  void vIndexDefTableNameFetch(void);
  void vIndexDefTableNameStore(void);

  // index reg
  void vRegIndexGet(void);
  void vRegIndexSet(void);

  // indexStream
  void vIndexStreamBufferNo(void);
  void vIndexStreamClose(void);
  void vIndexStreamDelete(void);
  void vIndexStreamDeleteNext(void);
  void vIndexStreamGet(void);
  void vIndexStreamIsNull(void);
  void vIndexStreamNew(void);
  void vIndexStreamOpen(void);
  void vIndexStreamRead(void);
  void vIndexStreamSeek(void);
  void vIndexStreamSet(void);

  // index tree
  void vIndexTreeAggregate(void);
  void vIndexTreeAsAlt(void);
  void vIndexTreeAsSrc(void);
  void vIndexTreeAsTmp(void);
  void vIndexTreeDelete(void);
  void vIndexTreeGet(void);
  void vIndexTreeInsEntBuffer(void);
  void vIndexTreeIsNull(void);
  void vIndexTreeNew(void);
  void vIndexTreeSet(void);

  // reg info set
  void vInfoSetAddLockStats(void);
  void vInfoSetAddNodeInfo(void);
  void vInfoSetAddProgressUnits(void);
  void vInfoSetAddRowCount(void);
  void vInfoSetAddTotalUnits(void);
  void vInfoSetChunkFile(void);
  void vInfoSetDelete(void);
  void vInfoSetFirstRow(void);
  void vInfoSetGet(void);
  void vInfoSetInfoRows(void);
  void vInfoSetIsProgress(void);
  void vInfoSetIsTotalUnits(void);
  void vInfoSetName(void);
  void vInfoSetNew(void);
  void vInfoSetNextRow(void);
  void vInfoSetPipeOut(void);
  void vInfoSetRdCsvChunk(void);
  void vInfoSetSet(void);
  void vInfoSetWriteCsv(void);

  // reg infoSet table
  void vInfoSetTableColumnFetch(void);
  void vInfoSetTableDump(void);
  void vInfoSetTableGet(void);
  void vInfoSetTableSet(void);

  // input
  void vInputChars(void);
  void vInputClose(void);
  void vInputCppTokens(void);
  void vInputDump(void);
  void vInputInspect(void);
  void vInputKind(void);
  void vInputNext(void);
  void vInputOpen(void);
  void vInputPop(void);
  void vInputPush(void);
  void vInputRead(void);
  void vInputSenTokens(void);
  void vInputStdin(void);
  void vInputUnknown(void);
  void vInputUseBegin(void);
  void vInputUseEnd(void);
  void vInputUseInclude(void);
  void vInputUseString(void);

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
  void vIntPercent(void);
  void vIntRandom(void);
  void vIntSubtract(void);
  void vUnsignedIntRandom(void);

  // job
  void vJobAcksRemaining(void);
  void vJobAddDemon(void);
  void vJobAddLocal(void);
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
  void vJobCreateBufferQueue(void);
  void vJobCreateResponseVec(void);

  void vJobDec2pcAcksRmg(void);
  void vJobDelete(void);

  void vJobTagSetFetch(void);
  void vJobErrorStore(void);
  void vJobErrorFetch(void);

  void vJobFindOriginator(void);
  void vJobFindParticipant(void);

  void vJobHasError(void);

  void vJobIdFetch(void);
  void vJobIsExecuting(void);

  void vJobJobClass(void);
  void vJobJobListFetch(void);

  void vJobKindFetch(void);

  void vJobNameFetch(void);
  void vJobNextOperand(void);
  void vJobNoWaitFetch(void);

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
  void vJobPhaseFetch(void);
  void vJobPhaseStore(void);
  void vJobPostRecv(void);
  void vJobPreSend(void);
  void vJobPrintMember(void);

  void vJobRemoveLocal(void);
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
  void vJobSignalCompletion(void);
  void vJobSlotFetch(void);
  void vJobScheduleNextPhase(void);
  void vJobSpawnReceived(void);
  void vJobStateDiagram(void);

  void vJobTaskFetch(void);
  void vJobTaskStore(void);

  // job ref
  void vJobRefHasTask(void);
  void vJobRefPointer(void);
  void vJobRefNone(void);
  void vJobRefReturn(void);
  void vJobRefReferences(void);

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

  // job Monitor
  void vJob_MonitorBegin(void);
  void vJob_MonitorTake(void);
  void vJob_MonitorEnd(void);

  // job NetHog
  void vJob_NetHogHogNoFetch(void);

  // job Pipeline
  void vJob_PipelineBegin(void);
  void vJob_PipelineCancelled(void);
  void vJob_PipelineCompile(void);
  void vJob_PipelineDeleteDynamo(void);
  void vJob_PipelineErrored(void);
  void vJob_PipelineExecute(void);
  void vJob_PipelineIsCancelled(void);
  void vJob_PipelineIsErrored(void);
  void vJob_PipelineIsPulsed(void);
  void vJob_PipelinePulsed(void);

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

  // job SubWalker
  void vJob_SubWalkerBegin(void);
  void vJob_SubWalkerChunk(void);
  void vJob_SubWalkerEnd(void);
  void vJob_SubWalkerWalkJobFetch(void);
  void vJob_SubWalkerSpawn(void);

  // job Walk
  void vJob_WalkBegin(void);
  void vJob_WalkEnd(void);
  void vJob_WalkIndexStore(void);
  void vJob_WalkTableStore(void);

  // kbase
  void vKbaseCallSchedule(void);
  void vKbaseDelete(void);
  void vKbaseFindFactNm(void);
  void vKbaseFindFactNo(void);
  void vKbaseGet(void);
  void vKbaseMarkerFetch(void);
  void vKbaseMarkerNew(void);
  void vKbaseMarkerStore(void);
  void vKbaseNew(void);
  void vKbasePrint(void);
  void vKbaseSeed(void);
  void vKbaseSet(void);
  void vKbaseTest(void);
  void vKbaseTransformFetch(void);
  void vKbaseTransformNew(void);
  void vKbaseTransformStore(void);
  void vKbaseTrim(void);
  void vKbaseTroll(void);

  // loader
  void vLoaderCreate(void);
  void vLoaderDelete(void);

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

  // logic compiler
  void vLogicCompilerCompile(void);
  void vLogicCompilerDelete(void);
  void vLogicCompilerExtract(void);
  void vLogicCompilerNew(void);

  // logic unit
  void vLogicUnitAltIndexFetch(void);
  void vLogicUnitAltTableFetch(void);
  void vLogicUnitCallAgainFetch(void);
  void vLogicUnitCallAgainStore(void);
  void vLogicUnitCalledAgainFetch(void);
  void vLogicUnitCalledAgainStore(void);
  void vLogicUnitDiaryAction(void);
  void vLogicUnitDiaryDir(void);
  void vLogicUnitDiaryFetch(void);
  void vLogicUnitDiaryJobNote(void);
  void vLogicUnitDiaryName(void);
  void vLogicUnitDiaryNote(void);
  void vLogicUnitDiaryTitle(void);
  void vLogicUnitDynFun(void);
  void vLogicUnitDynFunArgs(void);
  void vLogicUnitGenAsm(void);
  void vLogicUnitGenCode(void);
  void vLogicUnitDevFetch(void);
  void vLogicUnitDevStore(void);
  void vLogicUnitHandlerFetch(void);
  void vLogicUnitHandlerStore(void);
  void vLogicUnitInit(void);
  void vLogicUnitSrcIndexFetch(void);
  void vLogicUnitSrcTableFetch(void);
  void vLogicUnitTransform(void);
  void vLogicUnitTransformArgs(void);

  // machine
  void vMachineCoreCount(void);
  void vMachineCpuFreq(void);
  void vMachineHostName(void);
  void vMachineMemory(void);

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

  // now
  void vNowAsOf(void);
  void vNowDateStr(void);
  void vNowSinceEpoch(void);
  void vNowTimeStr(void);
  void vNowTimestampStr(void);

  // object
  void vObjectClassFetch(void);
  void vObjectClassStore(void);
  void vObjectFetch(void);
  void vObjectMagicMark(void);
  void vObjectStore(void);

  // olist (originator list)
  void vOlistFindJob(void);

  // operator
  void vOpAckPipelineJob(void);
  void vOpNextOpStore(void);
  void vOpNextStageStore(void);
  void vOpOpNameFetch(void);
  void vOpPartitionFetch(void);
  void vOpPiplineJobFetch(void);
  void vOpPipeOut(void);
  void vOpQueueLength(void);
  void vOpStageNameFetch(void);
  void vOpTopologyStore(void);
  void vOpXmitKindStore(void);

  // opsys
  void vOpSysIsLinux(void);
  void vOpSysIsMacOs(void);

  // persistence
  void vPersistenceCheckpoint(void);
  void vPersistenceReload(void);

  // pipeline
  void vPipelinePipeout(void);

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
  void vPlannerChoosePathsN(void);
  void vPlannerChosenPathN(void);
  void vPlannerDeleteN(void);
  void vPlannerFetchTechniqueFetchN(void);
  void vPlannerGenPathsN(void);
  void vPlannerGetN(void);
  void vPlannerHighKeyFetchN(void);
  void vPlannerIndexNmFetchN(void);
  void vPlannerIsNullN(void);
  void vPlannerKeyFetchN(void);
  void vPlannerLowKeyFetchN(void);
  void vPlannerMatchPathsN(void);
  void vPlannerNewN(void);
  void vPlannerParseWhereN(void);
  void vPlannerPhrasesFetchN(void);
  void vPlannerScorePathsN(void);
  void vPlannerSetN(void);
  void vPlannerShowPathsN(void);
  void vPlannerTableFetchN(void);
  void vPlannerTableStoreN(void);

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

  // pot
  void vPotCompleted(void);
  void vPotCompletedPtn(void);
  void vPotDangling(void);
  void vPotDanglingPtn(void);
  void vPotDelete(void);
  void vPotNew(void);
  void vPotUseStreamDefOut(void);

  // procedure
  void vProcedureAdd(void);

  // process
  void vProcessGetPid(void);
  void vProcessQuit(void);
  void vProcessTestPid(void);

  // profile
  void vProfileEnd(void);
  void vProfileNameStore(void);
  void vProfileStart(void);

  // prompt
  void vPromptContinue(void);
  void vPromptExecuteFunction(void);
  void vPromptGetCommand(void);
  void vPromptNew(void);
  void vPromptSession(void);

  // ptnStream
  void vPtnStreamBufferNo(void);
  void vPtnStreamClose(void);
  void vPtnStreamDelete(void);
  void vPtnStreamGet(void);
  void vPtnStreamIsNull(void);
  void vPtnStreamNew(void);
  void vPtnStreamOpen(void);
  void vPtnStreamRead(void);
  void vPtnStreamSeek(void);
  void vPtnStreamSet(void);

  // push
  void vPushI(void);
  void vPushJ(void);
  void vPushNullPtr(void);

  void vPushFipVal(void);
  void vPushIntVal(void);
  void vPushPtrVal(void);
  void vPushStzVal(void);

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

  // reg agg timer
  void vRegAggTimerGet(void);
  void vRegAggTimerSet(void);

  // Consume
  void vRegXmitsDelete(void);
  void vRegXmitsGet(void);
  void vRegXmitsHandleCancel(void);
  void vRegXmitsHandleEos(void);
  void vRegXmitsHandleError(void);
  void vRegXmitsHandleExit(void);
  void vRegXmitsHandlePulse(void);
  void vRegXmitsHandlePulsed(void);
  void vRegXmitsHandleStart(void);
  void vRegXmitsHandleStreamDef(void);
  void vRegXmitsIsNull(void);
  void vRegXmitsNew(void);

  // reg counter
  void vRegCounter0Get(void);
  void vRegCounter1Get(void);
  void vRegCounter2Get(void);
  void vRegCounter0Set(void);
  void vRegCounter1Set(void);
  void vRegCounter2Set(void);

  // reg folder
  void vRegFolderFindSymbol(void);

  // reg job
  void vRegJobGet(void);

  // reg now
  void vRegNowGet(void);

  // reg op
  void vRegOpGet(void);
  void vRegOpHandlerFetch(void);
  void vRegOpHandlerStore(void);
  void vRegOpHandlerAllStore(void);
  void vRegOpInit(void);

  // operand
  void vRegOperandAddColumnDef(void);
  void vRegOperandAsSrcCrwBuffer(void);
  void vRegOperandAsSrcRowSlab(void);
  void vRegOperandCsvAppend(void);
  void vRegOperandBide(void);
  void vRegOperandCrwDuplicate(void);
  void vRegOperandCrwToCsv(void);
  void vRegOperandCrwToRsb(void);
  void vRegOperandCsvData(void);
  void vRegOperandCsvToCrwH(void);
  void vRegOperandCsvToRsb(void);
  void vRegOperandDelete(void);
  void vRegOperandGet(void);
  void vRegOperandIsNotNull(void);
  void vRegOperandIsNull(void);
  void vRegOperandIsReport(void);
  void vRegOperandInsdelIntoTable(void);
  void vRegOperandInvokePython(void);
  void vRegOperandInvokePythonShell(void);
  void vRegOperandKind(void);
  void vRegOperandNewEosMarker(void);
  void vRegOperandNewExitMarker(void);
  void vRegOperandNewExitedMarker(void);
  void vRegOperandNewCsvBuffer(void);
  void vRegOperandNewErrorMarker(void);
  void vRegOperandNewErroredMarker(void);
  void vRegOperandNewPulseMarker(void);
  void vRegOperandNewPulsedMarker(void);
  void vRegOperandNewStartMarker(void);
  void vRegOperandNewStartedMarker(void);
  void vRegOperandNewStreamDef(void);
  void vRegOperandNext(void);
  void vRegOperandPipeIn(void);
  void vRegOperandPipeOut(void);
  void vRegOperandPipeOutSet(void);
  void vRegOperandPipeToJob(void);
  void vRegOperandReport(void);
  void vRegOperandRowCountFetch(void);
  void vRegOperandSet(void);
  void vRegOperandUpsertIntoTable(void);
  void vRegOperandRsbToCsv(void);
  void vRegOperandWriteToFile0(void);

  // operandB
  void vRegOperandBAppendClose(void);
  void vRegOperandBDelete(void);
  void vRegOperandBGet(void);
  void vRegOperandBIsNotNull(void);
  void vRegOperandBIsNull(void);
  void vRegOperandBPipeOut(void);
  void vRegOperandBSet(void);

  // operandC
  void vRegOperandCCollect(void);
  void vRegOperandCGet(void);
  void vRegOperandCPipeOut(void);
  void vRegOperandCPipeOutSet(void);
  void vRegOperandCSet(void);
  void vRegOperandCSetFirst(void);

  // OperandOoO
  void vRegOperandOoODelete(void);
  void vRegOperandOoOGet(void);
  void vRegOperandOoOIsNull(void);
  void vRegOperandNextOoO(void);
  void vRegOperandOoOPipeOut(void);
  void vRegOperandOoOPipeToJob(void);
  void vRegOperandOoOSet(void);

  // reg Path
  void vRegPathGet(void);

  // dicStack
  void vRegPathDepth(void);
  void vRegPathDiscard(void);
  void vRegPathDump(void);
  void vRegPathPick(void);
  void vRegPathPush(void);
  void vRegPathSave(void);
  void vRegPathRestore(void);

  // reg Pipein
  void vRegPipeinBide(void);
  void vRegPipeinDelete(void);
  void vRegPipeinGet(void);
  void vRegPipeinIsNull(void);
  void vRegPipeinKind(void);
  void vRegPipeinNext(void);
  void vRegPipeinPeek(void);
  void vRegPipeinPipeOut(void);
  void vRegPipeinPipeToJob(void);
  void vRegPipeinSet(void);
  void vRegPipeinWriteProfile(void);

  // reg task
  void vRegTaskGet(void);

  // reg timer
  void vRegTimerGet(void);
  void vRegTimerSet(void);

  // register
  void vRegister0Get(void);
  void vRegister0Set(void);

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

  // row
  void vRowGet(void);
  void vRowGetIntColumn(void);
  void vRowGetVchColumn(void);
  void vRowIsNull(void);
  void vRowSet(void);

  // rowOnce
  void vRowOnceDelete(void);
  void vRowOnceGet(void);
  void vRowOnceIsNull(void);
  void vRowOnceNew(void);

  // rsb buffer
  void vRsbBufferOperandDump(void);

  // rstack
  void vRstackCsPick(void);
  void vRstackCsRoll(void);
  void vRstackDepth(void);
  void vRstackDiscard(void);
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

  // csv buffer
  void vSeekOffsetsOperandDump(void);

  // session
  void vSessionCommand(void);
  void vSessionDicStackFetch(void);
  void vSessionDump(void);
  void vSessionTagSetFetch(void);
  void vSessionIsExiting(void);
  void vSessionPid(void);
  void vSessionSlotFetch(void);
  void vSessionThis(void);
  void vSessionToDoFetch(void);
  void vSessionToDoStore(void);
  void vSessionTxFetch(void);
  void vSessionTxStore(void);

  // shell
  void vShellExecute(void);

  // slots
  void vSlotsColumnFetch(void);
  void vSlotsColumnCountFetch(void);
  void vSlotsColumnName(void);
  void vSlotsCreateStreamDef(void);
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
  void vMetricsLockSample(void);

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
  void vStrGleanInt(void);
  void vStrLeft(void);
  void vStrLength(void);
  void vStrPrepend(void);
  void vStrRandom(void);
  void vStrRemovePrefix(void);
  void vStrRemoveSuffix(void);
  void vStrRemoveTailInt(void);
  void vStrReplace(void);
  void vStrRight(void);
  void vStrSame(void);
  void vStrSub(void);
  void vStrTailInt(void);
  void vStrToStrVec(void);

  // register stringA
  void vStringAGet(void);
  void vStringASet(void);

  // stream def
  void vStreamDefAsStr(void);
  void vStreamDefAsSrcStreamDef(void);
  void vStreamDefColumnFetch(void);
  void vStreamDefColumnCountFetch(void);
  void vStreamDefDelete(void);
  void vStreamDefDuplicate(void);
  void vStreamDefGet(void);
  void vStreamDefIsNull(void);
  void vStreamDefName(void);
  void vStreamDefNew(void);
  void vStreamDefNewCount(void);
  void vStreamDefNewToCsv(void);
  void vStreamDefParse(void);
  void vStreamDefPipeIn(void);
  void vStreamDefPipeOut(void);
  void vStreamDefPrint(void);
  void vStreamDefRowSize(void);
  void vStreamDefSet(void);

  // stream definition (out)
  void vStreamDefOutAddColumn(void);
  void vStreamDefOutAddColumns(void);
  void vStreamDefOutDelete(void);
  void vStreamDefOutDuplicate(void);
  void vStreamDefOutGet(void);
  void vStreamDefOutIsNull(void);
  void vStreamDefOutNameFetch(void);
  void vStreamDefOutNameStore(void);
  void vStreamDefOutNew(void);
  void vStreamDefOutPipeout(void);
  void vStreamDefOutPrint(void);
  void vStreamDefOutSet(void);

  // reg stream set
  void vStreamSetAddAnalyzeIndex(void);
  void vStreamSetAddAnalyzeTable(void);
  void vStreamSetAddColumnDef(void);
  void vStreamSetAddGridSanity(void);
  void vStreamSetAddListContents(void);
  void vStreamSetAddListFolder(void);
  void vStreamSetAddListIndex(void);
  void vStreamSetAddListPartitions(void);
  void vStreamSetAddNodeStatus(void);
  void vStreamSetAddPingInfo(void);
  void vStreamSetAddProgressUnits(void);
  void vStreamSetAddPwfInfo(void);
  void vStreamSetAddRowCount(void);
  void vStreamSetAddShowCluster(void);
  void vStreamSetAddSizeIndex(void);
  void vStreamSetAddSizeTable(void);
  void vStreamSetAddSpaceInfo(void);
  void vStreamSetAddTableSizes(void);
  void vStreamSetAddVerifyIndex(void);
  void vStreamSetAddVerifyTable(void);
  void vStreamSetDelete(void);
  void vStreamSetFirstRow(void);
  void vStreamSetGet(void);
  void vStreamSetInfoRows(void);
  void vStreamSetName(void);
  void vStreamSetNew(void);
  void vStreamSetNextRow(void);
  void vStreamSetPipeOut(void);
  void vStreamSetReadChunk(void);
  void vStreamSetSet(void);

  // reg infoSet table
  void vStreamSetTableColumnFetch(void);
  void vStreamSetTableDump(void);
  void vStreamSetTableGet(void);
  void vStreamSetTableSet(void);

  // sub loader
  void vSubLoaderDelete(void);

  // subset
  void vSubsetGet(void);
  void vSubsetNext(void);
  void vSubsetNextBuffer(void);
  void vSubsetSet(void);

  // table
  void vTableAddIndex(void);
  void vTableAsAltStreamDef(void);
  void vTableAsSrcStreamDef(void);
  void vTableColumnCountFetch(void);
  void vTableColumnFetch(void);
  void vTableCreatePtnDef(void);
  void vTableDelete(void);
  void vTableDump(void);
  void vTableFindColumn(void);
  void vTableGenCsvBuffer(void);
  void vTableGet(void);
  void vTableIndexCount(void);
  void vTableIsColumn(void);
  void vTableIsDefinitionOnlyFetch(void);
  void vTableIsDefinitionOnlyStore(void);
  void vTablePtnStreamDefFetch(void);
  void vTableLazyBuildIndexes(void);
  void vTableNew(void);
  void vTablePublish(void);
  void vTablePurge(void);
  void vTableDeleteIndex(void);
  void vTableScan(void);
  void vTableSet(void);
  void vTableSize(void);
  void vTableStreamDefFetch(void);
  void vTableStreamDefStore(void);
  void vTableTablePtnFetch(void);
  void vTableTablePtnZero(void);
  void vTableVerify(void);
  void vTableVisualize(void);

  // table definition
  void vTableDefParse(void);
  void vTableDefSyntax(void);

  // table partition
  void vTablePtnAttemptGrabRead(void);
  void vTablePtnAttemptGrabWrite(void);
  void vTablePtnDropRead(void);
  void vTablePtnDropWrite(void);

  // table partition
  void vTablePtnAppendBuffer(void);
  void vTablePtnGet(void);
  void vTablePtnGenCrwBuffers(void);
  void vTablePtnInflateCrwBuffer(void);
  void vTablePtnPopulateIndex(void);
  void vTablePtnPurge(void);
  void vTablePtnRoot(void);
  void vTablePtnSet(void);
  void vTablePtnTruncate(void);
  void vTablePtnTruncateIndex(void);

  // tag
  void vTagGet(void);
  void vTagSet(void);

  // task
  void vTaskAck(void);
  void vTaskAcksStore(void);
  void vTaskBreakPoint(void);
  void vTaskDestructorTraceFetch(void);
  void vTaskDestructorTraceStore(void);
  void vTaskDoneFinally(void);
  void vTaskDropLock(void);
  void vTaskDump(void);
  void vTaskDelete(void);
  void vTaskGrabLock(void);
  void vTaskIsAckSet(void);
  void vTaskIsWaiting(void);
  void vTaskJobFetch(void);
  void vTaskJobStore(void);
  void vTaskNameFetch(void);
  void vTaskProfileFetch(void);
  void vTaskProfileStore(void);
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

  // tenants
  void vTenantsDelete(void);
  void vTenantsNew(void);

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
  void vTestTest26(void);
  void vTestTest27(void);
  void vTestTest28(void);
  void vTestTest29(void);
  void vTestTest30(void);
  void vTestTest31(void);
  void vTestTest32(void);
  void vTestTest33(void);
  void vTestTest34(void);
  void vTestTest35(void);
  void vTestTest36(void);
  void vTestTest37(void);
  void vTestTest38(void);
  void vTestTest39(void);
  void vTestTest40(void);
  void vTestTest41(void);
  void vTestTest42(void);
  void vTestTest43(void);
  void vTestTest44(void);
  void vTestTest45(void);
  void vTestTest46(void);
  void vTestTest47(void);
  void vTestTest48(void);
  void vTestTest49(void);
  void vTestTest50(void);
  void vTestTest51(void);
  void vTestTest52(void);
  void vTestTest53(void);
  void vTestTest54(void);
  void vTestTest55(void);
  void vTestTest56(void);
  void vTestTest57(void);
  void vTestTest58(void);
  void vTestTest59(void);
  void vTestTest60(void);
  void vTestTest61(void);
  void vTestTest62(void);
  void vTestTest63(void);
  void vTestTest64(void);

  // text
  void vTextDelete(void);
  void vTextNew(void);

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

  // cpu
  void vTickingGetTick(void);
  void vTickingTicksPerSec(void);

  // timer
  void vTimerDelete(void);
  void vTimerInit(void);
  void vTimerMicrosecs(void);
  void vTimerMillisecs(void);
  void vTimerNanosecs(void);
  void vTimerNew(void);
  void vTimerReportStr(void);
  void vTimerReset(void);
  void vTimerSecs(void);
  void vTimerStart(void);
  void vTimerTicks(void);
  void vTimerUpdate(void);

  // time
  void vTimeCurrent(void);

  // to
  void vToInt(void);
  void vToPtr(void);
  void vToStr(void);
  void vToTask(void);

  // toHshCrw
  void vToHshCrwRegCompleted(void);
  void vToHshCrwRegConvert(void);
  void vToHshCrwRegCreate(void);
  void vToHshCrwRegDangling(void);
  void vToHshCrwRegDelete(void);

  // toNstRsb
  void vToNstRsbRegConvert(void);
  void vToNstRsbRegCreate(void);
  void vToNstRsbRegDelete(void);

  // toPtnRsb
  void vToPtnRsbRegCompleted(void);
  void vToPtnRsbRegConvert(void);
  void vToPtnRsbRegCreate(void);
  void vToPtnRsbRegDangling(void);
  void vToPtnRsbRegDelete(void);

  // toRsb
  void vToRsbRegCompleted(void);
  void vToRsbRegConvert(void);
  void vToRsbRegCreate(void);
  void vToRsbRegDangling(void);
  void vToRsbRegDelete(void);

  // tokenVec
  void vTokenVecDelete(void);
  void vTokenVecDump(void);
  void vTokenVecNew(void);
  void vTokenVecTokenise(void);

  // trace
  void vTraceOff(void);
  void vTraceOn(void);
  void vTracePoint(void);
  void vTraceSet(void);

  // tx
  void vTx0Begin(void);
  void vTx0Commit(void);
  void vTxDumpAll(void);
  void vTx0Perspective(void);

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

  // user
  void vUserName(void);

  // utility
  void vUtilityPrefetch(void);
  void vUtilitySchemaTest(void);

  // vortex
  void vVortexClose(void);
  void vVortexDelete(void);
  void vVortexNew(void);
  void vVortexOpen(void);
  void vVortexProcessH(void);
  void vVortexProcessHpp(void);

  // word
  void vWord(void);
  void vWordAllot(void);
  void vWordBreakPtFetch(void);
  void vWordBreakPtStore(void);
  void vWordCategoryFetch(void);
  void vWordCategoryStore(void);
  void vWordDelete(void);
  void vWordDump(void);
  void vWordEmbObject(void);
  void vWordEnmSetFetch(void);
  void vWordEnmSetStore(void);
  void vWordExitHookGet(void);
  void vWordExitHookSet(void);
  void vWordFind(void);
  void vWordImmediateGet(void);
  void vWordImmediateSet(void);
  void vWordIncludeFetch(void);
  void vWordIncludeStore(void);
  void vWordIsImmediate(void);
  void vWordIsIntrinsic(void);
  void vWordKindFetch(void);
  void vWordKindStore(void);
  void vWordLocate(void);
  void vWordNameFetch(void);
  void vWordNameStore(void);
  void vWordNew(void);
  void vWordObjectFetch(void);
  void vWordObjectStore(void);
  void vWordPrev(void);
  void vWordResetRoutine(void);
  void vWordSee(void);
  void vWordXcheckInxPtr(void);

  // write behind
  void vWriteBehindCreate(void);
  void vWriteBehindDelete(void);
  void vWriteBehindGet(void);
  void vWriteBehindHandleDangling(void);
  void vWriteBehindset(void);

  // write hash
  void vWriteHashCreate(void);
  void vWriteHashDelete(void);
  void vWriteHashCrwBufferFetch(void);
  void vWriteHashCrwBufferStore(void);
  void vWriteHashFullCrwBufferFetch(void);

  // XmitOperand
void vXmitOperandOperandStore(void);
  void vXmitOperandPipeOut(void);

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
