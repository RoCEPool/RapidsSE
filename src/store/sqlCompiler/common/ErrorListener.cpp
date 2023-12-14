/*============================================================================*/
/* ErrorListener.cpp - Listens for errors when a grammar is parsed.           */
/*============================================================================*/

#include "BASE.h"
#include "ErrorListener.hpp"
#include "../thirdparty/antlr/include/Token.h"

using namespace Parser;


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ErrorListener::ErrorListener()
: m_bGotError(false), m_iLine(-1), m_iColumn(-1), m_sToken(""_Stz), m_sErrorMsg(""_Stz)
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
  UNUSED(recognizer);
  UNUSED(e);

  if (!m_bGotError) {
    m_bGotError = true;
    m_iLine = line;
    m_iColumn = charPositionInLine;
    m_sToken.vCopy( (offendingSymbol == nullptr ? ""_Stz : (Stz*) offendingSymbol->getText().c_str()) );
    m_sErrorMsg.vCopy((Stz*) msg.c_str());
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

/*----------------------------------------------------------------------------*/
/* Return the line number that the error occurred on                          */
/*----------------------------------------------------------------------------*/
i8 ErrorListener::iLine(void)
{
  return m_iLine;
}

/*----------------------------------------------------------------------------*/
/* Return the column number of the line that the error occurred on            */
/*----------------------------------------------------------------------------*/
i8 ErrorListener::iColumn(void)
{
  return m_iColumn;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
