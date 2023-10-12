#ifndef _HGOL_AUDIO_VIEW_H_
#define _HGOL_AUDIO_VIEW_H_

#include "grid.h"

class AudioView : public GridView
{
public:
    AudioView();

    void update(const Grid *grid, float progress);

private:
    bool _enabled = false;
};

#endif