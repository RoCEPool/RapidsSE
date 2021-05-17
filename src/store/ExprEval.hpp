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
    void vAddArithmetic(Stz* szAsmBuffer, Token*  pTkn);
    void vAddLogical(Stz* szAsmBuffer, Token*  pTkn);
    void vAddRelational(Stz* szAsmBuffer, Token*  pTkn);

    void vAppendColumnToStreamDef(StreamDef* pStreamDefOut, Column* pColumn);

    void vCreateGbyIdxCol(Stz* szAsmBuffer, eCT eColumnType, i8 i_Local);
    void vCreatePayLoadKey(Stz* szAsmBuffer, eCT eColumnType, i8 i_Local);
    eEC eCollectSourceColumns(StreamDef* pStreamDefIn, TokenVec* pTknVec);

    void vPayStoreCol(Stz* szAsmBuffer, eCT eColumnType);

    void vPushDate(Stz* szAsmBuffer, Token*  pTkn);
    void vPushDecimal(Stz* szAsmBuffer, Token*  pTkn);
    void vPushInteger(Stz* szAsmBuffer, Token*  pTkn);
    void vPushLclCol(Stz* szAsmBuffer, eCT eColumnType, i8 i_Local);
    void vPushDstCol(Stz* szAsmBuffer);
    void vPushString(Stz* szAsmBuffer, Token*  pTkn);
    void vPushTime(Stz* szAsmBuffer, Token*  pTkn);
    void vPushTimestamp(Stz* szAsmBuffer, Token*  pTkn);
    void vPushUnknown(Stz* szAsmBuffer, Token*  pTkn);

    void vPopDstCol(Stz* szAsmBuffer);

    eEC eExprEval(StreamDef* pStreamDefIn, StreamDef* pStreamDefOut, TokenVec* pTknVec);
    eEC eExprEvalAgg(StreamDef* pStreamDefIn, StreamDef* pStreamDefOut, TokenVec* pTknVec);
    eEC eExprEvalGby(StreamDef* pStreamDefIn, StreamDef* pStreamDefOut, TokenVec* pTknVec);

    void vGenerateColumnCodeGby(StreamDef* pStreamDefOut, Column* pColumn);
    void vGenerateAggregateCode(eAG eAggType, Stz* szAsmBuffer, Stz* szAsmInitBuffer);

    void vGenerateCount(Stz* szAsmBuffer, Stz* sAggInit);
    void vGenerateCountAll(Stz* szAsmBuffer, Stz* sAggInit);
    void vGenerateMin(Stz* sAsmBuffer, Stz* sAggInit);
    void vGenerateMax(Stz* sAsmBuffer, Stz* sAggInit);
    void vGenerateSum(Stz* sAsmBuffer, Stz* sAggInit);

    i8 iOrdinalToLocal(i8 i_ColumnOrdinal);

    void vUpdateStreamDefOut(StreamDef* pStreamDefOut, Column* pColumn);
    Footer* aFooter(void);
  };
  
  ExprEval* _pExprEval(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
