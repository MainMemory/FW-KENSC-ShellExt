cmake_minimum_required(VERSION 3.29.6)

project(FW-KENSC_ShellExt LANGUAGES CXX)

add_library(FW-KENSC-ShellExt SHARED
	"FW-KENSC-ShellExt/ClassFactory.cpp"
	"FW-KENSC-ShellExt/ContextMenu.cpp"
	"FW-KENSC-ShellExt/dllmain.cpp"
	"FW-KENSC-ShellExt/FW-KENSC/comper.cc"
	"FW-KENSC-ShellExt/FW-KENSC/enigma.cc"
	"FW-KENSC-ShellExt/FW-KENSC/kosinski.cc"
	"FW-KENSC-ShellExt/FW-KENSC/kosplus.cc"
	"FW-KENSC-ShellExt/FW-KENSC/nemesis.cc"
	"FW-KENSC-ShellExt/FW-KENSC/rocket.cc"
	"FW-KENSC-ShellExt/FW-KENSC/saxman.cc"
	"FW-KENSC-ShellExt/Twizzler/Twizzler.cpp"
)

target_compile_definitions(FW-KENSC-ShellExt PRIVATE "UNICODE=1" "_UNICODE=1")
target_link_libraries(FW-KENSC-ShellExt PRIVATE shlwapi)
