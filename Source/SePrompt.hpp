/*============================================================================*/
/* SePrompt.hpp -- se> prompt                                                 */
/*============================================================================*/
#pragma once

#include "Prompt.h"


static Stz* C_DATE_FORMAT          = "date_format"_Stz;
static Stz* C_ENCLOSED_BY_LEADING  = "enclosed_by_leading"_Stz;
static Stz* C_ENCLOSED_BY_TRAILING = "enclosed_by_trailing"_Stz;
static Stz* C_ESCAPE_CHAR          = "escape_char"_Stz;
static Stz* C_FIELD_DELIMITER      = "field_delimiter"_Stz;
static Stz* C_RECORD_ENDS          = "record_ends"_Stz;
static Stz* C_RECORD_TERMINATOR    = "record_terminator"_Stz;
static Stz* C_SAME_MACHINE         = "same_machine"_Stz;

namespace Prompt
{
  class SePrompt : public Object
  {
  private:
    bool   m_bContinue;  // continue processing commands?
    SEitf* m_pSEitf;     // interface into rapidsse
    s48K   m_sResponse;  // response from seitf TO BE DELETED
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    SePrompt(void);
    ~SePrompt(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

    // these are the prompt.xyz task primitives
    bool bExecuteFunction(void);

    // other
    CommandProcessor* pCommandProcessor(void);
    bool bContinue(void);
    void vContinue(bool bContinue);
    void vGetCommand(void);
    SEitf* pSEitf(void);
    TokenVec* pTknVec(void);

  private:

    Footer* aFooter(void);
  
  //----------------------------------------------------------------------------
  // to be removed
    Token* pTkn(i8 i_Token);

  public:
    bool bCmd_Help(void);
  
  private:
    bool bReturnTable(void);
    void vIterateBuffers(void);
    bool bExecuteRequest(void);

  };

  SePrompt* _pSePrompt(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
