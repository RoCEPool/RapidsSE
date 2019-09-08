/*============================================================================*/
/* ExprOp.hpp -- Project                                                   */
/*============================================================================*/
#pragma once

namespace DataOperators
{
  class ExprOp : public Op
  {
  private:
  
    IrwBuffer* m_pIrwBuffer;
    StreamDef* m_pOriginalStreamDef;
    i8         m_iWhichVec[C_MAX_COLUMNVECS];
    i8         m_iWhichCol[C_MAX_COLUMNS];
    
    TokenVec*   m_pTokenVec;
    i8          m_iStartTkn;
    i8          m_iEndTkn;
    Calculator* m_pCalculator;
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    ExprOp(Stz* szOpLabel, Digest* pDigest, OpDefn* aOpDefn, bool bAppR);
   ~ExprOp(void) noexcept(false);
  
    static Op* pNew(Stz* szLabel,
                    Digest* pDigest,  OpDefn* aOpDefn, bool bAppR,
                    TokenVec* pTknVec, i8& i_Tkn_, i8 i_Slot);
    static eOR eCall(void* aThis);
    static eOR eDrain(void* aThis);

    eOR eOpLoop(void);
    eOR eDrainLoop(void);
    
  private:
    void vUpdateStreamDef(void);
    Footer* aFooter(void);
  };

  ExprOp* _pExprOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
