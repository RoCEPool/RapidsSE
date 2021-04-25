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

    s4K      m_sProjectAsm;
    s4K      m_sFilterAsm;

    s4K      m_sAsmGbyIdx;
    s4K      m_sPayLoadKey;
    s4K      m_sPayLoadStore;
    s4K      m_sPayLoadToBuffer;

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
    void vAddArithmetic(Stz* szAsmBuffer, Token*  pTkn, Column* pColumn);
    eEC  eAddCount(eCT eColumnType, i8 iColumn);
    eEC  eAddDate(Stz* szAsmBuffer, Token*  pTkn, Column* pColumnExpr);
    eEC  eAddDecimal(Stz* szAsmBuffer, Token*  pTkn, Column* pColumnExpr);
    void vAddLogical(Stz* szAsmBuffer, Token*  pTkn);
    void vAddNumeric(Stz* szAsmBuffer, Token*  pTkn, Column* pColumnExpr);
    void vAddPopCol(Stz* szAsmBuffer, eCT eColumnType);
    void vAddPushCol(Stz* szAsmBuffer, StreamDef* pStreamDefIn, i8 iColumn);
    void vAddRelational(Stz* szAsmBuffer, Token*  pTkn);
    void vAddStoreCol(Stz* szAsmBuffer, eCT eColumnType, i8 iColumn);
    void vAddString(Stz* szAsmBuffer, Token*  pTkn, Column* pColumnExpr);
    eEC  eAddSum(Stz* sAsmBuffer, eCT eColumnType);
    void vAddUnknown(Stz* szAsmBuffer, Token*  pTkn, Column* pColumnExpr);

    eEC eCollectSourceColumns(StreamDef* pStreamDefIn, TokenVec* pTknVec);
    eEC eExprEval(StreamDef* pStreamDefIn, StreamDef* pStreamDefOut, TokenVec* pTknVec);

    eEC eGenerateColumnCode(Stz* szAsmBuffer, StreamDef* pStreamDefOut, Column* pColumn, eAG eAggType);
    void vUpdateStreamDefOut(StreamDef* pStreamDefOut, Column* pColumn);
    Footer* aFooter(void);
  };
  
  ExprEval* _pExprEval(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
