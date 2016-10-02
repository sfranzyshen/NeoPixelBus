// NeoBitmapFileSPIFFS
// This example will animate pixels using a bitmap stored on a SPIFFS filesystem
// 
//
// This will demonstrate the use of the NeoBitmapFile object 
// NOTE:  The images provided in the example directory should be copied to
// the root of the SPIFFS filesystem so the below code will find it.
// NOTE:  This sample and the included images were built for a 16 pixel ring so
// running this with a smaller size may not look as interesting.  Try providing
// your own 24 bit bitmap for better results.

#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>
#include <FS.h>

typedef NeoGrbFeature MyPixelColorFeature;
//typedef NeoGrbwFeature MyPixelColorFeature;

const uint16_t PixelCount = 16; // the sample images are meant for 144 pixels
const uint16_t PixelPin = 2;
const uint16_t AnimCount = 1; // we only need one

NeoPixelBus<MyPixelColorFeature, NeoEsp8266Uart800KbpsMethod> strip(PixelCount, PixelPin);
// for esp8266 omit the pin
//NeoPixelBus<MyPixelColorFeature, Neo800KbpsMethod> strip(PixelCount);
NeoPixelAnimator animations(AnimCount); // NeoPixel animation management object

// our NeoBitmapFile will use the same color feature as NeoPixelBus and
// we want it to use the File object 
NeoBitmapFile<MyPixelColorFeature, File> image;

uint16_t animState = 0;
uint16_t animSpeed = 50;//change the animation speed playback
bool animDir = true; //change this to reverse the animation direction (flips image horizontally)
//RgbColor animMask = RgbColor(128, 255, 0); //change this to mask a color over grey scale images (to tint them) 
RgbColor animMask = NULL;//or leave it NULL

void LoopAnimUpdate(const AnimationParam& param)
{
    // wait for this animation to complete,
    // we are using it as a timer of sorts
    if (param.state == AnimationState_Completed)
    {
        // done, time to restart this position tracking animation/timer
        animations.RestartAnimation(param.index, animSpeed);

        // draw the complete row at animState to the complete strip
        image.Blt(strip, 0, 0, animState, image.Width(), animDir, animMask);
        animState = (animState + 1) % image.Height(); // increment and wrap
    }
}

void setup() {
    Serial.begin(115200);
    while (!Serial); // wait for serial attach

    strip.Begin();
    strip.Show();
    Serial.println("");
    Serial.print("Initializing SPIFFS filesystem...");

    // see if the filesystem is present and can be initialized:
    if (!SPIFFS.begin())
    {
        Serial.println("SPIFFS failed, or not present");
        // don't do anything more:
        return;
    }
    Serial.println("SPIFFS initialized.");

    // open the file
    //File bitmapFile = SPIFFS.open("/wild_comet_16.bmp", "r"); 
    //File bitmapFile = SPIFFS.open("/blue_comet_16.bmp", "r"); 
    //File bitmapFile = SPIFFS.open("/blue_sparkle_16.bmp", "r"); 
    //File bitmapFile = SPIFFS.open("/blue_wipe_16.bmp", "r"); 
    //File bitmapFile = SPIFFS.open("/dual_comet_16.bmp", "r"); 
    //File bitmapFile = SPIFFS.open("/orange_comet_16.bmp", "r"); 
    //File bitmapFile = SPIFFS.open("/rainbow_16.bmp", "r"); 
    //File bitmapFile = SPIFFS.open("/rainbow_chase_16.bmp", "r"); 
    //File bitmapFile = SPIFFS.open("/rainbow_wave_16.bmp", "r"); 
    //File bitmapFile = SPIFFS.open("/red_comet_16.bmp", "r"); 
    File bitmapFile = SPIFFS.open("/strandtest_16.bmp", "r"); 
    //File bitmapFile = SPIFFS.open("/white_chase_16.bmp", "r"); 
    //File bitmapFile = SPIFFS.open("/white_comet_16.bmp", "r"); 
    //File bitmapFile = SPIFFS.open("/white_sparkle_16.bmp", "r"); 
    //File bitmapFile = SPIFFS.open("/rainbow_long_16.bmp", "r");
    if (!bitmapFile)
    {
        Serial.println("File open fail, or not present");
        // don't do anything more:
        return;
    }

    // initialize the image with the file
    if (!image.Begin(bitmapFile))
    {
        Serial.println("File format fail, not a supported bitmap");
        // don't do anything more:
        return;
    }

    //animState = 0;
    // we use the index 0 animation to time how often we rotate all the pixels
    animations.StartAnimation(0, animSpeed, LoopAnimUpdate);
}

void loop() {
    // this is all that is needed to keep it running
    // and avoiding using delay() is always a good thing for
    // any timing related routines
    animations.UpdateAnimations();
    strip.Show();
}
