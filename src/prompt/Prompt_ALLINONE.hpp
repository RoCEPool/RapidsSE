/*============================================================================*/
/* Prompt_ALLINONE.hpp -- all the prompt functions in onw file                */
/*============================================================================*/

void vRapidsSE_ApiDemo_tableScan(void);
void vRapidsSE_ApiDemo_getRows(void);
void vRapidsSE_ApiDemo_putRows(void);


// the following probably need to go into File?

namespace PROMPT
{
  struct _fileRand {
    i8   m_iRandom;
    char m_caLine[256];
  };


  int iFileRandCompare(const void * a, const void * b);

}

// the following probably need to go into FltLib?
void vHuh(d8 d);


// ??
#define C_ATTEMPTS 12



//------------------------------------------------------------------------------




namespace PROMPT
{
  bool bPrompt_antlr(SePrompt* pPrompt);
  bool bPrompt_apiDemo(SePrompt* pPrompt);
  bool bPrompt_args(SePrompt* pPrompt);
  bool bPrompt_ascii(SePrompt* pPrompt);
  bool bPrompt_attachMap(SePrompt* pPrompt);
  bool bPrompt_bp(SePrompt* pPrompt);
  bool bPrompt_bpa(SePrompt* pPrompt);
  bool bPrompt_bye(SePrompt* pPrompt);
  bool bPrompt_calc(SePrompt* pPrompt);
  bool bPrompt_cancel(SePrompt* pPrompt);
  bool bPrompt_cd(SePrompt* pPrompt);
  bool bPrompt_classes(SePrompt* pPrompt);
  bool bPrompt_cmds(SePrompt* pPrompt);
  bool bPrompt_config(SePrompt* pPrompt);
  bool bPrompt_constants(SePrompt* pPrompt);
  bool bPrompt_ddlLock(SePrompt* pPrompt);
  bool bPrompt_describe(SePrompt* pPrompt);
  bool bPrompt_dlist(SePrompt* pPrompt);
  bool bPrompt_dsv(SePrompt* pPrompt);
  bool bPrompt_dumpTokens(SePrompt* pPrompt);
  bool bPrompt_errno(SePrompt* pPrompt);
  bool bPrompt_executing(SePrompt* pPrompt);
  bool bPrompt_fact(SePrompt* pPrompt);
  bool bPrompt_file(SePrompt* pPrompt);
  bool bPrompt_fileCat(SePrompt* pPrompt);
  bool bPrompt_fileProc(SePrompt* pPrompt);
  bool bPrompt_fileRand(SePrompt* pPrompt);
  bool bPrompt_flist(SePrompt* pPrompt);
  bool bPrompt_floats(SePrompt* pPrompt);
  bool bPrompt_funs(SePrompt* pPrompt);
  bool bPrompt_giveMem(SePrompt* pPrompt);
  bool bPrompt_globals(SePrompt* pPrompt);
  bool bPrompt_hashDefs(SePrompt* pPrompt);
  bool bPrompt_heapSummary(SePrompt* pPrompt);
  bool bPrompt_question(SePrompt* pPrompt);
  bool bPrompt_help(SePrompt* pPrompt);
  bool bPrompt_hexdump(SePrompt* pPrompt);
  bool bPrompt_how(SePrompt* pPrompt);
  bool bPrompt_hw(SePrompt* pPrompt);
  bool bPrompt_indexEntries(SePrompt* pPrompt);
  bool bPrompt_job(SePrompt* pPrompt);
  bool bPrompt_lastJob(SePrompt* pPrompt);
  bool bPrompt_limits(SePrompt* pPrompt);
  bool bPrompt_llist(SePrompt* pPrompt);
  bool bPrompt_locks(SePrompt* pPrompt);
  bool bPrompt_log(SePrompt* pPrompt);
  bool bPrompt_logNote(SePrompt* pPrompt);
  bool bPrompt_logSection(SePrompt* pPrompt);
  bool bPrompt_ls(SePrompt* pPrompt);
  bool bPrompt_lvl(SePrompt* pPrompt);
  bool bPrompt_masks(SePrompt* pPrompt);
  bool bPrompt_memoryLeaks(SePrompt* pPrompt);
  bool bPrompt_memoryLeaks_dump(SePrompt* pPrompt);
  bool bPrompt_memTest(SePrompt* pPrompt);
  bool bPrompt_memory(SePrompt* pPrompt);
  bool bPrompt_mkfifo(SePrompt* pPrompt);
  bool bPrompt_now(SePrompt* pPrompt);
  bool bPrompt_numbers(SePrompt* pPrompt);
  bool bPrompt_objectList(SePrompt* pPrompt);
  bool bPrompt_objectValidate(SePrompt* pPrompt);
  bool bPrompt_olist(SePrompt* pPrompt);
  bool bPrompt_opdq(SePrompt* pPrompt);
  bool bPrompt_operands(SePrompt* pPrompt);
  bool bPrompt_opsys(SePrompt* pPrompt);
  bool bPrompt_plist(SePrompt* pPrompt);
  bool bPrompt_pmc(SePrompt* pPrompt);
  bool bPrompt_pow2(SePrompt* pPrompt);
  bool bPrompt_prefixes(SePrompt* pPrompt);
  bool bPrompt_print(SePrompt* pPrompt);
  bool bPrompt_procedure(SePrompt* pPrompt);
  bool bPrompt_prompt(SePrompt* pPrompt);
  bool bPrompt_promptDebug(SePrompt* pPrompt);
  bool bPrompt_pwd(SePrompt* pPrompt);
  bool bPrompt_question(SePrompt* pPrompt);
  bool bPrompt_rassert(SePrompt* pPrompt);
  bool bPrompt_ready(SePrompt* pPrompt);
  bool bPrompt_reap(SePrompt* pPrompt);
  bool bPrompt_resources(SePrompt* pPrompt);
  bool bPrompt_retired(SePrompt* pPrompt);
  bool bPrompt_rmfifo(SePrompt* pPrompt);
  bool bPrompt_rnd(SePrompt* pPrompt);
  bool bPrompt_scan(SePrompt* pPrompt);
  bool bPrompt_select(SePrompt* pPrompt);
  bool bPrompt_selfies(SePrompt* pPrompt);
  bool bPrompt_setDbg(SePrompt* pPrompt);
  bool bPrompt_shell(SePrompt* pPrompt);
  bool bPrompt_shortcut(SePrompt* pPrompt);
  bool bPrompt_show(SePrompt* pPrompt);
  bool bPrompt_sizes(SePrompt* pPrompt);
  bool bPrompt_sleep(SePrompt* pPrompt);
  bool bPrompt_sleepers(SePrompt* pPrompt);
  bool bPrompt_sleeping(SePrompt* pPrompt);
  bool bPrompt_snapshotDelta(SePrompt* pPrompt);
  bool bPrompt_snapshotInspect(SePrompt* pPrompt);
  bool bPrompt_snapshotList(SePrompt* pPrompt);
  bool bPrompt_snapshotNewer(SePrompt* pPrompt);
  bool bPrompt_snapshotObjects(SePrompt* pPrompt);
  bool bPrompt_so(SePrompt* pPrompt);
  bool bPrompt_socket(SePrompt* pPrompt);
  bool bPrompt_ssn(SePrompt* pPrompt);
  bool bPrompt_ssnState(SePrompt* pPrompt);
  bool bPrompt_stepTiming(SePrompt* pPrompt);
  bool bPrompt_store(SePrompt* pPrompt);
  bool bPrompt_tag(SePrompt* pPrompt);
  bool bPrompt_tags(SePrompt* pPrompt);
  bool bPrompt_takeMem(SePrompt* pPrompt);
  bool bPrompt_taskQueues(SePrompt* pPrompt);
  bool bPrompt_tempName(SePrompt* pPrompt);
  bool bPrompt_threads(SePrompt* pPrompt);
  bool bPrompt_timerEnd(SePrompt* pPrompt);
  bool bPrompt_timerStart(SePrompt* pPrompt);
  bool bPrompt_tools(SePrompt* pPrompt);
  bool bPrompt_tpch(SePrompt* pPrompt);
  bool bPrompt_trackMemory(SePrompt* pPrompt);
  bool bPrompt_uptime(SePrompt* pPrompt);
  bool bPrompt_version(SePrompt* pPrompt);
  bool bPrompt_wait_bg(SePrompt* pPrompt);
  bool bPrompt_waiting(SePrompt* pPrompt);
  bool bPrompt_with(SePrompt* pPrompt);
  bool bPrompt_without(SePrompt* pPrompt);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
