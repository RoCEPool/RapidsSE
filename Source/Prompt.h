/*============================================================================*/
/* Prompt.h -- Prompt namespace                                               */
/*============================================================================*/
#pragma once

#include "Verbs.h"

//------------------------------------------------------------------------------

namespace Prompt
{
  void vCheckAssumptions(void);
}

//------------------------------------------------------------------------------

#include "ShortCut.hpp"
typedef VectorNL_t<CN_SHORTCUTVEC, ShortCut, C_MAX_SHORTCUTS> ShortCutVec;

#include "SePrompt.hpp"

#include "PromptDefn.hpp"
#include "PromptDesc.hpp"
#include "PromptCatalog.hpp"
#include "PromptHelper.hpp"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#include "Prompt_apiState.hpp"
#include "Prompt_args.hpp"
#include "Prompt_ascii.hpp"
#include "Prompt_attachMap.hpp"

#include "Prompt_bp.hpp"
#include "Prompt_bye.hpp"

#include "Prompt_cache.hpp"
#include "Prompt_cd.hpp"
#include "Prompt_classes.hpp"
#include "Prompt_cmds.hpp"
#include "Prompt_config.hpp"
#include "Prompt_constants.hpp"

#include "Prompt_enums.hpp"
#include "Prompt_errno.hpp"
#include "Prompt_executing.hpp"

#include "Prompt_file.hpp"
#include "Prompt_fileCat.hpp"
#include "Prompt_fileProc.hpp"
#include "Prompt_floats.hpp"
#include "Prompt_funs.hpp"

#include "Prompt_giveMem.hpp"

#include "Prompt_hashDefs.hpp"
#include "Prompt_heapMem.hpp"
#include "Prompt_help.hpp"
#include "Prompt_hexdump.hpp"
#include "Prompt_hw.hpp"

#include "Prompt_indexEntries.hpp"

#include "Prompt_job.hpp"
#include "Prompt_jobcat.hpp"
#include "Prompt_jobClass.hpp"

#include "Prompt_limits.hpp"
#include "Prompt_locks.hpp"
#include "Prompt_log.hpp"
#include "Prompt_logNote.hpp"
#include "Prompt_logSection.hpp"
#include "Prompt_logWrite.hpp"
#include "Prompt_ls.hpp"
#include "Prompt_lvl.hpp"

#include "Prompt_markApi.hpp"
#include "Prompt_markRes.hpp"
#include "Prompt_masks.hpp"
#include "Prompt_memory.hpp"
#include "Prompt_memTest.hpp"
#include "Prompt_mkfifo.hpp"
#include "Prompt_mon.hpp"
#include "Prompt_mri.hpp"

#include "Prompt_now.hpp"
#include "Prompt_numbers.hpp"

#include "Prompt_olist.hpp"
#include "Prompt_ops.hpp"
#include "Prompt_opsys.hpp"

#include "Prompt_plist.hpp"
#include "Prompt_pmc.hpp"
#include "Prompt_pow2.hpp"
#include "Prompt_prefixes.hpp"
#include "Prompt_print.hpp"
#include "Prompt_pwd.hpp"

#include "Prompt_question.hpp"

#include "Prompt_rassert.hpp"
#include "Prompt_ready.hpp"
#include "Prompt_resources.hpp"
#include "Prompt_rmfifo.hpp"
#include "Prompt_rnd.hpp"

#include "Prompt_se.hpp"
#include "Prompt_setDbg.hpp"
#include "Prompt_shell.hpp"
#include "Prompt_shortcut.hpp"
#include "Prompt_sizes.hpp"
#include "Prompt_sleep.hpp"
#include "Prompt_sleepers.hpp"
#include "Prompt_sleeping.hpp"
#include "Prompt_so.hpp"
#include "Prompt_socket.hpp"
#include "Prompt_ssn.hpp"
#include "Prompt_st.hpp"

#include "Prompt_takeMem.hpp"
#include "Prompt_taskQueues.hpp"
#include "Prompt_tempName.hpp"
#include "Prompt_threads.hpp"
#include "Prompt_timerEnd.hpp"
#include "Prompt_timerStart.hpp"
#include "Prompt_tpch.hpp"

#include "Prompt_uptime.hpp"

#include "Prompt_version.hpp"

#include "Prompt_waiting.hpp"


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
