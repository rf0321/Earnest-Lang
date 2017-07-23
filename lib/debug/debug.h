#ifndef PRIVATE_DBG_H_INCLUDED
#define PRIVATE_DBG_H_INCLUDED
#include <stdio.h>
#include "DBG.h"

struct DBG_Controller_tag{
    FILE  *debug_write_fp;　　//ファイルの読み取り関連
    int current_debug_level; //現在のデバッグレベルパラメータ
}
#endif /*included private debug header*/