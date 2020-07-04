/*============================================================================*/
/* Calculator.hpp -- Calculator                                               */
/*============================================================================*/
#pragma once

namespace Expressions
{
  static const i8 C_CALC_CODE  = C_4KB;
  static const i8 C_CALC_STACK = C_4KB;

  class Calculator : public Object
  {
  private:
    eEO  m_eaCode[C_CALC_CODE];   // { EO_code [<bytes>] }
                                  // EO_COLUMN <colNo> ...
    i8   m_iaStack[C_CALC_STACK];

  private:
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Calculator(void);
   ~Calculator(void) noexcept(false);
  
    bool bCompile(TokenVec* pTknVec);
    bool bCompile(TokenVec* pTknVec, StreamDef* pStreamDef, i8 iStartTkn, i8 iEndTkn);
    bool bCompileTest(TokenVec* pTknVec);

    i8   iEvaluate(RowSingle* pRow);
    i8   iEvaluate(Rsb* aRsb, RowSlab* pRowSlab);
    i8   iEvaluate(Rsb* aDstRsb, RowSlab* pDstRowSlab,
                   Rsb* aSrcRsb, RowSlab* pSrcRowSlab);

    Footer* aFooter(void);
  };
  
  Calculator* _pCalculator(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
