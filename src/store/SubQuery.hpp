/*============================================================================*/
/* SubQuery.hpp -- SubQuery analysis                                                */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "eQD.h"
#include "eQC.h"
#include "eQO.h"
#include "eRO.h"
#include "eFF.h"

namespace STORE
{
  class Board;
  class Card;
  class Card_SubQuery;
  class Card_SubSet;

  class SubQuery : public Object
  {
  protected:

    Board* m_pBoard;     // which board
    i8 m_i_Caller;       // which subquery calls this subquery

    eQD m_eDisposition;  // linear/simple or aggregation
    i8 m_nCorrCols;      // number of correlated columns

    eQC m_eClause;       // called via clause: from|where|having
    eQO m_eCalcOp;       // connecting operator: cmpOp|(not)exists|(not)in
    eRO m_eRelnOp;       // NONE|<|<=|==|!=|>=|> (if connecting/post call is cmpOp)

    eFF m_eFold;         // which fold

    i8 m_nSubQueries;    // how many sub-queries under this one

    Card_SubQuery* m_pCard_SubQuery; // subset of cards which are this folded subquery

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    SubQuery(Board* pBoard);
   ~SubQuery(void) noexcept(false);

    void vAddCard(Card* pCard);
    void vAddSubSet(Card_SubSet* pCard_SubSet);

    Board* pBoard(void);

    bool bByCmpOp(void);
    bool bByExists(void);
    bool bByIn(void);
    bool bByNotExists(void);
    bool bByNotIn(void);

    i8   i_Caller(void);
    void vCaller(i8 i_Caller);

    eQO  eCalcOp(void);
    void vCalcOp(eQO eCalcOp);

    eQC  eClause(void);
    void vClause(eQC eClause);

    i8   nCorrCols(void);
    void vCorrCols(i8 nCorrCols);

    eQD  eDisposition(void);
    void vDisposition(eQD eDisposition);

    void vDump(void);

    eFF  eFold(void);
    void vFold(eFF eFold);

    Card_SubQuery* pGet_Card_SubQuery(void);

    bool bIsAggregation(void);
    bool bIsCorrelated(void);
    bool bIsLinear(void);

    eRO  eRelnOp(void);
    void vRelnOp(eRO eRelnOp);

    i8   nSubQueries(void);
    void vSubQueries(i8 nSubQueries);

    bool bViaFrom(void);
    bool bViaHaving(void);
    bool bViaWhere(void);

  protected:
    Footer* aFooter(void);
  };

  SubQuery* _pSubQuery(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
