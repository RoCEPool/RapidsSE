 /*============================================================================*/
/* Bench.hpp -- Bench                                                         */
/*============================================================================*/
#pragma once

#include "Board.hpp"

namespace STORE
{
  class XF_Flatten;
  class XF_Fold;

  class Bench : public Object
              , public ListExo2_t<Board>
  {
  private:
    Optimiser* m_pOptimiser;

    i8   m_iBenchNo;
    s32  m_sPipelineNm;

    i8   m_iXform;
    s32  m_sXhand;
    s256 m_sXfunc;
    s256 m_sXdesc;
    i8   m_iPrint;

    eXF  m_eXform;


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

    Board* pFindBoard_By_sqyNm_v2(Stz* szSqyNm);
    Board* pFindBoard_By_sqyNo_v2(Stz* szSqyNo);
    Board* pFindBoard(Symbol* pSymbol_arg);
     SubQuery* pFindSubQuery_By_sqyNo_v2(Stz* szSqyNo);

    Board* pGetBoard(i8 i_Board);

    bool bKnown_sqyNo_v2(Stz* szSqyNo);

    Optimiser* pOptimiser(void);

    bool bPastPhase(CardIter* pCI, eOH ePhase);

    void vPhase(CardIter* pCI, eOH ePhase);
    Stz* szPipelineNm(void);
    void vPipelineNm(Stz* szPipelineNm);

    void vTransform(const char* azFunc, eXF eXform, Stz* szXdesc);

    Stz* szXdesc(void);
    void vXdesc(Stz* szXdesc);
    eXF  eXform(void);
    void vXform(eXF eXform);
    i8   iXform(void);
    void vXform_Inc(void);
    Stz* szXfunc(void);
    void vXfunc(Stz* szXfunc);
    Stz* szXhand(void);
    void vXhand(const char* asXhand);
    i8   iPrint(void);
    i8   iPrint_inc(void);
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
