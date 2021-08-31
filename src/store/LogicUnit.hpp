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


#include "EntWrk.hpp"

#include "RluCommon.h"
#include "RluDiary.hpp"
#include "DevIXT.hpp"
#include "DevROW.hpp"
#include "LogicTimer.hpp"
#include "LogicCompiler.hpp"
#include "ExprEval.hpp"
#include "Range.hpp"
#include "ShardDef.hpp"
#include "MetaGet.hpp"
#include "StreamDefRht.hpp"

#include "InflateSet.h"
#include "InfoSet.hpp"
#include "PipelineSpec.hpp"
#include "NamedBuffer.hpp"

#include "eLH.h"
#include "eXK.h"

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
  if (*aiDstk == C_DATUM_NULL)    {                                   \
    *(i8*)__azDst = C_DATUM_NULL;  \
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
    
    Job*  m_pJob;       // which job
    Op*   m_pOp;        // which op
    
    eLH   m_eDdlLocked; // this task has the ddl lock?
    i8    m_i_NextRnd;  // which next repeatable random nummber?
    i8    m_iDebugFlag; // general purpose debug flag

    REF<Job>            m_xpJobRef;   // job reference
    
    REG<Object>*        m_paReg[C_MAX_REGS];
    i8 m_nRegs;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    REG<AggTimer>       m_xpAggTimer;  // aggregate timer
    REG<Bucket>         m_xpBucket;
    REG<CrwBuffer>      m_xpCrwBuffer;
    REG<CrwToRow>       m_xpCrwToRow;
    REG<CsvBuffer>      m_xpCsvBuffer;
    REG<DataBuffer>     m_xpDataBuffer;
    REG<Dir>            m_xpDir0;     // dir0[]
    REG<Dir>            m_xpDir1;     // dir1[]
    REG<ExprEval>       m_xpExprEval; // expr eval
    REG<File>           m_xpFile0;    // file0[]
    REG<File>           m_xpFile1;    // file1[]
    REG<Grid>           m_xpGrid;
    REG<Word>           m_xpHandler;  // which handler is to be/was called
    REG<Index>          m_xpIndex;
    REG<IndexDef>       m_xpIndexDef;
    REG<IndexStream>    m_xpIndexStream;
    REG<InfoSet>        m_xpInfoSet;
    REG<Table>          m_xpInfoSetTable;
    REG<RowSingleList>  m_xpList;
    REG<LogicCompiler>  m_xpLogicCompiler;
    REG<Operand>        m_xpMarker;
    REG<RowBuffer>      m_xpMemoryHog; // memoryHog
    REG<MetaGet>        m_xpMeta;
    REG<NamedBuffer>    m_xpNamedBuffer;
    REG<Node>           m_xpNode;      // node reference
    REG<Node_I>         m_xpNode_I;    // node iterator
    REG<Now>            m_xpNow;       // now
    REG<Object>         m_xpRecvdObject;
    REG<Op>             m_xpOp;        // operator
    REG<Operand>        m_xpOperandA;  // operand A
    REG<Operand>        m_xpOperandOoO; // operand OoO
    REG<PathIdentifier> m_xpPathIdentifier;
    REG<Parcel>         m_xpParcel;
    REG<Operand>        m_xpPipein;    // Pipein
    REG<Pipeline>       m_xpPipeline;  // pipeline spec generate by dynamo
    REG<PipelineSpec>   m_xpPipelineSpec; // pipeline spec generate by dynamo
    REG<Operand>        m_xpPlanOperand;
    REG<PtnStream>      m_xpPtnStream;
    REG<Row>            m_xpInfoSetRow;
    REG<RowBuffer>      m_xpRowBuffer;
    REG<RowOnce>        m_xpRowOnce;
    REG<Node>           m_xpSendNode;  // node to send to
    REG<Sends>          m_xpSends;     // consume Sends
    REG<ShardDef>       m_xpShardDef;
    REG<Socket>         m_xpSocket;
    REG<StreamDef>      m_xpStreamDefLft;
    REG<StreamDef>      m_xpStreamDefOut;
    REG<StreamDefRht>   m_xpStreamDefRht;
    REG<Operand>        m_xpStreamSet;
    REG<Table>          m_xpStreamSetTable;
    REG<Operand>        m_xpSubset;
    REG<Table>          m_xpTable;
    REG<TablePtn>       m_xpTablePtn;
    REG<Timer>          m_xpTimer;     // timer
    REG<ToBktCrw>       m_xpToBktCrw;
    REG<ToBktRow>       m_xpToBktRow;
    REG<ToNstRow>       m_xpToNstRow;
    REG<ToPtnCrw>       m_xpToPtnCrw;
    REG<ToPtnRow>       m_xpToPtnRow;
    REG<ToRow>          m_xpToRow;
    REG<Tx>             m_xpTx;

    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    REG<Operand>        m_xpErrorMarker;  // error marker


    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    bool                m_bCompilerToExit;
    bool                m_bIsLftCrwBuffer; // left buffer is a CRW buffer (joins)

    i8                  m_iCounter0;   // counter
    i8                  m_iCounter1;   // counter
    i8                  m_iCounter2;   // counter
        
    s256                m_sStringA;    // string
        
    Ent*                m_aLowerEntry;
    Ent*                m_aUpperEntry;
    EntBuffer*          m_pEntBufferOuter;
        
    Atomic              m_ixRowsProcessed; // rows processed
    Atomic              m_ixRowsInvalid;   // invalid rows detected
    Atomic              m_ixRowsSkipped;   // rows skipped
    eEC                 m_eErrorCode;      // error code
    
    eZK                 m_ePipeInOperand;

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

    DevROW*    m_pDevDstRow;
    DevROW*    m_pDevLftRow;
    DevROW*    m_pDevRhtRow;
    DevROW*    m_pDevAltRow;

    DevIXT*    m_pDevAltIxt;
    DevIXT*    m_pDevLftIxt;
    DevIXT*    m_pDevRhtIxt;
    DevIXT*    m_pDevTmpIxt;
  
    LogicTimer* m_pLogicTimer;

    i8     m_i_Code;
    char    m_eaCode[C_LOGIC_CODE];    // { EO_code [<bytes>] }

    i8        m_nLabels;
    JumpLabel m_xaJumpLabel[C_LOGIC_LABEL];

    bool m_baCondInd[C_LOGIC_COND];

    i8 m_iNextCond;
  
    bool m_bSuperFrag;
    
    
    eXK  m_eSendKind;      // send kind

  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    LogicUnit(void);
   ~LogicUnit(void) noexcept(false);
  
    void vContentDump(void);
    i8   iTransform(i8 nArgs, i8 iArg0);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    void vAddReg(REG<Object>* pReg);
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
    i8   iDynFun(i8 nOuterArgs, i8 iOuterArg0);
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

    eEC eGenerateAggregate(ExprEval* pExprEval);
    eEC eGenerateFilter(ExprEval* pExprEval);
    eEC eGenerateGroupBy(ExprEval* pExprEval);
    eEC eGenerateHash(ExprEval* pExprEval);
    eEC eGenerateIndexJoin(ExprEval* pExprEval);
    eEC eGenerateIndexScan(ExprEval* pExprEval);
    eEC eGenerateLimit(i8 iLimit);
    eEC eGenerateLoopJoin(ExprEval* pExprEval);
    eEC eGenerateProjection(ExprEval* pExprEva);

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
