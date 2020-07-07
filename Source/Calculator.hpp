/*============================================================================*/
/* Calculator.hpp -- Calculator                                               */
/*============================================================================*/
#pragma once

namespace Expressions
{
  static const i8 C_CALC_CODE  = C_4KB;
  static const i8 C_CALC_STACK = C_4KB;
  static const i8 C_CALC_LABEL = 16;

  struct JumpLabel
  {
    s32  m_sLabel;
    bool m_bKnown;
    i8   m_i_Code;
  };

  //----------------------------------------------------------------------------

  class Calculator : public Object
  {
  private:

    RowSlab* m_pDstRowSlab;

    Rsb*     m_aSrcRsb_left;
    RowSlab* m_pSrcRowSlab_left;
 
    Rsb*     m_aSrcRsb_right;
    RowSlab* m_pSrcRowSlab_right;
   
    Ent*       m_pEnt;
    EntWrk     m_oEntWrk;
    IndexTree* m_pIndexTree;
  

    i8     m_i_Code;
    eEO    m_eaCode[C_CALC_CODE];    // { EO_code [<bytes>] }
    
    i8     m_iaStack[C_CALC_STACK];

    i8     m_nLabels;
    JumpLabel m_xaJumpLabel[C_CALC_LABEL];
  
  private:
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Calculator(void);
   ~Calculator(void) noexcept(false);
  
    i8   iAggregate(byte* aPayLoad, Rsb* aSrcRsb, RowSlab* pSrcRowSlab);
    bool bCompile(TokenVec* pTknVec);

    i8   iCalc(void);
    i8   iFilter(Rsb* aSrcRsb, RowSlab* pSrcRowSlab);
    i8   iProject(RowSlab* pDstRowSlab, Rsb* aSrcRsb, RowSlab* pSrcRowSlab);
    i8   iJoin(RowSlab* pDstRowSlab,
               Rsb* aSrcRsb_left,  RowSlab* pSrcRowSlab_left,
               Rsb* aSrcRsb_right, RowSlab* pSrcRowSlab_right);

  private:
    void vAddCode(eEO eCode);
    void vAddCode(eEO eCode, Stz* szLabelNm);
    i8   iAddLabel(Stz* szLabelNm, bool bKnown);
    void vCorrectLabel(i8 i_Label);
    i8   iDecodeLabel(Stz* szLabelNm);
    void vDefineLabel(Stz* szLabelNm);
    i8   iFindLabel(Stz* szLabelNm);
    void vPrintLabels(void);

    // last
    bool bCompileCode(TokenVec* pTknVec);
    i8   iCompute(void);

    Footer* aFooter(void);
  };
  
  Calculator* _pCalculator(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
