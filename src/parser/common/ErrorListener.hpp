/*============================================================================*/
/* ErrorListener.hpp - Listens for errors when a grammar is parsed.        */
/*============================================================================*/
#pragma once

#include "BaseErrorListener.h"
#include "parser/Parser.h"

namespace Parser
{
  /* Note: Not a proper object */
  class ErrorListener : public antlr4::BaseErrorListener
  {
   private:
    bool m_bGotError;
    i8   m_nLine;
    i8   m_nPos;
    s256 m_sToken;
    s256 m_sErrorMsg;
    
   public:
    ErrorListener();
    virtual ~ErrorListener() noexcept;
    
    virtual void syntaxError(antlr4::Recognizer *recognizer,
                             antlr4::Token * offendingSymbol,
                             size_t line,
                             size_t charPositionInLine,
                             const std::string &msg,
                             std::exception_ptr e) override;
    
    bool bGotError(void);
    Stz* szErrorMsg(void);
  };
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
