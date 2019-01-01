/*============================================================================*/
/* MungedSen.cpp - munged sen files                                           */
/*============================================================================*/
#include "StorageEngine.h"

void Task::vAddMunged(MP_Task mpTask)
{
  s48K sDefinition;

  // munge of se_folder.sen
  sDefinition =
    "\""
    "\\\"munged se_folder.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* se_folder.sen -- the set of folder related requests                        */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT se_folder.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of test_jData.sen
  sDefinition =
    "\""
    "\\\"munged test_jData.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* test_jData.sen -- generate jTest data                                      */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT test_jData.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of wordSets.sen
  sDefinition =
    "\""
    "\\\"munged wordSets.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* wordSets.sen -- additional high-level words                                */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT wordSets.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of Op_print.sen
  sDefinition =
    "\""
    "\\\"munged Op_print.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* op_print.sen -- print rows on console                                       */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT Op_print.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_AnalyzeIndex.sen
  sDefinition =
    "\""
    "\\\"munged job_AnalyzeIndex.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_AnalyzeIndex.sen -- Job Analyze Index                                  */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_AnalyzeIndex.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of db_index.sen
  sDefinition =
    "\""
    "\\\"munged db_index.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* db_index.sen -- index related words                                        */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT db_index.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of planners.sen
  sDefinition =
    "\""
    "\\\"munged planners.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* planners.sen -- the planners                                               */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT planners.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_CreateIndex.sen
  sDefinition =
    "\""
    "\\\"munged job_CreateIndex.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_CreateIndex.sen -- Job Create Index                                    */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_CreateIndex.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of wordSet_dicStack.sen
  sDefinition =
    "\""
    "\\\"munged wordSet_dicStack.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* wordSet_dicStack.sen -- dictionary stack related words                     */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT wordSet_dicStack.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of db_plan.sen
  sDefinition =
    "\""
    "\\\"munged db_plan.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* db_plan.sen -- plan related words                                          */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT db_plan.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_Periodic.sen
  sDefinition =
    "\""
    "\\\"munged job_Periodic.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_Periodic.sen -- periodic job                                           */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_Periodic.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of Op_read.sen
  sDefinition =
    "\""
    "\\\"munged Op_read.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* op_read.sen -- read records from file operator                              */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT Op_read.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of operators.sen
  sDefinition =
    "\""
    "\\\"munged operators.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* operators.sen -- the operators                                             */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT operators.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of Op_aggregate.sen
  sDefinition =
    "\""
    "\\\"munged Op_aggregate.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* op_aggregate.sen -- row aggregate by operator                              */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT Op_aggregate.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of planner_dfetch.sen
  sDefinition =
    "\""
    "\\\"munged planner_dfetch.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* planner_dfetch.sen -- distributed fetch planner                            */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT planner_dfetch.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_Ingest.sen
  sDefinition =
    "\""
    "\\\"munged job_Ingest.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_Ingest.sen -- Ingest Job                                               */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_Ingest.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of Op_merge.sen
  sDefinition =
    "\""
    "\\\"munged Op_merge.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* op_merge.sen -- row merge operator                                          */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT Op_merge.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_SubFetch.sen
  sDefinition =
    "\""
    "\\\"munged job_SubFetch.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_SubFetch.sen -- Job SubFetch                                           */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_SubFetch.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_Cf.sen
  sDefinition =
    "\""
    "\\\"munged job_Cf.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_Cf.sen -- Job Change Folder                                            */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_Cf.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of wordSet_core.sen
  sDefinition =
    "\""
    "\\\"munged wordSet_core.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* wordSet_core.sen -- core words                                             */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT wordSet_core.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_InsertTable.sen
  sDefinition =
    "\""
    "\\\"munged job_InsertTable.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_InsertTable.sen -- Job Insert Table                                    */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_InsertTable.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_SubTruncate.sen
  sDefinition =
    "\""
    "\\\"munged job_SubTruncate.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_SubTruncate.sen -- Job SubTruncate                                     */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_SubTruncate.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of Op_sort.sen
  sDefinition =
    "\""
    "\\\"munged Op_sort.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* op_sort.sen -- row sort operator                                            */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT Op_sort.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of Op_fetch.sen
  sDefinition =
    "\""
    "\\\"munged Op_fetch.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* op_fetch.sen -- table fetch (by-key) select operator                        */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT Op_fetch.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of se_docols.sen
  sDefinition =
    "\""
    "\\\"munged se_docols.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* se_docols.sen -- docol shorthands                                          */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT se_docols.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_InsertRows.sen
  sDefinition =
    "\""
    "\\\"munged job_InsertRows.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_InsertRows.sen -- Job Insert Rows                                      */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_InsertRows.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of wordSet_dic.sen
  sDefinition =
    "\""
    "\\\"munged wordSet_dic.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* wordSet_dic.sen -- dictionary related words                                */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT wordSet_dic.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_SubPublish.sen
  sDefinition =
    "\""
    "\\\"munged job_SubPublish.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_SubPublish.sen -- Job SubPublish                                       */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_SubPublish.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of planner_dex.sen
  sDefinition =
    "\""
    "\\\"munged planner_dex.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* planner_dex.sen -- distributed export planner                              */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT planner_dex.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_ListTenants.sen
  sDefinition =
    "\""
    "\\\"munged job_ListTenants.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_ListTenants.sen -- Job List Tenants                                    */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_ListTenants.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of wordSet_valset.sen
  sDefinition =
    "\""
    "\\\"munged wordSet_valset.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* wordSet_valset.sen -- valset related words                                 */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT wordSet_valset.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_DropIndex.sen
  sDefinition =
    "\""
    "\\\"munged job_DropIndex.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_DropIndex.sen -- Job Index Table                                       */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_DropIndex.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_DropSchema.sen
  sDefinition =
    "\""
    "\\\"munged job_DropSchema.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_DropSchema.sen -- Job Drop Schama                                      */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_DropSchema.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_Docol.sen
  sDefinition =
    "\""
    "\\\"munged job_Docol.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_Docol.sen -- Job Docol                                                 */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_Docol.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of Op_fan.sen
  sDefinition =
    "\""
    "\\\"munged Op_fan.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* op_fan.sen -- row fan out operator                                          */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT Op_fan.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_Connector.sen
  sDefinition =
    "\""
    "\\\"munged job_Connector.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_Connector.sen -- connector job                                         */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_Connector.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_TruncateTable.sen
  sDefinition =
    "\""
    "\\\"munged job_TruncateTable.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_TruncateTable.sen -- Job Truncate Table                                */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_TruncateTable.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of wordSet_corePlus.sen
  sDefinition =
    "\""
    "\\\"munged wordSet_corePlus.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* wordSet_corePlus.sen -- words complentary to core words                    */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT wordSet_corePlus.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of operator.sen
  sDefinition =
    "\""
    "\\\"munged operator.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* operator.sen -- operator modifier                                          */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT operator.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of Op_extract.sen
  sDefinition =
    "\""
    "\\\"munged Op_extract.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* op_extract.sen -- extract keys operator                                     */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT Op_extract.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_SizeTable.sen
  sDefinition =
    "\""
    "\\\"munged job_SizeTable.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_SizeTable.sen -- Job Size Table                                        */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_SizeTable.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of se_munge.sen
  sDefinition =
    "\""
    "\\\"munged se_munge.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* se_munge.sen -- create the .cpp from .sen                                  */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT se_munge.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_LoadTable.sen
  sDefinition =
    "\""
    "\\\"munged job_LoadTable.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_LoadTable.sen -- Job Load Table                                        */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_LoadTable.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_TableSizes.sen
  sDefinition =
    "\""
    "\\\"munged job_TableSizes.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_TableSizes.sen -- Job Table Sizes                                      */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_TableSizes.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of wordSet_useful.sen
  sDefinition =
    "\""
    "\\\"munged wordSet_useful.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* wordSet_useful.sen -- misc useful stuff                                    */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT wordSet_useful.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of Op_next.sen
  sDefinition =
    "\""
    "\\\"munged Op_next.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* op_next.sen -- flow next operator                                           */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT Op_next.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of Op_range.sen
  sDefinition =
    "\""
    "\\\"munged Op_range.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* op_range.sen -- table range select operator                                 */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT Op_range.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of test_unit.sen
  sDefinition =
    "\""
    "\\\"munged test_unit.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* test_unit.sen -- unit testing                                              */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT test_unit.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_VisualizeIndex.sen
  sDefinition =
    "\""
    "\\\"munged job_VisualizeIndex.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_VisualizeIndex.sen -- Job Visualize Index                              */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_VisualizeIndex.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job__Cancel.sen
  sDefinition =
    "\""
    "\\\"munged job__Cancel.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job__Cancel.sen -- generic job related words                               */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job__Cancel.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_Space.sen
  sDefinition =
    "\""
    "\\\"munged job_Space.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_Space.sen -- Job Space                                                 */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_Space.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of Op_decider.sen
  sDefinition =
    "\""
    "\\\"munged Op_decider.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* op_decider.sen -- flow decider operator                                    */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT Op_decider.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of test_iData.sen
  sDefinition =
    "\""
    "\\\"munged test_iData.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* test_iData.sen -- generate iTest data                                      */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT test_iData.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_SelectTable.sen
  sDefinition =
    "\""
    "\\\"munged job_SelectTable.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_SelectTable.sen -- Job Select Table                                    */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_SelectTable.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of test_1m.sen
  sDefinition =
    "\""
    "\\\"munged test_1m.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* test_1m.sen -- a million loop test                                         */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT test_1m.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of Op_write.sen
  sDefinition =
    "\""
    "\\\"munged Op_write.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* op_write.sen -- write rows to file operator                                 */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT Op_write.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of Op_mark.sen
  sDefinition =
    "\""
    "\\\"munged Op_mark.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* op_mark.sen -- mark operator                                                */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT Op_mark.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of Op_triggerRange.sen
  sDefinition =
    "\""
    "\\\"munged Op_triggerRange.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* op_triggerRange.sen -- flow trigger operator                                */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT Op_triggerRange.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of jobModel.sen
  sDefinition =
    "\""
    "\\\"munged jobModel.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* jobModel.sen -- Job model                                                  */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT jobModel.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of test_gGen.sen
  sDefinition =
    "\""
    "\\\"munged test_gGen.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* test_gGen.sen -- generate gGen files                                       */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT test_gGen.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_subPurger.sen
  sDefinition =
    "\""
    "\\\"munged job_subPurger.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_subPurger.sen -- Job SubPurge                                           */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_subPurger.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of Op_send.sen
  sDefinition =
    "\""
    "\\\"munged Op_send.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* op_send.sen -- send records to socket operator                              */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT Op_send.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of test_tData.sen
  sDefinition =
    "\""
    "\\\"munged test_tData.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* test_tData.sen -- generate tTest data                                      */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT test_tData.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_DropTenant.sen
  sDefinition =
    "\""
    "\\\"munged job_DropTenant.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_DropTenant.sen -- Job Drop Tenant                                      */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_DropTenant.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_ListColumns.sen
  sDefinition =
    "\""
    "\\\"munged job_ListColumns.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_ListColumns.sen -- Job list Columns                                    */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_ListColumns.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of Op_sink.sen
  sDefinition =
    "\""
    "\\\"munged Op_sink.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* op_sink.sen -- sink operator                                                */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT Op_sink.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_Export.sen
  sDefinition =
    "\""
    "\\\"munged job_Export.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_Export.sen -- Job Export                                               */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_Export.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of Op_scan.sen
  sDefinition =
    "\""
    "\\\"munged Op_scan.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* op_scan.sen -- table scan select operator                                   */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT Op_scan.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job__Cancelled.sen
  sDefinition =
    "\""
    "\\\"munged job__Cancelled.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job__Cancelled.sen -- generic job related words                            */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job__Cancelled.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of grid_node.sen
  sDefinition =
    "\""
    "\\\"munged grid_node.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* grid_node.sen -- node routines                                             */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT grid_node.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_WriteBuffer.sen
  sDefinition =
    "\""
    "\\\"munged job_WriteBuffer.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_WriteBuffer.sen -- Write Buffer Job                                    */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_WriteBuffer.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of wordSet_am.sen
  sDefinition =
    "\""
    "\\\"munged wordSet_am.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* wordSet_am.sen -- application mark related words                           */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT wordSet_am.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of jobWords.sen
  sDefinition =
    "\""
    "\\\"munged jobWords.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* jobWords.sen -- Job related words                                          */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT jobWords.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job__Executed.sen
  sDefinition =
    "\""
    "\\\"munged job__Executed.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job__Executed.sen -- generic job related words                             */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job__Executed.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_ListIdxColumns.sen
  sDefinition =
    "\""
    "\\\"munged job_ListIdxColumns.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_ListColumns.sen -- Job list Columns                                    */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_ListIdxColumns.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_FetchRows.sen
  sDefinition =
    "\""
    "\\\"munged job_FetchRows.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_FetchRows.sen -- Job Fetch Rows                                        */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_FetchRows.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_DropCatalog.sen
  sDefinition =
    "\""
    "\\\"munged job_DropCatalog.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_DropCatalog.sen -- Job Drop Catalog                                    */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_DropCatalog.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of Op_upper.sen
  sDefinition =
    "\""
    "\\\"munged Op_upper.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* op_upper.sen -- table upper bound select operator                           */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT Op_upper.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of Op_distinct.sen
  sDefinition =
    "\""
    "\\\"munged Op_distinct.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* op_distinct.sen -- row distinct operator                                    */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT Op_distinct.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of test_dData.sen
  sDefinition =
    "\""
    "\\\"munged test_dData.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* test_dData.sen -- generate d test data                                     */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT test_dData.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_SizeIndex.sen
  sDefinition =
    "\""
    "\\\"munged job_SizeIndex.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_SizeIndex.sen -- Job Size Table                                        */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_SizeIndex.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of grid_grid.sen
  sDefinition =
    "\""
    "\\\"munged grid_grid.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* grid.sen -- grid routines                                                  */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT grid_grid.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_Start.sen
  sDefinition =
    "\""
    "\\\"munged job_Start.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_Start.sen -- Start System                                              */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_Start.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_Repeater.sen
  sDefinition =
    "\""
    "\\\"munged job_Repeater.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_Repeater.sen -- Repeater Job                                           */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_Repeater.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of symbolicArgs.sen
  sDefinition =
    "\""
    "\\\"munged symbolicArgs.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* symbolicArgs.sen -- symbolic operator arguments                            */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT symbolicArgs.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job__Execute.sen
  sDefinition =
    "\""
    "\\\"munged job__Execute.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job__Execute.sen -- generic job related words                              */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job__Execute.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of Op_filter.sen
  sDefinition =
    "\""
    "\\\"munged Op_filter.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* op_filter.sen -- row filter operator                                        */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT Op_filter.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_WriteRow.sen
  sDefinition =
    "\""
    "\\\"munged job_WriteRow.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_WriteRowr.sen -- Write Row Job                                         */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_WriteRow.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of db_schema.sen
  sDefinition =
    "\""
    "\\\"munged db_schema.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* db_schema.sen -- schema related words                                      */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT db_schema.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of Op_join.sen
  sDefinition =
    "\""
    "\\\"munged Op_join.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* op_join.sen -- row join operator                                            */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT Op_join.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_Sink.sen
  sDefinition =
    "\""
    "\\\"munged job_Sink.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_Sink.sen -- Sink Job                                                   */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_Sink.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of wordSet_object.sen
  sDefinition =
    "\""
    "\\\"munged wordSet_object.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* wordSet_object.sen -- object words                                         */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT wordSet_object.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_UseSchema.sen
  sDefinition =
    "\""
    "\\\"munged job_UseSchema.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_UseSchema.sen -- Job Use Schema                                        */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_UseSchema.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_SubLoader.sen
  sDefinition =
    "\""
    "\\\"munged job_SubLoader.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_SubLoader.sen -- Job Table SubLoader                                   */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_SubLoader.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job__Begun.sen
  sDefinition =
    "\""
    "\\\"munged job__Begun.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job__Begun.sen -- generic job related words                                */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job__Begun.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_ListPartitions.sen
  sDefinition =
    "\""
    "\\\"munged job_ListPartitions.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_ListPartitions.sen -- Job list partitions                              */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_ListPartitions.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_ListSchemas.sen
  sDefinition =
    "\""
    "\\\"munged job_ListSchemas.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_ListSchemas.sen -- Job List Schemas                                    */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_ListSchemas.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_Purge.sen
  sDefinition =
    "\""
    "\\\"munged job_Purge.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_Purge.sen -- Purge Job                                                 */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_Purge.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_CreateSchema.sen
  sDefinition =
    "\""
    "\\\"munged job_CreateSchema.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_CreateSchema.sen -- Job Create Schema                                  */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_CreateSchema.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of testing.sen
  sDefinition =
    "\""
    "\\\"munged testing.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* testing.sen -- testing related words                                       */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT testing.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of wordSet_debug.sen
  sDefinition =
    "\""
    "\\\"munged wordSet_debug.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* wordSet_debug.sen -- debugging related words                               */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT wordSet_debug.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_Status.sen
  sDefinition =
    "\""
    "\\\"munged job_Status.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_Status.sen -- Job Status                                               */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_Status.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_Receiver.sen
  sDefinition =
    "\""
    "\\\"munged job_Receiver.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_Receiver.sen -- receiver job                                           */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_Receiver.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_Boot.sen
  sDefinition =
    "\""
    "\\\"munged job_Boot.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_Boot.sen -- compile the dictionary                                     */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_Boot.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of db_catalog.sen
  sDefinition =
    "\""
    "\\\"munged db_catalog.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* db_catalog.sen -- catalog related words                                    */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT db_catalog.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_ListTables.sen
  sDefinition =
    "\""
    "\\\"munged job_ListTables.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_ListTables.sen -- Job List Tables                                      */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_ListTables.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_StartCluster.sen
  sDefinition =
    "\""
    "\\\"munged job_StartCluster.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_StartCluster.sen -- Job Start Cluster                                  */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_StartCluster.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of test_arithmetic.sen
  sDefinition =
    "\""
    "\\\"munged test_arithmetic.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* test_arithemetic.sen -- arithmetic unit testing                            */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT test_arithmetic.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of se.sen
  sDefinition =
    "\""
    "\\\"munged se.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* se.sen -- se                                                               */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT se.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of db_table.sen
  sDefinition =
    "\""
    "\\\"munged db_table.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* db_table.sen -- table related words                                        */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT db_table.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_Lf.sen
  sDefinition =
    "\""
    "\\\"munged job_Lf.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_Lf.sen -- Job list folder                                              */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_Lf.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_Ping.sen
  sDefinition =
    "\""
    "\\\"munged job_Ping.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_Ping.sen -- Job Ping                                                   */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_Ping.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_InsertRow.sen
  sDefinition =
    "\""
    "\\\"munged job_InsertRow.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_InsertRow.sen -- Job Insert Row                                        */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_InsertRow.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_CompactIndex.sen
  sDefinition =
    "\""
    "\\\"munged job_CompactIndex.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_CompactIndex.sen -- Job Compact Index                                  */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_CompactIndex.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of wordSet_pp.sen
  sDefinition =
    "\""
    "\\\"munged wordSet_pp.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* wordSet_pp.sen -- performance point words                                  */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT wordSet_pp.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_DumpIndex.sen
  sDefinition =
    "\""
    "\\\"munged job_DumpIndex.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_DumpIndex.sen -- Job Dump Table                                        */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_DumpIndex.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_AnalyzeTable.sen
  sDefinition =
    "\""
    "\\\"munged job_AnalyzeTable.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_AnalyzeTable.sen -- Job Analyze Table                                  */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_AnalyzeTable.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_ConnectCluster.sen
  sDefinition =
    "\""
    "\\\"munged job_ConnectCluster.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_ConnectCluster.sen -- Job Connect Cluster                              */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_ConnectCluster.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job__Ended.sen
  sDefinition =
    "\""
    "\\\"munged job__Ended.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job__Ended.sen -- generic job related words                                */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job__Ended.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of db_partition.sen
  sDefinition =
    "\""
    "\\\"munged db_partition.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* db_partition.sen -- partition related words                                */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT db_partition.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of task.sen
  sDefinition =
    "\""
    "\\\"munged task.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* task.sen -- task ipc primitives                                            */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT task.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_ShowCluster.sen
  sDefinition =
    "\""
    "\\\"munged job_ShowCluster.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_ShowCluster.sen -- Show Cluster                                        */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_ShowCluster.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_Generate.sen
  sDefinition =
    "\""
    "\\\"munged job_Generate.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_Generate.sen -- Generate Data Job                                      */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_Generate.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_ListIndexes.sen
  sDefinition =
    "\""
    "\\\"munged job_ListIndexes.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_ListIndexes.sen -- Job list Indexes                                    */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_ListIndexes.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_Sanity.sen
  sDefinition =
    "\""
    "\\\"munged job_Sanity.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_Sanity.sen -- Job Sanity                                               */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_Sanity.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of Op_lower.sen
  sDefinition =
    "\""
    "\\\"munged Op_lower.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* op_lower.sen -- table lower bound select operator                           */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT Op_lower.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_WaitCluster.sen
  sDefinition =
    "\""
    "\\\"munged job_WaitCluster.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_WaitCluster.sen -- Job Wait CLuster                                    */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_WaitCluster.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of Op_emit.sen
  sDefinition =
    "\""
    "\\\"munged Op_emit.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* op_emit.sen -- row emit operator                                            */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT Op_emit.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of db_tenant.sen
  sDefinition =
    "\""
    "\\\"munged db_tenant.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* db_tenant.sen -- tenant related words                                      */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT db_tenant.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of test_logic.sen
  sDefinition =
    "\""
    "\\\"munged test_logic.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* test_logic.sen -- logic unit testing                                       */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT test_logic.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of test_misc.sen
  sDefinition =
    "\""
    "\\\"munged test_misc.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* test_misc.sen -- miscellaneous testing                                   */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT test_misc.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_CreateTable.sen
  sDefinition =
    "\""
    "\\\"munged job_CreateTable.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_CreateTable.sen -- Job Create Table                                    */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_CreateTable.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of test_flow.sen
  sDefinition =
    "\""
    "\\\"munged test_flow.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* test_flow.sen -- flow control unit testing                                 */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT test_flow.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of dbobjects.sen
  sDefinition =
    "\""
    "\\\"munged dbobjects.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* dbObjects.sen -- dbObjects group                                           */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT dbobjects.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of Op_tee.sen
  sDefinition =
    "\""
    "\\\"munged Op_tee.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* op_tee.sen -- row tee operator                                              */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT Op_tee.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of Op_project.sen
  sDefinition =
    "\""
    "\\\"munged Op_project.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* op_project.sen -- row project operator                                      */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT Op_project.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_Set.sen
  sDefinition =
    "\""
    "\\\"munged job_Set.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_Set.sen -- Job set                                                     */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_Set.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_Publish.sen
  sDefinition =
    "\""
    "\\\"munged job_Publish.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_Publish.sen -- Publish Job                                             */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_Publish.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of Op_recv.sen
  sDefinition =
    "\""
    "\\\"munged Op_recv.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* op_recv.sen -- receive records from socket operator                         */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT Op_recv.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_NetHog.sen
  sDefinition =
    "\""
    "\\\"munged job_NetHog.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_NetHog.sen -- Network Hog Job                                          */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_NetHog.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_VerifyIndex.sen
  sDefinition =
    "\""
    "\\\"munged job_VerifyIndex.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_VerifyIndex.sen -- Job Verify Index                                    */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_VerifyIndex.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of Op_limit.sen
  sDefinition =
    "\""
    "\\\"munged Op_limit.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* op_limit.sen -- row limit operator                                          */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT Op_limit.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_CreateTenant.sen
  sDefinition =
    "\""
    "\\\"munged job_CreateTenant.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_CreateTenant.sen -- Job Create Tenant                                  */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_CreateTenant.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of Op_response.sen
  sDefinition =
    "\""
    "\\\"munged Op_response.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* op_response.sen -- response operator                                        */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT Op_response.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of tasks.sen
  sDefinition =
    "\""
    "\\\"munged tasks.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* tasks.sen -- tasks group                                                   */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT tasks.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_ListPartColumns.sen
  sDefinition =
    "\""
    "\\\"munged job_ListPartColumns.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_ListPartColumns.sen -- Job list Partition Key Columns                  */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_ListPartColumns.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_Pwf.sen
  sDefinition =
    "\""
    "\\\"munged job_Pwf.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_Pwf.sen -- Job Present Working Folder                                  */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_Pwf.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of Op_triggerScan.sen
  sDefinition =
    "\""
    "\\\"munged Op_triggerScan.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* op_triggerScan.sen -- flow trigger operator                                 */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT Op_triggerScan.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of gridModel.sen
  sDefinition =
    "\""
    "\\\"munged gridModel.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* gridModel.sen -- grid model group                                          */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT gridModel.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_ListCatalogs.sen
  sDefinition =
    "\""
    "\\\"munged job_ListCatalogs.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_ListCatalogs.sen -- Job List Catalogs                                  */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_ListCatalogs.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_Alarm.sen
  sDefinition =
    "\""
    "\\\"munged job_Alarm.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_Alarm.sen -- alarm job                                                 */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_Alarm.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_Cat.sen
  sDefinition =
    "\""
    "\\\"munged job_Cat.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_Cat.sen -- Cat Job                                                     */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_Cat.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job__End.sen
  sDefinition =
    "\""
    "\\\"munged job__End.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job__End.sen -- generic job related words                                  */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job__End.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_DropTable.sen
  sDefinition =
    "\""
    "\\\"munged job_DropTable.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_DropTable.sen -- Job Drop Table                                        */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_DropTable.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_CreateCatalog.sen
  sDefinition =
    "\""
    "\\\"munged job_CreateCatalog.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_CreateCatalog.sen -- Job Create Catalog                                */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_CreateCatalog.sen"_Stz
  ;

  vCompileConstant(sDefinition);

  // munge of job_Answer.sen
  sDefinition =
    "\""
    "\\\"munged job_Answer.sen \\\" .verbose \n"
    "/*============================================================================*/\n"
    "/* job_Answer.sen -- answer job                                               */\n"
    "/*============================================================================*/\n"
    "\""
  " CONSTANT job_Answer.sen"_Stz
  ;

  vCompileConstant(sDefinition);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
