/*============================================================================*/
/* SubQuery.hpp -- SubQuery analysis                                                */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "eQD.h"
#include "eQC.h"
#include "eQO.h"
#include "eRO.h"

#include "Symbol.hpp"

namespace STORE
{
  class Board;
  class Card;
  class Card_SubSet;
  class CardIter;

  class SubQuery : public Object
  {
  protected:

    Symbol m_xSymbol;    // SF_SQY_NM and SF_SQY_NO

    Board* m_pBoard;     // which board

    i8  m_i_Caller;       // which subquery calls this subquery

    eQD m_eDisposition;  // linear/simple or aggregation
    i8  m_nCorrCols;      // number of correlated columns

    eQC m_eClause;       // called via clause: from|where|having
    eQO m_eCallOp;       // connecting operator: (not)exists|(not)in|relnOp
    eRO m_eRelnOp;       // NONE|<|<=|==|!=|>=|> (if connecting/post call is cmpOp)

    i8 m_nSubQueries;    // how many sub-queries under this one

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    SubQuery(i8 i_Board, Stz* szSqyNm, Board* pBoard);
   ~SubQuery(void) noexcept(false);

    Board* pBoard(void);

    bool bByExists(void);
    bool bByIn(void);
    bool bByNotExists(void);
    bool bByNotIn(void);
    bool bByRelnOp(void);

    i8   i_Caller(void);
    void vCaller(i8 i_Caller);

    eQO  eCallOp(void);
    void vCallOp(eQO eCalcOp);

    eQC  eClause(void);
    void vClause(eQC eClause);

    i8   nCorrCols(void);
    void vCorrCols(i8 nCorrCols);

    eQD  eDisposition(void);
    void vDisposition(eQD eDisposition);

    void vDump(i8 iDepth=0);

    bool bIsAggregation(void);
    bool bIsCorrelated(void);
    bool bIsLinear(void);

    eRO  eRelnOp(void);
    void vRelnOp(eRO eRelnOp);

    Stz* szSqyNm(void);
    void vSqyNm(Stz* szSqyNm);
    Stz* szSqyNo(void);
    void vSqyNo(Stz* szSqyNo);

    i8   nSubQueries(void);
    void vSubQueries(i8 nSubQueries);

    Symbol* pSymbol(void);

    bool bViaFrom(void);
    bool bViaHaving(void);
    bool bViaWhere(void);

    void vValidate(CardIter* pCI);

  protected:
    Footer* aFooter(void);
  };

  SubQuery* _pSubQuery(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
