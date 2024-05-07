/*============================================================================*/
/* RibbonLine.cpp -- Ribbon Line                                              */
/*============================================================================*/

#include "BASE.h"

#include "RibbonLine.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,RibbonLine,CN_RIBBONLINE)
__aNewFunction(RibbonLine,CN_RIBBONLINE)
__vDeleteFunction(RibbonLine,CN_RIBBONLINE)
__aFooterFunction(RibbonLine)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RibbonLine::RibbonLine(void)
: Object(CN_RIBBONLINE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBONLINE);
  
  m_caText[0] = '\0';
  StrLib::vPad(m_caText, sizeof(m_caText), C_RIBBON_TEXT - 1, ' ');

  _checkInitialised_(CN_RIBBONLINE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_RIBBONLINE);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RibbonLine::~RibbonLine(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBONLINE);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* insert a single char                                                       */
/*----------------------------------------------------------------------------*/
void RibbonLine::vInsert(i8 i_Char, char cChar)
{
  _precondition_(_MAGIC_,CN_RIBBONLINE,_FOOTER_);

  char* psAddr = &m_caText[i_Char];
  i8 iBufRemaining = (i8) sizeof(m_caText) - i_Char;
  StrLib::vInsertChar(psAddr, iBufRemaining, cChar);
}

/*----------------------------------------------------------------------------*/
/* insert some text                                                           */
/*----------------------------------------------------------------------------*/
void RibbonLine::vInsert(i8 i_Char, Stz* szStz)
{
  _precondition_(_MAGIC_,CN_RIBBONLINE,_FOOTER_);

  char* psAddr = &m_caText[i_Char];
  i8 iBufRemaining = (i8) sizeof(m_caText) - i_Char;
  StrLib::vInsertStrTrimRight(psAddr, iBufRemaining, szStz->azChar());
}

/*----------------------------------------------------------------------------*/
/* get margin                                                                 */
/*----------------------------------------------------------------------------*/
Stz* RibbonLine::szMargin(void)
{
  _precondition_(_MAGIC_,CN_RIBBONLINE,_FOOTER_);

  return m_sMargin;
}

/*----------------------------------------------------------------------------*/
/* set margin                                                                 */
/*----------------------------------------------------------------------------*/
void RibbonLine::vMargin(Stz* szMargin)
{
  _precondition_(_MAGIC_,CN_RIBBONLINE,_FOOTER_);

  m_sMargin= szMargin;
}

/*----------------------------------------------------------------------------*/
/* put a single char                                                          */
/*----------------------------------------------------------------------------*/
void RibbonLine::vPut(i8 i_Char, char cChr)
{
  _precondition_(_MAGIC_,CN_RIBBONLINE,_FOOTER_);
  
  StrLib::vPut(m_caText,sizeof(m_caText),i_Char,cChr);
}

/*----------------------------------------------------------------------------*/
/* put some text                                                              */
/*----------------------------------------------------------------------------*/
void RibbonLine::vPut(i8 i_Char, Stz* szStz)
{
  _precondition_(_MAGIC_,CN_RIBBONLINE,_FOOTER_);
  
  StrLib::vPut(m_caText,sizeof(m_caText),i_Char,(char*)szStz);
}

/*----------------------------------------------------------------------------*/
/* repeatedly put some single char                                            */
/*----------------------------------------------------------------------------*/
void RibbonLine::vPut(i8 i_Char, char cChr, i8 nTimes)
{
  _precondition_(_MAGIC_,CN_RIBBONLINE,_FOOTER_);
  
  while (nTimes--)
    StrLib::vPut(m_caText,sizeof(m_caText),i_Char++,cChr);
}

/*----------------------------------------------------------------------------*/
/* sprint at given position                                                   */
/*----------------------------------------------------------------------------*/
void RibbonLine::vSprint(i8 i_Char, Stz* szFormat, ...)
{
  _precondition_(_MAGIC_,CN_RIBBONLINE,_FOOTER_);

  const i8 nBufSize = 1024;
  char zText[1024];

  va_list xArgs;
  va_start(xArgs,szFormat);
  StrLib::vSprint(zText,nBufSize,xArgs,(char*)szFormat);
  va_end(xArgs);

  vPut(i_Char,(Stz*)zText);
}

/*----------------------------------------------------------------------------*/
/* get text                                                                   */
/*----------------------------------------------------------------------------*/
Stz* RibbonLine::szText(void)
{
  _precondition_(_MAGIC_,CN_RIBBONLINE,_FOOTER_);
  
  return (Stz*)&m_caText[0];
}

/*----------------------------------------------------------------------------*/
/* trim the line                                                              */
/*----------------------------------------------------------------------------*/
void RibbonLine::vTrim(void)
{
  _precondition_(_MAGIC_,CN_RIBBONLINE,_FOOTER_);
  
  StrLib::vTrim(m_caText, sizeof(m_caText), ' ');
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
