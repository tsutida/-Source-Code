#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>

#define APP_NAME TEXT("reversi")

#define BLACK        1  // 表示色　黒
#define WHITE        2  // 表示色　白
#define GREEN        3  // 表示色　緑
#define RED          4  // 表示色　赤

#define BSTONE          1 //黒石
#define WSTONE          -1  //白石
#define BPLAYER           1 //黒プレイヤー
#define WPLAYER           -1 //白プレイヤー

#define REVERSE     -1  //ひっくり返すときに使う定数　

#define EMP          0 // 空

/* ゲームの状態
1:初回起動時（初期化）
2:プレイ中
3:リザルト表示中*/
int status = 1;


//　マス一つ毎のデータ
typedef struct _masu {
	/*マスの状態を表す定数
	0:何もなし
	1:BLACK
	-1:WHITE */
	int  m_setstone;

	/*マスに石が置けるかどうか表す定数
	0:おけない
	1:黒が置ける
	-1:白が置ける*/
	int  m_state;

}MASU;

//　ゲーム全体のデータ
typedef struct _game {
	// masuは外周を検索することから8x8ではなく、10x10としている
	// したがって、有効範囲は、1-8までの位置となる
	MASU masu[10][10];

	/*現在の番のプレイヤー
	黒側 : 1
	白側 : -1*/

	int blackcnt;//マス上にある黒の石の総数
	int whitecnt;//マス上にある白の石の総数

	int passflg;
	int player;
	int winner;
}GAMEMASTER;

//クリックした座標
POINT pt;

//選択したマス座標
POINT cp;

//光らせるマスの座標
POINT pt2;

/*******************************************************************************
関数名 check 担当者 :益子
機能 すべてのマスを石が置けるかどうか判断し、判断結果をmasu[i][j].m_stateに格納する
引数：&ms　MASU (i/o)  //i =入力　o =出力
返却値　なし
*******************************************************************************/
int check(GAMEMASTER *);
/*******************************************************************************
関数名 reverse 担当者 :槌田
機能 選択したマスから石が返せるかどうか判断し、判断結果をmasu[i][j].m_setstoneに格納する
引数：&ms　GAMEMASTER ,POINT cp
返却値　なし
*******************************************************************************/
void reverse(GAMEMASTER *, POINT);
/*******************************************************************************
関数名 : WindowProc
機能   : メッセージ処理を行う
引数   : 省略
返却値 : 省略
*******************************************************************************/
LRESULT CALLBACK WindowProc(
	HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam
) {


	HDC hdc;
	//タイトル画面
	TCHAR Title[] = _T("オセロ");
	TCHAR Button[] = _T("右クリックorEnter");
	//操作説明
	TCHAR Restart[] = _T("ポーズ(一時停止):Enterか右クリック");
	TCHAR Mouse[] = _T("左クリック:コマを置く");
	TCHAR Yazirushi[] = _T("矢印キー:盤面移動");
	TCHAR Space[] = _T("スペースキー:コマを置く");
	//ポーズテキスト
	TCHAR Pause[] = _T("ポーズ中");
	//持ち時間
	TCHAR AllottedT[] = _T("持ち時間");
	//石の総数
	TCHAR TotalSt[] = _T("石の総数");
	//黒タイマー
	static int bCount;
	static TCHAR bstrCount[64];
	//白タイマー
	static int wCount;
	static TCHAR wstrCount[64];
	//自分のターン
	TCHAR BlackT[] = _T("黒のターンです");
	TCHAR WhiteT[] = _T("白のターンです");
	//パス
	TCHAR Pass[] = _T("パスしました");
	//石の総数をカウントする
	TCHAR BC[10];//黒の総数
	TCHAR WC[10];//白の総数
	TCHAR Draw[] = _T("引き分けです");
	TCHAR Blackwin[] = _T("黒の勝ちです");
	TCHAR Whitewin[] = _T("白の勝ちです");
	//フォント
	HFONT hFont1; //タイトル画面用フォント
	HFONT hFont2; //ターン描画用
	HFONT hFont3; //現在コマ描画用
	HFONT hFont4; //操作説明用
	HFONT hFont5; //一部テキスト用(石の総数、待ち時間)
	//描画パス
	PAINTSTRUCT ps;

	static BOOL blRight = TRUE;
	static HBRUSH hBrush[3];
	int i, j, x = 0;

	static GAMEMASTER ms;

	// 盤面上の小さい黒丸の座標点（描画時の各中心座標）
	static POINT dot[4] = {
	{ 100,100 },
	{ 300,100 },
	{ 100,300 },
	{ 300,300 }
	};

	//ゲーム開始に一度だけ行う処理/////////////////////////////
	if (status == 1) {

		bCount = 300;
		wCount = 300;
		cp.x = -1;
		cp.y = -1;

		pt2.x = 0;
		pt2.y = 0;

		memset(&ms, 0x00, sizeof(GAMEMASTER));

		ms.player = 1;
		ms.winner = 0;
		ms.passflg = 0;
		//masuをすべて石が置いてない状態にする
		for (i = 0; i < 10; i++) {
			for (j = 0; j < 10; j++) {
				(ms.masu[i][j]).m_setstone = 0;
			}
		}

		//中央4マスに石を配置する

		(ms.masu[4][5]).m_setstone = BSTONE;
		(ms.masu[5][4]).m_setstone = BSTONE;
		(ms.masu[4][4]).m_setstone = WSTONE;
		(ms.masu[5][5]).m_setstone = WSTONE;

		ms.blackcnt = 2;//マス上にある黒の石の総数
		ms.whitecnt = 2;//マス上にある白の石の総数

						/*
						関数名 check 担当者 :益子
						機能 すべてのマスを石が置けるかどうか判断し、判断結果をmasu[i][j].m_stateに格納する
						引数：&ms　MASU (i/o)  //i =入力　o =出力
						返却値　0:一切置けない　1:最低一か所置ける
						*/
		check(&ms);

		hBrush[4] = CreateSolidBrush(RGB(0xAA, 0xEE, 0));          // 黄緑
																   //プレイ状態にする
	}

	switch (uMsg) {

	case WM_CREATE:  // 初期生成時の処理（初期化）

					 //タイマーの定義
		SetTimer(hWnd, 1, 1000, NULL);

		//ブラシデータの定義
		hBrush[0] = CreateSolidBrush(RGB(0, 0xAA, 0));          // 盤面（緑）
		hBrush[1] = CreateSolidBrush(RGB(0xFF, 0xFF, 0xFF));    // WHITE
		hBrush[2] = CreateSolidBrush(RGB(0, 0, 0));		        // BLACK
		hBrush[3] = CreateSolidBrush(RGB(0xEE, 0xEE, 0));		// 黄色
		hBrush[4] = CreateSolidBrush(RGB(0xAA, 0xEE, 0));       // 黄緑

		return 0;

	case WM_TIMER:
		if (status == 2 && ms.player == BPLAYER) {
			bCount--;
		}
		if (status == 2 && ms.player == WPLAYER) {
			wCount--;
		}
		if (wCount == 0) {
			status = 3;
		}
		if (bCount == 0) {
			status = 3;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;

	case WM_DESTROY:  // ウィンドウ破棄時の処理


					  // ブラシの削除
		for (i = 0; i < 2; i++) DeleteObject(hBrush[i]);
		PostQuitMessage(0);
		return 0;
	case WM_RBUTTONDOWN:  //　右クリックしたときのイベント(初期化)
		if (status == 1) {
			status = 2;
			InvalidateRect(hWnd, NULL, TRUE);
			return 0;
		}
		//一時停止処理{
		//黒のターン時
		if (status == 2 && ms.player == BPLAYER) {
			status = 0;
			InvalidateRect(hWnd, NULL, TRUE);
			return 0;
		}
		if (status == 0 && ms.player == BPLAYER) {
			status = 2;
			InvalidateRect(hWnd, NULL, TRUE);
			return 0;
		}	//}

		//白のターン時{
		if (status == 2 && ms.player == WPLAYER) {
			status = 0;
			InvalidateRect(hWnd, NULL, TRUE);
			return 0;
		}
		if (status == 0 && ms.player == WPLAYER) {
			status = 2;
			InvalidateRect(hWnd, NULL, TRUE);
			return 0;
		}
		//}
		if (status == 3) {
			status = 1;
			InvalidateRect(hWnd, NULL, TRUE);
			return 0;
		}

	case WM_LBUTTONDOWN:  //　左クリックしたときのイベント 
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);

		if (ms.passflg == 1) {
			ms.passflg = 0;
		}


		if ((pt.x < 0) || (pt.x > 400) ||//領域外をクリックしても反応しない
			(pt.y < 0) || (pt.y > 400)) {
			return 0;
		}

		//クリックした座標をマス座標へ変換する
		cp.x = ((pt.x) / 50) + 1;
		cp.y = ((pt.y) / 50) + 1;

		//石が置いてある場所を選択しても反応しない
		if (status == 0 || (ms.masu[cp.x][cp.y]).m_setstone != 0)return 0;

		//石が返せない場所を選択しても反応しない
		if (status == 0 || ( ms.masu[cp.x][cp.y]).m_state != ms.player)return 0;
		//クリックした場所に石を追加する
		if (ms.player == BLACK) {
			(ms.masu[cp.x][cp.y]).m_setstone = BSTONE;
			ms.blackcnt++;//マス上にある黒の石の総数

		}
		else{
			(ms.masu[cp.x][cp.y]).m_setstone = WSTONE;
			ms.whitecnt++;//マス上にある白の石の総数
		}

		/*
		関数名:　　担当者 :槌田
		機能:石を置いたときにひっくり返す処理
		引数：変数名　&masu　MASU (i/o) ,cp.x+1 int(i) ,cp.y+1(i) int ,player int(i)

		引数意味
		&masu         :マス情報
		cp.x+1,cp.y+1 :選択したマス座標
		player        :どちらの番か表す変数

		返却値：無し
		備考　：関数内で石の総数を変動させる処理を行う
		*/
		reverse(&ms, cp);
		ms.player *= REVERSE;

		//関数checkを呼び出す
		if (check(&ms) == 0) {
			ms.player *= REVERSE;
			ms.passflg = 1;
			if (check(&ms) == 0) {
				status = 3;
			}
		}
	 

		/*
		if (status == 1) {
		ms.player *= -1;
		//プレイ状態にする
		status = 2;
		}
		*/
		/*リザルト処理
		　　  　  ->石の総数を表示
			   　　　　  勝敗表示
					 　　　  　エンターキーか左クリックで
						   　　　　  初期化してリスタートする処理　*/
		InvalidateRect(hWnd, NULL, TRUE); //再描画させる命令（ウインドウのハンドル,長方形の座標(NULLで全体),TRUEで背景をいったん消す）

	case WM_KEYDOWN:  //　キーを押したときのイベント　担当者:伊奈

		if (wParam == VK_RETURN) {   //エンターキーが押されたら初期化
			if (status == 1) {
				status = 2;
				InvalidateRect(hWnd, NULL, TRUE);
				return 0;
			}
			//一時停止処理{
			//黒のターン時{
			if (status == 2 && ms.player == BPLAYER) {
				status = 0;
				InvalidateRect(hWnd, NULL, TRUE);
				return 0;
			}
			if (status == 0 && ms.player == BPLAYER) {
				status = 2;
				InvalidateRect(hWnd, NULL, TRUE);
				return 0;
			}//}

			//白のターン時{
			if (status == 2 && ms.player == WPLAYER) {
				status = 0;
				InvalidateRect(hWnd, NULL, TRUE);
				return 0;
			}
			if (status == 0 && ms.player == WPLAYER) {
				status = 2;
				InvalidateRect(hWnd, NULL, TRUE);
				return 0;
			}
			//}
			if (status == 3) {
				status = 1;
				InvalidateRect(hWnd, NULL, TRUE);
				return 0;
			}
		}
		if (wParam == VK_RIGHT) pt2.x++;  //右→
		if (pt2.x > 8) {
			pt2.x = 1;
		}
		if (wParam == VK_LEFT) pt2.x--; //左←
		if (pt2.x < 1) {
			pt2.x = 8;
		}
		if (wParam == VK_UP) pt2.y--; //上↑ 
		if (pt2.y < 1) {
			pt2.y = 8;
		}
		if (wParam == VK_DOWN) pt2.y++;  //下↓   
		if (pt2.y > 8) {
			pt2.y = 1;
		}

		if (wParam == VK_SPACE) {//スペースキーが押されたら
			if ((ms.masu[pt2.x][pt2.y]).m_setstone != 0)return 0;

			//石が返せない場所を選択しても反応しない
			if ((ms.masu[pt2.x][pt2.y]).m_state != ms.player)return 0;

			if (ms.player == BLACK) {//プレイヤーが黒なら
				(ms.masu[pt2.x][pt2.y]).m_setstone = BSTONE;
				ms.blackcnt++;//マス上にある黒の石の総数
			}
			else {//プレイヤーが白なら
				(ms.masu[pt2.x][pt2.y]).m_setstone = WSTONE;
				ms.whitecnt++;//マス上にある白の石の総数
			}

			reverse(&ms, pt2);
			ms.player *= REVERSE;
			if (check(&ms) == 0) {
				ms.player *= REVERSE;
				ms.passflg = 1;
				if (check(&ms) == 0) {
					status = 3;
				}
			}
		}
		//石を置く場所をキーボードを使って選択する処理を作る

		/*wParamにキーコードの値が入る

		　　vk_LEFT  vk_SPACE
		  vk_RIGHT vk_UP
		  vk_DOWN*/

		  //選択しているマスを光らせる処理を作る（黄色）

		InvalidateRect(hWnd, NULL, TRUE); //再描画させる命令（ウインドウのハンドル,長方形の座標(NULLで全体),TRUEで背景をいったん消す）

	case WM_PAINT: // 描画イベント発生時　　（画面に変化があった際に頻繁に呼び出される)

				   //どちらの番か表示する処理　担当者 :山本

				   //持ち時間（プレイヤーごと）担当者 :山本

				   //置くことができるマスを光らせる処理

				   // ペイント開始
		hdc = BeginPaint(hWnd, &ps);
		hFont1 = CreateFont(
			50,							// フォントの高さ(大きさ)
			0,								// フォントの幅
			0,								// 水平軸の角度　反時計回りに回る
			0,								// X軸の角度　　 
			FW_BOLD,						// 文字の太さ
			FALSE,							// フォントがイタリックならTRUEを指定
			FALSE,							// 下線を引くならTRUE
			FALSE,							// 取り消し線を引くならTRUE
			SHIFTJIS_CHARSET,				// フォントの文字セット
			OUT_DEFAULT_PRECIS,				// 出力精度の設定
			CLIP_DEFAULT_PRECIS,			// クリッピング精度
			DEFAULT_QUALITY,				// フォントの出力品質
			VARIABLE_PITCH | FF_ROMAN,		// フォントのピッチとファミリを指定
			NULL							/* フォントのタイプフェイス名の指定*/);

		hFont2 = CreateFont(
			40,								// フォントの高さ(大きさ)
			0,								// フォントの幅
			0,								// 水平軸の角度　反時計回りに回る
			0,								// X軸の角度　　 
			FW_BOLD,						// 文字の太さ
			FALSE,							// フォントがイタリックならTRUEを指定
			FALSE,							// 下線を引くならTRUE
			FALSE,							// 取り消し線を引くならTRUE
			SHIFTJIS_CHARSET,				// フォントの文字セット
			OUT_DEFAULT_PRECIS,				// 出力精度の設定
			CLIP_DEFAULT_PRECIS,			// クリッピング精度
			DEFAULT_QUALITY,				// フォントの出力品質
			VARIABLE_PITCH | FF_ROMAN,		// フォントのピッチとファミリを指定
			NULL							/* フォントのタイプフェイス名の指定*/);

		hFont3 = CreateFont(
			50,								// フォントの高さ(大きさ)
			0,								// フォントの幅
			0,								// 水平軸の角度　反時計回りに回る
			0,								// X軸の角度　　 
			FW_BOLD,						// 文字の太さ
			FALSE,							// フォントがイタリックならTRUEを指定
			FALSE,							// 下線を引くならTRUE
			FALSE,							// 取り消し線を引くならTRUE
			SHIFTJIS_CHARSET,				// フォントの文字セット
			OUT_DEFAULT_PRECIS,				// 出力精度の設定
			CLIP_DEFAULT_PRECIS,			// クリッピング精度
			DEFAULT_QUALITY,				// フォントの出力品質
			VARIABLE_PITCH | FF_ROMAN,		// フォントのピッチとファミリを指定
			NULL							/* フォントのタイプフェイス名の指定*/);//フォント処理の終了

		hFont4 = CreateFont(
			40,								// フォントの高さ(大きさ)
			0,								// フォントの幅
			0,								// 水平軸の角度　反時計回りに回る
			0,								// X軸の角度　　 
			FW_BOLD,						// 文字の太さ
			FALSE,							// フォントがイタリックならTRUEを指定
			FALSE,							// 下線を引くならTRUE
			FALSE,							// 取り消し線を引くならTRUE
			SHIFTJIS_CHARSET,				// フォントの文字セット
			OUT_DEFAULT_PRECIS,				// 出力精度の設定
			CLIP_DEFAULT_PRECIS,			// クリッピング精度
			DEFAULT_QUALITY,				// フォントの出力品質
			VARIABLE_PITCH | FF_ROMAN,		// フォントのピッチとファミリを指定
			NULL							/* フォントのタイプフェイス名の指定*/);//フォント処理の終了

		hFont5 = CreateFont(
			30,								// フォントの高さ(大きさ)
			0,								// フォントの幅
			0,								// 水平軸の角度　反時計回りに回る
			0,								// X軸の角度　　 
			FW_BOLD,						// 文字の太さ
			FALSE,							// フォントがイタリックならTRUEを指定
			FALSE,							// 下線を引くならTRUE
			FALSE,							// 取り消し線を引くならTRUE
			SHIFTJIS_CHARSET,				// フォントの文字セット
			OUT_DEFAULT_PRECIS,				// 出力精度の設定
			CLIP_DEFAULT_PRECIS,			// クリッピング精度
			DEFAULT_QUALITY,				// フォントの出力品質
			VARIABLE_PITCH | FF_ROMAN,		// フォントのピッチとファミリを指定
			NULL							/* フォントのタイプフェイス名の指定*/);//フォント処理の終了


	  //自ターン描画
		if (ms.player == BPLAYER) {						//黒のターン
			SelectObject(hdc, hFont2);
			TextOut(hdc, 401, 0, BlackT, _tcslen(BlackT));
			SelectObject(hdc, GetStockObject(SYSTEM_FONT));	//元のフォントに戻す
			DeleteObject(hFont2);
		}

		if (ms.player == WPLAYER) {						//白のターン
			SetBkColor(hdc, RGB(0, 0, 0));			//背景色を黒に
			SetTextColor(hdc, RGB(255, 255, 255));	//文字色を白に
			SelectObject(hdc, hFont2);
			TextOut(hdc, 401, 0, WhiteT, _tcslen(WhiteT));
			SelectObject(hdc, GetStockObject(SYSTEM_FONT));
			DeleteObject(hFont2);
		}
		//石の総数と待ち時間テキストの描画
		SetBkColor(hdc, RGB(255, 255, 255));//背景色を白に
		SetTextColor(hdc, RGB(0, 0, 0));	//文字色を白に
		SelectObject(hdc, hFont5);
		TextOut(hdc, 461, 45, TotalSt, _tcslen(TotalSt));
		TextOut(hdc, 461, 133, AllottedT, _tcslen(AllottedT));
		SelectObject(hdc, GetStockObject(SYSTEM_FONT));	//元のフォントに戻す
		DeleteObject(hFont5);
		//現在コマ数の描画
		//白の総数
		SetBkColor(hdc, RGB(0, 0, 0));			//背景色を黒に
		SetTextColor(hdc, RGB(255, 255, 255));	//文字色を白に
		SelectObject(hdc, hFont3);
		wsprintf(WC, TEXT("白%d"), ms.whitecnt);
		TextOut(hdc, 570, 80, WC, _tcslen(WC));
		DeleteObject(hFont3);
		//黒の総数
		SetBkColor(hdc, RGB(255, 255, 255));
		SetTextColor(hdc, RGB(0, 0, 0));	//文字色を白に
		SelectObject(hdc, hFont3);
		wsprintf(BC, TEXT("黒%d"), ms.blackcnt);
		TextOut(hdc, 401, 80, BC, _tcslen(BC));
		SelectObject(hdc, GetStockObject(SYSTEM_FONT));	//元のフォントに戻す
		DeleteObject(hFont3);

		//操作説明の描画
		//リセット
		SetBkColor(hdc, RGB(235, 235, 235));
		SetTextColor(hdc, RGB(255, 0, 255));
		SelectObject(hdc, hFont3);
		TextOut(hdc, 401, 210, Restart, _tcslen(Restart));
		//マウス操作
		TextOut(hdc, 401, 230, Mouse, _tcslen(Mouse));
		//矢印キー操作
		TextOut(hdc, 401, 260, Yazirushi, _tcslen(Yazirushi));
		//スペースキー操作
		TextOut(hdc, 401, 290, Space, _tcslen(Space));


		//黒タイマーの描画
		SetBkColor(hdc, RGB(255, 255, 255));
		SetTextColor(hdc, RGB(0, 0, 0));
		SelectObject(hdc, hFont4);
		wsprintf(bstrCount, TEXT("%d"), bCount);
		TextOut(hdc, 406, 165, bstrCount, lstrlen(bstrCount));
		//白タイマーの描画
		SetBkColor(hdc, RGB(255, 255, 255));
		SetTextColor(hdc, RGB(0, 0, 0));	//文字色を白に
		SelectObject(hdc, hFont4);
		wsprintf(wstrCount, TEXT("%d"), wCount);
		TextOut(hdc, 575, 165, wstrCount, lstrlen(wstrCount));



		// オセロ盤の描画
		SelectObject(hdc, hBrush[0]);
		Rectangle(hdc, 0, 0, 401, 401);


		for (i = 0; i < 7; i++) {
			MoveToEx(hdc, 50 * (i + 1), 0, NULL);
			LineTo(hdc, 50 * (i + 1), 400);
			MoveToEx(hdc, 0, 50 * (i + 1), NULL);
			LineTo(hdc, 400, 50 * (i + 1));
		}

		//小さな黒丸の描画
		SelectObject(hdc, hBrush[2]);
		for (i = 0; i < 4; i++) {
			Ellipse(hdc, dot[i].x - 5, dot[i].y - 5,
				dot[i].x + 5, dot[i].y + 5);
		}

		//選択しているマスを黄色にする
		//	pt2.x = ((LOWORD(lParam)) / 50);
		//	pt2.y = ((HIWORD(lParam)) / 50);


		// おけるマスを光らせる
		for (i = 1; i <= 8; i++) {
			for (j = 1; j <= 8; j++) {
				if ((ms.masu[i][j]).m_setstone == EMP) {
					if ((ms.masu[i][j]).m_state == ms.player) {
						SelectObject(hdc, hBrush[4]);  //黄緑
						Rectangle(hdc, (i - 1) * 50, (j - 1) * 50, (i - 1) * 50 + 51, (j - 1) * 50 + 51);
					}
				}
			}
		}
		//タイトル画面の描画
		if (status == 1) {
			SelectObject(hdc, hFont1);
			SetBkColor(hdc, RGB(255, 255, 255));
			SetTextColor(hdc, RGB(0, 0, 0));
			TextOut(hdc, 120, 100, Title, _tcslen(Title));
			TextOut(hdc, 25, 250, Button, _tcslen(Button));
			SelectObject(hdc, GetStockObject(SYSTEM_FONT));	//元のフォントに戻す
			DeleteObject(hFont1);
		}
	
		SelectObject(hdc, hBrush[3]);  //黄
		Rectangle(hdc, (pt2.x - 1) * 50, (pt2.y - 1) * 50, (pt2.x - 1) * 50 + 51, (pt2.y - 1) * 50 + 51);

		// オセロ盤の表示
		for (i = 1; i <= 8; i++) {
			for (j = 1; j <= 8; j++) {
				if ((ms.masu[i][j]).m_setstone != EMP) {
					if ((ms.masu[i][j]).m_setstone == WSTONE) {
						SelectObject(hdc, hBrush[1]);  //白
					}
					else if ((ms.masu[i][j]).m_setstone == BSTONE) {
						SelectObject(hdc, hBrush[2]);  //黒
					}
					//石を描画
					Ellipse(hdc, (i - 1) * 50, (j - 1) * 50, i * 50, j * 50);
					//石を描画(ミニ)
					Ellipse(hdc, (i - 1) * 10 + 400, (j - 1) * 10 + 325, i * 10 + 400, j * 10 + 325);


				}
			}
		}
		//ポーズ描画
		if (status == 0) {
			SelectObject(hdc, hFont1);
			SetBkColor(hdc, RGB(255, 255, 255));
			SetTextColor(hdc, RGB(0, 0, 0));
			TextOut(hdc, 100, 100, Pause, _tcslen(Pause));
			SelectObject(hdc, GetStockObject(SYSTEM_FONT));	//元のフォントに戻す
			DeleteObject(hFont1);
		}
		if (status == 0) {
			SelectObject(hdc, hFont4);
			SetBkColor(hdc, RGB(255, 255, 255));
			SetTextColor(hdc, RGB(0, 0, 0));
			TextOut(hdc, 55, 250, Button, _tcslen(Button));
			SelectObject(hdc, GetStockObject(SYSTEM_FONT));	//元のフォントに戻す
			DeleteObject(hFont4);
		}
		//パステキスト描画
		if (ms.passflg == 1) {
			if (status != 3) {
				SelectObject(hdc, hFont1);
				SetBkColor(hdc, RGB(255, 255, 255));
				SetTextColor(hdc, RGB(0, 0, 0));
				TextOut(hdc, 110, 160, Pass, _tcslen(Pass));
				SelectObject(hdc, GetStockObject(SYSTEM_FONT));	//元のフォントに戻す
				DeleteObject(hFont1);
			}
			

		}
		if (status == 3) {//リザルト画面

			if(wCount == 0) { //白が全滅の場合
				ms.winner = BPLAYER;
			}

			else if (bCount == 0){ //黒が全滅の場合
				ms.winner = WPLAYER;
			}

			else if (ms.blackcnt > ms.whitecnt) {   //黒が多かったら
				ms.winner = BPLAYER;
			}
			else if(ms.whitecnt > ms.blackcnt) {//白が多かったら
				ms.winner = WPLAYER;
		    }
		    else{ //もし同じなら
			ms.winner = 0;
			}
		if (ms.winner == BPLAYER) { //黒の勝利の場合
			SelectObject(hdc, hFont1);
			SetBkColor(hdc, RGB(255, 255, 255));
			SetTextColor(hdc, RGB(0, 0, 0));
			TextOut(hdc, 50, 100, Blackwin, _tcslen(Blackwin));
			TextOut(hdc, 25, 250, Button, _tcslen(Button));
			SelectObject(hdc, GetStockObject(SYSTEM_FONT));	//元のフォントに戻す
		}
		else if (ms.winner == WPLAYER) { //白の勝利の場合
			SelectObject(hdc, hFont1);
			SetBkColor(hdc, RGB(0, 0, 0));
			SetTextColor(hdc, RGB(255, 255, 255));
			TextOut(hdc, 50, 100, Whitewin, _tcslen(Whitewin));
			TextOut(hdc, 25, 250, Button, _tcslen(Button));
			SelectObject(hdc, GetStockObject(SYSTEM_FONT));	//元のフォントに戻す
		}
		else { //もし同じなら
			SelectObject(hdc, hFont1);
			SetBkColor(hdc, RGB(255, 255, 255));
			SetTextColor(hdc, RGB(0, 0, 0));
			TextOut(hdc, 50, 100, Draw, _tcslen(Draw));
			TextOut(hdc, 25, 250, Button, _tcslen(Button));
			SelectObject(hdc, GetStockObject(SYSTEM_FONT));//引き分けと表示
		}
	
		}return 0;
		EndPaint(hWnd, &ps);

		return 0;

	} // switch文の終了


	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

/*******************************************************************************
関数名 : WinMain
機能   : ゲームの初期化とメッセージループ
引数   : 省略
返却値 : 省略
*******************************************************************************/

int WINAPI WinMain(
	HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR lpCmdLine, int nCmdShow
) {
	WNDCLASS wc;
	MSG msg;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;  //WindowProcedureの関数名を指定する
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_BACKGROUND + 1;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = APP_NAME;

	if (!RegisterClass(&wc)) return 0;

	if (CreateWindow(
		APP_NAME, TEXT(__FILE__),
		WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT,
		690, 450, // ここで、ウィンドウのサイズ（Width,Height)を指定する 900
		NULL, NULL, hInstance, NULL
	) == NULL) return 0;

	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

/*******************************************************************************
関数名 check 担当者 :益子
機能 すべてのマスを石が置けるかどうか判断し、判断結果をmasu[i][j].m_stateに格納する
引数：&ms　MASU (i/o)  //i =入力　o =出力
返却値　なし
*******************************************************************************/
int check(GAMEMASTER *ms) {
	int i, j, k;
	int flg = 0;
	int wayno = 0;
	//番ではないほうのプレイヤー
	int anplayer = ms->player * REVERSE;

	//現在参照している座標からこれから参照する座標までの差分
	POINT vect;
	vect.x = 0;
	vect.y = 0;

	/*探索する方角(ベクトル)を表す変数
	　   上
		7 0 1
		左6 　2右
		5 4 3
		下
		*/
	POINT way[8];

	way[0].x = 0;
	way[0].y = -1;

	way[1].x = +1;
	way[1].y = -1;

	way[2].x = +1;
	way[2].y = 0;

	way[3].x = +1;
	way[3].y = +1;

	way[4].x = 0;
	way[4].y = +1;

	way[5].x = -1;
	way[5].y = +1;

	way[6].x = -1;
	way[6].y = 0;

	way[7].x = -1;
	way[7].y = -1;

	for (i = 1; i <= 8; i++) {                                                               //すべてのマスを調べる
		for (j = 1; j <= 8; j++) {
			ms->masu[i][j].m_state = EMP;
		}
	}
	for (i = 1; i <= 8; i++) {                                                               //すべてのマスを調べる
		for (j = 1; j <= 8; j++) {
			if ((ms->masu[i][j]).m_setstone == EMP) {                                        //石が置いてあるマスは判定しない
				for (wayno = 0; wayno < 8; wayno++) {                                        //調べるマスを中心に8方向のマスを確認する
					vect.x = way[wayno].x;                                                   //確認する方角(ベクトル)を代入する
					vect.y = way[wayno].y;
					if (ms->masu[i + vect.x][j + vect.y].m_setstone == anplayer) {           //確認したマスに相手の石がある場合
						for (k = 0; k < 6; k++) {  //最大6回
							vect.x += way[wayno].x;                                          //確認した方角へさらに隣のマスを調べる
							vect.y += way[wayno].y;
							if (ms->masu[i + vect.x][j + vect.y].m_setstone == ms->player) { //調べたマスに自分の石がある場合
								(ms->masu[i][j]).m_state = ms->player;                       //マスを石が置ける状態にする
								flg = 1;
								break;
							}
							if (ms->masu[i + vect.x][j + vect.y].m_setstone == EMP) {         //調べたマスに石がない場合
								break;                                                       //何もせずfor文を抜ける
							}

						}
					}
				}
				vect.x = 0;                                                                  //ベクトルを初期化する
				vect.y = 0;

			}
		}
	}
	if (flg == 0) {
		return 0;
	}
	if (flg == 1) {
		return 1;
	}
}

void reverse(GAMEMASTER *ms, POINT cp) {
	int i, j;
	int wayno = 0;
	//番ではないほうのプレイヤー
	int anplayer = ms->player * REVERSE;

	//現在参照している座標からこれから参照する座標までの差分
	POINT vect;
	vect.x = 0;
	vect.y = 0;

	/*探索する方角(ベクトル)を表す変数
	　   上
		7 0 1
		左6 　2右
		5 4 3
		下
		*/
	POINT way[8];

	way[0].x = 0;
	way[0].y = -1;

	way[1].x = +1;
	way[1].y = -1;

	way[2].x = +1;
	way[2].y = 0;

	way[3].x = +1;
	way[3].y = +1;

	way[4].x = 0;
	way[4].y = +1;

	way[5].x = -1;
	way[5].y = +1;

	way[6].x = -1;
	way[6].y = 0;

	way[7].x = -1;
	way[7].y = -1;


	if ((ms->masu[cp.x][cp.y]).m_setstone != EMP) {                                        //石が置いてあるマスは判定しない
		for (wayno = 0; wayno < 8; wayno++) {                                        //調べるマスを中心に8方向のマスを確認する
			vect.x = way[wayno].x;                                                   //確認する方角(ベクトル)を代入する
			vect.y = way[wayno].y;
			if (ms->masu[cp.x + vect.x][cp.y + vect.y].m_setstone == anplayer) {           //確認したマスに相手の石がある場合
				for (i = 0; i < 6; i++) {  //最大6回
					vect.x += way[wayno].x;                                          //確認した方角へさらに隣のマスを調べる
					vect.y += way[wayno].y;
					if (ms->masu[cp.x + vect.x][cp.y + vect.y].m_setstone == ms->player) { //調べたマスに自分の石がある場合
						for (j = 0; j < 6; j++) {  //最大6回
							vect.x -= way[wayno].x;                                      //ひとつ前に戻る
							vect.y -= way[wayno].y;
							if (ms->masu[cp.x + vect.x][cp.y + vect.y].m_setstone == anplayer) {
								ms->masu[cp.x + vect.x][cp.y + vect.y].m_setstone *= REVERSE;//石をひっくり返す
								if (ms->player == BSTONE) {
									ms->blackcnt++;//マス上にある黒の石の総数
									ms->whitecnt--;
								}
								else {
									ms->blackcnt--;//マス上にある黒の石の総数
									ms->whitecnt++;
								}
							}
							else {
								break;
							}
						}
					}
					if (ms->masu[cp.x + vect.x][cp.y + vect.y].m_setstone == EMP) {         //調べたマスに石がない場合
						break;                                                       //何もせずfor文を抜ける
					}

				}
			}
		}
		vect.x = 0;                                                                  //ベクトルを初期化する
		vect.y = 0;

	}
}
//コピペ用デバックメッセージ
//MessageBox(NULL , TEXT(" MessageBox ") ,TEXT("デバック"), MB_OK);

//コピペ用関数説明文
/*
関数名
機能
引数：変数名　int (i)//i =入力　o =出力
返却値　NG：おけない
1：白は置ける
　　　　 2：黒は置ける
	 */