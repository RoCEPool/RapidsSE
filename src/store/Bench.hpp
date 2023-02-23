 /*============================================================================*/
/* Bench.hpp -- Bench                                                         */
/*============================================================================*/
#pragma once

#include "Board.hpp"


namespace STORE
{
  class Flattening;
  class Fold;

  class Bench : public Object
              , public ListExo2_t<Board>
  {
  private:
    Optimiser* m_pOptimiser;

    char m_cPrefix;

    bool m_bIsFlat;

    i8           m_iOdnLo;
    i8           m_iOdnHi;
    char         m_cTableSyn;

    Flattening*  m_pFlattening;
  public:
    CardIter*    m_pCI;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Bench(Optimiser* pOptimiser);
   ~Bench(void) noexcept(false);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // series
    bool bApplyTransformationMatrix(eEC* peErrorCode);
    void vApplyTransformationMatrix(void);

    void vSeries_PrepareSubQueries(void);
    void vSeries_MakeRunnable(void);
    void vSeries_Optimisations(void);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // group

    void vGroup_InitialAdjustments(void);
    void vGroup_CondenseInfo(void);
    void vGroup_MetaAnalysis(void);
    void vGroup_PreliminaryConversions(void);




    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // transform

    void vTransform_MoveOn(void);
    void vTransform_DeleteUnneeded(void);
    void vTransform_ScanBecomesTable(void);
    void vTransform_AddLevel(void);
    void vTransform_NotOps(void);

    void vTransform_TableAnalysis(void);
    void vTransform_IndexAnalysis(void);
    void vTransform_ProjectionAnalysis(void);

    void vTransform_RpnCalcToBoolCalc(void);


    void vTransform_OuterQueryAnalysis(void);




    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    void vAddBoard(Board* pBoard);
    bool bBoardExists(eBK eBoard, i8 i_SubBoard = C_UNKNOWN);
    i8   nBoards(void);
    CardCtl* pCardCtl(void);
    void vClearMarks(void);
    Board* pGetBoard(eBK eBoard, i8 i_Board);
    Board* pGetBoard(i8 i_Board);
    Board* pGetBoard(Stz* szBoardNm);
    bool bIsFlat(void);
    void vIsFlat(bool bIsFlat);
    Optimiser* pOptimiser(void);
    char cPrefix(void);
    void vPrefix(char cPrefix);

    void vEndSeries(eBK eBoard, i8 i_Board=C_UNKNOWN);
    void vSeries(eBK eBoard, i8 i_Board, const char* azHand);



    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -




    // vSeries_80_Index_Scan
   void vS80_T30_Morph_scan_To_indexScan(void);

    // vSeries_90_Stream_Ordering
    void vS90_T30_Derive_order(void);

    void vST00_Begin(void);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  private:
    Footer* aFooter(void);
  };

  Bench* _pBench(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
