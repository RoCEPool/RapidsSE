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
    /* 1 */ AG_ALL,
    /* 2 */ AG_AVG,
    /* 3 */ AG_CNT,
    /* 4 */ AG_MIN,
    /* 5 */ AG_MAX,
    /* 6 */ AG_STD,
    /* 7 */ AG_SUM,
    /* 8 */ AG_VAR,
    /* 9 */ AG_ENUMS
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

    s4K      m_sProjectAsm;
    s4K      m_sFilterAsm;
    s4K      m_sAggInitAsm;
    s4K      m_sAggAsm;

    s4K      m_sGbyIdxAsm;
    s4K      m_sPayLoadAggAsm;
    s4K      m_sPayLoadKeyAsm;
    s4K      m_sPayLoadStoreAsm;
    s4K      m_sPayLoadToBufferAsm;

    i8       m_iDstColumn;

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
    void vAddArithmetic(Stz* szAsmBuffer, Token*  pTkn, Column* pColumnExpr);
    void vAddDate(Stz* szAsmBuffer, Token*  pTkn);
    void vAddDecimal(Stz* szAsmBuffer, Token*  pTkn);
    void vAddGbyIdxCol(Stz* szAsmBuffer, eCT eColumnType, i8 i_Local);
    void vAddLogical(Stz* szAsmBuffer, Token*  pTkn);
    void vAddInteger(Stz* szAsmBuffer, Token*  pTkn);
    eEC  eAddPayLoadKey(Stz* szAsmBuffer, StreamDef* pStreamDefIn, i8 i_ColumnOrdinal);
    void vAddPopCol(Stz* szAsmBuffer, eCT eColumnType);
    void vAddPushCol(Stz* szAsmBuffer, StreamDef* pStreamDefIn, i8 iColumn);
    void vAddRelational(Stz* szAsmBuffer, Token*  pTkn);
    void vAddStoreCol(Stz* szAsmBuffer, Column* pColumnExpr);
    void vAddString(Stz* szAsmBuffer, Token*  pTkn);
    void vAddTime(Stz* szAsmBuffer, Token*  pTkn);
    void vAddTimestamp(Stz* szAsmBuffer, Token*  pTkn);
    void vAddUnknown(Stz* szAsmBuffer, Token*  pTkn);

    void vAddToBufferCol(Stz* szAsmBuffer, Column* pColumn);

    eEC eCollectSourceColumns(StreamDef* pStreamDefIn, TokenVec* pTknVec);

    eEC eExprEval(StreamDef* pStreamDefIn, StreamDef* pStreamDefOut, TokenVec* pTknVec);
    eEC eExprEvalAgg(StreamDef* pStreamDefIn, StreamDef* pStreamDefOut, TokenVec* pTknVec);
    eEC eExprEvalGby(StreamDef* pStreamDefIn, StreamDef* pStreamDefOut, TokenVec* pTknVec);

    eEC eGenerateColumnCode(Stz* szAsmBuffer, StreamDef* pStreamDefOut, Column* pColumn);
    eEC eGenerateColumnCodeGby(StreamDef* pStreamDefOut, Column* pColumn);
    eEC eGenerateAggregateCode(Stz* szAsmBuffer, StreamDef* pStreamDefOut, Column* pColumn, eAG eAggType);
    eEC eGenerateGroupbyCode(StreamDef* pStreamDefOut, Column* pColumn, eAG eAggType);

    eEC eGenerateCount(Stz* szAsmBuffer, Stz* sAggInit);
    eEC eGenerateCountAll(Stz* szAsmBuffer, Stz* sAggInit);
    eEC eGenerateMin(Stz* sAsmBuffer, Stz* sAggInit);
    eEC eGenerateMax(Stz* sAsmBuffer, Stz* sAggInit);
    eEC eGenerateSum(Stz* sAsmBuffer, Stz* sAggInit);

    i8 iOrdinalToLocal(i8 i_ColumnOrdinal);

    void vUpdateStreamDefOut(StreamDef* pStreamDefOut, Column* pColumn);
    Footer* aFooter(void);
  };
  
  ExprEval* _pExprEval(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
