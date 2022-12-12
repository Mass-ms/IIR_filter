#ifndef WAVE_H
#define WAVE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GROUND 32768.0
#define MAXPLUS 65535.0
#define MAXMINUS 0.0

// モノラルの音声データの定義
typedef struct
{
    int fs;     /* 標本化周波数 */
    int bits;   /* 量子化精度 */
    int length; /* 音データの長さ */
    double *s;  /* 音データ */
} MONO_PCM;

// ステレオの音声データの定義
typedef struct
{
    int fs;     /* 標本化周波数 */
    int bits;   /* 量子化精度 */
    int length; /* 音データの長さ */
    double *sL; /* 音データ（Lチャンネル） */
    double *sR; /* 音データ（Rチャンネル） */
} STEREO_PCM;

// RIFFチャンクの定義
typedef struct
{
    char chunkID[4];
    long chunkSize;
    char chunkFormType[4];
} RIFF_CHUNK;

// fmtチャンクの定義
typedef struct
{
    char chunkID[4];
    long chunkSize;
    short waveFormatType;
    short formatChannel;
    long samplesPerSec;
    long bytesPerSec;
    short blockSize;
    short bitsPerSample;
} FMT_CHUNK;

// dataチャンクの定義
typedef struct
{
    char chunkID[4];
    long chunkSize;
    short data;
} DATA_CHUNK;

// WAVEフォーマット
typedef struct
{
    RIFF_CHUNK riffChunk;
    FMT_CHUNK fmtChunk;
    DATA_CHUNK dataChunk;
} WAVE_FORMAT;

// モノラルの音声データ(waveファイル)の読み込み
void wave_read_16bit_mono(MONO_PCM *pcm, char *fileName);

// モノラルの音声データ(waveファイル)書き込み
void wave_write_16bit_mono(MONO_PCM *pcm, char *fileName);

// ステレオの音声データ(waveファイル)の読み込み
void wave_read_16bit_stereo(STEREO_PCM *pcm, char *file_name);

// ステレオの音声データ(waveファイル)書き込み
void wave_write_16bit_stereo(STEREO_PCM *pcm, char *file_name);

#endif