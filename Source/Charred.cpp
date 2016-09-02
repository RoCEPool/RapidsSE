/*============================================================================*/
/* Charred.cpp -- input processor, removing line and block comments           */
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
  m_iDepth = 1;
  m_pChain = nullptr;
  
  vResetAll();
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
/* begin using                                                                */
/*----------------------------------------------------------------------------*/
void Charred::vBeginUse(void)
{
  ASSERT(IS_NONE != m_eSource);
}

/*----------------------------------------------------------------------------*/
/* end use                                                                    */
/*----------------------------------------------------------------------------*/
void Charred::vEndUse(void)
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

  vResetAll();
}

/*----------------------------------------------------------------------------*/
/* set prompt                                                                 */
/*----------------------------------------------------------------------------*/
void Charred::vPrompt(s& sPrompt)
{
  m_sPrompt = sPrompt;
}

/*----------------------------------------------------------------------------*/
/* use given file for charring                                                */
/*----------------------------------------------------------------------------*/
bool Charred::bUseFile(s& sPath)
{
  vResetAll();
  
  m_xFile.vPath(sPath);
  
  bool bOpened = m_xFile.bOpen(C_OPEN_READ);
  
  if (bOpened)
    m_eSource = IS_FILE;
  
  return bOpened;
}

/*----------------------------------------------------------------------------*/
/* use stdin for charring                                                     */
/*----------------------------------------------------------------------------*/
void Charred::vUseStdin(void)
{
  vResetAll();
  
  m_xFile.vSet(stdin,"stdin",C_OPEN_READ);
  
  m_bPrompt = true;
  
  m_eSource = IS_STDIN;
}

/*----------------------------------------------------------------------------*/
/* use given string for charring                                              */
/*----------------------------------------------------------------------------*/
void Charred::vUseString(s& sString)
{
  vResetAll();
  
  m_sString = sString;
  //String::vStrip(m_sString.a(),'\n');
  
  m_eSource = IS_STRING;
}

/*----------------------------------------------------------------------------*/
/* get next charred char                                                      */
/*----------------------------------------------------------------------------*/
char Charred::cGet(void)
{
  // do we have any chars in the regs?
  if (! m_nChars)
    vGetMore();
  
  if (! m_nChars)
    return '\0';
  
  // consume the first char
  char c = cConsumeOne();
  
  // return the consumed char
  return c;
}

/*----------------------------------------------------------------------------*/
/* put char back into charred stream                                          */
/*----------------------------------------------------------------------------*/
void Charred::vUnget(char c)
{
  vRewindOne(c);
}

/*----------------------------------------------------------------------------*/
/* is there a nested chain?                                                   */
/*----------------------------------------------------------------------------*/
bool Charred::bNest(void)
{
  if (m_pChain)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* pop charred chain                                                          */
/*----------------------------------------------------------------------------*/
bool Charred::bPop(void)
{
  if (! m_pChain)
    return false;
  
  Charred* pCopy = m_pChain;
  
  Memory::vCopy(this,pCopy,sizeof(Charred));
  
  delete pCopy;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* push this charred onto chain                                               */
/*----------------------------------------------------------------------------*/
void Charred::vPush(void)
{
  Charred* pCopy = new(m_bMalloc) Charred;
  
  Memory::vCopy(pCopy,this,sizeof(Charred));
  
  m_pChain = pCopy;
}

/*----------------------------------------------------------------------------*/
/* depth of this parser                                                       */
/*----------------------------------------------------------------------------*/
i8 Charred::iDepth(void)
{
  return m_iDepth;
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
/*-- Character Processing ----------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* clear char registers                                                       */
/*----------------------------------------------------------------------------*/
void Charred::vClearRegs(void)
{
  m_nChars = 0;
  m_cOne   = '\0';
  m_cTwo   = '\0';
}

/*----------------------------------------------------------------------------*/
/* consume the first char                                                     */
/*----------------------------------------------------------------------------*/
char Charred::cConsumeOne(void)
{
  ASSERT(m_nChars);
  
  char c = m_cOne;
  m_cOne = m_cTwo;
  m_cTwo = '\0';
  
  m_nChars--;
  
  return c;
}

/*----------------------------------------------------------------------------*/
/* discard the first char                                                     */
/*----------------------------------------------------------------------------*/
void Charred::vDiscardOne(void)
{
  ASSERT(m_nChars);
  
  m_cOne = m_cTwo;
  m_cTwo = '\0';
  
  m_nChars--;
}

/*----------------------------------------------------------------------------*/
/* get next first char                                                        */
/*----------------------------------------------------------------------------*/
void Charred::vGetFirst(void)
{
  ASSERT(! m_nChars);
  
  m_cOne = cNextChar();
  
  if (m_cOne)
    m_nChars++;
}

/*----------------------------------------------------------------------------*/
/* no char(s) in regs, so get more char(s) from input                         */
/*----------------------------------------------------------------------------*/
void Charred::vGetMore(void)
{
  ASSERT(0 == m_nChars);
  
  while (! m_nChars) {
    
    vGetFirst();
    
    if (! m_nChars)
      break;
    
    if (! bPossibleComment() )
      continue;
    
    switch (eCommentType()) {
        
      case CT_BLOCK:
        vBlockCommentSkip();
        vRewindOne(' ');
        break;
        
      case CT_LINE:
        vLineCommentSkip();
        vRewindOne(' ');
        break;
        
      default:
        break;
    }
  }
}

/*----------------------------------------------------------------------------*/
/* get next second char                                                       */
/*----------------------------------------------------------------------------*/
void Charred::vGetSecond(void)
{
  ASSERT(1 == m_nChars);
  ASSERT(m_cOne);
  
  m_cTwo = cNextChar();
  
  if (m_cTwo)
    m_nChars++;
}

/*----------------------------------------------------------------------------*/
/* no char(s) in regs, get two more                                           */
/*----------------------------------------------------------------------------*/
void Charred::vGetTwo(void)
{
  ASSERT(! m_nChars);
  
  m_cOne = cNextChar();
  
  if (m_cOne) {
    m_nChars++;

    m_cTwo = cNextChar();
    
    if (m_cTwo)
      m_nChars++;
  }
}

/*----------------------------------------------------------------------------*/
/* reset                                                                      */
/*----------------------------------------------------------------------------*/
void Charred::vResetAll(void)
{
  ASSERT(! m_xFile.bIsOpen());
  
  m_eSource  = IS_NONE;
  m_xFile.vReset();
  m_sString  = "";
  m_pChar    = m_sString.a();
  m_pEnd     = m_pChar;

  m_sPrompt  = "";
  m_bPrompt  = false;
  
  m_nChars   = 0;
  m_cOne     = '\0';
  m_cTwo     = '\0';
  m_eComment = CT_NONE;
}

/*----------------------------------------------------------------------------*/
/* rewind charred stream by one character                                     */
/*----------------------------------------------------------------------------*/
void Charred::vRewindOne(char c)
{
  ASSERT(m_nChars < 2);
  ASSERT('\0' == m_cTwo);
  
  m_cTwo = m_cOne;
  m_cOne = c;
  
  m_nChars++;
}


/*----------------------------------------------------------------------------*/
/* -- Comment Processing -----------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* test for end of block comment                                              */
/*----------------------------------------------------------------------------*/
bool Charred::bBlockCommentEnd(void)
{
  if ('*' == m_cOne && '/' == m_cTwo)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* skip a block comment                                                       */
/*----------------------------------------------------------------------------*/
void Charred::vBlockCommentSkip(void)
{
  vClearRegs();
  
  vGetTwo();
  while (! bBlockCommentEnd()) {
    vDiscardOne();
    vGetSecond();
  }
  
  vClearRegs();
}

/*----------------------------------------------------------------------------*/
/* test for start of block comment                                            */
/*----------------------------------------------------------------------------*/
bool Charred::bBlockCommentStart(void)
{
  if ('/' == m_cOne && '*' == m_cTwo)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* test for possible start of comment                                         */
/*----------------------------------------------------------------------------*/
bool Charred::bPossibleComment(void)
{
  ASSERT(m_nChars);
  
  if ('/' == m_cOne) {
    if (1 == m_nChars)
      vGetSecond();
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
  if ('\n' == m_cOne)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* skip a line comment                                                        */
/*----------------------------------------------------------------------------*/
void Charred::vLineCommentSkip(void)
{
  vClearRegs();
  
  vGetFirst();
  while (! bLineCommentEnd()) {
    vDiscardOne();
    vGetFirst();
  }
  
  //vClearRegs();
}

/*----------------------------------------------------------------------------*/
/* test for start of line comment                                             */
/*----------------------------------------------------------------------------*/
bool Charred::bLineCommentStart(void)
{
  if ('/' == m_cOne && '/' == m_cTwo)
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
char Charred::cNextChar(void)
{
  char c;

  c = *m_pChar++;
  
  if (c)
    return c;
  
  switch (m_eSource) {
      
    case IS_FILE:
      ASSERT(m_xFile.bIsOpen());
      vReadLineFile();
      c = *m_pChar++;
      break;
      
    case IS_STDIN:
      ASSERT(m_xFile.bIsOpen());
      ASSERT(m_xFile.bIsStdin());
      vReadLineStdin();
      c = *m_pChar++;
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
/* read line from file                                                        */
/*----------------------------------------------------------------------------*/
void Charred::vReadLineFile(void)
{
  char c;
  
  m_pChar = m_sString.a();
  
  do {
    c = m_xFile.cGetChar();
    *m_pChar++ = c;
    if ('\n' == c)
      break;
  } while (c);
  
  *m_pChar++ = '\0';
  
  m_pChar = m_sString.a();
}

/*----------------------------------------------------------------------------*/
/* read line from stdin                                                       */
/*----------------------------------------------------------------------------*/
void Charred::vReadLineStdin(void)
{
  char c;
  
  m_pChar = m_sString.a();
  
  printf("%s",m_sPrompt.a());
  
  do {
    c = m_xFile.cGetChar();
    *m_pChar++ = c;
    if ('\n' == c)
      break;
  } while (c);

  *m_pChar++ = '\0';
  
  m_pChar = m_sString.a();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/