/*============================================================================*/
/* LogicUnit.hpp -- Logic Unit                                                */
/*============================================================================*/
#pragma once

#include "RowSingle.hpp"

#include "CrwToRow.hpp"
#include "ToPtnCrw.hpp"
#include "ToNstRow.hpp"
#include "ToPtnRow.hpp"
#include "ToRow.hpp"
#include "ToBktCrw.hpp"
#include "ToBktRow.hpp"


#include "Pot.hpp"
#include "Tub.hpp"
#include "Combined.hpp"

#include "EntWrk.hpp"

#include "RluCommon.h"
#include "RluDiary.hpp"
#include "DevIXT.hpp"
#include "DevROW.hpp"
#include "LogicTimer.hpp"
#include "LogicCompiler.hpp"
#include "ExprEval.hpp"
#include "ShardDef.hpp"
#include "MetaGet.hpp"
#include "BktMapGet.hpp"
#include "BucketGet.hpp"
#include "BucketInfo.hpp"
#include "StreamDefRht.hpp"
#include "Stencil.hpp"
#include "Stencil.hpp"
#include "SubCall.hpp"
#include "SubReturn.hpp"

#include "InflateSet.h"
#include "InfoSet.hpp"
#include "PipeSpec.hpp"
#include "NamedBuffer.hpp"
#include "Optimiser.hpp"

#include "IndexTree.hpp"
#include "JobMetrics.hpp"

#include "eLH.h"
#include "eXK.h"
#include "eEK.h"
#include "ePE.h"

#include "REG_t.hpp"


namespace STORE { class IndexStream; class Node; class Node_I;
                  class PathIdentifier;  class RowOnce;}

namespace STORE
{
  static const i8 C_LOGIC_DEV   = 16;
  static const i8 C_LOGIC_CODE  = C_4KB;
  static const i8 C_LOGIC_COND  = 8;
  static const i8 C_LOGIC_LABEL = 128;

  struct JumpLabel
  {
    s32  m_sLabelNm;
    bool m_bDefined;
    bool m_bSubroutine;
    bool m_bDestination;
    union {
      i8 m_i_Code;
      i8 m_i_Chain;
    };
  };


#define I_PushInt(__iValue) \
  *(++aiDstk) = __iValue;

#define I_PopInt()   *(aiDstk--)

#define P_PushPtr(__pPtr) \
  *(++aiDstk) = (i8)__pPtr;

#define P_PopPtr()   (void*)(*(aiDstk--))

#define S_PushStr(__azStr) \
  /*MemLib::vDumpWords(iaDataStack,nDifference(iaDataStack,aiDstk)+8);*/ \
  if (((i8)(__azStr)) == C_DATUM_NULL) {  \
    *(++aiDstk) = C_DATUM_NULL; \
  } \
  else { \
    nSize = StrLib::nSize((char*)__azStr); \
    azDst = (char*)(++aiDstk); \
    StrLib::vCopy(azDst,(char*)__azStr); \
    aiDstk += nWordRoundedStrBytes(nSize)/8 - 1; \
    *(++aiDstk) = nSize;   \
  }
  /*MemLib::vDumpWords(iaDataStack,nDifference(iaDataStack,aiDstk)+8); */

#define S_PopStr(__azDst) \
  /*MemLib::vDumpWords(iaDataStack,nDifference(iaDataStack,aiDstk)+8);*/ \
  if (*aiDstk == C_DATUM_NULL)    {                                      \
    *(i8*)__azDst = C_DATUM_NULL;                                        \
    aiDstk--;                                                            \
  } \
  else { \
    nSize = *(aiDstk--); \
    azSrc = (char*)(aiDstk - (nWordRoundedStrBytes(nSize)/8 - 1)); \
    StrLib::vCopy(__azDst,azSrc); \
    aiDstk -= nWordRoundedStrBytes(nSize)/8;                        \
  }                       \
  /*MemLib::vDumpWords(iaDataStack,nDifference(iaDataStack,aiDstk)+8);*/

#define S_StrAdr() \
  ( (char*)(aiDstk) - nWordRoundedStrBytes(*aiDstk))

#define S_StrDrp() \
  if (*aiDstk == C_DATUM_NULL)    {                              \
    aiDstk--;  \
  } \
  else {                 \
    nSize = *(aiDstk--); \
    aiDstk -= nWordRoundedStrBytes(nSize)/8;    \
  }

#define S_StrDup() \
  if (*aiDstk == C_DATUM_NULL)    {                                      \
    *(++aiDstk) = C_DATUM_NULL;  \
  } \
  else { \
    nSize = *(aiDstk); \
    azSrc = (char*)(aiDstk - (nWordRoundedStrBytes(nSize)/8 - 1) - 1);  \
    azDst = (char*)(++aiDstk); \
    StrLib::vCopy(azDst,(char*)azSrc); \
    aiDstk += nWordRoundedStrBytes(nSize)/8 - 1; \
    *(++aiDstk) = nSize;                                                 \
  }

#define JumpTo(__iArg,__EO) \
  aeCode = &m_eaCode[__iArg];

  //----------------------------------------------------------------------------

  class LogicUnit : public Object
  {
  private:
  
  public:
  
    void* m_aTinyFunc;  //
    
    Job*  m_pJob;       // which job
    Op*   m_pOp;        // which op
    
    eLH   m_eDdlLocked; // this task has the ddl lock?
    i8    m_i_NextRnd;  // which next repeatable random nummber?
    i8    m_iDebugFlag; // general purpose debug flag

    REF<Job> m_xpListedJob;   // listed job reference
    
    REG<Object,CN_NONE>*        m_paReg[C_MAX_REGS];
    i8 m_nRegs;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    REG<AggTimer,       CN_AGGTIMER      > m_xpAggTimer;
    REG<IntStack,       CN_AGGTIMER      > m_xpAggTypeStack;
    REG<IntStack,       CN_INTSTACK      > m_xpPreExpStack;
  
    REG<Bucket,         CN_BUCKET        > m_xpBucket;
    REG<BucketGet,      CN_BUCKETGET     > m_xpBucketGet;
    REG<BucketGet,      CN_BUCKETINFO    > m_xpBucketInfo;
    REG<BktMapGet,      CN_BKTMAPGET     > m_xpBktMapGet;

    REG<Combined,       CN_COMBINED      > m_xpCombined;
    REG<CrwBuffer,      CN_CRWBUFFER     > m_xpCrwBuffer;
    REG<CrwToRow,       CN_CRWTOROW      > m_xpCrwToRow;
    REG<CsvBuffer,      CN_CSVBUFFER     > m_xpCsvBuffer;

    REG<DataBuffer,     CN_NONE          > m_xpDataBuffer;
    REG<Dir,            CN_DIR           > m_xpDir0;
    REG<Dir,            CN_DIR           > m_xpDir1;
    REG<Dynamo,         CN_DYNAMO        > m_xpDynamo;

    REG<Operand,        CN_NONE          > m_xpErrorMarker;
    REG<ExprEval,       CN_EXPREVAL      > m_xpExprEval;

    REG<File,           CN_FILE          > m_xpFile0;
    REG<File,           CN_FILE          > m_xpFile1;
    REG<ExprEval,       CN_EXPREVAL      > m_xpFilter;
  
    REG<Grid,           CN_GRID          > m_xpGrid;
  
    REG<Word,           CN_WORD          > m_xpHandler;

    REG<Index,          CN_INDEX         > m_xpIndex;
    REG<IndexDef,       CN_INDEXDEF      > m_xpIndexDef;
    REG<IndexStream,    CN_INDEXSTREAM   > m_xpIndexStream;
    REG<InfoSet,        CN_INFOSET       > m_xpInfoSet;
    REG<Row,            CN_ROW           > m_xpInfoSetRow;
    REG<Table,          CN_TABLE         > m_xpInfoSetTable;
    REG<IntStack,       CN_INTSTACK      > m_xpIntStack;

    REG<JobMetrics,     CN_JOBMETRICS    > m_xpJobMetrics;
  
    REG<RowSingleList,  CN_ROWSINGLELIST > m_xpList;
    REG<LogicCompiler,  CN_LOGICCOMPILER > m_xpLogicCompiler;

    REG<Operand,        CN_NONE          > m_xpMarker;
    REG<RowBuffer,      CN_ROWBUFFER     > m_xpMemoryHog;
    REG<MetaGet,        CN_METAGET       > m_xpMetaGet;

    REG<NamedBuffer,    CN_NAMEDBUFFER   > m_xpNamedBuffer;
    REG<Node,           CN_NODE          > m_xpNode;
    REG<Node_I,         CN_NODE_I        > m_xpNode_I;
    REG<Now,            CN_NOW           > m_xpNow;

    REG<Op,             CN_OP            > m_xpOp;
    REG<Operand,        CN_NONE          > m_xpOperandOoO;
    REG<Optimiser,      CN_OPTIMISER     > m_xpOptimiser;

    REG<PathIdentifier, CN_PATHIDENTIFIER> m_xpPathIdentifier;
    REG<Parcel,         CN_PARCEL        > m_xpParcel;
    REG<Operand,        CN_NONE          > m_xpPipein;
    REG<Pipeline,       CN_PIPELINE      > m_xpPipeline;
    REG<PipeSpec,       CN_PIPESPEC      > m_xpPipeSpec;
    REG<Operand,        CN_NONE          > m_xpPlanOperand;
    REG<Pot,            CN_POT           > m_xpPot;
    REG<PtnStream,      CN_PTNSTREAM     > m_xpPtnStream;

    REG<Object,         CN_NONE          > m_xpRecvdObject;
    REG<RowBuffer,      CN_ROWBUFFER     > m_xpRowBuffer;
    REG<RowOnce,        CN_ROWONCE       > m_xpRowOnce;

    REG<Node,           CN_NODE          > m_xpSendNode;
    REG<Sends,          CN_SENDS         > m_xpSends;
    REG<ShardDef,       CN_SHARDDEF      > m_xpShardDef;
    REG<Socket,         CN_SOCKET        > m_xpSocket;
    REG<Stage,          CN_STAGE         > m_xpStage;
    REG<Stats,          CN_STATS         > m_xpStats;
    REG<Stencil,        CN_STENCIL       > m_xpStencil;
    REG<Stream,         CN_STREAM        > m_xpStream;
    REG<StreamDef,      CN_STREAMDEF     > m_xpStreamDefLft;
    REG<StreamDef,      CN_NONE          > m_xpStreamDefOut;
    REG<StreamDefRht,   CN_STREAMDEFRHT  > m_xpStreamDefRht;
    REG<Operand,        CN_NONE          > m_xpStreamSet;
    REG<Table,          CN_TABLE         > m_xpStreamSetTable;
    REG<SubCall,        CN_NONE          > m_xpSubCall;
    REG<Pipeline,       CN_PIPELINE      > m_xpSubPipe;
    REG<SubReturn,      CN_NONE          > m_xpSubReturn;
    REG<Operand,        CN_NONE          > m_xpSubset;

    REG<Table,          CN_TABLE         > m_xpTable;
    REG<TablePtn,       CN_TABLEPTN      > m_xpTablePtn;
    REG<TagSet,         CN_TAGSET        > m_xpTagSet;
    REG<Timer,          CN_TIMER         > m_xpTimer;
    REG<TinyC,          CN_TINYC         > m_xpTinyC;
    REG<ToBktCrw,       CN_TOBKTCRW      > m_xpToBktCrw;
    REG<ToBktRow,       CN_TOBKTROW      > m_xpToBktRow;
    REG<ToNstRow,       CN_TONSTROW      > m_xpToNstRow;
    REG<ToPtnCrw,       CN_TOPTNCRW      > m_xpToPtnCrw;
    REG<ToPtnRow,       CN_TOPTNROW      > m_xpToPtnRow;
    REG<ToRow,          CN_TOROW         > m_xpToRow;
    REG<Tub,            CN_TUB           > m_xpTub;
    REG<Tx,             CN_TX            > m_xpTx;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    bool m_bCompilerToExit;
    bool m_bIsLftCrwBuffer; // left buffer is a CRW buffer (joins)

    eEK  m_eExprKind;

    i8   m_iCounterA;   // counter
    i8   m_iCounter0;   // counter
    i8   m_iCounter1;   // counter
    i8   m_iCounter2;   // counter
    i8   m_iCounter3;   // counter
    i8   m_iCounter4;   // counter
    i8   m_iCounter5;   // counter
    i8   m_iCounter6;   // counter
    i8   m_iCounter7;   // counter
  
    bool (*m_afFun0)(void*);
    bool (*m_afFun1)(void*);
    bool (*m_afFun2)(void*);
    bool (*m_afFun3)(void*);

    s4K  m_sStringA;    // string
    s256 m_sString0;    // string
    s256 m_sString1;    // string
    s256 m_sString2;    // string
    s256 m_sString3;    // string
    s256 m_sString4;    // string
    s256 m_sString5;    // string
    s256 m_sString6;    // string
    s256 m_sString7;    // string

    Ent* m_aLowerEntry;
    Ent* m_aUpperEntry;
    EntBuffer* m_pEntBufferOuter;
        
    Atomic m_ixRowsProcessed; // rows processed
    Atomic m_ixRowsInvalid;   // invalid rows detected
    Atomic m_ixRowsSkipped;   // rows skipped
    eEC    m_eErrorCode_reg;      // error code
    
    eZK m_ePipeInOperand;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // inner loop
  
    i8 (*m_afDynFun)(void* aLU, i8 nOuterArgs, i8 iOuterArg0);
  
    TokenVec*  m_pTknVec;
    Token*     m_pTkn;
    i8         m_i_Tkn;
    
    RluCommon  m_xRluCommon ;
    RluDiary*  m_pRluDiary;
    Timer      m_oSince;
    Timer      m_oTimer;
    AggTimer   m_oAggTimer;

    RowPix     m_oRowPix;    // condensed stream def
    EntWrk     m_oEntWrk;
    EntWrk     m_oEntWrkPL;
    EntWrk     m_oEntWrkSRT;
    EntWrk     m_oEntWrkJ;
    EntWrk     m_oEntWrkT;

    DevROW*    m_pDevDstRow;
    DevROW*    m_pDevLftRow;
    DevROW*    m_pDevRhtRow;
    DevROW*    m_pDevAltRow;

    DevIXT*    m_pDevAltIxt;
    DevIXT*    m_pDevLftIxt;
    DevIXT*    m_pDevRhtIxt;
    DevIXT*    m_pDevTmpIxt;
  
    LogicTimer* m_pLogicTimer;

    u8      m_uSelectColumns;

    i8      m_i_Code;
    char    m_eaCode[C_LOGIC_CODE];    // { EO_code [<bytes>] }

    i8        m_nLabels;
    JumpLabel m_xaJumpLabel[C_LOGIC_LABEL];

    bool m_baCondInd[C_LOGIC_COND];

    i8 m_iNextCond;
  
    bool m_bSuperFrag;
    
    i8   m_nPotSize;
  
    eXK  m_eSendKind;      // send kind

  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    LogicUnit(void);
   ~LogicUnit(void) noexcept(false);
  
    void vContentDump(void);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    void vAddReg(REG<Object,CN_NONE>* pReg);
    void vCC(i8 i_Cond);
    void vCleanup(void);
    eEO  eCode(i8 i_Code);
    i8   nCodes(void);
    char* aCodeArg(i8 i_Code);
    i8   iCodeArg(i8 i_Code);
    RluDiary* pDiary(void);
    void vDiaryJobNote(Stz* szJobNote);
    void vDiaryName(Stz* szName);
    void vDiaryNote(i8 i_EnumGroup, i8 i_Enum, i8 iItem, i8 nNanoSecs,
                    i2 iAction, i2 i_ActEnumGroup, i2 i_ActEnum, i8 nQueueLength);
    void vDiaryDir(Stz* szDir);
    void vDiaryTitle(Stz* szTitle);
    void vDumpRegisters(void);
    void vEndOptimise(void);
    void vEnter(void);
    void vExit(void);
    void vFailure(void);
    i8   iFindLabel(i8 i_Code);
    i8   iFindLabel(Stz* szLabelNm);
    void vInit(void);
    Job* pJob(void);
    void vJob(Job* pJob);
    i8   iLabelCode(i8 i_Label);
    bool bLabelIsDst(i8 i_Label);
    bool bLabelIsSbr(i8 i_Label);
    i8   nLabels(void);
    Stz* szLabelNm(i8 i_Label);
    Op*  pOp(void);
    void vOp(Op* pOp);
    void vOptimise(eEO eOpt);
    i8   iQueryFragment(void);
    void vSetCC(i8 i_Cond, bool bState);
    void vSetPipein(Operand* pOperand);


    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
    EntWrk* pEntWrk(void);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
    // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

    void vAcceptToken(void);
    void vBeginToken(i8 i_Tkn);
    void vCollectOpcode(void);
  
    bool bGenerateAsm(Stz* sText);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
    #include "RLU_interface.h"

  private:
    void vAddCode(eEO eCode);
    void vAddCodeArg(eEO eCode, i8 iArg);
    void vAddCodeArg(eEO eCode, Token* pTkn);
    void vAddCodeDte(eEO eCode, Stz* szLiteral);
    void vAddCodeLabel(eEO eCode, Stz* szLabelNm);
    void vAddCodeLabel(eEO eCode, Token* pTkn);
    void vAddCodeString(eEO eCode, Stz* szString);
    void vAddCodeTme(eEO eCode, Stz* szLiteral);
    void vAddCodeTsp(eEO eCode, Stz* szLiteral);
    i8   iAddLabel(Stz* szLabelNm, bool bDefined);
    void vCorrectLabel(i8 i_Label);
    i8   iDecodeLabel(Stz* szLabelNm);
    void vDefineLabel(Stz* szLabelNm);
    void vDeleteLabel(Stz* szLabelNm);
    void vDiaryReport(void);
    void vPrintLabels(void);
    Stz* szFindCode(i8 i_Code);


    // last
    void vDumpCode(void);
    i8   iCompute(i8 nOuterArgs, i8 iArg0);

  private:
    Footer* aFooter(void);
  };
  
  LogicUnit* _pLogicUnit(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
