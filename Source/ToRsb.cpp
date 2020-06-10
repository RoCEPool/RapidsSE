/*============================================================================*/
/* ToRsb.cpp -- Fast ToRsb of Csv Record                                      */
/*============================================================================*/

#include "Load.h"

#define WITH_CONVERT_FAST_INLINE EndDefine

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
ToRsb::ToRsb(StreamDef* pStreamDef, Format* pFmt, bool bPtnHash)
: Object(CN_TORSB)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TORSB,_FOOTER_);

 
  m_pStreamDef = pStreamDef;
  m_pFmt       = pFmt;
  m_bOrdCsv    = pFmt->bIsOrdCsv();
  vTrace("ordCsv : %lld\n"_Stz,(i8)m_bOrdCsv);
  
  m_azSrc   = nullptr;
  m_acChr   = nullptr;

  m_cRecEnd = '\n';
  m_cFldSep = ',';

  m_iLine   = 1;
  m_iCol    = 1;
  m_i_Fld   = 0;
  m_i_Rec   = 0;
  
  m_pCol    = nullptr;

  m_bPtnHash = bPtnHash;
  
  m_bTrace  = false;
  m_dCalc   = 0.0;
  m_nChars  = 0;
  
  m_pRsbBuffer = nullptr;
  
//  m_pRowSlab = m_pRsbBuffer->pRowSlab();
//  m_pRowSlab->vInitPix(m_pStreamDef);
  m_pRowSlab = nullptr;
  
  m_pFmt->vShow();

  _postcondition_(_ADDRESS_,_MAGIC_,CN_TORSB,_FOOTER_);
  
  vNewRsbBuffer();

  
  s48K sDef;
  m_pStreamDef->vAsString(sDef);
  vTrace("%s\n"_Stz,sDef.szStz());
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ToRsb::~ToRsb(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TORSB,_FOOTER_);

  vTrace("Converted %lld chars\n"_Stz,m_nChars);
  
  delete m_pRsbBuffer;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert one csv chunk (ie multiple records) in one call                    */
/*----------------------------------------------------------------------------*/
bool ToRsb::bConvert(char* azSrc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TORSB,_FOOTER_);

#ifdef WITH_CONVERT_FAST_INLINE
  
  return bConvertFastInline(azSrc);
  
#else
  
  i8 nPtns = AM::nPartitions();
  i8 i_Ptn = 0;
  
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
    
    if (! m_pRowSlab->bNewRow()) {
      vNewRsbBuffer();
      bool b = m_pRowSlab->bNewRow();
      ASSERT(b);
    }
    
    
    while (bMoreFields()) {
    
      eDM eDatum = (eDM) m_pRowSlab->axaColPix()[m_i_Fld].m_uDatum;
    
      if (26 == m_i_Rec && 6 == m_i_Fld)
        vDebugPoint();
      
      vPreField(eDatum);
  
      // currently needed by the SN_ scanners (will go)
      m_pCol = m_pStreamDef->pGetColumn(m_i_Fld);
      
      switch (eDatum) {
        case DM_BOL : vConvertBoolean();   break;
        case DM_DTE : vConvertDate();      break;
        case DM_BCD : vConvertDecimal();   break;
        case DM_FLT : vConvertFloat();     break;
        case DM_INT : vConvertInteger();   break;
        case DM_IVL : vConvertInterval();  break;
        case DM_TME : vConvertTime();      break;
        case DM_TSP : vConvertTimestamp(); break;
        case DM_VCH : vConvertVarchar();   break;
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
    
    // set partition hash CURRENTLY ROUND ROBIN.  DISCUSS W ALKA
    if (m_bPtnHash) {
      m_pRowSlab->vDeposit((i8)i_Ptn);
      if (++i_Ptn >= nPtns)
        i_Ptn = 0;
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
#endif
  
}

/*----------------------------------------------------------------------------*/
/* convert a chunk                                                            */
/*----------------------------------------------------------------------------*/
bool ToRsb::bConvertFast(char* azSrc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TORSB,_FOOTER_);
  
  i8 nPtns = AM::nPartitions();
  i8 i_Ptn = 0;
  
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
    
    if (! m_pRowSlab->bNewRow()) {
      vNewRsbBuffer();
      bool b = m_pRowSlab->bNewRow();
      ASSERT(b);
    }
    
    
    while (bMoreFields()) {
      
      eDM eDatum = (eDM) m_pRowSlab->axaColPix()[m_i_Fld].m_uDatum;
      
      if (26 == m_i_Rec && 6 == m_i_Fld)
        vDebugPoint();
      
      vPreField(eDatum);
      
      // currently needed by the SN_ scanners (will go)
      m_pCol = m_pStreamDef->pGetColumn(m_i_Fld);
      
      switch (eDatum) {
        case DM_BOL : vConvertBoolean();     break;
        case DM_DTE : vConvertDateFast();    break;
        case DM_BCD : vConvertDecimalFast(); break;
        case DM_FLT : vConvertFloat();       break;
        case DM_INT : vConvertIntegerFast(); break;
        case DM_IVL : vConvertInterval();    break;
        case DM_TME : vConvertTime();        break;
        case DM_TSP : vConvertTimestamp();   break;
        case DM_VCH : vConvertVarcharFast(); break;
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
    
    // set partition hash CURRENTLY ROUND ROBIN.  DISCUSS W ALKA
    if (m_bPtnHash) {
      m_pRowSlab->vDeposit((i8)i_Ptn);
      if (++i_Ptn >= nPtns)
        i_Ptn = 0;
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
/* return last remaining dangling rsbBuffer (if any)                          */
/*----------------------------------------------------------------------------*/
RsbBuffer* ToRsb::pDanglingRsbBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TORSB,_FOOTER_);
  
  RsbBuffer* pRsbBuffer = m_pRsbBuffer;
  
  m_pRsbBuffer = nullptr;
  
  return pRsbBuffer;
}

/*----------------------------------------------------------------------------*/
/* return a full rsbBuffer (if any)                                           */
/*----------------------------------------------------------------------------*/
RsbBuffer* ToRsb::pFullRsbBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TORSB,_FOOTER_);

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

/*----------------------------------------------------------------------------*/
/* create a new rsbBuffer                                                     */
/*----------------------------------------------------------------------------*/
void ToRsb::vNewRsbBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TORSB,_FOOTER_);
  
  //  create a new buffer
  RsbBuffer* pRsbBuffer = new RsbBuffer;
  
  // link the new to old
  pRsbBuffer->vNextBuffer(m_pRsbBuffer);
  if (m_pRsbBuffer)
    m_pRsbBuffer->vPrevBuffer(pRsbBuffer);
  m_pRsbBuffer = pRsbBuffer;
  
  // initialise the row slab inside the new rsb buffer
  m_pRowSlab = m_pRsbBuffer->pRowSlab();
  m_pRowSlab->vInitPix(m_pStreamDef,m_bPtnHash);
}

/*----------------------------------------------------------------------------*/
/* get rsb buffer                                                             */
/*----------------------------------------------------------------------------*/
RsbBuffer* ToRsb::pRsbBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TORSB,_FOOTER_);

  return m_pRsbBuffer;
}

/*----------------------------------------------------------------------------*/
/* set rsb buffer                                                             */
/*----------------------------------------------------------------------------*/
void ToRsb::vRsbBuffer(RsbBuffer* pRsbBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TORSB,_FOOTER_);

  m_pRsbBuffer = pRsbBuffer;
}

/*----------------------------------------------------------------------------*/
/*                             -- FAST INLINE --                              */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* fast inline conversion of a chunk                                          */
/*----------------------------------------------------------------------------*/
bool ToRsb::bConvertFastInline(char* azSrc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TORSB,_FOOTER_);
  
  eDM eDatum;
  
  i8 nPtns = AM::nPartitions();
  i8 i_Ptn = 0;
  
  //vTrace("[>%s<]"_Stz,azSrc);
  
  m_azSrc = azSrc;
  m_acChr = m_azSrc;
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // while more csv data
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
L_TestMoreCsv:
  
  if (! bMoreData())
    goto L_EndOfCsv;
  
  vSkipBlankspace();
  
  if (! bMoreFields()) {
    if (m_bTrace)
      vTrace("blank line...\n"_Stz);
    m_acChr++;
    m_iCol++;
    m_iLine++;
    goto L_EndOfCsv;
  }
  
  // each record thats fits
  
  if (! m_pRowSlab->bNewRow()) {
    vNewRsbBuffer();
    bool b = m_pRowSlab->bNewRow();
    ASSERT(b);
  }
  
  
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  // while more fields
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
L_TestMoreFields:
  
  if (! bMoreFields())
    goto L_EndOfFields;

  eDatum = (eDM) m_pRowSlab->axaColPix()[m_i_Fld].m_uDatum;

  vPreField(eDatum);

  m_pCol = m_pStreamDef->pGetColumn(m_i_Fld);

  switch (eDatum) {
    case DM_BOL : vConvertBoolean();     break;
    case DM_DTE : vConvertDateFast();    break;
    case DM_BCD : vConvertDecimalFast(); break;
    case DM_FLT : vConvertFloat();       break;
    case DM_INT : vConvertIntegerFast(); break;
    case DM_IVL : vConvertInterval();    break;
    case DM_TME : vConvertTime();        break;
    case DM_TSP : vConvertTimestamp();   break;
    case DM_VCH : vConvertVarcharFast(); break;
    default:
      RASSERT(0);
  }

  vPostField();

  if (m_cFldSep == *m_acChr) {
    m_acChr++;
    m_iCol++;
  }

  m_i_Fld++;

  goto L_TestMoreFields;
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  
L_EndOfFields:
  
  if (m_bTrace) {
    if (! m_bSawFldSep)
      vTrace("\n"_Stz);
    vTrace("-- recSep -- \n"_Stz);
  }
  
  // set partition hash CURRENTLY ROUND ROBIN.  DISCUSS W ALKA
  if (m_bPtnHash) {
    m_pRowSlab->vDeposit((i8)i_Ptn);
    if (++i_Ptn >= nPtns)
      i_Ptn = 0;
  }
  
  m_i_Rec++;
  m_i_Fld = 0;
  if (*m_acChr)
    m_acChr++;
  m_iLine++;
  m_iCol = 1;
  
  goto L_TestMoreCsv;
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

L_EndOfCsv:
    
  m_pRowSlab->vDumpAllRows();
  m_pRowSlab->vSlabReport();
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
