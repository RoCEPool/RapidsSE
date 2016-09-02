/*============================================================================*/
/* InputStream.hpp - Input Stream Handler                                     */
/*============================================================================*/

#pragma once

#include "Tasks.h"

namespace Tasks
{
  class InputParser : public ObjVal
  {
  private:
    Charred* m_pCharred;  // parser
    bool     m_bMalloc;   // malloc or pool heap?
    bool     m_bNormal;   // normal tokens?
    
    char     m_cCurrChar; // current char
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    InputParser(void);
    ~InputParser(void);
    
    void vPush(void);
    bool bPop(void);
    i8   iDepth(void);
    
    void vMalloc(bool bMalloc);
    void vNormal(bool bNormal);
    void vBegin(void);
    
    bool bUseFile(s& sPath);
    void vUseStdin(void);
    void vUseString(s& sString);

    eLT  eNextLiteral(char* wLiteral);
    
  private:
    eLT  eNextNormal(char* wLiteral);
    eLT  eNextSpecial(char* wLiteral);
    
    eLT  eParseNormal(char* wLiteral);
    eLT  eParseNormalInteger(char* wLiteral);
    eLT  eParseNormalName(char* wLiteral);
    eLT  eParseNormalString(char* wLiteral);
    
    eLT  eParseSpecial(char* wLiteral);
    eLT  eParseSpecialInteger(char* wLiteral);
    eLT  eParseSpecialName(char* wLiteral);
    eLT  eParseSpecialString(char* wLiteral);
    
    void vSkipWhitespace(void);
    bool bStartNormalInteger(void);
    bool bStartNormalName(void);
    bool bStartNormalString(void);
    bool bStartSpecialInteger(void);
    bool bStartSpecialName(void);
    bool bStartSpecialString(void);

    void vGetChar(void);
    void vUngetChar(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/