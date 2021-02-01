/*============================================================================*/
/* ExprEval.hpp -- ExprEval columns                                             */
/*============================================================================*/
#pragma once

#include "Definition.hpp"

namespace STORE
{
  class StreamDef;

  class ExprEval : public Definition
  {
  public:

    u8      m_uExprEvalColumns;
    s4K     m_sAsmText;
    s4K     m_sAsmGbyIdx;
    s4K     m_sAsmGbyKey;
    i8      m_iStreamInColumns;
    bool    m_bHasAgg;
    bool    m_bHasGby;
    i8      m_nPayLoadBytes;
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

    bool bHasAggregate(void);
    bool bHasGroupBy(void);

  private:
    void vAddArithmetic(Token*  pTkn, Column* pColumn);
    eEC  eAddDate(Token*  pTkn, Column* pColumnExpr);
    eEC  eAddDecimal(Token*  pTkn, Column* pColumnExpr);
    eEC  eAddGby(Column* pColumn);
    void vAddLogical(Token*  pTkn);
    void vAddNumeric(Token*  pTkn, Column* pColumnExpr);
    void vAddRelational(Token*  pTkn);
    void vAddString(Token*  pTkn, Column* pColumnExpr);
    eEC  eAddSum(eCT eColumnType, i8 iColumn);
    void vAddPopCol(eCT eColumnType, i8 iColumn);

    eEC eCollectSourceColumns(StreamDef* pStreamDefIn, TokenVec* pTknVec);
    eEC eExprEvalColumn(StreamDef* pStreamDefIn, StreamDef* pStreamDefOut, TokenVec* pTknVec);
    eEC eHandleOutColumn(StreamDef* pStreamDefOut, Column* pColumn,
                         Stz* sAliasName, i8 i_DstColumn, bool bSum);
    void vUpdateStreamDefOut(StreamDef* pStreamDefOut, Column* pColumn, Stz* sAliasName);
    Footer* aFooter(void);
  };
  
  ExprEval* _pExprEval(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
