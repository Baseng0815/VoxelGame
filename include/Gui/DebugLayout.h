#pragma once

#include "Layout.h"

class GUI;

class DebugLayout : public Layout {
    private:
        Text *m_textFps, *m_textRenderTime, *m_textChunkCount;

    public:
        DebugLayout(GUI* gui);

        // TODO add a timer that raises an event each second
        void setValues(int fps, int rendertime, int chunkCount);
};
