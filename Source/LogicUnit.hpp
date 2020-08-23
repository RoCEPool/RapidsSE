/*============================================================================*/
/* LogicUnit.hpp -- Logic Unit                                                */
/*============================================================================*/
#pragma once

#include "InflateSet.h"

namespace Expressions
{
  static const i8 C_LOGIC_DEV   = 16;
  static const i8 C_LOGIC_CODE  = C_4KB;
  static const i8 C_LOGIC_COND  = 8;
  static const i8 C_LOGIC_LABEL = 64;
  static const i8 C_LOCALS      = 8;


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

  //----------------------------------------------------------------------------

  class LogicUnit : public Object
  {
  friend class Tasks::Task;
  
  public:
    Task* m_pTask; // which task
  
    eLH              m_eDdlLocked; // this task has the ddl lock?
    i8               m_i_NextRnd;  // which next repeatable random nummber?
    Node*            m_pSendNode;  // node to send to
  
    REG<Word>        m_xpHandler;  // which handler is to be/was called
  
    i8               m_iDebugFlag; // general purpose debug flag
  
    REF<Job>         m_xpJobRef;   // job reference
    REF<Node>        m_xpNodeRef;  // node reference

    REG<Dir>         m_xpDir0;     // dir0[]
    REG<Dir>         m_xpDir1;     // dir1[]

    REG<File>        m_xpFile0;    // file0[]
    REG<File>        m_xpFile1;    // file1[]

    REG<Dynamo>      m_xpDynamo;   // dynamo pipleine compiler
  
    REG<AggregateRsb> m_xpAggregateRsb;
    REG<CrwHash>     m_xpCrwHash;
    REG<Index>       m_xpIndex;
    REG<IndexDef>    m_xpIndexDef;
    REG<IndexStream> m_xpIndexStream;
    REG<IndexTree>   m_xpIndexTree;
    REG<Operand>     m_xpInfoSet;
    REG<Table>       m_xpInfoSetTable;
    REG<JoinRsb>     m_xpJoinRsb;
    REG<Loader>      m_xpLoader;
    REG<LogicCompiler> m_xpLogicCompiler;
    REG<Operand>     m_xpPipein;    // Pipein
    REG<Planner>     m_xpPlanner;
    REG<ProjectRsb>  m_xpProjectRsb;
    REG<PtnStream>   m_xpPtnStream;
    REG<Row>         m_xpRow;
    REG<RowOnce>     m_xpRowOnce;
    REG<StreamDef>   m_xpStreamDef;     // maybe rename this to stream def in ?
    REG<StreamDef>   m_xpStreamDefOut;
    REG<Operand>     m_xpStreamSet;
    REG<Pot>         m_xpPot;
    REG<Table>       m_xpStreamSetTable;
    REG<Operand>     m_xpSubset;
    REG<Table>       m_xpTable;
    REG<TablePtn>    m_xpTablePtn;
    REG<ToHshCrw>    m_xpToHshCrw;
    REG<ToNstRsb>    m_xpToNstRsb;
    REG<ToPtnRsb>    m_xpToPtnRsb;
    REG<ToRsb>       m_xpToRsb;
    REG<Tx>          m_xpTx;
    REG<Xmits>       m_xpXmits;     // consume xmits
    
    REG<AggTimer>    m_xpAggTimer;  // aggregate timer
    REG<Now>         m_xpNow;       // now
    REG<Timer>       m_xpTimer;     // timer
  
    REG<Op>          m_xpOp;        // operator
    REG<Operand>     m_xpOperandA;  // operand A
    REG<Operand>     m_xpOperandB;  // operand B
    REG<Operand>     m_xpOperandC;  // operand C
    REG<Operand>     m_xpOperandOoO; // operand OoO
    
    i8               m_iCounter0;   // counter
    i8               m_iCounter1;   // counter
    i8               m_iCounter2;   // counter
      
    Ent*             m_aLowerEntry;
    Ent*             m_aUpperEntry;
    EntBuffer*       m_pEntBufferOuter;
    
    REG<Object>      m_xpRegister0; // general purpose reg 0[]
    REG<Object>      m_xpRegister1; // general purpose reg 1[]
    REG<Object>      m_xpRegister2; // general purpose reg 2[]
    REG<Object>      m_xpRegister3; // general purpose reg 3[]
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // inner loop
  
    i8 (*m_afDynFun)(void* aLU, i8 nOuterArgs, i8 iOuterArg0);
  
    RluCommon  m_xRluCommon ;
  
    RluDiary*  m_pRluDiary;
  
    void*      m_aaDev[C_LOGIC_DEV]; // device array
  
    Timer      m_oSince;
  
    Timer      m_oTimer;
    AggTimer   m_oAggTimer;

    
    StreamPix  m_oStreamPix;    // condensed stream def
  
    EntWrk     m_oEntWrk;

    DevRSB*    m_pDevDstRsb;
    DevRSB*    m_pDevSrcRsb;
    DevIXT*    m_pDevSrcIxt;
    DevRSB*    m_pDevAltRsb;
    DevIXT*    m_pDevAltIxt;
    DevIXT*    m_pDevTmpIxt;
  
    LogicTimer* m_pLogicTimer;

    i8     m_i_Code;
    eEO    m_eaCode[C_LOGIC_CODE];    // { EO_code [<bytes>] }

    i8        m_nLabels;
    JumpLabel m_xaJumpLabel[C_LOGIC_LABEL];

    bool m_baCondInd[C_LOGIC_COND];

    i8 m_iNextCond;
  
    bool m_bSuperFrag;
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    LogicUnit(Task* pTask);
   ~LogicUnit(void) noexcept(false);
  
    void vContentDump(void);
    i8   iTransform(i8 nArgs, i8 iArg0);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    void vCC(i8 i_Cond);
    eEO  eCode(i8 i_Code);
    i8   nCodes(void);
    eEO* aCodeArg(i8 i_Code);
    i8   iCodeArg(i8 i_Code);
    i8   iDynFun(i8 nOuterArgs, i8 iOuterArg0);
    void vEnter(void);
    void vExit(void);
    i8   iFindLabel(i8 i_Code);
    i8   iFindLabel(Stz* szLabelNm);
    void vInit(void);
    i8   iLabelCode(i8 i_Label);
    bool bLabelIsDst(i8 i_Label);
    bool bLabelIsSbr(i8 i_Label);
    i8   nLabels(void);
    Stz* szLabelNm(i8 i_Label);
    i8   iQueryFragment(void);
    void vSetCC(i8 i_Cond, bool bState);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
    EntWrk* pEntWrk(void);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
    // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

    void vGenCode(i8 iTestNo);
  
    // TPCH Queries
    void vGenerateQuery100(void);
    void vGenerateQuery103(void);

    void vGenerateQuery300(void);
    void vGenerateQuery301(void);
    void vGenerateQuery302(void);

    void vGenerateQuery400(void);
    void vGenerateQuery401(void);

    void vGenerateQuery600(void);
    void vGenerateQuery601(void);

    void vGenerate1700(void);
    void vGenerate1701(void);
    void vGenerate1702(void);

    void vGenerate100000(void);
    void vGenerate100001(void);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
    #include "RLU_interface.h"

  private:
    void vAddCode(eEO eCode);
    void vAddCodeArg(eEO eCode, i8 iArg);
    void vAddCodeArg(eEO eCode, Token* pTkn);
    void vAddCodeArg(eEO eCode, Stz* szLiteral);
    void vAddCodeLabel(eEO eCode, Stz* szLabelNm);
    void vAddCodeLabel(eEO eCode, Token* pTkn);
    void vAddCodeString(eEO eCode, Stz* szString);
    i8   iAddLabel(Stz* szLabelNm, bool bDefined);
    void vCorrectLabel(i8 i_Label);
    i8   iDecodeLabel(Stz* szLabelNm);
    void vDefineLabel(Stz* szLabelNm);
    void vDiaryReport(void);
    void vPrintLabels(void);
    Stz* szFindCode(i8 i_Code);


    // last
    void vDumpCode(void);
    i8   iCompute(i8 nOuterArgs, i8 iArg0);

    i8   iCompute2(i8 nOuterArgs, i8 iArg0);
    i8   iCompute3(i8 nOuterArgs, i8 iArg0);


  private:
    Footer* aFooter(void);
  };
  
  LogicUnit* _pLogicUnit(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
