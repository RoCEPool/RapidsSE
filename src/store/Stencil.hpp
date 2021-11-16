/*============================================================================*/
/* Stencil.hpp -- Op Stencil                                                  */
/*============================================================================*/
#pragma once

#include "eLP.h"
#include "eLS.h"
#include "CodeSynth.hpp"
#include "Shape.hpp"
#include "Combined.hpp"

namespace STORE
{
static const i8 C_MAX_SHAPES = 32;

  class Stencil : public VectorNL_t<CN_STENCIL,Shape,C_MAX_SHAPES>
  {
  private:
  
    eLP        m_ePattern;
  
    CodeSynth* m_pBannerHead;
    CodeSynth* m_pBannerTail;
    CodeSynth* m_pPreliminary;
    CodeSynth* m_pPreliminaryEos;
    CodeSynth* m_pConcluding;
    CodeSynth* m_pConcludingEos;
    CodeSynth* m_pOpHeader;
    CodeSynth* m_pOpSource;
    CodeSynth* m_pEmptyFun0;
    CodeSynth* m_pEmptyFun1;
    CodeSynth* m_pEmptyFun2;

    Shape*     m_pEOSShape;

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Stencil(void);
   ~Stencil(void) noexcept(false);

    static void vRepair(Object* pObject);

    void vCombine(void);
  
    Stz* szOpSource(void);

    eLP ePattern(void);
    void vPattern(eLP ePattern);
    void vPrint(void);
    void vSetup(Combined* pCombined, Tub* pTub, Stream* pStream);
    void vSketch(Combined* pCombined, Tub* pTub, Stream* pStream);

  private:
    Shape* pSketchIndexScan(Combined* pCombined, Tub* pTub, Stream* pStream,
                            Op* pOp, Stz* szStageNm);
    Shape* pSketchInflateCrw(Combined* pCombined, Op* pOp, Stz* szStageNm);
    Shape* pSketchInflateRow(Combined* pCombined, Op* pOp, Stz* szStageNm);

    void vGetNextLabel(Stz* sLabelNext, Stream* pStream, i8 i_Op, Stz* sDefault);

    Footer* aFooter(void);
  };
  
  Stencil* _pStencil(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
