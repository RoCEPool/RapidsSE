/*============================================================================*/
/* SqlErrorListener.cpp - Listens for errors when a grammar is parsed.        */
/*============================================================================*/

#include "SqlErrorListener.hpp"
#include "Parser.h"

using namespace Parser;


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SqlErrorListener::SqlErrorListener()
: m_bGotError(false), m_nLine(-1), m_nPos(-1), m_sToken(""_Stz), m_sErrorMsg(""_Stz)
{
  
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SqlErrorListener::~SqlErrorListener() noexcept
{
 
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Called by the parser when it encounters a syntax error                     */
/*----------------------------------------------------------------------------*/
void SqlErrorListener::syntaxError(antlr4::Recognizer *recognizer,
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
bool SqlErrorListener::bGotError(void)
{
  return m_bGotError;
}

/*----------------------------------------------------------------------------*/
/* Return the error message encountered during parsing                        */
/*----------------------------------------------------------------------------*/
Stz* SqlErrorListener::szErrorMsg(void)
{
  return m_sErrorMsg.szStz();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
