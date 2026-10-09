#!/bin/bash

# 隠しファイル（.で始まるファイル）もワイルドカード展開に含める設定
shopt -s dotglob nullglob

echo "Makefile のみを含むディレクトリを検索しています..."

# カレントディレクトリ以下の全ディレクトリを検索
find . -mindepth 1 -type d | while IFS= read -r dir; do
    # ディレクトリが存在するか確認（親ディレクトリが削除された場合への配慮）
    [ -d "$dir" ] || continue

    # ディレクトリ内の全エントリーを取得
    entries=( "$dir"/* )

    # エントリーが1つだけで、かつその名前が "Makefile" であるか判定
    if [ ${#entries[@]} -eq 1 ] && [ "$(basename "${entries[0]}")" = "Makefile" ]; then
        echo "----------------------------------------"
        echo "対象ディレクトリ: $dir"
        
        # パイプ経由のループ内でも対話入力を受け取れるよう /dev/tty から読み込む
        read -r -p "'$dir' (および内部の Makefile) を削除しますか？ [y/N]: " response < /dev/tty

        case "$response" in
            [yY][eE][sS]|[yY])
                rm -rf "$dir"
                echo "削除しました: $dir"
                ;;
            *)
                echo "スキップしました: $dir"
                ;;
        esac
    fi
done

echo "----------------------------------------"
echo "処理が完了しました。"

