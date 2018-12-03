#------------------------------------------------------------------------------#
# Pmc.s -- Performance Monitoring Counters                                     #
#------------------------------------------------------------------------------#

#------------------------------------------------------------------------------#
# use cpuid to get pmc information                                             #
#------------------------------------------------------------------------------#

#ifdef __APPLE__

.globl _iPmc_Cpuid
.intel_syntax noprefix

_iPmc_Cpuid:

#else

.globl iPmc_Cpuid
.intel_syntax noprefix

iPmc_Cpuid:

#endif

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -#

# .. to be written ..
#mov rax,73

  mov rax,0
  mov eax,0x0A

  cpuid

#mov eax,edx

ret

#------------------------------------------------------------------------------#
# setup msr to record llc misses                                               #
#------------------------------------------------------------------------------#

#ifdef __APPLE__

.globl _iPmc_GetLlcMisses
.intel_syntax noprefix

_iPmc_GetLlcMisses:

#else

.globl iPmc_GetLlcMisses
.intel_syntax noprefix

iPmc_GetLlcMisses:

#endif

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -#

# .. to be written ..
#mov rax,73

  mov rax,0
  mov eax,0x0A

  cpuid

#mov eax,edx

ret

#------------------------------------------------------------------------------#
# end of file                                                                  #
#------------------------------------------------------------------------------#
