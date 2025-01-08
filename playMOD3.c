/**
 * Usage of ptplayer
 * Many thanks to Weij Ju Wu (https://github.com/weiju)
 */
#include <hardware/custom.h>
#include <graphics/gfxbase.h>
#include <devices/input.h>

#include <clib/exec_protos.h>
#include <clib/graphics_protos.h>
#include <clib/intuition_protos.h>
#include <clib/alib_protos.h>

#include <stdio.h>

// Download from Aminet
// http://aminet.net/package/dev/c/SDI_headers
// http://aminet.net/package/mus/play/ptplayer
#include <SDI/SDI_compiler.h>
#include "ptplayer.h"

extern struct GfxBase *GfxBase;
extern struct Custom custom;

static int should_exit = 0;



#define MOD_SIZE (115038)
#define MOD_NAME "deephouse.mod"
static UBYTE __chip mod_data[MOD_SIZE];

int pause(){
    mt_Enable=0;
}

int play_mod(const char *mod_name) 
{
    //printf("Starting example_01...\n");


    //printf("Loading MOD file: %s\n", MOD_NAME);
    FILE *fp = fopen(MOD_NAME, "rb");
    if (!fp) {
        printf("Error: Could not open file '%s'. Exiting...\n", MOD_NAME);
        return 1;
    }

    int bytes_read = fread(mod_data, sizeof(UBYTE), MOD_SIZE, fp);
    fclose(fp);
    if (bytes_read != MOD_SIZE) {
        printf("Error: Read %d bytes, expected %d bytes. Exiting...\n", bytes_read, MOD_SIZE);
        return 1;
    }
    //printf("MOD file successfully loaded (%d bytes).\n", bytes_read);

    BOOL is_pal = (((struct GfxBase *)GfxBase)->DisplayFlags & PAL) == PAL;
    //printf("Display mode detected: %s\n", is_pal ? "PAL" : "NTSC");

    // Initialize ptplayer
    printf("Initializing ptplayer...\n");
    void *p_samples = NULL;
    UBYTE start_pos = 0;

    //printf("Calling mt_install_cia...\n");
    mt_install_cia(&custom, NULL, is_pal);
    //printf("mt_install_cia completed.\n");

    //printf("Calling mt_init...\n");
    mt_init(&custom, mod_data, p_samples, start_pos);
    //printf("mt_init completed.\n");

    mt_Enable = 1;
    //printf("Music playback started. Waiting for user interaction...\n");

   
   // printf("Exiting main loop. Cleaning up...\n");
   // mt_Enable = 0;

   // printf("Calling mt_end...\n");
   // mt_end(&custom);
   // printf("mt_end completed.\n");

  //  printf("Calling mt_remove_cia...\n");
  //  mt_remove_cia(&custom);
  //  printf("mt_remove_cia completed.\n");

  //  printf("Program terminated successfully.\n");
    return 0;
}
