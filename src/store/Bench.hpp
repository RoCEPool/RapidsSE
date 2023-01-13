 /*============================================================================*/
/* Bench.hpp -- Bench                                                         */
/*============================================================================*/
#pragma once

#include "Board.hpp"

namespace STORE
{
  class Bench : public Object
              , public ListExo2_t<Board>
  {
  private:
    Optimiser* m_pOptimiser;

    char m_cPrefix;

    bool m_bIsFlat;

  public:
    CardIter*    m_pCI;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Bench(Optimiser* pOptimiser);
   ~Bench(void) noexcept(false);

    void vAddBoard(Board* pBoard);
    bool bBoardExists(eBK eBoard, i8 i_SubBoard = C_UNKNOWN);
    CardCtl* pCardCtl(void);
    void vClearMarks(void);
    Board* pGetBoard(eBK eBoard, i8 i_Board = C_UNKNOWN);
    Board* pGetBoard(i8 i_Board);
    Board* pGetBoard(Stz* szBoardNm);
    bool bIsFlat(void);
    void vIsFlat(bool bIsFlat);
    Optimiser* pOptimiser(void);
    char cPrefix(void);
    void vPrefix(char cPrefix);

    // vSeries_A0_Make_Weavable
    void vSA0_T00_Begin(void);
    void vSA0_T05_Cull_Nonused_bounds_of_Chosen_index(void);
    void vSA0_T10_Eliminate_bounds_From_filter(void);
    void vSA0_T20_Individuate_filter(void);
    void vSA0_TZZ_End(void);

    // vSeries_E0_To_StmOdn
    void vSE0_T10_Convert_TblColOdn_To_StmOdn(void);
    void vSE0_T20_Convert_tblColOdn_Of_constraint_To_stmColNm(void);

    // vSeries_Z0_Restate_Pipeline
    void vSZ0_T20_Reinstate_stagesV3(void);
    void vSZ0_T30_Add_Partitioning(void);

    void vAssert_Board_any(void);
    void vAssert_Board_mqA(void);
    void vAssert_Board_sqB(void);

    void vST00_Begin(void);
    void vSTZZ_End(void);
  private:
    Footer* aFooter(void);
  };

  Bench* _pBench(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
