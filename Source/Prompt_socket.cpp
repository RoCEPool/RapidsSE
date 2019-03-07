/*============================================================================*/
/* Prompt_socket.cpp -- socket statistics                                     */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_socket (
  PF_SOCKET,           // prompt function
  "Prompt_socket"_Stz, // name of the prompt function
  "comment"_Stz,       // comment about the functopm
  "socket"_Stz,        // the prompt command
  nullptr,             // the second word
  bPrompt_socket,      // prompt function
  true,                // enabled

  true,                // visible
  "socket"_Stz,        // help
  ""_Stz               // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_socket(SePrompt* pPrompt)
{  
  Socket::vStatistics();
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
