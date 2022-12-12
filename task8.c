#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"
#include "iir_filter.h"

#define MONO 0
#define STEREO 1
#define LPF 0
#define HPF 1
#define BPF 2
#define BSF 3
#define Reso 4
#define Notch 5
#define LSF 6
#define HSF 7
#define Peak 8

int main(void)
{
  printf("このプログラムは16bitのwavファイルを読み込み、IIRフィルタ処理を行った結果を16bitのwavで出力します。\n");
  /* ファイル名を読み込み */
  char filename[100];
  printf("読み込むファイル名を入力して下さい。\n");
  printf("例: example.wav\n");
  scanf("%s", filename);

  int channel;
  printf("読み込むファイルのチャンネル数を選択して下さい。\n");
  printf("0:モノラル, 1:ステレオ\n");
  printf("例: 1\n");
  do
  {
    scanf("%d", &channel);
    if (channel < 0 || channel > 1)
    {
      printf("\a有効な値を入力してください。\n");
    }
  } while (channel < 0 || channel > 1);

  /* wavを読み込み */
  MONO_PCM pcm0, pcm1;
  STEREO_PCM pcm2, pcm3;
  int fs;
  if (channel == MONO)
  {
    printf("wavファイル(モノラル)の読み込みを開始\n");
    wave_read_16bit_mono(&pcm0, filename);
    printf("wavファイル(モノラル)を読み込みました。\n");
    pcm1.fs = pcm0.fs;                            /* 標本化周波数 */
    pcm1.bits = pcm0.bits;                        /* 量子化精度 */
    pcm1.length = pcm0.length;                    /* 音データの長さ */
    pcm1.s = calloc(pcm1.length, sizeof(double)); /* 音データ */
    fs = pcm0.fs;
  }
  else
  {
    wave_read_16bit_stereo(&pcm2, filename);
    printf("wavファイル(ステレオ)を読み込みました。\n");
    pcm3.fs = pcm2.fs;                             /* 標本化周波数 */
    pcm3.bits = pcm2.bits;                         /* 量子化精度 */
    pcm3.length = pcm2.length;                     /* 音データの長さ */
    pcm3.sL = calloc(pcm3.length, sizeof(double)); /* 音データL */
    pcm3.sR = calloc(pcm3.length, sizeof(double)); /* 音データR */
    fs = pcm2.fs;
  }

  /* フィルタの選択 */
  int filterType;
  printf("通過させるフィルタの種類を選択してください。\n");
  printf("0:LPF, 1:HPF, 2:BPF, 3:BSF\n");
  printf("4:Reso, 5:Notch, 6:LSF, 7:HSF, 8:Peak\n");
  printf("例: 0\n");
  do
  {
    scanf("%d", &filterType);
    if (filterType < 0 || filterType > 8)
    {
      printf("\a有効な値を入力してください。\n");
    }
  } while (filterType < 0 || filterType > 8);

  /* フィルタの設定 */
  int n, m, I, J;
  double f, q, fc, fc1, fc2, Q, G, a[3], b[3];
  IIR_setup(filterType, fs, &fc, &fc1, &fc2, &Q, &G);
  I = 2; /* 遅延器の数 */
  J = 2; /* 遅延器の数 */
  printf("フィルタのセットアップ完了。\n");

  /* フィルタ係数の設定 */
  switch (filterType)
  {
  case LPF:
    IIR_LPF(fc, Q, a, b);
    break;
  case HPF:
    IIR_HPF(fc, Q, a, b);
    break;
  case BPF:
    IIR_BPF(fc1, fc2, a, b);
    break;
  case BSF:
    IIR_BEF(fc1, fc2, a, b);
    break;
  case Reso:
    IIR_resonator(fc, Q, a, b);
    break;
  case Notch:
    IIR_notch(fc, Q, a, b);
    break;
  case LSF:
    IIR_low_shelving(fc, Q, G, a, b);
    break;
  case HSF:
    IIR_high_shelving(fc, Q, G, a, b);
    break;
  case Peak:
    IIR_high_shelving(fc, Q, G, a, b);
    break;
  default:
    break;
  }
  printf("フィルタ係数の設定完了。\n");

  /* フィルタリング */
  printf("フィルタリング処理を実行します。\n");
  if (channel == MONO)
  {
    IIR_filtering(pcm0.s, pcm1.s, pcm0.length, a, b, I, J);
  }
  else
  {
    IIR_filtering(pcm2.sL, pcm3.sL, pcm2.length, a, b, I, J);
    IIR_filtering(pcm2.sR, pcm3.sR, pcm2.length, a, b, I, J);
  }

  /* wavの書き込み */
  if (channel == MONO)
  {
    wave_write_16bit_mono(&pcm1, "processed_mono.wav");
    free(pcm0.s);
    free(pcm1.s);
  }
  else
  {
    wave_write_16bit_stereo(&pcm3, "processed_stereo.wav");
    free(pcm2.sL);
    free(pcm2.sR);
    free(pcm3.sL);
    free(pcm3.sR);
  }
  printf("wavファイルを書き込みました。\n");

  return 0;
}