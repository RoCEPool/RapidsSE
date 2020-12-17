/*============================================================================*/
/* Resources.h -- Resources namespace                                         */
/*============================================================================*/
#pragma once

//------------------------------------------------------------------------------

namespace Resources
{
  void vCheckAssumptions(void);
}

//------------------------------------------------------------------------------

//#pragma GCC diagnostic warning "-Wunused-parameter"
//#pragma GCC diagnostic warning "-Wunused-variable"

#define EndDefine

#define _static_ EndDefine
#define _nonProper_ EndDefine

#include "CompiledWith.h"
#include "MachineTypes.h"
#include "SystemIncludes.h"
#include "UsefulConstants.h"
#include "UserStrSizes.h"
#include "Namespaces.h"

//------------------------------------------------------------------------------

namespace Resources {

  template <int BYTES> class String_t;
  typedef String_t<32>     s32;
  typedef String_t<256>    s256;
  typedef String_t<C_2K>   s2K;   // note: 2K not 2KB
  typedef String_t<C_4K>   s4K;   // note: 4K not 4KB
  typedef String_t<C_16KB> s16KB; // note: this is 16KB
  typedef String_t<C_24KB> s24KB; // note: this is 17KB
  typedef String_t<C_48K>  s48K;  // note: 48K not 24KB
  typedef String_t<C_1M>   s1M;   // note: 1M not 1MB

  typedef String_t<C_MAX_STATEMENT> sSTMT;
  typedef String_t<C_MAX_VCH>       sVCH;

  template <int BYTES> class Bytes_t;
  typedef Bytes_t<32>      b32;
  typedef Bytes_t<256>     b256;
  typedef Bytes_t<C_4K>    b4K;   // note: K, not KB
  typedef Bytes_t<C_8K>    b8K;   // note: K, not KB
  typedef Bytes_t<C_32K>   b32K;  // note: K, not KB
  typedef Bytes_t<C_256K>  b256K; // note: K, not KB
}

namespace Resources       { class ForthCell;            class Focal;
                              class Header;          class Object;
                              class Statistics;     class Mutex;              }
namespace Resources            { class ReqTuple;                              }
namespace Resources           { class Lock;            class LockTracker;       }
namespace Resources              { class BufferPool;      class HeapBuffer;
                              class HeapMem;         class PoolBuffer;

class IvlLib;

}


using namespace Resources;


//------------------------------------------------------------------------------
#include "AxiomaticConstants.h"
#include "BitLib.hpp"
#include "ByteLiterals.hpp"
#include "CommonTypes.h"
#include "EnvFlags.h"
#include "Gid.h"
#include "Nibbler.h"
#include "Null.hpp"
#include "WordAsBytes.h"


#if !( (defined(WITH_APPLE) || defined(WITH_LINUX)) )
  #include "Asm.h"
#endif

#include "Asm.h"

#include "PmcUse.h"


#include "Stz.hpp"
#include "StrLib.hpp"



#include "ClassNumbers.hpp"
#include "FnDesc.hpp"
#include "FnDefn.hpp"
#include "FnCatalog.hpp"
#include "LimitsFormats.h"

#include "AM.hpp"
#include "eHT.h"

struct SizeHeap {
  
  i8  m_nClassSize;
  eHT m_eWhichHeap;
  
  i8  m_nVtableAdjust; // temp: explore support for vtable

};



#include "Hack.hpp"

#define PASTE3(a,b,c) a##b##c
#define GLOBALNM(_fun,_class,_line) PASTE3(_fun,_class,_line)
#define HACK(_class,_text) Hack GLOBALNM(Hack,_class,__LINE__)(_text,__FILE__,__LINE__)

#include "eFK.h"
#include "Exception.hpp"


#include "Footer.hpp"
#include "Header.hpp"

#include "Correct.hpp"

#include "Fault.hpp"

#include "MemLib.hpp"

#include "AssertMacros.h"

#include "Object.hpp"
#include "StackObject.hpp"
#include "NonProper.hpp"

#include "Addressing.hpp"
#include "SizedHeader.hpp"
#include "Trace.hpp"
#include "Jot.hpp"
#include "IntLib.hpp"

#include "Statistics.hpp"
#include "Focal.hpp"

#include "String.hpp"

#include "PTR.hpp"
#include "SMA.hpp"
#include "REF_t.hpp"
#include "REG_t.hpp"


namespace Tasking {
  typedef void (Step::*StepFp)(void);
}


#include "ForthCell.hpp"


//typedef void (pDUMPFUN)(eCN eClass, Object* pObject, Stz*);
//extern pDUMPFUN *gpDump[];

typedef void (pOUTLINEFUN)(Focal* pFocal, Stz*);
extern pOUTLINEFUN *gpOutline[];



#include "Bytes_t.hpp"
#include "String_t.hpp"

#include "SP.hpp"
#include "SPmedium.hpp"
#include "SPsmall.hpp"




#include "FocalDesc.hpp"

#include "NamedObject.hpp"
#include "ReqTuple.hpp"
#include "SP_Gid.hpp"


#include "IntLib.hpp"
#include "MagLib.hpp"
#include "Dtm.hpp"
#include "HshLib.hpp"

#include "SP_Bits.hpp"

//#include "Compress.hpp"
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


#include "Atomic.hpp"
#include "Lock.hpp"
#include "LockTracker.hpp"
#include "Mutex.hpp"

#include "SP_Lock.hpp"
#include "SP_Mutex.hpp"


#include "SingleWord.hpp"
#include "AlignedObject.hpp"

#include "Line.hpp"
#include "DoubleWord.hpp"
#include "QuadWord.hpp"
#include "CacheLine.hpp"
#include "SpaceUnit.hpp"

#include "WspConvergent_t.hpp"
#include "Wsp_t.hpp"



#include "ShmLib.hpp"



static const u8 C_SIZE_POOL_BUFFER   = C_1MB;

static const i8 C_SIZE_TINY_OBJECT   = C_1KB;
static const i8 C_SIZE_SMALL_OBJECT  = C_4KB;
static const i8 C_SIZE_MEDIUM_OBJECT = C_64KB;
static const i8 C_SIZE_LARGE_OBJECT  = C_SIZE_POOL_BUFFER - C_SIZE_SMALL_OBJECT;

static const u8 C_MASK_TINY_OBJECT   = 0x3FF;
static const u8 C_MASK_SMALL_OBJECT  = 0xFFF;
static const u8 C_MASK_MEDIUM_OBJECT = 0xFFFF;
static const u8 C_MASK_LARGE_OBJECT  = 0xFFFFF;

static const i8 C_LOW_BUFFERS        = 512;

static const u8 C_OBJECT_TO_BUFFER_MASK = BITWISE_NOT((C_SIZE_POOL_BUFFER - 1));

static const i8 C_VIA_MALLOC         = 1;

#include "PoolBuffer.hpp"
#include "FreeBuffer.hpp"
#include "FreeChunk.hpp"
#include "BufferPool.hpp"

#include "HeapBuffer.hpp"
#include "HeapMem.hpp"
#include "Heap.hpp"
#include "ObjectsInPool.hpp"
#include "Region.hpp"
#include "Slab.hpp"
#include "SlabAllocator.hpp"


extern BufferPool* gpBufferPool;

/*---------------------------------------------------------------------------*/
/* pCheck: check address                                                     */
/*---------------------------------------------------------------------------*/
template <class OBJECT>
static OBJECT pPoolCheck(void* aAddress)
{
#ifdef WITH_ADDRESS_CHECKING
  if (aAddress)
    gpBufferPool->vCheckAddress(aAddress);
#endif
  
  return static_cast<OBJECT>(aAddress);
}

namespace Resources
{
  bool bTraceWith(Stz* szWith);
  void vTraceWith(void);

  bool bTraceWithout(Stz* szWith);
  void vTraceWithout(void);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
