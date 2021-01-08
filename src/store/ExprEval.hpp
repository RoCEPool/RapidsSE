/*============================================================================*/
/* ExprEval.hpp -- ExprEval columns                                             */
/*============================================================================*/
#pragma once

namespace STORE
{
  class ExprEval : public Definition
  {
  public:

    u8      m_uExprEvalColumns;
    s4K     m_sAsmText;
    i8      m_iStreamInColumns;
    Column* m_pStreamInCol[C_MAX_COLUMNS];

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    ExprEval(void);
   ~ExprEval(void) noexcept(false);

    eEC eExprEvalAll(StreamDef* pStreamDefIn);
    eEC eEvaluate(StreamDef* pStreamDefIn, StreamDef* pStreamDefOut, Stz* sExprEvalColumns);

  private:
    void vAddArithmetic(Token*  pTkn);
    eEC  eAddDecimal(Token*  pTkn, Column* pColumnExpr);
    void vAddNumeric(Token*  pTkn, Column* pColumnExpr);
    void vAddRelational(Token*  pTkn);
    void vAddPopCol(eCT eColumnType, i8 iColumn);

    eEC eCollectSourceColumns(StreamDef* pStreamDefIn, TokenVec* pTknVec);
    eEC eExprEvalColumn(StreamDef* pStreamDefIn, StreamDef* pStreamDefOut, TokenVec* pTknVec);
    void vUpdateStreamDefOut(StreamDef* pStreamDefOut, Column* pColumn, Stz* sAliasName);
    Footer* aFooter(void);
  };
  
  ExprEval* _pExprEval(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
