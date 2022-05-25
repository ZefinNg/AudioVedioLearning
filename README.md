# AudioVedioLearning
音视频学习记录



* PCMplayer.c
  * linux下alsa播放pcm的demo；
  * 编译命令为：gcc PCMplayer.c-o PCMplayer-lasound -ldl -lm
  * 使用：./player hw:0,0 /home/twdz/Music/pcmFile/浪花一朵朵片段8k16bit单声道.pcm