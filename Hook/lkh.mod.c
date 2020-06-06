#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x96b3eea1, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0xe33f5a06, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0xf9a482f9, __VMLINUX_SYMBOL_STR(msleep) },
	{ 0x754d539c, __VMLINUX_SYMBOL_STR(strlen) },
	{ 0x24428be5, __VMLINUX_SYMBOL_STR(strncpy_from_user) },
	{ 0x52f79a83, __VMLINUX_SYMBOL_STR(param_ops_charp) },
	{ 0xf1997f66, __VMLINUX_SYMBOL_STR(current_task) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xb2bfec72, __VMLINUX_SYMBOL_STR(netlink_kernel_release) },
	{ 0x9166fada, __VMLINUX_SYMBOL_STR(strncpy) },
	{ 0xc0c88844, __VMLINUX_SYMBOL_STR(netlink_unicast) },
	{ 0xeb676390, __VMLINUX_SYMBOL_STR(init_net) },
	{ 0xd8d4c641, __VMLINUX_SYMBOL_STR(init_task) },
	{ 0x6aa32a, __VMLINUX_SYMBOL_STR(__alloc_skb) },
	{ 0xdb7305a1, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{        0, __VMLINUX_SYMBOL_STR(__fentry__) },
	{ 0x4aaa31f9, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0xb3e96735, __VMLINUX_SYMBOL_STR(__netlink_kernel_create) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x28318305, __VMLINUX_SYMBOL_STR(snprintf) },
	{        0, __VMLINUX_SYMBOL_STR(_copy_from_user) },
	{ 0xa4c8728b, __VMLINUX_SYMBOL_STR(__nlmsg_put) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "68E465CCCB9693588219F41");
