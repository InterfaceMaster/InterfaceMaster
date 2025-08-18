/*=============================================================================
 * This file is a Linker Script for the external SDRAM on the STM32H723G.
 * It is meant to be used alongside your project's main linker script.
 *===========================================================================*/

/*
 * The MEMORY block defines the processor's memory map.
 * We specify the starting address (ORIGIN) and the size (LENGTH) for the SDRAM.
 * NOTE: The values below are a general example. Be sure to use the correct
 * addresses for your specific hardware as found in the reference manual.
 */
MEMORY
{
  /* The correct starting address for FMC SDRAM Bank1 is 0xCC000000.
   * This is based on the table you provided.
   * rwx: read, write, execute - indicates the memory is both readable and writable.
   */
  SDRAM (rw) : ORIGIN = 0xCC000000, LENGTH = 2M
}

/*
 * The SECTIONS block defines where different parts of the compiled program
 * will be placed in the memory map.
 * We create a special section named ".sdram_data" to place variables
 * in the SDRAM memory region defined above.
 */
SECTIONS
{
  /* The .sdram_data section holds user-defined data placed in SDRAM. */
  .sdram_data :
  {
    /* Align the data to a 4-byte (32-bit) boundary. */
    . = ALIGN(4);

    /* This symbol marks the start address of the .sdram_data section. */
    _sdsram_start = .;

  /* Any variables assigned the section(".sdram_data") attribute will be placed here. */
  
    . = ALIGN(4);
    _sdsram_end = .;

  /* ">SDRAM AT>SDRAM" indicates that the data in this section should be
   * placed in the "SDRAM" memory region. */
  } >SDRAM AT>SDRAM
}