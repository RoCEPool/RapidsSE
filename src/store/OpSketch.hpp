/*============================================================================*/
/* OpSketch.hpp -- Stencil Shape                                              */
/*============================================================================*/
#pragma once

#include "eSK.h"
#include "CodeSynth_v2.hpp"

namespace STORE
{

  // stencil row reference information
  struct StencilRow
  {
    Stz* m_szInclude;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  class OpSketch : public Object
  {

    friend class CombinedSrc;

  private:

    eSK  m_eSketchKind;

    // expression string
    s256 m_sExprString;

    // definitions
    StreamDef* m_pStreamDefLft;
    StreamDef* m_pStreamDefOut;
    IndexDef*  m_pIndexDef;

    // stencils
    eSR m_eStencilFrom;
    eSR m_eStencilTo;

    // stencil sketch
    s256 m_sStencil;

    // labels
    s256 m_sLabel;
    s256 m_sLabelNext;
    s256 m_sLabelLoop;

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
    OpSketch(Stz* sEprStr, eSK eSketchKind, eSR eStencilIn, eSR eStencilOut,
             StreamDef* pStreamDefIn, StreamDef* pStreamDefOut);

   ~OpSketch(void) noexcept(false);

    static void vMakeColumnNm(StreamDef *pStreamDef, Stz *szColumn, eSR eStencilRow, i8 i_Column);
    static void vMakeColumnNm(StreamDef* pStreamDef, s256& sColNm, i8& i_DupCtr, i8 i_Column);
    static void vMakeColumnNmStruct(StreamDef* pStreamDef, s256& sColNm, i8& i_DupCtr, i8 i_Column);

    void vGenerateExprStacks(IntStack* pPreExpStack);
    void vGenerateStencilStruct(void);

    eSK eSketchKind(void);

    void vSetLabels(Stz* sLabel, Stz* sLabelNext, Stz* sLabelLoop);

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

    void vGetValue(eDM eDatum, char *sValue, i8& iValue, d8& dValue);
    void vInitAgg(ePE ePreExp);

    void vPayloadAgg(void);

    void vPayloadCreate(void);
    void vPayloadInsert(ePL ePayload, i8 iTopCount);
    void vPayloadKey(TypedStack* pExprStack);
    void vPayloadPopulate(ePL ePayload);
    void vPayloadToBuffer(IndexDef *pIndexDef, TypedStack* pPostStack);
    void vPayloadTraversal(IndexDef *pIndexDef, TypedStack* pPostStack, Stz* sLabelNext);

    void vPushAggCol(TypedStack *pExprStack, ePE ePreExp, eDM eExprType,
                     i8 i_Col, i8 iColPL, i8& i_DupCtr);
    void vPushAssignCol(TypedStack* pExprStack, ePE ePreExp, i8 i_Col, i8 i_ColPL, i8& i_DupCtr);
    void vPushArithmetic(TypedStack *pExprStack, IntStack *pTypeStack, Token *pTkn);
    void vPushCast(TypedStack *pExprStack, IntStack *pTypeStack, eDM eCastType);
    void vPushColumn(TypedStack *pExprStack, IntStack *pTypeStack, StreamDef* pStreamDef, Token *pTkn);
    void vPushCount(TypedStack *pExprStack, IntStack *pExprType, Token *pTkn);
    void vPushFunc(TypedStack* pExprStack, IntStack* pTypeStack, ePE ePreExp);
    void vPushLiteral(TypedStack *pExprStack, IntStack *pTypeStack, Token *pTkn, ePE ePreExp, eDM eLiteral);
    void vPushLogical(TypedStack *pExprStack, IntStack *pTypeStack, Token *pTkn);
    void vPushOrdCol(TypedStack *pExprStack, IntStack* pTypeStack, ePE ePreExp, Token *pTkn);
    void vPushRelational(TypedStack *pExprStack, IntStack *pTypeStack, Token *pTkn);

    TypedStack* pWorkingStack(ePE ePreExp);

  private:
  
    Footer* aFooter(void);
  };

  OpSketch* _pOpSketch(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
