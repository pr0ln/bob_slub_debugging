#include<linux/module.h>
#include<linux/version.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/kprobes.h>

int pre_handler(struct kprobe *p, struct pt_regs *regs) {
	char * slab_name;
	// r15 == kmem_cache
	//
	if(regs->ax == 0)
		return 0;

	if(regs->r12 == 0)
		return 0;

	slab_name = *((char **)(regs->r12 + 0x60));

	if(slab_name == 0)
		return 0;

	// print only slubtest
	if(strncmp(slab_name, "slubtest", 8) == 0)
	{
		printk(KERN_INFO "[SLUB_DEUBG] CPU %d enter kmem_cache_alloc slow-path-2", smp_processor_id());
	}
        return 0;
}

//void post_handler(struct kprobe *p, struct pt_regs *regs, unsigned long flags){
//        printk(KERN_INFO "instruction start\n");
//}

struct kprobe kp;

int kprobe_init(void) {
	memset(&kp, 0, sizeof(kp));
	//kmem_cache slow-path1 offset 4.15.0-191
	kp.addr =  (kprobe_opcode_t *)((unsigned long)kallsyms_lookup_name("___slab_alloc")+0x174);
	kp.pre_handler = pre_handler;
	register_kprobe(&kp);
        printk(KERN_INFO "Slub slow-path2 kprobe at %p\n", kp.addr);
        return 0;
}

void kprobe_exit(void) {
        unregister_kprobe(&kp);
        printk(KERN_INFO "kprobe at %p unregistered\n", kp.addr);
}

module_init(kprobe_init);
module_exit(kprobe_exit);
MODULE_AUTHOR("Pr0ln");
MODULE_DESCRIPTION("kprobe slub slowpath-2");
MODULE_LICENSE("GPL");
