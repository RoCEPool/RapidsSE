/*============================================================================*/
/* Input2.cpp -- Input Processing                                             */
/*============================================================================*/

#include "Input2.hpp"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Input2::Input2(void)
{
  m_xTokenizer.vMalloc(true);
  m_xTokenizer.vPrompt("se.2> ");
  
  m_xTokenizer.vPush();
  m_xTokenizer.vUseStdin();
  m_xTokenizer.vLineMode(true);
  m_xTokenizer.vBegin();
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Input2::~Input2(void)
{
}

/*----------------------------------------------------------------------------*/
/* run seitf>                                                                 */
/*----------------------------------------------------------------------------*/
void Input2::vNext(char* pWords[])
{
  while (true) {
   
    for (i8 i = 0; i < 16; i++)
      *pWords[i] = '\0';
    
    i8 i = 0;
    while (LT_NONE != m_xTokenizer.eNextLiteral(pWords[i])) {
      //vTrace("Word [%lld] %s\n",i,pWords[i]);
      i++;
      ASSERT(i < 16);
    }
    
    if (m_xTokenizer.bEOF()) {
      vPopFile();
      continue;
    }
    
    break;
  }
    
}

/*----------------------------------------------------------------------------*/
/* run seitf>                                                                 */
/*----------------------------------------------------------------------------*/
void Input2::vInit(char* wWorkarea, char* pWords[])
{
  pWords[0]  = &wWorkarea[C_1KB * 0];
  pWords[1]  = &wWorkarea[C_1KB * 1];
  pWords[2]  = &wWorkarea[C_1KB * 2];
  pWords[3]  = &wWorkarea[C_1KB * 3];
  pWords[4]  = &wWorkarea[C_1KB * 4];
  pWords[5]  = &wWorkarea[C_1KB * 5];
  pWords[6]  = &wWorkarea[C_1KB * 6];
  pWords[7]  = &wWorkarea[C_1KB * 7];
  pWords[8]  = &wWorkarea[C_1KB * 8];
  pWords[9]  = &wWorkarea[C_1KB * 9];
  pWords[10] = &wWorkarea[C_1KB * 10];
  pWords[11] = &wWorkarea[C_1KB * 11];
  pWords[12] = &wWorkarea[C_1KB * 12];
  pWords[13] = &wWorkarea[C_1KB * 13];
  pWords[14] = &wWorkarea[C_1KB * 14];
  pWords[15] = &wWorkarea[C_1KB * 15];
 }

/*----------------------------------------------------------------------------*/
/* set prompt                                                                 */
/*----------------------------------------------------------------------------*/
void Input2::vPrompt(const char* wPrompt)
{
  m_xTokenizer.vPrompt("se.2> ");
}

/*----------------------------------------------------------------------------*/
/* get EOF?                                                                   */
/*----------------------------------------------------------------------------*/
bool Input2::bEOF(void)
{
  bool bEOF = m_xTokenizer.bEOF();
  
  return bEOF;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* set line mode                                                              */
/*----------------------------------------------------------------------------*/
void Input2::vLineMode(bool bLineMode)
{
  m_xTokenizer.vLineMode(bLineMode);
}

/*----------------------------------------------------------------------------*/
/* push command file                                                          */
/*----------------------------------------------------------------------------*/
void Input2::vPushFile(char* pPath)
{
  s256 sPath(pPath);
  
  m_xTokenizer.vPush();
  
  bool b = m_xTokenizer.bUseFile(sPath);
  ASSERT(b);
  
  m_xTokenizer.vBegin();
}

/*----------------------------------------------------------------------------*/
/* pop command file                                                           */
/*----------------------------------------------------------------------------*/
void Input2::vPopFile(void)
{
  bool b = m_xTokenizer.bPop();
  
  ASSERT(b);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/