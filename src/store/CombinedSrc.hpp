/*============================================================================*/
/* CombinedSrc.hpp -- Combined Source                                         */
/*============================================================================*/
#pragma once

#include "TinyC.hpp"
#include "Tub.hpp"
#include "ePE.h"
#include "ePL.h"
#include "eEK.h"
#include "eFM.h"
#include "IndexDef.hpp"
#include "FunMatrix.h"

namespace STORE {

  static const i8 C_FLATTEN_SOURCE_V2 = C_SIZE_LARGE_OBJECT - C_1KB;

  //----------------------------------------------------------------------------

  class CombinedSrc : public Object {
  private:

    char m_caCode[C_FLATTEN_SOURCE_V2];
    char *m_azCodeCrs;

    TokenVec*   m_pTokenVec;
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
    StreamDef* m_pStreamDefLft;
    StreamDef* m_pStreamDefOut;
    IndexDef*  m_pIndexDef;
    eSR m_eStencilFrom;
    eSR m_eStencilTo;

    i8 m_iLimitRows;

    // must be last
    Footer m_xFooter;

  public:
    void *operator new(size_t);

    void operator delete(void *);

    CombinedSrc(void);

    CombinedSrc(StreamDef *pStreamDefOut, eSR eStencilTo);

    CombinedSrc(StreamDef *pStreamDefLeft, StreamDef *pStreamDefOut, IndexDef* pIndexDef,
              IntStack* pPreExpStack, eSR eStencilFrom, eSR eStencilTo);

    ~CombinedSrc(void) noexcept(false);

    TypedStack *pInitStack(void);

    static void vMakeColumnNm(StreamDef *pStreamDef, Stz *szColumn, eSR eStencilRow, i8 i_Column);
    static void vMakeColumnNm(StreamDef* pStreamDef, s256& sColNm, i8& i_DupCtr, i8 i_Column);
    static void vMakeColumnNmStruct(StreamDef* pStreamDef, s256& sColNm, i8& i_DupCtr, i8 i_Column);
    static void vMakeColumnNmStruct_O(StreamDef* pStreamDef, s256& sColNm, i8& i_DupCtr, i8 i_Column);
    static void vMakeIdxColNmStruct(IndexDef* pIndexDef, s256& sColNm, i8 i_Column);
    static void vMakeIdxColNmStruct_O(IndexDef* pIndexDef, s256& sColNm, i8 i_Column);

    void vAppendFragment(Stz *szFragment);

    void vAppendFragment(Stz *szFragment, i8 *piLineNumber);

    void vAppendFragment(CodeSynth_v2 *pFragment);

    void vAppendFragment(CodeSynth_v2 *pFragment, i8 *piLineNumber);

    char *azCode(void);

    void vPrint(void);

    void vReset(void);


  private:

    Footer *aFooter(void);
  };

  CombinedSrc *_pCombinedSrc(void *aAddr);

}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
