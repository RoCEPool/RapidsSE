/*============================================================================*/
/* StepsSet.hpp - the set of steps                                            */
/*============================================================================*/
#pragma once

public:

  // access path
  static void vAccessPathTableFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vAccessPathTableStore(Tasks::MP_Task mpTask, Forth* pForth);

  // accum
  static void vAccumDelete(Tasks::MP_Task mpTask, Forth* pForth);
  static void vAccumNew(Tasks::MP_Task mpTask, Forth* pForth);

  static void vAccumAccumBuffer(Tasks::MP_Task mpTask, Forth* pForth);
  static void vAccumCountFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vAccumCountStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vAccumGroupFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vAccumGroupStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vAccumMaxFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vAccumMaxStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vAccumMinFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vAccumMinStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vAccumSumFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vAccumSumStore(Tasks::MP_Task mpTask, Forth* pForth);

  // address
  static void vAddressEqual(Tasks::MP_Task mpTask, Forth* pForth);
  static void vAddressFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vAddressStore(Tasks::MP_Task mpTask, Forth* pForth);

  // apiState
  static void vApiStateIncBegun(Tasks::MP_Task mpTask, Forth* pForth);
  static void vApiStateIncCancelled(Tasks::MP_Task mpTask, Forth* pForth);
  static void vApiStateIncEnded(Tasks::MP_Task mpTask, Forth* pForth);
  static void vApiStateIncExecuted(Tasks::MP_Task mpTask, Forth* pForth);
  static void vApiStateSenLoc(Tasks::MP_Task mpTask, Forth* pForth);
  static void vApiStateTraceSession(Tasks::MP_Task mpTask, Forth* pForth);

  // args
  static void vArgsOscillate(Tasks::MP_Task mpTask, Forth* pForth);
  static void vArgsPrint(Tasks::MP_Task mpTask, Forth* pForth);

  // article
  static void vArticleDump(Tasks::MP_Task mpTask, Forth* pForth);

  // assert
  static void vAssert(Tasks::MP_Task mpTask, Forth* pForth);

  static void vAssertClass(Tasks::MP_Task mpTask, Forth* pForth);
  static void vAssertClass_0(Tasks::MP_Task mpTask, Forth* pForth);
  static void vAssertClass_1(Tasks::MP_Task mpTask, Forth* pForth);
  static void vAssertClass_2(Tasks::MP_Task mpTask, Forth* pForth);
  static void vAssertClass_3(Tasks::MP_Task mpTask, Forth* pForth);
  static void vAssertClass_4(Tasks::MP_Task mpTask, Forth* pForth);

  static void vAssertStack_0(Tasks::MP_Task mpTask, Forth* pForth);
  static void vAssertStack_0more(Tasks::MP_Task mpTask, Forth* pForth);
  static void vAssertStack_1(Tasks::MP_Task mpTask, Forth* pForth);
  static void vAssertStack_1more(Tasks::MP_Task mpTask, Forth* pForth);
  static void vAssertStack_2(Tasks::MP_Task mpTask, Forth* pForth);
  static void vAssertStack_2more(Tasks::MP_Task mpTask, Forth* pForth);
  static void vAssertStack_3(Tasks::MP_Task mpTask, Forth* pForth);
  static void vAssertStack_3more(Tasks::MP_Task mpTask, Forth* pForth);
  static void vAssertStack_4(Tasks::MP_Task mpTask, Forth* pForth);
  static void vAssertStack_4more(Tasks::MP_Task mpTask, Forth* pForth);

  // astack
  static void vAstackClass(Tasks::MP_Task mpTask, Forth* pForth);
  static void vAstackDepth(Tasks::MP_Task mpTask, Forth* pForth);
  static void vAstackDrop(Tasks::MP_Task mpTask, Forth* pForth);
  static void vAstackDup(Tasks::MP_Task mpTask, Forth* pForth);
  static void vAstackFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vAstackNip(Tasks::MP_Task mpTask, Forth* pForth);
  static void vAstackPick(Tasks::MP_Task mpTask, Forth* pForth);
  static void vAstackReverse(Tasks::MP_Task mpTask, Forth* pForth);
  static void vAstackStk(Tasks::MP_Task mpTask, Forth* pForth);
  static void vAstackSwap(Tasks::MP_Task mpTask, Forth* pForth);
  static void vAstackToA(Tasks::MP_Task mpTask, Forth* pForth);
  static void vAstackToP(Tasks::MP_Task mpTask, Forth* pForth);
  static void vAstackTransfer(Tasks::MP_Task mpTask, Forth* pForth);
  static void vAstack0(Tasks::MP_Task mpTask, Forth* pForth);
  static void vAstack1(Tasks::MP_Task mpTask, Forth* pForth);
  static void vAstack2(Tasks::MP_Task mpTask, Forth* pForth);
  static void vAstack3(Tasks::MP_Task mpTask, Forth* pForth);
  static void vAstack4(Tasks::MP_Task mpTask, Forth* pForth);
  static void vAstack5(Tasks::MP_Task mpTask, Forth* pForth);
  static void vAstack6(Tasks::MP_Task mpTask, Forth* pForth);
  static void vAstack7(Tasks::MP_Task mpTask, Forth* pForth);
  static void vAstack8(Tasks::MP_Task mpTask, Forth* pForth);
  static void vAstack9(Tasks::MP_Task mpTask, Forth* pForth);

  // attachmap
  static void vAttachMapTenantsFetch(Tasks::MP_Task mpTask, Forth* pForth);

  // bp
  static void vBpAnalyze(Tasks::MP_Task mpTask, Forth* pForth);
  static void vBpDump(Tasks::MP_Task mpTask, Forth* pForth);
  static void vBpEpochDump(Tasks::MP_Task mpTask, Forth* pForth);
  static void vBpEpochTake(Tasks::MP_Task mpTask, Forth* pForth);
  static void vBpReadBufferPool(Tasks::MP_Task mpTask, Forth* pForth);
  static void vBpWriteBufferPool(Tasks::MP_Task mpTask, Forth* pForth);
  static void vBpFreeBuffers(Tasks::MP_Task mpTask, Forth* pForth);
  static void vBpFreePercent(Tasks::MP_Task mpTask, Forth* pForth);
  static void vBpFreeSpace(Tasks::MP_Task mpTask, Forth* pForth);
  static void vBpInUseSpace(Tasks::MP_Task mpTask, Forth* pForth);
  static void vBpTotalSpace(Tasks::MP_Task mpTask, Forth* pForth);

  // cast
  static void vCastInt(Tasks::MP_Task mpTask, Forth* pForth);
  static void vCastPtr(Tasks::MP_Task mpTask, Forth* pForth);

  // class
  static void vClassName(Tasks::MP_Task mpTask, Forth* pForth);

  // column
  static void vColumnIsNullableFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vColumnIsPartitionKeyFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vColumnLengthFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vColumnNameFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vColumnScaleFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vColumnTypeFetch(Tasks::MP_Task mpTask, Forth* pForth);

  // command processor
  static void vCommandBlankLine(Tasks::MP_Task mpTask, Forth* pForth);
  static void vCommandDeleteJob(Tasks::MP_Task mpTask, Forth* pForth);
  static void vCommandExecuteFile(Tasks::MP_Task mpTask, Forth* pForth);
  static void vCommandFindJobClass(Tasks::MP_Task mpTask, Forth* pForth);
  static void vCommandGetCommand(Tasks::MP_Task mpTask, Forth* pForth);
  static void vCommandJobClass(Tasks::MP_Task mpTask, Forth* pForth);
  static void vCommandPostJob(Tasks::MP_Task mpTask, Forth* pForth);
  static void vCommandPreJob(Tasks::MP_Task mpTask, Forth* pForth);
  static void vCommandSpawnJob(Tasks::MP_Task mpTask, Forth* pForth);

  // comment
  static void vComment(Tasks::MP_Task mpTask, Forth* pForth);

  // comment
  static void vCompilerCurrent(Tasks::MP_Task mpTask, Forth* pForth);

  // cpu
  static void vCpuRdtscp(Tasks::MP_Task mpTask, Forth* pForth);

  // cursor
  static void vCursorFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vCursorStore(Tasks::MP_Task mpTask, Forth* pForth);

  // ddllock
  static void vDdlLockDropRead(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDdlLockDropWrite(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDdlLockGrabRead(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDdlLockGrabWrite(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDdlLockTestDrop(Tasks::MP_Task mpTask, Forth* pForth);

  // debug
  static void vDebugTrap(Tasks::MP_Task mpTask, Forth* pForth);

  // dic
  static void vDicInit(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDicKindGet(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDicKindSet(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDicLastWord(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDicName(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDicRef(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDicRoot(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDicWordCount(Tasks::MP_Task mpTask, Forth* pForth);

  // dicLock
  static void vDicLockDropWrite(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDicLockGrabWrite(Tasks::MP_Task mpTask, Forth* pForth);

  // dicPath
  static void vDicPathDepth(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDicPathDump(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDicPathPick(Tasks::MP_Task mpTask, Forth* pForth);

  // dicStack
  static void vDicStackDepth(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDicStackDrop(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDicStackDump(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDicStackPick(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDicStackPop(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDicStackPush(Tasks::MP_Task mpTask, Forth* pForth);

  // dir
  static void vDirChangeCurrent(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDirGetCurrent(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDirExists(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDirMake(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDirRemove(Tasks::MP_Task mpTask, Forth* pForth);

  static void vDirClose(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDirDelete(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDirEntryName(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDirEntryType(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDirIsClosed(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDirIsOpen(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDirNew(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDirOpen(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDirPathGet(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDirRead(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDirRewind(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDirSeek(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDirTell(Tasks::MP_Task mpTask, Forth* pForth);

  // docol
  static void vDocolCompiling(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDocolInterpreting(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDocolIsCompiling(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDocolIsInterpreting(Tasks::MP_Task mpTask, Forth* pForth);

  // dot
  static void vDot(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDotPut(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDotNewline(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDotVerbose(Tasks::MP_Task mpTask, Forth* pForth);

  // dstack
  static void vDstackClass(Tasks::MP_Task mpTask, Forth* pForth);                 //! DUP OF class@
  static void vDstackDepth(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDstackDrop(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDstackDropTwo(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDstackDup(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDstackDupNonZero(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDstackNip(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDstackOver(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDstackPick(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDstackReverse(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDstackRoll(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDstackRollInv(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDstackRot(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDstackRotInv(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDstackStk(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDstackSwap(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDstackTransfer(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDstackTuck(Tasks::MP_Task mpTask, Forth* pForth);

  // encode
  static void vEncodeAgain(Tasks::MP_Task mpTask, Forth* pForth);
  static void vEncodeAhead(Tasks::MP_Task mpTask, Forth* pForth);
  static void vEncodeContinue(Tasks::MP_Task mpTask, Forth* pForth);
  static void vEncodeHere(Tasks::MP_Task mpTask, Forth* pForth);
  static void vEncodeIf(Tasks::MP_Task mpTask, Forth* pForth);
  static void vEncodeIfAgain(Tasks::MP_Task mpTask, Forth* pForth);
  static void vEncodeLoop(Tasks::MP_Task mpTask, Forth* pForth);
  static void vEncodeLoopPlus(Tasks::MP_Task mpTask, Forth* pForth);
  static void vEncodeNext(Tasks::MP_Task mpTask, Forth* pForth);
  static void vEncodeThen(Tasks::MP_Task mpTask, Forth* pForth);
  static void vEncodeUntil(Tasks::MP_Task mpTask, Forth* pForth);

  // ename-set
  static void vEnmSetAddInt(Tasks::MP_Task mpTask, Forth* pForth);
  static void vEnmSetAddStr(Tasks::MP_Task mpTask, Forth* pForth);
  static void vEnmSetAsStr(Tasks::MP_Task mpTask, Forth* pForth);
  static void vEnmSetGetStr(Tasks::MP_Task mpTask, Forth* pForth);
  static void vEnmSetHasName(Tasks::MP_Task mpTask, Forth* pForth);
  static void vEnmSetNew(Tasks::MP_Task mpTask, Forth* pForth);

  // enter
  static void vEnterObject(Tasks::MP_Task mpTask, Forth* pForth);
  static void vEnterRoutine(Tasks::MP_Task mpTask, Forth* pForth);

  // entry
  static void vEntryMatches(Tasks::MP_Task mpTask, Forth* pForth);
  static void vEntrySame(Tasks::MP_Task mpTask, Forth* pForth);
  static void vEntryToDataRow(Tasks::MP_Task mpTask, Forth* pForth);

  // entry buffer
  static void vEntBufferDump(Tasks::MP_Task mpTask, Forth* pForth);
  static void vEntBufferIndexFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vEntBufferIndexStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vEntBufferIsLastFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vEntBufferIsLastStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vEntBufferNextBuffer(Tasks::MP_Task mpTask, Forth* pForth);
  static void vEntBufferPartitionFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vEntBufferPartitionStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vEntBufferToEntBuffer(Tasks::MP_Task mpTask, Forth* pForth);

  // env
  static void vEnvGet(Tasks::MP_Task mpTask, Forth* pForth);

  // errors
  static void vErrorMsg(Tasks::MP_Task mpTask, Forth* pForth);

  // file
  static void vFileClose(Tasks::MP_Task mpTask, Forth* pForth);
  static void vFileDelete(Tasks::MP_Task mpTask, Forth* pForth);
  static void vFileNew(Tasks::MP_Task mpTask, Forth* pForth);
  static void vFileOpen(Tasks::MP_Task mpTask, Forth* pForth);
  static void vFilePathGet(Tasks::MP_Task mpTask, Forth* pForth);
  static void vFileRead(Tasks::MP_Task mpTask, Forth* pForth);
  static void vFileReadRecord(Tasks::MP_Task mpTask, Forth* pForth);
  static void vFileWrite(Tasks::MP_Task mpTask, Forth* pForth);

  // flow
  static void vFlowBegin_(Tasks::MP_Task mpTask, Forth* pForth);
  static void vFlowExecute(Tasks::MP_Task mpTask, Forth* pForth);
  static void vFlowHazard(Tasks::MP_Task mpTask, Forth* pForth);
  static void vFlowJump(Tasks::MP_Task mpTask, Forth* pForth);
  static void vFlowJumpBegin_(Tasks::MP_Task mpTask, Forth* pForth);
  static void vFlowJumpFalse(Tasks::MP_Task mpTask, Forth* pForth);
  static void vFlowJumpTrue(Tasks::MP_Task mpTask, Forth* pForth);
  static void vFlowNop(Tasks::MP_Task mpTask, Forth* pForth);
  static void vFlowRetryPoint(Tasks::MP_Task mpTask, Forth* pForth);
  static void vFlowTest(Tasks::MP_Task mpTask, Forth* pForth);
  static void vFlowThrow(Tasks::MP_Task mpTask, Forth* pForth);

  // forth
  static void vAllot(Tasks::MP_Task mpTask, Forth* pForth);
  static void vCell(Tasks::MP_Task mpTask, Forth* pForth);
  static void vColon(Tasks::MP_Task mpTask, Forth* pForth);
  static void vComma(Tasks::MP_Task mpTask, Forth* pForth);
  static void vCompileComma(Tasks::MP_Task mpTask, Forth* pForth);
  static void vCreate(Tasks::MP_Task mpTask, Forth* pForth);
  static void vCreateFrom(Tasks::MP_Task mpTask, Forth* pForth);
  static void vCreateFromInRoot(Tasks::MP_Task mpTask, Forth* pForth);
  static void vCreateFromPersistent(Tasks::MP_Task mpTask, Forth* pForth);
  static void vCreatePersistent(Tasks::MP_Task mpTask, Forth* pForth);
  static void vCreateTenants(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDoes(Tasks::MP_Task mpTask, Forth* pForth);
  static void vDoes_RT(Tasks::MP_Task mpTask, Forth* pForth);
  static void vExit_RT(Tasks::MP_Task mpTask, Forth* pForth);
  static void vFind(Tasks::MP_Task mpTask, Forth* pForth);
  static void vFindInCurrent(Tasks::MP_Task mpTask, Forth* pForth);
  static void vForget(Tasks::MP_Task mpTask, Forth* pForth);
  static void vImmediate(Tasks::MP_Task mpTask, Forth* pForth);
  static void vLocate(Tasks::MP_Task mpTask, Forth* pForth);
  static void vPostpone(Tasks::MP_Task mpTask, Forth* pForth);
  static void vPrompt(Tasks::MP_Task mpTask, Forth* pForth);
  static void vSee(Tasks::MP_Task mpTask, Forth* pForth);
  static void vSemicolon(Tasks::MP_Task mpTask, Forth* pForth);
  static void vSeriesName(Tasks::MP_Task mpTask, Forth* pForth);
  static void vSubDic(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTick(Tasks::MP_Task mpTask, Forth* pForth);

  // grid
  static void vGridConnectedNodes(Tasks::MP_Task mpTask, Forth* pForth);
  static void vGridDefinedNodes(Tasks::MP_Task mpTask, Forth* pForth);
  static void vGridFindNode(Tasks::MP_Task mpTask, Forth* pForth);
  static void vGridIncConnectedNodes(Tasks::MP_Task mpTask, Forth* pForth);
  static void vGridNode(Tasks::MP_Task mpTask, Forth* pForth);
  static void vGridPartitions(Tasks::MP_Task mpTask, Forth* pForth);
  static void vGridShow(Tasks::MP_Task mpTask, Forth* pForth);
  static void vGridSlots(Tasks::MP_Task mpTask, Forth* pForth);
  static void vGridStartedStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vGridStartedFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vGridStartNodeStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vGridStartNodeFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vGridThisNodeFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vGridThisNodeNameFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vGridToNode(Tasks::MP_Task mpTask, Forth* pForth);
  static void vGridToPartition(Tasks::MP_Task mpTask, Forth* pForth);
  static void vGridToSlot(Tasks::MP_Task mpTask, Forth* pForth);

  // group by
  static void vGroupByApplyBuffer(Tasks::MP_Task mpTask, Forth* pForth);
  static void vGroupByApplyEntry(Tasks::MP_Task mpTask, Forth* pForth);
  static void vGroupByApplyPartial(Tasks::MP_Task mpTask, Forth* pForth);
  static void vGroupByDelete(Tasks::MP_Task mpTask, Forth* pForth);
  static void vGroupByEnd(Tasks::MP_Task mpTask, Forth* pForth);
  static void vGroupByNew(Tasks::MP_Task mpTask, Forth* pForth);
  static void vGroupByStart(Tasks::MP_Task mpTask, Forth* pForth);

  // handler
  static void vHandlerServiceSleepQueue(Tasks::MP_Task mpTask, Forth* pForth);

  // index
  static void vIndexAnalyze(Tasks::MP_Task mpTask, Forth* pForth);
  static void vIndexColumnFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vIndexColumnCountFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vIndexColumnName(Tasks::MP_Task mpTask, Forth* pForth);
  static void vIndexColumnType(Tasks::MP_Task mpTask, Forth* pForth);
  static void vIndexCompact(Tasks::MP_Task mpTask, Forth* pForth);
  static void vIndexCreateColumnVec(Tasks::MP_Task mpTask, Forth* pForth);
  static void vIndexDump(Tasks::MP_Task mpTask, Forth* pForth);
  static void vIndexInit(Tasks::MP_Task mpTask, Forth* pForth);
  static void vIndexIsPrimaryKeyFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vIndexIsPrimaryKeyStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vIndexIsUniqueFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vIndexIsUniqueStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vIndexIsValidUniqueIndex(Tasks::MP_Task mpTask, Forth* pForth);
  static void vIndexLowerBound(Tasks::MP_Task mpTask, Forth* pForth);
  static void vIndexNameFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vIndexPopulate(Tasks::MP_Task mpTask, Forth* pForth);
  static void vIndexRange(Tasks::MP_Task mpTask, Forth* pForth);
  static void vIndexShowColumnVec(Tasks::MP_Task mpTask, Forth* pForth);
  static void vIndexSpace(Tasks::MP_Task mpTask, Forth* pForth);
  static void vIndexSubDic(Tasks::MP_Task mpTask, Forth* pForth);
  static void vIndexTableFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vIndexTableStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vIndexTruncate(Tasks::MP_Task mpTask, Forth* pForth);
  static void vIndexUpperBound(Tasks::MP_Task mpTask, Forth* pForth);
  static void vIndexVerify(Tasks::MP_Task mpTask, Forth* pForth);
  static void vIndexVisualize(Tasks::MP_Task mpTask, Forth* pForth);

  // input
  static void vInputClose(Tasks::MP_Task mpTask, Forth* pForth);
  static void vInputDepth(Tasks::MP_Task mpTask, Forth* pForth);
  static void vInputNext(Tasks::MP_Task mpTask, Forth* pForth);
  static void vInputOpen(Tasks::MP_Task mpTask, Forth* pForth);
  static void vInputPop(Tasks::MP_Task mpTask, Forth* pForth);
  static void vInputPush(Tasks::MP_Task mpTask, Forth* pForth);
  static void vInputStdin(Tasks::MP_Task mpTask, Forth* pForth);
  static void vInputText(Tasks::MP_Task mpTask, Forth* pForth);
  static void vInputToken(Tasks::MP_Task mpTask, Forth* pForth);
  static void vInputType(Tasks::MP_Task mpTask, Forth* pForth);
  static void vInputUnknown(Tasks::MP_Task mpTask, Forth* pForth);

  // int
  static void vIntAdd(Tasks::MP_Task mpTask, Forth* pForth);
  static void vIntDivide(Tasks::MP_Task mpTask, Forth* pForth);
  static void vIntEqual(Tasks::MP_Task mpTask, Forth* pForth);
  static void vIntGreater(Tasks::MP_Task mpTask, Forth* pForth);
  static void vIntGreaterEqual(Tasks::MP_Task mpTask, Forth* pForth);
  static void vIntHash(Tasks::MP_Task mpTask, Forth* pForth);
  static void vIntHo16(Tasks::MP_Task mpTask, Forth* pForth);
  static void vIntLess(Tasks::MP_Task mpTask, Forth* pForth);
  static void vIntLessEqual(Tasks::MP_Task mpTask, Forth* pForth);
  static void vIntModulo(Tasks::MP_Task mpTask, Forth* pForth);
  static void vIntMultiply(Tasks::MP_Task mpTask, Forth* pForth);
  static void vIntNeg(Tasks::MP_Task mpTask, Forth* pForth);
  static void vIntNotEqual(Tasks::MP_Task mpTask, Forth* pForth);
  static void vIntRandom(Tasks::MP_Task mpTask, Forth* pForth);
  static void vIntSubtract(Tasks::MP_Task mpTask, Forth* pForth);
  static void vUnsignedIntRandom(Tasks::MP_Task mpTask, Forth* pForth);

  // jobClass
  static void vJobClassBuffer(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobClassFind(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobClassName(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobClassNew(Tasks::MP_Task mpTask, Forth* pForth);

  static void vJobClassSeedRun(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobClassSeedReq(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobClassSeedRcv(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobClassSeedExe(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobClassSeedEnd(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobClassSeedCan(Tasks::MP_Task mpTask, Forth* pForth);

  // job
  static void vJobAcksRemaining(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobAddOriginator(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobAddParticipant(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobAssertBegin(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobAssertNeutral(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobAssertOriginator(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobAssertParticipant(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobAssertReceived(Tasks::MP_Task mpTask, Forth* pForth);

  // job mark 2
  static void vJobMark2IndexStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobMark2TableNameFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobMark2TableStore(Tasks::MP_Task mpTask, Forth* pForth);

  static void vJobCatalogNameFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobClass(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobColumnCountFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobColumnNameFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobColumnIsPartKey(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobColumnTypeFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobCompleted(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobCreateResponseVec(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobOperandFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobOperandStore(Tasks::MP_Task mpTask, Forth* pForth);

  static void vJobDecAcksRemaining(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobDelete(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobDicPath(Tasks::MP_Task mpTask, Forth* pForth);

  static void vJobEnameFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobEnameStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobErrorStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobErrorFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobExecuteFetch(Tasks::MP_Task mpTask, Forth* pForth);

  static void vJobFindOriginator(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobFindParticipant(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobFolderNameFetch(Tasks::MP_Task mpTask, Forth* pForth);

  static void vJobHasError(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobHasOperand(Tasks::MP_Task mpTask, Forth* pForth);

  static void vJobIdFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobIndexNameFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobIsExecuting(Tasks::MP_Task mpTask, Forth* pForth);

  static void vJobJobClass(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobJobListFetch(Tasks::MP_Task mpTask, Forth* pForth);

  static void vJobMetadataStore(Tasks::MP_Task mpTask, Forth* pForth);

  static void vJobNodeNameFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobNoWaitFetch(Tasks::MP_Task mpTask, Forth* pForth);

  static void vJobObjectNameFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobOriginatingTaskFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobOriginatingTaskStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobOriginatorFetch(Tasks::MP_Task mpTask, Forth* pForth);

  static void vJobParentIdFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobParticipantFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobParticipantStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobParticipantResponseStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobParticipantHasError(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobPartitionsFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobPhaseFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobPhaseStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobPostRecv(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobPreSend(Tasks::MP_Task mpTask, Forth* pForth);

  static void vJobRecv(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobRegionSizeFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobRemoveOriginator(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobRemoveParticipant(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobReseed(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobResponseStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobResponseFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobRoutineNameFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobRowsInvalidFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobRowsInvalidStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobRowsProcessedFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobRowsProcessedStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobRowsSkippedFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobRowsSkippedStore(Tasks::MP_Task mpTask, Forth* pForth);

  static void vJobSchemaNameFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobSeedFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobSend(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobSessionStateFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobShow(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobSignalWaiter(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobSlotFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobSpawn(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobSpawnDedicated(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobStateDiagram(Tasks::MP_Task mpTask, Forth* pForth);

  static void vJobTaskFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobTaskStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobTenantNameFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJobThis(Tasks::MP_Task mpTask, Forth* pForth);

  static void vJobWaitParticipant(Tasks::MP_Task mpTask, Forth* pForth);

  // job generic
  static void vJob__BegunNew(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob__CancelNew(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob__CancelledNew(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob__EndNew(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob__EndedNew(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob__ExecuteNew(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob__ExecutedNew(Tasks::MP_Task mpTask, Forth* pForth);

  // job analyzeIndex
  static void vJob_AnalyzeIndexIsDetailed(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_AnalyzeTableIsDetailed(Tasks::MP_Task mpTask, Forth* pForth);

  // job Cat
  static void vJob_CatBegin(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_CatChunk(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_CatEnd(Tasks::MP_Task mpTask, Forth* pForth);

  // job CreateIndex
  static void vJob_CreateIndexIsUniqueFetch(Tasks::MP_Task mpTask, Forth* pForth);

  // job Dynamic
  static void vJob_DynamicBegin(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_DynamicEnd(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_DynamicExecute(Tasks::MP_Task mpTask, Forth* pForth);

  // job Export
  static void vJob_ExportAuxAckDecidedFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_ExportAuxAckDecidedStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_ExportBegin(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_ExportEnd(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_ExportTableStore(Tasks::MP_Task mpTask, Forth* pForth);

  // job FetchRows
  static void vJob_FetchRowsColumnNameFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_FetchRowsColumnStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_FetchRowsLastBuffersStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_FetchRowsPartitionFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_FetchRowsPlannerStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_FetchRowsSpawnSub(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_FetchRowsSpawnSubInflate(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_FetchRowsTableStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_FetchRowsWhereFetch(Tasks::MP_Task mpTask, Forth* pForth);

  // job GenData
  static void vJob_GenerateGenerate(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_GenerateTableStore(Tasks::MP_Task mpTask, Forth* pForth);

  // job IndexJoin
  static void vJob_IndexJoinLastBuffersStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_IndexJoinLeftIndexStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_IndexJoinLeftTableStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_IndexJoinRightIndexStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_IndexJoinRightIndexNameFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_IndexJoinRightTableStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_IndexJoinRightTableNameFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_IndexJoinSpawnSub(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_SubIndexJoinIndexJoin(Tasks::MP_Task mpTask, Forth* pForth);

  // job inflate
  static void vJob_InflateRowsColumnNameFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_InflateRowsColumnStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_InflateRowsSpawnSub(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_InflateRowsSpawnSubInflate(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_InflateRowsSubInfTasksFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_InflateRowsTableStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_InflateRowsWhereFetch(Tasks::MP_Task mpTask, Forth* pForth);

  // job Ingest
  static void vJob_IngestBegin(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_IngestChunk(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_IngestEnd(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_IngestTableStore(Tasks::MP_Task mpTask, Forth* pForth);

  // job InsertRows
  static void vJob_CopyBufferCopy(Tasks::MP_Task mpTask, Forth* pForth);

  static void vJob_InsertRowPartitionFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_InsertRowRecordStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_InsertRowNewJobWriteRow(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_InsertRowSpawnJobWriteRow(Tasks::MP_Task mpTask, Forth* pForth);

  static void vJob_InsertTableSubInserterJobFetch(Tasks::MP_Task mpTask, Forth* pForth);

  static void vJob_SubInserterBegin(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_SubInserterEnd(Tasks::MP_Task mpTask, Forth* pForth);

  // job KidExport
  static void vJob_KidExportExportOrigin(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_KidExportExportJobId(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_KidExportWriteExportBuffer(Tasks::MP_Task mpTask, Forth* pForth);

  // job LoadTable
  static void vJob_LoadTableBegin(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_LoadTableChunk(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_LoadTableDumpMap(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_LoadTableEnd(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_LoadTableFilePathFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_LoadTableTableStore(Tasks::MP_Task mpTask, Forth* pForth);

  // job Monitor
  static void vJob_MonitorBegin(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_MonitorTake(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_MonitorEnd(Tasks::MP_Task mpTask, Forth* pForth);

  // job NetHog
  static void vJob_NetHogHogNoFetch(Tasks::MP_Task mpTask, Forth* pForth);

  // job NodeInfo
  static void vJob_NodeInfoNodeNameFetch(Tasks::MP_Task mpTask, Forth* pForth);

  // job publish
  static void vJob_PublishSpawnSub(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_PublishTableStore(Tasks::MP_Task mpTask, Forth* pForth);

  // job Purge
  static void vJob_PurgeSpawnSub(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_PurgeTableStore(Tasks::MP_Task mpTask, Forth* pForth);

  // job Query
  static void vJob_QueryBegin(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_QueryEnd(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_QueryExecute(Tasks::MP_Task mpTask, Forth* pForth);

  // job Receiver
  static void vJob_ReceiverConnNoFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_ReceiverConnNoStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_ReceiverJobReceived(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_ReceiverNodeFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_ReceiverNodeStore(Tasks::MP_Task mpTask, Forth* pForth);



  // job Repeater
  static void vJob_RepeaterBegin(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_RepeaterChunk(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_RepeaterEnd(Tasks::MP_Task mpTask, Forth* pForth);

  // job Report
  static void vJob_ReportBegin(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_ReportTake(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_ReportEnd(Tasks::MP_Task mpTask, Forth* pForth);

  // job Select
  static void vJob_SelectBegin(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_SelectEnd(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_SelectExecute(Tasks::MP_Task mpTask, Forth* pForth);

  // job SelectTable
  static void vJob_SelectTablePartitionFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_SelectTableTableStore(Tasks::MP_Task mpTask, Forth* pForth);

  // job Sink
  static void vJob_SinkBegin(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_SinkChunk(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_SinkEnd(Tasks::MP_Task mpTask, Forth* pForth);

  // job Sort
  static void vJob_SortBegin(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_SortEnd(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_SortExecute(Tasks::MP_Task mpTask, Forth* pForth);

  // job SubExporter
  static void vJob_SubExporterBegin(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_SubExporterChunk(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_SubExporterEnd(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_SubExporterSpawn(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_SubExporterExportJobFetch(Tasks::MP_Task mpTask, Forth* pForth);

  // job SubFetch
  static void vJob_SubFetchBufferQueueFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_SubFetchPhraseFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_SubFetchPlannerFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_SubFetchSlotFetch(Tasks::MP_Task mpTask, Forth* pForth);

  // job SubInflate
  static void vJob_SubInflateJobInflateRowsFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_SubInflateInflateRows(Tasks::MP_Task mpTask, Forth* pForth);

  // job SubIngester
  static void vJob_SubIngesterBegin(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_SubIngesterChunk(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_SubIngesterEnd(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_SubIngesterJobIngest(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_SubIngesterSpawn(Tasks::MP_Task mpTask, Forth* pForth);

  // job SubLoader
  static void vJob_SubLoaderBegin(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_SubLoaderChunk(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_SubLoaderEnd(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_SubLoaderSpawn(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_SubLoaderTerminate(Tasks::MP_Task mpTask, Forth* pForth);

  // job SubPublish
  static void vJob_SubPublishBegin(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_SubPublishChunk(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_SubPublishEnd(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_SubPublishSlotFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_SubPublishTableStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_SubPublishPublishJobIdFetch(Tasks::MP_Task mpTask, Forth* pForth);

  // job SubPurger
  static void vJob_SubPurgerBegin(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_SubPurgerChunk(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_SubPurgerEnd(Tasks::MP_Task mpTask, Forth* pForth);

  static void vJob_SubPurgerSlotFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_SubPurgerPurgeJobIdFetch(Tasks::MP_Task mpTask, Forth* pForth);

  // job SubScan
  static void vJob_SubScanBufferQueueFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_SubScanSlotFetch(Tasks::MP_Task mpTask, Forth* pForth);

  // Job SubTruncate
  static void vJob_SubTruncateSlotFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_SubTruncateTruncateJobIdFetch(Tasks::MP_Task mpTask, Forth* pForth);

  // job SubWalker
  static void vJob_SubWalkerBegin(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_SubWalkerChunk(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_SubWalkerEnd(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_SubWalkerWalkJobFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_SubWalkerSpawn(Tasks::MP_Task mpTask, Forth* pForth);

  // Job Truncate
  static void vJob_TruncateTableSpawnSub(Tasks::MP_Task mpTask, Forth* pForth);

  // job Walk
  static void vJob_WalkBegin(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_WalkEnd(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_WalkIndexStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_WalkTableStore(Tasks::MP_Task mpTask, Forth* pForth);

  // Job WriteBuffer
  static void vJob_WriteBufferParentJobIdFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_WriteBufferInsertRows(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_WriteBufferIsIngesting(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_WriteBufferIsInserting(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_WriteBufferIsLoading(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_WriteBufferPartitionFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_WriteBufferTableStore(Tasks::MP_Task mpTask, Forth* pForth);

  // Job WriteRow
  static void vJob_WriteRowParentJobIdFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_WriteRowEncodeRow(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_WriteRowInsertRow(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_WriteRowPartitionFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vJob_WriteRowTableStore(Tasks::MP_Task mpTask, Forth* pForth);

  // Job Operand
  static void vJob_OperandPipeOut(Tasks::MP_Task mpTask, Forth* pForth);

  // load table
  static void vLoadTableChunkFile(Tasks::MP_Task mpTask, Forth* pForth);
  static void vLoadTableDelete(Tasks::MP_Task mpTask, Forth* pForth);
  static void vLoadTableDumpMap(Tasks::MP_Task mpTask, Forth* pForth);
  static void vLoadTableNew(Tasks::MP_Task mpTask, Forth* pForth);
  static void vLoadTableTableStore(Tasks::MP_Task mpTask, Forth* pForth);

  // locals
  static void vLocalFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vLocalStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vLocalsBar(Tasks::MP_Task mpTask, Forth* pForth);
  static void vLocalsPop(Tasks::MP_Task mpTask, Forth* pForth);
  static void vLocalsPush(Tasks::MP_Task mpTask, Forth* pForth);
  static void vLocalsTo(Tasks::MP_Task mpTask, Forth* pForth);

  // lock
  static void vLockDropRead(Tasks::MP_Task mpTask, Forth* pForth);
  static void vLockDropWrite(Tasks::MP_Task mpTask, Forth* pForth);
  static void vLockGrabRead(Tasks::MP_Task mpTask, Forth* pForth);
  static void vLockGrabWrite(Tasks::MP_Task mpTask, Forth* pForth);
  static void vLockInit(Tasks::MP_Task mpTask, Forth* pForth);

  // locks
  static void vLockTrackerList(Tasks::MP_Task mpTask, Forth* pForth);
  static void vLockTrackerReset(Tasks::MP_Task mpTask, Forth* pForth);

  // log
  static void vLogDump(Tasks::MP_Task mpTask, Forth* pForth);
  static void vLogMark(Tasks::MP_Task mpTask, Forth* pForth);
  static void vLogMarkEntry(Tasks::MP_Task mpTask, Forth* pForth);
  static void vLogMarkExit(Tasks::MP_Task mpTask, Forth* pForth);
  static void vLogMarkPhase(Tasks::MP_Task mpTask, Forth* pForth);

  static void vLogMarkWord(Tasks::MP_Task mpTask, Forth* pForth);
  static void vLogMarkSeed(Tasks::MP_Task mpTask, Forth* pForth);

  static void vLogJobSeed(Tasks::MP_Task mpTask, Forth* pForth);

  // logic
  static void vLogicAnd(Tasks::MP_Task mpTask, Forth* pForth);
  static void vLogicNot(Tasks::MP_Task mpTask, Forth* pForth);
  static void vLogicOr(Tasks::MP_Task mpTask, Forth* pForth);

  // magnitude
  static void vBytesMagnitude(Tasks::MP_Task mpTask, Forth* pForth);
  static void vNumberMagnitude(Tasks::MP_Task mpTask, Forth* pForth);
  static void vToDigitName(Tasks::MP_Task mpTask, Forth* pForth);

  // memory
  static void vMemoryByteDump(Tasks::MP_Task mpTask, Forth* pForth);
  static void vMemoryReset(Tasks::MP_Task mpTask, Forth* pForth);
  static void vMemoryStatistics(Tasks::MP_Task mpTask, Forth* pForth);
  static void vMemoryWordDump(Tasks::MP_Task mpTask, Forth* pForth);

  // node (of grid)
  static void vNodeForget(Tasks::MP_Task mpTask, Forth* pForth);
  static void vNodeIP(Tasks::MP_Task mpTask, Forth* pForth);
  static void vNodeName(Tasks::MP_Task mpTask, Forth* pForth);
  static void vNodeNew(Tasks::MP_Task mpTask, Forth* pForth);
  static void vNodeNo(Tasks::MP_Task mpTask, Forth* pForth);
  static void vNodePartitions(Tasks::MP_Task mpTask, Forth* pForth);
  static void vNodePort(Tasks::MP_Task mpTask, Forth* pForth);
  static void vNodeRecvSocketStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vNodeRecvSocketFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vNodeSendSocketStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vNodeSendSocketFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vNodeSize(Tasks::MP_Task mpTask, Forth* pForth);
  static void vNodeStartNode(Tasks::MP_Task mpTask, Forth* pForth);
  static void vNodeStateStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vNodeStateFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vNodeStatus(Tasks::MP_Task mpTask, Forth* pForth);

  // object
  static void vObjectClassFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vObjectClassStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vObjectDump(Tasks::MP_Task mpTask, Forth* pForth);
  static void vObjectFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vObjectMagicMark(Tasks::MP_Task mpTask, Forth* pForth);
  static void vObjectStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vObjectWordBodySet(Tasks::MP_Task mpTask, Forth* pForth);

  // persistence
  static void vPersistenceCheckpoint(Tasks::MP_Task mpTask, Forth* pForth);
  static void vPersistenceReload(Tasks::MP_Task mpTask, Forth* pForth);

  // plan
  static void vPlanCompile(Tasks::MP_Task mpTask, Forth* pForth);
  static void vPlanDelete(Tasks::MP_Task mpTask, Forth* pForth);
  static void vPlanDic(Tasks::MP_Task mpTask, Forth* pForth);
  static void vPlanExecutingTaskFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vPlanExecutingTaskStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vPlanFinished(Tasks::MP_Task mpTask, Forth* pForth);
  static void vPlanIdFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vPlanLambdaAppend(Tasks::MP_Task mpTask, Forth* pForth);
  static void vPlanLambdaFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vPlanLambdaStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vPlanNew(Tasks::MP_Task mpTask, Forth* pForth);
  static void vPlanNameFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vPlanNameStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vPlanShow(Tasks::MP_Task mpTask, Forth* pForth);

  // planner
  static void vPlannerChoosePaths(Tasks::MP_Task mpTask, Forth* pForth);
  static void vPlannerChosenPath(Tasks::MP_Task mpTask, Forth* pForth);
  static void vPlannerDelete(Tasks::MP_Task mpTask, Forth* pForth);
  static void vPlannerFetchTechniqueFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vPlannerGenPaths(Tasks::MP_Task mpTask, Forth* pForth);
  static void vPlannerHighKeyFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vPlannerIndexNmFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vPlannerKeyFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vPlannerLowKeyFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vPlannerMatchPaths(Tasks::MP_Task mpTask, Forth* pForth);
  static void vPlannerNew(Tasks::MP_Task mpTask, Forth* pForth);
  static void vPlannerParseWhere(Tasks::MP_Task mpTask, Forth* pForth);
  static void vPlannerPhrasesFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vPlannerScorePaths(Tasks::MP_Task mpTask, Forth* pForth);
  static void vPlannerShowPaths(Tasks::MP_Task mpTask, Forth* pForth);
  static void vPlannerTableFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vPlannerTableStore(Tasks::MP_Task mpTask, Forth* pForth);

  // process
  static void vProcessGetPid(Tasks::MP_Task mpTask, Forth* pForth);
  static void vProcessQuit(Tasks::MP_Task mpTask, Forth* pForth);
  static void vProcessTestPid(Tasks::MP_Task mpTask, Forth* pForth);

  // prompt
  static void vPromptContinue(Tasks::MP_Task mpTask, Forth* pForth);
  static void vPromptExecuteFunction(Tasks::MP_Task mpTask, Forth* pForth);
  static void vPromptGetCommand(Tasks::MP_Task mpTask, Forth* pForth);
  static void vPromptNew(Tasks::MP_Task mpTask, Forth* pForth);
  static void vPromptSession(Tasks::MP_Task mpTask, Forth* pForth);

  // push
  static void vPushI(Tasks::MP_Task mpTask, Forth* pForth);
  static void vPushJ(Tasks::MP_Task mpTask, Forth* pForth);
  static void vPushNullPtr(Tasks::MP_Task mpTask, Forth* pForth);
  static void vPushValue(Tasks::MP_Task mpTask, Forth* pForth);

  // queue
  static void vQueueAppend(Tasks::MP_Task mpTask, Forth* pForth);
  static void vQueueDelete(Tasks::MP_Task mpTask, Forth* pForth);
  static void vQueueDump(Tasks::MP_Task mpTask, Forth* pForth);
  static void vQueueInit(Tasks::MP_Task mpTask, Forth* pForth);
  static void vQueueNew(Tasks::MP_Task mpTask, Forth* pForth);
  static void vQueuePop(Tasks::MP_Task mpTask, Forth* pForth);

  // queues
  static void vQueuesCheckSleeping(Tasks::MP_Task mpTask, Forth* pForth);
  static void vQueuesExecutingLength(Tasks::MP_Task mpTask, Forth* pForth);
  static void vQueuesReadyLength(Tasks::MP_Task mpTask, Forth* pForth);
  static void vQueuesSleepingLength(Tasks::MP_Task mpTask, Forth* pForth);
  static void vQueuesWaitingLength(Tasks::MP_Task mpTask, Forth* pForth);

  // rbCursor
  static void vRbCursorBegin(Tasks::MP_Task mpTask, Forth* pForth);
  static void vRbCursorDelete(Tasks::MP_Task mpTask, Forth* pForth);
  static void vRbCursorEnd(Tasks::MP_Task mpTask, Forth* pForth);
  static void vRbCursorFirst(Tasks::MP_Task mpTask, Forth* pForth);
  static void vRbCursorGet(Tasks::MP_Task mpTask, Forth* pForth);
  static void vRbCursorInit(Tasks::MP_Task mpTask, Forth* pForth);
  static void vRbCursorNew(Tasks::MP_Task mpTask, Forth* pForth);
  static void vRbCursorNext(Tasks::MP_Task mpTask, Forth* pForth);
  static void vRbCursorTableFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vRbCursorTableStore(Tasks::MP_Task mpTask, Forth* pForth);

  // rowCursor
  static void vRowCursorBegin(Tasks::MP_Task mpTask, Forth* pForth);
  static void vRowCursorDelete(Tasks::MP_Task mpTask, Forth* pForth);
  static void vRowCursorEnd(Tasks::MP_Task mpTask, Forth* pForth);
  static void vRowCursorEntBufferFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vRowCursorEntBufferStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vRowCursorFirst(Tasks::MP_Task mpTask, Forth* pForth);
  static void vRowCursorInit(Tasks::MP_Task mpTask, Forth* pForth);
  static void vRowCursorNew(Tasks::MP_Task mpTask, Forth* pForth);
  static void vRowCursorNext(Tasks::MP_Task mpTask, Forth* pForth);
  static void vRowCursorRow(Tasks::MP_Task mpTask, Forth* pForth);
  static void vRowCursorTableFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vRowCursorTableStore(Tasks::MP_Task mpTask, Forth* pForth);

  // rstack
  static void vRstackCsPick(Tasks::MP_Task mpTask, Forth* pForth);
  static void vRstackCsRoll(Tasks::MP_Task mpTask, Forth* pForth);
  static void vRstackDepth(Tasks::MP_Task mpTask, Forth* pForth);
  static void vRstackDrop(Tasks::MP_Task mpTask, Forth* pForth);
  static void vRstackFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vRstackFetchTwo(Tasks::MP_Task mpTask, Forth* pForth);
  static void vRstackPick(Tasks::MP_Task mpTask, Forth* pForth);
  static void vRstackReverse(Tasks::MP_Task mpTask, Forth* pForth);
  static void vRstackRoll(Tasks::MP_Task mpTask, Forth* pForth);
  static void vRstackRollInv(Tasks::MP_Task mpTask, Forth* pForth);
  static void vRstackStk(Tasks::MP_Task mpTask, Forth* pForth);
  static void vRstackSwap(Tasks::MP_Task mpTask, Forth* pForth);
  static void vRstackToP(Tasks::MP_Task mpTask, Forth* pForth);
  static void vRstackToR(Tasks::MP_Task mpTask, Forth* pForth);
  static void vRstackToRTwo(Tasks::MP_Task mpTask, Forth* pForth);
  static void vRstack0(Tasks::MP_Task mpTask, Forth* pForth);
  static void vRstack1(Tasks::MP_Task mpTask, Forth* pForth);
  static void vRstack2(Tasks::MP_Task mpTask, Forth* pForth);
  static void vRstack3(Tasks::MP_Task mpTask, Forth* pForth);
  static void vRstack4(Tasks::MP_Task mpTask, Forth* pForth);
  static void vRstack5(Tasks::MP_Task mpTask, Forth* pForth);
  static void vRstack6(Tasks::MP_Task mpTask, Forth* pForth);
  static void vRstack7(Tasks::MP_Task mpTask, Forth* pForth);
  static void vRstack8(Tasks::MP_Task mpTask, Forth* pForth);
  static void vRstack9(Tasks::MP_Task mpTask, Forth* pForth);

  // schLock
  static void vSchLockDropWrite(Tasks::MP_Task mpTask, Forth* pForth);
  static void vSchLockGrabWrite(Tasks::MP_Task mpTask, Forth* pForth);

  // session
  static void vSessionCommand(Tasks::MP_Task mpTask, Forth* pForth);
  static void vSessionDicStack(Tasks::MP_Task mpTask, Forth* pForth);
  static void vSessionDump(Tasks::MP_Task mpTask, Forth* pForth);
  static void vSessionIsExiting(Tasks::MP_Task mpTask, Forth* pForth);
  static void vSessionPid(Tasks::MP_Task mpTask, Forth* pForth);
  static void vSessionSlotFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vSessionTxFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vSessionTxStore(Tasks::MP_Task mpTask, Forth* pForth);

  // shell
  static void vShellExecute(Tasks::MP_Task mpTask, Forth* pForth);

  // slots
  static void vSlotsColumnCount(Tasks::MP_Task mpTask, Forth* pForth);
  static void vSlotsColumnName(Tasks::MP_Task mpTask, Forth* pForth);
  static void vSlotsCreateColumnVec(Tasks::MP_Task mpTask, Forth* pForth);
  static void vSlotsSubDic(Tasks::MP_Task mpTask, Forth* pForth);

  // socket
  static void vSocketAccept(Tasks::MP_Task mpTask, Forth* pForth);
  static void vSocketClose(Tasks::MP_Task mpTask, Forth* pForth);
  static void vSocketConnect(Tasks::MP_Task mpTask, Forth* pForth);
  static void vSocketDropWrite(Tasks::MP_Task mpTask, Forth* pForth);
  static void vSocketGrabWrite(Tasks::MP_Task mpTask, Forth* pForth);
  static void vSocketListen(Tasks::MP_Task mpTask, Forth* pForth);
  static void vSocketOpen(Tasks::MP_Task mpTask, Forth* pForth);
  static void vSocketRecvObject(Tasks::MP_Task mpTask, Forth* pForth);
  static void vSocketRecvObject_LowMem(Tasks::MP_Task mpTask, Forth* pForth);
  static void vSocketSendObject(Tasks::MP_Task mpTask, Forth* pForth);

  // steps
  static void vStepTimingReset(Tasks::MP_Task mpTask, Forth* pForth);
  static void vStepTimingStart(Tasks::MP_Task mpTask, Forth* pForth);
  static void vStepTimingWrite(Tasks::MP_Task mpTask, Forth* pForth);

  // string
  static void vStrAlgebra(Tasks::MP_Task mpTask, Forth* pForth);
  static void vStrCat(Tasks::MP_Task mpTask, Forth* pForth);
  static void vStrClip(Tasks::MP_Task mpTask, Forth* pForth);
  static void vStrEqual(Tasks::MP_Task mpTask, Forth* pForth);
  static void vStrEscape(Tasks::MP_Task mpTask, Forth* pForth);
  static void vStrFirstCsv(Tasks::MP_Task mpTask, Forth* pForth);
  static void vStrFormat(Tasks::MP_Task mpTask, Forth* pForth);
  static void vStrLeft(Tasks::MP_Task mpTask, Forth* pForth);
  static void vStrLength(Tasks::MP_Task mpTask, Forth* pForth);
  static void vStrPrepend(Tasks::MP_Task mpTask, Forth* pForth);
  static void vStrRandom(Tasks::MP_Task mpTask, Forth* pForth);
  static void vStrRemovePrefix(Tasks::MP_Task mpTask, Forth* pForth);
  static void vStrRemoveSuffix(Tasks::MP_Task mpTask, Forth* pForth);
  static void vStrReplace(Tasks::MP_Task mpTask, Forth* pForth);
  static void vStrRight(Tasks::MP_Task mpTask, Forth* pForth);
  static void vStrSub(Tasks::MP_Task mpTask, Forth* pForth);
  static void vStrTailInt(Tasks::MP_Task mpTask, Forth* pForth);

  // sub loader
  static void vSubLoaderDelete(Tasks::MP_Task mpTask, Forth* pForth);

  // table
  static void vTableAddIndex(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTableAnalyze(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTableColumnCountFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTableCreateColumnVec(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTableDrop(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTableDump(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTableFindColumn(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTableFirstBuffer(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTableGenData(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTableGetColumnsAsString(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTableColumnFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTableGetPtn(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTableInflateScan(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTableIndexCount(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTableInit(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTableIsColumn(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTableIsLoadingStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTableLazyBuildIndexes(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTableList(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTableName(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTablePublish(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTablePurge(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTableRemoveIndex(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTableScan(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTableShowColumnVec(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTableSlots(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTableSpace(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTableSubDic(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTableSubLoaders(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTableTruncate(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTableIsValidIndexColumns(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTableVerify(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTableVisualize(Tasks::MP_Task mpTask, Forth* pForth);

  // table partition
  static void vTablePtnAttemptGrabRead(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTablePtnAttemptGrabWrite(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTablePtnDropRead(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTablePtnDropWrite(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTablePtnDump(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTablePtnIsLocked(Tasks::MP_Task mpTask, Forth* pForth);

  // task
  static void vTaskAck(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTaskAcksStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTaskBreakPoint(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTaskDestructorTraceFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTaskDestructorTraceStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTaskDicStack(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTaskDropLock(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTaskDump(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTaskDelete(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTaskGrabLock(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTaskIsAckSet(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTaskJobFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTaskJobStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTaskNameFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTaskThis(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTaskSeed(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTaskSessionFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTaskSessionStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTaskSendNodeFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTaskSendNodeStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTaskSleepMsec(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTaskSleepNsec(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTaskSleepSsec(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTaskSleepUsec(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTaskStateFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTaskThread(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTaskWait(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTaskYield(Tasks::MP_Task mpTask, Forth* pForth);

  // test
  static void vTestTest0(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTestTest1(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTestTest2(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTestTest3(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTestTest4(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTestTest5(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTestTest6(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTestTest7(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTestTest8(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTestTest9(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTestTest10(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTestTest11(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTestTest12(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTestTest13(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTestTest14(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTestTest15(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTestTest16(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTestTest17(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTestTest18(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTestTest19(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTestTest20(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTestTest21(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTestTest22(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTestTest23(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTestTest24(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTestTest25(Tasks::MP_Task mpTask, Forth* pForth);

  // thread
  static void vThreadBind(Tasks::MP_Task mpTask, Forth* pForth);
  static void vThreadCpu(Tasks::MP_Task mpTask, Forth* pForth);
  static void vThreadDedicatedFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vThreadDedicatedStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vThreadDisableRealtime(Tasks::MP_Task mpTask, Forth* pForth);
  static void vThreadDump(Tasks::MP_Task mpTask, Forth* pForth);
  static void vThreadEnableRealtime(Tasks::MP_Task mpTask, Forth* pForth);
  static void vThreadId(Tasks::MP_Task mpTask, Forth* pForth);
  static void vThreadSleepMsec(Tasks::MP_Task mpTask, Forth* pForth);
  static void vThreadSleepNsec(Tasks::MP_Task mpTask, Forth* pForth);
  static void vThreadSleepSsec(Tasks::MP_Task mpTask, Forth* pForth);
  static void vThreadSleepUsec(Tasks::MP_Task mpTask, Forth* pForth);

  // timer
  static void vTimerDelete(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTimerInit(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTimerMicrosecs(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTimerMillisecs(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTimerNanosecs(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTimerNew(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTimerReset(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTimerSecs(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTimerStart(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTimerTicks(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTimerUpdate(Tasks::MP_Task mpTask, Forth* pForth);

  static void vTimeCurrent(Tasks::MP_Task mpTask, Forth* pForth);

  // to
  static void vToBody(Tasks::MP_Task mpTask, Forth* pForth);
  static void vToInt(Tasks::MP_Task mpTask, Forth* pForth);
  static void vToPtr(Tasks::MP_Task mpTask, Forth* pForth);
  static void vToStr(Tasks::MP_Task mpTask, Forth* pForth);
  static void vToTask(Tasks::MP_Task mpTask, Forth* pForth);

  // token
  static void vTokenColon(Tasks::MP_Task mpTask, Forth* pForth);

  // trace
  static void vTraceOff(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTraceOn(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTracePoint(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTraceSet(Tasks::MP_Task mpTask, Forth* pForth);

  // tx
  static void vTxBegin(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTxCommit(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTxCounterDec(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTxCounterFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTxCounterStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTxDistMemberFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTxDistMemberStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTxDump(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTxDumpAll(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTxFree(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTxId(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTxPrepare(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTxRollback(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTxTestDist(Tasks::MP_Task mpTask, Forth* pForth);

  // txCollection
  static void vTxCollectionDump(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTxCollectionInit(Tasks::MP_Task mpTask, Forth* pForth);

  // txCursor
  static void vTxCursorBegin(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTxCursorDelete(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTxCursorEnd(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTxCursorFirst(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTxCursorGet(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTxCursorInit(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTxCursorNew(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTxCursorNext(Tasks::MP_Task mpTask, Forth* pForth);

  // txMap
  static void vTxMapInit(Tasks::MP_Task mpTask, Forth* pForth);

  // txRef
  static void vTxRefBegin(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTxRefCheckDistId(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTxRefCommit(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTxRefDump(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTxRefEnd(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTxRefId(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTxRefInit(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTxRefNew(Tasks::MP_Task mpTask, Forth* pForth);
  static void vTxRefRollback(Tasks::MP_Task mpTask, Forth* pForth);

  // utility
  static void vUtilityPrefetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vUtilitySchemaTest(Tasks::MP_Task mpTask, Forth* pForth);

  // vector
  static void vVectorDifference(Tasks::MP_Task mpTask, Forth* pForth);

  // word
  static void vWord(Tasks::MP_Task mpTask, Forth* pForth);
  static void vWordCategoryFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vWordCategoryStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vWordDump(Tasks::MP_Task mpTask, Forth* pForth);
  static void vWordEnmSetFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vWordEnmSetStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vWordExitHookGet(Tasks::MP_Task mpTask, Forth* pForth);
  static void vWordExitHookSet(Tasks::MP_Task mpTask, Forth* pForth);
  static void vWordForget(Tasks::MP_Task mpTask, Forth* pForth);
  static void vWordImmediateGet(Tasks::MP_Task mpTask, Forth* pForth);
  static void vWordImmediateSet(Tasks::MP_Task mpTask, Forth* pForth);
  static void vWordIsImmediate(Tasks::MP_Task mpTask, Forth* pForth);
  static void vWordIsIntrinsic(Tasks::MP_Task mpTask, Forth* pForth);
  static void vWordKindFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vWordKindStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vWordNameFetch(Tasks::MP_Task mpTask, Forth* pForth);
  static void vWordNameStore(Tasks::MP_Task mpTask, Forth* pForth);
  static void vWordPrev(Tasks::MP_Task mpTask, Forth* pForth);

  // zero
  static void vZeroLess(Tasks::MP_Task mpTask, Forth* pForth);
  static void vZeroLessEqual(Tasks::MP_Task mpTask, Forth* pForth);
  static void vZeroEqual(Tasks::MP_Task mpTask, Forth* pForth);
  static void vZeroNotEqual(Tasks::MP_Task mpTask, Forth* pForth);
  static void vZeroGreaterEqual(Tasks::MP_Task mpTask, Forth* pForth);
  static void vZeroGreater(Tasks::MP_Task mpTask, Forth* pForth);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
