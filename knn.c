#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <string.h>

#define AMOUNT 150     // データ量
#define LENGTH 4       // 一データのサイズ
#define NAME_BUF 256   // データのグループ名のバッファサイズ
#define K 150

typedef struct data{
  char   name[NAME_BUF];
  double num[LENGTH];
  double result;
} DATA;

void set_data(DATA *target, DATA data_set[AMOUNT]);
void show_result(DATA data_set[AMOUNT]);

void swap(DATA *x, DATA *y);
void sort(DATA data_set[AMOUNT]);

int  mix_index(double result[AMOUNT][LENGTH+1]);
void countup(DATA *target, DATA data_set[AMOUNT]);
void set_answer(DATA *target, DATA data_set[AMOUNT]);

void get_match_group(DATA *target);

void test(void) {
}

int main(void){
  DATA target_data ={"",{3.4,4.2,5.6,1.3}, 0};
  get_match_group(&target_data);

  printf("ans_name: %s\n", target_data.name);

  return 0;
}

// data.datからデータを取得しdata_set変数に代入
void set_data(DATA *target, DATA data_set[AMOUNT]){
  FILE *fp;
  int i, j;
  double ans = 0;

  if( (fp = fopen("iris.data", "r")) == NULL){
    printf("Not found data.dat");
    exit(-1);
  }

  for( i = 0; i < AMOUNT; i++ ){
    ans = 0;
    for( j = 0; j < LENGTH; j++ ){
      fscanf(fp, "%lf,", &data_set[i].num[j]);
      ans += pow(target->num[j]-data_set[i].num[j], 2);
    }
    fscanf(fp, "%s", data_set[i].name);
    // ユークリッド距離
    data_set[i].result = sqrt(ans);
  }

  fclose(fp);
}

void show_result(DATA data_set[AMOUNT]){
  int i, j;
  for( i = 0; i < AMOUNT; i++ ){
    printf("Name of \"%s\" 's data_set[%2d] = squere( ", data_set[i].name, i);
    for( j = 0; j < LENGTH-1; j++ ){
      printf("%4.1lf^2 + ", data_set[i].num[j]);
    }
    printf("%4.1lf^2 ) = ", data_set[i].num[j]);
    printf("%4.1lf\n", data_set[i].result);
  }
}

// sortの時のswapに使用
void swap(DATA *x, DATA *y){
  DATA tmp = *x;
  *x = *y;
  *y = tmp;
}

// DATAのresult(距離)を基準にデータセットを昇順に並び替え
void sort(DATA data_set[AMOUNT]){
  int i, j;
  for( i = 0; i < AMOUNT-1; i++ ){
    for( j = i+1; j < AMOUNT; j++ ){
      if( data_set[i].result > data_set[j].result ){
        swap(&data_set[i], &data_set[j]);
      }
    }
  }
}

// targetから距離が最も近いデータのindexを返す
int min_index(DATA data_set[AMOUNT]){
  int i;
  int min = INT_MAX;
  int min_i = 0;

  for( i = 0; i < AMOUNT; i++ ){
    printf("%lf < %lf\n", i, data_set[i].result, min_i, data_set[min_i].result);
    if( i != min_i ){
      if( data_set[i].result < data_set[min_i].result ){
        min = data_set[i].result;
        min_i = i;
      }
    }
  }
  return min_i;
}

void countup(DATA *target, DATA data_set[AMOUNT]){
  int i, j;
  const int count_buf = 256;
  int count_amount = 0;
  int max       = 0;
  int max_index = 0;
  // 連想配列のまねごと
  struct {
    char name[NAME_BUF];
    int count;
  } counter[count_buf];

  // 初期化
  for( i = 0; i < count_buf; i++ ){
    counter[i].name[0] = '\0';
    counter[i].count = 0;
  }
  // 全てのデータセットにアクセス
  for( i = 0; i < K; i++ ){
    // セットされてるカウンタにアクセス
    for( j = 0; j < count_amount; j++ ){
      // カウンタに名前があるならカウントアップ
      if( !strcmp(counter[j].name, data_set[i].name) ){
        counter[j].count++;
        break;
      }
    }
    // カウンターにまだない名前なら新たに追加
    if( j == count_amount ){
      count_amount++;
      counter[j].count++;
      strcpy(counter[j].name, data_set[i].name);
    }
  }
  // 範囲内に最も出現数が多いものをtargetにset
  printf("resultNum: %d\n", count_amount);
  for( i = 0; i < count_amount; i++ ){
    if( counter[i].count > max ){
      max = counter[i].count;
      max_index = i;
    }
    printf("result name: %s count: %d\n", counter[i].name, counter[i].count);
  }
  strcpy(target->name, counter[max_index].name);
}

void set_answer(DATA *target, DATA data_set[AMOUNT]){
  int flg = 1;
  if( flg == 0 ){
    // 最も近いグループ名をtarget.nameにせっと
    strcpy(target->name, data_set[min_index(data_set)].name);
  } else if( flg == 1 ){
    countup(target, data_set);
  }
}

// targetにグループ名をsetする
void get_match_group(DATA *target){
  DATA data_set[AMOUNT];
  int ans_index = 0;

  // データを変数にセット
  set_data(target, data_set);
  // それぞれのtargetからの距離を基準にソート
  sort(data_set);

  // 確認
  show_result(data_set);
  // どのグループに属するかをtarget.nameにセット
  set_answer(target, data_set);
}

