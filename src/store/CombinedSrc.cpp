
/*============================================================================*/
/* CombinedSrc.cpp -- Code Synthesizer                                          */
/*============================================================================*/

#include "BASE.h"

#include "Stream.hpp"
#include "StreamDef.hpp"
#include "TokenVec.hpp"
#include "Token.hpp"
#include "LogicUnit.hpp"
#include "Index.hpp"
#include "SN_Dte.hpp"
#include "SN_Tme.hpp"
#include "SN_Tsp.hpp"
#include "SN_Bol.hpp"
#include "SN_Flt.hpp"
#include "SN_Bcd.hpp"
#include "eFM.h"

RowRef xRowRef_[]
{
  { "none"_Stz },   // SR_NONE

  {"SR_AT"_Stz},    // SR_AT
  {"SR_IJ"_Stz},    // SR_IJ
  {"SR_IT"_Stz},    // SR_IT
  {"SR_PL"_Stz},    // SR_PL
  {"SR_PT"_Stz},    // SR_PT
  {"SR_TJ"_Stz},    // SR_TJ
  {"SR_H"_Stz},     // SR_H

  {nullptr}
};

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,CombinedSrc,CN_COMBINEDSRC)
__aNewFunction(CombinedSrc,CN_COMBINEDSRC)
__vDeleteFunction(CombinedSrc,CN_COMBINEDSRC)
__aFooterFunction(CombinedSrc)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CombinedSrc::CombinedSrc(void)
: Object(CN_COMBINEDSRC)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMBINEDSRC,_FOOTER_);
  
#ifdef WITH_DEBUG_VERSION
  MemLib::vSet(m_caCode,0xFF,C_FLATTEN_SOURCE_V2,CN_COMBINEDSRC);
#endif

#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_caCode,sizeof(m_caCode),CN_COMBINEDSRC);
#endif

  m_caCode[0]   = '\0';
  m_azCodeCrs   = m_caCode;
  m_pTokenVec   = nullptr;

  m_pExprStack  = new TypedStack(KK_DATA_STACK);
  m_pNullStack  = new TypedStack(KK_DATA_STACK);
  m_pGbyStack    = new TypedStack(KK_DATA_STACK);
  m_pOrdAggStack = new TypedStack(KK_DATA_STACK);
  m_pLowerStack = new TypedStack(KK_DATA_STACK);
  m_pUpperStack = new TypedStack(KK_DATA_STACK);
  m_pInitStack  = new TypedStack(KK_DATA_STACK);

  m_sLowerPosition = ""_Stz;
  m_sUpperPosition = ""_Stz;

  m_pPreExpStack  = nullptr;
  m_pStreamDefLft = nullptr;
  m_pStreamDefOut = nullptr;
  m_pIndexDef     = nullptr;
  m_eStencilFrom  = SR_NONE;
  m_eStencilTo    = SR_NONE;

  m_iLimitRows    = 0;

  _checkInitialised_(CN_COMBINEDSRC);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_COMBINEDSRC,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CombinedSrc::CombinedSrc(StreamDef* pStreamDefRht, eSR eStencilTo)
    : Object(CN_COMBINEDSRC)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMBINEDSRC,_FOOTER_);

#ifdef WITH_DEBUG_VERSION
  MemLib::vSet(m_caCode,0xFF,C_FLATTEN_SOURCE_V2,CN_COMBINEDSRC);
#endif

#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_caCode,sizeof(m_caCode),CN_COMBINEDSRC);
#endif

  m_caCode[0]   = '\0';
  m_azCodeCrs   = m_caCode;
  m_pTokenVec   = nullptr;

  m_pExprStack  = new TypedStack(KK_DATA_STACK);
  m_pNullStack  = new TypedStack(KK_DATA_STACK);
  m_pGbyStack = new TypedStack(KK_DATA_STACK);
  m_pOrdAggStack = new TypedStack(KK_DATA_STACK);
  m_pLowerStack = new TypedStack(KK_DATA_STACK);
  m_pUpperStack = new TypedStack(KK_DATA_STACK);
  m_pInitStack  = new TypedStack(KK_DATA_STACK);

  m_sLowerPosition = ""_Stz;
  m_sUpperPosition = ""_Stz;

  m_pPreExpStack = nullptr;
  m_pStreamDefLft = nullptr;
  m_pStreamDefOut = pStreamDefRht;
  m_pIndexDef     = nullptr;
  m_eStencilFrom  = SR_NONE;
  m_eStencilTo    = eStencilTo;

  m_iLimitRows    = 0;

  _checkInitialised_(CN_COMBINEDSRC);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_COMBINEDSRC,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CombinedSrc::CombinedSrc(StreamDef* pStreamDefLft, StreamDef* pStreamDefRht, IndexDef* pIndexDef,
                     IntStack* pPreExpStack, eSR eStencilFrom, eSR eStencilTo)
    : Object(CN_COMBINEDSRC)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMBINEDSRC,_FOOTER_);

#ifdef WITH_DEBUG_VERSION
  MemLib::vSet(m_caCode,0xFF,C_FLATTEN_SOURCE_V2,CN_COMBINEDSRC);
#endif

#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_caCode,sizeof(m_caCode),CN_COMBINEDSRC);
#endif

  m_pTokenVec   = nullptr;

  m_pExprStack   = nullptr;
  m_pNullStack   = nullptr;
  m_pGbyStack    = nullptr;
  m_pOrdAggStack = nullptr;
  m_pLowerStack  = nullptr;
  m_pUpperStack  = nullptr;
  m_pInitStack   = nullptr;

  m_sLowerPosition = ""_Stz;
  m_sUpperPosition = ""_Stz;

  m_pPreExpStack  = nullptr;
  m_pStreamDefLft = nullptr;
  m_pStreamDefOut = nullptr;
  m_pIndexDef     = nullptr;

  try {
    m_caCode[0]   = '\0';
    m_azCodeCrs   = m_caCode;
    m_pTokenVec   = nullptr;

    m_pExprStack  = new TypedStack(KK_DATA_STACK);
    m_pNullStack  = new TypedStack(KK_DATA_STACK);
    m_pGbyStack = new TypedStack(KK_DATA_STACK);
    m_pOrdAggStack = new TypedStack(KK_DATA_STACK);
    m_pLowerStack = new TypedStack(KK_DATA_STACK);
    m_pUpperStack = new TypedStack(KK_DATA_STACK);
    m_pInitStack  = new TypedStack(KK_DATA_STACK);

    m_sLowerPosition = ""_Stz;
    m_sUpperPosition = ""_Stz;

    m_pPreExpStack = pPreExpStack;
    m_pStreamDefLft = pStreamDefLft;
    m_pStreamDefOut = pStreamDefRht;
    m_pIndexDef     = pIndexDef;
    m_eStencilFrom  = eStencilFrom;
    m_eStencilTo    = eStencilTo;

    m_iLimitRows    = 0;

    _checkInitialised_(CN_COMBINEDSRC);
  }
  catch (LowMemory xLowMemory) {
    delete m_pTokenVec;

    delete m_pExprStack;
    delete m_pNullStack;
    delete m_pGbyStack;
    delete m_pOrdAggStack;
    delete m_pLowerStack;
    delete m_pUpperStack;
    delete m_pInitStack;

    throw xLowMemory;
  }


  _postcondition_(_ADDRESS_,_MAGIC_,CN_COMBINEDSRC,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
CombinedSrc::~CombinedSrc(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMBINEDSRC,_FOOTER_);

  delete m_pTokenVec;

  delete m_pExprStack;
  delete m_pNullStack;
  delete m_pGbyStack;
  delete m_pOrdAggStack;
  delete m_pLowerStack;
  delete m_pUpperStack;
  delete m_pInitStack;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* form column name from streamDef for given ordinal column                   */
/*----------------------------------------------------------------------------*/
void CombinedSrc::vMakeColumnNm(StreamDef* pStreamDef, Stz* sColumn, eSR eStencilRow, i8 i_Column)
{
  _static_

  // get the column from the streamDef
  Column* pColumn = pStreamDef->pGetColumn(i_Column);

  // convert column name to uppercase
  s256 sColNm;
  sColNm = pColumn->szColNm();
  StrLib::vToUpper((char*)sColNm.azChar());

  // form the column name
  sColumn->vSprint("p%s->%s.m_iAsI8"_Stz, xRowRef_[eStencilRow], sColNm.azChar());
}

/*----------------------------------------------------------------------------*/
/* form column name from streamDef for given ordinal column                   */
/*----------------------------------------------------------------------------*/
void CombinedSrc::vMakeColumnNm(StreamDef* pStreamDef, s256& sColNm, i8& i_DupCtr, i8 i_Column)
{
  _static_

  // get the column from the streamDef
  Column* pColumn = pStreamDef->pGetColumn(i_Column);

  if (pStreamDef->bIsDuplicateName(pColumn))
    sColNm.vSprint("%s%lld"_Stz, pColumn->szColNm(), i_DupCtr++);
  else
    sColNm = pColumn->szColNm();

  StrLib::vToUpper((char *) sColNm.azChar());
}

/*----------------------------------------------------------------------------*/
/* form column name from streamDef for given ordinal column                   */
/*----------------------------------------------------------------------------*/
void CombinedSrc::vMakeColumnNmStruct(StreamDef* pStreamDef, s256& sColNm, i8& i_DupCtr, i8 i_Column)
{
  _static_

  s256 sUpper;

  // get the column from the streamDef
  Column* pColumn = pStreamDef->pGetColumn(i_Column);

  sUpper = pColumn->szColNm();
  StrLib::vToUpper((char *) sUpper.azChar());

  if (pStreamDef->bIsDuplicateName(pColumn))
    sColNm.vSprint("%s%lld.m_iAsI8"_Stz, sUpper.azChar(), i_DupCtr++);
  else
    sColNm.vSprint("%s.m_iAsI8"_Stz, sUpper.azChar());

}

/*----------------------------------------------------------------------------*/
/* form column name from streamDef for given ordinal column                   */
/*----------------------------------------------------------------------------*/
void CombinedSrc::vMakeColumnNmStruct_O(StreamDef* pStreamDef, s256& sColNm, i8& i_DupCtr, i8 i_Column)
{
  _static_

  s256 sUpper;

  // get the column from the streamDef
  Column* pColumn = pStreamDef->pGetColumn(i_Column);

  sUpper = pColumn->szColNm();
  StrLib::vToUpper((char *) sUpper.azChar());

  if (pColumn->eColumnType() == CT_VARCHAR ||
      pColumn->eColumnType() == CT_CHAR) {
    if (pStreamDef->bIsDuplicateName(pColumn))
      sColNm.vSprint("%s%lld.m_iOffset"_Stz, sUpper.azChar(), i_DupCtr++);
    else
      sColNm.vSprint("%s.m_iOffset"_Stz, sUpper.azChar());
  }
  else {
    if (pStreamDef->bIsDuplicateName(pColumn))
      sColNm.vSprint("%s%lld.m_iAsI8"_Stz, sUpper.azChar(), i_DupCtr++);
    else
      sColNm.vSprint("%s.m_iAsI8"_Stz, sUpper.azChar());
  }

}
/*----------------------------------------------------------------------------*/
/* form column name from streamDef for given ordinal column                   */
/*----------------------------------------------------------------------------*/
void CombinedSrc::vMakeIdxColNmStruct(IndexDef* pIndexDef, s256& sColNm, i8 i_Column)
{
  _static_

  s256 sUpper;

  // get the column from the IndexDef
  Column* pColumn = pIndexDef->pGetColumn(i_Column);

  sUpper = pColumn->szColNm();
  StrLib::vToUpper((char *) sUpper.azChar());

  sColNm.vSprint("%s.m_iAsI8"_Stz, sUpper.azChar());

}

/*----------------------------------------------------------------------------*/
/* form column name from streamDef for given ordinal column                   */
/*----------------------------------------------------------------------------*/
void CombinedSrc::vMakeIdxColNmStruct_O(IndexDef* pIndexDef, s256& sColNm, i8 i_Column)
{
  _static_

  s256 sUpper;

  // get the column from the IndexDef
  Column* pColumn = pIndexDef->pGetColumn(i_Column);

  sUpper = pColumn->szColNm();
  StrLib::vToUpper((char *) sUpper.azChar());

  if (pColumn->eColumnType() == CT_VARCHAR ||
      pColumn->eColumnType() == CT_CHAR) {
    sColNm.vSprint("%s.m_iOffset"_Stz, sUpper.azChar());
  }
  else {
    sColNm.vSprint("%s.m_iAsI8"_Stz, sUpper.azChar());
  }

}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append code fragment                                                       */
/*----------------------------------------------------------------------------*/
void CombinedSrc::vAppendFragment(Stz* szFragment)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMBINEDSRC,_FOOTER_);

  StrLib::vAppendString(&m_azCodeCrs, szFragment->azChar());
}

/*----------------------------------------------------------------------------*/
/* append code fragment                                                       */
/*----------------------------------------------------------------------------*/
void CombinedSrc::vAppendFragment(Stz* szFragment, i8* piLineNumber)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMBINEDSRC,_FOOTER_);

#ifdef WITH_DEBUG_VERSION
  StrLib::vAppendString(&m_azCodeCrs, szFragment->azChar(), piLineNumber);
#else
  UNUSED(piLineNumber);
  StrLib::vAppendString(&m_azCodeCrs, szFragment->azChar());
#endif
}

/*----------------------------------------------------------------------------*/
/* append code fragment                                                       */
/*----------------------------------------------------------------------------*/
void CombinedSrc::vAppendFragment(CodeSynth_v2* pFragment)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMBINEDSRC,_FOOTER_);

  StrLib::vAppendString(&m_azCodeCrs, pFragment->azCode());
}

/*----------------------------------------------------------------------------*/
/* append code fragment                                                       */
/*----------------------------------------------------------------------------*/
void CombinedSrc::vAppendFragment(CodeSynth_v2* pFragment, i8* piLineNumber)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMBINEDSRC,_FOOTER_);

#ifdef WITH_DEBUG_VERSION
  StrLib::vAppendString(&m_azCodeCrs, pFragment->azCode(), piLineNumber);
#else
  UNUSED(piLineNumber);
  StrLib::vAppendString(&m_azCodeCrs, pFragment->azCode());
#endif
}

/*----------------------------------------------------------------------------*/
/* get initStack                                                              */
/*----------------------------------------------------------------------------*/
TypedStack* CombinedSrc::pInitStack(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_COMBINEDSRC, _FOOTER_);

  return m_pInitStack;
}

/*----------------------------------------------------------------------------*/
/* print the synthesized code                                                 */
/*----------------------------------------------------------------------------*/
void CombinedSrc::vPrint(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_COMBINEDSRC, _FOOTER_);

  vTrace("%s\n"_Stz,m_caCode);
}


/*----------------------------------------------------------------------------*/
/* return source code                                                         */
/*----------------------------------------------------------------------------*/
  char *CombinedSrc::azCode(void) {
    _precondition_(_ADDRESS_, _MAGIC_, CN_COMBINEDSRC, _FOOTER_);

    return m_caCode;
  }

/*----------------------------------------------------------------------------*/
/* reset                                                                      */
/*----------------------------------------------------------------------------*/
  void CombinedSrc::vReset(void) {
    _precondition_(_ADDRESS_, _MAGIC_, CN_COMBINEDSRC, _FOOTER_);

    MemLib::vZero(m_caCode, sizeof(m_caCode), CN_COMBINEDSRC);
  }

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
