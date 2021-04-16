/*============================================================================*/
/* ExprEval.hpp -- ExprEval columns                                           */
/*============================================================================*/
#pragma once

#include "Definition.hpp"
#include "IntStack.hpp"

namespace STORE
{
  class StreamDef;

  class ExprEval : public Definition
  {
  public:

    u8       m_uExprEvalColumns;
    i8       m_nPayLoadBytes;
    i8       m_iStreamInColumns;

    bool     m_bHasAgg;
    bool     m_bHasGby;

    IntStack m_xTypeStack;

    s4K      m_sAsmText;
    s4K      m_sAsmGbyIdx;
    s4K      m_sPayLoadKey;
    s4K      m_sPayLoadStore;
    s4K      m_sPayLoadToBuffer;

    Column*  m_pStreamInCol[C_MAX_COLUMNS];

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    ExprEval(void);
   ~ExprEval(void) noexcept(false);

    eEC eEvaluate(StreamDef* pStreamDefIn, StreamDef* pStreamDefOut, Stz* sExprEvalColumns);

    bool bHasAggregate(void);
    bool bHasGroupBy(void);

  private:
    void vAddArithmetic(Token*  pTkn, Column* pColumn);
    eEC  eAddDate(Token*  pTkn, Column* pColumnExpr);
    eEC  eAddDecimal(Token*  pTkn, Column* pColumnExpr);
    eEC  eAddGby(Column* pColumn, i8 iGby);
    void vAddLogical(Token*  pTkn);
    void vAddNumeric(Token*  pTkn, Column* pColumnExpr);
    void vAddPopCol(eCT eColumnType, i8 iColumn);
    void vAddRelational(Token*  pTkn);
    void vAddStoreCol(eCT eColumnType, i8 iColumn);
    void vAddString(Token*  pTkn, Column* pColumnExpr);
    eEC  eAddSum(eCT eColumnType, i8 iColumn);
    eEC  eAddSumGby(eCT eColumnType, i8 iColumn);
    void vAddUnknown(Token*  pTkn, Column* pColumnExpr);

    eEC eCollectSourceColumns(StreamDef* pStreamDefIn, TokenVec* pTknVec);
    eEC eExprEval(StreamDef* pStreamDefIn, StreamDef* pStreamDefOut, TokenVec* pTknVec);
    eEC eHandleGbyColumn(Column* pColumn, i8 iGby);
    eEC eHandleOutColumn(StreamDef* pStreamDefOut, Column* pColumn,
                         i8 i_DstColumn, bool bSum);
    void vUpdateStreamDefOut(StreamDef* pStreamDefOut, Column* pColumn);
    Footer* aFooter(void);
  };
  
  ExprEval* _pExprEval(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
