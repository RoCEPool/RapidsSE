/*============================================================================*/
/* LogicUnit.hpp -- Logic Unit                                                */
/*============================================================================*/
#pragma once

namespace Expressions
{
  static const i8 C_CALC_CODE  = C_4KB;
  static const i8 C_CALC_STACK = C_1KB;
  static const i8 C_CALC_LABEL = 64;

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
  
  
    // EXPERIMENT
    //Calculator       m_oCalculator;
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // ..from Calculator

    bool       m_bPixInited;
    StreamPix  m_oStreamPix;    // condensed stream def

    Pot*     m_pPot;
    Rsb*     m_aDstRsb;
    RowSlab* m_pDstRowSlab;

    i8       m_i_SrcRow;
    i8       m_nSrcRows;
    Rsb*     m_aSrcRsb;
    RowSlab* m_pSrcRowSlab;

    i8       m_i_AltRow;
    i8       m_nAltRows;
    Rsb*     m_aAltRsb;
    RowSlab* m_pAltRowSlab;
    Rsb*     m_aPayRsb;

    Ent*       m_pEnt;
    EntWrk     m_oEntWrk;
//    EntBuffer* m_pEntBuffer;
    IndexTree* m_pIndexTree;

    i8     m_i_Code;
    eEO    m_eaCode[C_CALC_CODE];    // { EO_code [<bytes>] }

    i8     m_iaDataStack[C_CALC_STACK];
    eEO*   m_aaReturnStack[C_CALC_STACK];

    i8        m_nLabels;
    JumpLabel m_xaJumpLabel[C_CALC_LABEL];

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
  
    //! TEMP: TO GO
    void vGenerateQuery6(void);
    
    void vGenerateAggCpy(void);
    void vGenerateAggGby(void);
    void vGenerateSort(void);

    i8   iAggregate(RowSlab* pDstRowSlab, Rsb* aSrcRsb, RowSlab* pSrcRowSlab);
    i8   iAggregateGby(Pot* pPot, IndexTree* pIndexTree, RowSlab* pDstRowSlab,
                       Rsb* aSrcRsb, RowSlab* pSrcRowSlab);
    bool bCompile(void);
    bool bCompile(TokenVec* pTknVec);

    i8   iCalc(void);
    i8   iFilter(RowSlab* pSrcRowSlab);
    i8   iProject(RowSlab* pSrcRowSlab);
    i8   iJoin(RowSlab* pDstRowSlab,
               Rsb* aSrcRsb_left,  RowSlab* pSrcRowSlab_left,
               Rsb* aSrcRsb_right, RowSlab* pSrcRowSlab_right);
  
    Stz* szSource(void);
    void vSource(Stz* szSource);


  private:
    void vAddCode(eEO eCode);
    void vAddCodeArg(eEO eCode, i8 iArg);
    void vAddCodeArg(eEO eCode, Token* pTkn);
    void vAddCodeLabel(eEO eCode, Stz* szLabelNm);
    void vAddCodeLabel(eEO eCode, Token* pTkn);
    i8   iAddLabel(Stz* szLabelNm, bool bDefined);
    void vCorrectLabel(i8 i_Label);
    i8   iDecodeLabel(Stz* szLabelNm);
    void vDefineLabel(Stz* szLabelNm);
    i8   iFindLabel(Stz* szLabelNm);
    void vPrintLabels(void);

    // last
    bool bCompileCode(TokenVec* pTknVec);
    void vDumpCode1(void);
    void vDumpCode2(void);
    void vDumpCode(i8& i_Code, eEO eCode);
    void vDumpCode(eEO eCode, i8& i_Code);
    i8   iCompute(void);


  private:
    Footer* aFooter(void);
  };
  
  LogicUnit* _pLogicUnit(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
