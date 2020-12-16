#==============================================================================#
# PmcUse.s -- Use Performance Monitoring Counters                              #
#==============================================================================#

#------------------------------------------------------------------------------#
# iPmc_Core -- use cpuid to get current core                                   #
#------------------------------------------------------------------------------#
#ifdef __APPLE__
.globl _iPmc_Core
_iPmc_Core:
#else
.globl iPmc_Core
iPmc_Core:
#endif
.intel_syntax noprefix

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -#
#                                                                              #
# Refer: CPUID -- CPU identification instruction in Intel Manual Vol 2A        #
# Refer: Table 3-8 Information Returned by CPUID Instruction Vol 2A            #
#                                                                              #
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -#

  # use RBX, RCX, RDX
  push rbx
  push rcx
  push rdx

  # clear RAX
  mov rax,0

  # put x01 into eax to get Basic CPUID Information leaf (part 01)
  mov eax,0x0B
  mov ecx,0

  # get leaf
  cpuid

  //shr ebx,24
  //mov eax,ebx

  #shl ebx,16
  #shr ebx,16
  mov eax,edx

  # restore
  pop rdx
  pop rcx
  pop rbx

ret
#------------------------------------------------------------------------------#


#------------------------------------------------------------------------------#
# iPmc_Info -- use cpuid to get pmc information                                #
#------------------------------------------------------------------------------#
#ifdef __APPLE__
.globl _iPmc_Info
_iPmc_Info:
#else
.globl iPmc_Info
iPmc_Info:
#endif
.intel_syntax noprefix

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -#
#                                                                              #
# Refer: CPUID -- CPU identification instruction in Intel Manual Vol 2A        #
#                                                                              #
# Refer: Table 3-8 Information Returned by CPUID Instruction Vol 2A            #
#                                                                              #
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -#

  # use RBX, RCX, RDX
  push rbx
  push rcx
  push rdx

  # clear RAX
  mov rax,0

  # put x0A into eax to get the Architectural Performance Monitoring Leaf
  mov eax,0x0A

  # get leaf
  cpuid

  # rax/eax contains:
  #   bits 07..00: version ID of architectural performance monitoring
  #   bits 15..08: number of general purpose pm counters per logical processor
  #   bits 23..16: bit width of each general purpose pm counter
  #   bits 31..24: length of EBX vector to enumerate architectural pm events

  # restore
  pop rdx
  pop rcx
  pop rbx

ret
#------------------------------------------------------------------------------#


#------------------------------------------------------------------------------#
# iPmc_GetLlcMisses -- get llc misses from IA32_PMC                            #
#------------------------------------------------------------------------------#
#ifdef __APPLE__
.globl _iPmc_GetLlcMisses
_iPmc_GetLlcMisses:
#else
.globl iPmc_GetLlcMisses
iPmc_GetLlcMisses:
#endif
.intel_syntax noprefix

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -#
#                                                                              #
# Refer: RDPMC -- Read Performance Monitoring Counter in Intel Manual Vol 2B   #
#                                                                              #
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -#

  # use RBX, RCX, RDX
  push rbx
  push rcx
  push rdx

  # serialize
  mov eax,0x0A
  cpuid

  # zero RAX, RDX
  mov rax,0x0
  mov rdx,0x0

  # RDPMC uses the index relative to the base of each counter type
  mov ecx,0x1  # we will read IA32_PMC1

  # read IA32_PMC1 into EDX:EAX
  rdpmc

  # create a 64 bit integer in RAX from EDX:EAX
  shl rdx,32
  or  rax,rdx

  # restore
  pop rdx
  pop rcx
  pop rbx

ret
#------------------------------------------------------------------------------#


#==============================================================================#
# end of file                                                                  #
#==============================================================================#
