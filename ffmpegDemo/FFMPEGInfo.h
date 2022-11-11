#ifndef FFMPEGINFO_H
#define FFMPEGINFO_H

#include <string>

extern "C" {
    #include "libavcodec/avcodec.h"
    #include "libavformat/avformat.h"
    #include "libavfilter/avfilter.h"
}

class FFMPEGInfo
{
public:
    FFMPEGInfo();
    ~FFMPEGInfo();

    std::string configurationInfo();

    std::string urlprotocolInfo();

    std::string avformatInfo();

    std::string avcodecInfo();

    std::string avfilterInfo();
};

#endif // FFMPEGINFO_H