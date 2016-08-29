/*============================================================================*/
/* SePrompt.hpp -- se> prompt                                                 */
/*============================================================================*/

#pragma once

#include "Api.hpp"
#include "Input.hpp"
#include "History.hpp"

class SePrompt
{
private:
  static const i8 C_INPUT_ARRAY = 4;

  i8     m_iPartition; // which partition
  SEitf* m_pSEitf;     // interface into rapidsse
  s256   m_sPrompt;    // the prompt
  
  s256        m_sInputLine;
  i8          m_iInput;
  InputStream m_xInputStream[C_INPUT_ARRAY]; // input stream handler
  
  Input       m_xInput;
  s8K         m_sResponse;       // response
  
  History     m_xHistory;
  
  
public:
  SePrompt(i8 iPartition);
  ~SePrompt(void);
  
  void vPrompt(void);
  void vPrompt2(void);
  
  
  
private:
  void   vExecute(char* pWords[]);
  InputStream* pInput(void);
  void   vIterateRows(void);
  
  //----------------------------------------------------------------------------
  bool bTestFile(char* pWord);
  void vPushFile(char* pWord);
  void vPopFile(void);
  
  //----------------------------------------------------------------------------
  bool bHelp(char* pWords[]);
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/