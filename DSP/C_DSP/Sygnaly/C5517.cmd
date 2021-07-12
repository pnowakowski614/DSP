
-stack    0x2000      /* Primary stack size   */
-sysstack 0x1000      /* Secondary stack size */
-heap     0x2000      /* Heap area size       */

-c                    /* Use C linking conventions: auto-init vars at runtime */
-u _Reset             /* Force load of reset interrupt handler                */

/* SPECIFY THE SYSTEM MEMORY MAP */

MEMORY
{
 PAGE 0:  /* ---- Unified Program/Data Address Space ---- */

  MMR    (RWIX): origin = 0x000000, length = 0x0000c0  /* MMRs */
  DARAM0 (RWIX): origin = 0x0000c0, length = 0x00ff40  /*  64KB - MMRs */
  SARAM0 (RWIX): origin = 0x010000, length = 0x010000  /*  64KB */
  SARAM1 (RWIX): origin = 0x020000, length = 0x020000  /* 128KB */
  SARAM2 (RWIX): origin = 0x040000, length = 0x00FE00  /*  64KB */
  VECS   (RWIX): origin = 0x04FE00, length = 0x000200  /*  512B */
  PDROM   (RIX): origin = 0xff8000, length = 0x008000  /*  32KB */

 PAGE 2:  /* -------- 64K-word I/O Address Space -------- */

  IOPORT (RWI) : origin = 0x000000, length = 0x020000
}

/* SPECIFY THE SECTIONS ALLOCATION INTO MEMORY */

SECTIONS
{
   .text     >> SARAM1|SARAM2|SARAM0  /* Code                        */

   /* Both stacks must be on same physical memory page               */
   .stack    >  DARAM0                /* Primary system stack        */
   .sysstack >  DARAM0                /* Secondary system stack      */

   .data     >> DARAM0|SARAM0|SARAM1  /* Initialized vars            */
   .bss      >> DARAM0|SARAM0|SARAM1  /* Global & static vars        */
   .const    >> DARAM0|SARAM0|SARAM1  /* Constant data               */
   .sysmem   >  DARAM0|SARAM0|SARAM1  /* Dynamic memory (malloc)     */
   .switch   >  SARAM2                /* Switch statement tables     */
   .cinit    >  SARAM2                /* Auto-initialization tables  */
   .pinit    >  SARAM2                /* Initialization fn tables    */
   .cio      >  SARAM2                /* C I/O buffers               */
   .args     >  SARAM2                /* Arguments to main()         */

    vectors  >  VECS                  /* Interrupt vectors           */

   .ioport   >  IOPORT PAGE 2         /* Global & static ioport vars */

   .fftcode  >  SARAM0
   .data:twiddle  >  SARAM1, align(2048)
   .input    >  DARAM0, align(4)
}
