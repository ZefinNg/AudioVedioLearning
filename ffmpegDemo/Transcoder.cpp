#include "Transcoder.h"

Transcoder::Transcoder()
{

}

Transcoder::~Transcoder()
{

}
int Transcoder::MP42Flv(const std::string& mp4Path, const std::string& flvPath)
{
    AVFormatContext *ifmtCtx = NULL;
    AVFormatContext *ofmtCtx = NULL;
    int ret = 0;
    int streamMappingSize = 0;
    int *streamMapping = NULL;
    AVOutputFormat *ofmt = NULL;
    unsigned int i = 0;
    int streamIndex = 0;
    AVPacket pkt;

    av_log_set_level(AV_LOG_INFO);

    ret = avformat_open_input(&ifmtCtx, mp4Path.c_str(), NULL, NULL);
    if (ret < 0){
        goto ERROR;
    }

    ret = avformat_find_stream_info(ifmtCtx, NULL);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Failed to retrive input stream information.");
        goto ERROR;
    }

    av_dump_format(ifmtCtx, 0, mp4Path.c_str(), 0);

    //第二个参数可以为"mpegts","mpeg","flv"
    avformat_alloc_output_context2(&ofmtCtx, NULL, NULL, flvPath.c_str());

    if (ofmtCtx == NULL) {
        av_log(NULL, AV_LOG_ERROR, "Could not create output context.\n");
        ret = AVERROR_UNKNOWN;
        goto ERROR;
    }

    //sreamMappin 记录视频、音频、字幕流的索引
    streamMappingSize = ifmtCtx->nb_streams;
    streamMapping = static_cast<int*>(av_mallocz_array(streamMappingSize, sizeof(*streamMapping)));
    if (streamMapping == NULL) {
        ret = AVERROR(ENOMEM);
        goto ERROR;
    }

    ofmt = ofmtCtx->oformat;
    for (int i = 0; i < ifmtCtx->nb_streams; i++) {
        AVStream *outStream;
        AVStream *inStream = ifmtCtx->streams[i];
        AVCodecParameters *inCodecPar = inStream->codecpar;

        if (inCodecPar->codec_type != AVMEDIA_TYPE_AUDIO && inCodecPar->codec_type != AVMEDIA_TYPE_VIDEO && inCodecPar->codec_type != AVMEDIA_TYPE_SUBTITLE) {
                streamMapping[i] = -1;
                continue;
        }

        streamMapping[i] = streamIndex++;

        outStream = avformat_new_stream(ofmtCtx, NULL);
        if (outStream == NULL) {
            av_log(NULL, AV_LOG_ERROR, "Failed allocation output stream.\n");
            ret = AVERROR_UNKNOWN;
            goto ERROR;
        }

        ret = avcodec_parameters_copy(outStream->codecpar, inCodecPar);

        if (ret < 0) {
            av_log(NULL, AV_LOG_ERROR, "Failed to copy codec parameters.\n");
            goto ERROR;
        }
        outStream->codecpar->codec_tag = 0;
    }

    av_dump_format(ofmtCtx, 0, flvPath.c_str(), 1);

    av_init_packet(&pkt);
    pkt.size = 0;
    pkt.data = NULL;

    if (!(ofmt->flags & AVFMT_NOFILE)) {
        ret = avio_open(&ofmtCtx->pb, flvPath.c_str(), AVIO_FLAG_WRITE);
        if (ret < 0) {
            av_log(NULL, AV_LOG_ERROR, "Could not open output file '%s'", flvPath.c_str());
            goto ERROR;
        }
    }

    ret = avformat_write_header(ofmtCtx, NULL);

    while (1) {
        AVStream *inStream, *outStream;
        ret = av_read_frame(ifmtCtx, &pkt);
        if (ret < 0) {
            break;
        }

        inStream = ifmtCtx->streams[pkt.stream_index];

        if (pkt.stream_index >= streamMappingSize || streamMapping[pkt.stream_index] < 0) {
            av_packet_unref(&pkt);
            continue;
        }

        pkt.stream_index = streamMapping[pkt.stream_index];
        outStream = ofmtCtx->streams[pkt.stream_index];

        pkt.pts = av_rescale_q_rnd(pkt.pts, inStream->time_base, outStream->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
        pkt.dts = av_rescale_q_rnd(pkt.dts, inStream->time_base, outStream->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
        pkt.duration = av_rescale_q(pkt.duration, inStream->time_base, outStream->time_base);
        pkt.pos = -1;

        ret = av_write_frame(ofmtCtx, &pkt);
        av_packet_unref(&pkt);

        if (ret < 0) {
            av_log(NULL, AV_LOG_ERROR, "Error muxing pakcet.\n");
            break;
        }
    }

    av_write_trailer(ofmtCtx);
    av_log(NULL, AV_LOG_ERROR, "Remux success.\n");
    av_dump_format(ofmtCtx, 0, flvPath.c_str(), 1);
    
ERROR:
    if (ifmtCtx != NULL) {
        avformat_close_input(&ifmtCtx);
        avformat_free_context(ifmtCtx);
    }

    if (ofmtCtx != NULL && !(ofmt->flags & AVFMT_NOFILE)) {
        avio_closep(&ofmtCtx->pb);
    }

    if (ofmtCtx != NULL) {
        avformat_free_context(ofmtCtx);
    }

    if (streamMapping != NULL) {
        av_freep(&streamMapping);
    }

    return ret;

}