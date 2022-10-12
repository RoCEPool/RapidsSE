/*============================================================================*/
/* Board.hpp -- Board                                                         */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "eBK.h"
#include "eBT.h"

#include "Card.hpp"
#include "CardCtl.hpp"

#include "Card_AbsJoin.hpp"
#include "Card_AggElist.hpp"
#include "Card_AggLevel.hpp"
#include "Card_Aggreg.hpp"
#include "Card_Aggregates.hpp"
#include "Card_As.hpp"

#include "Card_Board.hpp"

#include "Card_Calc.hpp"
#include "Card_CalcBool.hpp"
#include "Card_CandIndex.hpp"
#include "Card_CardPtr.hpp"
#include "Card_Combined.hpp"
#include "Card_Comment.hpp"
#include "Card_ColumnRef.hpp"
#include "Card_CoPtnJoin.hpp"
#include "Card_CoPtnJoinList.hpp"
#include "Card_CoPtnPred.hpp"
#include "Card_CoPtnPredList.hpp"
#include "Card_CrossPtnJoin.hpp"
#include "Card_CrossPtnJoinList.hpp"
#include "Card_CrossPtnPred.hpp"

#include "Card_Debug.hpp"
#include "Card_Direction.hpp"

#include "Card_EBound.hpp"
#include "Card_Exists.hpp"
#include "Card_Expr.hpp"
#include "Card_ExprBool.hpp"

#include "Card_Filter.hpp"

#include "Card_GbyElist.hpp"

#include "Card_Hash.hpp"
#include "Card_Having.hpp"

#include "Card_IdxColNm.hpp"
#include "Card_IdxColList.hpp"
#include "Card_IndexPtr.hpp"
#include "Card_IndexJoin.hpp"
#include "Card_IndexList.hpp"
#include "Card_IndexPred.hpp"
#include "Card_IndexScan.hpp"

#include "Card_JoinOrder.hpp"
#include "Card_JoinOrderSeq.hpp"
#include "Card_JoinPred.hpp"
#include "Card_JoinPredList.hpp"

#include "Card_LBound.hpp"
#include "Card_Limit.hpp"

#include "Card_OrdAgg.hpp"
#include "Card_ObyElist.hpp"

#include "Card_Permutation.hpp"
#include "Card_AndPred.hpp"
#include "Card_CmpPred.hpp"
#include "Card_OrPred.hpp"
#include "Card_Project.hpp"
#include "Card_ProjElist.hpp"
#include "Card_PtndJoin.hpp"
#include "Card_PtndJoinSet.hpp"

#include "Card_RBound.hpp"
#include "Card_Retrieve.hpp"
#include "Card_RpnTerms.hpp"

#include "Card_SbyElist.hpp"
#include "Card_Scan.hpp"
#include "Card_ScanPred.hpp"
#include "Card_Send.hpp"
#include "Card_Sort.hpp"
#include "Card_Stage.hpp"
#include "Card_StageTopo.hpp"
#include "Card_Stream.hpp"
#include "Card_StreamElist.hpp"
#include "Card_SubSet.hpp"

#include "Card_Table.hpp"
#include "Card_TableList.hpp"
#include "Card_TablePtn.hpp"
#include "Card_TablePtr.hpp"
#include "Card_Term.hpp"
#include "Card_Type.hpp"

#include "Card_UBound.hpp"
#include "Card_Unique.hpp"

#include "Card_Where.hpp"

#include "VertexSpec.hpp"
#include "StageSpec.hpp"
#include "OpSpec.hpp"


namespace STORE
{
  class Bench;
  class Dissector;
  class Drafter;
  class Optimiser;
  class Weaver;

  class Board : public Object
  {
  protected:
    eBK          m_eBoard;
    eBT          m_eBoardType;
    Card_Board*  m_pCard_Board;

    Optimiser*   m_pOptimiser;
    Bench*       m_pBench;

    b8           m_bBeenCued;

  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Board(eBK eBoard, eBT eBoardType, Bench* pBench);
   ~Board(void) noexcept(false);

    void vAppendMain(Card* pNewMainCard, bool bRenumber= true);

    eBK eBoard(void);
    CardCtl* pCardCtl(void);
    Card_Board* pCard_Board(void);

    void vDuplicateBoard(Board* pBoard_arg);

    Card_Table* pFindTableCard(char cTableSyn, Card*& rpMainCard);

    void vInsertMainCard_after(Card* pNewMainCard, Card* pAfterMainCard, bool bRenumber=true);

    void vPinCards(Ribbon* pRibbon, Stz* szTransformation=nullptr, Stz* szExplain=nullptr);
  
    void vPrintBoard(Stz* szCue=nullptr, Stz* szExplain=nullptr);

  private:
      Footer* aFooter(void);
  };

  Board* _pBoard(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
