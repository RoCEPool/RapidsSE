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

    eLP  ePattern(void);
    void vPattern(eLP ePattern);
    void vPrint(void);
    void vSetup(Combined* pCombined, Stream* pStream);
    eEC  eSketch(Combined* pCombined, Stream* pStream);

  private:
    void vGetNextLabel(Stz* sLabelNext, Stream* pStream, i8 i_Op, Stz* sDefault);
    u8   uGetSelectColumns(Stream* pStream);

    Shape* pSketchFilter(Stream* pStream, Op* pOp, eSR ePrevStencil);
    Shape* pSketchIndexScan(Combined* pCombined, Stream* pStream, Op* pOp, Stz* szStageNm);
    Shape* pSketchIndexJoin(Combined* pCombined, Stream* pStream, Op* pOp, eSR ePrevStencil, Stz* sLabel, Stz* sLabelLoop);

    Shape* pSketchInflateCrw(Combined* pCombined, Stream* pStream, Op* pOp, Stz* szStageNm);
    Shape* pSketchInflateRow(Op* pOp, Stz* szStageNm);
    Shape* pSketchKeyExists(Combined* pCombined, Stream* pStream, Op* pOp, eSR ePrevStencil);
    Shape* pSketchOrdAgg(Combined* pCombined, Stream* pStream, Op* pOp,
                         eSR ePrevStencil, Stz* sLabelLoop, TypedStack** ppPostStack);
    Shape* pSketchProjectAgg(Combined* pCombined, Stream* pStream, Op* pOp,
                             eSR ePrevStencil, Stz* sLabelLoop, Stz* sReturnLabel);
    Shape* pSketchProjectExp(Combined* pCombined, Stream* pStream, Op* pOp,
                                      eSR ePrevStencil, Stz* sLabelLoop);
    Shape* pSketchProjectGby(Combined* pCombined, Stream* pStream, Op* pOp,
                             eSR ePrevStencil, Stz* sLabelLoop);
    Shape* pSketchSmlAgg(Combined* pCombined, Stream* pStream, Op* pOp, eSR ePrevStencil, Stz* sLabel, Stz* sLabelLoop);
    Shape* pSketchTop(Combined* pCombined, Stream* pStream, Op* pOp, eSR ePrevStencil, Stz* sLabelLoop, bool bHasOrdAgg);

    Footer* aFooter(void);
  };
  
  Stencil* _pStencil(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
