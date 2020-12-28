/*============================================================================*/
/* Prompt_socket.cpp -- socket statistics                                     */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"


#include "Socket.hpp" //?

PromptFun __Prompt_socket (
  PF_SOCKET,           // prompt function
  "Prompt_socket"_Stz, // name of the prompt function
  "comment"_Stz,       // comment about the function
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
bool PROMPT::bPrompt_socket(SePrompt* pPrompt)
{  
  Socket::vStatistics();
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
