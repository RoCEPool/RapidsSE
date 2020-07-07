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
  
    bool   m_bCompilingKey;

    i8     m_i_Code;
    eEO    m_eaCode[C_CALC_CODE];    // { EO_code [<bytes>] }
  
    i8     m_i_CodeKey;
    eEO    m_eaCodeKey[C_CALC_CODE]; // { EO_code [<bytes>] }
  
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
  
    bool bCompile(TokenVec* pTknVec);
    bool bCompileKey(TokenVec* pTknVec);

    i8   iCalc(void);
    i8   iFilter(Rsb* aSrcRsb, RowSlab* pSrcRowSlab);
    i8   iProject(RowSlab* pDstRowSlab, Rsb* aSrcRsb, RowSlab* pSrcRowSlab);
    i8   iJoin(RowSlab* pDstRowSlab,
               Rsb* aSrcRsb_left,  RowSlab* pSrcRowSlab_left,
               Rsb* aSrcRsb_right, RowSlab* pSrcRowSlab_right);

  private:
    void vAddCode(eEO eCode);
    bool bCompile(TokenVec* pTknVec, bool bKeyCode);
    i8   iCompute(void);
  

    Footer* aFooter(void);
  };
  
  Calculator* _pCalculator(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
