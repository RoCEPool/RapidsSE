/*============================================================================*/
/* LogicUnit.hpp -- Logic Unit                                                */
/*============================================================================*/
#pragma once

namespace Expressions
{
  static const i8 C_LOGIC_CODE  = C_4KB;
  static const i8 C_LOGIC_STACK = C_1KB;
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


  // query1 output
  static const i8 Q1_RETURNFLAG     = 0;  // L_RETURNFLAG
  static const i8 Q1_LINESTATUS     = 1;  // L_LINESTATUS
  static const i8 Q1_SUM_QTY        = 2;  // SUM(L_QUANTITY)
  static const i8 Q1_SUM_BASE_PRICE = 3;  // SUM(L_EXTENDEDPRICE)
  static const i8 Q1_SUM_DISC_PRICE = 4;  // SUM(L_EXTENDEDPRICE*(1-L_DISCOUNT))
  static const i8 Q1_SUM_CHARGE     = 5;  // SUM(L_EXTENDEDPRICE*(1-L_DISCOUNT)*(1+L_TAX))
  static const i8 Q1_AVG_QTY        = 6;  // AVG(L_QUANTITY)
  static const i8 Q1_AVG_PRICE      = 7;  // AVG(L_EXTENDEDPRICE)
  static const i8 Q1_AVG_DISC       = 8;  // AVG(L_DISCOUNT)
  static const i8 Q1_COUNT_ORDER    = 9;  // COUNT(*)
  
  // q300 output
  static const i8 Q300_ORDERKEY     = 0;
  static const i8 Q300_ORDERDATE    = 1;
  static const i8 Q300_SHIPPRIORITY = 2;

  // q301 output
  static const i8 Q301_ORDERKEY     = 0;
  static const i8 Q301_REVENUE      = 1;
  static const i8 Q301_ORDERDATE    = 2;
  static const i8 Q301_SHIPPRIORITY = 3;



  struct JumpLabel
  {
    s32  m_sLabel;
    bool m_bDefined;
    union {
      i8 m_i_Code;
      i8 m_i_Chain;
    };
  };

  struct EoArg
  {
    eEA m_eArgument;
    i8  m_nArgBytes;
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
    EntBuffer*       m_pEntBuffer;
    
    REG<Object>      m_xpRegister0; // general purpose reg 0[]
    REG<Object>      m_xpRegister1; // general purpose reg 1[]
    REG<Object>      m_xpRegister2; // general purpose reg 2[]
    REG<Object>      m_xpRegister3; // general purpose reg 3[]
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // inner loop

    i8         m_iSql;
    
    bool       m_bPixInited;
    StreamPix  m_oStreamPix;    // condensed stream def

    Rsb*     m_aDstRsb;
    RowSlab* m_pDstRowSlab;

    i8         m_i_SrcRow;
    i8         m_nSrcRows;
    Rsb*       m_aSrcRsb;
    RowSlab*   m_pSrcRowSlab;

    Ent*       m_aSrcEnt;
    IndexTree* m_pSrcIdxTree;
    EntBuffer* m_pSrcEntBuffer;
    Crw*       m_aSrcCrw;
    StreamDef* m_pSrcStreamDef;
  
    i8         m_i_AltRow;
    i8         m_nAltRows;
    Rsb*       m_aAltRsb;
    RowSlab*   m_pAltRowSlab;
    Crw*       m_aAltCrw;
    StreamDef* m_pAltStreamDef;

    Ent*       m_aAltEnt;
    IndexTree* m_pAltIdxTree;
    EntBuffer* m_pAltEntBuffer;
    i8*        m_aPayload;

    Ent*       m_pEnt;
    EntWrk     m_oEntWrk;
//    EntBuffer* m_pEntBuffer;
    IndexTree* m_pIndexTree;

    i8     m_i_Code;
    eEO    m_eaCode[C_LOGIC_CODE];    // { EO_code [<bytes>] }

    i8     m_iaDataStack[C_LOGIC_STACK];
    eEO*   m_aaReturnStack[C_LOGIC_STACK];

    i8        m_nLabels;
    JumpLabel m_xaJumpLabel[C_LOGIC_LABEL];

    s2K     m_sSource;

  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    LogicUnit(Task* pTask);
   ~LogicUnit(void) noexcept(false);
  
    void vContentDump(void);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // ..from Calculator

    void vGenCode(i8 iTestNo);
  
    // TPCH Queries
    void vGenerateQuery100(void);
    void vGenerateQuery101(void);
    void vGenerateQuery102(void);

    void vGenerateQuery300(void);
    void vGenerateQuery301(void);

    void vGenerateQuery400(void);
  
    void vGenerateQuery600(void);
    void vGenerateQuery601(void);
    
    //! TEMP: TO GO
    void vGenerateAgg13000(void);
    void vGenerateAggGby14000(void);
    void vGenerateAggGby14001(void);
    void vGenerateAggGby14002(void);
    void vGenerateSort12000(void);

    i8   iAggregate(RowSlab* pSrcRowSlab, StreamDef* pStreamDefOut);
    i8   iAggregateGby(Pot* pPot, IndexTree* pIndexTree, RowSlab* pDstRowSlab,
                       Rsb* aSrcRsb, RowSlab* pSrcRowSlab);
    bool bCompile(void);
    bool bCompile(TokenVec* pTknVec);

    i8   iTransform(void);
  
    Stz* szSource(void);
    void vSource(Stz* szSource);


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
    i8   iFindLabel(Stz* szLabelNm);
    void vPrintLabels(void);

    // last
    bool bCompileCode(TokenVec* pTknVec);
    void vDumpCode(void);
    i8   iCompute(void);


  private:
    Footer* aFooter(void);
  };
  
  LogicUnit* _pLogicUnit(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
