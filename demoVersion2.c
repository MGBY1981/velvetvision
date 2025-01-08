#include <proto/intuition.h>
#include <proto/graphics.h>
#include <stdio.h>
#include <exec/memory.h>
#include <diskfont/diskfont.h>
#include "modplayer.h"
#include "subanimation1.h"

#define SCREENWIDTH 320
#define SCREENHEIGHT 240
#define PLANES 2
#define IMAGEPLANES 2
#define NUM_IMAGES 20

#define IMG2_PLANES 2
#define IMG2_IMAGEPLANES 2
#define IMG2_NUM_IMAGES 19

#define IMG3_PLANES 2
#define IMG3_IMAGEPLANES 2
#define IMG3_NUM_IMAGES 7

#define IMG4_PLANES 2
#define IMG4_IMAGEPLANES 2
#define IMG4_NUM_IMAGES 17

#define IMG5_PLANES 2
#define IMG5_IMAGEPLANES 2
#define IMG5_NUM_IMAGES 20

#define IMG7_PLANES 4
#define IMG7_IMAGEPLANES 4
#define IMG7_NUM_IMAGES 9


static struct RastPort screenContext;
static struct BitMap screenBitMap;

static struct RastPort screenContextTwo;
static struct BitMap screenBitMapTwo;
static struct Screen *screen;
struct TextFont *font;



static struct NewScreen screenProperties = {
    0, 0, SCREENWIDTH, SCREENHEIGHT, PLANES, 0, 0, 0,
    CUSTOMSCREEN | CUSTOMBITMAP | SCREENQUIET,
    NULL, NULL, NULL, NULL,
};


extern struct GfxBase *GfxBase;

#include <stdio.h>


#include <dos/dos.h>
#include <proto/dos.h>
#include <stdio.h>



// setting RGB4 pallette
void setRGB4(struct ViewPort *vp, UWORD colorIndex, UWORD r, UWORD g, UWORD b) {
    UWORD color = (r << 8) | (g << 4) | b;
    LoadRGB4(vp, &colorIndex, 1);
    SetRGB4(vp, colorIndex, r, g, b);
}

//tried different methods of cleaning up memory, not every one worked for me
void FreePreloadedBitmapsImproved(struct BitMap *bitmaps, int numImages, int width, int height) {
    for (int img = 0; img < numImages; img++) {
         printf("Freeing preloadedBitmaps[%d]\n", img);
        for (int i = 0; i < IMAGEPLANES; i++) {
            if (bitmaps[img].Planes[i]) {
                FreeRaster(bitmaps[img].Planes[i], width, height);
                bitmaps[img].Planes[i] = NULL; // Doppelte Freigabe verhindern
            }
        }
    }
}

//tried different methods of cleaning up memory, not every one worked for me
void FreePreloadedBitmapsImprovedTwo(struct BitMap *bitmaps, int numImages, int width, int height) {
    for (int img = 0; img < numImages; img++) {
         printf("Freeing preloadedBitmaps[%d]\n", img);
        for (int i = 0; i < IMG7_IMAGEPLANES; i++) {
            if (bitmaps[img].Planes[i]) {
                FreeRaster(bitmaps[img].Planes[i], width, height);
                bitmaps[img].Planes[i] = NULL; // Doppelte Freigabe verhindern
            }
        }
    }
}

// tried different methods of cleaning up memory, not every one worked for me
void FreeScreenBitmapImproved(struct BitMap *bitmap) {
    for (int i = 0; i < IMAGEPLANES; i++) {
        printf("Freeing screenBitmap[%d]\n", i);
        if (bitmap->Planes[i]) {
            FreeRaster(bitmap->Planes[i], 240, 240);
            bitmap->Planes[i] = NULL;
        }
    }
}

// scrolls text from right to left with given APen color
void scrollText(char *textToScroll, int APen) {
    int textLength = strlen(textToScroll);
    int scrollStartPos = SCREENWIDTH-10; // Startposition am rechten Rand
    int scrollSpeed = 1; // Geschwindigkeit des Scrollens
    int charSpacing = 8; // Abstand zwischen den Buchstaben (in Pixeln)

    // Für jedes Zeichen sukzessive einfügen
    for (int i = 0; i < textLength; i++) {
        int fillRect = 0;
        for (int k=0; k < charSpacing; k++){
        // Hintergrund löschen
        SetAPen(&screenContext, 0);
        SetBPen(&screenContext, 1);
        if (fillRect == 0){
            scrollStartPos--;
        } 
        if (fillRect == 3){
            fillRect = 0;
        }
        fillRect++;

        // Bisher gezeichnete Zeichen inklusive des aktuellen zeichnen
        SetAPen(&screenContext, APen);
        SetBPen(&screenContext, 0);
        for (int j = 0; j <= i; j++) {
            int currentPos = scrollStartPos + (j * charSpacing);
            Move(&screenContext, currentPos, 220);
            Text(&screenContext, &textToScroll[j], 1); // Nur ein Zeichen zeichnen
        }

        //Delay(2);
        // Scroll-Startposition anpassen
        scrollStartPos -= scrollSpeed;
    }
    }

    
}

// scrolls text from right to left with a given APen color
void scrollTextTwo(char *textToScroll, int APen) {
    int textLength = strlen(textToScroll);
    int scrollStartPos = SCREENWIDTH-10; // Startposition am rechten Rand
    int scrollSpeed = 1; // Geschwindigkeit des Scrollens
    int charSpacing = 8; // Abstand zwischen den Buchstaben (in Pixeln)

    // Für jedes Zeichen sukzessive einfügen
    for (int i = 0; i < textLength; i++) {
        int fillRect = 0;
        for (int k=0; k < charSpacing; k++){
        // Hintergrund löschen
        SetAPen(&screenContextTwo, 0);
        SetBPen(&screenContextTwo, 1);
        if (fillRect == 0){
            //RectFill(&screenContext, 0, 200, SCREENWIDTH - 1, SCREENHEIGHT - 1);
            scrollStartPos--;
        } 
        if (fillRect == 3){
            fillRect = 0;
        }
        fillRect++;

        // Bisher gezeichnete Zeichen inklusive des aktuellen zeichnen
        SetAPen(&screenContextTwo, APen);
        SetBPen(&screenContextTwo, 0);
        for (int j = 0; j <= i; j++) {
            int currentPos = scrollStartPos + (j * charSpacing);
            Move(&screenContextTwo, currentPos, 220);
            Text(&screenContextTwo, &textToScroll[j], 1); // Nur ein Zeichen zeichnen
        }

        //Delay(2);
        // Scroll-Startposition anpassen
        scrollStartPos -= scrollSpeed;
    }
    }

    
}


// print 
void printAsciiArt(){
    printf("||   ||  |||||  ||   ||   ||  |||||  ||||||\n");
    printf("||   ||  ||     ||   ||   ||  ||       ||\n");
    printf("\\\\   //  |||||  ||   \\\\   //  |||||    ||\n");
    printf(" \\\\ //   ||     ||    \\\\ //   ||       ||\n");
    printf("   V     |||||  ||___   V     |||||    ||\n");

    printf("||   ||  ||    |||||   ||   |||||  ||  ||\n");
    printf("||   ||  ||    ||      ||   || ||  ||| ||\n");
    printf("\\\\   //  ||    |||||   ||   || ||  ||||||\n");
    printf(" \\\\ //   ||       ||   ||   || ||  ||  ||\n");
    printf("   V     ||    |||||   ||   |||||  ||  ||\n");    
    printf("=============================================\n");
    printf("Music, Design & Code: Martin Grabsch, 2024\n");
    printf("martin.grabsch@gmail.com\n");
    printf("=============================================\n\n");
 
}


int main() {

    ULONG chipMemAfter = AvailMem(MEMF_CHIP);
    printAsciiArt();
    printf("-->please ensure DISK2 is already in DF1.\n"); 
    printf("-->move mouse pointer to lower right corner for best view\n");
    printf("have some distance to your screen for best optical result\n");
    play_mod("deephouse.mod");


    const char *imageFiles[NUM_IMAGES] = {"scene00001.planes", 
    "scene00005.planes", 
    "scene00009.planes", 
    "scene00013.planes", 
    "scene00017.planes", 
    "scene00021.planes", 
    "scene00025.planes", 
    "scene00029.planes", 
    "scene00033.planes", 
    "scene00037.planes", 
    "scene00041.planes", 
    "scene00045.planes", 
    "scene00049.planes", 
    "scene00053.planes", 
    "scene00057.planes", 
    "scene00061.planes", 
    "scene00065.planes", 
    "scene00069.planes", 
    "scene00073.planes", 
    "scene00081.planes"};

    struct BitMap preloadedBitmaps[NUM_IMAGES];
    FILE *fileHandle;
    int imageWidth = 240, imageHeight = 240;
    const unsigned short paletteScene[16] = {0X111,0X665,0Xaaa,0Xddd};
    const unsigned short paletteSceneNegative[16] = {0xEEE, 0x99A, 0x555, 0x222}; 

    // preload images
    for (int img = 0; img < NUM_IMAGES; img++) {
        // Prozentuale Fortschrittsanzeige
        int progress = (img + 1) * 100 / NUM_IMAGES;
        printf("\rBuffering: %d%%", progress);
        fflush(stdout);
        fileHandle = fopen(imageFiles[img], "rb");
        if (fileHandle == NULL) {
            fprintf(stderr, "Failed to open image file: %s\n", imageFiles[img]);
            exit(1);
        }

        InitBitMap(&preloadedBitmaps[img], IMAGEPLANES, imageWidth, imageHeight);
        for (int i = 0; i < IMAGEPLANES; i++) {
            ULONG rowBytes = (imageWidth + 15) / 16 * 2; // Breite in Bytes (word-aligned)
            preloadedBitmaps[img].Planes[i] = AllocRaster(imageWidth, imageHeight);
            if (preloadedBitmaps[img].Planes[i] == NULL) {
                fprintf(stderr, "Failed to allocate raster memory for image %d plane %d\n", img, i);
                exit(1);
            }
            fread(preloadedBitmaps[img].Planes[i], 1, rowBytes * imageHeight, fileHandle);
        }

        fclose(fileHandle);
    }

    

    // make screen
    InitRastPort(&screenContext);
    screenContext.BitMap = &screenBitMap;

    InitBitMap(&screenBitMap, PLANES, SCREENWIDTH, SCREENHEIGHT);
    for (int i = 0; i < PLANES; i++) {
        screenBitMap.Planes[i] = AllocRaster(SCREENWIDTH, SCREENHEIGHT);
        if (screenBitMap.Planes[i] == NULL) {
            fprintf(stderr, "Failed to allocate screen bitmap raster.\n");
            exit(1);
        }
    }

    screenProperties.CustomBitMap = screenContext.BitMap;

    // open screen
    if ((screen = OpenScreen((void *)(&screenProperties))) == NULL) {
        fprintf(stderr, "Failed to open screen.\n");
        exit(1);
    }
    ShowTitle(screen, FALSE);


    int dx = (SCREENWIDTH - imageWidth) / 2;
    int dy = (SCREENHEIGHT - imageHeight) / 2;

    int doPaint = 0;
    int firstRun = 0;
    // display images one after another to look like an animation
    for (int i = 0; i < 1; i++) {
    for (int img = 0; img < NUM_IMAGES; img++) {
            struct BitMap *currentImage = &preloadedBitmaps[img];

            if (doPaint == 2){
                LoadRGB4(&screen->ViewPort, (UWORD *)paletteScene, 16);
                BltBitMap(currentImage, 0, 0, &screenBitMap, dx, dy, imageWidth, imageHeight, 0xC0, 0x0F, NULL);
                WaitTOF();
                MakeScreen(screen);
                RethinkDisplay();
                if (firstRun == 0){
                    BltBitMap(currentImage, 0, 0, &screenBitMap, dx, dy, imageWidth, imageHeight, 0xC0, 0x0F, NULL);
                    WaitTOF();
                    MakeScreen(screen);
                     RethinkDisplay();
                    Delay(20);

                    firstRun = 1;
                } else {
                    Delay(22);
                }
                
            }
            if (doPaint == 4){
                LoadRGB4(&screen->ViewPort, (UWORD *)paletteSceneNegative, 16);
                BltBitMap(currentImage, 0, 0, &screenBitMap, dx, dy, imageWidth, imageHeight, 0xC0, 0x0F, NULL);
                doPaint = 0;
                WaitTOF();
                MakeScreen(screen);
                RethinkDisplay();
                Delay(22);
                
            }
            WaitTOF();
            MakeScreen(screen);
            RethinkDisplay();
            //Delay(1);
            doPaint++;
            
        }
    }

    for (int i = 0; i < 2; i++) {
        for (int img = 0; img < NUM_IMAGES; img++) {
                struct BitMap *currentImage = &preloadedBitmaps[img];

                LoadRGB4(&screen->ViewPort, (UWORD *)paletteScene, 16);
                BltBitMap(currentImage, 0, 0, &screenBitMap, dx, dy, imageWidth, imageHeight, 0xC0, 0x0F, NULL); 
                WaitTOF();
                MakeScreen(screen);
                RethinkDisplay();
                if (i==0){
                    Delay(7);
                } else {
                    Delay(2);
                }
                
        }
    
        
        LoadRGB4(&screen->ViewPort, (UWORD *)paletteSceneNegative, 16);
        for (int img = (NUM_IMAGES-1); img > 0; img--) {
            struct BitMap *currentImage = &preloadedBitmaps[img];

            BltBitMap(currentImage, 0, 0, &screenBitMap, dx, dy, imageWidth, imageHeight, 0xC0, 0x0F, NULL);

            WaitTOF();
            MakeScreen(screen);
            RethinkDisplay();
            if (i==0){
                Delay(7);
                } else {
                    Delay(2);
                }
        }
    }

    for (int i = 0; i < 1; i++) {
        LoadRGB4(&screen->ViewPort, (UWORD *)paletteScene, 16);
        
        for (int img = 0; img < NUM_IMAGES; img++) {
            struct BitMap *currentImage = &preloadedBitmaps[img];

            BltBitMap(currentImage, 0, 0, &screenBitMap, dx, dy, imageWidth, imageHeight, 0xC0, 0x0F, NULL);

            WaitTOF();
            MakeScreen(screen);
            RethinkDisplay();
            //Delay(1);
        }
        
        LoadRGB4(&screen->ViewPort, (UWORD *)paletteScene, 16);
        for (int img = (NUM_IMAGES-1); img > 0; img--) {
            struct BitMap *currentImage = &preloadedBitmaps[img];

            BltBitMap(currentImage, 0, 0, &screenBitMap, dx, dy, imageWidth, imageHeight, 0xC0, 0x0F, NULL);

            WaitTOF();
            MakeScreen(screen);
            RethinkDisplay();
            //Delay(1);
        }
    }

    for (int img = 0; img < NUM_IMAGES; img++) {
            struct BitMap *currentImage = &preloadedBitmaps[img];

            BltBitMap(currentImage, 0, 0, &screenBitMap, dx, dy, imageWidth, imageHeight, 0xC0, 0x0F, NULL);

            WaitTOF();
            MakeScreen(screen);
            RethinkDisplay();
            //Delay(1);
        }

        struct BitMap *currentImage = &preloadedBitmaps[19];

         for (int k=0; k<300; k++){
            dx++;
                        BltBitMap(currentImage, 0, 0, &screenBitMap, dx, dy, imageWidth, imageHeight, 0xC0, 0x0F, NULL);
                        WaitTOF();
                        //Delay(1);

                     }

        dx++;
        BltBitMap(currentImage, 0, 0, &screenBitMap, dx, dy, imageWidth, imageHeight, 0xC0, 0x0F, NULL);
        WaitTOF();
        Delay(1);
        dx++;
        BltBitMap(currentImage, 0, 0, &screenBitMap, dx, dy, imageWidth, imageHeight, 0xC0, 0x0F, NULL);
        WaitTOF();
        Delay(2);
        dx++;
        BltBitMap(currentImage, 0, 0, &screenBitMap, dx, dy, imageWidth, imageHeight, 0xC0, 0x0F, NULL);
        WaitTOF();
        Delay(3);
        dx++;
        BltBitMap(currentImage, 0, 0, &screenBitMap, dx, dy, imageWidth, imageHeight, 0xC0, 0x0F, NULL);
        WaitTOF();
        Delay(4);
         dx++;
        BltBitMap(currentImage, 0, 0, &screenBitMap, dx, dy, imageWidth, imageHeight, 0xC0, 0x0F, NULL);
        WaitTOF();
        Delay(5);


 
Delay(20);
 
SetAPen(&screenContext, 0);
 SetBPen(&screenContext, 1);
 RectFill(&screenContext, 0, 200, SCREENWIDTH - 1, SCREENHEIGHT - 1);
SetAPen(&screenContext, 2);
scrollText("Buffering...",2);
ULONG chipMemBefore = AvailMem(MEMF_CHIP);
printf("Freier Speicher vor: %lu, nach: %lu\n", chipMemAfter, chipMemAfter);      
chipMemAfter = AvailMem(MEMF_CHIP);
printf("Freier Speicher vor: %lu, nach: %lu\n", chipMemBefore, chipMemAfter);

 playAnimationThree(screen, &preloadedBitmaps);
 SetAPen(&screenContext, 0);
 SetBPen(&screenContext, 1);
 RectFill(&screenContext, 0, 200, SCREENWIDTH - 1, SCREENHEIGHT - 1);
 SetAPen(&screenContext, 2);
 scrollText("Buffering...",2);
 playAnimationFive(screen, &preloadedBitmaps);

 SetAPen(&screenContext, 0);
 SetBPen(&screenContext, 1);
 RectFill(&screenContext, 0, 200, SCREENWIDTH - 1, SCREENHEIGHT - 1);
 SetAPen(&screenContext, 1);
 scrollText("Buffering...",2);
 playAnimationLogo(screen, &preloadedBitmaps);

 // dump memory in order to change display mode to 16 colors
 FreePreloadedBitmapsImproved(&preloadedBitmaps, IMG5_NUM_IMAGES, 240,240);

 SetAPen(&screenContext, 0);
 SetBPen(&screenContext, 1);
 RectFill(&screenContext, 0, 200, SCREENWIDTH - 1, SCREENHEIGHT - 1);
 scrollText("Buffering...",2);
 // play animation with 16 colors
 playAnimationSeven(screen, &preloadedBitmaps);

    

// Credits:
 SetAPen(&screenContextTwo, 0);
 SetBPen(&screenContextTwo, 1);
 RectFill(&screenContextTwo, 0, 200, SCREENWIDTH - 1, SCREENHEIGHT - 1);


 scrollTextTwo("demo is coded in C...",4);
 Delay(25);
 SetAPen(&screenContextTwo, 0);
 SetBPen(&screenContextTwo, 1);
 RectFill(&screenContextTwo, 0, 200, SCREENWIDTH - 1, SCREENHEIGHT - 1);
 scrollTextTwo("using 80 bitmaps",5);
 Delay(25);
 SetAPen(&screenContextTwo, 0);
 SetBPen(&screenContextTwo, 1);
 RectFill(&screenContextTwo, 0, 200, SCREENWIDTH - 1, SCREENHEIGHT - 1);
 scrollTextTwo("1,6mb of images...",5);
 Delay(25);
 SetAPen(&screenContextTwo, 0);
 SetBPen(&screenContextTwo, 1);
 RectFill(&screenContextTwo, 0, 200, SCREENWIDTH - 1, SCREENHEIGHT - 1);
 scrollTextTwo("100k of music...",5);
 Delay(25);
 SetAPen(&screenContextTwo, 0);
 SetBPen(&screenContextTwo, 1);
 RectFill(&screenContextTwo, 0, 200, SCREENWIDTH - 1, SCREENHEIGHT - 1);
 scrollTextTwo("music (C) 2024 M. Grabsch",6);
 Delay(50);
 pause();
 SetAPen(&screenContextTwo, 0);
 SetBPen(&screenContextTwo, 1);
 RectFill(&screenContextTwo, 0, 200, SCREENWIDTH - 1, SCREENHEIGHT - 1);
 scrollTextTwo("female models are AI generated art",7);
 Delay(50);
SetAPen(&screenContextTwo, 0);
 SetBPen(&screenContextTwo, 1);
 RectFill(&screenContextTwo, 0, 200, SCREENWIDTH - 1, SCREENHEIGHT - 1);
 scrollTextTwo("so pls dont ask phone numbers ;)",8);
 Delay(50);
SetAPen(&screenContextTwo, 0);
 SetBPen(&screenContextTwo, 1);
 RectFill(&screenContextTwo, 0, 200, SCREENWIDTH - 1, SCREENHEIGHT - 1);
 scrollTextTwo("special thx to:   ",9);
 Delay(50);
 SetAPen(&screenContextTwo, 0);
 SetBPen(&screenContextTwo, 1);
 RectFill(&screenContextTwo, 0, 200, SCREENWIDTH - 1, SCREENHEIGHT - 1);
 scrollTextTwo("Wei-ju Wu for his VBCC tutorials",5);
 Delay(50);
 SetAPen(&screenContextTwo, 0);
 SetBPen(&screenContextTwo, 1);
 RectFill(&screenContextTwo, 0, 200, SCREENWIDTH - 1, SCREENHEIGHT - 1);
 scrollTextTwo("Steffest for DPaint JS editor",7);
 Delay(50);
SetAPen(&screenContextTwo, 0);
 SetBPen(&screenContextTwo, 1);
 RectFill(&screenContextTwo, 0, 200, SCREENWIDTH - 1, SCREENHEIGHT - 1);
 scrollTextTwo("this demo should prove... ",10);
 Delay(50);
 SetAPen(&screenContextTwo, 0);
 SetBPen(&screenContextTwo, 1);
 RectFill(&screenContextTwo, 0, 200, SCREENWIDTH - 1, SCREENHEIGHT - 1);
 scrollTextTwo("that the A500 was capable",11);
 Delay(50);
 SetAPen(&screenContextTwo, 0);
 SetBPen(&screenContextTwo, 1);
 RectFill(&screenContextTwo, 0, 200, SCREENWIDTH - 1, SCREENHEIGHT - 1);
 scrollTextTwo("of full screen vid with high FPS",12);
 Delay(50);
 SetAPen(&screenContextTwo, 0);
 SetBPen(&screenContextTwo, 1);
 RectFill(&screenContextTwo, 0, 200, SCREENWIDTH - 1, SCREENHEIGHT - 1);
 scrollTextTwo("by only using fast chip memory",12);
 Delay(50);
 SetAPen(&screenContextTwo, 0);
 SetBPen(&screenContextTwo, 1);
 RectFill(&screenContextTwo, 0, 200, SCREENWIDTH - 1, SCREENHEIGHT - 1);
 scrollTextTwo("for info, contact me:",15);
 Delay(50);
 SetAPen(&screenContextTwo, 0);
 SetBPen(&screenContextTwo, 1);
 RectFill(&screenContextTwo, 0, 200, SCREENWIDTH - 1, SCREENHEIGHT - 1);
 scrollTextTwo("martin.grabsch@gmail.com",15);
 Delay(50);
 SetAPen(&screenContextTwo, 0);
 SetBPen(&screenContextTwo, 1);
 RectFill(&screenContextTwo, 0, 200, SCREENWIDTH - 1, SCREENHEIGHT - 1);
 scrollTextTwo("or visit:",15);
 Delay(50);
 SetAPen(&screenContextTwo, 0);
 SetBPen(&screenContextTwo, 1);
 RectFill(&screenContextTwo, 0, 200, SCREENWIDTH - 1, SCREENHEIGHT - 1);
 scrollTextTwo("ljupkamartin.blogspot.com",15);
 Delay(50);
 SetAPen(&screenContextTwo, 0);
 SetBPen(&screenContextTwo, 1);
 RectFill(&screenContextTwo, 0, 200, SCREENWIDTH - 1, SCREENHEIGHT - 1);
 scrollTextTwo("Thx for watching!",15);
 Delay(100);
 SetAPen(&screenContextTwo, 0);
 SetBPen(&screenContextTwo, 1);
 RectFill(&screenContextTwo, 0, 200, SCREENWIDTH - 1, SCREENHEIGHT - 1);
 scrollTextTwo("special greetings to 'Commo'",15);
 Delay(100);
 printf("Thank You for watching!\n");
    if (screen) { 
        CloseScreen(screen);
        screen = NULL; 
    }

    return 0;
}




int playAnimationThree(struct Screen *screen, struct BitMap *preloadedBitmaps) {

    const char *imageFiles[IMG2_NUM_IMAGES] = {
       "gcene00021.planes", "gcene00025.planes", "gcene00029.planes", "gcene00033.planes", "gcene00037.planes", "gcene00041.planes", "gcene00047.planes", "gcene00051.planes", "gcene00055.planes", "gcene00067.planes", "gcene00071.planes", "gcene00075.planes", "gcene00079.planes", "gcene00083.planes", "gcene00087.planes", "gcene00089.planes", "gcene00091.planes", "gcene00093.planes", "gcene00095.planes"
    };

    FILE *fileHandle;
    int imageWidth = 240, imageHeight = 240;
    const unsigned short paletteScene[16] = {0X111,0X555,0X999,0Xfff};
    const unsigned short paletteSceneNegative[16] = {0xEEE, 0xAAA, 0x667, 0x000}; 

    // Überschreiben der vorhandenen Speicherbereiche mit neuen Bilddaten
    for (int img = 0; img < IMG2_NUM_IMAGES; img++) {
        fileHandle = fopen(imageFiles[img], "rb");
        if (fileHandle == NULL) {
            fprintf(stderr, "Failed to open image file: %s\n", imageFiles[img]);
            exit(1);
        }

        InitBitMap(&preloadedBitmaps[img], IMAGEPLANES, imageWidth, imageHeight);
        for (int i = 0; i < IMG2_IMAGEPLANES; i++) {
            ULONG rowBytes = (imageWidth + 15) / 16 * 2; // Breite in Bytes (word-aligned)
            if (preloadedBitmaps[img].Planes[i] == NULL) {
                fprintf(stderr, "Failed to allocate raster memory for image %d plane %d\n", img, i);
                exit(1);
            }
            fread(preloadedBitmaps[img].Planes[i], 1, rowBytes * imageHeight, fileHandle);
        }

        
        

        fclose(fileHandle);
    }

    SetAPen(&screenContext, 0);
    SetBPen(&screenContext, 1);
    RectFill(&screenContext, 0, 0, SCREENWIDTH - 1, SCREENHEIGHT - 1);

    // Animation starten
    LoadRGB4(&screen->ViewPort, (UWORD *)paletteScene, 16);

    //struct ViewPort *vp = &screen->ViewPort;
    //vp->RasInfo->BitMap = &screenBitMap;
    MakeScreen(screen);
    RethinkDisplay();

    int dx = (SCREENWIDTH - imageWidth) / 2;
    int dy = (SCREENHEIGHT - imageHeight) / 2;

    int palletteCounter = 0;

    for (int i = 0; i < 2; i++) {
        LoadRGB4(&screen->ViewPort, (UWORD *)paletteScene, 16);
        for (int img = 0; img < IMG2_NUM_IMAGES; img++) {
            if (palletteCounter <= 2){
                LoadRGB4(&screen->ViewPort, (UWORD *)paletteScene, 16);
            } 
            struct BitMap *currentImage = &preloadedBitmaps[img];

            BltBitMap(currentImage, 0, 0, &screenBitMap, dx, dy, imageWidth, imageHeight, 0xC0, 0x0F, NULL);

            WaitTOF();
            if (img==7){
                Delay(25);
            }
            Delay(10);
            palletteCounter++;
            if (palletteCounter >= 5){
                palletteCounter = 0;
            }
        }
        Delay(25);

        for (int img = (IMG2_NUM_IMAGES - 1); img > 0; img--) {
            if (palletteCounter <= 2){
                LoadRGB4(&screen->ViewPort, (UWORD *)paletteScene, 16);
            } else {
                LoadRGB4(&screen->ViewPort, (UWORD *)paletteSceneNegative, 16);
            }
            struct BitMap *currentImage = &preloadedBitmaps[img];

            BltBitMap(currentImage, 0, 0, &screenBitMap, dx, dy, imageWidth, imageHeight, 0xC0, 0x0F, NULL);

            WaitTOF();
            if (img==7){
                Delay(25);
            }
            Delay(7);
            palletteCounter++;
            if (palletteCounter >= 10){
                palletteCounter = 0;
            }
        }
        //Delay(25);
    }

    LoadRGB4(&screen->ViewPort, (UWORD *)paletteScene, 16);
        for (int img = 0; img < IMG2_NUM_IMAGES; img++) {
            struct BitMap *currentImage = &preloadedBitmaps[img];

            BltBitMap(currentImage, 0, 0, &screenBitMap, dx, dy, imageWidth, imageHeight, 0xC0, 0x0F, NULL);

            WaitTOF();
            if (img==7){
                Delay(15);
            }
            Delay(5);
        }
        for (int img = (IMG2_NUM_IMAGES - 1); img > 0; img--) {
            
            struct BitMap *currentImage = &preloadedBitmaps[img];

            BltBitMap(currentImage, 0, 0, &screenBitMap, dx, dy, imageWidth, imageHeight, 0xC0, 0x0F, NULL);

            WaitTOF();
            Delay(5);
            
            
        }
        for (int img = 0; img < IMG2_NUM_IMAGES; img++) {
            struct BitMap *currentImage = &preloadedBitmaps[img];

            BltBitMap(currentImage, 0, 0, &screenBitMap, dx, dy, imageWidth, imageHeight, 0xC0, 0x0F, NULL);

            WaitTOF();
            Delay(2);
        }


    return 0;
}

int playAnimationFive(struct Screen *screen, struct BitMap *preloadedBitmaps) {
    const char *imageFilesDf1[IMG5_NUM_IMAGES] = {
        "DF1:scene00001.planes", 
        "DF1:scene00005.planes", 
        "DF1:scene00009.planes", 
        "DF1:scene00013.planes", 
        "DF1:scene00017.planes", 
        "DF1:scene00021.planes", 
        "DF1:scene00025.planes",
        "DF1:scene00029.planes",
        "DF1:scene00033.planes",
        "DF1:scene00037.planes",
        "DF1:scene00041.planes",
        "DF1:scene00045.planes",
        "DF1:scene00049.planes",
        "DF1:scene00053.planes",
        "DF1:scene00057.planes",
        "DF1:scene00061.planes",
        "DF1:scene00065.planes",
        "DF1:scene00069.planes",
        "DF1:scene00073.planes",
        "DF1:scene00077.planes"}; 

    FILE *fileHandle;
    int imageWidth = 240, imageHeight = 240;
    const unsigned short paletteScene[16] = {0X333,0X665,0Xaaa,0Xddd};
    const unsigned short paletteSceneNegative[16] = {0xEEE, 0x99A, 0x555, 0x222}; 
    const unsigned short paletteSceneWithColor[16] = {0X257,0X743,0X4ac,0Xca7};

    // Überschreiben der vorhandenen Speicherbereiche mit neuen Bilddaten
    for (int img = 0; img < IMG5_NUM_IMAGES; img++) {
        fileHandle = fopen(imageFilesDf1[img], "rb");
        if (fileHandle == NULL) {
            fprintf(stderr, "Failed to open image file: %s\n", imageFilesDf1[img]);
            exit(1);
        }

        InitBitMap(&preloadedBitmaps[img], IMAGEPLANES, imageWidth, imageHeight);
        for (int i = 0; i < IMG5_IMAGEPLANES; i++) {
            ULONG rowBytes = (imageWidth + 15) / 16 * 2; // Breite in Bytes (word-aligned)
            if (preloadedBitmaps[img].Planes[i] == NULL) {
                fprintf(stderr, "Failed to allocate raster memory for image %d plane %d\n", img, i);
                exit(1);
            }
            fread(preloadedBitmaps[img].Planes[i], 1, rowBytes * imageHeight, fileHandle);
        }

        
        

        fclose(fileHandle);
    }

    // Animation starten
    LoadRGB4(&screen->ViewPort, (UWORD *)paletteScene, 16);

    //struct ViewPort *vp = &screen->ViewPort;
    //vp->RasInfo->BitMap = &screenBitMap;
    MakeScreen(screen);
    RethinkDisplay();

    int dx = (SCREENWIDTH - imageWidth) / 2;
    int dy = (SCREENHEIGHT - imageHeight) / 2;


    for (int i = 1; i < 3; i++) {
        LoadRGB4(&screen->ViewPort, (UWORD *)paletteScene, 16);
        for (int img = 0; img < IMG5_NUM_IMAGES; img++) {
            struct BitMap *currentImage = &preloadedBitmaps[img];

            BltBitMap(currentImage, 0, 0, &screenBitMap, dx, dy, imageWidth, imageHeight, 0xC0, 0x0F, NULL);

            WaitTOF();
            if (i < 2){
            Delay(5);
            } else {
               Delay(2);
            }

        }

        for (int img = (IMG5_NUM_IMAGES - 1); img > 0; img--) {
            struct BitMap *currentImage = &preloadedBitmaps[img];

            BltBitMap(currentImage, 0, 0, &screenBitMap, dx, dy, imageWidth, imageHeight, 0xC0, 0x0F, NULL);

            WaitTOF();
            if (i < 2){
            Delay(5);
            } else {
               Delay(2);
            }
            
 
        }
    }

    for (int i = 0; i < 2; i++) {
        LoadRGB4(&screen->ViewPort, (UWORD *)paletteScene, 16);
        for (int img = 0; img < IMG5_NUM_IMAGES; img++) {
            struct BitMap *currentImage = &preloadedBitmaps[img];
            BltBitMap(currentImage, 0, 0, &screenBitMap, dx, dy, imageWidth, imageHeight, 0xC0, 0x0F, NULL);
            WaitTOF();
        }

        for (int img = (IMG5_NUM_IMAGES - 1); img > 0; img--) {
            struct BitMap *currentImage = &preloadedBitmaps[img];
            BltBitMap(currentImage, 0, 0, &screenBitMap, dx, dy, imageWidth, imageHeight, 0xC0, 0x0F, NULL);
            WaitTOF();
        }
    }


    for (int i = 0; i < 1; i++) {
        LoadRGB4(&screen->ViewPort, (UWORD *)paletteSceneWithColor, 16);
        for (int img = 0; img < IMG5_NUM_IMAGES; img++) {
            struct BitMap *currentImage = &preloadedBitmaps[img];
            BltBitMap(currentImage, 0, 0, &screenBitMap, dx, dy, imageWidth, imageHeight, 0xC0, 0x0F, NULL);
            WaitTOF();
            Delay(2);
        }

        for (int img = (IMG5_NUM_IMAGES - 1); img > 0; img--) {
            struct BitMap *currentImage = &preloadedBitmaps[img];
            BltBitMap(currentImage, 0, 0, &screenBitMap, dx, dy, imageWidth, imageHeight, 0xC0, 0x0F, NULL);
            WaitTOF();
            Delay(2);
        }
    }
    LoadRGB4(&screen->ViewPort, (UWORD *)paletteScene, 16);
    printf("Alle Ressourcen wurden erfolgreich wiederverwendet.\n");
    return 0;
}

int playAnimationSeven(struct Screen *screen) {
    const char *imageFilesDf1[IMG7_NUM_IMAGES] = {
        "DF1:girlquality00079farbe.planes",
        "DF1:girlquality00080.planes",
        "DF1:girlquality00083.planes",
        "DF1:girlquality00085.planes",
        "DF1:girlquality00087.planes",
        "DF1:girlquality00089.planes",
        "DF1:girlquality00091.planes",
        "DF1:girlquality00093.planes",
        "DF1:velvet_vision.planes"}; 

    struct BitMap preloadedBitmaps[IMG7_NUM_IMAGES];
    FILE *fileHandle;
    int imageWidth = 240, imageHeight = 240;
    const unsigned short paletteSceneGraustufen[16] = {0X000,0X111,0X333,0X333,0X555,0X555,0X666,0X777,0X999,0X999,0Xbbb,0Xccc,0Xccc,0Xddd,0Xfff,0Xfff};
    const unsigned short paletteSceneGirlColor[16] = {0X000,0X432,0X433,0X544,0X256,0X643,0X367,0X864,0X478,0X766,0X987,0X79a,0Xc96,0Xabb,0Xdb9,0Xfff};
    const unsigned short paletteSceneGirlPos[16] = {0X000,0X112,0X323,0X422,0X511,0X168,0X942,0X854,0X665,0X279,0X59b,0X998,0Xc96,0Xe95,0Xfb7,0Xfc9};
    const unsigned short paletteSceneVelvet[16] = {0X544,0X733,0X568,0X954,0Xc53,0Xb75,0Xc86,0Xd96,0Xf85,0Xbaa,0Xea7,0Xfa6,0Xfc8,0Xfd9,0Xfea,0Xfff};

    // Überschreiben der vorhandenen Speicherbereiche mit neuen Bilddaten
    for (int img = 0; img < IMG7_NUM_IMAGES; img++) {
        fileHandle = fopen(imageFilesDf1[img], "rb");
        if (fileHandle == NULL) {
            fprintf(stderr, "Failed to open image file: %s\n", imageFilesDf1[img]);
            exit(1);
        }

        InitBitMap(&preloadedBitmaps[img], IMG7_IMAGEPLANES, imageWidth, imageHeight);
        for (int i = 0; i < IMG7_IMAGEPLANES; i++) {
            ULONG rowBytes = (imageWidth + 15) / 16 * 2; // Für 32-Farben-Paletten
            preloadedBitmaps[img].Planes[i] = AllocRaster(imageWidth, imageHeight);
            if (preloadedBitmaps[img].Planes[i] == NULL) {
                fprintf(stderr, "Failed to allocate raster memory for image %d plane %d\n", img, i);
                exit(1);
            }
            fread(preloadedBitmaps[img].Planes[i], 1, rowBytes * imageHeight, fileHandle);
        }
        fclose(fileHandle);
    }

    // Bildschirm vorbereiten
    InitRastPort(&screenContextTwo);
    screenContextTwo.BitMap = &screenBitMapTwo;

    InitBitMap(&screenBitMapTwo, IMG7_IMAGEPLANES, SCREENWIDTH, SCREENHEIGHT);
    for (int i = 0; i < IMG7_IMAGEPLANES; i++) {
        screenBitMapTwo.Planes[i] = AllocRaster(SCREENWIDTH, SCREENHEIGHT);
        if (screenBitMapTwo.Planes[i] == NULL) {
            fprintf(stderr, "Failed to allocate screen bitmap raster.\n");
            exit(1);
        }
    }

    screenProperties.CustomBitMap = screenContextTwo.BitMap;

    // Animation starten
    
    struct ViewPort *vp = &screen->ViewPort;
    vp->RasInfo->BitMap = &screenBitMapTwo;
    MakeScreen(screen);
    RethinkDisplay();

    int dx = (SCREENWIDTH - imageWidth) / 2;
    int dy = (SCREENHEIGHT - imageHeight) / 2;


    SetAPen(&screenContextTwo, 0);
    SetBPen(&screenContextTwo, 1);
    RectFill(&screenContextTwo, 0, 0, SCREENWIDTH - 1, SCREENHEIGHT - 1);

    int firstRun=0;
    //LoadRGB4(&screen->ViewPort, (UWORD *)paletteSceneGirlColor, 16);
    for (int i = 1; i < 7; i++) {
        if (i>=1 && i < 4){
            LoadRGB4(&screen->ViewPort, (UWORD *)paletteSceneGraustufen, 16);
        }
        if (i>=4){
            LoadRGB4(&screen->ViewPort, (UWORD *)paletteSceneVelvet, 16);
        }
       
        for (int img = 1; img < 8; img++) {
            struct BitMap *currentImage = &preloadedBitmaps[img];

            BltBitMap(currentImage, 0, 0, &screenBitMapTwo, dx, dy, imageWidth, imageHeight, 0xC0, 0x0F, NULL);

            WaitTOF();
             MakeScreen(screen);
             RethinkDisplay();
             if (firstRun==0){
                Delay(100);
                firstRun=1;
                SetAPen(&screenContextTwo, 0);
                SetBPen(&screenContextTwo, 1);
                RectFill(&screenContextTwo, 0, 200, SCREENWIDTH - 1, SCREENHEIGHT - 1);
                scrollTextTwo("photorealistic 16color greyscale",15);
                Delay(50);
                SetAPen(&screenContextTwo, 0);
                SetBPen(&screenContextTwo, 1);
                RectFill(&screenContextTwo, 0, 200, SCREENWIDTH - 1, SCREENHEIGHT - 1);
             }

            if (i < 3 || i >4){
            Delay(5);
            } else {
                Delay(2);
            }

        }
        Delay(25);
        
        

        for (int img = 7; img > 1; img--) {
            struct BitMap *currentImage = &preloadedBitmaps[img];
            BltBitMap(currentImage, 0, 0, &screenBitMapTwo, dx, dy, imageWidth, imageHeight, 0xC0, 0x0F, NULL);
            WaitTOF();
            MakeScreen(screen);
            RethinkDisplay();
            Delay(5);
        }
    }
    

    Delay(100);

    LoadRGB4(&screen->ViewPort, (UWORD *)paletteSceneGraustufen, 16);

    for (int i=0; i<50; i++){
    for (int img = 5; img <7; img++) {
            struct BitMap *currentImage = &preloadedBitmaps[img];
            BltBitMap(currentImage, 0, 0, &screenBitMapTwo, dx, dy, imageWidth, imageHeight, 0xC0, 0x0F, NULL);
            WaitTOF();
            //MakeScreen(screen);
            //RethinkDisplay();
            Delay(1);
        }
    }

    LoadRGB4(&screen->ViewPort, (UWORD *)paletteSceneGirlColor, 16);
    struct BitMap *currentImage = &preloadedBitmaps[0];

    BltBitMap(currentImage, 0, 0, &screenBitMapTwo, dx, dy, imageWidth, imageHeight, 0xC0, 0x0F, NULL);
    WaitTOF();
    Delay(20);

    //twitching effect
    for (int i=0; i<150; i++){

            BltBitMap(currentImage, 0, 0, &screenBitMapTwo, dx+1, dy, imageWidth, imageHeight, 0xC0, 0x0F, NULL);
            WaitTOF();
            //Delay(1);      
            BltBitMap(currentImage, 0, 0, &screenBitMapTwo, dx-1, dy, imageWidth, imageHeight, 0xC0, 0x0F, NULL);
            WaitTOF();    
            //Delay(1);
}   



    
    LoadRGB4(&screen->ViewPort, (UWORD *)paletteSceneVelvet, 16);
    struct BitMap *currentImageTwo = &preloadedBitmaps[8];
    BltBitMap(currentImageTwo, 0, 0, &screenBitMapTwo, dx, dy, imageWidth, imageHeight, 0xC0, 0x0F, NULL);
    WaitTOF();
    MakeScreen(screen);
    RethinkDisplay();
    Delay(200);
   
    FreePreloadedBitmapsImprovedTwo(&preloadedBitmaps, IMG7_NUM_IMAGES, 240,240);
    printf("Alle Ressourcen wurden erfolgreich wiederverwendet.\n");

    return 0;
}


int playAnimationLogo(struct Screen *screen, struct BitMap *preloadedBitmaps) {

    const char *imageFiles[IMG3_NUM_IMAGES] = {
       "DF1:logo_sequence1.planes", "DF1:logo_sequence2.planes", "DF1:logo_sequence3.planes", "DF1:logo_sequence4.planes", "DF1:logo_sequence5.planes", "DF1:logo_sequence7.planes", "DF1:logo_sequence10.planes"
    };

    FILE *fileHandle;
    int imageWidth = 240, imageHeight = 240;
    const unsigned short paletteScene[16] = {0X000,0X444,0Xaaa,0Xddd};

    for (int img = 0; img < IMG3_NUM_IMAGES; img++) {
        fileHandle = fopen(imageFiles[img], "rb");
        if (fileHandle == NULL) {
            fprintf(stderr, "Failed to open image file: %s\n", imageFiles[img]);
            exit(1);
        }

        InitBitMap(&preloadedBitmaps[img], IMG3_IMAGEPLANES, imageWidth, imageHeight);
        for (int i = 0; i < IMG3_IMAGEPLANES; i++) {
            ULONG rowBytes = (imageWidth + 15) / 16 * 2; // Breite in Bytes (word-aligned)
            if (preloadedBitmaps[img].Planes[i] == NULL) {
                fprintf(stderr, "Failed to allocate raster memory for image %d plane %d\n", img, i);
                exit(1);
            }
            fread(preloadedBitmaps[img].Planes[i], 1, rowBytes * imageHeight, fileHandle);
        }

        
        

        fclose(fileHandle);
    }



    // Animation starten
    LoadRGB4(&screen->ViewPort, (UWORD *)paletteScene, 16);


    MakeScreen(screen);
    RethinkDisplay();

    int dx = (SCREENWIDTH - imageWidth) / 2;
    int dy = (SCREENHEIGHT - imageHeight) / 2;

    LoadRGB4(&screen->ViewPort, (UWORD *)paletteScene, 16);

    for (int i = 0; i < 7; i++) {
        
        for (int img = 0; img < IMG3_NUM_IMAGES; img++) {
            struct BitMap *currentImage = &preloadedBitmaps[img];
            BltBitMap(currentImage, 0, 0, &screenBitMap, dx, dy, imageWidth, imageHeight, 0xC0, 0x0F, NULL);
            WaitTOF();           
        }

    }

    for (int i = 0; i < 2; i++) {
        
        for (int img = 0; img < IMG3_NUM_IMAGES; img++) {
            struct BitMap *currentImage = &preloadedBitmaps[img];

            BltBitMap(currentImage, 0, 0, &screenBitMap, dx, dy, imageWidth, imageHeight, 0xC0, 0x0F, NULL);

            WaitTOF();
            if (i==0){
                Delay(2);
            }
            if (i==1){
            Delay(img);
            }        
           
        }

    }





    

struct BitMap *currentImage = &preloadedBitmaps[0];

BltBitMap(currentImage, 0, 0, &screenBitMap, dx, dy, imageWidth, imageHeight, 0xC0, 0x0F, NULL);
WaitTOF();     
Delay(50);

for (int i=0; i<5; i++){

            BltBitMap(currentImage, 0, 0, &screenBitMap, dx+2, dy, imageWidth, imageHeight, 0xC0, 0x0F, NULL);
            WaitTOF();
             Delay(1);      
            BltBitMap(currentImage, 0, 0, &screenBitMap, dx-2, dy, imageWidth, imageHeight, 0xC0, 0x0F, NULL);
            WaitTOF();    
            Delay(1);   
            BltBitMap(currentImage, 0, 0, &screenBitMap, dx, dy+1, imageWidth, imageHeight, 0xC0, 0x0F, NULL);
            WaitTOF();
             Delay(1);      
            BltBitMap(currentImage, 0, 0, &screenBitMap, dx, dy-1, imageWidth, imageHeight, 0xC0, 0x0F, NULL);
            WaitTOF();    
            Delay(1);     
}   

for (int i=0; i<5; i++){

            BltBitMap(currentImage, 0, 0, &screenBitMap, dx+5, dy, imageWidth, imageHeight, 0xC0, 0x0F, NULL);
            WaitTOF();
             Delay(1);      
            BltBitMap(currentImage, 0, 0, &screenBitMap, dx-5, dy, imageWidth, imageHeight, 0xC0, 0x0F, NULL);
            WaitTOF();    
            Delay(1);   
            BltBitMap(currentImage, 0, 0, &screenBitMap, dx, dy+3, imageWidth, imageHeight, 0xC0, 0x0F, NULL);
            WaitTOF();
             Delay(1);      
            BltBitMap(currentImage, 0, 0, &screenBitMap, dx, dy-3, imageWidth, imageHeight, 0xC0, 0x0F, NULL);
            WaitTOF();    
            Delay(1);     
}   
for (int i=0; i<5; i++){

            BltBitMap(currentImage, 0, 0, &screenBitMap, dx+10, dy, imageWidth, imageHeight, 0xC0, 0x0F, NULL);
            WaitTOF();
             Delay(1);      
            BltBitMap(currentImage, 0, 0, &screenBitMap, dx-10, dy, imageWidth, imageHeight, 0xC0, 0x0F, NULL);
            WaitTOF();    
            Delay(1);   
            BltBitMap(currentImage, 0, 0, &screenBitMap, dx, dy+3, imageWidth, imageHeight, 0xC0, 0x0F, NULL);
            WaitTOF();
             Delay(1);      
            BltBitMap(currentImage, 0, 0, &screenBitMap, dx, dy-3, imageWidth, imageHeight, 0xC0, 0x0F, NULL);
            WaitTOF();    
            Delay(1);     
}   



    //Delay(100);
    int delaycount=0;

    printf("cleaning up.\n");
    return 0;
}