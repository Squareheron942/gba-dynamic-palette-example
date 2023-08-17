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

    unsigned short *palette_mem_ref = ((unsigned short*)0x05000000);

    bn::bg_palettes::set_fade_color(bn::color(0x0000));

    bn::regular_bg_ptr bg = bn::regular_bg_items::bg.create_bg(0, 0);
    unsigned char counter[16][3] {{}};
    unsigned char index = 0;
    unsigned char channel = 0;
    bool dark = false;
    while(1)
    {
        if (bn::keypad::start_pressed()) {
            for (unsigned char i = 0; i <= 60; i++) {bn::bg_palettes::set_fade_intensity(dark ? 1 - i / bn::fixed(60) : i / bn::fixed(60));bn::core::update();}
            dark = !dark;
        }
        if (bn::keypad::up_pressed()) counter[index][channel]++;
        if (bn::keypad::down_pressed()) counter[index][channel]--;
        if (bn::keypad::left_pressed()) index--;
        if (bn::keypad::right_pressed()) index++;
        if (bn::keypad::select_pressed()) channel++;
        counter[index][channel] %= 32;
        index %= 16;
        channel %= 3;

        palette_mem_ref[bg.palette().id() * 16 + index] = (counter[index][2] << 10) | (counter[index][1] << 5) | (counter[index][0] & 0x001f);

        bn::core::update();
    }
}