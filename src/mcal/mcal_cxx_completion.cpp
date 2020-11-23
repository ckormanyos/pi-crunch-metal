extern "C" int  __aeabi_atexit(void*);
extern "C" void exit          (int);
extern "C" void abort         (void);

extern "C" int  __aeabi_atexit(void*) { return 0; }
extern "C" void exit          (int)   { }
extern "C" void abort         (void)  { }
