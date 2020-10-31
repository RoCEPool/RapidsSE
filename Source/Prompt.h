/*============================================================================*/
/* Prompt.h -- Prompt namespace                                               */
/*============================================================================*/
#pragma once

#include "StorageEngine.h"

//------------------------------------------------------------------------------

namespace Prompt
{
  void vCheckAssumptions(void);
}

//------------------------------------------------------------------------------

#include "ShortCut.hpp"
typedef VectorNL_t<CN_SHORTCUTVEC, ShortCut, C_MAX_SHORTCUTS> ShortCutVec;

#include "SeDeputy.hpp"
#include "SePrompt.hpp"

#include "PromptFun.hpp"
#include "PromptCatalog.hpp"
#include "PromptHelper.hpp"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#include "Prompt_antlr.hpp"
#include "Prompt_args.hpp"
#include "Prompt_ascii.hpp"
#include "Prompt_attachMap.hpp"

#include "Prompt_bp.hpp"
#include "Prompt_bye.hpp"

#include "Prompt_cache.hpp"
#include "Prompt_calc.hpp"
#include "Prompt_cancel.hpp"
#include "Prompt_cd.hpp"
#include "Prompt_classes.hpp"
#include "Prompt_cmds.hpp"
#include "Prompt_config.hpp"
#include "Prompt_constants.hpp"

#include "Prompt_ddlLock.hpp"
#include "Prompt_delta.hpp"
#include "Prompt_deltaReset.hpp"
#include "Prompt_dlist.hpp"
#include "Prompt_dumpTokens.hpp"

#include "Prompt_enums.hpp"
#include "Prompt_errno.hpp"
#include "Prompt_executing.hpp"

#include "Prompt_fact.hpp"
#include "Prompt_file.hpp"
#include "Prompt_fileCat.hpp"
#include "Prompt_fileProc.hpp"
#include "Prompt_fileRand.hpp"
#include "Prompt_floats.hpp"
#include "Prompt_funs.hpp"

#include "Prompt_giveMem.hpp"
#include "Prompt_globals.hpp"

#include "Prompt_hashDefs.hpp"
#include "Prompt_heapMem.hpp"
#include "Prompt_help.hpp"
#include "Prompt_hexdump.hpp"
#include "Prompt_how.hpp"
#include "Prompt_hw.hpp"

#include "Prompt_indexEntries.hpp"

#include "Prompt_job.hpp"

#include "Prompt_limits.hpp"
#include "Prompt_llist.hpp"
#include "Prompt_locks.hpp"
#include "Prompt_log.hpp"
#include "Prompt_logNote.hpp"
#include "Prompt_logSection.hpp"
#include "Prompt_logWrite.hpp"
#include "Prompt_ls.hpp"
#include "Prompt_lvl.hpp"

#include "Prompt_masks.hpp"
#include "Prompt_memory.hpp"
#include "Prompt_memoryLeaks.hpp"
#include "Prompt_memTest.hpp"
#include "Prompt_mkfifo.hpp"
#include "Prompt_mon.hpp"
#include "Prompt_mri.hpp"

#include "Prompt_now.hpp"
#include "Prompt_numbers.hpp"

#include "Prompt_objects.hpp"
#include "Prompt_olist.hpp"
#include "Prompt_opdq.hpp"
#include "Prompt_operands.hpp"
#include "Prompt_opsys.hpp"

#include "Prompt_plist.hpp"
#include "Prompt_pmc.hpp"
#include "Prompt_pow2.hpp"
#include "Prompt_prefixes.hpp"
#include "Prompt_print.hpp"
#include "Prompt_procedure.hpp"
#include "Prompt_prompt.hpp"
#include "Prompt_promptDebug.hpp"
#include "Prompt_pwd.hpp"

#include "Prompt_question.hpp"

#include "Prompt_rassert.hpp"
#include "Prompt_ready.hpp"
#include "Prompt_resources.hpp"
#include "Prompt_rmfifo.hpp"
#include "Prompt_rnd.hpp"

#include "Prompt_select.hpp"
#include "Prompt_setDbg.hpp"
#include "Prompt_shell.hpp"
#include "Prompt_shortcut.hpp"
#include "Prompt_show.hpp"
#include "Prompt_sizes.hpp"
#include "Prompt_sleep.hpp"
#include "Prompt_sleepers.hpp"
#include "Prompt_sleeping.hpp"
#include "Prompt_so.hpp"
#include "Prompt_socket.hpp"
#include "Prompt_ssn.hpp"
#include "Prompt_ssnState.hpp"
#include "Prompt_stepTiming.hpp"
#include "Prompt_store.hpp"

#include "Prompt_tag.hpp"
#include "Prompt_tags.hpp"
#include "Prompt_takeMem.hpp"
#include "Prompt_taskQueues.hpp"
#include "Prompt_tempName.hpp"
#include "Prompt_threads.hpp"
#include "Prompt_timerEnd.hpp"
#include "Prompt_timerStart.hpp"
#include "Prompt_tpch.hpp"
#include "Prompt_trackMemory.hpp"

#include "Prompt_uptime.hpp"

#include "Prompt_version.hpp"

#include "Prompt_waiting.hpp"
#include "Prompt_with.hpp"
#include "Prompt_without.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
