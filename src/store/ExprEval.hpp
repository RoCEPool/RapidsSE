/*============================================================================*/
/* ExprEval.hpp -- ExprEval columns                                           */
/*============================================================================*/
#pragma once

#include "Definition.hpp"
#include "IntStack.hpp"

namespace STORE
{
  class StreamDef;

  enum eAG {
    /* 0 */ AG_NONE=0,
    /* 1 */ AG_AVG,
    /* 2 */ AG_CNT,
    /* 3 */ AG_MIN,
    /* 4 */ AG_MAX,
    /* 5 */ AG_STD,
    /* 6 */ AG_SUM,
    /* 7 */ AG_VAR,
    /* 8 */ AG_ENUMS
  };

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
    eEC  eAddCount(eCT eColumnType, i8 iColumn);
    eEC  eAddDate(Token*  pTkn, Column* pColumnExpr);
    eEC  eAddDecimal(Token*  pTkn, Column* pColumnExpr);
    eEC  eAddGby(Column* pColumn, i8 iGby);
    void vAddLogical(Token*  pTkn);
    eEC  eAddMin(eCT eColumnType, i8 iColumn);
    eEC  eAddMax(eCT eColumnType, i8 iColumn);
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
                         i8 i_DstColumn, eAG eAGgType);
    void vUpdateStreamDefOut(StreamDef* pStreamDefOut, Column* pColumn);
    Footer* aFooter(void);
  };
  
  ExprEval* _pExprEval(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
