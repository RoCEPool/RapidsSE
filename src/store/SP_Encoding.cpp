/*============================================================================*/
/* SP_Encoding.cpp - Row Encoding2 sprint helper                              */
/*============================================================================*/

#include "BASE.h"

#include "StreamDef.hpp"

#include "RowCoder.hpp"
#include "Encoding.hpp"
#include "SP_Encoding.hpp"

#include "SP_Bcd.hpp"
#include "SP_Bol.hpp"
#include "SP_Dte.hpp"
#include "SP_Flt.hpp"
#include "SP_Int.hpp"
#include "SP_Ivd.hpp"
#include "SP_Ivy.hpp"
#include "SP_Tme.hpp"
#include "SP_Tsp.hpp"
#include "SP_Vch.hpp"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SP_Encoding::SP_Encoding(void)
: Sprint(CN_ROW)
{
  _precondition_(_MAGIC_,CN_ROW,_FOOTER_);
  
  // set default enclosing char
  vFormat(F_VCH_enclose);

#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_caEncoding,sizeof(m_caEncoding),CN_ROW);
#else
  m_caEncoding[0] = '\0';
#endif
  
  _checkInitialised_(CN_ROW);
  
  _postcondition_(_MAGIC_,CN_ROW,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SP_Encoding::~SP_Encoding(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_ROW,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* return Stz* of result                                                      */
/*----------------------------------------------------------------------------*/
void SP_Encoding::vEncoding(Encoding* pEncoding)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  m_caEncoding[0] = '\0';
  StrLib::vSprint(m_caEncoding,
                  sizeof(m_caEncoding),
                  nullptr,
                  "HeadSize %lldB <W=%lld> MetaSize %lldB "
                  "BytesNeeded %lldB ShiftOver %lldB\n"_Stz,
                  pEncoding->m_jHeadSize,
                  MemLib::nQuantaRequired(pEncoding->m_jHeadSize,C_BYTES_PER_WORD),
                  pEncoding->m_jMetaSize,
                  pEncoding->m_jBytesNeeded,
                  pEncoding->m_nShiftOver
                  );

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  char caLine[C_MAX_RECORD];
  const char* azBufLastChar = caLine + sizeof(caLine) - 1;

  for (i8 i_Col = 0; i_Col < pEncoding->m_nColumns; i_Col++) {
  
    char* szCrs = &caLine[0];

    _ColEnc* aColEnc = & pEncoding->xaColEnc[i_Col];
    
    Column* pCol = pEncoding->m_pStreamDef->pGetColumn(i_Col);
    eDM eDatum   = pCol->eDatum();
    Stz* szDatum = (Stz*)aDM[eDatum];
    
    u1 uHigh;
    u1 uLow;
    
    METACODE_SPLIT(aColEnc->m_uMetaCode,uHigh,uLow);
    
    StrLib::vAppendString(&szCrs,azBufLastChar,(char*)" "_Stz);
    StrLib::vAppendPad(&szCrs,caLine, sizeof(caLine),2);
    StrLib::vAppendChar(&szCrs, azBufLastChar, '[');
    StrLib::vAppendDecimal(&szCrs, azBufLastChar, i_Col);
    StrLib::vAppendChar(&szCrs, azBufLastChar, ']');
    
    
    StrLib::vAppendChar(&szCrs, azBufLastChar, aColEnc->m_bSplit ? '*' : ' ');

    StrLib::vAppendPad(&szCrs,caLine, sizeof(caLine),7);
    StrLib::vAppendString(&szCrs,azBufLastChar,(char*)pCol->szColNm());
    
    StrLib::vAppendPad(&szCrs,caLine, sizeof(caLine), 25);
    StrLib::vAppendString(&szCrs,azBufLastChar,(char*)szDatum);
    
    StrLib::vAppendPad(&szCrs,caLine, sizeof(caLine), 30);
    if (aColEnc->m_uMetaCode < 16)
      StrLib::vAppendChar(&szCrs, azBufLastChar, '0');
    StrLib::vAppendHex(&szCrs, azBufLastChar, aColEnc->m_uMetaCode);
    StrLib::vAppendChar(&szCrs, azBufLastChar, ':');
    StrLib::vAppendString(&szCrs,azBufLastChar,(char*)aMC[uHigh]);
    StrLib::vAppendChar(&szCrs, azBufLastChar, '.');
    StrLib::vAppendDecimal(&szCrs, azBufLastChar, uLow);

    StrLib::vAppendPad(&szCrs,caLine, sizeof(caLine), 50);
    StrLib::vAppendString(&szCrs,azBufLastChar,(char*)"C_"_Stz);
    StrLib::vAppendDecimal(&szCrs, azBufLastChar, aColEnc->m_jDataBytes);
    StrLib::vAppendString(&szCrs,azBufLastChar,(char*)"B"_Stz);

    StrLib::vAppendPad(&szCrs,caLine, sizeof(caLine), 60);
    
    if (pEncoding->xaColEnc[i_Col].m_o_DataBreak) {
      StrLib::vAppendDecimal(&szCrs, azBufLastChar, aColEnc->m_o_DataFrom);
      StrLib::vAppendString(&szCrs,azBufLastChar,(char*)".."_Stz);
      StrLib::vAppendDecimal(&szCrs, azBufLastChar, aColEnc->m_o_DataBreak);
    }
    
    StrLib::vAppendPad(&szCrs,caLine, sizeof(caLine), 70);
    StrLib::vAppendDecimal(&szCrs, azBufLastChar, aColEnc->m_i_Tail);

    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    
    StrLib::vAppendPad(&szCrs,caLine, sizeof(caLine), 75);
    
    if (aColEnc->m_wDataCode.bIsNull()) {
      StrLib::vAppendString(&szCrs,azBufLastChar,C_NULL->azChar());
    }
    else {
      switch (eDatum) {
        case DM_BCD: {
          SP_Bcd SP_Bcd;
          StrLib::vAppendString(&szCrs, azBufLastChar,
                                (char*)SP_Bcd.sz(aColEnc->m_wDataCode));
          break;
        }

        case DM_BOL: {
          SP_Bol SP_Bol;
          StrLib::vAppendString(&szCrs, azBufLastChar,
                                (char*)SP_Bol.sz(aColEnc->m_wDataCode));
          break;
        }
          
        case DM_DTE: {
          SP_Dte SP_Dte;
          StrLib::vAppendString(&szCrs, azBufLastChar,
                                (char*)SP_Dte.sz(aColEnc->m_wDataCode));
          break;
        }
          
        case DM_FLT: {
          SP_Flt SP_Flt;
          StrLib::vAppendString(&szCrs, azBufLastChar,
                                (char*)SP_Flt.sz(*(d8*)&aColEnc->m_wDataCode));
          break;
        }
          
        case DM_INT: {
          SP_Int spInt;
          StrLib::vAppendString(&szCrs, azBufLastChar,
                                (char*)spInt.sz(aColEnc->m_wDataCode));
          break;
        }
          
        case DM_IVD: {
          SP_Ivd spIvd;
          StrLib::vAppendString(&szCrs, azBufLastChar,
                                (char*)spIvd.sz(aColEnc->m_wDataCode));
          break;
        }
          
        case DM_IVY: {
          SP_Ivy spIvy;
          StrLib::vAppendString(&szCrs, azBufLastChar,
                                (char*)spIvy.sz(aColEnc->m_wDataCode));
          break;
        }

        case DM_TME: {
          SP_Tme SP_Tme;
          StrLib::vAppendString(&szCrs, azBufLastChar,
                                (char*)SP_Tme.sz(aColEnc->m_wDataCode));
          break;
        }
          
        case DM_TSP: {
          SP_Tsp SP_Tsp;
          StrLib::vAppendString(&szCrs, azBufLastChar,
                                (char*)SP_Tsp.sz(aColEnc->m_wDataCode));
          break;
        }
          
        case DM_VCH: {
          StrLib::vAppendString(&szCrs,azBufLastChar,aColEnc->m_wDataCode.m_azStr);
          break;
        }
          
        case DM_UNK: {
          NEVER_REACH;
          break;
        }

        default:
          NEVER_REACH;
      }
    }
    
    StrLib::vAppendString(&szCrs,azBufLastChar,(char*)"\n"_Stz);
    
    StrLib::vConcat(m_caEncoding, sizeof(m_caEncoding), caLine);
  }
}

/*----------------------------------------------------------------------------*/
/* return Stz* of result                                                      */
/*----------------------------------------------------------------------------*/
Stz* SP_Encoding::sz(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return (Stz*) &m_caEncoding[0];
}

/*----------------------------------------------------------------------------*/
/* sprint Vch and return Stz* to result                                       */
/*----------------------------------------------------------------------------*/
Stz* SP_Encoding::sz(Encoding* pEncoding)
{
  _precondition_(_MAGIC_,_FOOTER_);

  vEncoding(pEncoding);
  
  return (Stz*) &m_caEncoding[0];
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* SP_Encoding::aFooter(void)
{
  _precondition_(_MAGIC_,CN_ROW);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
