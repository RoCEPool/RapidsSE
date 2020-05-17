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
  
  m_azSrc = nullptr;
  m_acChr = nullptr;

  m_pStreamDef = pStreamDef;
  
  m_pFmt  = pFmt;
  
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
  
  char cRecEnd = '\n';
  char cFldSep = ',';

  i8 i_Fld = 0;
  i8 i_Rec = 0;
  
  d8 dCalc = 0;
  
  Column* pCol;
  eCT eCol;
  
  
  bool bTrace = false;
  
  // while more data
  while (*m_acChr) {
    
    // each record
    while (*m_acChr && cRecEnd != *m_acChr) {
      
      pCol = m_pStreamDef->pGetColumn(i_Fld);
      eCol = pCol->eColumnType();
      
      if (bTrace)
        vTrace("[%lld,%lld %10s] "_Stz,i_Rec,i_Fld,aCT[eCol]);
      
      // each field
      while (*m_acChr && cRecEnd != *m_acChr && cFldSep != *m_acChr) {
        
#define CALC ((dCalc * 3.141 + (d8)*m_acChr) / 1.618)
        switch (eCol) {
          case CT_BOOLEAN  : dCalc = (CALC) - 0.0; break;
          case CT_CHAR     : dCalc = (CALC) - 1.0; break;
          case CT_DATE     : dCalc = (CALC) - 2.0; break;
          case CT_DECIMAL  : dCalc = (CALC) - 3.0; break;
          case CT_FLOAT    : dCalc = (CALC) - 4.0; break;
          case CT_INTERVAL : dCalc = (CALC) - 5.0; break;
          case CT_INTEGER  : dCalc = (CALC) - 6.0; break;
          case CT_TIME     : dCalc = (CALC) - 7.0; break;
          case CT_TIMESTAMP: dCalc = (CALC) - 8.0; break;
          case CT_VARCHAR  : dCalc = (CALC) - 9.0; break;
          default:
            RASSERT(0);
        }
        
        if (bTrace)
          vTrace("%c"_Stz,*m_acChr);
        m_acChr++;
      
      } // end-field
      
      if (cFldSep == *m_acChr) {
        if (bTrace)
          vTrace("<fld>\n"_Stz);
        m_acChr++;
      }
  
      i_Fld++;
      
    } // end-record
    
    if (bTrace)
      vTrace("\n-- <rec> -- \n"_Stz);
    
    i_Rec++;
    i_Fld = 0;
    if (*m_acChr)
      m_acChr++;
    
  } // end-data
  
  RASSERT(dCalc != 1.0);
  
  return true;
}
/*----------------------------------------------------------------------------*/
//bool Convertor::bInspect(char* azSrc)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_CONVERTOR);
//
//  //vTrace("[>%s<]"_Stz,azSrc);
//
//  m_azSrc = azSrc;
//
//  m_acChr = m_azSrc;
//
//  char cRecEnd = '\n';
//  char cFldSep = ',';
//
//  while (*m_acChr) {
//
//    do {
//
//      do {
//
//        vTrace("%c"_Stz,*m_acChr++);
//
//      } while (*m_acChr && cRecEnd != *m_acChr && cFldSep != *m_acChr);
//
//    } while (*m_acChr && cRecEnd != *m_acChr);
//  }
//
//  return true;
//}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
