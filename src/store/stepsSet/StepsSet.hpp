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

  // adaptorWL
  void vAdaptorProcessEventA(void);
  void vAdaptorProcessEventB(void);
  void vAdaptorWLAction1(void);
  void vAdaptorWLAction2(void);
  void vAdaptorWLDelete(void);
  void vAdaptorWLNew(void);

  // AnswerWL
  void vAnswerProcessEventA(void);
  void vAnswerProcessEventB(void);
  void vAnswerWLAction1(void);
  void vAnswerWLAction2(void);
  void vAnswerWLDelete(void);
  void vAnswerWLNew(void);

  // address
  void vAddressEqual(void);
  void vAddressFetch(void);
  void vAddressStore(void);

  // agg timer
  void vAggTimerAdd(void);
  void vAggTimerDelete(void);
  void vAggTimerGet(void);
  void vAggTimerMicrosecs(void);
  void vAggTimerMillisecs(void);
  void vAggTimerNanosecs(void);
  void vAggTimerNew(void);
  void vAggTimerReportStr(void);
  void vAggTimerReset(void);
  void vAggTimerSecs(void);
  void vAggTimerSet(void);
  void vAggTimerTicks(void);

  // AM
  void vAM_No_Free(void);
  void vAM_Show_Boot(void);
  void vAM_Show_Dir(void);
  void vAM_Show_Dir_Failure(void);
  void vAM_Show_Connection(void);
  void vAM_Show_Exception(void);
  void vAM_Show_File(void);
  void vAM_Show_File_Failure(void);
  void vAM_Show_LowMem(void);
  void vAM_Show_LowMem_Recv(void);
  void vAM_Show_Recurring(void);
  void vAM_Show_Socket(void);
  void vAM_Show_Socket_Failure(void);

  // api
  void vApiAddProcedure(void);
  void vApiDerefMetricsDisable(void);
  void vApiDerefMetricsEnable(void);
  void vApiExecuteCommand(void);
  void vApiForkCommand(void);
  void vApiJobMetricsDisable(void);
  void vApiJobMetricsEnable(void);
  void vApiReapDisable(void);
  void vApiReapEnable(void);
  void vApiReapJobs(void);
  void vApiSpawnDemon(void);
  void vApiSysMetricsDisable(void);
  void vApiSysMetricsEnable(void);

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
  void vAstackRot(void);
  void vAstackRotInv(void);
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
  void vAttachMapAddSysMetrics(void);
  void vAttachMapErrorTextFetch(void);
  void vAttachMapIsLowMemory(void);
  void vAttachMapOperandsFetch(void);
  void vAttachMapRegionSizeFetch(void);
  void vAttachMapTenantsFetch(void);
  void vAttachMapTenantsStore(void);

  // bkt map
  void vBktMapSee(void);

  // bp
  void vBpReadBufferPool(void);
  void vBpWriteBufferPool(void);
  void vBpFreeBuffers(void);
  void vBpFreePercent(void);
  void vBpFreeSpace(void);
  void vBpInUseSpace(void);
  void vBpTotalSpace(void);

  // bucket
  void vBucketGetGetBucketData(void);
  void vBucketGetGetBucketTable(void);
  void vBucketGetGetBucket(void);
  void vBucketGetGetBucketNumber(void);
  void vBucketSendGetBucketCrw(void);
  void vBucketGetGetTablePtn(void);
  void vBucketSendGetBucketPtnStream(void);
  void vBucketGet(void);
  void vBucketGetCrw(void);
  void vBucketGetAndClear(void);
  void vBucketGetNew(void);
  void vBucketSet(void);
  void vBucketIsNotNull(void);
  void vBucketIsNull(void);
  void vBucketDelete(void);
  void vBucketRecvPutParts(void);

  // BktMapGet
  void vBktMapDelete(void);
  void vBktMapGet(void);
  void vBktMapGetAndClear(void);
  void vBktMapIsNotNull(void);
  void vBktMapIsNull(void);
  void vBktMapSet(void);
  void vBktMapGetGetBktMapdata(void);
  void vBktMapGetGetTableName(void);

  // BktMapSend
  void vBktMapSendGetParts(void);
  void vBktMapSendGetPartsByName(void);

  // BktMapRecv
  void vBktMapRecvPutParts(void);

  // build
  void vBuildBranch(void);
  void vBuildDate(void);
  void vBuildGitHash(void);
  void vBuildHostName(void);
  void vBuildUserName(void);

  // class
  void vClassName(void);
  void vClassPostRecvWordStore(void);
  void vClassPreSendWordStore(void);

  // column
  void vColumnIsNullableFetch(void);
  void vColumnIsPartitionKeyFetch(void);
  void vColumnLengthFetch(void);
  void vColumnNameFetch(void);
  void vColumnScaleFetch(void);
  void vColumnTypeFetch(void);

  // Combined
  void vCombinedDelete(void);
  void vCombinedExecute(void);
  void vCombinedHashRowsFetch(void);
  void vCombinedGet(void);
  void vCombinedIndexT(void);
  void vCombinedNew(void);

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

  // counter
  void vCounterAGet(void);
  void vCounter0Get(void);
  void vCounter1Get(void);
  void vCounter2Get(void);
  void vCounter3Get(void);
  void vCounter4Get(void);
  void vCounter5Get(void);
  void vCounter6Get(void);
  void vCounter7Get(void);

  void vCounterASet(void);
  void vCounter0Set(void);
  void vCounter1Set(void);
  void vCounter2Set(void);
  void vCounter3Set(void);
  void vCounter4Set(void);
  void vCounter5Set(void);
  void vCounter6Set(void);
  void vCounter7Set(void);

  // crwBuffer
  void vCrwBufferDeleteOld(void);
  void vCrwBufferDuplicate(void);
  void vCrwBufferOperandDump(void);
  void vCrwBufferNew(void);
  void vCrwBufferSnapshot(void);

  void vCrwBufferAppendClose(void);
  void vCrwBufferDelete(void);
  void vCrwBufferGet(void);
  void vCrwBufferGetAndClear(void);
  void vCrwBufferFlatten(void);
  void vCrwBufferInflate(void);
  void vCrwBufferInflateTcc(void);
  void vCrwBufferInsdelIntoTable(void);
  void vCrwBufferIsNotNull(void);
  void vCrwBufferIsNull(void);
  void vCrwBufferIsReport(void);
  void vCrwBufferSet(void);
  void vCrwBufferToCsv(void);
  void vCrwBufferToCsv_OLD(void);
  void vCrwBufferWriteToFile0(void);

  // crw to row
  void vCrwToRowCompleted(void);
  void vCrwToRowConvert(void);
  void vCrwToRowCreate(void);
  void vCrwToRowDangling(void);
  void vCrwToRowDelete(void);

  // csv buffer
  void vCsvBufferNewOld(void);
  void vCsvBufferOperandDump(void);
  void vCsvBufferPrint(void);

  void vCsvBufferCsvAppend(void);
  void vCsvBufferCsvData(void);
  void vCsvBufferDelete(void);
  void vCsvBufferGet(void);
  void vCsvBufferGetAndClear(void);
  void vCsvBufferIsNotNull(void);
  void vCsvBufferIsNull(void);
  void vCsvBufferIsReport(void);
  void vCsvBufferNew(void);
  void vCsvBufferReport(void);
  void vCsvBufferSet(void);
  void vCrwBufferUpsertIntoTable(void);
  void vCsvBufferWriteToFile0(void);

  // cursor
  void vCursorFetch(void);
  void vCursorStore(void);

  // dataBuffer
  void vDataBufferDelete(void);
  void vDataBufferGet(void);
  void vDataBufferGetAndClear(void);
  void vDataBufferIsNotNull(void);
  void vDataBufferIsNull(void);
  void vDataBufferSet(void);

  // ddllock
  void vDdlLockDropRead(void);
  void vDdlLockDropWrite(void);
  void vDdlLockGrabRead(void);
  void vDdlLockGrabWrite(void);
  void vDdlLockNone(void);
  void vDdlLockTestDrop(void);

  // debug
  void vDebugFlagFetch(void);
  void vDebugFlagStore(void);
  void vDebugBreakpoint(void);

  // deck
  void vDeckAddAccum(void);
  void vDeckCancelledFetch(void);
  void vDeckCancelledStore(void);
  void vDeckCreate(void);
  void vDeckDropDebugLock(void);
  void vDeckDump(void);
  void vDeckEarlyExitFetch(void);
  void vDeckEarlyExitStore(void);
  void vDeckEnqueueInvoke(void);
  void vDeckErroredStore(void);
  void vDeckErroredFetch(void);
  void vDeckGet(void);
  void vDeckGetAccum(void);
  void vDeckGrabDebugLock(void);
  void vDeckOp0Fetch(void);
  void vDeckOpsExeOnNodeFetch(void);
  void vDeckPulsedFetch(void);
  void vDeckPulsedStore(void);
  void vDeckSet(void);
  void vDeckSetInvoker(void);
  void vDeckSpawnOpTasks(void);

  // deck (of op)
  void vDeckOfOpDump(void);
  void vDeckOfOpGet(void);

  // devLftRow
  void vDevLftRowStreamDefStore(void);
  void vDevLftRowDeleteEachRowBuffer(void);
  void vDevLftRowRowStore(void);
  void vDevLftRowEachRowBufferStore(void);
  void vDevLftRowRowBufferFetch(void);
  void vDevLftRowRowBufferStore(void);
  void vDevLftRowCrwBufferStore(void);

  // devRhtRow
  void vDevRhtRowRowBufferStore(void);
  void vDevRhtRowStreamDefStore(void);

  // devLftIxt
  void vDevLftIxtCrwBufferFetch(void);
  void vDevLftIxtCrwBufferStore(void);
  void vDevLftIxtIndexTreeStore(void);

  // devRhtIxt
  void vDevRhtIxtCrwBufferStore(void);
  void vDevRhtIxtIndexTreeStore(void);

  // dic
  void vDicDelete(void);
  void vDicFirstWord(void);
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

  // Discern
  void vDiscernRdCsvChunk(void);

  // dot
  void vDot(void);
  void vDotBoot(void);
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

  void vDynamoCompile(void);
  void vDynamoDelete(void);
  void vDynamoGet(void);
  void vDynamoNew(void);

  // encode
  void vEncodeCurrentCall(void);
  void vEncodeCurrentPush(void);

  void vEncodeCall(void);
  void vEncodePush(void);

  void vEncodeAgain(void);
  void vEncodeAhead(void);
  void vEncodeContinue(void);
  void vEncodeContinue_if(void);
  void vEncodeHere(void);
  void vEncodeIf(void);
  void vEncodeLoop(void);
  void vEncodeLoopPlus(void);
  void vEncodeNext(void);
  void vEncodeNextPlus(void);
  void vEncodeThen(void);
  void vEncodeUntil(void);

  // entry
  void vEntryMatches(void);
  void vEntrySame(void);
  void vEntryToDataRow(void);

  // entry buffer
  void vEntBufferDump(void);
  void vEntBufferIndexFetch(void);
  void vEntBufferIsLastFetch(void);
  void vEntBufferIsLastStore(void);
  void vEntBufferNextBuffer(void);
  void vEntBufferPartitionFetch(void);
  void vEntBufferPartitionStore(void);
  void vEntBufferToEntBuffer(void);

  // env
  void vEnvGet(void);

  // error code
  void vErrorCodeDecodeError(void);
  void vErrorCodeGet(void);
  void vErrorCodeSet(void);

  // event manager
  void vEvMgrEventCountFetch(void);
  void vEvMgrCurEventNmFetch(void);
  void vEvMgrDecodeHandlerNm(void);
  void vEvMgrDecodeEventNm(void);

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

  // flush
  void vFlushDerefMetrics(void);

  // reg fun
  void vFun0Call(void);
  void vFun0Get(void);
  void vFun0Set(void);
  void vFun0Setup(void);
  void vFun1Call(void);
  void vFun1Get(void);
  void vFun1Set(void);
  void vFun1Setup(void);
  void vFun2Call(void);
  void vFun2Get(void);
  void vFun2Set(void);
  void vFun2Setup(void);
  void vFun3Call(void);
  void vFun3Get(void);
  void vFun3Set(void);
  void vFun3Setup(void);

  // grid
  void vGridDumpInfo(void);
  void vGridGetGrid(void);
  void vGridGetGridParts(void);
  void vGridFindNodeByName(void);
  void vGridKnownNodes(void);
  void vGridFindNodeByNo(void);
  void vGridPutGridParts(void);
  void vGridSeeNodes(void);
  void vGridShow(void);
  void vGridPtnsPerNode(void);
  void vGridTablePtnToPtnNo(void);
  void vGridThisNodeIdFetch(void);
  void vGridThisNodeNameFetch(void);
  void vGridToNode(void);

  void vGridDelete(void);
  void vGridGet(void);
  void vGridGetAndClear(void);
  void vGridIsNotNull(void);
  void vGridIsNull(void);
  void vGridSet(void);

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
  void vIndexSize(void);
  void vIndexVerify(void);
  void vIndexVerifyPtn(void);
  void vIndexVisualize(void);

  // index steps - to go
  void vIndexDelete(void);
  void vIndexDeleteNext(void);
  void vIndexIndexDefFetch(void);
  void vIndexIndexDefStore(void);
  void vIndexIndexTree(void);
  void vIndexIsNull(void);
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
  void vIndexDefTableNameFetch(void);
  void vIndexDefTableNameStore(void);

  // index reg
  void vIndexGet(void);
  void vIndexSet(void);

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

  // indexStream : 2nd use case (sacn indextree)
  void vIndexStreamScanBegin(void);
  void vIndexStreamScanNext(void);
  void vIndexStreamScanNew(void);
  void vIndexStreamScanPopulate(void);
  void vIndexStreamScanPopulateTop(void);

  // index tree
  void vIndexTreeAsAlt(void);
  void vIndexTreeAsSrc(void);
  void vIndexTreeAsTmp(void);
  void vIndexTreeDelete(void);
  void vIndexTreeGet(void);
  void vIndexTreeInsEntBuffer(void);
  void vIndexTreeIsNull(void);
  void vIndexTreeNew(void);
  void vIndexTreeSet(void);

  // infoSet
  void vInfoSetAddCommandError(void);
  void vInfoSetAddExecError(void);
  void vInfoSetAddLockStats(void);
  void vInfoSetAddNodeInfo(void);
  void vInfoSetAddProgressUnits(void);
  void vInfoSetAddRowCount(void);
  void vInfoSetAddRowVargs(void);
  void vInfoSetAddTotalUnits(void);
  void vInfoSetChunkFile(void);
  void vInfoSetColValFetch(void);
  void vInfoSetDelete(void);
  void vInfoSetFirstRow(void);
  void vInfoSetGet(void);
  void vInfoSetInfoRows(void);
  void vInfoSetIsNotNull(void);
  void vInfoSetIsNull(void);
  void vInfoSetIsProgress(void);
  void vInfoSetIsTotalUnits(void);
  void vInfoSetName(void);
  void vInfoSetNew(void);
  void vInfoSetNextRow(void);
  void vInfoSetOpInfosetFetch(void);
  void vInfoSetOpInfosetStore(void);
  void vInfoSetRdCsvChunk(void);
  void vInfoSetRowGet(void);
  void vInfoSetRowGetIntColumn(void);
  void vInfoSetRowGetVchColumn(void);
  void vInfoSetRowIntColumnFetch(void);
  void vInfoSetRowIsNull(void);
  void vInfoSetRowSet(void);
  void vInfoSetSet(void);
  void vInfoSetWriteCsv(void);
  void vInfoSetTableColumnFetch(void);
  void vInfoSetTableDump(void);
  void vInfoSetTableGet(void);
  void vInfoSetTableSet(void);

  // inject
  void vInjectBloat(void);
  void vInjectBloatCleanup(void);
  void vInjectSigsegv(void);
  void vInjectSigsegv_dbg(void);
  void vInjectThrow(void);

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
  void vInputPushVecAsStr(void);
  void vInputRead(void);
  void vInputSenTokens(void);
  void vInputStdin(void);
  void vInputTillSemicolon(void);
  void vInputUnknown(void);
  void vInputUseBegin(void);
  void vInputUseEnd(void);
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

  // intStack
  void vIntStackDelete(void);
  void vIntStackItemsFetch(void);
  void vIntStackNew(void);
  void vIntStackPop(void);
  void vIntStackPush(void);

  // job
  void vJobAddDemon(void);
  void vJobAddLocal(void);
  void vJobAddOriginator(void);
  void vJobAddParticipant(void);
  void vJobAddRetired(void);
  void vJobAssertBegin(void);
  void vJobAssertStructure(void);
  void vJobAssertOrdinary(void);
  void vJobAssertOriginator(void);
  void vJobAssertParticipant(void);
  void vJobAssertReceived(void);

  void vJobClass(void);

  void vJobDelete(void);

  void vJobEntryPointFetch(void);
  void vJobEntryFetch_other(void);
  void vJobEntryPointStore(void);
  void vJobEntryStore_other(void);
  void vJobErrorFetch(void);
  void vJobErrorStore(void);

  void vJobFolderPathFetch(void);
  void vJobFolderPathStore(void);

  void vJobGet(void);

  void vJobIdFetch(void);
  void vJobIdFetch_other(void);

  void vJobJobClass(void);
  void vJobJobListFetch(void);

  void vJobKindFetch(void);

  void vJobNameFetch(void);
  void vJobNoWaitFetch(void);

  void vJobOriginJob(void);
  void vJobOriginNodeIdFetch(void);

  void vJobParticipantNodeIdFetch(void);
  void vJobParticipantNodeIdStore(void);
  void vJobPipeToOp0(void);
  void vJobPreExecuteTest(void);
  void vJobPhaseFetch(void);
  void vJobPhaseFetch_other(void);
  void vJobPhaseStore(void);
  void vJobPrintMember(void);
  void vJobProgress(void);
  void vJobProgressReset(void);
  void vJobProgressSeconds(void);

  void vJobReapableFetch(void);
  void vJobReapableFetch_other(void);
  void vJobReapableStore(void);
  void vJobReapableStore_other(void);
  void vJobRemoveLocal(void);
  void vJobRemoveOriginator(void);
  void vJobRemoveParticipant(void);
  void vJobRemoveRetired(void);

  void vJobReportMetrics(void);
  void vJobReportNetDissect(void);
  void vJobReportNetwork(void);
  void vJobReportOperands(void);
  void vJobReportOperators(void);

  void vJobScheduleNextPhase(void);
  void vJobSessionFetch(void);
  void vJobSessionStateFetch(void);
  void vJobShow(void);
  void vJobSignalWaiter(void);
  void vJobSpawn(void);
  void vJobStateFetch(void);
  void vJobStateStore(void);

  void vJobTagSetFetch(void);
  void vJobTaskFetch(void);
  void vJobTaskFetch_other(void);

  void vJobTscBeginning(void);
  void vJobTscBegun(void);
  void vJobTscExecuting(void);
  void vJobTscExecuted(void);
  void vJobTscEnding(void);
  void vJobTscEnded(void);
  void vJobTscCanceling(void);
  void vJobTscCanceled(void);
  void vJobTscParticipant(void);

  void vJobWasForkedFetch(void);
  void vJobWasForkedStore(void);

  // job metrics
  void vJobMetricsCollect(void);
  void vJobMetricsDelete(void);
  void vJobMetricsJobSet(void);
  void vJobMetricsNew(void);
  void vJobMetricsUpdate(void);

  // job generic
  void vJob_BegunNew(void);
  void vJob_CancelNew(void);
  void vJob_CancelledNew(void);
  void vJob_EndNew(void);
  void vJob_EndedNew(void);
  void vJob_ExecuteNew(void);
  void vJob_ExecutedNew(void);

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

  // jobs
  void vJobsSeeDlist(void);
  void vJobsSeeLlist(void);
  void vJobsSeeOlist(void);
  void vJobsSeePlist(void);
  void vJobsSeeRlist(void);
  void vJobsSeeSqueue(void);

  // list
  void vListAddRowBuffer(void);
  void vListDelete(void);
  void vListGet(void);
  void vListNew(void);
  void vListPrint(void);

  // listed job
  void vListedJobAssertNoRef(void);
  void vListedJobFindOriginator(void);
  void vListedJobFindParticipant(void);
  void vListedJobHasTask(void);
  void vListedJobPointer(void);
  void vListedJobReturn(void);
  void vListedJobReferences(void);
  void vListedJobTaskFetch(void);

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
  void vLocksShow(void);
  void vLocksReset(void);

  // logic
  void vLogicAnd(void);
  void vLogicNot(void);
  void vLogicOr(void);

  // logic unit
  void vLogicUnitCallAgainFetch(void);
  void vLogicUnitCallAgainStore(void);
  void vLogicUnitCalledAgainFetch(void);
  void vLogicUnitCalledAgainStore(void);
  void vLogicUnitCleanup(void);
  void vLogicUnitCompilerToExit(void);
  void vLogicUnitDiaryAction(void);
  void vLogicUnitDiaryDir(void);
  void vLogicUnitDiaryFetch(void);
  void vLogicUnitDiaryJobNote(void);
  void vLogicUnitDiaryName(void);
  void vLogicUnitDiaryNote(void);
  void vLogicUnitDiaryTitle(void);
  void vLogicUnitHandlerFetch(void);
  void vLogicUnitHandlerStore(void);
  void vLogicUnitInit(void);
  void vLogicUnitIsLftCrwBufferFetch(void);
  void vLogicUnitIsLftCrwBufferStore(void);
  void vLogicUnitIntStackFetch(void);
  void vLogicUnitIntStackStore(void);
  void vLogicUnitJobMetricsFetch(void);
  void vLogicUnitJobMetricsStore(void);

  // ErrorMarker[]
  void vMarkerDelete(void);
  void vMarkerGet(void);
  void vMarkerGetAndClear(void);
  void vMarkerIsNotNull(void);
  void vMarkerIsNull(void);
  void vMarkerNewCancelling(void);
  void vMarkerNewCancelledMarker(void);
  void vMarkerNewCsvBuffer(void);
  void vMarkerNewCleaningMarker(void);
  void vMarkerNewCleanedupMarker(void);
  void vMarkerNewDataPulse(void);
  void vMarkerNewDestroyMarker(void);
  void vMarkerNewEachScanEosMarker(void);
  void vMarkerNewEndMarker(void);
  void vMarkerNewEndedMarker(void);
  void vMarkerNewEndingMarker(void);
  void vMarkerNewEndCrwRhtMarker(void);
  void vMarkerNewEndRowRhtMarker(void);
  void vMarkerNewEosMarker(void);
  void vMarkerNewErroringMarker(void);
  void vMarkerNewErroredMarker(void);
  void vMarkerNewExitingMarker(void);
  void vMarkerNewExitedMarker(void);
  void vMarkerNewFinishedMarker(void);
  void vMarkerNewFlushMarker(void);
  void vMarkerNewInvokeMarker(void);
  void vMarkerNewLeaveMarker(void);
  void vMarkerNewLeavingMarker(void);
  void vMarkerNewLeftMarker(void);
  void vMarkerNewNextBufferMarker(void);
  void vMarkerNewStartedMarker(void);
  void vMarkerNewStartMarker(void);
  void vMarkerNewStartCrwRhtMarker(void);
  void vMarkerNewStartRowRhtMarker(void);
  void vMarkerNewStreamDefRht(void);
  void vMarkerNewTrigger(void);
  void vMarkerPipeIn(void);
  void vMarkerPipeToOutput(void);
  void vMarkerSet(void);

// machine
  void vMachineCoreCount(void);
  void vMachineCpuFreq(void);
  void vMachineHostName(void);
  void vMachineMemory(void);

  // magnitude
  void vBytesMagnitude(void);
  void vNumberMagnitude(void);

  // ErrorMarker[]
  void vErrorMarkerDelete(void);
  void vErrorMarkerGet(void);
  void vErrorMarkerGetAndClear(void);
  void vErrorMarkerIsNotNull(void);
  void vErrorMarkerIsNull(void);
  void vErrorMarkerNewErrorMarker(void);
  void vErrorMarkerPipeIn(void);
  void vErrorMarkerPipeOut(void);
  void vErrorMarkerPipeToOutput(void);
  void vErrorMarkerSet(void);

  // memory
  void vMemoryByteDump(void);
  void vMemoryReset(void);
  void vMemoryStatistics(void);
  void vMemoryWordDump(void);

  // metaGet
  void vMetaGetDelete(void);
  void vMetaGetGet(void);
  void vMetaGetGetAndClear(void);
  void vMetaGetGetParts(void);
  void vMetaGetIsNotNull(void);
  void vMetaGetIsNull(void);
  void vMetaGetSet(void);
  void vMetaGetNew(void);
  void vMetaGetTableNameFetch(void);
  void vMetaGetPutParts(void);

  // metaTable
  void vMetaTableListTables(void);

  // Monitor (Nodes)
  void vMonitorUpdateNodeStatus(void);

  // named buffer
  void vNamedBufferDelete(void);
  void vNamedBufferGet(void);
  void vNamedBufferGetAndClear(void);
  void vNamedBufferIsNotNull(void);
  void vNamedBufferIsNull(void);
  void vNamedBufferMorphClass(void);
  void vNamedBufferNameFetch(void);
  void vNamedBufferNameStore(void);
  void vNamedBufferPipeIn(void);
  void vNamedBufferSet(void);

  // network
  void vNetworkReset(void);
  void vNetworkShow(void);

  // node (of grid)
  void vNodeForget(void);
  void vNodeGet(void);
  void vNodeIP(void);
  void vNodeIsThisNode(void);
  void vNodeName(void);
  void vNodeNew(void);
  void vNodeId(void);
  void vNodePartitions(void);
  void vNodePort(void);
  void vNodeRecvSocketStore(void);
  void vNodeRecvSocketFetch(void);
  void vNodeSendSocketStore(void);
  void vNodeSendSocketFetch(void);
  void vNodeSet(void);
  void vNodeSize(void);
  void vNodeStartNode(void);
  void vNodeStateStore(void);
  void vNodeStateFetch(void);
  void vNodeStatus(void);

  // node iterator
  void vNode_IDelete(void);
  void vNode_IMore(void);
  void vNode_INext(void);
  void vNode_INode(void);
  void vNode_INew(void);

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
  void vObjectPostRecvWordFetch(void);
  void vObjectPreSendWordFetch(void);
  void vObjectStore(void);

  // op
  void vOpAckInvokingTask(void);
  void vOpEnqueOperand(void);
  void vOpFindOp(void);
  void vOpGet(void);
  void vOpHandlerEntry(void);
  void vOpHandlerExit(void);
  void vOpHandlerFetch(void);
  void vOpHandlerStore(void);
  void vOpHandlerAllStore(void);
  void vOpIsEventValid(void);
  void vOpEventCountInc(void);
  void vOpNameFetch(void);
  void vOpOpNameFetch(void);
  void vOpPartitionFetch(void);
  void vOpJobFetch(void);
  void vOpPipeBackIn(void);
  void vOpPipeOut(void);
  void vOpPtnIdFetch(void);
  void vOpQueueLength(void);
  void vOpStageNameFetch(void);
  void vOpStageNmFetch(void);
  void vOpTagSetFetch(void);

  // optimiser
  void vOptimiserDelete(void);
  void vOptimiserDissect(void);
  void vOptimiserGet(void);
  void vOptimiserOptimise(void);
  void vOptimiserNew(void);
  void vOptimiserWeave(void);

  // operand
  void vOperandToParcel(void);

  // OperandOoO
  void vOperandOoODelete(void);
  void vOperandOoOGet(void);
  void vOperandOoOGetAndClear(void);
  void vOperandOoOIsNull(void);
  void vOperandOoONext(void);
  void vOperandOoOPipeToOutput(void);
  void vOperandOoOSet(void);

  // opsys
  void vOpSysIsLinux(void);
  void vOpSysIsMacOs(void);

  // paging metrics
  void vPagingMetricsDelete(void);
  void vPagingMetricsUpdate(void);

  // path identifier
  void vPathIdentifierChoosePaths(void);
  void vPathIdentifierChosenPath(void);
  void vPathIdentifierDelete(void);
  void vPathIdentifierFetchTechniqueFetch(void);
  void vPathIdentifierGenPaths(void);
  void vPathIdentifierGet(void);
  void vPathIdentifierHighKeyFetch(void);
  void vPathIdentifierIndexNmFetch(void);
  void vPathIdentifierIsNull(void);
  void vPathIdentifierKeyFetch(void);
  void vPathIdentifierLowKeyFetch(void);
  void vPathIdentifierMatchPaths(void);
  void vPathIdentifierNew(void);
  void vPathIdentifierParseWhere(void);
  void vPathIdentifierPhrasesFetch(void);
  void vPathIdentifierScorePaths(void);
  void vPathIdentifierSet(void);
  void vPathIdentifierShowPaths(void);
  void vPathIdentifierTableFetch(void);
  void vPathIdentifierTableStore(void);

  // PlanOperand
  void vPlanOperandDelete(void);
  void vPlanOperandGet(void);
  void vPlanOperandGetAndClear(void);
  void vPlanOperandIsNotNull(void);
  void vPlanOperandIsNull(void);
  void vPlanOperandSet(void);

  // persistence
  void vPersistenceCheckpoint(void);
  void vPersistenceReload(void);

  // pipeline
  void vPipelineCreate(void);
  void vPipelineDump(void);
  void vPipelineGet(void);
  void vPipelineSet(void);
  void vPipelineRootDeckFetch(void);
  void vPipelineSpawnOpTasks(void);

  // pipelineOfOp
  void vPipelineOfOpAddAccum(void);
  void vPipelineOfOpCancelledFetch(void);
  void vPipelineOfOpCancelledStore(void);
  void vPipelineOfOpDropDebugLock(void);
  void vPipelineOfOpEarlyExitFetch(void);
  void vPipelineOfOpEarlyExitStore(void);
  void vPipelineOfOpErroredStore(void);
  void vPipelineOfOpErroredFetch(void);
  void vPipelineOfOpGet(void);
  void vPipelineOfOpGetAccum(void);
  void vPipelineOfOpGrabDebugLock(void);
  void vPipelineOfOpPulsedFetch(void);
  void vPipelineOfOpPulsedStore(void);

  // pipeSpec

  void vPipeSpecDelete(void);
  void vPipeSpecDump(void);
  void vPipeSpecGet(void);
  void vPipeSpecOptimisable(void);
  void vPipeSpecSet(void);


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

  // pot
  void vPotCompleted(void);
  void vPotCompletedPtn(void);
  void vPotDangling(void);
  void vPotDanglingPtn(void);
  void vPotDelete(void);
  void vPotIsNull(void);
  void vPotNew(void);
  void vPotPushBuffer(void);
  void vPotRowPixStore(void);

  // pot Rlu
  void vPotRluCompleted(void);
  void vPotRluCompletedPtn(void);
  void vPotRluDangling(void);
  void vPotRluDanglingPtn(void);
  void vPotRluDelete(void);
  void vPotRluIsNull(void);
  void vPotRluNew(void);
  void vPotRluPushBuffer(void);
  void vPotRluRowPixStore(void);

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

  // project
  void vProjectGet(void);
  void vProjectNew(void);
  void vProjectColumns(void);
  void vProjectSet(void);

  // prompt
  void vPromptContinue(void);
  void vPromptExecuteFunction(void);
  void vPromptGetCommand(void);
  void vPromptNew(void);
  void vPromptSession(void);

  // ptnMap
  void vPtnMapAdjust(void);
  void vPtnMapPtnsInGrid(void);
  void vPtnMapSee(void);

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

  void vPushBcdVal(void);
  void vPushBolVal(void);
  void vPushFipVal(void);
  void vPushFltVal(void);
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

  // range
  void vRangeGet(void);
  void vRangeNew(void);
  void vRangeBounds(void);
  void vRangeSet(void);

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

  // recvWL
  void vRecvWLGet(void);
  void vRecvWLIsNotNull(void);
  void vRecvWLIsNull(void);
  void vRecvWLSet(void);

  // reference timer
  void vRefTimerReset(void);
  void vRefTimerMilliseconds(void);

  // Consume
  void vRegSendsDelete(void);
  void vRegSendsGet(void);
  void vRegSendsHandleCancelling(void);
  void vRegSendsHandleCleaning(void);
  void vRegSendsHandleDataPulse(void);
  void vRegSendsHandleDestroy(void);
  void vRegSendsHandleEnd(void);
  void vRegSendsHandleEnding(void);
  void vRegSendsHandleEos(void);
  void vRegSendsHandleErroring(void);
  void vRegSendsHandleExiting(void);
  void vRegSendsHandleLeaving(void);
  void vRegSendsHandleStart(void);
  void vRegSendsHandleStreamDef(void);
  void vRegSendsHandleStreamDefRht(void);
  void vRegSendsIsNull(void);
  void vRegSendsNew(void);

  // reg folder
  void vRegFolderFindSymbol(void);


  // reg now
  void vRegNowGet(void);


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
  void vRegPathRoot(void);

  // Parcel
  void vParcelDeleteOld(void);
  void vParcelDemonNmFetch(void);
  void vParcelDemonNmStore(void);
  void vParcelDemonOut(void);
  void vParcelJobIdFetch(void);
  void vParcelLabel(void);
  void vParcelPipeOut(void);
  void vParcelSendOut(void);

  void vParcelDelete(void);
  void vParcelGet(void);
  void vParcelGetAndClear(void);
  void vParcelIsNotNull(void);
  void vParcelIsNull(void);
  void vParcelSet(void);

  // reg Pipein
  void vPipeinBide(void);
  void vPipeinBideNext(void);
  void vPipeinDelete(void);
  void vPipeinGet(void);
  void vPipeinGetAndClear(void);
  void vPipeinIsBuffer(void);
  void vPipeinIsNull(void);
  void vPipeinKind(void);
  void vPipeinNext(void);
  void vPipeinPeek(void);
  void vPipeinPipeToOutput(void);
  void vPipeinSet(void);
  void vPipeinWriteProfile(void);

  // ptn def
  void vPtnDefColumnFetch(void);
  void vPtnDefColumnCountFetch(void);
  void vPtnDefGet(void);
  void vPtnDefGetAndClear(void);
  void vPtnDefIsNull(void);
  void vPtnDefPrint(void);
  void vPtnDefSet(void);

  // received object
  void vRecvdObjectGet(void);
  void vRecvdObjectIsNotNull(void);
  void vRecvdObjectIsNull(void);
  void vRecvdObjectSet(void);

  // respondToOp
  void vResponseOpGet(void);
  void vResponseOpPipeout(void);

  // row buffer
  void vRowBufferCopy(void);
  void vRowBufferDecile(void);
  void vRowBufferDelete(void);
  void vRowBufferGet(void);
  void vRowBufferGetAndClear(void);
  void vRowBufferInsdelIntoTable(void);
  void vRowBufferIsNotNull(void);
  void vRowBufferIsNull(void);
  void vRowBufferIterate(void);
  void vRowBufferPrint(void);
  void vRowBufferPtnId(void);
  void vRowBufferRows(void);
  void vRowBufferSet(void);
  void vRowBufferToCsv(void);
  void vRowBufferToCsv_OLD(void);
  void vRowBufferOperandDump(void);

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

  // rowOnce
  void vRowOnceDelete(void);
  void vRowOnceGet(void);
  void vRowOnceIsNull(void);
  void vRowOnceNew(void);
  void vRowOnceSet(void);

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

  // sen
  void vSenColon(void);
  void vSenComment(void);
  void vSenCreateFromInRoot(void);
  void vSenDoColon(void);
  void vSenExecute(void);
  void vSenExit_RT(void);
  void vSenFind(void);
  void vSenImmediate(void);
  void vSenInnerInterpreter(void);
  void vSenMungedText(void);
  void vSenPostpone(void);
  void vSenProcText(void);
  void vSenSee(void);
  void vSenSemicolon(void);
  void vSenSeriesName(void);

  // sen2 (was flow
  void vSenHazard(void);
  void vSenJump(void);
  void vSenJumpFalse(void);
  void vSenJumpTrue(void);
  void vSenNop(void);
  void vSenReenter(void);
  void vSenTest(void);
  void vSenThrow(void);

  // sen compiler
  void vSenCompilerCompiling(void);
  void vSenCompilerCondBegin(void);
  void vSenCompilerCondEnd(void);
  void vSenCompilerCondElse(void);
  void vSenCompilerCondTest(void);
  void vSenCompilerCurrentFetch(void);
  void vSenCompilerCurrentStore(void);
  void vSenCompilerDiscarding(void);
  void vSenCompilerInterpreting(void);
  void vSenCompilerIsCompiling(void);
  void vSenCompilerIsDiscarding(void);
  void vSenCompilerIsInterpreting(void);
  void vSenCompilerIsPostponing(void);
  void vSenCompilerNestedFetch(void);
  void vSenCompilerNestedStore(void);
  void vSenCompilerPostponing(void);

  void vSendNodeGet(void);
  void vSendNodeSet(void);

  // SendWL
  void vSendProcessEventA(void);
  void vSendProcessEventB(void);
  void vSendWLAction1(void);
  void vSendWLAction2(void);
  void vSendWLDelete(void);
  void vSendWLNew(void);

  // session
  void vSessionCascadeFetch(void);
  void vSessionCascadeStore(void);
  void vSessionCommand(void);
  void vSessionHasCascadeFetch(void);
  void vSessionHasCascadeStore(void);
  void vSessionLastJobId(void);
  void vSessionSystemCloserFetch(void);
  void vSessionSystemCloserStore(void);
  void vSessionTagSetFetch(void);

  // shell
  void vShellExecute(void);

  // socket
  void vSocketAccept(void);
  void vSocketClose(void);
  void vSocketConnect(void);
  void vSocketGet(void);
  void vSocketDropWrite(void);
  void vSocketGrabWrite(void);
  void vSocketIsNotNull(void);
  void vSocketIsNull(void);
  void vSocketListen(void);
  void vSocketObjectFetch(void);
  void vSocketObjectFetchClear(void);
  void vSocketNew(void);
  void vSocketRecvObject(void);
  void vSocketSendObject(void);
  void vSocketSet(void);
  void vSocketSuccessFetch(void);
  void vSocketSuccessStore(void);


  // sql compiler
  void vSqlCompilerAggregateSplit(void);
  void vSqlCompilerCorrBySquash(void);
  void vSqlCompilerDeletePlan(void);
  void vSqlCompilerDoExplain(void);
  void vSqlCompilerDoExplainTliterate(void);
  void vSqlCompilerExplainFormat(void);
  void vSqlCompilerExplainPlan(void);
  void vSqlCompilerExprPushdown(void);
  void vSqlCompilerExprValidate(void);
  void vSqlCompilerHasExplain(void);
  void vSqlCompilerNameValidate(void);
  void vSqlCompilerJoinOptimisation(void);
  void vSqlCompilerParser(void);
  void vSqlCompilerPartitionExplain(void);
  void vSqlCompilerPartitionPlan(void);
  void vSqlCompilerPostProject(void);
  void vSqlCompilerProjValidation(void);
  void vSqlCompilerSetOpValidate(void);
  void vSqlCompilerStageMarkers(void);
  void vSqlCompilerTransliterate(void);

  // ssn
  void vSsnIncBegun(void);
  void vSsnIncCancelled(void);
  void vSsnIncEnded(void);
  void vSsnIncExecuted(void);
  void vSsnSenLoc(void);
  void vSsnTraceSession(void);

  // stage
  void vStageGet(void);
  void vStageSet(void);

  // stage (of op)
  void vStageOfOpDump(void);
  void vStageOfOpGet(void);

  // stats
  void vStatsBuffInInc(void);
  void vStatsDelete(void);
  void vStatsMicrosecs(void);
  void vStatsMillisecs(void);
  void vStatsNanosecs(void);
  void vStatsNew(void);
  void vStatsOperandsInc(void);
  void vStatsSecs(void);
  void vStatsStartTimer(void);
  void vStatsUpdateTimer(void);

  // stencil
  void vStencilDelete(void);
  void vStencilGet(void);
  void vStencilNew(void);
  void vStencilSetup(void);
  void vStencilSketch(void);
  void vStencilSourceFetch(void);

  // steps
  void vStepsReset(void);
  void vStepsStart(void);
  void vStepsWrite(void);

  // stream
  void vStreamGet(void);
  void vStreamSet(void);

  // stream (of op)
  void vStreamOfOpDump(void);
  void vStreamOfOpFindOp(void);
  void vStreamOfOpGet(void);

  // string
  void vStrAlgebra(void);
  void vStrCat(void);
  void vStrClip(void);
  void vStrContains(void);
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
  void vStrReplaceTags(void);
  void vStrRight(void);
  void vStrSame(void);
  void vStrSub(void);
  void vStrTailInt(void);
  void vStrToPromptOut(void);
  void vStrToStrVec(void);

  // register stringA
  void vStringAGet(void);
  void vString0Get(void);
  void vString1Get(void);
  void vString2Get(void);
  void vString3Get(void);
  void vString4Get(void);
  void vString5Get(void);
  void vString6Get(void);
  void vString7Get(void);
  void vStringASet(void);
  void vString0Set(void);
  void vString1Set(void);
  void vString2Set(void);
  void vString3Set(void);
  void vString4Set(void);
  void vString5Set(void);
  void vString6Set(void);
  void vString7Set(void);

  // stream def Left
  void vStreamDefLftAsStr(void);
  void vStreamDefLftColumnFetch(void);
  void vStreamDefLftColumnCountFetch(void);
  void vStreamDefLftDelete(void);
  void vStreamDefLftDuplicate(void);
  void vStreamDefLftGet(void);
  void vStreamDefLftGetAndClear(void);
  void vStreamDefLftIsNull(void);
  void vStreamDefLftName(void);
  void vStreamDefLftNew(void);
  void vStreamDefLftNewToCsv(void);
  void vStreamDefLftParse(void);
  void vStreamDefLftPipeIn(void);
  void vStreamDefLftPrint(void);
  void vStreamDefLftRowSize(void);
  void vStreamDefLftSelectColumns(void);
  void vStreamDefLftSet(void);

  // stream definition (out)
  void vStreamDefOutAddColumn(void);
  void vStreamDefOutDelete(void);
  void vStreamDefOutDuplicate(void);
  void vStreamDefOutGet(void);
  void vStreamDefOutGenerate(void);
  void vStreamDefOutIsNull(void);
  void vStreamDefOutJoin(void);
  void vStreamDefOutNameFetch(void);
  void vStreamDefOutNameStore(void);
  void vStreamDefOutNew(void);
  void vStreamDefOutNewFromTable(void);
  void vStreamDefOutPrint(void);
  void vStreamDefOutSet(void);

  // stream def Right
  void vStreamDefRhtDelete(void);
  void vStreamDefRhtDuplicate(void);
  void vStreamDefRhtGet(void);
  void vStreamDefRhtGetAndClear(void);
  void vStreamDefRhtIsNull(void);
  void vStreamDefRhtNew(void);
  void vStreamDefRhtNewFromTable(void);
  void vStreamDefRhtPrint(void);
  void vStreamDefRhtSelectColumns(void);
  void vStreamDefRhtSet(void);

  // reg stream set
  void vStreamSetAddAnalyzeIndex(void);
  void vStreamSetAddAnalyzeTable(void);
  void vStreamSetAddColumnDef(void);
  void vStreamSetAddGridSanity(void);
  void vStreamSetAddListContents(void);
  void vStreamSetAddListFolder(void);
  void vStreamSetAddListIndex(void);
  void vStreamSetAddListPartitions(void);
  void vStreamSetAddNodeHeartbeat(void);
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
  void vStreamSetIsNull(void);
  void vStreamSetName(void);
  void vStreamSetNew(void);
  void vStreamSetNextRow(void);
  void vStreamSetReadChunk(void);
  void vStreamSetSet(void);

  // reg infoSet table
  void vStreamSetTableColumnFetch(void);
  void vStreamSetTableDump(void);
  void vStreamSetTableGet(void);
  void vStreamSetTableSet(void);

  // sub call
  void vSubCallGet(void);
  void vSubCallSet(void);

  // sub return
  void vSubReturnGet(void);
  void vSubReturnSet(void);

  // subset
  void vSubsetGet(void);
  void vSubsetNext(void);
  void vSubsetNextBuffer(void);
  void vSubsetSet(void);

  // Supervisor
  void vSupervisorCollectInfo(void);
  void vSupervisorHaltFetch(void);
  void vSupervisorHaltStore(void);

  // system schedule
  void vScheduleCheckSleeping(void);
  void vScheduleExecutingLength(void);
  void vScheduleReadyLength(void);
  void vScheduleSleepingLength(void);
  void vScheduleWaitingLength(void);

  // sys args
  void vSysArgsDefinedNodes(void);
  void vSysArgsDerefMetricsFetch(void);
  void vSysArgsDerefMetricsStore(void);
  void vSysArgsJobMetricsFetch(void);
  void vSysArgsJobMetricsStore(void);
  void vSysArgsMetricsSleepFetch(void);
  void vSysArgsMetricsSleepStore(void);
  void vSysArgsPrint(void);
  void vSysArgsSysMetricsFetch(void);
  void vSysArgsSysMetricsStore(void);
  void vSysArgsTagSet(void);

  // sys metrics
  void vSysMetricsNew(void);

  // sys report
  void vSysReportSysLocks(void);
  void vSysReportSysMetrics(void);

  // system
  void vSystemActiveJobs(void);
  void vSystemBpa(void);
  void vSystemOpenFetch(void);
  void vSystemOpenStore(void);
  void vSystemSee(void);
  void vSystemStartedStore(void);
  void vSystemStartedFetch(void);

  // table
  void vTableAddIndex(void);
  void vTableAppendBuffer(void);
  void vTableColumnCountFetch(void);
  void vTableColumnFetch(void);
  void vTableCreatePtnDef(void);
  void vTableDelete(void);
  void vTableDeleteIndex(void);
  void vTableDump(void);
  void vTableDumpBkt(void);
  void vTableDumpBktMap(void);
  void vTableDumpMeta(void);
  void vTableFindColumn(void);
  void vTableGet(void);
  void vTableGetName(void);
  void vTableIndexCount(void);
  void vTableIsColumn(void);
  void vTableIsInternallyDefined(void);
  void vTableIsReplicated(void);
  void vTableIsUserCreated(void);
  void vTableListPartition(void);
  void vTableNameFetch(void);
  void vTableNew(void);
  void vTablePtnDefFetch(void);
  void vTableSet(void);
  void vTableSize(void);
  void vTableStreamDefFetch(void);
  void vTableStreamDefStore(void);
  void vTableTablePtnFetch(void);
  void vTableTablePtnGet(void);
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
  void vTablePtnPopulateIndex(void);
  void vTablePtnRoot(void);
  void vTablePtnSet(void);
  void vTablePtnTruncate(void);
  void vTablePtnTruncateIndex(void);

  // tag
  void vTagGet(void);

  // tag-set
  void vTagSetAppendInt(void);
  void vTagSetAppendStr(void);
  void vTagSetAsStr(void);
  void vTagSetCopy(void);
  void vTagSetGet(void);
  void vTagSetGetInt(void);
  void vTagSetGetStr(void);
  void vTagSetHasName(void);
  void vTagSetInsertInt(void);
  void vTagSetInsertStr(void);
  void vTagSetReplaceInt(void);
  void vTagSetReplaceStr(void);
  void vTagSetSet(void);
  void vTagSetUpsertStr(void);
  void vTagSetNew(void);

  // task
  void vTaskAck(void);
  void vTaskAcksStore(void);
  void vTaskBreakPoint(void);
  void vTaskCycleLock(void);
  void vTaskDestructorTraceFetch(void);
  void vTaskDestructorTraceStore(void);
  void vTaskDropLock(void);
  void vTaskDump(void);
  void vTaskException(void);
  void vTaskGet(void);
  void vTaskGrabLock(void);
  void vTaskIsAckSet(void);
  void vTaskIsLocked(void);
  void vTaskIsWaiting(void);
  void vTaskNameFetch(void);
  void vTaskOperandYieldFetch(void);
  void vTaskOperandYieldStore(void);
  void vTaskProfileFetch(void);
  void vTaskProfileStore(void);
  void vTaskReleaseLockSets(void);
  void vTaskSeeFrameStack(void);
  void vTaskSeeLockSets(void);
  void vTaskSeeLogicUnit(void);
  void vTaskSleepMsec(void);
  void vTaskSleepNsec(void);
  void vTaskSleepSsec(void);
  void vTaskSleepUsec(void);
  void vTaskStateFetch(void);
  void vTaskThread(void);
  void vTaskTransitionStore(void);
  void vTaskWait(void);
  void vTaskYield(void);
  void vTaskYield_opLoop(void);

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

  // time
  void vTimeCurrent(void);

  // timer
  void vTimerGet(void);
  void vTimerDelete(void);
  void vTimerInit(void);
  void vTimerMicrosecs(void);
  void vTimerMillisecs(void);
  void vTimerNanosecs(void);
  void vTimerNew(void);
  void vTimerReportStr(void);
  void vTimerReset(void);
  void vTimerSecs(void);
  void vTimerSet(void);
  void vTimerStart(void);
  void vTimerTicks(void);
  void vTimerUpdate(void);

  // tinyC
  void vTinyCCompile(void);
  void vTinyCDelete(void);
  void vTinyCFind(void);
  void vTinyCNew(void);
  void vTinyCRelocate(void);
  void vTinyCSymbols(void);

  // to
  void vToBcd(void);
  void vToBol(void);
  void vToDigitName(void);
  void vToFlt(void);
  void vToInt(void);
  void vToPtr(void);
  void vToStr(void);
  void vToTask(void);

  // tokenVec
  void vTokenVecDelete(void);
  void vTokenVecDump(void);
  void vTokenVecNew(void);
  void vTokenVecRemoveToken(void);
  void vTokenVecTokenise(void);
  void vTokenVecToString(void);

  // toKeyHshRow
  void vToKeyHshRowCompleted(void);
  void vToKeyHshRowCountersFetch(void);
  void vToKeyHshRowCsvToRow(void);
  void vToKeyHshRowDangling(void);
  void vToKeyHshRowDelete(void);
  void vToKeyHshRowFetch(void);
  void vToKeyHshRowNew(void);

  // toLglPtnCrw
  void vToLglPtnCrwCompleted(void);
  void vToLglPtnCrwConvert(void);
  void vToLglPtnCrwCreate(void);
  void vToLglPtnCrwDangling(void);
  void vToLglPtnCrwDelete(void);

  // toLglPtnNst
  void vToLglPtnNstConvert(void);
  void vToLglPtnNstCreate(void);
  void vToLglPtnNstDelete(void);

  // toLglPtnRow
  void vToLglPtnRowCompleted(void);
  void vToLglPtnRowConvert(void);
  void vToLglPtnRowCreate(void);
  void vToLglPtnRowDangling(void);
  void vToLglPtnRowDelete(void);

  // trace
  void vTraceOff(void);
  void vTraceOn(void);
  void vTracePoint(void);
  void vTraceSet(void);

  // tub
  void vTubAppend(void);
  void vTubCompleted(void);
  void vTubDangling(void);
  void vTubDelete(void);
  void vTubGet(void);
  void vTubIsEmpty(void);
  void vTubNew(void);

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

  // vertex
  void vVertexDump(void);
  void vVertexGet(void);
  void vVertexSet(void);

  // vertex (of op)
  void vVertexOfOpDump(void);
  void vVertexOfOpGet(void);

  // zero
  void vWithSubPipeline(void);

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
  void vWordIsImmediate(void);
  void vWordIsIntrinsic(void);
  void vWordKindFetch(void);
  void vWordKindStore(void);
  void vWordLocate(void);
  void vWordNameFetch(void);
  void vWordNameStore(void);
  void vWordNew(void);
  void vWordNext(void);
  void vWordObjectFetch(void);
  void vWordObjectStore(void);
  void vWordPrev(void);
  void vWordResetRoutine(void);
  void vWordSee(void);
  void vWordXcheckInxPtr(void);

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
