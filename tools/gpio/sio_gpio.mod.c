#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif


static const char ____versions[]
__used __section("__versions") =
	"\x1c\x00\x00\x00\x8d\x5d\xa1\xa9"
	"nonseekable_open\0\0\0\0"
	"\x24\x00\x00\x00\x52\x3f\x0a\x4b"
	"gic_nonsecure_priorities\0\0\0\0"
	"\x18\x00\x00\x00\x7f\x13\x45\x66"
	"misc_deregister\0"
	"\x18\x00\x00\x00\x92\x6c\xdf\xdb"
	"ioport_resource\0"
	"\x1c\x00\x00\x00\xc2\xc7\x35\x10"
	"__release_region\0\0\0\0"
	"\x14\x00\x00\x00\xd4\xc3\xa8\xdc"
	"logic_outb\0\0"
	"\x1c\x00\x00\x00\x08\x16\xbd\x85"
	"__request_region\0\0\0\0"
	"\x14\x00\x00\x00\xad\x00\x83\xf1"
	"logic_inb\0\0\0"
	"\x2c\x00\x00\x00\x61\xe5\x48\xa6"
	"__ubsan_handle_shift_out_of_bounds\0\0"
	"\x10\x00\x00\x00\xd8\x7e\x99\x92"
	"_printk\0"
	"\x18\x00\x00\x00\xb3\xc5\x71\xda"
	"misc_register\0\0\0"
	"\x18\x00\x00\x00\x64\xbd\x8f\xba"
	"_raw_spin_lock\0\0"
	"\x1c\x00\x00\x00\x31\x92\x07\x6b"
	"alt_cb_patch_nops\0\0\0"
	"\x18\x00\x00\x00\x30\xfb\x88\xb7"
	"gic_pmr_sync\0\0\0\0"
	"\x1c\x00\x00\x00\x34\x4b\xb5\xb5"
	"_raw_spin_unlock\0\0\0\0"
	"\x14\x00\x00\x00\xcc\xcd\x48\x89"
	"cpu_hwcaps\0\0"
	"\x18\x00\x00\x00\x8a\xef\x86\xdc"
	"module_layout\0\0\0"
	"\x00\x00\x00\x00\x00\x00\x00\x00";

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "0FF81E12BEFD966C9C45D20");
