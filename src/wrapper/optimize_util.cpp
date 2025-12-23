#include "wrapper.h"



std::string get_nlopt_result_description(nlopt::result res) {
    switch (res) {
        case nlopt::SUCCESS:          return "SUCCESS\n成功 (汎用)";
        case nlopt::STOPVAL_REACHED:  return "STOPVAL_REACHED\n目標値(stopval)に到達しました";
        case nlopt::FTOL_REACHED:     return "FTOL_REACHED\n関数値の変化が許容値(ftol)以下になりました";
        case nlopt::XTOL_REACHED:     return "XTOL_REACHED\n変数の変化が許容値(xtol)以下になりました";
        case nlopt::MAXEVAL_REACHED:  return "MAXEVAL_REACHED\n最大評価回数(maxeval)に達しました";
        case nlopt::MAXTIME_REACHED:  return "MAXTIME_REACHED\n制限時間(maxtime)に達しました";
        case nlopt::FAILURE:          return "FAILURE\n失敗 (汎用)";
        case nlopt::INVALID_ARGS:     return "INVALID_ARGS\n無効な引数 (例: 負の許容値など)";
        case nlopt::OUT_OF_MEMORY:    return "OUT_OF_MEMORY\nメモリ不足";
        case nlopt::ROUNDOFF_LIMITED: return "ROUNDOFF_LIMITED\n丸め誤差による限界";
        case nlopt::FORCED_STOP:      return "FORCED_STOP\n強制停止されました";
        default:                      return "不明な結果コード";
    }
}

std::string get_nlopt_result_string(nlopt::result res) {
    switch (res) {
        case nlopt::SUCCESS:          return "SUCCESS";
        case nlopt::STOPVAL_REACHED:  return "STOPVAL_REACHED";
        case nlopt::FTOL_REACHED:     return "FTOL_REACHED";
        case nlopt::XTOL_REACHED:     return "XTOL_REACHED";
        case nlopt::MAXEVAL_REACHED:  return "MAXEVAL_REACHED";
        case nlopt::MAXTIME_REACHED:  return "MAXTIME_REACHED";
        case nlopt::FAILURE:          return "FAILURE";
        case nlopt::INVALID_ARGS:     return "INVALID_ARGS";
        case nlopt::OUT_OF_MEMORY:    return "OUT_OF_MEMORY";
        case nlopt::ROUNDOFF_LIMITED: return "ROUNDOFF_LIMITED";
        case nlopt::FORCED_STOP:      return "FORCED_STOP";
        default:                      return "不明な結果コード";
    }
}

