/*============================================================================*/
/* Board.hpp -- Board                                                         */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "eBK.h"
#include "eBT.h"
#include "eBR.h"
#include "eBF.h"
#include "eBC.h"

#include "eQS.h"

#include "Card.hpp"
#include "CardCtl.hpp"

#include "Card_AbsJoin.hpp"
#include "Card_AggElist.hpp"
#include "Card_Aggreg.hpp"
#include "Card_Aggregates.hpp"
#include "Card_AndPred.hpp"
#include "Card_As.hpp"

#include "Card_BoolCalc.hpp"
#include "Card_Bounds.hpp"

#include "Card_CalcOp.hpp"
#include "Card_Call.hpp"
#include "Card_CandIndex.hpp"
#include "Card_Cast.hpp"
#include "Card_Clause.hpp"
#include "Card_CmpPred.hpp"
#include "Card_Combined.hpp"
#include "Card_Comment.hpp"
#include "Card_Constraint.hpp"
#include "Card_ColumnRef.hpp"
#include "Card_CoPtnJoin.hpp"
#include "Card_CoPtnJoinList.hpp"
#include "Card_CoPtnPred.hpp"
#include "Card_CoPtnPredList.hpp"
#include "Card_CrossJoin.hpp"
#include "Card_CrossPtnJoin.hpp"
#include "Card_CrossPtnJoinList.hpp"
#include "Card_CrossPtnPred.hpp"

#include "Card_Debug.hpp"
#include "Card_Deck.hpp"
#include "Card_Direction.hpp"

#include "Card_EBound.hpp"
#include "Card_ExePred.hpp"
#include "Card_Exists.hpp"
#include "Card_Expr.hpp"

#include "Card_Filter.hpp"
#include "Card_Fold.hpp"

#include "Card_GbyAgg.hpp"
#include "Card_GbyElist.hpp"

#include "Card_Hash.hpp"
#include "Card_Having.hpp"

#include "Card_IdxCol.hpp"
#include "Card_IdxColList.hpp"
#include "Card_IdxColNm.hpp"
#include "Card_Index.hpp"
#include "Card_IndexJoin.hpp"
#include "Card_IndexList.hpp"
#include "Card_IndexNm.hpp"
#include "Card_IndexPred.hpp"
#include "Card_IndexScan.hpp"

#include "Card_JoinOrder.hpp"
#include "Card_JoinOrderSeq.hpp"
#include "Card_JoinPredList.hpp"

#include "Card_KeyExists.hpp"

#include "Card_LBound.hpp"
#include "Card_Level.hpp"
#include "Card_Limit.hpp"
#include "Card_LinksTo.hpp"

#include "Card_Manner.hpp"
#include "Card_MaybeIndexJoin.hpp"

#include "Card_NCorrCols.hpp"
#include "Card_Next.hpp"
#include "Card_Note.hpp"
#include "Card_NParmCols.hpp"

#include "Card_ObyElist.hpp"
#include "Card_OrPred.hpp"
#include "Card_OrdAgg.hpp"

#include "Card_Param.hpp"
#include "Card_ParamList.hpp"
#include "Card_Partition.hpp"
#include "Card_Permutation.hpp"
#include "Card_PmzWhere.hpp"
#include "Card_Project.hpp"
#include "Card_ProjElist.hpp"
#include "Card_PtnCol.hpp"
#include "Card_PtnColList.hpp"
#include "Card_PtnColNm.hpp"
#include "Card_PtndJoin.hpp"
#include "Card_PtndJoinSet.hpp"

#include "Card_RBound.hpp"
#include "Card_RelnOp.hpp"
#include "Card_RpnCalc.hpp"
#include "Card_RpnTerms.hpp"

#include "Card_SbyElist.hpp"
#include "Card_Scan.hpp"
#include "Card_Send.hpp"
#include "Card_Size.hpp"
#include "Card_Sort.hpp"
#include "Card_Stage.hpp"
#include "Card_StmColNm.hpp"
#include "Card_StmColOdn.hpp"
#include "Card_Stream.hpp"
#include "Card_SubSet.hpp"

#include "Card_Table.hpp"
#include "Card_TableList.hpp"
#include "Card_TableNm.hpp"
#include "Card_TablePtn.hpp"
#include "Card_TblColOdn.hpp"
#include "Card_Term.hpp"
#include "Card_Topology.hpp"
#include "Card_TstPred.hpp"
#include "Card_Twist.hpp"
#include "Card_Type.hpp"

#include "Card_UBound.hpp"
#include "Card_Unique.hpp"

#include "Card_Where.hpp"

#include "VertexSpec.hpp"
#include "DeckSpec.hpp"
#include "StageSpec.hpp"
#include "OpSpec.hpp"

#include "SubQuery.hpp"

namespace STORE
{
  class Bench;
  class Dissector;
  class Drafter;
  class Optimiser;
  class Xgearing;
  class Weaver;

  class Board : public Object
  {
  protected:
    i8          m_i_Board;
    s32         m_sBoardNm;
    eBK         m_eBoardKind;  // costEst, partition, permute, subpipe
    eBT         m_eBoardType;  // original, view
    eBR         m_eBoardRole;  // driver, follower
    eBF         m_eBoardForm;  // fanin, linear
    eBC         m_eBoardCall;  // exists, from, in

    eQS         m_eSubQuery;

    Card_Deck*  m_pCard_Deck;

    Optimiser*  m_pOptimiser;
    Bench*      m_pBench;

    s32         m_sXhand;
    s256        m_sXform;
    i8          m_iXform;
    s256        m_sXdesc;

    bool        m_bMayHaveStmColOdn;

    SubQuery*   m_pSubQuery;

  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Board(Stz* szBoardNm, eBK eBoardKind, i8 i_Board, eBT eBoardType, Bench* pBench);
   ~Board(void) noexcept(false);

    i8  i_Board(void);
    eBC  eBoardCall(void);
    void vBoardCall(eBC eBoardCall);
    eBF  eBoardForm(void);
    void vBoardForm(eBF eBoardForm);
    eBK eBoardKind(void);
    Stz* szBoardNm(void);
    void vBoardNm(Stz* szBoardNm);
    eBR  eBoardRole(void);
    void vBoardRole(eBR eBoardRole);

    CardCtl* pCardCtl(void);
    void vClearMarks(void);

    Card_Deck* pDeck(void);

    bool bMayHaveStmColOdn(void);
    void vMayHaveStmColOdn(bool bMayHaveStmColOdn);

    void vPinBanners(Ribbon* pRibbon);
    void vPinCards(Ribbon* pRibbon);
    void vPinTitle(Ribbon* pRibbon,
                   i8 iTransformNo,
                   Stz* szExplain=nullptr);
    void vPrintBoard(Stz* szDescription=nullptr);

    eQS  eSubQuery(void);
    void vSubQuery(eQS  eSubQuery);

    SubQuery* pSubQuery(void);

    Stz* szXdesc(void);
    void vXdesc(Stz* szDescription);
    i8   iXform(void);
    void vXform(i8   iXform);
    Stz* szXform(void);
    void vXform(Stz* szTransformation);
    Stz* szXhand(void);
    void vXhand(const char* asXhand);

  private:
    Footer* aFooter(void);
  };

  Board* _pBoard(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
