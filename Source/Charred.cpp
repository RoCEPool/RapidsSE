/*============================================================================*/
/* Charred.cpp -- input processor, stripping out line and block comments      */
/*============================================================================*/

#include "Common.h"
#include "StorageEngine.h"

extern Heap* gpSmallHeap;

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* Charred::operator new(size_t, bool bMalloc)
{
  Charred* pChard;
  
  if (bMalloc)
    pChard = reinterpret_cast<Charred*>(malloc(sizeof(Charred)));
  else
    pChard = reinterpret_cast<Charred*>(gpSmallHeap->pAllocObject());
  
  pChard->m_bMalloc = bMalloc;
  
  return pChard;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void Charred::operator delete(void* pChardAddress)
{
  Charred* pChard = reinterpret_cast<Charred*>(pChardAddress);
  
  if (pChard->m_bMalloc)
    free(pChard);
  else
    gpSmallHeap->vFreeObject((Object*)pChard);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Charred::Charred(void)
: cn(CN_DIR)
{
  //m_bMalloc set in new
  m_iDepth = 0;
  m_bEOF   = false;
  m_pChain = nullptr;
  
  vResetParser();
};

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Charred::~Charred(void)
{
  if (m_xFile.bIsOpen())
    m_xFile.vClose();
}

/*----------------------------------------------------------------------------*/
/* begin parsing                                                              */
/*----------------------------------------------------------------------------*/
void Charred::vBeginParsing(void)
{
  ASSERT(IS_NONE != m_eSource);
}

/*----------------------------------------------------------------------------*/
/* end parsing                                                                */
/*----------------------------------------------------------------------------*/
void Charred::vEndParsing(void)
{
  switch (m_eSource) {

    case IS_FILE:
      ASSERT( m_xFile.bIsOpen() );
      m_xFile.vClose();
      break;
      
    case IS_STDIN:
      break;
    
    case IS_STRING:
      break;
    
    default: ;
  }

  vResetParser();
}

/*----------------------------------------------------------------------------*/
/* set user prompt                                                            */
/*----------------------------------------------------------------------------*/
void Charred::vPrompt(s& sPrompt)
{
  m_sPrompt = sPrompt;
}

/*----------------------------------------------------------------------------*/
/* set line mode                                                              */
/*----------------------------------------------------------------------------*/
void Charred::vLineMode(bool bLineMode)
{
  m_bLineMode = bLineMode;
  m_bPrompt   = bLineMode ? true : false;
}

/*----------------------------------------------------------------------------*/
/* use given file for parsing                                                 */
/*----------------------------------------------------------------------------*/
bool Charred::bUseFile(s& sPath)
{
  vResetParser();
  
  m_xFile.vPath(sPath);
  
  bool bOpened = m_xFile.bOpen(C_OPEN_READ);
  
  if (bOpened)
    m_eSource = IS_FILE;
  
  return bOpened;
}

/*----------------------------------------------------------------------------*/
/* use stdin for parsing                                                      */
/*----------------------------------------------------------------------------*/
void Charred::vUseStdin(void)
{
  vResetParser();
  
  m_xFile.vSet(stdin,"stdin",C_OPEN_READ);
  
  m_bPrompt = true;
  
  m_eSource = IS_STDIN;
}

/*----------------------------------------------------------------------------*/
/* use given string for parsing                                               */
/*----------------------------------------------------------------------------*/
void Charred::vUseString(s& sString)
{
  vResetParser();
  
  m_sRawLine = sString;
  //String::vStrip(m_sRawLine.a(),'\n');
  
  m_eSource = IS_STRING;
}

/*----------------------------------------------------------------------------*/
/* get next parsed char                                                       */
/*----------------------------------------------------------------------------*/
char Charred::cGet(void)
{
  // do we have any chars in the regs?
  if (! m_nLoaded)
    vLoadRegisters();
  
  if (! m_nLoaded)
    return '\0';
  
  // consume the first char reg
  char cParsedChar = cConsumeFirst();
  
  // return the consumed char
  return cParsedChar;
}

/*----------------------------------------------------------------------------*/
/* put char back into parsing stream                                          */
/*----------------------------------------------------------------------------*/
void Charred::vUnget(char cParsed)
{
  vRewindRegisters(cParsed);
}

/*----------------------------------------------------------------------------*/
/* pop parsing stack                                                          */
/*----------------------------------------------------------------------------*/
bool Charred::bPop(void)
{
  if (! m_pChain)
    return false;
  
  Charred* pCopy = m_pChain;
  
  Memory::vCopy(this,pCopy,sizeof(Charred));
  
  pCopy->vZero();
  
  delete pCopy;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* push copy of this parser onto stack                                        */
/*----------------------------------------------------------------------------*/
void Charred::vPush(void)
{
  Charred* pCopy = new(m_bMalloc) Charred;
  
  Memory::vCopy(pCopy,this,sizeof(Charred));
  
  m_pChain = pCopy;
  m_iDepth++;
  
  m_eSource = IS_NONE;
  m_xFile.vZero();
  m_sRawLine = "";
  m_pRawInput = m_sRawLine.a();
  m_bLineMode = false;
  m_bPrompt = false;
  m_nLoaded = 0;
  m_cFirst  = 0;
  m_cSecond  = 0;
  m_eComment = CT_NONE;
}

/*----------------------------------------------------------------------------*/
/* depth of this parser                                                       */
/*----------------------------------------------------------------------------*/
i8 Charred::iDepth(void)
{
  return m_iDepth;
}

/*----------------------------------------------------------------------------*/
/* have we seen an EOF?                                                       */
/*----------------------------------------------------------------------------*/
bool Charred::bEOF(void)
{
  return m_bEOF;
}

/*----------------------------------------------------------------------------*/
/* zero this parser                                                           */
/*----------------------------------------------------------------------------*/
void Charred::vZero(void)
{
  m_xFile.vZero();
}


// private: ////////////////////////////////////////////////////////////////////


/*----------------------------------------------------------------------------*/
/* set depth of this parser                                                   */
/*----------------------------------------------------------------------------*/
void Charred::vDepth(i8 iDepth)
{
  m_iDepth = iDepth;
}

/*----------------------------------------------------------------------------*/
/* reset this parser                                                          */
/*----------------------------------------------------------------------------*/
void Charred::vResetParser(void)
{
  ASSERT(! m_xFile.bIsOpen());
  
  m_eSource   = IS_NONE;
  m_xFile.vReset();
  m_sRawLine  = "";
  m_aRawLine  = m_sRawLine.a();
  m_pRawInput = m_sRawLine.a();
  
  m_bPrompt   = false;
  
  m_nLoaded   = 0;
  m_cFirst    = '\0';
  m_cSecond   = '\0';
  m_eComment  = CT_NONE;
}


/*----------------------------------------------------------------------------*/
/*-- Character Processing ----------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* clear char registers                                                       */
/*----------------------------------------------------------------------------*/
void Charred::vClearRegisters(void)
{
  m_nLoaded = 0;
  m_cFirst  = '\0';
  m_cSecond = '\0';
}

/*----------------------------------------------------------------------------*/
/* consume the char in the first register                                     */
/*----------------------------------------------------------------------------*/
char Charred::cConsumeFirst(void)
{
  ASSERT(m_nLoaded);
  
  char cWasFirst = m_cFirst;
  
  m_cFirst  = m_cSecond;
  m_cSecond = '\0';
  
  m_nLoaded--;
  
  return cWasFirst;
}

/*----------------------------------------------------------------------------*/
/* discard the char in the first register                                     */
/*----------------------------------------------------------------------------*/
void Charred::vDiscardFirst(void)
{
  ASSERT(m_nLoaded);
  
  m_cFirst  = m_cSecond;
  m_cSecond = '\0';
  
  m_nLoaded--;
}

/*----------------------------------------------------------------------------*/
/* load both the first and second registers from raw input stream             */
/*----------------------------------------------------------------------------*/
void Charred::vLoadBoth(void)
{
  ASSERT(! m_nLoaded);
  
  m_cFirst = cFromRawInput();
  
  if (m_cFirst) {
    
    m_nLoaded++;
    
    m_cSecond = cFromRawInput();
    
    if (m_cSecond)
      m_nLoaded++;
  }
}

/*----------------------------------------------------------------------------*/
/* load first register from input stream                                      */
/*----------------------------------------------------------------------------*/
void Charred::vLoadFirst(void)
{
  ASSERT(! m_nLoaded);
  
  m_cFirst = cFromRawInput();
  
  if (m_cFirst)
    m_nLoaded++;
}

/*----------------------------------------------------------------------------*/
/* load first (and maybe) second register from input stream, strip comments   */
/*----------------------------------------------------------------------------*/
void Charred::vLoadRegisters(void)
{
  ASSERT(0 == m_nLoaded);
  
  while (! m_nLoaded) {
    
    vLoadFirst();
    
    if (! m_nLoaded)
      break;
    
    if (! bPossibleComment())  // this may leave a char in second register
      continue;                // e.g. /t
    
    vStripComment();
  }
}

/*----------------------------------------------------------------------------*/
/* load second register from input stream                                     */
/*----------------------------------------------------------------------------*/
void Charred::vLoadSecond(void)
{
  ASSERT(1 == m_nLoaded);
  ASSERT(m_cFirst);
  
  m_cSecond = cFromRawInput();
  
  if (m_cSecond)
    m_nLoaded++;
}

/*----------------------------------------------------------------------------*/
/* rewind registers by one character                                          */
/*----------------------------------------------------------------------------*/
void Charred::vRewindRegisters(char cParsed)
{
  ASSERT(m_nLoaded < 2);
  ASSERT('\0' == m_cSecond);
  
  m_cSecond = m_cFirst;
  m_cFirst  = cParsed;
  
  m_nLoaded++;
}

/*----------------------------------------------------------------------------*/
/* strip a possible line or block comment                                     */
/*----------------------------------------------------------------------------*/
void Charred::vStripComment(void)
{
  switch (eCommentType()) {
      
    case CT_BLOCK:
      vBlockCommentSkip();
      vRewindRegisters(' ');
      break;
      
    case CT_LINE:
      vLineCommentSkip();
      vRewindRegisters(' ');
      break;
      
    default:
      break;
  }
}


/*----------------------------------------------------------------------------*/
/* -- Comment Processing -----------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* test for end of block comment                                              */
/*----------------------------------------------------------------------------*/
bool Charred::bBlockCommentEnd(void)
{
  if ('*' == m_cFirst && '/' == m_cSecond)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* skip a block comment                                                       */
/*----------------------------------------------------------------------------*/
void Charred::vBlockCommentSkip(void)
{
  vClearRegisters();
  
  vLoadBoth();
  
  while (! bBlockCommentEnd()) {
    vDiscardFirst();
    vLoadSecond();
  }
  
  vClearRegisters();
}

/*----------------------------------------------------------------------------*/
/* test for start of block comment                                            */
/*----------------------------------------------------------------------------*/
bool Charred::bBlockCommentStart(void)
{
  if ('/' == m_cFirst && '*' == m_cSecond)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* test for possible start of comment                                         */
/*----------------------------------------------------------------------------*/
bool Charred::bPossibleComment(void)
{
  ASSERT(m_nLoaded);
  
  if ('/' == m_cFirst) {
    if (1 == m_nLoaded)
      vLoadSecond();
    return true;
  }
  else {
    return false;
  }
}

/*----------------------------------------------------------------------------*/
/* test for start of comment                                                  */
/*----------------------------------------------------------------------------*/
Charred::eCT Charred::eCommentType(void)
{
  if (bLineCommentStart())
    return CT_LINE;
  
  if (bBlockCommentStart())
    return CT_BLOCK;
  
  return CT_NONE;
}

/*----------------------------------------------------------------------------*/
/* test for end of line comment                                               */
/*----------------------------------------------------------------------------*/
bool Charred::bLineCommentEnd(void)
{
  if ('\n' == m_cFirst)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* skip a line comment                                                        */
/*----------------------------------------------------------------------------*/
void Charred::vLineCommentSkip(void)
{
  vClearRegisters();
  
  vLoadFirst();
  
  while (! bLineCommentEnd()) {
    vDiscardFirst();
    vLoadFirst();
  }
  
  ASSERT('\n' == m_cFirst || '\0' == m_cFirst);
}

/*----------------------------------------------------------------------------*/
/* test for start of line comment                                             */
/*----------------------------------------------------------------------------*/
bool Charred::bLineCommentStart(void)
{
  if ('/' == m_cFirst && '/' == m_cSecond)
    return true;
  else
    return false;
}


/*----------------------------------------------------------------------------*/
/* -- File Reading -----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get next char from input source                                            */
/*----------------------------------------------------------------------------*/
char Charred::cFromRawInput(void)
{
  char c;

  c = *m_pRawInput++;
  
  if (c)
    return c;
  
  switch (m_eSource) {
      
    case IS_FILE:
      ASSERT(m_xFile.bIsOpen());
      if (m_bLineMode) {
        if (m_bPrompt) {
          m_bPrompt = false;
          vReadLineFile();
          c = *m_pRawInput++;
        }
        else {
          m_bPrompt = true;
          m_pRawInput  = m_sRawLine.a();
          *m_pRawInput = '\0';
          c = '\0';
        }
      }
      else {
        vReadLineFile();
        c = *m_pRawInput++;
      }
      break;
      
    case IS_STDIN:
      ASSERT(m_xFile.bIsOpen());
      ASSERT(m_xFile.bIsStdin());
      if (m_bLineMode) {
        if (m_bPrompt) {
          m_bPrompt = false;
          vReadLineStdin();
          c = *m_pRawInput++;
        }
        else {
          m_bPrompt = true;
          m_pRawInput = m_sRawLine.a();
          *m_pRawInput = '\0';
          c = '\0';
        }
      }
      else {
        vReadLineStdin();
        c = *m_pRawInput++;
      }
      break;
      
    case IS_STRING:
      c = '\0';
      break;
      
    default:
      ASSERT(0);
  }
  
  return c;
}

/*----------------------------------------------------------------------------*/
/* read line from input file                                                  */
/*----------------------------------------------------------------------------*/
void Charred::vReadLineFile(void)
{
  char c;
  
  char* pCharInLine = m_sRawLine.a();
  
  do {
    
    c = m_xFile.cGetChar();
    
    *pCharInLine++ = c;
    
    if (C_END_OF_LINE == c)
      break;
    
  } while (c);
  
  if (C_END_OF_FILE == c)
    m_bEOF = true;
  
  *pCharInLine++ = '\0';
  
  m_pRawInput = m_sRawLine.a();
}

/*----------------------------------------------------------------------------*/
/* read line from stdin                                                       */
/*----------------------------------------------------------------------------*/
void Charred::vReadLineStdin(void)
{
  char c;
  
  m_pRawInput = m_sRawLine.a();
  
  printf("%s",m_sPrompt.a());
  fflush(stdin);
  
  for (;;) {
    
    c = m_xFile.cGetChar();
    
    *m_pRawInput++ = c;
    
    if (C_END_OF_LINE == c)
      break;

  }
  
  ASSERT(c);

  *m_pRawInput++ = '\0';
  
  m_pRawInput = m_sRawLine.a();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/