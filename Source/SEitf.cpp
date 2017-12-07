/*============================================================================*/
/* SEitf.cpp - se interface                                                   */
/*============================================================================*/

#include "Interface.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

struct ReqTable {
  Stz* aCommand;
  bool (SEitf::*aFunc)(TokenVec& xTokenVec, i8 iReqNo);
  Stz* aRequest;
};

ReqTable SeRequests[] =
{
 { "addcol"_Stz,         &SEitf::bAddColumn,            "addColumn_req"_Stz       },
 { "antbl"_Stz,          &SEitf::bAnalyzeTable,         "analyzeTable_req"_Stz    },
 { "anidx"_Stz,          &SEitf::bAnalyzeIndex,         "analyzeIndex_req"_Stz    },
 { "addidxcol"_Stz,      &SEitf::bAddIndexColumn,       "addIdxColumn_req"_Stz    },
 { "addpartcol"_Stz,     &SEitf::bAddPartitionColumn,   "addPartColumn_req"_Stz   },
 { "cf"_Stz,             &SEitf::bChangeFolder,         "cf_req"_Stz              },
 { "checkpoint"_Stz,     &SEitf::bCheckpoint,           "checkpoint_req"_Stz      },
 { "connectCluster"_Stz, &SEitf::bConnectCluster,       "connectCluster_req"_Stz  },
 { "cp"_Stz,             &SEitf::bCopyTable,            "copyTable_req"_Stz       },
 { "danidx"_Stz,         &SEitf::bDanalyzeIndex,        "danalyzeIndex_req"_Stz   },
 { "dantbl"_Stz,         &SEitf::bDanalyzeTable,        "danalyzeTable_req"_Stz   },
 { "dex"_Stz,            &SEitf::bDistExportTable,      "distExport_table"_Stz    },
 { "dfetch"_Stz,         &SEitf::bDistFetchRowsIndex,   "DistFetchRowsIndex_req"_Stz},
 { "docol"_Stz,          &SEitf::bDocol,                "docol_req"_Stz           },
 { "displayOff"_Stz,     &SEitf::bDisplayOff,           "displayOff_req"_Stz      },
 { "displayOn"_Stz,      &SEitf::bDisplayOn,            "displayOn_req"_Stz       },
 { "dumpidx"_Stz,        &SEitf::bDumpIndex,            "dumpIndex_req"_Stz       },
 { "dump"_Stz,           &SEitf::bDumpTable,            "dumpTable_req"_Stz       },
 { "endmkidx"_Stz,       &SEitf::bEndCreateIndex,       "endCreateIndex_req"_Stz  },
 { "endmktbl"_Stz,       &SEitf::bEndCreateTable,       "endCreateTable_req"_Stz  },
 { "ex"_Stz,             &SEitf::bExportTable,          "exportTable_req"_Stz     },
 { "fetch"_Stz,          &SEitf::bFetchRows,            "fetchRowsIndex_req"_Stz  },
 { "getAttachMap"_Stz,   &SEitf::bGetAttachMap,         "getAttachMap_req"_Stz    },
 { "getFreeSpace"_Stz,   &SEitf::bGetFreeSpace,         "getFreeSpace"_Stz        },
 { "getInUseSpace"_Stz,  &SEitf::bGetInUseSpace,        "getInUseSpace"_Stz       },
 { "getidx"_Stz,         &SEitf::bGetIndex,             "getIndex_req"_Stz        },
 { "gettbl"_Stz,         &SEitf::bGetTable,             "getTable_req"_Stz        },
 { "gettblNew"_Stz,      &SEitf::bGetTableNew,          "getTableNew_req"_Stz     },
 { "gettsk"_Stz,         &SEitf::bGetTask,              "getTask_req"_Stz         },
 { "getTotalSpace"_Stz,  &SEitf::bGetTotalSpace,        "getTotalSpace"_Stz       },
 { "TblInUse"_Stz,       &SEitf::bInUseTable,           "tableInuse_req"_Stz      },
 { "ld"_Stz,             &SEitf::bLoadTable,            "loadTable_req"_Stz       },
 { "lds"_Stz,            &SEitf::bLoadTableStream,      "loadTableStream_req"_Stz },
 { "lf"_Stz,             &SEitf::bListFolder,           "lf_req"_Stz              },
 { "lscat"_Stz,          &SEitf::bListCatalogs,         "listCatalogs_req"_Stz    },
 { "lscol"_Stz,          &SEitf::bListColumns,          "listColumns_req"_Stz     },
 { "lsidx"_Stz,          &SEitf::bListIndices,          "listIndexes_req"_Stz     },
 { "lsidxcol"_Stz,       &SEitf::bListIndexColumns,     "listIndexColumns_req"_Stz },
 { "lspartcol"_Stz,      &SEitf::bListPartitionColumns, "listPartitionColumns_req"_Stz },
 { "lssch"_Stz,          &SEitf::bListSchemas,          "listSchemas_req"_Stz     },
 { "lstbl"_Stz,          &SEitf::bListTables,           "listTables_req"_Stz      },
 { "lsten"_Stz,          &SEitf::bListTenants,          "listTenants_req"_Stz     },
 { "mkcat"_Stz,          &SEitf::bCreateCatalog,        "createCatalog_req"_Stz   },
 { "mkidx"_Stz,          &SEitf::bCreateIndex,          "createIndex_req"_Stz     },
 { "mksch"_Stz,          &SEitf::bCreateSchema,         "createSchema_req"_Stz    },
 { "mktbl"_Stz,          &SEitf::bCreateTable,          "createTable_req"_Stz     },
 { "mkten"_Stz,          &SEitf::bCreateTenant,         "createTenant_req"_Stz    },
 { "pwf"_Stz,            &SEitf::bPrintWorkingFolder,   "pwf_req"_Stz             },
 { "reload"_Stz,         &SEitf::bReload,               "reload_req"_Stz          },
 { "retidx"_Stz,         &SEitf::bRetIndex,             "returnIndex_req"_Stz     },
 { "rettbl"_Stz,         &SEitf::bRetTable,             "returnTable_req"_Stz     },
 { "rettsk"_Stz,         &SEitf::bRetTask,              "returnTask_req"_Stz      },
 { "rf"_Stz,             &SEitf::bRemoveFolder,         "remove_folder"_Stz       },
 { "rmcat"_Stz,          &SEitf::bDropCatalog,          "dropCatalog_req"_Stz     },
 { "rmdic"_Stz,          &SEitf::bRemoveFolder,         "remove_folder"_Stz       },
 { "rmidx"_Stz,          &SEitf::bDropIndex,            "dropIndex_req"_Stz       },
 { "rmsch"_Stz,          &SEitf::bDropSchema,           "dropSchema_req"_Stz      },
 { "rmtbl"_Stz,          &SEitf::bDropTable,            "dropTable_req"_Stz       },
 { "rmten"_Stz,          &SEitf::bDropTenant,           "dropTenant_req"_Stz      },
 { "sanity"_Stz,         &SEitf::bNodeSanity,           "nodeSanity_req"_Stz      },
 { "setAttachMap"_Stz,   &SEitf::bSetAttachMap,         "setAttachMap_req"_Stz    },
 { "setidxunique"_Stz,   &SEitf::bIndexSetUnique,       "indexSetUnique_req"_Stz  },
 { "space"_Stz,          &SEitf::bPoolSpace,            "poolSpace_req"_Stz       },
 { "spnFetch"_Stz,       &SEitf::bSpawnFetchRows,       "spawnfetchRows_apiReq"_Stz },
 { "status"_Stz,         &SEitf::bNodeStatus,           "nodeStatus_req"_Stz      },
 { "szidx"_Stz,          &SEitf::bSizeIndex,            "sizeIndex_req"_Stz       },
 { "sz"_Stz,             &SEitf::bSizeTable,            "sizeTable_req"_Stz       },
 { "tableSizes"_Stz,     &SEitf::bSizeTables,           "sizeTables_req"_Stz      },
 { "tr"_Stz,             &SEitf::bTruncateTable,        "truncateTable_req"_Stz   },
 { "tridx"_Stz,          &SEitf::bTruncateIndex,        "truncateIndex_req"_Stz   },
 { "txBeg"_Stz,          &SEitf::bTxBegin,              "txBegin_req"_Stz         },
 { "txCmt"_Stz,          &SEitf::bTxCommit,             "txCommit_req"_Stz        },
 { "txDmp"_Stz,          &SEitf::bTxDumpAll,            "txDumpAll_req"_Stz       },
 { "txEnd"_Stz,          &SEitf::bTxEnd,                "txEnd_req"_Stz           },
 { "txRbk"_Stz,          &SEitf::bTxRollback,           "txRollback_req"_Stz      },
 { "usesch"_Stz,         &SEitf::bUseSchema,            "useSchema_req"_Stz       },
 { "ping"_Stz,           &SEitf::bPing,                 "ping_req"_Stz            },
 { "mon"_Stz,            &SEitf::bGetMonitor,           "getMonitor_req"_Stz      },
 { "print"_Stz,          &SEitf::bPrint,                nullptr                   },
 { "seitf"_Stz,          &SEitf::bSeitfCount,           "seitfCount_req"_Stz      },
 { "vfyidx"_Stz,         &SEitf::bVerifyIndex,          "verifyIndex_req"_Stz     },
 { "version"_Stz,        &SEitf::bGetVersion,           "getVersion_req"_Stz      },
 { "visidx"_Stz,         &SEitf::bVisualizeIndex,       "visualizeIndex_req"_Stz  },
 { nullptr,              nullptr,                       nullptr }
};

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
SEitf* Interface::_pSEitf(void* aAddr)
{
  SEitf* pSEitf = reinterpret_cast<SEitf*>(aAddr);
  
  if (pSEitf && CN_SEITF != pSEitf->eClass())
    vTrace("INVALID SEITF %p %s\n"_Stz, pSEitf, aCN[pSEitf->eClass()]);
  
  RASSERT( !pSEitf || CN_SEITF == pSEitf->eClass() );
  
  return pSEitf;
}

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
SEitf* Interface::_pSEitf(i8 iAddr)
{
  SEitf* pSEitf = reinterpret_cast<SEitf*>(iAddr);
  
  if (pSEitf && CN_SEITF != pSEitf->eClass())
    vTrace("INVALID SEITF %p %s\n"_Stz, pSEitf, aCN[pSEitf->eClass()]);
  
  RASSERT( !pSEitf || CN_SEITF == pSEitf->eClass() );
  
  return pSEitf;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* SEitf::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Object* pObject = Heap::pAllocate(CN_SEITF);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void SEitf::operator delete(void* pSEinterfaceAddress)
{
  SEitf* pSEitf = reinterpret_cast<SEitf*>(pSEinterfaceAddress);
  
  Heap::vFree(pSEitf);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SEitf::SEitf(i8 iPartition)
: Object(CN_SEITF)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  // pid
  m_iPid = Process::iGetPid();

  m_iSlots = gpAttachMap->m_pGrid->iSlots();
  m_iPartition  = iPartition;
  
  //m_xPathStack;
  m_pBufferQueue    = nullptr;
  m_pRequestQueue   = nullptr;
  m_pResponseQueue  = nullptr;
  m_eErrorCode      = EC_NONE;
  m_sErrorMsg       = aEC[m_eErrorCode];
  m_sResponse       = ""_Stz;
  m_aRowPtr         = nullptr;
  m_pEntryBuffer    = nullptr;
  m_pPlanner        = nullptr;
  m_pAccessPath     = nullptr;
  m_pEntry          = nullptr;
  m_pCurrentTable   = nullptr;
  m_pCurrentIndex   = nullptr;
  m_pTx             = nullptr;
  m_sMetadata       = ""_Stz;
  m_bDisplay        = true;
  m_iRowsProcessed    = 0;
  m_iCurrentRequestId = 0;
  m_iNextRequestId    = -1;
  m_iRowPartition     = -1;
  m_ziLastBuffers      = 0;
  m_ziLastBuffersScanned = 0;
  
  m_ziFetchTasks      = 0;
  m_bSeitfExiting     = false;

  m_ziAcksRemaining = 0;
  
  SEitf* pSEitf = this;
  gpAttachMap->m_pSEitfList->vAppend(pSEitf);
  
  for (i8 i_Partition = 0; i_Partition < C_SLOTS_PER_NODE; i_Partition++) {
    m_pRowBuffer[i_Partition] = nullptr;
    m_pGroupBy[i_Partition] = nullptr;
  }
  
  m_pRowOnce = nullptr;
  
  //m_xFormatters
  
  _checkInitialised_(CN_SEITF);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SEitf::~SEitf(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);

  SEitf* pSEitf = this;
  
  m_pResponseQueue->vFinalize();
  delete m_pResponseQueue;
  
  gpAttachMap->m_pSEitfList->vRemove(pSEitf);
  
  RASSERT(*m_ziAcksRemaining == 0);
  
  if (m_pRowOnce)
    delete m_pRowOnce;
}

/*----------------------------------------------------------------------------*/
/* get buffer queue                                                           */
/*----------------------------------------------------------------------------*/
Queue* SEitf::pBufferQueue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  return m_pBufferQueue;
}

/*----------------------------------------------------------------------------*/
/* set buffer queue                                                           */
/*----------------------------------------------------------------------------*/
void SEitf::vBufferQueue(Queue* pBufferQueue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  m_pBufferQueue = pBufferQueue;
}

/*----------------------------------------------------------------------------*/
/* get path stack                                                             */
/*----------------------------------------------------------------------------*/
DicPath* SEitf::pPathStack(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);

  return &m_xPathStack;
}

/*----------------------------------------------------------------------------*/
/* set the current index                                                      */
/*----------------------------------------------------------------------------*/
void SEitf::vCurrentIndex(Index* pIndex)
{
  m_pCurrentIndex = pIndex;
}

/*----------------------------------------------------------------------------*/
/* get the current index                                                      */
/*----------------------------------------------------------------------------*/
Index* SEitf::pCurrentIndex(void)
{
  return m_pCurrentIndex;
}

/*----------------------------------------------------------------------------*/
/* set the current table                                                      */
/*----------------------------------------------------------------------------*/
void SEitf::vCurrentTable(Table* pTable)
{
  m_pCurrentTable = pTable;
}

/*----------------------------------------------------------------------------*/
/* get the current table                                                      */
/*----------------------------------------------------------------------------*/
Table* SEitf::pCurrentTable(void)
{
  return m_pCurrentTable;
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void SEitf::vContentDump(void)
{
  vTrace("SEitf %p: pid %lld (%s)\n"_Stz,
         this,
         m_iPid,
         Process::bTestPid(m_iPid) ? "exists"_Stz : "non-existant"_Stz
         );
  
  vTrace("  RequestQueue %p  ResponseQueue %p\n"_Stz,
         m_pRequestQueue,
         m_pResponseQueue);
}

/*----------------------------------------------------------------------------*/
/* create row once object                                                     */
/*----------------------------------------------------------------------------*/
void SEitf::vCreateRowOnce(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  ASSERT(! m_pRowOnce);
  
  m_pRowOnce = new RowOnce;
}

/*----------------------------------------------------------------------------*/
/* dec acks remaining                                                         */
/*----------------------------------------------------------------------------*/
void SEitf::vDecAcksRemaining(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  --m_ziAcksRemaining;
  //vTrace("acksRemaining %lld\n"_Stz, *m_ziAcksRemaining);
}

/*----------------------------------------------------------------------------*/
/* dump seitf                                                                 */
/*----------------------------------------------------------------------------*/
void SEitf::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  ASSERT(eClass == CN_SEITF);
  ASSERT(pObject->eClass() == CN_SEITF);
  
  SEitf* pSEitf = reinterpret_cast<SEitf*>(pObject);
  
  vTrace("pid %lld %s partitions %lld partition %lld slot %lld"_Stz
         " requestQ %p responseQ %p response '%s'\n"_Stz,
         pSEitf->m_iPid,
         Process::bTestPid(pSEitf->m_iPid) ? "exists"_Stz : "non-existant"_Stz,
         pSEitf->m_iSlots,
         pSEitf->m_iPartition,
         pSEitf->iSlot(),
         pSEitf->m_pRequestQueue,
         pSEitf->m_pResponseQueue,
         pSEitf->m_sResponse.aChar());
}

/*----------------------------------------------------------------------------*/
/* end fetch                                                                  */
/*----------------------------------------------------------------------------*/
void SEitf::vEndFetch(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  // wait till all fetch tasks have ended
  while (*m_ziFetchTasks != 0)
    ;
  
  // reset the index
  m_pCurrentIndex = nullptr;
  
  // delete the planner
  delete m_pPlanner;
  m_pPlanner = nullptr;
  
  delete m_pRowOnce;
  m_pRowOnce = nullptr;
  
  // now delete the buffer queue from this seitf
  m_pBufferQueue->vFinalize();
  delete m_pBufferQueue;
  m_pBufferQueue = nullptr;
  
}

/*----------------------------------------------------------------------------*/
/* get acks remaining                                                         */
/*----------------------------------------------------------------------------*/
i8 SEitf::iGetAcksRemaining(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  i8 iAcksRemaining = *m_ziAcksRemaining;
  
  return iAcksRemaining;
}

/*----------------------------------------------------------------------------*/
/* initialise this seitf                                                      */
/*----------------------------------------------------------------------------*/
void SEitf::vInit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  // request queue
  Word* pReqQueWord = gpAttachMap->m_xMainDic.pFindWord("requestQueue"_Stz);
  ASSERT(pReqQueWord);
  m_pRequestQueue = reinterpret_cast<Queue*>(pReqQueWord->aBodyArea());
  
  // response queue
  m_pResponseQueue = new Queue;
  
  // set up initial default path
  
  // /tenants
  //Word* pTenants = gpAttachMap->m_xMainDic.pFindWord("tenants"_Stz);
  Word* pTenants = gpAttachMap->m_pTenants;
  ASSERT(pTenants);
  Cell* pCell = pTenants->pFirstCell();
  m_xPathStack.vPush(reinterpret_cast<Dic*>(pCell));
  
  // /tenants/boray
  Word* pTenant = m_xPathStack.pFindInCurrent("boray"_Stz);
  ASSERT(pTenant);
  pCell = pTenant->pFirstCell();
  m_xPathStack.vPush(reinterpret_cast<Dic*>(pCell));
  
  // /tenants/boray/catalogs
  Word* pCatalogs = m_xPathStack.pFindInCurrent("catalogs"_Stz);
  ASSERT(pCatalogs);
  pCell = pCatalogs->pFirstCell();
  m_xPathStack.vPush(reinterpret_cast<Dic*>(pCell));
 
  // /tenants/boray/catalogs/rapidsse
  Word* pCatalog = m_xPathStack.pFindInCurrent("rapidsse"_Stz);
  ASSERT(pCatalog);
  pCell = pCatalog->pFirstCell();
  m_xPathStack.vPush(reinterpret_cast<Dic*>(pCell));
  
  // /tenants/boray/catalogs/rapidsse/schemas
  Word* pSchemas = m_xPathStack.pFindInCurrent("schemas"_Stz);
  ASSERT(pSchemas);
  pCell = pSchemas->pFirstCell();
  m_xPathStack.vPush(reinterpret_cast<Dic*>(pCell));
  
  // /tenants/boray/catalogs/rapidsse/schemas/public
  Word* pSchema = m_xPathStack.pFindInCurrent("public"_Stz);
  ASSERT(pSchema);
  pCell = pSchema->pFirstCell();
  m_xPathStack.vPush(reinterpret_cast<Dic*>(pCell));

  // /tenants/boray/catalogs/rapidsse/schemas/public/tables
  Word* pTables = m_xPathStack.pFindInCurrent("tables"_Stz);
  ASSERT(pTables);
  
  pCell = pTables->pFirstCell();
  m_xPathStack.vPush(reinterpret_cast<Dic*>(pCell));
}

/*----------------------------------------------------------------------------*/
/* insert row into row buffer                                                 */
/*----------------------------------------------------------------------------*/
bool SEitf::bInsertRow(Stz* aRow, i8 iLength)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  // convert the seitf partition to node slot
  i8 iSlot = Partition::iPartitionToSlot(m_iPartition);
  
  //vTrace("Inserting into slot %lld partition %lld\n"_Stz, iSlot, m_iPartition);
  
  // check for working buffer
  if (! m_pRowBuffer[iSlot]) {
    m_pRowBuffer[iSlot] = new EntryBuffer(m_pCurrentTable->pHiddenIndex(),
                                          0, m_iPartition);
    if (! m_pRowBuffer[iSlot]) {
      vTrace("EntryBuffer allocation failed\n"_Stz);
      m_eErrorCode = EC_LOW_BUFFERS;
      return false;
    }
    m_pRowBuffer[iSlot]->vDurability(BD_TEMPORARY);
  }
  
  // get working buffer for partition row hashed into
  EntryBuffer* pRowBuffer = m_pRowBuffer[iSlot];
  
  // get tx id
  i8 iDistTxId = m_pTx->iDistTxId();
  
  // append row into working buffer
  if (pRowBuffer->bAppendRow(iDistTxId,aRow,iLength))
    return true;
  
  // buffer is full, copy buffer into table
  bool bResult = bInsertBuffer();
  
  // copy successful ?
  if (!bResult)
    return bResult;
  
  // get response
  eEC eError = eGetResponse();
  if (EC_NONE != eError) {
    vSetLastError(eError);
    return false;
  }
  
  // get a new working buffer
  m_pRowBuffer[iSlot] = new EntryBuffer(m_pCurrentTable->pHiddenIndex(),
                                        0, m_iPartition);
  if (! m_pRowBuffer[iSlot]) {
    vTrace("EntryBuffer allocation failed\n"_Stz);
    m_eErrorCode = EC_LOW_BUFFERS;
    return false;
  }
  m_pRowBuffer[iSlot]->vDurability(BD_TEMPORARY);
  
  pRowBuffer = m_pRowBuffer[iSlot];
  
  // append row into working buffer
  if (pRowBuffer->bAppendRow(iDistTxId,aRow,iLength))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* insert row into row buffer of given partition                              */
/*----------------------------------------------------------------------------*/
bool SEitf::bInsertRow(Stz* aRow, i8 iLength, i8 iPartition)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  
  //vTrace("Inserting into partition %lld\n"_Stz, iPartition);
  
  // check for working buffer
  if (! m_pRowBuffer[iPartition]) {
    m_pRowBuffer[iPartition] = new EntryBuffer(m_pCurrentTable->pHiddenIndex(),
                                               0, iPartition);
    if (! m_pRowBuffer[iPartition]) {
      vTrace("EntryBuffer allocation failed\n"_Stz);
      m_eErrorCode = EC_LOW_BUFFERS;
      return false;
    }
    m_pRowBuffer[iPartition]->vDurability(BD_TEMPORARY);
  }
  
  // get working buffer for partition row hashed into
  EntryBuffer* pRowBuffer = m_pRowBuffer[iPartition];
  
  // get tx id
  i8 iDistTxId = m_pTx->iDistTxId();
  
  // append row into working buffer
  if (pRowBuffer->bAppendRow(iDistTxId,aRow,iLength))
    return true;
  
  // buffer is full, copy buffer into table
  //vTrace("Buffer full\n"_Stz);
  bool bResult = bInsertBuffer(iPartition);
  
  // copy successful ?
  if (!bResult)
    return bResult;
  
  // get response
  eEC eError = eGetResponse();
  if (EC_NONE != eError) {
    vSetLastError(eError);
    return false;
  }
  
  //vTrace("creating new buffer\n"_Stz);
  // get a new working buffer
  m_pRowBuffer[iPartition] = new EntryBuffer(m_pCurrentTable->pHiddenIndex(),
                                             0, iPartition);
  if (! m_pRowBuffer[iPartition]) {
    vTrace("EntryBuffer allocation failed\n"_Stz);
    m_eErrorCode = EC_LOW_BUFFERS;
    return false;
  }
  m_pRowBuffer[iPartition]->vDurability(BD_TEMPORARY);
  
  pRowBuffer = m_pRowBuffer[iPartition];
  
  // append row into working buffer
  if (pRowBuffer->bAppendRow(iDistTxId,aRow,iLength))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* insert rowbuffer into current table                                        */
/*----------------------------------------------------------------------------*/
bool SEitf::bInsertBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  //vTrace("inserting entries from row buffer into table...\n"_Stz);
  
  i8 iSlot = Partition::iPartitionToSlot(m_iPartition);
  
  if (! m_pRowBuffer[iSlot]) {
    return false;
  }
  
  //m_pRowBuffer[iSlot]->vDump();
  
  // now insert the entries from this temporary buffer into the table
  bool bResult = bInsertEntriesTableSlot(m_pRowBuffer[iSlot],iSlot);
  
  // null the buffer
  m_pRowBuffer[iSlot] = nullptr;
  
  return bResult;
}

/*----------------------------------------------------------------------------*/
/* insert rowbuffer into current table for partition                          */
/*----------------------------------------------------------------------------*/
bool SEitf::bInsertBuffer(i8 iPartition)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  //vTrace("inserting entries from row buffer into table..%lld.\n"_Stz, iPartition);
  
  if (! m_pRowBuffer[iPartition]) {
    return false;
  }
  ASSERT(m_pRowBuffer[iPartition]->eDurability() == BD_TEMPORARY);
  
  //vTrace("inserting ..%lld.\n"_Stz, iPartition);
  
  //m_pRowBuffer[iSlot]->vDump();
  
  // now insert the entries from this temporary buffer into the table
  m_pEntryBuffer = m_pRowBuffer[iPartition];
  bool bResult = bInsertEntriesTable(m_pRowBuffer[iPartition],iPartition);
  
  // wait for response
  //eEC eResult = eGetResponse();
  //ASSERT(eResult == EC_NONE);
  
  // null the buffer
  m_pRowBuffer[iPartition] = nullptr;
  
  return bResult;
}

/*----------------------------------------------------------------------------*/
/* insert last rowbuffer into current table                                   */
/*----------------------------------------------------------------------------*/
bool SEitf::bInsertLastBuffer(i8 iPartition)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  //vTrace("%p inserting entries from row buffer into table..%lld.\n"_Stz, this, iPartition);
  
  if (! m_pRowBuffer[iPartition]) {
    m_pRowBuffer[iPartition] = new EntryBuffer(m_pCurrentTable->pHiddenIndex(),
                                               0, m_iPartition);
    m_pRowBuffer[iPartition]->vDurability(BD_TEMPORARY);
  }
  ASSERT(m_pRowBuffer[iPartition]->eDurability() == BD_TEMPORARY);
  
  //vTrace("inserting %p ..%lld.\n"_Stz, m_pRowBuffer[iPartition], iPartition);
  
  //m_pRowBuffer[iSlot]->vDump();
  //vTrace("No of entries %lld %lld\n"_Stz, iPartition, m_pRowBuffer[iPartition]->nEntries());
  
  // mark this buffer as the last buffer
  m_pRowBuffer[iPartition]->vIsLast(true);
  
  // now insert the entries from this temporary buffer into the table
  bool bResult = bInsertEntriesTable(m_pRowBuffer[iPartition],iPartition);
  
  // wait for response
  //eEC eResult = eGetResponse();
  //ASSERT(eResult == EC_NONE);
  
  // null the buffer
  m_pRowBuffer[iPartition] = nullptr;
  
  //vTrace("bInsertLastBuffer\n"_Stz);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get number of last buffers                                                 */
/*----------------------------------------------------------------------------*/
i8 SEitf::nLastBuffers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  return m_ziLastBuffers;
}

/*----------------------------------------------------------------------------*/
/* set number of last buffers                                                 */
/*----------------------------------------------------------------------------*/
void SEitf::vLastBuffers(i8 nLastBuffers)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  m_ziLastBuffers = nLastBuffers;
  
  //vTrace("No of fetchTasks set to %lld\n"_Stz, nLastBuffers);
  m_ziFetchTasks = m_ziLastBuffers;
}

/*----------------------------------------------------------------------------*/
/* iterate entry buffers from the buffer queue (rows in response buffer)      */
/*----------------------------------------------------------------------------*/
bool SEitf::bNextRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  // get index buffer from seitf
  Entry* pEntry = nullptr;
  i8 iRowCount = 0;
  
  while (true) {
    
    // if no entry buffer , pop one off the queue
    if (!m_pEntryBuffer) {
      
      // pop buffer off the buffer queue
      ValWrk xValWrk;
      while (!m_pBufferQueue->bPop(&xValWrk)) {
        PosixThread::vSleepMilli(1);
      }
      
      // get entry buffer
      PtrVal* pPtrVal = xValWrk.pNestedPtrVal();
      m_pEntryBuffer = (EntryBuffer*)(pPtrVal->pPtr());
      
      // get first entry from buffer
      pEntry = m_pEntryBuffer->pFirstEntry();
      
      if (m_pEntryBuffer->bIsLast()) {
        m_ziLastBuffersScanned++;
        //vTrace("m_ziLastBuffersScanned: %lld\n"_Stz, pSEitf->m_ziLastBuffersScanned );
      }
      
    }
    else {
      
      // seitf should have an entry
      ASSERT(m_pEntry);
      
      // get next entry from buffer
      pEntry = m_pEntryBuffer->pNextEntry(m_pEntry);
    }
    
    // if no entry, get next buffer
    if (!pEntry) {
      
      // if we had an entry buffer delete it
      if (m_pEntryBuffer)
        delete m_pEntryBuffer;
      
      // null the buffer and get the next
      m_pEntryBuffer = nullptr;
    }
    
    // if found an entry break or last buffer break out of loop
    if (pEntry || (!pEntry && m_ziLastBuffersScanned == m_ziLastBuffers))
      break;
  }
  
  if (!pEntry) {
    m_ziLastBuffersScanned = 0;
    return false;
  }
  else {
    // save entry in SEitf
    m_pEntry = pEntry;
    return true;
  }
  
}

/*----------------------------------------------------------------------------*/
/* which partition                                                            */
/*----------------------------------------------------------------------------*/
i8 SEitf::iPartition(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  return m_iPartition;
}

/*----------------------------------------------------------------------------*/
/* get request queue                                                          */
/*----------------------------------------------------------------------------*/
Queue* SEitf::pRequestQueue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  return m_pRequestQueue;
}

/*----------------------------------------------------------------------------*/
/* set request queue                                                          */
/*----------------------------------------------------------------------------*/
void SEitf::vRequestQueue(Queue* pRequestQueue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  m_pRequestQueue = pRequestQueue;
}

/*----------------------------------------------------------------------------*/
/* get response queue                                                         */
/*----------------------------------------------------------------------------*/
Queue* SEitf::pResponseQueue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  return m_pResponseQueue;
}

/*----------------------------------------------------------------------------*/
/* set response queue                                                         */
/*----------------------------------------------------------------------------*/
void SEitf::vResponseQueue(Queue* pResponseQueue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  m_pResponseQueue = pResponseQueue;
}

/*----------------------------------------------------------------------------*/
/* which slot                                                                 */
/*----------------------------------------------------------------------------*/
i8 SEitf::iSlot(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  return Partition::iPartitionToSlot(m_iPartition);
}

/*----------------------------------------------------------------------------*/
/* set acks remaining                                                         */
/*----------------------------------------------------------------------------*/
void SEitf::vSetAcksRemaining(i8 iAcksRemaining)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  RASSERT(*m_ziAcksRemaining == 0);
  m_ziAcksRemaining = iAcksRemaining;
}

/*----------------------------------------------------------------------------*/
/* test if row has been seen so far                                           */
/*----------------------------------------------------------------------------*/
bool SEitf::bRowSeen(Id xRowId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  if (! m_pRowOnce)
    return false;
  
  bool bSeen = m_pRowOnce->bRowSeen2(xRowId);
  
  return bSeen;
}

/*----------------------------------------------------------------------------*/
/* get tx                                                                     */
/*----------------------------------------------------------------------------*/
Tx* SEitf::pTx(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  return m_pTx;
}

/*----------------------------------------------------------------------------*/
/* set tx                                                                     */
/*----------------------------------------------------------------------------*/
void SEitf::vTx(Tx* pTx)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  m_pTx = pTx;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* begin a tx                                                                 */
/*----------------------------------------------------------------------------*/
bool SEitf::bBeginTx()
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  m_pTx = new Tx;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* commit a tx                                                                */
/*----------------------------------------------------------------------------*/
void SEitf::vCommitTx()
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  m_pTx->vPhase(TP_COMMITTED);
}

/*----------------------------------------------------------------------------*/
/* end a tx                                                                   */
/*----------------------------------------------------------------------------*/
void SEitf::vEndTx()
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  delete m_pTx;
  
  m_pTx = nullptr;
}

/*----------------------------------------------------------------------------*/
/* decode request                                                             */
/*----------------------------------------------------------------------------*/
bool SEitf::bDecode(TokenVec& xTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  if (0 == xTokenVec[0]->iLength())
    return false;
  
  for (i8 i_ReqNo = 0; SeRequests[i_ReqNo].aCommand; i_ReqNo++) {
    if (xTokenVec[0]->bSame((Stz*)SeRequests[i_ReqNo].aCommand)) {
      //vTrace("request: %s\n"_Stz,SeRequests[i_ReqNo].aRequest);
      return (this->*SeRequests[i_ReqNo].aFunc)(xTokenVec,i_ReqNo);
    }
  }
  
  vSetLastError(EC_INVALID_COMMAND);
  return false;
}

/*----------------------------------------------------------------------------*/
/* connect to se                                                              */
/*----------------------------------------------------------------------------*/
bool SEitf::bConnect(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(xTokenVec[0]->bSame("con"_Stz));
  
  sRequest = SeRequests[iReqNo].aRequest;

  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* take a checkpoint                                                          */
/*----------------------------------------------------------------------------*/
bool SEitf::bCheckpoint(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Stz);
  sRequest.vConcat(xTokenVec[1]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* connect the se cluster nodes                                               */
/*----------------------------------------------------------------------------*/
bool SEitf::bConnectCluster(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(xTokenVec[0]->bSame("connectCluster"_Stz));
  
  sRequest = SeRequests[iReqNo].aRequest;
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* disconnect from se                                                         */
/*----------------------------------------------------------------------------*/
bool SEitf::bDisconnect(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(xTokenVec[0]->bSame("dic"_Stz));
  
  sRequest = SeRequests[iReqNo].aRequest;
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* get value from attachmap                                                   */
/*----------------------------------------------------------------------------*/
bool SEitf::bGetAttachMap(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Stz);
  sRequest.vConcat(xTokenVec[1]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);

  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* set value in attachmap                                                     */
/*----------------------------------------------------------------------------*/
bool SEitf::bSetAttachMap(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 3) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Stz);
  sRequest.vConcat(xTokenVec[1]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat("\""_Stz);
  sRequest.vConcat(xTokenVec[2]->aText());
  sRequest.vConcat("\""_Stz);

  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* reload                                                                     */
/*----------------------------------------------------------------------------*/
bool SEitf::bReload(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Stz);
  sRequest.vConcat(xTokenVec[1]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* remove a folder                                                            */
/*----------------------------------------------------------------------------*/
bool SEitf::bRemoveFolder(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  sRequest.vCopy("\""_Stz);
  sRequest.vConcat(xTokenVec[1]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* create a tenant                                                            */
/*----------------------------------------------------------------------------*/
bool SEitf::bCreateTenant(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }

  sRequest.vCopy("\""_Stz);
  sRequest.vConcat(xTokenVec[1]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* list tenants                                                               */
/*----------------------------------------------------------------------------*/
bool SEitf::bListTenants(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 1) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }

  ASSERT(xTokenVec[0]->bSame("lsten"_Stz));
  
  sRequest = SeRequests[iReqNo].aRequest;
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* drop a tenant                                                              */
/*----------------------------------------------------------------------------*/
bool SEitf::bDropTenant(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Stz);
  sRequest.vConcat(xTokenVec[1]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* create a catalog                                                           */
/*----------------------------------------------------------------------------*/
bool SEitf::bCreateCatalog(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Stz);
  sRequest.vConcat(xTokenVec[1]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* list catalogs                                                              */
/*----------------------------------------------------------------------------*/
bool SEitf::bListCatalogs(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
    
  if (xTokenVec.nTokens() != 1) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  ASSERT(xTokenVec[0]->bSame("lscat"_Stz));
  
  sRequest = SeRequests[iReqNo].aRequest;
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* drop a catalog                                                             */
/*----------------------------------------------------------------------------*/
bool SEitf::bDropCatalog(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }

  sRequest.vCopy("\""_Stz);
  sRequest.vConcat(xTokenVec[1]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* create a schema                                                            */
/*----------------------------------------------------------------------------*/
bool SEitf::bCreateSchema(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Stz);
  sRequest.vConcat(xTokenVec[1]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* list schemas                                                               */
/*----------------------------------------------------------------------------*/
bool SEitf::bListSchemas(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 1) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  ASSERT(xTokenVec[0]->bSame("lssch"_Stz));

  sRequest = SeRequests[iReqNo].aRequest;
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* drop a schema                                                              */
/*----------------------------------------------------------------------------*/
bool SEitf::bDropSchema(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }

  sRequest.vCopy("\""_Stz);
  sRequest.vConcat(xTokenVec[1]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* use a schema                                                               */
/*----------------------------------------------------------------------------*/
bool SEitf::bUseSchema(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Stz);
  sRequest.vConcat(xTokenVec[1]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* create an index                                                            */
/*----------------------------------------------------------------------------*/
bool SEitf::bCreateIndex(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 3) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vConcat(" \""_Stz);
  sRequest.vConcat(xTokenVec[2]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" \""_Stz);
  sRequest.vConcat(xTokenVec[1]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* list indices                                                               */
/*----------------------------------------------------------------------------*/
bool SEitf::bListIndices(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Stz);
  sRequest.vConcat(xTokenVec[1]->aText());
  sRequest.vConcat("\""_Stz);

  ASSERT(xTokenVec[0]->bSame("lsidx"_Stz));
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* set index uniqueness                                                       */
/*----------------------------------------------------------------------------*/
bool SEitf::bIndexSetUnique(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 4) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  //sRequest.vCopy("\""_Stz);
  sRequest.vCopy(xTokenVec[3]->aText());
  //sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat("\""_Stz);
  sRequest.vConcat(xTokenVec[2]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat("\""_Stz);
  sRequest.vConcat(xTokenVec[1]->aText());
  sRequest.vConcat("\""_Stz);
  
  ASSERT(xTokenVec[0]->bSame("setidxunique"_Stz));
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* create a table                                                             */
/*----------------------------------------------------------------------------*/
bool SEitf::bCreateTable(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Stz);
  sRequest.vConcat(xTokenVec[1]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* list tables                                                                */
/*----------------------------------------------------------------------------*/
bool SEitf::bListTables(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 1) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  ASSERT(xTokenVec[0]->bSame("lstbl"_Stz));
  
  sRequest = SeRequests[iReqNo].aRequest;
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* dump an index                                                              */
/*----------------------------------------------------------------------------*/
bool SEitf::bDumpIndex(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 3) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Stz);
  sRequest.vConcat(xTokenVec[2]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat("\""_Stz);
  sRequest.vConcat(xTokenVec[1]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* dump a table                                                               */
/*----------------------------------------------------------------------------*/
bool SEitf::bDumpTable(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Stz);
  sRequest.vConcat(xTokenVec[1]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* docol                                                                      */
/*----------------------------------------------------------------------------*/
bool SEitf::bDocol(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if ( ! (xTokenVec.nTokens() >= 2) ) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Stz);
  
  for (i8 i_Word = 1; i_Word <= xTokenVec.nTokens(); i_Word++) {
    sRequest.vConcat(xTokenVec[i_Word]->aText());
    sRequest.vConcat(" "_Stz);
  }
  
  sRequest.vConcat("\""_Stz);
  
  //
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* display off selected rows                                                  */
/*----------------------------------------------------------------------------*/
bool SEitf::bDisplayOff(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 1) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  //
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* display selected rows                                                      */
/*----------------------------------------------------------------------------*/
bool SEitf::bDisplayOn(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 1) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  //
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* drop an index                                                              */
/*----------------------------------------------------------------------------*/
bool SEitf::bDropIndex(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 3) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Stz);
  sRequest.vConcat(xTokenVec[2]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat("\""_Stz);
  sRequest.vConcat(xTokenVec[1]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}


/*----------------------------------------------------------------------------*/
/* drop a table                                                               */
/*----------------------------------------------------------------------------*/
bool SEitf::bDropTable(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Stz);
  sRequest.vConcat(xTokenVec[1]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* add a column                                                               */
/*----------------------------------------------------------------------------*/
bool SEitf::bAddColumn(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 4) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Stz);
  sRequest.vConcat(xTokenVec[3]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" \""_Stz);
  sRequest.vConcat(xTokenVec[2]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" \""_Stz);
  sRequest.vConcat(xTokenVec[1]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* add an index column                                                        */
/*----------------------------------------------------------------------------*/
bool SEitf::bAddIndexColumn(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 4) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy(" \""_Stz);
  sRequest.vConcat(xTokenVec[3]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" \""_Stz);
  sRequest.vConcat(xTokenVec[2]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" \""_Stz);
  sRequest.vConcat(xTokenVec[1]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* add a partition column                                                     */
/*----------------------------------------------------------------------------*/
bool SEitf::bAddPartitionColumn(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 3) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vConcat(" \""_Stz);
  sRequest.vConcat(xTokenVec[2]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" \""_Stz);
  sRequest.vConcat(xTokenVec[1]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* end of create index sequence                                               */
/*----------------------------------------------------------------------------*/
bool SEitf::bEndCreateIndex(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 3) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Stz);
  sRequest.vConcat(xTokenVec[2]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat("\""_Stz);
  sRequest.vConcat(xTokenVec[1]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* end of create table sequence                                               */
/*----------------------------------------------------------------------------*/
bool SEitf::bEndCreateTable(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Stz);
  sRequest.vConcat(xTokenVec[1]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* list columns                                                               */
/*----------------------------------------------------------------------------*/
bool SEitf::bListColumns(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Stz);
  sRequest.vConcat(xTokenVec[1]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* list index columns                                                         */
/*----------------------------------------------------------------------------*/
bool SEitf::bListIndexColumns(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 3) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Stz);
  sRequest.vConcat(xTokenVec[1]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat("\""_Stz);
  sRequest.vConcat(xTokenVec[2]->aText());
  sRequest.vConcat("\""_Stz);

  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* list partition columns                                                     */
/*----------------------------------------------------------------------------*/
bool SEitf::bListPartitionColumns(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Stz);
  sRequest.vConcat(xTokenVec[1]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* analyze an index                                                           */
/*----------------------------------------------------------------------------*/
bool SEitf::bAnalyzeIndex(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 3) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Stz);
  sRequest.vConcat(xTokenVec[2]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat("\""_Stz);
  sRequest.vConcat(xTokenVec[1]->aText());
  sRequest.vConcat("\""_Stz);

  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* analyze a table                                                            */
/*----------------------------------------------------------------------------*/
bool SEitf::bAnalyzeTable(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Stz);
  sRequest.vConcat(xTokenVec[1]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* detailed analysis of an index                                              */
/*----------------------------------------------------------------------------*/
bool SEitf::bDanalyzeIndex(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 3) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Stz);
  sRequest.vConcat(xTokenVec[2]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat("\""_Stz);
  sRequest.vConcat(xTokenVec[1]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* detailed analysis of a table                                               */
/*----------------------------------------------------------------------------*/
bool SEitf::bDanalyzeTable(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Stz);
  sRequest.vConcat(xTokenVec[1]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* copy a table                                                               */
/*----------------------------------------------------------------------------*/
bool SEitf::bCopyTable(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 3) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Stz);
  sRequest.vConcat(xTokenVec[2]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" \""_Stz);
  sRequest.vConcat(xTokenVec[1]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* distributed export of a table                                              */
/*----------------------------------------------------------------------------*/
bool SEitf::bDistExportTable(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 3) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Stz);
  sRequest.vConcat(xTokenVec[2]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" \""_Stz);
  sRequest.vConcat(xTokenVec[1]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* distributed fetch of an index                                              */
/*----------------------------------------------------------------------------*/
bool SEitf::bDistFetchRowsIndex(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 2 && xTokenVec.nTokens() != 3 ) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  if (xTokenVec.nTokens() == 3) {
    sRequest.vCopy("\""_Stz);
    sRequest.vConcat(xTokenVec[2]->aText());
    sRequest.vConcat("\""_Stz);
  }
  
  sRequest.vConcat(" \""_Stz);
  sRequest.vConcat(xTokenVec[1]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* export a table                                                             */
/*----------------------------------------------------------------------------*/
bool SEitf::bExportTable(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;

  if (xTokenVec.nTokens() != 3) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Stz);
  sRequest.vConcat(xTokenVec[2]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" \""_Stz);
  sRequest.vConcat(xTokenVec[1]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* fetch rows from a table                                                    */
/*----------------------------------------------------------------------------*/
bool SEitf::bFetchRows(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
//  sRequest.vCopy("\""_Stz);
//  sRequest.vConcat(xTokenVec[3]);
//  sRequest.vConcat("\""_Stz);
  
  // hack for groupBy performance testing
  if (xTokenVec.nTokens() == 4)
    sRequest.vConcat("1"_Stz);
  else
    sRequest.vConcat("0"_Stz);
  
  sRequest.vConcat(" \""_Stz);
  sRequest.vConcat(xTokenVec[2]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" \""_Stz);
  sRequest.vConcat(xTokenVec[1]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* load a table from file                                                     */
/*----------------------------------------------------------------------------*/
bool SEitf::bLoadTable(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 3) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vConcat(" \""_Stz);
  sRequest.vConcat(xTokenVec[2]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" \""_Stz);
  sRequest.vConcat(xTokenVec[1]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* load a table from stream                                                   */
/*----------------------------------------------------------------------------*/
bool SEitf::bLoadTableStream(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 3) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Stz);
  sRequest.vConcat(xTokenVec[2]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" \""_Stz);
  sRequest.vConcat(xTokenVec[1]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* get address of an index                                                    */
/*----------------------------------------------------------------------------*/
bool SEitf::bGetIndex(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 3) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Stz);
  sRequest.vConcat(xTokenVec[2]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat("\""_Stz);
  sRequest.vConcat(xTokenVec[1]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* get address/reference of a table                                           */
/*----------------------------------------------------------------------------*/
bool SEitf::bGetTable(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Stz);
  sRequest.vConcat(xTokenVec[1]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* get address/reference of a table                                           */
/*----------------------------------------------------------------------------*/
bool SEitf::bGetTableNew(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Stz);
  sRequest.vConcat(xTokenVec[1]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* return whether table is in use or not                                      */
/*----------------------------------------------------------------------------*/
bool SEitf::bInUseTable(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Stz);
  sRequest.vConcat(xTokenVec[1]->aText());
  sRequest.vConcat("\""_Stz);

  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* return address of an index                                                 */
/*----------------------------------------------------------------------------*/
bool SEitf::bRetIndex(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 1) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* return address/refernce of a table                                         */
/*----------------------------------------------------------------------------*/
bool SEitf::bRetTable(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 1) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* get pool space                                                             */
/*----------------------------------------------------------------------------*/
bool SEitf::bPoolSpace(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 1) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* check node sanity                                                          */
/*----------------------------------------------------------------------------*/
bool SEitf::bNodeSanity(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 1) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* get node status                                                            */
/*----------------------------------------------------------------------------*/
bool SEitf::bNodeStatus(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 1) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* size of index                                                              */
/*----------------------------------------------------------------------------*/
bool SEitf::bSizeIndex(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 3) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Stz);
  sRequest.vConcat(xTokenVec[2]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat("\""_Stz);
  sRequest.vConcat(xTokenVec[1]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* size of table                                                              */
/*----------------------------------------------------------------------------*/
bool SEitf::bSizeTable(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Stz);
  sRequest.vConcat(xTokenVec[1]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* size of table                                                              */
/*----------------------------------------------------------------------------*/
bool SEitf::bSizeTables(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 1) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* truncate an index                                                          */
/*----------------------------------------------------------------------------*/
bool SEitf::bTruncateIndex(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 3) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Stz);
  sRequest.vConcat(xTokenVec[2]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat("\""_Stz);
  sRequest.vConcat(xTokenVec[1]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* truncate a table                                                           */
/*----------------------------------------------------------------------------*/
bool SEitf::bTruncateTable(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }

  sRequest.vCopy("\""_Stz);
  sRequest.vConcat(xTokenVec[1]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* begin transaction                                                          */
/*----------------------------------------------------------------------------*/
bool SEitf::bTxBegin(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 1) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* commit transaction                                                         */
/*----------------------------------------------------------------------------*/
bool SEitf::bTxCommit(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 1) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* dump all transactions                                                      */
/*----------------------------------------------------------------------------*/
bool SEitf::bTxDumpAll(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 1) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* end transaction                                                            */
/*----------------------------------------------------------------------------*/
bool SEitf::bTxEnd(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 1) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* rollback transaction                                                       */
/*----------------------------------------------------------------------------*/
bool SEitf::bTxRollback(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 1) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* visualize an index                                                         */
/*----------------------------------------------------------------------------*/
bool SEitf::bVisualizeIndex(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 3) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Stz);
  sRequest.vConcat(xTokenVec[2]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat("\""_Stz);
  sRequest.vConcat(xTokenVec[1]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* append rowbuffer entries into current table                                */
/*----------------------------------------------------------------------------*/
bool SEitf::bInsertEntriesTableSlot(EntryBuffer* pRowBuffer, i8 iSlot)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);

  s256 sRequest;
  s256 sBuffer;

  sBuffer = iSlot;
  sRequest.vCopy("\""_Stz);
  sRequest.vConcat(sBuffer);
  sRequest.vConcat("\""_Stz);
  
  sBuffer = (i8)pRowBuffer;
  sRequest.vConcat(" \""_Stz);
  sRequest.vConcat(sBuffer);
  sRequest.vConcat("\""_Stz);
  
  sBuffer = (i8)m_pCurrentTable;
  sRequest.vConcat(" \""_Stz);
  sRequest.vConcat(sBuffer);
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" insertTableEntriesSlot_req"_Stz);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* append rowbuffer entries into current table                                */
/*----------------------------------------------------------------------------*/
bool SEitf::bInsertEntriesTable(EntryBuffer* pRowBuffer, i8 iPartition)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  s256 sBuffer;
  
  sBuffer = iPartition;
  sRequest.vCopy("\""_Stz);
  sRequest.vConcat(sBuffer);
  sRequest.vConcat("\""_Stz);
  
  sBuffer = (i8)pRowBuffer;
  sRequest.vConcat(" \""_Stz);
  sRequest.vConcat(sBuffer);
  sRequest.vConcat("\""_Stz);
  
  sBuffer = m_pCurrentTable->aTableName();
  sRequest.vConcat(" \""_Stz);
  sRequest.vConcat(sBuffer);
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" insertTableEntries_req"_Stz);
  
  //vTrace("%s\n"_Stz, sRequest.aChar());
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* change folder                                                              */
/*----------------------------------------------------------------------------*/
bool SEitf::bChangeFolder(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  sRequest.vCopy("\""_Stz);
  sRequest.vConcat(xTokenVec[1]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* print                                                                      */
/*----------------------------------------------------------------------------*/
bool SEitf::bPrint(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  vInfo("%s\n"_Stz,xTokenVec[1]->aText());
  
  //return true;
  return false;
}

/*----------------------------------------------------------------------------*/
/* print working folder                                                       */
/*----------------------------------------------------------------------------*/
bool SEitf::bPrintWorkingFolder(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(xTokenVec[0]->bSame("pwf"_Stz));
  
  sRequest = SeRequests[iReqNo].aRequest;
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* list folder contents                                                       */
/*----------------------------------------------------------------------------*/
bool SEitf::bListFolder(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(xTokenVec[0]->bSame("lf"_Stz));
  
  sRequest = SeRequests[iReqNo].aRequest;
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* get monitor string                                                         */
/*----------------------------------------------------------------------------*/
bool SEitf::bGetMonitor(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(xTokenVec[0]->bSame("mon"_Stz));
  
  sRequest = SeRequests[iReqNo].aRequest;
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* get free space                                                             */
/*----------------------------------------------------------------------------*/
bool SEitf::bGetFreeSpace(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(xTokenVec[0]->bSame("getFreeSpace"_Stz));
  
  sRequest = SeRequests[iReqNo].aRequest;

  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* get inUse space                                                            */
/*----------------------------------------------------------------------------*/
bool SEitf::bGetInUseSpace(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(xTokenVec[0]->bSame("getInUseSpace"_Stz));
  
  sRequest = SeRequests[iReqNo].aRequest;
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* get total space                                                            */
/*----------------------------------------------------------------------------*/
bool SEitf::bGetTotalSpace(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(xTokenVec[0]->bSame("getTotalSpace"_Stz));
  
  sRequest = SeRequests[iReqNo].aRequest;
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* verify an index                                                            */
/*----------------------------------------------------------------------------*/
bool SEitf::bVerifyIndex(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenVec.nTokens() != 3) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Stz);
  sRequest.vConcat(xTokenVec[2]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat("\""_Stz);
  sRequest.vConcat(xTokenVec[1]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* get version                                                                */
/*----------------------------------------------------------------------------*/
bool SEitf::bGetVersion(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(xTokenVec[0]->bSame("version"_Stz));
  
  sRequest = SeRequests[iReqNo].aRequest;
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* ping test                                                                  */
/*----------------------------------------------------------------------------*/
bool SEitf::bPing(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(xTokenVec[0]->bSame("ping"_Stz));
  
  sRequest = SeRequests[iReqNo].aRequest;
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* get seitf count                                                            */
/*----------------------------------------------------------------------------*/
bool SEitf::bSeitfCount(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(xTokenVec[0]->bSame("seitf"_Stz));
  
  sRequest = SeRequests[iReqNo].aRequest;
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* return task                                                                */
/*----------------------------------------------------------------------------*/
bool SEitf::bRetTask(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(xTokenVec[0]->bSame("rettsk"_Stz));
  
  sRequest.vCopy("\""_Stz);
  sRequest.vConcat(xTokenVec[1]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* get task                                                                   */
/*----------------------------------------------------------------------------*/
bool SEitf::bGetTask(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(xTokenVec[0]->bSame("gettsk"_Stz));
  
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* fetch rows from a table                                                    */
/*----------------------------------------------------------------------------*/
bool SEitf::bSpawnFetchRows(TokenVec& xTokenVec, i8 iReqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  //  sRequest.vCopy("\""_Stz);
  //  sRequest.vConcat(xTokenVec[3]);
  //  sRequest.vConcat("\""_Stz);
  
  // hack for groupBy performance testing
  if (xTokenVec.nTokens() == 4)
    sRequest.vConcat("1"_Stz);
  else
    sRequest.vConcat("0"_Stz);
  
  sRequest.vConcat(" \""_Stz);
  sRequest.vConcat(xTokenVec[2]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" \""_Stz);
  sRequest.vConcat(xTokenVec[1]->aText());
  sRequest.vConcat("\""_Stz);
  
  sRequest.vConcat(" "_Stz);
  sRequest.vConcat(SeRequests[iReqNo].aRequest);
  
  return bAppendRequest(sRequest);
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append request to request queue                                            */
/*----------------------------------------------------------------------------*/
bool SEitf::bAppendRequest(StrVal& sRequest)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s48K sWorkspace;
  Stz* aCursor = nullptr;
  
  ValWrk xValWrk;
  ValSet xValSet;
  
  // Before appending request for se, ensure that se is running
  // get the se pid from the attachmap
  i8 iPid = gpAttachMap->m_iPid;
  if (!Process::bTestPid(iPid)) {
    // SE is not running anymore!
    vError("Cannot find pid %lld\n"_Stz, gpAttachMap->m_iPid);
    vSetLastError(EC_SE_NOT_RUNNING);
    return false;
  }
  
  sWorkspace = (i8)this;
  sWorkspace.vConcat(" >seitf task::this task.seitf! "_Stz);
  xValWrk = sWorkspace;
  //vTrace("%s\n"_Stz,xVal.aChar());
  xValSet.vAppend(aCursor,xValWrk.pNestedValueSelf());
  
  xValWrk = sRequest;
  //vTrace("%s\n"_Stz,xVal.aChar());
  xValSet.vAppend(aCursor,xValWrk.pNestedValueSelf());
  
  // if the current request id matches the next request id, it means
  // we have not read a response of the previous request
  if (m_iCurrentRequestId == m_iNextRequestId) {
    vError("Response for previous request not retrieved. Out-Of-Sync request-response.\n"_Stz);
    vSetLastError(EC_RESPONSE_PENDING);
    return false;
  }
  
  // set the new current request id to the next one sent by previous response
  m_iCurrentRequestId = m_iNextRequestId;
  
  // spawn a request task, passing the request to it
  // create new task (will be automatically appended to task list)
  MP_Task nnpNewTask = new Task(&xValSet);
  nnpNewTask->vSeedWord("requestLoop"_Stz);
  
  gpAttachMap->m_pServerScheduler->vEnqueueTask_Mutex(*nnpNewTask);
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop response from response queue (response is from pointer in seitf(shm))  */
/*----------------------------------------------------------------------------*/
eEC SEitf::eGetResponse(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  while (! bPopResponse(m_eErrorCode, m_sResponse))
    ;
  
  vSetLastError(m_eErrorCode);
  
  // generate a random number and send it back
  do {
    m_iNextRequestId = Random::iRandom(0,99999);
  } while (m_iNextRequestId == m_iCurrentRequestId);

  //vTrace("NextRequest %lld\n"_Stz, m_iNextRequestId);
  
  return m_eErrorCode;
}

/*----------------------------------------------------------------------------*/
/* pop response from response queue                                           */
/*----------------------------------------------------------------------------*/
bool SEitf::bPopResponse(eEC& eResult, StrVal& sResponse)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  bool b;
  ValSet xValSet;
  Stz* aCursor = nullptr;
  
  if (! m_pResponseQueue->bPop(&xValSet))
    return false;
  
  Value* pValue;
  
  b = xValSet.bExtractValue(aCursor,pValue);
  ASSERT(b);
  eResult = (eEC)_pIntVal(pValue)->iInt();
  
  b = xValSet.bExtractValue(aCursor,pValue);
  ASSERT(b);
  *_pStrVal(&sResponse) = _pStrVal(pValue);
  
  b = xValSet.bExtractValue(aCursor,pValue);
  ASSERT(! b);
  
  //  vTrace("sResponse: >>>\n%s\n<<<\n\n"_Stz,sResponse.aChar());
  //  vTrace("\n---\n\n"_Stz,sResponse.aChar());
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get last error                                                             */
/*----------------------------------------------------------------------------*/
Stz* SEitf::pGetLastError(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  return m_sErrorMsg;
}

/*----------------------------------------------------------------------------*/
/* clear last error                                                           */
/*----------------------------------------------------------------------------*/
void SEitf::vClearLastError(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  m_eErrorCode = EC_NONE;
  m_sErrorMsg = ""_Stz;
}

/*----------------------------------------------------------------------------*/
/* get last error                                                             */
/*----------------------------------------------------------------------------*/
eEC SEitf::eGetLastError(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
 
  return m_eErrorCode;
}

/*----------------------------------------------------------------------------*/
/* set last error                                                             */
/*----------------------------------------------------------------------------*/
void SEitf::vSetLastError(eEC eError)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  ASSERT(eError >= EC_NONE);
  ASSERT(eError <  EC_ENUMS);
  
  m_eErrorCode = eError;
  m_sErrorMsg = aEC[m_eErrorCode];
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* SEitf::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
