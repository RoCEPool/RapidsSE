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
  protected:
    // Constructor
    ParserException(const Stz* szErrorMsg)
      : std::runtime_error(const_cast<Stz*>(szErrorMsg)->azChar())    { /* noop */ }

  public:
    Stz* szError()    { return (Stz*) this->what(); }
  };

  // An error with the format of the input
  class SyntaxException : public ParserException
  {
  public:
    // Constructor
    SyntaxException(const Stz* szErrorMsg) : ParserException(szErrorMsg)    { /* noop */ }

    // Creates and throws a SyntaxException
    static void vThrow(Stz* szFormat, ...) {
      s256 sErrMsg;
      va_list vaArgs;

      va_start(vaArgs,szFormat);
      sErrMsg.vSprint(vaArgs, szFormat);
      va_end(vaArgs);

      throw new SyntaxException(sErrMsg.szStz());
    }
  };

  // An error with the meaning of the input
  class SemanticException : public ParserException
  {
  public:
    // Constructor
    SemanticException(const Stz* szErrorMsg) : ParserException(szErrorMsg)   { /* noop */ }

    // Creates and throws a SemanticException
    static void vThrow(Stz* szFormat, ...) {
      s256 sErrMsg;
      va_list vaArgs;

      va_start(vaArgs,szFormat);
      sErrMsg.vSprint(vaArgs, szFormat);
      va_end(vaArgs);

      throw new SemanticException(sErrMsg.szStz());
    }
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
