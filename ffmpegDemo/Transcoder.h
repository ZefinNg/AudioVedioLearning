#ifndef TRANSCODER_H
#define TRANSCODER_H

#include <string>

extern "C" {
    #include "libavcodec/avcodec.h"
    #include "libavformat/avformat.h"
    #include "libavutil/avutil.h"
    #include "libavutil/log.h"
    #include "libavutil/timestamp.h"
}

class Transcoder
{
public:
    Transcoder();
    ~Transcoder();

    int MP42Flv(const std::string& mp4Path, const std::string& flvPath);
};

#endif //TRANSCODER_H