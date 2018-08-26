/*============================================================================*/
/* Prompt.h -- Prompt namespace                                               */
/*============================================================================*/
#pragma once

#include "Interface.h"

//-----------------------------------------------------------------------------
namespace Prompt
{
  void vCheckAssumptions(void);
}

using namespace Prompt;

//-----------------------------------------------------------------------------

#include "SePrompt.hpp"

#include "PromptFun.hpp"
#include "PromptCatalog.hpp"
#include "PromptNote.hpp"

#include "Prompt_bye.hpp"
#include "Prompt_cd.hpp"
#include "Prompt_displayOff.hpp"
#include "Prompt_displayOn.hpp"
#include "Prompt_help.hpp"
#include "Prompt_jobcat.hpp"
#include "Prompt_ls.hpp"
#include "Prompt_print.hpp"
#include "Prompt_pwd.hpp"
#include "Prompt_question.hpp"
#include "Prompt_se.hpp"
#include "Prompt_sleep.hpp"
#include "Prompt_timerEnd.hpp"
#include "Prompt_timerStart.hpp"
#include "Prompt_version.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
