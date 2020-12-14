/*============================================================================*/
/* ErrorListener.cpp - Listens for errors when a grammar is parsed.           */
/*============================================================================*/

#include "ErrorListener.hpp"
#include "Parser.h"

using namespace Parser;


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ErrorListener::ErrorListener()
: m_bGotError(false), m_nLine(-1), m_nPos(-1), m_sToken(""_Stz), m_sErrorMsg(""_Stz)
{
  
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ErrorListener::~ErrorListener() noexcept
{
 
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Called by the parser when it encounters a syntax error                     */
/*----------------------------------------------------------------------------*/
void ErrorListener::syntaxError(antlr4::Recognizer *recognizer,
                                antlr4::Token *offendingSymbol,
                                size_t line,
                                size_t charPositionInLine,
                                const std::string &msg,
                                std::exception_ptr e)
{
  if (!m_bGotError) {
    m_bGotError = true;
    m_nLine = line;
    m_nPos = charPositionInLine;
    m_sToken.vCopy((Stz*) offendingSymbol->getText().c_str());
    m_sErrorMsg.vSprint("Syntax error at line:char %ld:%ld near token '%s' : %s"_Stz,
                        line,
                        charPositionInLine, offendingSymbol->getText().c_str(),
                        msg.c_str());
  }
  
}

/*----------------------------------------------------------------------------*/
/* Return true if an error was encountered during parsing                     */
/*----------------------------------------------------------------------------*/
bool ErrorListener::bGotError(void)
{
  return m_bGotError;
}

/*----------------------------------------------------------------------------*/
/* Return the error message encountered during parsing                        */
/*----------------------------------------------------------------------------*/
Stz* ErrorListener::szErrorMsg(void)
{
  return m_sErrorMsg.szStz();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
