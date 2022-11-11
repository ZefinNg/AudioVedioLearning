#include <iostream>
#include "FFMPEGInfo.h"
#include "Transcoder.h"

int main(int argc, char *argv[])
{
#if 0
    FFMPEGInfo ffmpegInfo;
    
    std::cout << "configuration Info:\n" << ffmpegInfo.configurationInfo() << std::endl;
    std::cout << "urlProtocol Info:\n" << ffmpegInfo.urlprotocolInfo() << std::endl;
    std::cout << "avfomat Info:\n" << ffmpegInfo.avformatInfo() << std::endl;
    std::cout << "avcodec Info:\n" << ffmpegInfo.avcodecInfo() << std::endl;
    std::cout << "avfilter Info:\n" << ffmpegInfo.avfilterInfo() << std::endl;
#endif
    Transcoder transcoder;
    transcoder.MP42Flv("/home/feng/VScodeProjects/ffmpegDemo/test.mp4", "/home/feng/VScodeProjects/ffmpegDemo/output/test.flv");

    return 0;
}