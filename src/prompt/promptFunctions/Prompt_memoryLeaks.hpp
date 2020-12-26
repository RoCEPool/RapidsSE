/*============================================================================*/
/* Prompt_memoryLeaks.hpp -- memory leaks                                     */
/*============================================================================*/
#pragma once

namespace PROMPT
{
#define C_ATTEMPTS 12

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  bool bPrompt_memoryLeaks(SePrompt* pPrompt);

  bool bPrompt_memoryLeaks_dump(SePrompt* pPrompt);
}

/*
 
 trackmemory;
 memoryleaks dump;
 
 */
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
