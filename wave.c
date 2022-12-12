#include "wave.h"

void wave_read_16bit_mono(MONO_PCM *pcm, char *fileName)
{
    FILE *fp;
    int i;
    WAVE_FORMAT waveFormat;

    fp = fopen(fileName, "rb");

    if (!fp)
    {
        printf("\aファイルの読み込みに失敗しました。\n");
        exit(0);
    }

    // RIFFチャンクの読み込み
    fread(waveFormat.riffChunk.chunkID, 1, 4, fp);
    fread(&waveFormat.riffChunk.chunkSize, 4, 1, fp);
    fread(waveFormat.riffChunk.chunkFormType, 1, 4, fp);

    // fmtチャンクの読み込み
    fread(waveFormat.fmtChunk.chunkID, 1, 4, fp);
    fread(&waveFormat.fmtChunk.chunkSize, 4, 1, fp);
    fread(&waveFormat.fmtChunk.waveFormatType, 2, 1, fp);
    fread(&waveFormat.fmtChunk.formatChannel, 2, 1, fp);
    fread(&waveFormat.fmtChunk.samplesPerSec, 4, 1, fp);
    fread(&waveFormat.fmtChunk.bytesPerSec, 4, 1, fp);
    fread(&waveFormat.fmtChunk.blockSize, 2, 1, fp);
    fread(&waveFormat.fmtChunk.bitsPerSample, 2, 1, fp);

    // dataチャンクの読み込み
    fread(waveFormat.dataChunk.chunkID, 1, 4, fp);
    fread(&waveFormat.dataChunk.chunkSize, 4, 1, fp);

    pcm->fs = waveFormat.fmtChunk.samplesPerSec;
    pcm->bits = waveFormat.fmtChunk.bitsPerSample;
    pcm->length = waveFormat.dataChunk.chunkSize;
    pcm->s = calloc(pcm->length, sizeof(double));

    short data;
    for (i = 0; i < pcm->length; i++)
    {
        fread(&data, 2, 1, fp);
        pcm->s[i] = (double)data / GROUND;
    }

    fclose(fp);
}

void wave_write_16bit_mono(MONO_PCM *pcm, char *fileName)
{
    FILE *fp;
    int i;
    WAVE_FORMAT waveFormat;

    strcpy(waveFormat.riffChunk.chunkID, "RIFF");
    waveFormat.riffChunk.chunkSize = 36 + pcm->length;
    strcpy(waveFormat.riffChunk.chunkFormType, "WAVE");

    strcpy(waveFormat.fmtChunk.chunkID, "fmt ");
    waveFormat.fmtChunk.chunkSize = 16;
    // PCMの場合は1
    waveFormat.fmtChunk.waveFormatType = 1;
    // モノラルの場合は1,ステレオの場合は2
    waveFormat.fmtChunk.formatChannel = 1;
    waveFormat.fmtChunk.samplesPerSec = pcm->fs;
    waveFormat.fmtChunk.bytesPerSec = (pcm->fs * pcm->bits) / 8;
    waveFormat.fmtChunk.blockSize = pcm->bits / 8;
    waveFormat.fmtChunk.bitsPerSample = pcm->bits;

    strcpy(waveFormat.dataChunk.chunkID, "data");
    waveFormat.dataChunk.chunkSize = pcm->length;

    fp = fopen(fileName, "wb");

    if (!fp)
    {
        printf("\aファイルの生成に失敗しました。\n");
        exit(0);
    }

    // RIFFチャンクの書き込み
    fwrite(waveFormat.riffChunk.chunkID, 1, 4, fp);
    fwrite(&waveFormat.riffChunk.chunkSize, 4, 1, fp);
    fwrite(waveFormat.riffChunk.chunkFormType, 1, 4, fp);

    // fmtチャンクの書き込み
    fwrite(waveFormat.fmtChunk.chunkID, 1, 4, fp);
    fwrite(&waveFormat.fmtChunk.chunkSize, 4, 1, fp);
    fwrite(&waveFormat.fmtChunk.waveFormatType, 2, 1, fp);
    fwrite(&waveFormat.fmtChunk.formatChannel, 2, 1, fp);
    fwrite(&waveFormat.fmtChunk.samplesPerSec, 4, 1, fp);
    fwrite(&waveFormat.fmtChunk.bytesPerSec, 4, 1, fp);
    fwrite(&waveFormat.fmtChunk.blockSize, 2, 1, fp);
    fwrite(&waveFormat.fmtChunk.bitsPerSample, 2, 1, fp);

    // dataチャンクの書き込み
    fwrite(waveFormat.dataChunk.chunkID, 1, 4, fp);
    fwrite(&waveFormat.dataChunk.chunkSize, 4, 1, fp);

    short data;
    double s;
    for (i = 0; i < pcm->length; i++)
    {
        s = ((pcm->s[i] + 1.0) / 2) * (MAXPLUS + 1.0);
        if (s > MAXPLUS)
            s = MAXPLUS;
        else if (s < MAXMINUS)
            s = MAXMINUS;

        data = (short)((int)(s + 0.5) - GROUND);
        fwrite(&data, 2, 1, fp);
    }

    fclose(fp);
}

void wave_read_16bit_stereo(STEREO_PCM *pcm, char *fileName)
{
    FILE *fp;
    int i;
    WAVE_FORMAT waveFormat;

    fp = fopen(fileName, "rb");

    if (!fp)
    {
        printf("\aファイルの読み込みに失敗しました。\n");
        exit(0);
    }

    // RIFFチャンクの読み込み
    fread(waveFormat.riffChunk.chunkID, 1, 4, fp);
    fread(&waveFormat.riffChunk.chunkSize, 4, 1, fp);
    fread(waveFormat.riffChunk.chunkFormType, 1, 4, fp);

    // fmtチャンクの読み込み
    fread(waveFormat.fmtChunk.chunkID, 1, 4, fp);
    fread(&waveFormat.fmtChunk.chunkSize, 4, 1, fp);
    fread(&waveFormat.fmtChunk.waveFormatType, 2, 1, fp);
    fread(&waveFormat.fmtChunk.formatChannel, 2, 1, fp);
    fread(&waveFormat.fmtChunk.samplesPerSec, 4, 1, fp);
    fread(&waveFormat.fmtChunk.bytesPerSec, 4, 1, fp);
    fread(&waveFormat.fmtChunk.blockSize, 2, 1, fp);
    fread(&waveFormat.fmtChunk.bitsPerSample, 2, 1, fp);

    // dataチャンクの読み込み
    fread(waveFormat.dataChunk.chunkID, 1, 4, fp);
    fread(&waveFormat.dataChunk.chunkSize, 4, 1, fp);

    pcm->fs = waveFormat.fmtChunk.samplesPerSec;
    pcm->bits = waveFormat.fmtChunk.bitsPerSample;
    pcm->length = waveFormat.dataChunk.chunkSize;
    pcm->sL = calloc(pcm->length, sizeof(double));
    pcm->sR = calloc(pcm->length, sizeof(double));

    /* 音データの読み取り&正規化 */
    short data;
    for (i = 0; i < pcm->length; i++)
    {
        fread(&data, 2, 1, fp);
        pcm->sL[i] = (double)data / GROUND;

        fread(&data, 2, 1, fp);
        pcm->sR[i] = (double)data / GROUND;
    }

    fclose(fp);
}

void wave_write_16bit_stereo(STEREO_PCM *pcm, char *fileName)
{
    FILE *fp;
    int i;
    WAVE_FORMAT waveFormat;

    strcpy(waveFormat.riffChunk.chunkID, "RIFF");
    waveFormat.riffChunk.chunkSize = 36 + pcm->length;
    strcpy(waveFormat.riffChunk.chunkFormType, "WAVE");

    strcpy(waveFormat.fmtChunk.chunkID, "fmt ");
    waveFormat.fmtChunk.chunkSize = 16;
    // PCMの場合は1
    waveFormat.fmtChunk.waveFormatType = 1;
    // モノラルの場合は1,ステレオの場合は2
    waveFormat.fmtChunk.formatChannel = 2;
    waveFormat.fmtChunk.samplesPerSec = pcm->fs;
    waveFormat.fmtChunk.bytesPerSec = (pcm->fs * pcm->bits) / 8 * 2;
    waveFormat.fmtChunk.blockSize = pcm->bits / 8 * 2;
    waveFormat.fmtChunk.bitsPerSample = pcm->bits;

    strcpy(waveFormat.dataChunk.chunkID, "data");
    waveFormat.dataChunk.chunkSize = pcm->length;

    fp = fopen(fileName, "wb");

    if (!fp)
    {
        printf("\aファイルの生成に失敗しました。\n");
        exit(0);
    }

    // RIFFチャンクの書き込み
    fwrite(waveFormat.riffChunk.chunkID, 1, 4, fp);
    fwrite(&waveFormat.riffChunk.chunkSize, 4, 1, fp);
    fwrite(waveFormat.riffChunk.chunkFormType, 1, 4, fp);

    // fmtチャンクの書き込み
    fwrite(waveFormat.fmtChunk.chunkID, 1, 4, fp);
    fwrite(&waveFormat.fmtChunk.chunkSize, 4, 1, fp);
    fwrite(&waveFormat.fmtChunk.waveFormatType, 2, 1, fp);
    fwrite(&waveFormat.fmtChunk.formatChannel, 2, 1, fp);
    fwrite(&waveFormat.fmtChunk.samplesPerSec, 4, 1, fp);
    fwrite(&waveFormat.fmtChunk.bytesPerSec, 4, 1, fp);
    fwrite(&waveFormat.fmtChunk.blockSize, 2, 1, fp);
    fwrite(&waveFormat.fmtChunk.bitsPerSample, 2, 1, fp);

    // dataチャンクの書き込み
    fwrite(waveFormat.dataChunk.chunkID, 1, 4, fp);
    fwrite(&waveFormat.dataChunk.chunkSize, 4, 1, fp);

    /* 音データの書き込み&クリッピング */
    short data;
    double sL;
    double sR;
    for (i = 0; i < pcm->length; i++)
    {
        sL = ((pcm->sL[i] + 1.0) / 2) * (MAXPLUS + 1.0);
        if (sL > MAXPLUS)
            sL = MAXPLUS;
        else if (sL < MAXMINUS)
            sL = MAXMINUS;

        data = (short)((int)(sL + 0.5) - GROUND);
        fwrite(&data, 2, 1, fp);

        sR = ((pcm->sR[i] + 1.0) / 2) * (MAXPLUS + 1.0);
        if (sR > MAXPLUS)
            sR = MAXPLUS;
        else if (sR < MAXMINUS)
            sR = MAXMINUS;

        data = (short)((int)(sR + 0.5) - GROUND);
        fwrite(&data, 2, 1, fp);
    }

    fclose(fp);
}