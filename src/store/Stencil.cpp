/*============================================================================*/
/* Stencil.cpp -- Op Stencil                                                  */
/*============================================================================*/

#include "BASE.h"

#include "Stencil.hpp"
#include "Stream.hpp"
#include "StreamDef.hpp"
#include "Op.hpp"
#include "Fragments.hpp"

#include "LogicUnit.hpp"
#include "TagConstants.h"

#include "Grid.hpp"
#include "Node.hpp"

ShapeRef xaShapeRef[]
{
  { "none"_Stz         }, // LS_NONE
  { "LS_INFLATE_T"_Stz }, // LS_INFLATE_T
  { "LS_FILTER_T"_Stz  }, // LS_FILTER_T
  { "LS_PROJECT_T"_Stz }, // LS_PROJECT_T
  { "LS_JOIN_KEY"_Stz  }, // LS_JOIN_KEY
  { "LS_JOIN"_Stz      }, // LS_JOIN
  { "LS_INFLATE_J"_Stz }, // LS_INFLATE_J
  { "LS_PROJECT_J"_Stz }, // LS_PROJECT_J
  { "LS_AGG_KEY"_Stz   }, // LS_AGG_KEY
  { "LS_AGG"_Stz       }, // LS_AGG
  { "LS_fetch"_Stz     }, // LS_fetch
  { "LS_PROJECT_H"_Stz }, // LS_PROJECT_H
  { "LS_FILTER_H"_Stz  }, // LS_FILTER_H
  { "LS_OUT_H"_Stz     }, // LS_OUT_H
  
  { nullptr            },
};

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Stencil,CN_STENCIL)
__aNewFunction(Stencil,CN_STENCIL)
__vDeleteFunction(Stencil,CN_STENCIL)
__aFooterFunction(Stencil)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Stencil::Stencil(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STENCIL,_FOOTER_);
    
  m_ePattern    = LP_NONE;

  m_pBannerHead  = new CodeSynth;
  m_pBannerTail  = new CodeSynth;
  m_pPreliminary = new CodeSynth;
  m_pConcluding  = new CodeSynth;
  m_pOpSource    = new CodeSynth;

  _checkInitialised_(CN_STENCIL);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_STENCIL,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Stencil::~Stencil(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STENCIL,_FOOTER_);

  delete m_pBannerHead;
  delete m_pBannerTail;
  delete m_pPreliminary;
  delete m_pConcluding;
  delete m_pOpSource;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* repair and delete the stencil                                              */
/*----------------------------------------------------------------------------*/
void Stencil::vRepair(Object* pObject)
{
  _static_

  Stencil* pStencil = _pStencil(pObject);

  // repair the stencil
  // ...
  
  // delete the stencil object
  delete pStencil;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* combine synthesized op code (put code for all shapes together)             */
/*----------------------------------------------------------------------------*/
void Stencil::vCombine(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STENCIL,_FOOTER_);

  i8 iLineNumber = 1;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append banner head
  m_pOpSource->vAppendFragment(m_pBannerHead, &iLineNumber);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append preliminary
  m_pOpSource->vAppendFragment(m_pPreliminary, &iLineNumber);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append shapes
  for(i8 i_Shape = 0; i_Shape < nItems(); i_Shape++) {
    Shape* pShape = pGet(i_Shape);

    if (pShape->pStructSynth())
      m_pOpSource->vAppendFragment(pShape->pStructSynth(), &iLineNumber);

    if (pShape->pCodeSynthA())
      m_pOpSource->vAppendFragment(pShape->pCodeSynthA(), &iLineNumber);

    if (pShape->pCodeSynthB())
      m_pOpSource->vAppendFragment(pShape->pCodeSynthB(), &iLineNumber);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append preliminary
  m_pOpSource->vAppendFragment(m_pConcluding, &iLineNumber);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append preliminary
  m_pOpSource->vAppendFragment(m_pBannerTail, &iLineNumber);

#ifdef WITH_DEBUG_VERSION

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // write to a file

  s256 sFileName;
  LocalFile oFile;

  i8 iQueryNo = AM::iQueryNoInc();
  sFileName.vSprint("../tmp/FlatQuery%lld_%s.txt"_Stz, iQueryNo,
                    AM::pGrid()->pThisNode()->szNodeNm());
  bool bResult = oFile.bOpenWrite(sFileName);
  oFile.vPrint((Stz*)m_pOpSource->azCode());
  oFile.vClose();

#endif


}

/*----------------------------------------------------------------------------*/
/* return op source code                                                      */
/*----------------------------------------------------------------------------*/
Stz* Stencil::szOpSource(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STENCIL,_FOOTER_);
  
  char* azCode = m_pOpSource->azCode();
  
  return (Stz*)azCode;
}

/*----------------------------------------------------------------------------*/
/* get stencil pattern                                                        */
/*----------------------------------------------------------------------------*/
eLP Stencil::ePattern(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STENCIL,_FOOTER_);

  return m_ePattern;
}

/*----------------------------------------------------------------------------*/
/* set stencil pattern                                                        */
/*----------------------------------------------------------------------------*/
void Stencil::vPattern(eLP ePattern)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STENCIL,_FOOTER_);

  m_ePattern = ePattern;
}

/*----------------------------------------------------------------------------*/
/* print the synthesized code                                                 */
/*----------------------------------------------------------------------------*/
void Stencil::vPrint(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STENCIL,_FOOTER_);

  vTrace("Synthesized Code:\n %s\n"_Stz,m_pOpSource->azCode());
}

/*----------------------------------------------------------------------------*/
/* sketch source from pattern                                                 */
/*----------------------------------------------------------------------------*/
void Stencil::vSketch(Tub* pTub, Stream* pStream)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STENCIL,_FOOTER_);

  CodeSynth *pCodeSynth;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append banner head
  Fragments::vAppendBannerHead(m_pBannerHead->azCode());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append preliminary code
  Fragments::vAppendPreliminary(m_pPreliminary->azCode());

  // for each operator in the stream
  for (i8 i_Op = 0; i_Op < pStream->nItems(); i_Op++) {

    // get the op
    Op *pOp = pStream->pGet(i_Op);

    // skip consumeOP
    if (pOp->szOpNm()->bSame("consumeOp"_Stz))
      continue;

    // skip scan OP
    if (pOp->szOpNm()->bSame("scan"_Stz))
      continue;

    // break when we reach the combined OP
    if (pOp->szOpNm()->bSame("combined"_Stz))
      break;

    // create a new shape for this OP
    Shape *pShape = new Shape;

    // get the tags
    TagSet* pOpTags = pOp->pOpTags();

    // get the streamDefOut
    LogicUnit *pLogicUnit = pOp->pOpLogicUnit();
    StreamDef *pStreamDefOut = _pStreamDef(*pLogicUnit->m_xpStreamDefOut);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the filterOP
    if (pOp->szOpNm()->bSame("filter"_Stz)) {

      // get filter expr from OP
      Stz* sFilterExpr = pOpTags->szValue(C_TAG_EXPR);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // generate the code for the streamDef
      pCodeSynth = new CodeSynth;
      pCodeSynth->vSynthStruct(pStreamDefOut, SR_IT);
      pCodeSynth->vPrint();
      pShape->vStructSynth(pCodeSynth);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // generate the code for the row inflate
      pCodeSynth = new CodeSynth;
      pCodeSynth->vSynthInflate(pStreamDefOut);
      pCodeSynth->vPrint();
      pShape->vCodeSynthA(pCodeSynth);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // generate the code for the OP
      pCodeSynth = new CodeSynth;
      pCodeSynth->vSynthFilter(sFilterExpr, SR_IT);
      pCodeSynth->vPrint();
      pShape->vCodeSynthB(pCodeSynth);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the projectOP
    if (pOp->szOpNm()->bSame("project"_Stz) ||
        pOp->szOpNm()->bSame("aggreg"_Stz)) {

      // get the indexDef
      IndexDef *pIndexDef = _pIndexDef(*pLogicUnit->m_xpIndexDef);

      // get the aggregate type stack
      IntStack *pAggTypeStack = _pIntStack(*pLogicUnit->m_xpAggTypeStack);

      // get the project kind
      ePJ eProjectKind = pLogicUnit->m_eProjectKind;

      // get project expr from OP
      Stz* sProjectExpr = pOpTags->szValue(C_TAG_EXPR);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // create rowPix for tub from the streamDefOut of project
      RowPix* pRowPix = pStreamDefOut->pRowPix();
      pTub->vRowPix(pRowPix);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // generate the code for the streamDef
      pCodeSynth = new CodeSynth;
      pCodeSynth->vSynthStruct(pStreamDefOut, SR_PT);
      pCodeSynth->vPrint();
      pShape->vStructSynth(pCodeSynth);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // generate the code for the row projection
      pCodeSynth = new CodeSynth;
      pCodeSynth->vSynthProject(pStreamDefOut, pIndexDef,
                                SR_IT, SR_PT, eProjectKind,
                                pAggTypeStack, sProjectExpr);
      pCodeSynth->vPrint();
      pShape->vCodeSynthA(pCodeSynth);

      // delete the aggregate type stack in the LU of OP
      // not the correct place... will come back to it
      delete pAggTypeStack;
      pLogicUnit->m_xpAggTypeStack = nullptr;
    }

    // add the shape to the stencil
    vAppend(pShape);

  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append concluding code
  Fragments::vAppendConcluding(m_pConcluding->azCode());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append banner tail
  Fragments::vAppendBannerTail(m_pBannerTail->azCode());

}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

//*============================================================================*/
/* end                                                                        */
/*============================================================================*/
