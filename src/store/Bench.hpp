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
    Board* pGetBoard(eBK eBoard, i8 i_SubBoard = C_UNKNOWN);
    Board* pGetBoard(Stz* szBoardNm);
    Optimiser* pOptimiser(void);
    void vPrintBoards(const char* azFuncNm, eBK eBoard, i8 i_SubBoard = C_UNKNOWN);

  private:
    Footer* aFooter(void);
  };

  Bench* _pBench(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
