/*============================================================================*/
/* TransformerTool.hpp --                                                       */
/*============================================================================*/
#pragma once

#include "Bench.hpp"
#include "BoardIter.hpp"

#include "eOT.h"

namespace STORE
{
  class TransformerTool : public Object
  {
  protected:

    Bench*       m_pBench;
    Board*       m_pBoard;

    CardIter*    m_pMainCardIter;

    eCK m_eMainCard;
    eCK m_eNextMain;
    eYG m_eStageTopo;

    Card*            m_pMainCard;
    Card*            m_pNextMain;

    Card_AbsJoin*    m_p_absJoin;
    Card_Aggreg*     m_p_aggreg;
    Card_Combined*    m_p_combined;
    Card_Filter*     m_p_filter;
    Card_GbyElist*   m_p_gbyElist;
    Card_IndexJoin*  m_p_indexJoin;
    Card_IndexList*  m_p_indexList;
    Card_IndexScan*  m_p_indexScan;
    Card_LBound*     m_p_lBound;
    Card_Project*    m_p_project;
    Card_SbyElist*   m_p_sbyElist;
    Card_Scan*       m_p_scan;
    Card_ScanPred*   m_p_scanPred;
    Card_Sort*       m_p_sort;
    Card_Stage*      m_p_stage;
    Card_Stage*      m_p_stage_a;
    Card_Stage*      m_p_stage_b;
    Card_StageTopo*  m_p_stageTopo;
    Card_StageTopo*  m_p_stageTopo_a;
    Card_StageTopo*  m_p_stageTopo_b;
    Card_TablePtr*      m_p_table;
    Card_UBound*     m_p_uBound;

    bool m_bInteractive;
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    TransformerTool(eCN eClass, Bench* pBench);
   ~TransformerTool(void) noexcept(false);

    void vBeginIteration(void);
    Board* pBoard(void);
    eCK   eDecodeMain(Card* pCard);
    Card* pEachMain(void);
    Card* pEachMain(eCK eCard);
    void vEndIteration(void);

    Card* pFindMainOf(Card* pSubCard);
    Card* pFindMainOf(Card* pSubCard, Card*& rpParentCard);

    bool bRecursivelyTheSame(Card* p_card_a, Card* p_card_b, i8 iDepth=0, bool bTestL0=false);

    void vTransformation(eOT eTransformation);

  private:

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
