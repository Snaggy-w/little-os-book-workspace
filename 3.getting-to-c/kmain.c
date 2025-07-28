void kmain(void){
    while(1)
     	__asm__ __volatile__(
        "movl $0xDEADBEEF, %%ebx\n"
        "movl $0xCAFED00D, %%eax\n"
        ::: "ebx", "eax"
    );
}
