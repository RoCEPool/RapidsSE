/*============================================================================*/
/* ExprEval.hpp -- ExprEval columns                                           */
/*============================================================================*/
#pragma once

#include "Definition.hpp"
#include "IntStack.hpp"
#include "eEO.h"

namespace STORE
{
  class StreamDef;

  enum eAG {
    /* 0 */ AG_NONE=0,
    /* 1 */ AG_ALL,
    /* 2 */ AG_CNT,
    /* 3 */ AG_MIN,
    /* 4 */ AG_MAX,
    /* 5 */ AG_STD,
    /* 6 */ AG_SUM,
    /* 7 */ AG_VAR,
    /* 8 */ AG_ENUMS
  };

  enum ePK {
    /* 0 */ PK_NONE=0,
    /* 1 */ PK_AGG,
    /* 2 */ PK_EXP,
    /* 3 */ PK_GBY,
    /* 4 */ PK_ENUMS
  };

  class StoreStack : public IntStack
  {

  };

  class ExprEval : public Definition
  {
  public:

    u8       m_uExprEvalColumns;
    u8       m_uExprEvalColumnsRht;
    i8       m_nPayLoadBytes;
    i8       m_iStreamInColumns;
    i8       m_iStreamInColumnsRht;
    i8       m_iJoinStartColumn;

    bool     m_bHasAgg;
    bool     m_bHasGby;
    bool     m_bLeftOuter;

    IntStack m_xTypeStack;
    IntStack m_xTypeStack_Row;
    IntStack m_xStoreStack;
    IntStack m_xAggStack;
    IntStack m_xKeyStack;

    s4K      m_sProjectAsm;
    s4K      m_sAggInitAsm;
    s4K      m_sAggAsm;
    s4K      m_sJoinAsm_CrwRht;
    s4K      m_sJoinAsm_RowRht;

    s4K      m_sGbyIdxAsm;
    s4K      m_sPayLoadAggAsm;
    s4K      m_sPayLoadKeyAsm;
    s4K      m_sPayLoadStoreAsm;
    s4K      m_sPayLoadToBufferAsm;

    s4K      m_sLowerBoundAsm;
    s4K      m_sUpperBoundAsm;

    s256     m_sLftCrwCol;
    s256     m_sLftRowCol;

    eEO      m_eLowerBound;
    eEO      m_eUpperBound;

    i8       m_iProjectColumn;

    i8       m_iProjections;

    ePK      m_eProjectKind[C_MAX_COLUMNS];

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    ExprEval(void);
   ~ExprEval(void) noexcept(false);

    static void vRepair(Object* pObject, eRF eRepair);

    eEC eEvaluate(StreamDefBase* pStreamDefIn, StreamDefBase* pStreamDefOut, Stz* sExprEvalColumns);
    eEC eEvaluateIndexJoin(StreamDefBase* pStreamDefLft, StreamDefBase* pStreamDefRht,
                           StreamDefBase* pStreamDefOut, IndexDef* pIndexDefRht,
                           Stz* sJoinExpr);
    eEC eEvaluateIndexScan(IndexDef* pIndexDef, Stz* sExprEvalColumns);
    eEC eEvaluateLoopJoin(StreamDefBase* pStreamDefLft, StreamDefBase* pStreamDefRht,
                          StreamDefBase* pStreamDefOut, Stz* sJoinExpr);

    bool bHasAggregate(void);
    bool bHasGroupBy(void);

  private:
    void vAddArithmetic(Stz* szAsmBuffer, IntStack* pTypeStack, Token*  pTkn);
    void vAddLogical(Stz* szAsmBuffer, IntStack* pTypeStack, Token*  pTkn);
    void vAddRelational(Stz* szAsmBuffer, IntStack* pTypeStack, Token*  pTkn);

    void vAppendColumnToStreamDef(StreamDefBase* pStreamDefOut, Column* pColumn);

    void vBoundRelation(Stz* szAsmBuffer, IntStack* pTypeStack, Token* pTkn);

    eEC  eCollectLeftColumns(StreamDefBase* pStreamDefLft, TokenVec* pTknVec);
    eEC  eCollectRightColumns(StreamDefBase* pStreamDefRht, TokenVec* pTknVec);
    eEC  eCollectSourceColumns(StreamDefBase* pStreamDefIn, TokenVec* pTknVec);

    void vCreateGbyIdxCol(Stz* szAsmBuffer, eDM eDatum, i8 i_IdxCol);
    void vCreateIdxKey(Stz* szAsmBuffer, IntStack* pTypeStack);

    eEC eExprEvalAggGby(StreamDefBase* pStreamDefIn, StreamDefBase* pStreamDefOut, TokenVec* pTknVec);
    eEC eExprEvalIndexJoin(StreamDefBase* pStreamDefLft, StreamDefBase* pStreamDefRht,
                           StreamDefBase* pStreamDefOut, IndexDef* pIndexDef, TokenVec* pTknVec);
    eEC eExprEvalIndexScan(StreamDefBase* pStreamDefLft, IndexDef* pIndexDef, bool bLeftTable, TokenVec* pTknVec);
    eEC eExprEvalLoopJoin(StreamDefBase* pStreamDefLft, StreamDefBase* pStreamDefRht, StreamDefBase* pStreamDefOut, TokenVec* pTknVec);
    eEC eExprEvalProject(StreamDefBase* pStreamDefIn, StreamDefBase* pStreamDefOut, TokenVec* pTknVec);

    void vGenerateAggregateCode(eAG eAggType, Stz* szAsmBuffer, Stz* szAsmInitBuffer, IntStack* pTypeStack);

    void vGenerateCount(Stz* szAsmBuffer, Stz* sAggInit, IntStack* pTypeStack);
    void vGenerateCountAll(Stz* szAsmBuffer, Stz* sAggInit, IntStack* pTypeStack);
    void vGenerateMin(Stz* sAsmBuffer, Stz* sAggInit, IntStack* pTypeStack);
    void vGenerateMax(Stz* sAsmBuffer, Stz* sAggInit, IntStack* pTypeStack);
    void vGenerateSum(Stz* sAsmBuffer, Stz* sAggInit, IntStack* pTypeStack);

    i8   iOrdinalToLocal(i8 i_ColumnOrdinal);

    void vPayFetchCol(Stz* szAsmBuffer, IntStack* pTypeStack, eDM eDatum);
    void vPayStoreCol(Stz* szAsmBuffer, IntStack* pTypeStack, eCT eColumnType);

    void vPopDstCol(Stz* szAsmBuffer, IntStack* pTypeStack);

    void vPushDatum(eDM eDatum, Stz* szAsmBuffer, IntStack* pTypeStack, Token*  pTkn);
    void vPushDstCol(Stz* szAsmBuffer, IntStack* pTypeStack);
    void vPushLftCrwCol(Stz* szAsmBuffer, IntStack* pTypeStack, eCT eColumnType, i8 i_Column);
    void vPushRhtCrwCol(Stz* szAsmBuffer, IntStack* pTypeStack, eCT eColumnType, i8 i_Column);
    void vPushRhtRowCol(Stz* szAsmBuffer, IntStack* pTypeStack, eCT eColumnType, i8 i_Column);
    void vPushLftRowCol(Stz* szAsmBuffer, IntStack* pTypeStack, eCT eColumnType, i8 i_Column);
    void vPushLftRowCol2(Stz* szAsmBuffer, IntStack* pTypeStack, eCT eColumnType, i8 i_Column);

    void vCheckIsNull(Stz* szAsmBuffer, IntStack* pTypeStack);
    void vPushKeyElm(Stz* szAsmBuffer, IntStack* pTypeStack, eCT eColumnType, i8 i_Key);
    void vPushLclCol(Stz* szAsmBuffer, IntStack* pTypeStack, eCT eColumnType, i8 i_Local);

  public:
    Footer* aFooter(void);
  };
  
  ExprEval* _pExprEval(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
