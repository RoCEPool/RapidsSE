/*============================================================================*/
/* Formatters.hpp -- set of formatting characters                             */
/*============================================================================*/

#include "DBobjects.h"

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Formatters::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Formatters* pFormatters = (Formatters*)gpBufferPool->pAllocBuffer();
  
  return pPoolCheck<void*>(pFormatters);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Formatters::operator delete(void* pFormattersAddress)
{
  Formatters* pFormatters = static_cast<Formatters*>(pFormattersAddress);
  
  gpBufferPool->vFree((Buffer*)pFormatters);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Formatters::Formatters(void)
: Object(CN_FORMATTERS)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMATTERS);

  m_cEnclosedBy_Leading  = '\'';
  m_cEnclosedBy_Trailing = '\'';
  m_cEscapeChar = '\\';
  m_cFieldSeparator = ',';
  m_cRecordTerminator = '\0';
  
  m_eDateComponent[0] = DC_YYYY;  // default is YYYY=MM-DD
  m_eDateComponent[1] = DC_MM;
  m_eDateComponent[2] = DC_DD;
  m_cDateSeparator = '-';
  
  _checkInitialised_(CN_FORMATTERS);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_FORMATTERS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Formatters::~Formatters(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMATTERS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* reset the formatters to given set of defaults                              */
/*----------------------------------------------------------------------------*/
void Formatters::vReset(Formatters* pDefaults)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMATTERS);
  
  m_cEnclosedBy_Leading  = pDefaults->m_cEnclosedBy_Leading;
  m_cEnclosedBy_Trailing = pDefaults->m_cEnclosedBy_Trailing;
  
  m_cEscapeChar       = pDefaults->m_cEscapeChar;
  m_cFieldSeparator   = pDefaults->m_cFieldSeparator;
  m_cRecordTerminator = pDefaults->m_cRecordTerminator;

  m_eDateComponent[0] = pDefaults->m_eDateComponent[0];
  m_eDateComponent[1] = pDefaults->m_eDateComponent[1];
  m_eDateComponent[2] = pDefaults->m_eDateComponent[2];
  
  m_cDateSeparator    = pDefaults->m_cDateSeparator;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Formatters::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMATTERS);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
