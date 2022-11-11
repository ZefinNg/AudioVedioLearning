#include "FFMPEGInfo.h"

FFMPEGInfo::FFMPEGInfo()
{

}

FFMPEGInfo::~FFMPEGInfo()
{

}

std::string FFMPEGInfo::configurationInfo()
{
    //这个函数已经被弃用，不调用也行
    //av_register_all();
    return std::string(avcodec_configuration());
}

std::string FFMPEGInfo::urlprotocolInfo()
{
    std::string result;
    char *info = new char[40000]();
    struct URLProtocol *pup = NULL;
    struct URLProtocol ** pTemp = &pup;
    avio_enum_protocols((void **)pTemp, 0);

    while ((*pTemp) != NULL) {
        #if 0//std::string(NULL)会报错
        result += "[In]";
        result += std::string(avio_enum_protocols((void **)pTemp, 0));
        result += "\n";
        #else
        sprintf(info, "%s[In ][%10s]\n", info, avio_enum_protocols((void **)pTemp, 0));
        #endif
    }

    pup = NULL;
    avio_enum_protocols((void **)pTemp, 1);
    while ((*pTemp != NULL)) {
        #if 0
        result += "[Out]";
        result += std::string(avio_enum_protocols((void **)pTemp, 1));
        result += "\n";
        #else
        sprintf(info, "%s[In ][%10s]\n", info, avio_enum_protocols((void **)pTemp, 1));
        #endif
    }

    result = std::string(info);

    delete []info;
    return result;
}

std::string FFMPEGInfo::avformatInfo()
{
    AVInputFormat *ifTemp = av_iformat_next(NULL);
    AVOutputFormat *ofTemp = av_oformat_next(NULL);

    std::string result;
    while (ifTemp != NULL) {
        result += std::string("[In ]");
        result += std::string(ifTemp->name);
        result += "\n";
        ifTemp = ifTemp->next;
    }

    while (ofTemp != NULL) {
        result += std::string("[Out ]");
        result += std::string(ofTemp->name);
        result += "\n";
        ofTemp = ofTemp->next;
    }

    return result;
}

std::string FFMPEGInfo::avcodecInfo()
{
    AVCodec *codecTemp = av_codec_next(NULL);
    std::string result;

    while (codecTemp != NULL) {
        if (codecTemp->decode != NULL) {
            result += std::string("[Dec]");
        }
        else {
            result += std::string("[Enc]");
        }

        switch (codecTemp->type) {
        case AVMEDIA_TYPE_VIDEO:
            result += std::string("[Video]");
            break;
        case AVMEDIA_TYPE_AUDIO:
            result += std::string("[Audio]");
            break;
        default:
            result += std::string("[Other]");
            break;
        }

        result += std::string(codecTemp->name);
        result += "\n";

        codecTemp = codecTemp->next;
    }

    return result;
}

std::string FFMPEGInfo::avfilterInfo()
{
    avfilter_register_all();
    AVFilter *filterTemp = (AVFilter *)avfilter_next(NULL);
    std::string result;

    while (filterTemp != NULL) {
        result += std::string(filterTemp->name);
        result += "\n";
        filterTemp = filterTemp->next;
    }

    return result;
}
