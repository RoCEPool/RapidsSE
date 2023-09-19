/*============================================================================*/
/* Board.hpp -- Board                                                         */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "eBK.h"
#include "eXF.h"
#include "eOH.h"

#include "CarryForward.hpp"

#include "Card.hpp"
#include "CardCtl.hpp"

#include "Card_AbsJoin.hpp"
#include "Card_Address.hpp"
#include "Card_AGGREGATE.hpp"
#include "Card_Aggreg.hpp"
#include "Card_AndConj.hpp"
#include "Card_Arriving.hpp"
#include "Card_As.hpp"

#include "Card_BoolCalc.hpp"
#include "Card_Bound.hpp"
#include "Card_Bounds.hpp"

#include "Card_Call.hpp"
#include "Card_Caller.hpp"
#include "Card_CallOp.hpp"
#include "Card_Clause.hpp"
#include "Card_CmpConj.hpp"
#include "Card_COLUMN.hpp"
#include "Card_Combined.hpp"
#include "Card_CONDITION.hpp"
#include "Card_Constraint.hpp"
#include "Card_CorrCols.hpp"
#include "Card_Correlation.hpp"
#include "Card_CostJoining.hpp"  //
#include "Card_CostNetwork.hpp"  //
#include "Card_CostStream.hpp"   //
#include "Card_CostUnicast.hpp"  //
#include "Card_Count.hpp"        //
#include "Card_CrossJoin.hpp"

#include "Card_Debug.hpp"
#include "Card_Deck.hpp"
#include "Card_Decision.hpp"
#include "Card_Direction.hpp"

#include "Card_Expr.hpp"         //

#include "Card_Filter.hpp"       //
#include "Card_FiltPred.hpp"     //
#include "Card_FiltPredList.hpp" //
#include "Card_Flat.hpp"         //
#include "Card_Fold.hpp"         //
#include "Card_From.hpp"         //

#include "Card_GbyAgg.hpp"

#include "Card_Hash.hpp"
#include "Card_Hashing.hpp"
#include "Card_Having.hpp"
#include "Card_HshCol.hpp"
#include "Card_HshColList.hpp"

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
#include "Card_KeyCells.hpp"     //
#include "Card_KeyExists.hpp"    //
#include "Card_KeyNotExists.hpp" //

#include "Card_Level.hpp"        //
#include "Card_Limit.hpp"        //

#include "Card_MAIN.hpp"
#include "Card_Map.hpp"
#include "Card_Mapping.hpp"      //

#include "Card_Disposition.hpp"
#include "Card_Needed.hpp"
#include "Card_Cast.hpp"
#include "Card_Note.hpp"
#include "Card_NotConj.hpp"

#include "Card_On.hpp"
#include "Card_OrdAgg.hpp"
#include "Card_OrConj.hpp"
#include "Card_OuterColNm.hpp"
#include "Card_OuterJoin.hpp"

#include "Card_PayloadCells.hpp"
#include "Card_Project.hpp"
#include "Card_Projection.hpp"

#include "Card_RelnOp.hpp"
#include "Card_Required.hpp"
#include "Card_RowCount.hpp"
#include "Card_RowSize.hpp"
#include "Card_RpnCalc.hpp"
#include "Card_RpnTerms.hpp"

#include "Card_Scan.hpp"
#include "Card_Section.hpp"
#include "Card_Send.hpp"
#include "Card_Sort.hpp"
#include "Card_Spare.hpp"
#include "Card_SqyCalls.hpp"
#include "Card_SqyInfo.hpp"
#include "Card_SqyNo.hpp"
#include "Card_Stage.hpp"
#include "Card_Stream.hpp"
#include "Card_StreamHead.hpp"
#include "Card_StreamJoin.hpp"
#include "Card_StreamKey.hpp"
#include "Card_StreamPer.hpp"  //
#include "Card_StreamPtn.hpp"
#include "Card_StreamSet.hpp"  //
#include "Card_SubQuery.hpp"
#include "Card_SubSet.hpp"
#include "Card_Synthesis.hpp"

#include "Card_Table.hpp"
#include "Card_TableInfo.hpp"
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

#include "Card_XCard.hpp"

#include "VertexSpec.hpp"
#include "DeckSpec.hpp"
#include "StageSpec.hpp"
#include "OpSpec.hpp"

#include "SubQuery.hpp"
#include "Scope.hpp"
#include "Mark.hpp"


namespace STORE
{
  class Bench;
  class Dissector;
  class Optimiser;
  class CardIter;

  class Board : public Object
  {
  protected:
    i8          m_i_Board;
    s32         m_sBoardNm;
    eBK         m_eBoard;  // joinOrdering, subquery

    Bench*      m_pBench;
    SubQuery*   m_pSubQuery;
    Card_Deck*  m_pDeck;

    TagSet*     m_pState;

    Mark*       m_pMark;

    eOH         m_ePhase;

  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Board(Stz* szBoardNm, eBK eBoard, i8 i_Board, Bench* pBench);
   ~Board(void) noexcept(false);

    static void vDerive_foldExpr_H_A_NC_ro(CardIter* pCI);
    static void vDerive_foldProjection_F(CardIter* pCI);

    static bool bFindAndMarkSubQryCall(CardIter* pCI);

    Bench* pBench(void);
    i8   i_Board(void);
    void vBoardNo(i8 i_Board);
    eBK  eBoard(void);
    Stz* szBoardNm(void);
    void vBoardNm(Stz* szBoardNm);

    CardCtl* pCardCtl(void);
    void vClearCardMarks(void);

    Card_Deck* pDeck(void);

    void vEndPhase(void);

    Mark* pMark(void);

    void vPrintBoard(CardIter* pCI, Stz* szDescription=nullptr);

    bool bPastPhase(eOH ePhase);

    eOH  ePhase(void);
    void vPhase(eOH ePhase);

    void vRenumberCards(void);

    TagSet* pState(void);
    SubQuery* pSubQuery(void);

    void vValidate(CardIter* pCI);
    void vValidate_SubQuery(CardIter* pCI);

  private:
    void vPinMargin(Ribbon* pRibbon);
    void vPinCards(CardIter* pCI, Ribbon* pRibbon);
    void vPinTitle(Ribbon* pRibbon,
                   i8 iTransformNo,
                   Stz* szExplain=nullptr);

    Footer* aFooter(void);
  };

  Board* _pBoard(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
