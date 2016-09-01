/*============================================================================*/
/* Charred.cpp -- char input processor which removes line abd block comments  */
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
  //m_bMalloc
  m_eWhich  = CW_NONE;
  //m_xFile
  //m_sString
  m_pChar   = nullptr;
  
  m_sPrompt = "";
  m_bPrompt = false;
  
  m_nChars  = 0;
  m_cOne    = '\0';
  m_cTwo    = '\0';
  
  m_pChain  = nullptr;
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
  ASSERT(CW_NONE != m_eWhich);
}

/*----------------------------------------------------------------------------*/
/* end use                                                                    */
/*----------------------------------------------------------------------------*/
void Charred::vEndUse(void)
{
  switch (m_eWhich) {

    case CW_FILE:
      ASSERT( m_xFile.bIsOpen() );
      m_xFile.vClose();
      break;
      
    case CW_STDIN:
      m_xFile.vReset();
      break;
    
    case CW_STRING:
      break;
    
    default: ;
  }

  vReset();
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
  vReset();
  
  m_xFile.vPath(sPath);
  
  bool bOpened = m_xFile.bOpen(C_OPEN_READ);
  
  if (bOpened)
    m_eWhich = CW_FILE;
  
  return bOpened;
}

/*----------------------------------------------------------------------------*/
/* use stdin for charring                                                     */
/*----------------------------------------------------------------------------*/
void Charred::vUseStdin(void)
{
  vReset();
  
  m_xFile.vSet(stdin,"stdin",C_OPEN_READ);
  
  m_bPrompt = true;
  
  m_eWhich = CW_STDIN;
}

/*----------------------------------------------------------------------------*/
/* use given string for charring                                              */
/*----------------------------------------------------------------------------*/
void Charred::vUseString(s& sString)
{
  vReset();
  
  m_sString = sString;
  
  m_eWhich = CW_STRING;
}

/*----------------------------------------------------------------------------*/
/* get next charred char                                                      */
/*----------------------------------------------------------------------------*/
char Charred::cGet(void)
{
  // do we have any chars in the regs?
  if (! m_nChars)
    vAdvanceOne();
  
  // consume the first char
  char c = m_cOne;
  vConsumeOne();
  
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


// private: ////////////////////////////////////////////////////////////////////


/*----------------------------------------------------------------------------*/
/*-- Character Processing ----------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* no char(s) in regs, so advance to next charred char(s) from input          */
/*----------------------------------------------------------------------------*/
void Charred::vAdvanceOne(void)
{
  ASSERT(0 == m_nChars);

  while (! m_nChars) {
    
    vGetMore();
    
    if (bEndLine()) {
      vDiscardOne();
      continue;
    }
  
    if (! bCommentStart() )
      continue;
    
    switch (eCommentType()) {
      
      case CT_BLOCK:
        vSkipBlockComment();
        break;

      case CT_LINE:
        vSkipLineComment();
        break;
        
      default:
        ASSERT(0);
    }
  }
  
  ASSERT(m_nChars);
}

/*----------------------------------------------------------------------------*/
/* consume one char                                                           */
/*----------------------------------------------------------------------------*/
void Charred::vConsumeOne(void)
{
  switch (m_nChars) {
      
    case 0:
      return;
      
    case 1:
      m_nChars = 0;
      return;
      
    case 2:
      m_cOne = m_cTwo;
      m_nChars = 1;
      return;
      
    default:
      ASSERT(0);
  }
}

/*----------------------------------------------------------------------------*/
/* discard one char                                                           */
/*----------------------------------------------------------------------------*/
void Charred::vDiscardOne(void)
{
  m_cOne = m_cTwo;
  m_cTwo = '\0';
  
  m_nChars--;
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
/*-- Register Management -----------------------------------------------------*/
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
/* get more input                                                             */
/*----------------------------------------------------------------------------*/
void Charred::vGetMore(void)
{
  ASSERT(m_nChars < 2);
  
  ASSERT( (0 == m_nChars) || (1 == m_nChars && m_cOne) );
  
  char c;
  
  switch (m_eWhich) {
      
    case CW_FILE:
      ASSERT(m_xFile.bIsOpen());
      c = *m_pChar++;
      if (!c) {
        vReadLineFile();
        c = *m_pChar++;
      }
      break;
      
    case CW_STDIN:
      ASSERT(m_xFile.bIsOpen());
      ASSERT(m_xFile.bIsStdin());
      c = *m_pChar++;
      if (!c) {
        vReadLineStdin();
        c = *m_pChar++;
      }
      break;
      
    case CW_STRING:
      c = *m_pChar++;
      break;
      
    default:
      ASSERT(0);
  }
  
  m_nChars++;
  if (1 == m_nChars)
    m_cOne = c;
  else
    m_cTwo = c;
}

/*----------------------------------------------------------------------------*/
/* reset                                                                      */
/*----------------------------------------------------------------------------*/
void Charred::vReset(void)
{
  ASSERT(! m_xFile.bIsOpen());
  
  m_eWhich   = CW_NONE;
  m_bPrompt  = false;
  m_eComment = CT_NONE;
  
  m_sString  = "";
  m_pChar    = m_sString.a();
  m_nChars   = 0;
  m_cOne     = '\0';
  m_cTwo     = '\0';
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
bool Charred::bCommentStart(void)
{
  ASSERT(m_nChars);
  
  if ('/' == m_cOne) {
    if (1 == m_nChars)
      vGetMore();
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
/* skip a block comment                                                       */
/*----------------------------------------------------------------------------*/
void Charred::vSkipBlockComment(void)
{
  vClearRegs();
  
  while (! bBlockCommentEnd())
    vGetMore();
  
  vClearRegs();
}

/*----------------------------------------------------------------------------*/
/* skip a line comment                                                        */
/*----------------------------------------------------------------------------*/
void Charred::vSkipLineComment(void)
{
  vClearRegs();
  
  while (! bLineCommentEnd())
    vGetMore();
  
  vClearRegs();
}


/*----------------------------------------------------------------------------*/
/* -- File Reading -----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* test for end-of-line                                                       */
/*----------------------------------------------------------------------------*/
bool Charred::bEndLine(void)
{
  if ('\n' == m_cOne)
    return true;
  else
    return false;
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
    if ('\n' == c) {
      *m_pChar++ = '\0';
      break;
    }
    *m_pChar++ = c;
  } while (c);
  
  m_pChar = m_sString.a();
}

/*----------------------------------------------------------------------------*/
/* read line from stdin                                                       */
/*----------------------------------------------------------------------------*/
void Charred::vReadLineStdin(void)
{
  char c;
  
  m_pChar = m_sString.a();
  
  do {
    c = m_xFile.cGetChar();
    if ('\n' == c) {
      *m_pChar++ = '\0';
      break;
    }
    *m_pChar++ = c;
  } while (c);

  m_pChar = m_sString.a();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/