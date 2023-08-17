#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_regular_bg_items_bg.h"
#include "bn_regular_bg_ptr.h"
#include "bn_bg_palette_ptr.h"
#include "bn_bg_palettes_actions.h"
#include "bn_fixed.h"
#include "bn_color.h"

int main()
{
    bn::core::init();

    unsigned short *palette_mem_ref = ((unsigned short*)0x05000000); // bg palette memory address

    bn::bg_palettes::set_fade_color(bn::color(0x0000)); // setup for fading effect, sets the fading colour to black

    bn::regular_bg_ptr bg = bn::regular_bg_items::bg.create_bg(0, 0); // create a bg_ptr which gets the background to display
    unsigned char counter[16][3] {{}};
    unsigned char index = 0;
    unsigned char channel = 0;
    bool dark = false;
    bn::fixed intensity = 0;
    while(1)
    {
        if (bn::keypad::start_pressed()) {
            for (unsigned char i = 0; i <= 60; i++) {
                intensity = i / bn::fixed(60); // increment fade intensity over time
                bn::bg_palettes::set_fade_intensity(
                    dark ? 1 - intensity : // if the screen is already dark, then to fade back the intensity should be inverted
                    intensity
                );
                bn::core::update(); // refresh the screen each time the fade intensity is incremented
            }
            dark = !dark;
        }
        if (bn::keypad::up_pressed()) counter[index][channel]++; // increment the brightness of the selected channel for the selected palette entry
        if (bn::keypad::down_pressed()) counter[index][channel]--; // decrement the brightnes
        if (bn::keypad::left_pressed()) index--; // change the selected palette entry
        if (bn::keypad::right_pressed()) index++;
        if (bn::keypad::select_pressed()) channel++; // change the selected channel
        counter[index][channel] &= 0x1F; // caps the palette value at 31 
        index &= 0xF; // same thing here but to 15
        channel %= 3;

        /**
         * GBA colours are formatted as 15-bit BGR
         * This sets the colour palette value at the selected index (taking into account the location of the specific palette) to the corresponding value in the counter
        */
        palette_mem_ref[bg.palette().id() * 16 + index] = (counter[index][2] << 10) | (counter[index][1] << 5) | (counter[index][0] & 0x001f); 

        bn::core::update(); // update the screen
    }
}