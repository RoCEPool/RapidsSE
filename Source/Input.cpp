/*============================================================================*/
/* Input.cpp -- Input Processing                                              */
/*============================================================================*/

#include "Input.hpp"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Input::Input(void)
{
  m_sPrompt     = "se> ";
  m_sInputLine  = "";
  m_iInputDepth = 0;
  //m_xInputStream
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Input::~Input(void)
{
}

/*----------------------------------------------------------------------------*/
/* run seitf>                                                                 */
/*----------------------------------------------------------------------------*/
void Input::vNext(char* pWords[])
{
  InputStream* pInput;
  
  while (true) {
   
    for (i8 i = 0; i < 16; i++)
      *pWords[i] = '\0';
    
    pInput = pTopInput();
    
    if (pInput->bStdin()) {
      pInput->vUseStdin(m_sInputLine);
      pInput->bGetLine(m_sInputLine);
      pInput->vUseText(m_sInputLine);
    }
    else {
      pInput->vUseFile(m_sInputLine);
      if (! pInput->bFirstChar() ) {
        if ( ! pInput->bGetLine(m_sInputLine) ) {
          vPopFile();
          continue;
        }
      }
      pInput->vUseText(m_sInputLine);
    }
    
    i8 i = 0;
    while (LT_NONE != pInput->eNextLiteral(pWords[i])) {
      //vTrace("Word [%lld] %s\n",i,pWords[i]);
      i++;
    }
    
    break;
  }
    
}

/*----------------------------------------------------------------------------*/
/* run seitf>                                                                 */
/*----------------------------------------------------------------------------*/
void Input::vInit(char* wWorkarea, char* pWords[])
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
 
  InputStream* pInput = pTopInput();

  pInput->vUseStdin(m_sInputLine);
  pInput->vPrompt(m_sPrompt.a());
}

/*----------------------------------------------------------------------------*/
/* set prmpt                                                                  */
/*----------------------------------------------------------------------------*/
void Input::vPrompt(const char* wPrompt)
{
  m_sPrompt = wPrompt;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* push command file                                                          */
/*----------------------------------------------------------------------------*/
void Input::vPushFile(char* pWord)
{
  ASSERT(m_iInputDepth < C_INPUT_DEPTH - 1);
  m_iInputDepth++;
  
  InputStream* pInput = &m_xInputStream[m_iInputDepth];
  
  bool b = pInput->bOpenFile(m_sInputLine,pWord);
  ASSERT(b);
  
}

/*----------------------------------------------------------------------------*/
/* pop command file                                                           */
/*----------------------------------------------------------------------------*/
void Input::vPopFile(void)
{
  ASSERT(m_iInputDepth);
  
  InputStream* pInput = &m_xInputStream[m_iInputDepth];
  
  pInput->vCloseFile();
  
  m_iInputDepth--;
}

/*----------------------------------------------------------------------------*/
/* get current input stream                                                   */
/*----------------------------------------------------------------------------*/
InputStream* Input::pTopInput(void)
{
  return &m_xInputStream[m_iInputDepth];
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/