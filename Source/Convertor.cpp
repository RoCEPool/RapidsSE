/*============================================================================*/
/* Convertor.cpp -- Fast Convertor of Csv Record                                  */
/*============================================================================*/

#include "Load.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Load,Convertor,CN_CONVERTOR)
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

  m_i_Fld   = 0;
  m_i_Rec   = 0;
   
  m_pCol    = nullptr;
  m_eCol    = CT_NONE;

  m_dCalc   = 0.0;
  m_bTrace  = true;

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
  while (*m_acChr) {
    
    // each record
    while (*m_acChr && m_cRecEnd != *m_acChr) {
      
      m_pCol = m_pStreamDef->pGetColumn(m_i_Fld);
      m_eCol = m_pCol->eColumnType();
      
      if (m_bTrace)
        vTrace("[%lld,%lld %10s] "_Stz,m_i_Rec,m_i_Fld,aCT[m_eCol]);
      
      // each field
      while (*m_acChr && m_cRecEnd != *m_acChr && m_cFldSep != *m_acChr) {
        
        switch (m_eCol) {
          case CT_BOOLEAN  : m_dCalc = (CALC) - 0.0; break;
          case CT_CHAR     : m_dCalc = (CALC) - 1.0; break;
          case CT_DATE     : m_dCalc = (CALC) - 2.0; break;
          case CT_DECIMAL  : m_dCalc = (CALC) - 3.0; break;
          case CT_FLOAT    : m_dCalc = (CALC) - 4.0; break;
          case CT_INTERVAL : m_dCalc = (CALC) - 5.0; break;
          case CT_INTEGER  : m_dCalc = (CALC) - 6.0; break;
          case CT_TIME     : m_dCalc = (CALC) - 7.0; break;
          case CT_TIMESTAMP: m_dCalc = (CALC) - 8.0; break;
          case CT_VARCHAR  : m_dCalc = (CALC) - 9.0; break;
          default:
            RASSERT(0);
        }
        
        if (m_bTrace)
          vTrace("%c"_Stz,*m_acChr);
        m_acChr++;
      
      } // end-field
      
      if (m_cFldSep == *m_acChr) {
        if (m_bTrace)
          vTrace("<fld>\n"_Stz);
        m_acChr++;
      }
  
      m_i_Fld++;
      
    } // end-record
    
    if (m_bTrace)
      vTrace("\n-- <rec> -- \n"_Stz);
    
    m_i_Rec++;
    m_i_Fld = 0;
    if (*m_acChr)
      m_acChr++;
    
  } // end-data
  
  RASSERT(m_dCalc != 1.0);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
