 /*============================================================================*/
/* Bench.hpp -- Bench                                                         */
/*============================================================================*/
#pragma once

#include "Board.hpp"

namespace STORE
{
  class XF_Flatten_subquery;
  class XF_Fold;

  class Bench : public Object
              , public ListExo2_t<Board>
  {
  private:
    Optimiser* m_pOptimiser;

    i8   m_iBenchNo;
    s32  m_sPipelineNm;

    s256 m_sXfile;
    eXF  m_eXform;
    s256 m_sXfunc;

    i8   m_nPrintBoard;

    i8   m_iXform;

  public:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Bench(Optimiser* pOptimiser);
   ~Bench(void) noexcept(false);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    void vAddBoard(Board* pBoard);
    i8   nBoards(void);
    void vDump(const char* azFunc);
    void vEndPhase(CardIter* pCI);

    Board* pFindBoard_By_sqyNm(Stz* szSqyNm);
    Board* pFindBoard_By_sqyNo(Stz* szSqyNo);
    Board* pFindBoard(Symbol* pSymbol_arg);
    SubQuery* pFindSubQuery(Symbol* pSymbol_arg);

    Board* pGetBoard(i8 i_Board);

    bool bKnown_sqy(Symbol* pSymbol_arg);

    Optimiser* pOptimiser(void);

    bool bPastPhase(CardIter* pCI, eOH ePhase);

    void vPhase(CardIter* pCI, eOH ePhase);
    Stz* szPipelineNm(void);
    void vPipelineNm(Stz* szPipelineNm);

    void vTransform(const char* azXfile, const char* azXfunc, eXF eXform);

    Stz* szXfile(void);
    void vXfile(Stz* szXfile);
    eXF  eXform(void);
    void vXform(eXF eXform);
    i8   iXform(void);
    void vXform_Inc(void);
    Stz* szXfunc(void);
    void vXfunc(Stz* szXfunc);

    i8   nPrintBoard(void);
    i8   nPrintBoard_inc(void);
    i8   iBenchNo(void);

    void vValidate(CardIter* pCI);

  private:
    Footer* aFooter(void);
  };

  Bench* _pBench(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
