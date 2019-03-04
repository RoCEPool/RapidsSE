#------------------------------------------------------------------------------#
# Asm.s: assembler routines                                                    #
#------------------------------------------------------------------------------#

#
# OS X calling convention:
#
# the first six integer or pointer arguments are passed in
# registers RDI, RSI, RDX, RCX, R8, and R9
#
# the return value is passed in RAX
#
# refer: http://en.wikipedia.org/wiki/X86_calling_conventions
#

#------------------------------------------------------------------------------#
# _iAtomicDec: atomically decrement integer at [rdi]                           #
#------------------------------------------------------------------------------#

#ifdef __APPLE__

.globl _iAtomicDec
.intel_syntax noprefix

_iAtomicDec:

#else

.globl iAtomicDec
.intel_syntax noprefix

iAtomicDec:

#endif

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -#

  mov rcx,-1
  lock xadd [rdi], rcx
  mov rax, rcx
  dec rax

ret

#------------------------------------------------------------------------------#
# _iAtomicInc: atomically increment integer at [rdi]                           #
#------------------------------------------------------------------------------#

#ifdef __APPLE__

.globl _iAtomicInc
.intel_syntax noprefix

_iAtomicInc:

#else

.globl iAtomicInc
.intel_syntax noprefix

iAtomicInc:

#endif

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -#

  mov rcx,1
  lock xadd [rdi], rcx
  mov rax, rcx
  inc rax

ret

#------------------------------------------------------------------------------#
# _iCpuid: cpuid with arg in rdi, register to return in rsi                    #
#------------------------------------------------------------------------------#

#ifdef __APPLE__

.globl _iCpuid
.intel_syntax noprefix

_iCpuid:

#else

.globl iCpuid
.intel_syntax noprefix

iCpuid:

#endif

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -#

  mov rax,0
  mov rax,rdi

  cpuid

   # 0 is eax
  cmp rsi,0
  jne _1

  shl rax,32
  shr rax,32

  ret

   # 1 is ebx
_1:
  cmp rsi,1
  jne _2

  mov rax,0
  mov eax,ebx

  ret

   # 2 is ecx
_2:
  cmp rsi,2
  jne _3

  mov rax,0
  mov eax,ecx

  ret

   # 3 is edx
_3:
  mov rax,0
  mov eax,edx

  ret

ret

#------------------------------------------------------------------------------#
# _iDropRead: drop read lock at [rdi] (no wait)                                #
#------------------------------------------------------------------------------#

#ifdef __APPLE__

.globl _iDropRead
.intel_syntax noprefix

_iDropRead:

#else

.globl iDropRead
.intel_syntax noprefix

iDropRead:

#endif

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -#

_DropRead:
  mov rsi, [rdi]   # get value at rdi
  mov rdx, rsi
  dec rdx          # remove one read lock
  call _TestSet    # attempt to drop read lock

  cmp rax,0

  je _DropRead

  mov rax, 1

ret

#------------------------------------------------------------------------------#
# _iDropWrite: drop write lock at [rdi] (no wait)                              #
#------------------------------------------------------------------------------#

#ifdef __APPLE__

.globl _iDropWrite
.intel_syntax noprefix

_iDropWrite:

#else

.globl iDropWrite
.intel_syntax noprefix

iDropWrite:

#endif

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -#

  mov rax, 0
  mov [rdi], rax   # clear the write lock

  mov rax, 1

ret

#------------------------------------------------------------------------------#
# _iGrabRead: grab read lock at [rdi] (no wait)                                #
#------------------------------------------------------------------------------#

#ifdef __APPLE__

.globl _iGrabRead
.intel_syntax noprefix

_iGrabRead:

#else

.globl iGrabRead
.intel_syntax noprefix

iGrabRead:

#endif

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -#

  mov r9, 1        # construct mask
  shl r9, 63

_iGrabReadRetry:

  mov rax, [rdi]   # get value at rdi
  mov r8, rax      # keep copy in r8

  and rax, r9      # test if currently write locked

  js _GrabReadFailed   # is locked, fail

  mov rsi, r8      # the <if> value
  mov rdx, r8      # construcr <then> value
  inc rdx

  call _TestSet    # attempt to grab

  cmp rax,0
  je _iGrabReadRetry

  mov rax,1
  ret

_GrabReadFailed:
  mov rax, 0
  ret

ret

#------------------------------------------------------------------------------#
# _iGrabWrite: grab write lock at [rdi] (no wait)                              #
#------------------------------------------------------------------------------#

#ifdef __APPLE__

.globl _iGrabWrite
.intel_syntax noprefix

_iGrabWrite:

#else

.globl iGrabWrite
.intel_syntax noprefix

iGrabWrite:

#endif

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -#

  mov rax, [rdi]   # get value at rdi
  mov r8, rax      # keep copy in r8
  mov r9, 1        # construct mask
  shl r9, 63

  and rax, r9      # test if write locked already

  js _GrabWriteFailed   # is locked, fail

  mov rsi, r8      # the <if> value
  mov rdx, r8      # construct <then> value
  or  rdx, r9

  call _TestSet    # attempt to grab

  cmp rax,0
  je _GrabWriteFailed

_GrabWriteWait:
  mov rax, [rdi]
  cmp rax, r9
  jne _GrabWriteWait
  
  mov rax,1
  ret

_GrabWriteFailed:
  mov rax, 0
  ret

ret

#------------------------------------------------------------------------------#
# _iIsLocked: return true if locked                                            #
#------------------------------------------------------------------------------#

#ifdef __APPLE__

.globl _iIsLocked
.intel_syntax noprefix

_iIsLocked:

#else

.globl iIsLocked
.intel_syntax noprefix

iIsLocked:

#endif

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -#

  mov rax, [rdi]   # get value at rdi

  mov r9, 0

  cmp rax, r9      # test if currently locked

  jne _IsLocked    # is locked

  mov rax,0
  ret

_IsLocked:
  mov rax, 1
  ret

ret

#------------------------------------------------------------------------------#
# _iPmcInit: initialise performance measurement counters                       #
#------------------------------------------------------------------------------#

#ifdef __APPLE__

.globl _iPmcInit
.intel_syntax noprefix

_iPmcInit:

#else

.globl iPmcInit
.intel_syntax noprefix

iPmcInit:

#endif

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -#

# to be written ...

ret

#------------------------------------------------------------------------------#
# _iRdPmc: read performance measurement counter                                #
#------------------------------------------------------------------------------#

#ifdef __APPLE__

.globl _iRdPmc
.intel_syntax noprefix

_iRdPmc:

#else

.globl iRdPmc
.intel_syntax noprefix

iRdPmc:

#endif

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -#

# to be written ...

ret

#------------------------------------------------------------------------------#
# _iRdtscp: read time-stamp counter                                            #
#------------------------------------------------------------------------------#

#ifdef __APPLE__

.globl _iRdtscp
.intel_syntax noprefix

_iRdtscp:

#else

.globl iRdtscp
.intel_syntax noprefix

iRdtscp:

#endif

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -#

  push rbx
  push rcx
  push rdx

  rdtscp
  shl rdx,32
  or rax,rdx

  pop rdx
  pop rcx
  pop rbx

ret

#------------------------------------------------------------------------------#
# _iTestSet: atomic test and set                                               #
#------------------------------------------------------------------------------#

#ifdef __APPLE__

.globl _iTestSet
.intel_syntax noprefix

_iTestSet:

#else

.globl iTestSet
.intel_syntax noprefix

iTestSet:

#endif

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -#

_TestSet:
  mov rax, rsi

  lock cmpxchg [rdi], rdx

  cmp rax, rsi

  jne _Failed

_Success:
  mov rax,1
  ret

_Failed:
  mov rax,0
  ret


#------------------------------------------------------------------------------#
#------------------------------------------------------------------------------#
#------------------------------------------------------------------------------#


#------------------------------------------------------------------------------#
# _vPrefetch: prefetch                                                         #
#------------------------------------------------------------------------------#

#ifdef __APPLE__

.globl _vPrefetch
.intel_syntax noprefix

_vPrefetch:

#else

.globl vPrefetch
.intel_syntax noprefix

vPrefetch:

#endif

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -#

  prefetcht0 [rdi]

ret


#------------------------------------------------------------------------------#
# _vPrefetchT0: prefetch data into all levels of the cache hierarchy           #
#------------------------------------------------------------------------------#

#ifdef __APPLE__

.globl _vPrefetchT0
.intel_syntax noprefix

_vPrefetchT0:

#else

.globl vPrefetchT0
.intel_syntax noprefix

vPrefetchT0:

#endif

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -#

  prefetcht0 [rdi]

ret


#------------------------------------------------------------------------------#
# _vPrefetchT1: prefetch data into level 2 cache and higher                    #
#------------------------------------------------------------------------------#

#ifdef __APPLE__

.globl _vPrefetchT1
.intel_syntax noprefix

_vPrefetchT1:

#else

.globl vPrefetchT1
.intel_syntax noprefix

vPrefetchT1:

#endif

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -#

  prefetcht1 [rdi]

ret


#------------------------------------------------------------------------------#
# _vPrefetchT2: prefetch data into level 3 cache and higher                    #
#------------------------------------------------------------------------------#

#ifdef __APPLE__

.globl _vPrefetchT2
.intel_syntax noprefix

_vPrefetchT2:

#else

.globl vPrefetchT2
.intel_syntax noprefix

vPrefetchT2:

#endif

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -#

  prefetcht2 [rdi]

ret


#------------------------------------------------------------------------------#
# _vPrefetchNta: prefetch data into non-temporal cache structure               #
#------------------------------------------------------------------------------#

#ifdef __APPLE__

.globl _vPrefetchNta
.intel_syntax noprefix

_vPrefetchNta:

#else

.globl vPrefetchNta
.intel_syntax noprefix

vPrefetchNta:

#endif

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -#

  prefetchnta [rdi]

ret


#------------------------------------------------------------------------------#
# _iTestPrefetch: test for prefetch                                            #
#------------------------------------------------------------------------------#

#ifdef __APPLE__

.globl _iTestPrefetch
.intel_syntax noprefix

_iTestPrefetch:

#else

.globl iTestPrefetch
.intel_syntax noprefix

iTestPrefetch:

#endif

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -#

  mov ecx,0x1a4
  rdmsr

ret

#------------------------------------------------------------------------------#
# end of file                                                                  #
#------------------------------------------------------------------------------#
