# Copyright (c) 2015 Jason Power
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met: redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer;
# redistributions in binary form must reproduce the above copyright
# notice, this list of conditions and the following disclaimer in the
# documentation and/or other materials provided with the distribution;
# neither the name of the copyright holders nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

"""
This is the ARM equivalent to `simple.py` (which is designed to run using the
X86 ISA). More detailed documentation can be found in `simple.py`.
"""

import sys, os
import m5
from m5.objects import *

# create the system we are going to simulate
system = System()

# Set the clock frequency of the system (and all of its children)
system.clk_domain = SrcClockDomain()
system.clk_domain.clock = "2GHz"
system.clk_domain.voltage_domain = VoltageDomain()

# Set up the system
system.mem_mode = "atomic" # Use timing accesses
system.mem_ranges = [AddrRange("4GiB")] # Create an address range
system.cpu = ArmAtomicSimpleCPU()

# Create a memory bus, a system crossbar, in this case
system.membus = SystemXBar()

# Hook the CPU ports up to the membus
system.cpu.icache_port = system.membus.cpu_side_ports
system.cpu.dcache_port = system.membus.cpu_side_ports

# create the interrupt controller for the CPU and connect to the membus
system.cpu.createInterruptController()

# Create a DDR3 memory controller and connect it to the membus
system.mem_ctrl = MemCtrl()
system.mem_ctrl.dram = DDR4_2400_8x8()
system.mem_ctrl.dram.range = system.mem_ranges[0]
system.mem_ctrl.port = system.membus.mem_side_ports

# Connect the system up to the membus
system.system_port = system.membus.cpu_side_ports

#Branch Prediction
if (len(sys.argv) > 1):
    match sys.argv[1].lower():
        case 'gag':
            system.cpu.branchPred = GAgBP()
        case 'gap':
            system.cpu.branchPred = GApBP()
        case 'pag':
            system.cpu.branchPred = PAgBP()
        case 'pap':
            system.cpu.branchPred = PApBP()
        case 'bimodal':
            system.cpu.branchPred = BimodalBP()
        case 'gshare':
            system.cpu.branchPred = GShareBP()
        case 'perceptron':
            system.cpu.branchPred = MultiperspectivePerceptron8KB()
        case 'tournament':
            system.cpu.branchPred = TournamentBP()
else:
    system.cpu.branchPred = GAgBP()
    # system.cpu.branchPred = BimodalBP()
    # system.cpu.branchPred = GApBP()

if (len(sys.argv) > 2) and os.path.isfile(sys.argv[2]):
    binary = sys.argv[2]
else:
    # Here we set the arm "hello world" binary. With other ISAs you must specify
    # workloads compiled to those ISAs. Other "hello world" binaries for other ISAs
    # can be found in "tests/test-progs/hello".
    thispath = os.path.dirname(os.path.realpath(__file__))
    binary = os.path.join(
        thispath,
        #Sets up three directory levels
        "../../../",
        #Change to the path of our program
        #"configs/learning_gem5/part1/test_files/multihelloworld-arm"
        # "configs/learning_gem5/telecomm/CRC32/crc"
        "mibench/automotive/basicmath/basicmath_small"
        ,
    )

system.workload = SEWorkload.init_compatible(binary)

# Create a process for a simple "Hello World" application
process = Process()
# Set the command
# cmd is a list which begins with the executable (like argv)
process.cmd = [binary]
# Set the cpu to use the process as its workload and create thread contexts
system.cpu.workload = process
system.cpu.createThreads()

# set up the root SimObject and start the simulation
root = Root(full_system=False, system=system)
# instantiate all of the objects we've created above
m5.instantiate()

print(f"Beginning simulation!")
exit_event = m5.simulate()
print(f"Exiting @ tick {m5.curTick()} because {exit_event.getCause()}")
