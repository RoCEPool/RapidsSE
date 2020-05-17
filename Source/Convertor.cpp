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

  while (*m_acChr) {
    
    while (*m_acChr && cRecEnd != *m_acChr) {
      while (*m_acChr && cRecEnd != *m_acChr && cFldSep != *m_acChr) {
        vTrace("%c"_Stz,*m_acChr++);
      }
      if (cFldSep == *m_acChr) {
        vTrace("<fld>\n"_Stz);
        m_acChr++;
      }
    }
    vTrace("\n<rec>\n"_Stz);
    if (*m_acChr)
      m_acChr++;
  }
  
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
