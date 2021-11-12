/*============================================================================*/
/* Dissect.hpp -- Dissect                                                     */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Board.hpp"

#include "Card.hpp"
#include "Card_Aggreg.hpp"
#include "Card_AggregList.hpp"
#include "Card_AndPred.hpp"
#include "Card_Atom.hpp"
#include "Card_By.hpp"
#include "Card_ByList.hpp"
#include "Card_ComparePred.hpp"
#include "Card_CorrSubquery.hpp"
#include "Card_CreateTable.hpp"
#include "Card_DropTable.hpp"
#include "Card_Expr.hpp"
#include "Card_ExprList.hpp"
#include "Card_Filter.hpp"
#include "Card_FilterList.hpp"
#include "Card_HashCast.hpp"
#include "Card_Having.hpp"
#include "Card_HavingList.hpp"
#include "Card_IndexJoin.hpp"
#include "Card_IndexJoinList.hpp"
#include "Card_IndexScan.hpp"
#include "Card_IndexScanList.hpp"
#include "Card_Limit.hpp"
#include "Card_MainSubquery.hpp"
#include "Card_NodeCast.hpp"
#include "Card_NoncSubquery.hpp"
#include "Card_On.hpp"
#include "Card_OnList.hpp"
#include "Card_OriginCast.hpp"
#include "Card_OrPred.hpp"
#include "Card_Project.hpp"
#include "Card_ProjectList.hpp"
#include "Card_Scan.hpp"
#include "Card_Sort.hpp"
#include "Card_SortList.hpp"
#include "Card_StreamDef.hpp"
#include "Card_Table.hpp"
#include "Card_TableList.hpp"

#include "PipelineSpec.hpp"
#include "StageSpec.hpp"
#include "OpSpec.hpp"

#include "ShardDef.hpp"

#include "Eval_Filter.hpp"

#include "Ribbon.hpp"

namespace STORE
{
  class Dissect : public Object
  {
  protected:
  
    Board* m_pBoard;
  
    PipelineSpec* m_pPipelineSpec;
    StageSpec*    m_pStageSpec;
    OpSpec*       m_pOpSpec;

    bool m_bSeenAgg;

  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Dissect(Board* pBoard);
   ~Dissect(void) noexcept(false);
  
    void vDissectSpec(PipelineSpec* pPipelineSpec);
    void vDissectStage(void);
    void vDissectOp(void);

    void vDissectAggreg(OpSpec* pOpSpec);
    void vDissectConsume(OpSpec* pOpSpec);
    void vDissectFilter(OpSpec* pOpSpec);
    void vDissectIndexJoin(OpSpec* pOpSpec);
    void vDissectIndexScan(OpSpec* pOpSpec);
    void vDissectLimit(OpSpec* pOpSpec);
    void vDissectOutput(OpSpec* pOpSpec);
    void vDissectPulse(OpSpec* pOpSpec);
    void vDissectProject(OpSpec* pOpSpec);
    void vDissectScan(OpSpec* pOpSpec);
    void vDissectSend(OpSpec* pOpSpec);
    void vDissectSink(OpSpec* pOpSpec);
    void vDissectSort(OpSpec* pOpSpec);
    void vDissectStart(OpSpec* pOpSpec);

    Card_Expr* pAtomiseExpr(Stz* szExpr);

    void vAnalyseAggregExpr(Stz* szExpr);
    void vAnalyseFilterExpr(Stz* szExpr);
    void vAnalyseProjectExpr(Stz* szExpr);
    void vSeparateAtoms(Card* pCard, Stz* szExpr);

    void vAddBy(Column* pColumn, Table* pTable);
    void vAddLimit(i8 nRows);
    void vAddOn(Column* pColumn1, Table* pTable1,
                Column* pColumn2, Table* pTable2);
    void vAddSort(Column* pColumn, Table* pTable);
    Card_Table* pAddTable(Table* pTable);

    Index* pFindIndex(Table* pTable, Stz* szTableNm);
    Table* pFindTable(Stz* szTableNm);

    void vEvaluateFilter(Card_Filter* pCard_Filter);

  private:
    Footer* aFooter(void);
  };

  Dissect* _pDissect(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
