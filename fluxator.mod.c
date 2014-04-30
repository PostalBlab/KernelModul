#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
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
	{ 0x568fba06, "module_layout" },
	{ 0x27e44a3d, "cdev_alloc" },
	{ 0x68f58ca6, "cdev_del" },
	{ 0x9c10fd4, "cdev_init" },
	{ 0x4d884691, "malloc_sizes" },
	{ 0x9ee3dc36, "device_destroy" },
	{ 0x7026f1bc, "kobject_set_name" },
	{ 0x7485e15e, "unregister_chrdev_region" },
	{ 0x27e1a049, "printk" },
	{ 0x2b2a7c67, "class_unregister" },
	{ 0x8c971e0b, "device_create" },
	{ 0xd641eabf, "cdev_add" },
	{ 0x7a172b67, "kmem_cache_alloc_trace" },
	{ 0xd52bf1ce, "_raw_spin_lock" },
	{ 0x10f2eb76, "vsnprintf" },
	{ 0x37a0cba, "kfree" },
	{ 0xd3f74e31, "class_destroy" },
	{ 0x704af77a, "__class_create" },
	{ 0x29537c9e, "alloc_chrdev_region" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

