/*============================================================================*/
/* BASE.h -- Base namespace                                                   */
/*============================================================================*/
#pragma once

//#pragma GCC diagnostic warning "-Wunused-parameter"
//#pragma GCC diagnostic warning "-Wunused-variable"

#include <iostream>
#include <string.h>
#include <functional> // for std::function

//typedef unsigned long size_t;

#define EndDefine

#define _static_ EndDefine
#define _nonProper_ EndDefine
#define _noPreconditions_ EndDefine
#define _nonLocalStatic_ EndDefine
#define _outsideOfPool_ EndDefine

#define TRUE(condition)  true
#define FALSE(condition) false


//------------------------------------------------------------------------------

namespace BASE { } using namespace BASE;

#define UNUSED(arg) (void)arg

#include "CompiledWith.h"

#include "BaseTypes.h"
#include "UsefulConstants.h"
#include "StringTypes.h"
#include "BytesTypes.h"

// initialised in IntLib::vInitGlobals()
extern i8 C_DATUM_NULL;
extern i8 C_DATUM_MINUS_INFINITY;
extern i8 C_DATUM_PLUS_INFINITY;

extern i8 C_I8_MIN;
extern i8 C_I8_MAX;
extern i8 C_I8_MID;

// initialised in FltLib::vInitGlobals()
extern d8 C_D8_MIN;
extern d8 C_D8_MAX;
extern d8 C_D8_MID;

//------------------------------------------------------------------------------

#include "BitLib.hpp"

#include "ByteLiterals.hpp"
#include "CommonTypes.h"
#include "EnvFlags.h"
#include "Id_2part.hpp"
#include "Null.hpp"

#include "PadWord.hpp"

#include "Asm.h"

#include "Stz.hpp"
#include "StrLib.hpp"

#include "ClassNumbers.hpp"
#include "LimitsFormats.h"

#include "FnDesc.hpp"
#include "FnDefn.hpp"
#include "FnCatalog.hpp"

#include "AM.hpp"

//------------------------------------------------------------------------------

#include "Hack.hpp"

#include "eFK.h"
#include "Exception.hpp"

#include "Footer.hpp"
#include "Header.hpp"

#include "Correct.hpp"

#include "Fault.hpp"

#include "MemLib.hpp"

#include "AssertMacros.h"

#include "Object.hpp"
#include "Verbose.hpp"
#include "StackObject.hpp"
#include "NonProper.hpp"

#include "Addressing.hpp"
#include "SizedHeader.hpp"
#include "Trace.hpp"
#include "Jot.hpp"
#include "IntLib.hpp"
#include "FltLib.hpp"

#include "String.hpp"

#include "PTR.hpp"
#include "SMA.hpp"
#include "REF_t.hpp"

#include "ClassFn.hpp"
#include "ClassCatalog.hpp"

#include "SelfFn.hpp"
#include "SelfCatalog.hpp"

#include "SubSysFn.hpp"
#include "SubSysCatalog.hpp"

#include "GizmoFn.hpp"
#include "GizmoCatalog.hpp"

#include "ToolFn.hpp"
#include "ToolCatalog.hpp"

#include "CodeCell.hpp"

#include "Bytes_t.hpp"
#include "String_t.hpp"
#include "Atomic.hpp"
#include "NextGid.hpp"
#include "Numbering.hpp"

#include "SP.hpp"
#include "SPmedium.hpp"
#include "SPsmall.hpp"

#include "FocalDesc.hpp"

#include "NamedObject.hpp"

#include "IntLib.hpp"
#include "MagLib.hpp"
#include "Dtm.hpp"
#include "TypedCell.hpp"
#include "CellSingle.hpp"

#include "HshLib.hpp"

#include "IvlLib.hpp"
#include "RndLib.hpp"
#include "TspLib.hpp"

#include "SysCall.hpp"

#include "Env.hpp"
#include "Hardware.hpp"
#include "HwCounters.hpp"
#include "OpSys.hpp"
#include "Process.hpp"
#include "Shell.hpp"

#include "Gid_t.hpp"
#include "JobId.hpp"
#include "LglPtn.hpp"
#include "PhyPtn.hpp"
#include "Lock.hpp"
#include "MasterLock.hpp"
#include "DdlLock.hpp"
#include "DicLock.hpp"
#include "TccLock.hpp"
#include "Locks.hpp"
#include "Mutex.hpp"

#include "SingleWord.hpp"
#include "DoubleWord.hpp"
#include "QuadWord.hpp"
#include "CacheLine.hpp"

#include "AlignedObject.hpp"

#include "Line.hpp"
#include "SpaceUnit.hpp"

#include "WspConvergent_t.hpp"
#include "Wsp_t.hpp"

#include "ShmLib.hpp"
#include "Scanner.hpp"

#include "DecVal.hpp"

//------------------------------------------------------------------------------

#include "PoolBuffer.hpp"
#include "FreePoolBuffer.hpp"
#include "FreeChunk.hpp"
#include "FreedNot.hpp"
#include "BufferPool.hpp"
#include "PoolHelper.hpp"
#include "ResourceHelper.hpp"
#include "PersistenceHelper.hpp"
#include "BufferAddress.h"
#include "BufferId.hpp"
#include "BufferMap.hpp"


#include "HeapBuffer.hpp"
#include "Heaps.hpp"
#include "Heap.hpp"
#include "ObjectSnapshot.hpp"
#include "Region.hpp"
#include "Slab.hpp"
#include "SlabAllocator.hpp"

//------------------------------------------------------------------------------

#include "Birth.hpp"
#include "CRM.hpp"
#include "DecaCnt.hpp"
#include "Decider.hpp"
#include "ClassFlags.hpp"
#include "FreqDist.hpp"
#include "HV.hpp"
#include "MemTest.hpp"
#include "Now.hpp"
#include "Rand.hpp"
#include "SeeMe.hpp"
#include "Wakeup.hpp"
#include "Scan.hpp"
#include "Sprint.hpp"
#include "TagSet.hpp"
#include "TempName.hpp"
#include "TickingDevice.hpp"
#include "Timer.hpp"
#include "AggTimer.hpp"
#include "Gizmos.hpp"
#include "Stats.hpp"

#include "ByteRate.hpp"
#include "Peruse.hpp"
#include "Numbat.hpp"

#include "RU.hpp"
#include "RU_Cpu.hpp"
#include "RU_Memory.hpp"

#include "ArrayNL_t.hpp"

#include "FreeItem.hpp"

#include "HashMapSlot.hpp"
#include "HashMap.hpp"

#include "LatticeNL_t.hpp"
#include "Link_t.hpp"
#include "ListItem_t.hpp"
#include "List_t.hpp"
#include "ListExo_t.hpp"
#include "ListNL_t__.hpp"
#include "ListNL_t.hpp"
#include "Location.hpp"

#include "QueueBuffer.hpp"
#include "Queue.hpp"

#include "SkipNode.hpp"
#include "SkipListNL.hpp"
#include "StackNL_t.hpp"

#include "TextBuffer.hpp"
#include "TextLarge.hpp"
#include "TextMedium.hpp"
#include "TextSmall.hpp"
#include "TextTiny.hpp"
#include "Text.hpp"

#include "Vector_t.hpp"
#include "VectorNL_t.hpp"

#include "Wilderness.hpp"

// -- -- -- -- -- -- -- --

#include "MiniHeap_t.hpp"
#include "TrayNL_t.hpp"

// -- -- -- -- -- -- -- --

#include "Dir.hpp"
#include "Fifo.hpp"
#include "File.hpp"
#include "LocalFile.hpp"
#include "LocalDir.hpp"
#include "PsxThd.hpp"

#include "TlsKeys.hpp"

#include "Resources.hpp"

#include "AnnChar.hpp"
#include "Token.hpp"
#include "TokenVec.hpp"

#include "Charser.hpp"
#include "Input.hpp"

#include "ShmUtility.hpp"

#include "Thing.hpp"
#include "Arrange.hpp"
#include "IntSet.hpp"
#include "IntStack.hpp"

#include "Format.hpp"

#include "PFileUtility.hpp"

//------------------------------------------------------------------------------

#include "ColPix.h"

#include "RowPix.hpp"

#include "Column.hpp"

//------------------------------------------------------------------------------

#include "Literal.hpp"

#include "Licence.hpp"
#include "Limits.hpp"

//------------------------------------------------------------------------------

#include "Operand.hpp"

#include "CodeBlock.hpp"
#include "Word.hpp"

#include "Dic.hpp"
#include "DicStack.hpp"
#include "Local.hpp"
#include "LocalChain.hpp"
#include "TypedStack.hpp"

#include "SenCompiler.hpp"

#include "Profile.hpp"
#include "Task.hpp"

#include "TaskList.hpp"
#include "TaskQueue.hpp"

//------------------------------------------------------------------------------

#include "DataChunk.hpp"
#include "RecordChunk.hpp"
#include "FileChunk.hpp"

#include "IngestChunk.hpp"
#include "IoChunk.hpp"
#include "IoCopy.hpp"
#include "LoadChunk.hpp"

#include "OperandQueue.hpp"

#include "BufferNo.hpp"
#include "LinkedBuffer.hpp"


#include "FileMap.hpp"
#include "PageFile.hpp"
//------------------------------------------------------------------------------

#include "Tx.hpp"
#include "TxSet.hpp"

#include "Ref.hpp"
#include "DataBuffer.hpp"
#include "ObjBuffer.hpp"

#include "CsvMeta.hpp"
#include "CsvBuffer.hpp"
#include "Reader.hpp"

#include "CmdHelper.hpp"


#include "ClassPlaceHolder.hpp"

//------------------------------------------------------------------------------

#include "Scheduler.hpp"

#include "Thread.hpp"
#include "ThreadQueue.hpp"
#include "ThreadVec.hpp"

#include "Schedule.hpp"

//------------------------------------------------------------------------------

#include "SizeHeap.h"

namespace BASE
{
  void vCheckAssumptions(void);
  void vSubSystem(TokenVec* pTokenVec);

  bool bTraceWith(Stz* szWith);
  void vTraceWith(void);

  bool bTraceWithout(Stz* szWith);
  void vTraceWithout(void);

  void vInitAllGlobals(void);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
