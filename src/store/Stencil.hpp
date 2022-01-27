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
  
    eLP         m_ePattern;

    bool        m_bScan;
    bool        m_bIndexScan;

    TypedStack* m_pPostStack;

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

    Shape*     m_pShapeCRW;
    Shape*     m_pShapeROW;
    Shape*     m_pShapeIDX;
    Shape*     m_pShapeEOS;

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
    void vAppendFuncEmpty(CodeSynth* pCodeSynth, Stz* szDesc, Stz* szFunNm, Stz* szStageNm);
    void vAppendFuncHeader0(Stz* szStageNm);
    void vAppendFuncHeader1(Stz* szStageNm);
    void vAppendFuncHeader2(Stz* szStageNm);
    void vAppendFuncHeader3(Stz* szStageNm);
    void vAppendHeaderEntries(i8 iStage, Stz* szStageNm);

    void vGetNextLabel(Stz* sLabelNext, Stream* pStream, i8 i_Op, Stz* sDefault);
    u8   uGetSelectColumns(Stream* pStream);

    void vSetupAggreg(Combined* pCombined, Stream* pStream, Op* pOp);
    void vSetupIndexJoin(Combined* pCombined, Stream* pStream, Op* pOp);
    void vSetupIndexScan(Combined* pCombined, Stream* pStream, Op* pOp);
    void vSetupKeyExists(Combined* pCombined, Stream* pStream, Op* pOp);
    void vSetupOrdAgg(Combined* pCombined, Stream* pStream, Op* pOp);
    void vSetupProjectAgg(Combined* pCombined, Stream* pStream, Op* pOp);
    void vSetupProjectExp(Combined* pCombined, Stream* pStream, Op* pOp);
    void vSetupScan(Combined* pCombined, Stream* pStream, Op* pOp);
    void vSetupSmlAgg(Combined* pCombined, Stream* pStream, Op* pOp);
    void vSetupTop(Combined* pCombined, Stream* pStream, Op* pOp);

    void vSketchEosProjectAgg(Combined* pCombined, Stream* pStream, Op* pOp,
                              Stz* sHashExpr, Stz* sReturnLabel);
    void vSketchEosProjectGby(Combined* pCombined, Stream* pStream, Op* pOp);
    void vSketchEosTop(Combined* pCombined, Stream* pStream, Op* pOp);

    Shape* pSketchFilter(Stream* pStream, Op* pOp, eSR ePrevStencil);
    Shape* pSketchIndexScan(Combined* pCombined, Stream* pStream, Op* pOp);
    Shape* pSketchIndexJoin(Combined* pCombined, Stream* pStream, Op* pOp, eSR ePrevStencil, Stz* sLabel);

    Shape* pSketchInflateCrw(Combined* pCombined, Stream* pStream, Op* pOp);
    Shape* pSketchInflateRow(Stream* pStream, Op* pOp);
    Shape* pSketchKeyExists(Combined* pCombined, Stream* pStream, Op* pOp, eSR ePrevStencil);
    Shape* pSketchLimit(Combined* pCombined, Stream* pStream, Op* pOp, eSR ePrevStencil, Stz* sLabelLoop);
    Shape* pSketchOrdAgg(Combined* pCombined, Stream* pStream, Op* pOp,
                         eSR ePrevStencil, Stz* sLabelLoop);
    Shape* pSketchPrjAgg(Combined* pCombined, Stream* pStream, Op* pOp, eSR ePrevStencil, Stz* sLabelLoop);
    Shape* pSketchPrjExp(Combined* pCombined, Stream* pStream, Op* pOp,
                                      eSR ePrevStencil, Stz* sLabelLoop);
    Shape* pSketchProjectGby(Combined* pCombined, Stream* pStream, Op* pOp,
                             eSR ePrevStencil, Stz* sLabelLoop);
    Shape* pSketchSmlAgg(Combined* pCombined, Stream* pStream, Op* pOp, eSR ePrevStencil, Stz* sLabel, Stz* sLabelLoop);
    Shape* pSketchPot(Combined* pCombined, Op* pLastOp, ePJ eProjectKind, eSR ePrevStencil,
                      IntStack* pAggTypeStack, Stz* sHashExpr, Stz* szLabel, Stz* sLabelLoop, Stz* sReturnLabel);
    Shape* pSketchTub(Combined* pCombined, Op* pLastOp, ePJ eProjectKind, eSR ePrevStencil,
                      IntStack* pAggTypeStack, Stz* szLabel, Stz* sLabelLoop, Stz* sReturnLabel);
    Shape* pSketchTop(Combined* pCombined, Stream* pStream, Op* pOp, eSR ePrevStencil, Stz* sLabelLoop, bool bHasOrdAgg);


    Footer* aFooter(void);
  };
  
  Stencil* _pStencil(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
