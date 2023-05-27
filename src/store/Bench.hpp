 /*============================================================================*/
/* Bench.hpp -- Bench                                                         */
/*============================================================================*/
#pragma once

#include "Board.hpp"

#define F_PRE_INDIVIDUATE 1

namespace STORE
{
  class MakeFlat;
  class Fold;

  class Bench : public Object
              , public ListExo2_t<Board>
  {
  private:
    Optimiser* m_pOptimiser;

    char m_cPrefix;

    bool m_bIsFlat;

    bool m_bMadeSimpler;
    bool m_bMadeFlat;
    bool m_bMadeOptimal;
    bool m_bMadeWeavable;
    bool m_bMadeCode;


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

    void vAddBoard(Board* pBoard);
    bool bBoardExists(eBK eBoard, i8 i_SubBoard = C_UNKNOWN);
    i8   nBoards(void);
    CardCtl* pCardCtl(void);
    void vClearMarks(void);
    Board* pGetBoard(eBK eBoard, i8 i_Board);
    Board* pGetBoard(i8 i_Board);
    Board* pGetBoard(Stz* szBoardNm);
    SubQuery* pGetSubQuery(Stz* szSubQueryNm);
    bool bIsFlat(void);
    void vIsFlat(bool bIsFlat);
    bool bIsSubQueryNm(Stz* szSubQueryNm);
    Optimiser* pOptimiser(void);
    char cPrefix(void);
    void vPrefix(char cPrefix);

    void vEndSeries(eBK eBoard, i8 i_Board=C_UNKNOWN);
    void vSeries(eBK eBoard, i8 i_Board, const char* azHand);


    bool bMadeSimpler(void);
    void vMadeSimpler(bool bMadeSimpler);
    bool bMadeFlat(void);
    void vMadeFlat(bool bMadeFlat);
    bool bMadeOptimal(void);
    void vMadeOptimal(bool bMadeOptimal);
    bool bMadeWeavable(void);
    void vMadeWeavable(bool bMadeWeavable);
    bool bMadeCode(void);
    void vMadeCode(bool bMadeCode);


  private:
    Footer* aFooter(void);
  };

  Bench* _pBench(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
