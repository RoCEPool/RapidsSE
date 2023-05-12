/*============================================================================*/
/* Board.hpp -- Board                                                         */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "eBK.h"

#include "Card.hpp"
#include "CardCtl.hpp"

#include "Card_AbsJoin.hpp"
#include "Card_Aggreg.hpp"
#include "Card_AndPred.hpp"
#include "Card_As.hpp"

#include "Card_BoolCalc.hpp"
#include "Card_Bound.hpp"
#include "Card_Bounds.hpp"

#include "Card_Call.hpp"
#include "Card_CmpPred.hpp"
#include "Card_COL.hpp"
#include "Card_Combined.hpp"
#include "Card_Constraint.hpp"
#include "Card_Correlation.hpp"
#include "Card_CostJoining.hpp"  //
#include "Card_CostNetwork.hpp"  //
#include "Card_CostStream.hpp"   //
#include "Card_CostUnicast.hpp"  //
#include "Card_CrossJoin.hpp"

#include "Card_Debug.hpp"
#include "Card_Deck.hpp"
#include "Card_Direction.hpp"

#include "Card_Expr.hpp"         //

#include "Card_Filter.hpp"       //
#include "Card_FiltPred.hpp"     //
#include "Card_FiltPredList.hpp" //
#include "Card_Flat.hpp"         //
#include "Card_Fold.hpp"         //
#include "Card_FoldElist.hpp"    //
#include "Card_From.hpp"         //

#include "Card_GbyAgg.hpp"

#include "Card_Hash.hpp"
#include "Card_Having.hpp"
#include "Card_HshCol.hpp"

#include "Card_IdxCol.hpp"
#include "Card_IdxColList.hpp"
#include "Card_Index.hpp"
#include "Card_IndexJoin.hpp"
#include "Card_IndexList.hpp"
#include "Card_IndexNm.hpp"
#include "Card_IndexScan.hpp"

#include "Card_JoinCast.hpp"
#include "Card_JoinEstimate.hpp" //
#include "Card_JoinOrder.hpp"
#include "Card_JoinOrdering.hpp" //
#include "Card_JoinPredList.hpp" //

#include "Card_Key.hpp"          //
#include "Card_KeyExists.hpp"    //
#include "Card_KeyNotExists.hpp" //

#include "Card_Level.hpp"        //
#include "Card_Limit.hpp"        //

#include "Card_NCorrCols.hpp"
#include "Card_Needed.hpp"
#include "Card_Cast.hpp"
#include "Card_Note.hpp"
#include "Card_NotPred.hpp"

#include "Card_On.hpp"
#include "Card_OP.hpp"
#include "Card_OrdAgg.hpp"
#include "Card_OrPred.hpp"
#include "Card_OuterColNm.hpp"
#include "Card_OuterJoin.hpp"

#include "Card_Project.hpp"
#include "Card_Projection.hpp"

#include "Card_RowCount.hpp"
#include "Card_RowSize.hpp"
#include "Card_RpnCalc.hpp"
#include "Card_RpnTerms.hpp"

#include "Card_Scan.hpp"
#include "Card_Section.hpp"
#include "Card_Send.hpp"
#include "Card_Sort.hpp"
#include "Card_Stage.hpp"
#include "Card_StreamHead.hpp"
#include "Card_StreamJoin.hpp"
#include "Card_StreamKey.hpp"
#include "Card_StreamPer.hpp"  //
#include "Card_StreamPtn.hpp"
#include "Card_StreamSet.hpp"  //
#include "Card_SubSet.hpp"

#include "Card_Table.hpp"
#include "Card_TableList.hpp"
#include "Card_TableNm.hpp"
#include "Card_TablePtn.hpp"
#include "Card_TableSize.hpp"
#include "Card_TblSyn.hpp"
#include "Card_Term.hpp"
#include "Card_Top.hpp"
#include "Card_Topology.hpp"

#include "Card_Unique.hpp"

#include "Card_Where.hpp"

#include "VertexSpec.hpp"
#include "DeckSpec.hpp"
#include "StageSpec.hpp"
#include "OpSpec.hpp"

#include "SubQuery.hpp"
#include "Scope.hpp"


namespace STORE
{
  class Bench;
  class Dissector;
  class Optimiser;
  class CardIter;
  class Weaver;

  class Board : public Object
  {
  protected:
    i8          m_i_Board;
    s32         m_sBoardNm;
    eBK         m_eBoardKind;  // joinOrdering, subquery

    Bench*      m_pBench;
    SubQuery*   m_pSubQuery;
    Card_Deck*  m_pCard_Deck;

    s32         m_sXhand;
    s256        m_sXform;
    i8          m_iXform;
    s256        m_sXdesc;

    bool        m_bPrepareSubQueries;
    bool        m_bFlattenSubQueries;
    bool        m_bApplyOptimisations;
    bool        m_bMakeWeavable;

  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Board(Stz* szBoardNm, eBK eBoardKind, i8 i_Board, Bench* pBench);
   ~Board(void) noexcept(false);

    static void vDerive_foldExpr_H_A_NC_ro(CardIter* pCI);
    static void vDerive_foldProjection_F(CardIter* pCI);

    static bool bFindAndMarkSubQryCall(CardIter* pCI);

    static void vMorphAll_table_To_EEop(CardIter* pCI);

    static void vIndividuate_indexJoin_filter_b(CardIter* pCI);
    static void vIndividuate_crossJoin_filter(CardIter* pCI);

    static void vValidate(CardIter* pCI);
    static void vValidate_SubQuery(CardIter* pCI);

    Bench* pBench(void);

    i8  i_Board(void);
    void vBoardNo(i8 i_Board);
    eBK eBoardKind(void);
    Stz* szBoardNm(void);
    void vBoardNm(Stz* szBoardNm);

    CardCtl* pCardCtl(void);
    void vClearMarks(void);

    Card_Deck* pDeck(void);


    void vPinBanners(Ribbon* pRibbon);
    void vPinCards(Ribbon* pRibbon);
    void vPinTitle(Ribbon* pRibbon,
                   i8 iTransformNo,
                   Stz* szExplain=nullptr);
    void vPrintBoard(Stz* szDescription=nullptr);

    SubQuery* pSubQuery(void);


    Stz* szXdesc(void);
    void vXdesc(Stz* szDescription);
    i8   iXform(void);
    void vXform(i8   iXform);
    Stz* szXform(void);
    void vXform(Stz* szTransformation);
    Stz* szXhand(void);
    void vXhand(const char* asXhand);


    bool bPrepareSubQueries(void);
    void vPrepareSubQueries(bool bPrepareSubQueries);
    bool bFlattenSubQueries(void);
    void vFlattenSubQueries(bool bFlattenSubQueries);
    bool bApplyOptimisations(void);
    void vApplyOptimisations(bool bApplyOptimisations);
    bool bMakeWeavable(void);
    void vMakeWeavable(bool bMakeWeavable);

  private:
    Footer* aFooter(void);
  };

  Board* _pBoard(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
