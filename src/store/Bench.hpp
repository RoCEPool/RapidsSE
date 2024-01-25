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
    CardCtl* pCardCtl(void);
    void vDump(void);

    void vEndPhase(CardIter* pCI);

    Board* pGetBoard(i8 i_Board);
    Board* pGetBoard(Stz* szSubPipeNm);
    SubQuery* pGetSubQuery(Stz* szSubQueryNm);
    bool bIsSqyNm(Stz* szSqyNm);
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
