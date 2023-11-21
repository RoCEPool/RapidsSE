/*============================================================================*/
/* OpSketch.hpp -- Stencil Shape                                              */
/*============================================================================*/
#pragma once

#include "eSK.h"
#include "eHK.h"
#include "eSR.h"
#include "Tub.hpp"
#include "ePE.h"
#include "ePL.h"
#include "eEK.h"
#include "eFM.h"
#include "eIP.h"
#include "eII.h"
#include "eIV.h"
#include "IndexDef.hpp"
#include "FunMatrix.h"
#include "ColInj.hpp"

#include "ColMap.hpp"
#include "StageDef.hpp"

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
    friend class Stencil;
    friend class CodeSynthUtils;

  private:

    i8   m_iStageNo;
    s256 m_sStageNm;

    i8 m_iOperatorNo;

    eSK  m_eSketchKind;
    eHK  m_eHandlerKind;

    // expression string
    s4K m_sExprString;

    s4K m_sLimitExpr;
    i8  m_iTopCount;

    // definitions
    StreamDef* m_pStreamDefLft;
    StreamDef* m_pStreamDefRht;
    StreamDef* m_pStreamDefOut;
    Table*     m_pTable;
    Index*     m_pIndex;
    IndexDef*  m_pIndexDef;

		ColHsh*    m_pColHsh;
		ColInj*    m_pColInj;
    ColMap*    m_pColMap;
		ColPpg*    m_pColPpg;

		KeyMap*    m_pKeyMap;


    // stencils
    eSR m_eStencilFrom;
    eSR m_eStencilTo;

    // stencil sketch
    s4K m_sStencilTo;
    s4K m_sStencilFrom;

    // index information
    eII m_eIndexInfo;

    // labels
    s256 m_sLabel;
		s256 m_sLabelLoop;
    s256 m_sLabelNext;
		s256 m_sLabelOrdAgg;
    s256 m_sLabelOuterLoop;

    // expression stacks
    TypedStack* m_pConstraintStack;
    TypedStack* m_pExprStack;
    TypedStack* m_pNullStack;
    TypedStack* m_pGbyStack;
    TypedStack* m_pOrdAggStack;
    TypedStack* m_pLowerStack;
    TypedStack* m_pUpperStack;
    TypedStack* m_pInitStack;

    bool m_bLeftOuter;
    bool m_bCrossJoin;

    eIP m_eLowerIP;
    eIP m_eUpperIP;

    i8 m_nPayloadBytes;

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    OpSketch(StageDef* pStageDef, i8 iOperatorNo, Stz* sEprStr, eHK eHandlerKind, eSK eSketchKind, eSR eStencilIn,
             eSR eStencilOut, eII eIndexInfo, TypedStack* pInitStack = nullptr);

   ~OpSketch(void) noexcept(false);

		void vCopyColHsh(ColHsh* pColHsh);
		void vCopyColInj(ColInj* pColsInj);
    void vCopyColMap(ColMap* pColMap);
		void vCopyColPpg(ColPpg* pColPpg);
		void vCopyKeyMap(KeyMap* pKeyMap);

		void vCopyConstraintStack(TypedStack* pConstraintStack);
    void vCopyInitStack(TypedStack* pInitStack);

    void vGenerateExprStacks(Stz* sExpr, bool bConstraint = false);

    void vTable(Table* pTable);
		bool bIsEmittedColPayload(i8 i_emittedCol);
    bool bIsPayload(i8 i_Col);
    void vIndex(Index* pIndex);
    void vIndexDef(IndexDef* pIndexDef);
    void vIndexPositionLower(eIP eIndexPosition);
    void vLimitExpr(Stz* sLimitExpr);
    void vPayloadBytes(i8 nPayloadBytes);
    void vSetLabels(Stz* sLabel, Stz* sLabelNext, Stz* sLabelLoop, Stz* sLabelOuterLoop = ""_Stz);
    void vStreamDefs(StreamDef* pStreamDefLft, StreamDef* pStreamDefRht, StreamDef* pStreamDefOut);
    eSK  eSketchKind(void);
    void vTopCount(i8 iTopCount);

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
                     i8 i_Col, i8 iColPL);
    void vPushAssignCol(TypedStack* pExprStack, ePE ePreExp, i8 i_Col, i8 i_ColPL, bool bConstraint = false);
    void vPushArithmetic(TypedStack *pExprStack, IntStack *pTypeStack, Token *pTkn);
    void vPushCast(TypedStack *pExprStack, IntStack *pTypeStack, eDM eCastType);
    void vPushColumn(TypedStack *pExprStack, IntStack *pTypeStack, StreamDef* pStreamDef, Token *pTkn, bool bConstaint);
    void vPushCount(TypedStack *pExprStack, IntStack *pExprType, Token *pTkn);
    void vPushFunc(TypedStack* pExprStack, IntStack* pTypeStack, ePE ePreExp);
    void vPushLiteral(TypedStack *pExprStack, IntStack *pTypeStack, Token *pTkn, ePE ePreExp, eDM eLiteral);
    void vPushLogical(TypedStack *pExprStack, IntStack *pTypeStack, Token *pTkn);
    void vPushOrdCol(TypedStack *pExprStack, IntStack* pTypeStack, ePE ePreExp, Token *pTkn, bool bConstaint);
    void vPushRelational(TypedStack *pExprStack, IntStack *pTypeStack, Token *pTkn);

    TypedStack* pWorkingStack(ePE ePreExp, bool bConstraint);

  private:
  
    Footer* aFooter(void);
  };

  OpSketch* _pOpSketch(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
