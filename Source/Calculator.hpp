/*============================================================================*/
/* Calculator.hpp -- Calculator                                               */
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

  class Calculator : public Object
  {
  private:

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
    EntBuffer* m_pEntBuffer;
    IndexTree* m_pIndexTree;
  
    i8     m_i_Code;
    eEO    m_eaCode[C_CALC_CODE];    // { EO_code [<bytes>] }
    
    i8     m_iaDataStack[C_CALC_STACK];
    eEO*   m_aaReturnStack[C_CALC_STACK];

    i8        m_nLabels;
    JumpLabel m_xaJumpLabel[C_CALC_LABEL];
  
  private:
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Calculator(void);
   ~Calculator(void) noexcept(false);
  
    //! TEMP: TO GO
    void vGenerateAggGby(void);
    void vGenerateSort(void);

    i8   iAggregate(RowSlab* pDstRowSlab, Rsb* aSrcRsb, RowSlab* pSrcRowSlab);
    i8   iAggregateGby(IndexTree* pIndexTree, RowSlab* pDstRowSlab,
                       Rsb* aSrcRsb, RowSlab* pSrcRowSlab);
    bool bCompile(TokenVec* pTknVec);

    i8   iCalc(void);
    i8   iFilter(Rsb* aSrcRsb, RowSlab* pSrcRowSlab);
    i8   iProject(Pot* pPot, RowSlab* pDstRowSlab,
                  Rsb* aSrcRsb, RowSlab* pSrcRowSlab);
    i8   iJoin(RowSlab* pDstRowSlab,
               Rsb* aSrcRsb_left,  RowSlab* pSrcRowSlab_left,
               Rsb* aSrcRsb_right, RowSlab* pSrcRowSlab_right);

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

    Footer* aFooter(void);
  };
  
  Calculator* _pCalculator(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
