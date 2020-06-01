/*============================================================================*/
/* Convertor.cpp -- Fast Convertor of Csv Record                              */
/*============================================================================*/

#include "UpperLoad.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(UpperLoad,Convertor,CN_CONVERTOR)
__aNewFunction(Convertor,CN_CONVERTOR)
__vDeleteFunction(Convertor,CN_CONVERTOR)
__aFooterFunction(Convertor)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Convertor::Convertor(StreamDef* pStreamDef, Format* pFmt)
: Object(CN_CONVERTOR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CONVERTOR);
  
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
  m_i_Cell  = C_UNKNOWN;
  
  m_pCol    = nullptr;
  m_eCol    = CT_NONE;

  m_bTrace  = false;
  m_dCalc   = 0.0;
  m_nChars  = 0;
  
  m_pRowSlab = new RowSlab(m_pStreamDef);
  
  m_pFmt->vShow();

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CONVERTOR);
  
  s48K sDef;
   m_pStreamDef->vAsString(sDef);
   vTrace("%s\n"_Stz,sDef.szStz());
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Convertor::~Convertor(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CONVERTOR);
  
  vTrace("Converted %lld chars\n"_Stz,m_nChars);
  
  delete m_pRowSlab;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert a chunck                                                           */
/*----------------------------------------------------------------------------*/
bool Convertor::bConvert(char* azSrc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CONVERTOR);
  
  bInspect(azSrc);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* inspect a chunck                                                           */
/*----------------------------------------------------------------------------*/
bool Convertor::bInspect(char* azSrc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CONVERTOR);
  
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
    
    if (! m_pRowSlab->bNewRow(m_i_Cell))
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
  
  m_pRowSlab->vDumpAllRows();
  m_pRowSlab->vSlabReport();
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
