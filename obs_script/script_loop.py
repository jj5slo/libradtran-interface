import sys
import subprocess

def run_target_program():
	ns = "n"
	band = 1
	
	year = 2022
	month = 5
	day = 1

	minute = 0

	for hour in range(0, 24):
		# 実行するコマンドをリストで指定します
		# 最初の要素は実行するプログラム（python）、2番目は実行するスクリプト、3番目以降が引数です
		command = ['python3', 'makedata.py', str(year), str(month), str(day), str(hour), str(minute), ns, str(band)]

		try:
			# subprocess.run() でコマンドを実行します
			# check=True を指定すると、実行が失敗した場合に例外が発生します
			result = subprocess.run(command, check=True, capture_output=True, text=True, encoding='utf-8')

			# 実行結果（標準出力）を表示します
			print(result.stdout)

		except subprocess.CalledProcessError as e:
			# プログラムの実行が失敗した場合の処理
			print(f"エラーが発生しました: {e}")
			print(f"標準エラー出力:\n{e.stderr}")
		except FileNotFoundError:
			print("エラー: 'python' または 'target_program.py' が見つかりません。")

# プログラムを実行します
if __name__ == "__main__":
	run_target_program()

