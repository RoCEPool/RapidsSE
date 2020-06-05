/*============================================================================*/
/* ToRsb.cpp -- Fast ToRsb of Csv Record                                      */
/*============================================================================*/

#include "Load.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Load,ToRsb,CN_TORSB)
__aNewFunction(ToRsb,CN_TORSB)
__vDeleteFunction(ToRsb,CN_TORSB)
__aFooterFunction(ToRsb)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ToRsb::ToRsb(StreamDef* pStreamDef, Format* pFmt)
: Object(CN_TORSB)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TORSB);
  
  m_pStreamDef = pStreamDef;
  m_pFmt       = pFmt;
  
  m_azSrc   = nullptr;
  m_acChr   = nullptr;

  m_cRecEnd = '\n';
  m_cFldSep = ',';

  m_iLine   = 1;
  m_iCol    = 1;
  m_i_Fld   = 0;
  m_i_Rec   = 0;
  
  m_pCol    = nullptr;
  m_eCol    = CT_NONE;

  m_bTrace  = false;
  m_dCalc   = 0.0;
  m_nChars  = 0;
  
  m_pRsbBuffer = new RsbBuffer;
  
  m_pRowSlab = m_pRsbBuffer->pRowSlab();
  m_pRowSlab->vInitPix(pStreamDef);

  m_pFmt->vShow();

  _postcondition_(_ADDRESS_,_MAGIC_,CN_TORSB);
  
  s48K sDef;
   m_pStreamDef->vAsString(sDef);
   vTrace("%s\n"_Stz,sDef.szStz());
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ToRsb::~ToRsb(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TORSB);
  
  vTrace("Converted %lld chars\n"_Stz,m_nChars);
  
  delete m_pRsbBuffer;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert a chunck                                                           */
/*----------------------------------------------------------------------------*/
bool ToRsb::bConvert(char* azSrc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TORSB);
  
  bInspect(azSrc);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* inspect a chunck                                                           */
/*----------------------------------------------------------------------------*/
bool ToRsb::bInspect(char* azSrc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TORSB);
  
  //vTrace("[>%s<]"_Stz,azSrc);
  
  m_azSrc = azSrc;
  
  m_acChr = m_azSrc;
    
  // while more data
  while (bMoreData()) {
    
    vSkipBlankspace();
    
    if (! bMoreFields()) {
      if (m_bTrace)
        vTrace("blank line...\n"_Stz);
      m_acChr++;
      m_iCol++;
      m_iLine++;
      continue;
    }
    
    // each record thats fits
    
    if (! m_pRowSlab->bNewRow())
      break;
    
    while (bMoreFields()) {
      
      m_pCol = m_pStreamDef->pGetColumn(m_i_Fld);
      m_eCol = m_pCol->eColumnType();
      m_bSawFldSep = false;

      if (26 == m_i_Rec && 6 == m_i_Fld)
        vDebugPoint();
      
      vPreField();
      
      switch (m_eCol) {
        case CT_BOOLEAN  : vConvertBoolean();   break;
        case CT_CHAR     : vConvertVarchar();   break;
        case CT_DATE     : vConvertDate();      break;
        case CT_DECIMAL  : vConvertDecimal();   break;
        case CT_FLOAT    : vConvertFloat();     break;
        case CT_INTEGER  : vConvertInteger();   break;
        case CT_INTERVAL : vConvertInterval();  break;
        case CT_TIME     : vConvertTime();      break;
        case CT_TIMESTAMP: vConvertTimestamp(); break;
        case CT_VARCHAR  : vConvertVarchar();   break;
        default:
          RASSERT(0);
      }
      
      vPostField();

      if (m_cFldSep == *m_acChr) {
        m_acChr++;
        m_iCol++;
      }
  
      m_i_Fld++;
      
    } // end-record
    
    if (m_bTrace) {
      if (! m_bSawFldSep)
        vTrace("\n"_Stz);
      vTrace("-- recSep -- \n"_Stz);
    }
    
    m_i_Rec++;
    m_i_Fld = 0;
    if (*m_acChr)
      m_acChr++;
    m_iLine++;
    m_iCol = 1;

  } // end-data
  
  RASSERT(m_dCalc != 1.0);
  
  //m_pRowSlab->vDumpAllRows();
  //m_pRowSlab->vSlabReport();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get rsb buffer                                                             */
/*----------------------------------------------------------------------------*/
RsbBuffer* ToRsb::pRsbBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TORSB);
  
  return m_pRsbBuffer;
}

/*----------------------------------------------------------------------------*/
/* set rsb buffer                                                             */
/*----------------------------------------------------------------------------*/
void ToRsb::vRsbBuffer(RsbBuffer* pRsbBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TORSB);
  
  m_pRsbBuffer = pRsbBuffer;
}

/*----------------------------------------------------------------------------*/
/* return last remaining dangling rsbBuffer (if any)                          */
/*----------------------------------------------------------------------------*/
RsbBuffer* ToRsb::pDanglingRsbBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWHASH,_FOOTER_);
  
  RsbBuffer* pRsbBuffer = m_pRsbBuffer;
  
  m_pRsbBuffer = nullptr;
  
  return pRsbBuffer;
}

/*----------------------------------------------------------------------------*/
/* return a full rsbBuffer (if any)                                           */
/*----------------------------------------------------------------------------*/
RsbBuffer* ToRsb::pFullRsbBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWHASH,_FOOTER_);

  RsbBuffer* pFullRsbBuffer = nullptr;
  
  RsbBuffer* pRsbBuffer = m_pRsbBuffer ?
                            _pRsbBuffer(m_pRsbBuffer->pNextBuffer()) : nullptr;
  while (pRsbBuffer) {
    pFullRsbBuffer = pRsbBuffer;
    pRsbBuffer = _pRsbBuffer(pRsbBuffer->pNextBuffer());
  }
  
  if (pFullRsbBuffer) {
    RsbBuffer* pPrevBuffer = _pRsbBuffer(pFullRsbBuffer->pPrevBuffer());
    pPrevBuffer->vNextBuffer(nullptr);
  }
  
  return pFullRsbBuffer;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
