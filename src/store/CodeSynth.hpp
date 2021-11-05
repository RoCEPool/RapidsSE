/*============================================================================*/
/* CodeSynth.hpp -- Code Synthesizer                                          */
/*============================================================================*/
#pragma once

#include "TinyC.hpp"
#include "Tub.hpp"
#include "eAG.h"
#include "ePJ.h"
#include "IndexDef.hpp"

namespace STORE
{
  static const i8 C_FLATTEN_SOURCE  = C_SIZE_LARGE_OBJECT - C_1KB;

  //----------------------------------------------------------------------------

  class CodeSynth : public Object
  {
  private:
  
    char        m_caCode[C_FLATTEN_SOURCE];
    char*       m_azCodeCrs;

    TokenVec*   m_pTokenVec;
    TypedStack* m_pExprStack;
    TypedStack* m_pLowerStack;
    TypedStack* m_pUpperStack;
    TypedStack* m_pPostExpr;
    TypedStack* m_pInitStack;

    s256        m_sLowerPosition;
    s256        m_sUpperPosition;

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    CodeSynth(void);
   ~CodeSynth(void) noexcept(false);

    void  vAppendFragment(Stz* szFragment, i8* piLineNumber);
    void  vAppendFragment(CodeSynth* pFragment, i8* piLineNumber);
    void  vPrint(void);
    char* azCode(void);
    void  vSynthEos(StreamDef* pStreamDefOut, IndexDef* pIndexDef, eSR eStencilTo, IntStack* pAggTypeStack);
    void  vSynthFilter(StreamDef* pStreamDefLft, Stz* sFilterExpr, eSR eStencilRow, Stz* sLabel, Stz* sLabelNext);
    void  vSynthIndexJoin(StreamDef* pStreamDefOut, StreamDef* pStreamDefLft, StreamDef* pStreamDefRht,
                          eSR eStencilLeft, eSR eStencilRight, eSR eStencilJoin,
                          IndexDef* pIndexDefJ, Stz* sJoinExpr, Stz* sLabel);
    void  vSynthIndexScan(StreamDef* pStreamDefOut, StreamDef* pStreamDefLft, IndexDef* pIndexDef, eSR eStencilLeft,
                          Stz* sIndexScanExpr, Stz* sLabel, Stz* sNextLabel);
    void  vSynthInflate(StreamDef* pStreamDefIn, Stz* sTableNm, Stz* sLabel, Stz* sReturnLabel);
    void  vSynthProject(StreamDef* pStreamDefLft, StreamDef* pStreamDefOut, IndexDef* pIndexDef,
                        eSR eStencilFrom, eSR eStencilTo, ePJ eProjectKind,
                        IntStack* pAggTypeStack, Stz* sProjectExpr, Stz* sLabel);
    void  vSynthRowInflate(Stz* sLabel);
    void  vSynthStruct(StreamDef* pStreamDefOut, eSR eStencilRow);
    void  vReset(void);

  private:
    void vFindKey(StreamDef* pStreamDef, IndexDef* pIndexDef, eSR eStencilFrom, IntStack* pAggTypeStack);
    void vGbyIndex(IndexDef* pIndexDef);
    void vInitAgg(eAG eAggType, eDM eExprType);
    void vPayloadAgg(StreamDef* pStreamDef, IndexDef* pIndexDef, eSR eStencilFrom, IntStack* pAggTypeStack);
    void vPayloadKey(IndexDef* pIndexDef, eSR eStencilFrom);
    void vPayloadStore(StreamDef* pStreamDef, IndexDef* pIndexDef, eSR eStencilFrom);
    void vPayloadToBuffer(StreamDef* pStreamDefOut, eSR eStencilTo, IntStack* pAggTypeStack);
    void vPayloadTraversal(StreamDef* pStreamDefOut, IndexDef* pIndexDef, eSR eStencilTo, IntStack* pAggTypeStack);
    void vProjectAgg(StreamDef* pStreamDefOut, eSR eStencilRow, IntStack* pAggTypeStack, Stz* sLabel);
    void vProjectExp(StreamDef* pStreamDefOut, eSR eStencilRow, IntStack* pAggTypeStack, Stz* sLabel);
    void vProjectGby(StreamDef* pStreamDefOut, IndexDef* pIndexDef, eSR eStencilFrom, eSR eStencilTo,
                     IntStack* pAggTypeStack, Stz* sLabel);
    void vToInfix(StreamDef* pStreamDefLft, char* sExpr, eSR eStencilRow, IntStack* pAggTypeStack, ePJ eProjectKind);

    Footer* aFooter(void);
  };
  
  CodeSynth* _pCodeSynth(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
