#ifndef _HGOL_KEYPAD_H_
#define _HGOL_KEYPAD_H_

class Keypad
{
public:
    Keypad();

    // Get index of pressed button.
    // 1, 2, 3 -> 0,1,2
    // 4, 5, 6 -> 3,4,5
    // 7, 8, 9 -> 6,7,8
    // *, 0, # -> 9,10,11
    // Nothing -> -1
    int sample();

private:
    int sampleRow(int rowPin);
    int lastPressMs = 0;
    int lastPress;
};

#endif