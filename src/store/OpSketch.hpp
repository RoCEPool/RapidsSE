/*============================================================================*/
/* OpSketch.hpp -- Stencil Shape                                              */
/*============================================================================*/
#pragma once

#include "eLS.h"
#include "CodeSynth_v2.hpp"

namespace STORE
{
  // shape reference information
  struct ShapeRef_v2
  {
    Stz* m_szInclude;
  };

  // stencil row reference information
  struct RowRef_v2
  {
    Stz* m_szInclude;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  class OpSketch : public Object
  {
  private:
  
    eLS        m_eStencilSlot;

    // definitions
    StreamDef* m_pStreamDefLft;
    StreamDef* m_pStreamDefOut;
    IndexDef*  m_pIndexDef;

    // stencils
    eSR m_eStencilFrom;
    eSR m_eStencilTo;

    // expression stacks
    TypedStack* m_pExprStack;
    TypedStack* m_pNullStack;
    TypedStack* m_pGbyStack;
    TypedStack* m_pOrdAggStack;
    TypedStack* m_pLowerStack;
    TypedStack* m_pUpperStack;
    TypedStack* m_pInitStack;

    s256 m_sLowerPosition;
    s256 m_sUpperPosition;

    IntStack*  m_pPreExpStack;

    i8 m_iLimitRows;

    /////////////////////////////////
    // TO GO
    /////////////////////////////////
    CodeSynth_v2* m_pStructSynthA;
    CodeSynth_v2* m_pStructSynthB;
    CodeSynth_v2* m_pCodeSynthA;
    CodeSynth_v2* m_pCodeSynthB;
    CodeSynth_v2* m_pCodeSynthC;
    /////////////////////////////////

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    OpSketch(void);
   ~OpSketch(void) noexcept(false);
  
    eLS eShape(void);
    void vShape(eLS eShape);
  
    CodeSynth_v2* pStructSynthA(void);
    void vStructSynthA(CodeSynth_v2* pStructSynth);

    CodeSynth_v2* pStructSynthB(void);
    void vStructSynthB(CodeSynth_v2* pStructSynth);

    CodeSynth_v2* pCodeSynthA(void);
    void vCodeSynthA(CodeSynth_v2* pCodeSynthA);

    CodeSynth_v2* pCodeSynthB(void);
    void vCodeSynthB(CodeSynth_v2* pCodeSynthB);

    CodeSynth_v2* pCodeSynthC(void);
    void vCodeSynthC(CodeSynth_v2* pCodeSynthC);

  private:
  
    Footer* aFooter(void);
  };

  OpSketch* _pOpSketch(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
