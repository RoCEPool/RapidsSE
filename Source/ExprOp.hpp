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
    i8         m_iWhichVec[C_MAX_COLUMNS];
    i8         m_iWhichCol[C_MAX_COLUMNS];
    i8         m_iTkn;
    
    TokenVec*   m_pTknVec;    
    Calculator* m_pCalculator[C_MAX_COLUMNS];
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    ExprOp(OpSpec* pOpSpec, Pipeline* pPipeline);
   ~ExprOp(void) noexcept(false);
  
    static Op* pNew(OpSpec* pOpSpec, Pipeline* pPipeline);
    static eOR eCall(void* aThis);
    static eOR eDrain(void* aThis);

    eOR eOpLoop(void);
    eOR eDrainLoop(void);
    
  private:
    Column* pCreateColumn(i8 i_ExprCol, Stz* sColumnNm, i8 iStartTkn, i8 iEndTkn);
    void vUpdateStreamDef(void);
    Footer* aFooter(void);
  };

  ExprOp* _pExprOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
