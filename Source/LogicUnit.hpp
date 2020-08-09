/*============================================================================*/
/* LogicUnit.hpp -- Logic Unit                                                */
/*============================================================================*/
#pragma once

namespace Expressions
{
  static const i8 C_LOGIC_DEV   = 16;
  static const i8 C_LOGIC_CODE  = C_4KB;
  static const i8 C_LOGIC_LABEL = 64;

  // customer columns
  static const i8 C_CUSTKEY       = 0;
  static const i8 C_NAME          = 1;
  static const i8 C_ADDRESS       = 2;
  static const i8 C_NATIONKEY     = 3;
  static const i8 C_PHONE         = 4;
  static const i8 C_ACCTBAL       = 5;
  static const i8 C_MKTSEGMENT    = 6;
  static const i8 C_COMMENT       = 7;

  // lineitem columns
  static const i8 L_ORDERKEY      = 0;
  static const i8 L_PARTKEY       = 1;
  static const i8 L_SUPPKEY       = 2;
  static const i8 L_LINENUMBER    = 3;
  static const i8 L_QUANTITY      = 4;
  static const i8 L_EXTENDEDPRICE = 5;
  static const i8 L_DISCOUNT      = 6;
  static const i8 L_TAX           = 7;
  static const i8 L_RETURNFLAG    = 8;
  static const i8 L_LINESTATUS    = 9;
  static const i8 L_SHIPDATE      = 10;
  static const i8 L_COMMITDATE    = 11;
  static const i8 L_RECEIPTDATE   = 12;
  static const i8 L_SHIPINSTRUCT  = 13;
  static const i8 L_SHIPMODE      = 14;
  static const i8 L_COMMENT       = 15;
  
  // orders columns
  static const i8 O_ORDERKEY      = 0;
  static const i8 O_CUSTKEY       = 1;
  static const i8 O_ORDERSTATUS   = 2;
  static const i8 O_TOTALPRICE    = 3;
  static const i8 O_ORDERDATE     = 4;
  static const i8 O_ORDERPRIORITY = 5;
  static const i8 O_CLERK         = 6;
  static const i8 O_SHIPPRIORITY  = 7;
  static const i8 O_COMMENT       = 8;


  // query1 100 output
  static const i8 Q100_RETURNFLAG     = 0;  // L_RETURNFLAG
  static const i8 Q100_LINESTATUS     = 1;  // L_LINESTATUS
  static const i8 Q100_SUM_QTY        = 2;  // SUM(L_QUANTITY)
  static const i8 Q100_SUM_BASE_PRICE = 3;  // SUM(L_EXTENDEDPRICE)
  static const i8 Q100_SUM_DISC_PRICE = 4;  // SUM(L_EXTENDEDPRICE*(1-L_DISCOUNT))
  static const i8 Q100_SUM_CHARGE     = 5;  // SUM(L_EXTENDEDPRICE*(1-L_DISCOUNT)*(1+L_TAX))
  static const i8 Q100_COUNT_QTY      = 6;  // COUNT(L_QUANTITY)
  static const i8 Q100_COUNT_PRICE    = 7;  // COUNT(L_EXTENDEDPRICE)
  static const i8 Q100_SUM_DISC       = 8;  // SUM(L_DISCOUNT)
  static const i8 Q100_COUNT_DISC     = 9;  // COUNT(L_DISCOUNT)
  static const i8 Q100_COUNT_ORDER    = 10; // COUNT(*)
  static const i8 Q100_COLUMNS        = 11; // TOTAL COLUMNS
  
  // query1 101 output
  static const i8 Q101_RETURNFLAG     = 0;  // L_RETURNFLAG
  static const i8 Q101_LINESTATUS     = 1;  // L_LINESTATUS
  static const i8 Q101_SUM_QTY        = 2;  // SUM(L_QUANTITY)
  static const i8 Q101_SUM_BASE_PRICE = 3;  // SUM(L_EXTENDEDPRICE)
  static const i8 Q101_SUM_DISC_PRICE = 4;  // SUM(L_EXTENDEDPRICE*(1-L_DISCOUNT))
  static const i8 Q101_SUM_CHARGE     = 5;  // SUM(L_EXTENDEDPRICE*(1-L_DISCOUNT)*(1+L_TAX))
  static const i8 Q101_COUNT_QTY      = 6;  // COUNT(L_QUANTITY)
  static const i8 Q101_COUNT_PRICE    = 7;  // COUNT(L_EXTENDEDPRICE)
  static const i8 Q101_SUM_DISC       = 8;  // SUM(L_DISCOUNT)
  static const i8 Q101_COUNT_DISC     = 9;  // COUNT(L_DISCOUNT)
  static const i8 Q101_COUNT_ORDER    = 10; // COUNT(*)
  static const i8 Q101_COLUMNS        = 11; // TOTAL COLUMNS
  
  // query1 104 output
  static const i8 Q104_RETURNFLAG     = 0;  // L_RETURNFLAG
  static const i8 Q104_LINESTATUS     = 1;  // L_LINESTATUS
  static const i8 Q104_SUM_QTY        = 2;  // SUM(L_QUANTITY)
  static const i8 Q104_SUM_BASE_PRICE = 3;  // SUM(L_EXTENDEDPRICE)
  static const i8 Q104_SUM_DISC_PRICE = 4;  // SUM(L_EXTENDEDPRICE*(1-L_DISCOUNT))
  static const i8 Q104_SUM_CHARGE     = 5;  // SUM(L_EXTENDEDPRICE*(1-L_DISCOUNT)*(1+L_TAX))
  static const i8 Q104_AVG_QTY        = 6;  // AVG(L_QUANTITY)
  static const i8 Q104_AVG_PRICE      = 7;  // AVG(L_EXTENDEDPRICE)
  static const i8 Q104_AVG_DISC       = 8;  // AVG(L_DISCOUNT)
  static const i8 Q104_COUNT_ORDER    = 9;  // COUNT(*)
  
  // q300 output
  static const i8 Q300_ORDERKEY     = 0;
  static const i8 Q300_ORDERDATE    = 1;
  static const i8 Q300_SHIPPRIORITY = 2;

  // q301 output
  static const i8 Q301_ORDERKEY     = 0;
  static const i8 Q301_REVENUE      = 1;
  static const i8 Q301_ORDERDATE    = 2;
  static const i8 Q301_SHIPPRIORITY = 3;

  // q302 output
  static const i8 Q302_ORDERKEY     = 0;
  static const i8 Q302_REVENUE      = 1;
  static const i8 Q302_ORDERDATE    = 2;
  static const i8 Q302_SHIPPRIORITY = 3;

  // q400 output
  static const i8 Q400_ORDERPRIORITY = 0;
  static const i8 Q400_ORDERCOUNT    = 1;

  // q401 output
  static const i8 Q401_ORDERPRIORITY = 0;
  static const i8 Q401_ORDERCOUNT    = 1;

  // query6 600 output
  static const i8 Q600_REVENUE    = 0;  // SUM(L_EXTENDEDPRICE*L_DISCOUNT)
  static const i8 Q600_COLUMNS    = 1;  // TOTAL COLUMNS
  
  // query6 601 output
  static const i8 Q601_REVENUE    = 0;  // SUM(L_EXTENDEDPRICE*L_DISCOUNT)
  static const i8 Q601_COLUMNS    = 1;  // TOTAL COLUMNS
  
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
    bool             m_bCallAgain; // call same handler again
  
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
  
    RluCommon  m_xRluCommon ;
  
    void*      m_aaDev[C_LOGIC_DEV]; // device array
  
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

    i8     m_i_Code;
    eEO    m_eaCode[C_LOGIC_CODE];    // { EO_code [<bytes>] }

    i8        m_nLabels;
    JumpLabel m_xaJumpLabel[C_LOGIC_LABEL];

  
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

    eEO  eCode(i8 i_Code);
    i8   nCodes(void);
    i8   iCodeArg(i8 i_Code);
    i8   iFindLabel(i8 i_Code);
    i8   iFindLabel(Stz* szLabelNm);
    i8   iLabelCode(i8 i_Label);
    bool bLabelIsDst(i8 i_Label);
    bool bLabelIsSbr(i8 i_Label);
    i8   nLabels(void);
    Stz* szLabelNm(i8 i_Label);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
    EntWrk* pEntWrk(void);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
    // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

    void vGenCode(i8 iTestNo);
  
    // TPCH Queries
    void vGenerateQuery101(void);
    void vGenerateQuery103(void);
    void vGenerateQuery104(void);
    void vGenerateQuery110(void);
    void vGenerateQuery112(void);

    void vGenerateQuery300(void);
    void vGenerateQuery301(void);
    void vGenerateQuery302(void);
    void vGenerateQuery302b(void);

    void vGenerateQuery400(void);
    void vGenerateQuery401(void);

    void vGenerateQuery600(void);
    void vGenerateQuery601(void);
    void vGenerateQuery666(void);

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
    Stz* szFindCode(i8 i_Code);
     void vPrintLabels(void);

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
