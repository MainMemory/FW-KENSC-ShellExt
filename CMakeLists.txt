cmake_minimum_required(VERSION 3.29.6)

project(FW-KENSC_ShellExt LANGUAGES CXX)

add_library(FW-KENSC-ShellExt SHARED
	"FW-KENSC-ShellExt/ClassFactory.cpp"
	"FW-KENSC-ShellExt/ContextMenu.cpp"
	"FW-KENSC-ShellExt/dllmain.cpp"
	"FW-KENSC-ShellExt/mdcomp/src/lib/comper.cc"
	"FW-KENSC-ShellExt/mdcomp/src/lib/enigma.cc"
	"FW-KENSC-ShellExt/mdcomp/src/lib/kosinski.cc"
	"FW-KENSC-ShellExt/mdcomp/src/lib/kosplus.cc"
	"FW-KENSC-ShellExt/mdcomp/src/lib/nemesis.cc"
	"FW-KENSC-ShellExt/mdcomp/src/lib/rocket.cc"
	"FW-KENSC-ShellExt/mdcomp/src/lib/saxman.cc"
	"FW-KENSC-ShellExt/Twizzler/Twizzler.cpp"
)

target_compile_definitions(FW-KENSC-ShellExt PRIVATE "UNICODE=1" "_UNICODE=1")
target_link_libraries(FW-KENSC-ShellExt PRIVATE shlwapi)
target_include_directories(FW-KENSC-ShellExt PRIVATE "FW-KENSC-ShellExt/mdcomp/include")
target_include_directories(FW-KENSC-ShellExt PRIVATE "FW-KENSC-ShellExt/boost/config/include")
target_include_directories(FW-KENSC-ShellExt PRIVATE "FW-KENSC-ShellExt/boost/mpl/include")
target_include_directories(FW-KENSC-ShellExt PRIVATE "FW-KENSC-ShellExt/boost/preprocessor/include")
