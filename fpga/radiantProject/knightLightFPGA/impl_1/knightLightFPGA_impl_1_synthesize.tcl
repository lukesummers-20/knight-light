if {[catch {

# define run engine funtion
source [file join {C:/lscc/radiant/2024.1} scripts tcl flow run_engine.tcl]
# define global variables
global para
set para(gui_mode) "1"
set para(prj_dir) "C:/Users/mdesomma/Documents/knight-light/fpga/radiantProject/knightLightFPGA"
# synthesize IPs
# synthesize VMs
# synthesize top design
file delete -force -- knightLightFPGA_impl_1.vm knightLightFPGA_impl_1.ldc
::radiant::runengine::run_engine_newmsg synthesis -f "knightLightFPGA_impl_1_lattice.synproj" -logfile "knightLightFPGA_impl_1_lattice.srp"
::radiant::runengine::run_postsyn [list -a iCE40UP -p iCE40UP5K -t SG48 -sp High-Performance_1.2V -oc Industrial -top -w -o knightLightFPGA_impl_1_syn.udb knightLightFPGA_impl_1.vm] [list C:/Users/mdesomma/Documents/knight-light/fpga/radiantProject/knightLightFPGA/impl_1/knightLightFPGA_impl_1.ldc]

} out]} {
   ::radiant::runengine::runtime_log $out
   exit 1
}
