/*============================================================================*/
/* Chard.hpp -- char input stream (with comments removed)                     */
/*============================================================================*/
#pragma once

#include "Common.h"

namespace Common
{
  class Chard : public mn,
                public cn
  {
  private:
    enum eCH { CH_NONE, CH_FILE, CH_STRING };
    
  private:
    bool   m_bMalloc; // was malloc'd?
    eCH    m_eWhich;  // which of file or string
    File   m_xFile;   // file
    s256   m_sString; // string
    
    char*  m_pChar;   // ptr into string
    i8     m_nChars;  // 0: neither has char, 1: m_cOne has char, 2: m_cOne & m_cTwo
    char   m_cOne;    // first single char buffer
    char   m_cTwo;    // second single char buffer
    
    Chard* m_pChain;  // next chard in chain
    
  public:
    void* operator new(size_t, bool bMalloc);
    void operator delete(void* pDirAddress);
    
    Chard(void);
    ~Chard(void);
    
    bool bUseFile(s& sPath);
    void vUseStdin(void);
    void vUseString(s& sPath);

    char cGet(void);
    void vUnget(char c);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/