/*============================================================================*/
/* ParserException.hpp - exceptions that can occur while parsing a SQL stmt   */
/*============================================================================*/
#pragma once

#include <exception>

namespace Parser
{
  // Base class for exceptions that can be thrown during the sql parsing
  class ParserException : public std::runtime_error
  {
  public:
    // Constructor
    ParserException(const Stz* szErrorMsg)
      : std::runtime_error(const_cast<Stz*>(szErrorMsg)->azChar())    { /* noop */ }

    Stz* szError()    { return (Stz*) this->what(); }
  };

  // An error with the format of the input
  class SyntaxException : public ParserException
  {
  public:
    // Constructor
    SyntaxException(const Stz* szErrorMsg) : ParserException(szErrorMsg)    { /* noop */ }
  };

  // An error with the meaning of the input
  class SemanticException : public ParserException
  {
  public:
    // Constructor
    SemanticException(const Stz* szErrorMsg) : ParserException(szErrorMsg)   { /* noop */ }
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
