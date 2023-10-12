#include "audio_view.h"

#include <Audio.h>

AudioPlaySdWav playSdWav1;
AudioOutputI2S i2s1;
AudioConnection patchCord1(playSdWav1, 0, i2s1, 0);
AudioConnection patchCord2(playSdWav1, 1, i2s1, 1);
AudioControlSGTL5000 sgtl5000_1;

// Use these with the Teensy Audio Shield
#define SDCARD_CS_PIN 10
#define SDCARD_MOSI_PIN 7 // Teensy 4 ignores this, uses pin 11
#define SDCARD_SCK_PIN 14 // Teensy 4 ignores this, uses pin 13

#define ENDING_FILENAME "ENDING.WAV"

AudioView::AudioView()
{
    AudioMemory(8);
    sgtl5000_1.enable();
    sgtl5000_1.volume(0.625);
    SPI.setMOSI(SDCARD_MOSI_PIN);
    SPI.setSCK(SDCARD_SCK_PIN);
    _enabled = SD.begin(SDCARD_CS_PIN);
}

void AudioView::update(const Grid *grid, float progress)
{
    if (_enabled)
    {
        int eolCount = grid->eolCount();

        if (eolCount == 1)
        {
            playSdWav1.play(ENDING_FILENAME);
        }
    }
}
