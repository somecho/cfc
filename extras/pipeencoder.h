#define _POSIX_C_SOURCE 200112L

#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "lockfreequeue.h"

#ifndef SOYA_PIPEENCORDER_H_
#define SOYA_PIPEENCORDER_H_

typedef struct syPipeEncoderOptions
{
  uint32_t width;
  uint32_t height;
  uint32_t inputFps;
  uint32_t outputFps;
  char *outputPath;
  char *codec;
  char *outputPixelFormat;
  char *extraInputArgs;
  char *extraOutputArgs;
  char *inputPixelFormat;
} syPipeEncoderOptions;

typedef struct syPipeEncoder
{
  uint8_t numChannels;
  uint32_t width, height;
  syLFQ frames;
  _Atomic bool isRecording;
  FILE *pipe;
  pthread_t piper;
  char *_cmd;
} syPipeEncoder;

void syPipeEncoderInit(syPipeEncoder *enc, syPipeEncoderOptions *opts)
{
  if (opts->outputPath == NULL)
  {
    puts("syPipeEncoder: warning - outputPath is empty!");
  }

  enc->_cmd = calloc(2048, sizeof(char));
  sprintf(enc->_cmd, "ffmpeg");
  sprintf(enc->_cmd, "%s -y", enc->_cmd);  // overwrite
  sprintf(enc->_cmd, "%s -an", enc->_cmd); // disable audio
  sprintf(enc->_cmd, "%s -framerate %u", enc->_cmd, opts->inputFps);
  sprintf(enc->_cmd, "%s -s %ux%u", enc->_cmd, opts->width, opts->height);
  sprintf(enc->_cmd, "%s -f rawvideo", enc->_cmd);
  sprintf(enc->_cmd, "%s -pix_fmt %s", enc->_cmd, opts->inputPixelFormat);
  if (opts->extraInputArgs != NULL)
  {
    sprintf(enc->_cmd, "%s %s ", enc->_cmd, opts->extraInputArgs);
  }
  sprintf(enc->_cmd, "%s -i pipe:", enc->_cmd);
  sprintf(enc->_cmd, "%s -c:v %s", enc->_cmd, opts->codec);
  sprintf(enc->_cmd, "%s -r %u", enc->_cmd, opts->outputFps);
  if (opts->extraOutputArgs != NULL)
  {
    sprintf(enc->_cmd, "%s %s", enc->_cmd, opts->extraOutputArgs);
  }
  sprintf(enc->_cmd, "%s -pix_fmt %s", enc->_cmd, opts->outputPixelFormat);
  sprintf(enc->_cmd, "%s %s", enc->_cmd, opts->outputPath);

  enc->numChannels = (strcmp(opts->inputPixelFormat, "rgba") == 0) ? 4 : 3;
  enc->width = opts->width;
  enc->height = opts->height;
  syLFQInit(&enc->frames);
}

void *syPipeEncoderProcessFrame(void *args)
{
  syPipeEncoder *enc = (syPipeEncoder *)args;
  while (atomic_load(&enc->isRecording) || atomic_load(&enc->frames.count) > 0)
  {
    void *data = syLFQConsume(&enc->frames);
    if (data != NULL)
    {
      size_t len = enc->width * enc->height * enc->numChannels;
      fwrite(data, sizeof(char), len, enc->pipe);
      free(data);
    }
  }
  return NULL;
}

void syPipeEncoderEncode(syPipeEncoder *enc, void *data)
{
  syLFQProduce(&enc->frames, data);
}

void syPipeEncoderStart(syPipeEncoder *enc)
{
  if (atomic_load(&enc->isRecording))
  {
    perror("syPipeEncoder: Already recording.");
    return;
  }
  else
  {
    atomic_store(&enc->isRecording, true);
    enc->pipe = popen(enc->_cmd, "w");
    if (!enc->pipe)
    {
      perror("syPipEncoder: Unable to open pipe.\n");
      return;
    }
    setvbuf(enc->pipe, NULL, _IONBF, 0);
    pthread_create(&enc->piper, NULL, syPipeEncoderProcessFrame, enc);
  }
}

void syPipeEncoderStop(syPipeEncoder *enc)
{
  if (atomic_load(&enc->isRecording))
  {
    puts("syPipeEncoder: Stopping encoding. Wating for thread to join...");
    atomic_store(&enc->isRecording, false);

    pthread_join(enc->piper, NULL);

    int res = pclose(enc->pipe);
    if (res < 0)
    {
      perror("syPipeEncoder: Error closing FFmpeg pipe.\n");
      enc->pipe = NULL;
    }
  }
}

#endif // PIPERECORDER_H_
