/*============================================================================*/
/* FilterOp.hpp -- Where                                                       */
/*============================================================================*/
#pragma once

namespace DataOperators
{
  class FilterOp : public Op
  {
  private:
  
    TokenVec*   m_pTokenVec;
    i8          m_iStartTkn;
    i8          m_iEndTkn;
    Calculator* m_pCalculator;
    IrwBuffer*  m_pIrwBuffer;
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    FilterOp(OpSpec* pOpSpec, Stz* szOpLabel, Digest* pDigest, OpDefn* aOpDefn, bool bAppR);
    ~FilterOp(void) noexcept(false);
    
    static Op* pNew(Stz* szLabel,
                    Digest* pDigest,
                    OpDefn* aOpDefn, bool bAppR, OpSpec* pOpSpec,
                    TokenVec* pTknVec, i8& i_Tkn_, i8 i_Slot);
    static eOR eCall(void* aThis);
    static eOR eDrain(void* aThis);
  
    eOR eOpLoop(void);
    eOR eDrainLoop(void);
    
  private:
    Footer* aFooter(void);
  };

  FilterOp* _pFilterOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
