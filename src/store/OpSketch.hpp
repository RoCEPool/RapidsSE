/*============================================================================*/
/* OpSketch.hpp -- Stencil Shape                                              */
/*============================================================================*/
#pragma once

#include "eSK.h"
#include "eHK.h"
#include "TinyC.hpp"
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
#include "ColsReq.hpp"

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
    friend class Stencil_v2;
    friend class CodeSynthUtils;

  private:

    i8 m_iOperatorNo;

    eSK  m_eSketchKind;
    eHK  m_eHandlerKind;

    // expression string
    s4K m_sExprString;

    // definitions
    StreamDef* m_pStreamDefLft;
    StreamDef* m_pStreamDefRht;
    StreamDef* m_pStreamDefOut;
    Table*     m_pTable;
    Index*     m_pIndex;
    IndexDef*  m_pIndexDef;

    ColsReq*   m_pColsReq;
    ColsReq*   m_pColsMap;

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
    s256 m_sLabelNext;
    s256 m_sLabelLoop;
    s256 m_sLabelOuterLoop;

    // expression stacks
    TypedStack* m_pExprStack;
    TypedStack* m_pNullStack;
    TypedStack* m_pGbyStack;
    TypedStack* m_pOrdAggStack;
    TypedStack* m_pLowerStack;
    TypedStack* m_pUpperStack;
    TypedStack* m_pInitStack;

    bool m_bLeftOuter;

    eIP m_eLowerIP;
    eIP m_eUpperIP;

    i8 m_nPayloadBytes;

    i8 m_iLimitRows;

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    OpSketch(void);
    OpSketch(i8 iOperatorNo, Stz* sEprStr, eHK eHandlerKind, eSK eSketchKind, eSR eStencilIn, eSR eStencilOut, eII eIndexInfo,
             StreamDef* pStreamDefIn, StreamDef* pStreamDefOut, TypedStack* pInitStack = nullptr);

   ~OpSketch(void) noexcept(false);

    static void vMakeColumnNm(StreamDef *pStreamDef, Stz *szColumn, eSR eStencilRow, i8 i_Column);
    static void vMakeColumnNm(StreamDef* pStreamDef, s256& sColNm, i8& i_DupCtr, i8 i_Column);
    static void vMakeColumnNmStruct(StreamDef* pStreamDef, s256& sColNm, i8& i_DupCtr, i8 i_Column);

    void vCopyColsReq(ColsReq* pColsReq);
    void vCopyColsMap(ColsReq* pColsMap);
    void vCopyInitStack(TypedStack* pInitStack);

    void vGenerateExprStacks(void);

    void vTable(Table* pTable);
    bool bIsPayload(i8 i_Col);
    void vIndex(Index* pIndex);
    void vIndexDef(IndexDef* pIndexDef);
    void vIndexPositionLower(eIP eIndexPosition);
    void vPayloadBytes(i8 nPayloadBytes);
    void vSetLabels(Stz* sLabel, Stz* sLabelNext, Stz* sLabelLoop, Stz* sLabelOuterLoop = ""_Stz);
    void vStreamDefRht(StreamDef* pStreamDefRht);
    eSK eSketchKind(void);

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
