#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>

#define APP_NAME TEXT("reversi")

#define BLACK        1  // �\���F�@��
#define WHITE        2  // �\���F�@��
#define GREEN        3  // �\���F�@��
#define RED          4  // �\���F�@��

#define BSTONE          1 //����
#define WSTONE          -1  //����
#define BPLAYER           1 //���v���C���[
#define WPLAYER           -1 //���v���C���[

#define REVERSE     -1  //�Ђ�����Ԃ��Ƃ��Ɏg���萔�@

#define EMP          0 // ��

/* �Q�[���̏��
1:����N�����i�������j
2:�v���C��
3:���U���g�\����*/
int status = 1;


//�@�}�X����̃f�[�^
typedef struct _masu {
	/*�}�X�̏�Ԃ�\���萔
	0:�����Ȃ�
	1:BLACK
	-1:WHITE */
	int  m_setstone;

	/*�}�X�ɐ΂��u���邩�ǂ����\���萔
	0:�����Ȃ�
	1:�����u����
	-1:�����u����*/
	int  m_state;

}MASU;

//�@�Q�[���S�̂̃f�[�^
typedef struct _game {
	// masu�͊O�����������邱�Ƃ���8x8�ł͂Ȃ��A10x10�Ƃ��Ă���
	// ���������āA�L���͈͂́A1-8�܂ł̈ʒu�ƂȂ�
	MASU masu[10][10];

	/*���݂̔Ԃ̃v���C���[
	���� : 1
	���� : -1*/

	int blackcnt;//�}�X��ɂ��鍕�̐΂̑���
	int whitecnt;//�}�X��ɂ��锒�̐΂̑���

	int passflg;
	int player;
	int winner;
}GAMEMASTER;

//�N���b�N�������W
POINT pt;

//�I�������}�X���W
POINT cp;

//���点��}�X�̍��W
POINT pt2;

/*******************************************************************************
�֐��� check �S���� :�v�q
�@�\ ���ׂẴ}�X��΂��u���邩�ǂ������f���A���f���ʂ�masu[i][j].m_state�Ɋi�[����
�����F&ms�@MASU (i/o)  //i =���́@o =�o��
�ԋp�l�@�Ȃ�
*******************************************************************************/
int check(GAMEMASTER *);
/*******************************************************************************
�֐��� reverse �S���� :�Ɠc
�@�\ �I�������}�X����΂��Ԃ��邩�ǂ������f���A���f���ʂ�masu[i][j].m_setstone�Ɋi�[����
�����F&ms�@GAMEMASTER ,POINT cp
�ԋp�l�@�Ȃ�
*******************************************************************************/
void reverse(GAMEMASTER *, POINT);
/*******************************************************************************
�֐��� : WindowProc
�@�\   : ���b�Z�[�W�������s��
����   : �ȗ�
�ԋp�l : �ȗ�
*******************************************************************************/
LRESULT CALLBACK WindowProc(
	HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam
) {


	HDC hdc;
	//�^�C�g�����
	TCHAR Title[] = _T("�I�Z��");
	TCHAR Button[] = _T("�E�N���b�NorEnter");
	//�������
	TCHAR Restart[] = _T("�|�[�Y(�ꎞ��~):Enter���E�N���b�N");
	TCHAR Mouse[] = _T("���N���b�N:�R�}��u��");
	TCHAR Yazirushi[] = _T("���L�[:�Ֆʈړ�");
	TCHAR Space[] = _T("�X�y�[�X�L�[:�R�}��u��");
	//�|�[�Y�e�L�X�g
	TCHAR Pause[] = _T("�|�[�Y��");
	//��������
	TCHAR AllottedT[] = _T("��������");
	//�΂̑���
	TCHAR TotalSt[] = _T("�΂̑���");
	//���^�C�}�[
	static int bCount;
	static TCHAR bstrCount[64];
	//���^�C�}�[
	static int wCount;
	static TCHAR wstrCount[64];
	//�����̃^�[��
	TCHAR BlackT[] = _T("���̃^�[���ł�");
	TCHAR WhiteT[] = _T("���̃^�[���ł�");
	//�p�X
	TCHAR Pass[] = _T("�p�X���܂���");
	//�΂̑������J�E���g����
	TCHAR BC[10];//���̑���
	TCHAR WC[10];//���̑���
	TCHAR Draw[] = _T("���������ł�");
	TCHAR Blackwin[] = _T("���̏����ł�");
	TCHAR Whitewin[] = _T("���̏����ł�");
	//�t�H���g
	HFONT hFont1; //�^�C�g����ʗp�t�H���g
	HFONT hFont2; //�^�[���`��p
	HFONT hFont3; //���݃R�}�`��p
	HFONT hFont4; //��������p
	HFONT hFont5; //�ꕔ�e�L�X�g�p(�΂̑����A�҂�����)
	//�`��p�X
	PAINTSTRUCT ps;

	static BOOL blRight = TRUE;
	static HBRUSH hBrush[3];
	int i, j, x = 0;

	static GAMEMASTER ms;

	// �Ֆʏ�̏��������ۂ̍��W�_�i�`�掞�̊e���S���W�j
	static POINT dot[4] = {
	{ 100,100 },
	{ 300,100 },
	{ 100,300 },
	{ 300,300 }
	};

	//�Q�[���J�n�Ɉ�x�����s������/////////////////////////////
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
		//masu�����ׂĐ΂��u���ĂȂ���Ԃɂ���
		for (i = 0; i < 10; i++) {
			for (j = 0; j < 10; j++) {
				(ms.masu[i][j]).m_setstone = 0;
			}
		}

		//����4�}�X�ɐ΂�z�u����

		(ms.masu[4][5]).m_setstone = BSTONE;
		(ms.masu[5][4]).m_setstone = BSTONE;
		(ms.masu[4][4]).m_setstone = WSTONE;
		(ms.masu[5][5]).m_setstone = WSTONE;

		ms.blackcnt = 2;//�}�X��ɂ��鍕�̐΂̑���
		ms.whitecnt = 2;//�}�X��ɂ��锒�̐΂̑���

						/*
						�֐��� check �S���� :�v�q
						�@�\ ���ׂẴ}�X��΂��u���邩�ǂ������f���A���f���ʂ�masu[i][j].m_state�Ɋi�[����
						�����F&ms�@MASU (i/o)  //i =���́@o =�o��
						�ԋp�l�@0:��ؒu���Ȃ��@1:�Œ�ꂩ���u����
						*/
		check(&ms);

		hBrush[4] = CreateSolidBrush(RGB(0xAA, 0xEE, 0));          // ����
																   //�v���C��Ԃɂ���
	}

	switch (uMsg) {

	case WM_CREATE:  // �����������̏����i�������j

					 //�^�C�}�[�̒�`
		SetTimer(hWnd, 1, 1000, NULL);

		//�u���V�f�[�^�̒�`
		hBrush[0] = CreateSolidBrush(RGB(0, 0xAA, 0));          // �Ֆʁi�΁j
		hBrush[1] = CreateSolidBrush(RGB(0xFF, 0xFF, 0xFF));    // WHITE
		hBrush[2] = CreateSolidBrush(RGB(0, 0, 0));		        // BLACK
		hBrush[3] = CreateSolidBrush(RGB(0xEE, 0xEE, 0));		// ���F
		hBrush[4] = CreateSolidBrush(RGB(0xAA, 0xEE, 0));       // ����

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

	case WM_DESTROY:  // �E�B���h�E�j�����̏���


					  // �u���V�̍폜
		for (i = 0; i < 2; i++) DeleteObject(hBrush[i]);
		PostQuitMessage(0);
		return 0;
	case WM_RBUTTONDOWN:  //�@�E�N���b�N�����Ƃ��̃C�x���g(������)
		if (status == 1) {
			status = 2;
			InvalidateRect(hWnd, NULL, TRUE);
			return 0;
		}
		//�ꎞ��~����{
		//���̃^�[����
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

		//���̃^�[����{
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

	case WM_LBUTTONDOWN:  //�@���N���b�N�����Ƃ��̃C�x���g 
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);

		if (ms.passflg == 1) {
			ms.passflg = 0;
		}


		if ((pt.x < 0) || (pt.x > 400) ||//�̈�O���N���b�N���Ă��������Ȃ�
			(pt.y < 0) || (pt.y > 400)) {
			return 0;
		}

		//�N���b�N�������W���}�X���W�֕ϊ�����
		cp.x = ((pt.x) / 50) + 1;
		cp.y = ((pt.y) / 50) + 1;

		//�΂��u���Ă���ꏊ��I�����Ă��������Ȃ�
		if (status == 0 || (ms.masu[cp.x][cp.y]).m_setstone != 0)return 0;

		//�΂��Ԃ��Ȃ��ꏊ��I�����Ă��������Ȃ�
		if (status == 0 || ( ms.masu[cp.x][cp.y]).m_state != ms.player)return 0;
		//�N���b�N�����ꏊ�ɐ΂�ǉ�����
		if (ms.player == BLACK) {
			(ms.masu[cp.x][cp.y]).m_setstone = BSTONE;
			ms.blackcnt++;//�}�X��ɂ��鍕�̐΂̑���

		}
		else{
			(ms.masu[cp.x][cp.y]).m_setstone = WSTONE;
			ms.whitecnt++;//�}�X��ɂ��锒�̐΂̑���
		}

		/*
		�֐���:�@�@�S���� :�Ɠc
		�@�\:�΂�u�����Ƃ��ɂЂ�����Ԃ�����
		�����F�ϐ����@&masu�@MASU (i/o) ,cp.x+1 int(i) ,cp.y+1(i) int ,player int(i)

		�����Ӗ�
		&masu         :�}�X���
		cp.x+1,cp.y+1 :�I�������}�X���W
		player        :�ǂ���̔Ԃ��\���ϐ�

		�ԋp�l�F����
		���l�@�F�֐����Ő΂̑�����ϓ������鏈�����s��
		*/
		reverse(&ms, cp);
		ms.player *= REVERSE;

		//�֐�check���Ăяo��
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
		//�v���C��Ԃɂ���
		status = 2;
		}
		*/
		/*���U���g����
		�@�@  �@  ->�΂̑�����\��
			   �@�@�@�@  ���s�\��
					 �@�@�@  �@�G���^�[�L�[�����N���b�N��
						   �@�@�@�@  ���������ă��X�^�[�g���鏈���@*/
		InvalidateRect(hWnd, NULL, TRUE); //�ĕ`�悳���閽�߁i�E�C���h�E�̃n���h��,�����`�̍��W(NULL�őS��),TRUE�Ŕw�i��������������j

	case WM_KEYDOWN:  //�@�L�[���������Ƃ��̃C�x���g�@�S����:�ɓ�

		if (wParam == VK_RETURN) {   //�G���^�[�L�[�������ꂽ�珉����
			if (status == 1) {
				status = 2;
				InvalidateRect(hWnd, NULL, TRUE);
				return 0;
			}
			//�ꎞ��~����{
			//���̃^�[����{
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

			//���̃^�[����{
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
		if (wParam == VK_RIGHT) pt2.x++;  //�E��
		if (pt2.x > 8) {
			pt2.x = 1;
		}
		if (wParam == VK_LEFT) pt2.x--; //����
		if (pt2.x < 1) {
			pt2.x = 8;
		}
		if (wParam == VK_UP) pt2.y--; //�な 
		if (pt2.y < 1) {
			pt2.y = 8;
		}
		if (wParam == VK_DOWN) pt2.y++;  //����   
		if (pt2.y > 8) {
			pt2.y = 1;
		}

		if (wParam == VK_SPACE) {//�X�y�[�X�L�[�������ꂽ��
			if ((ms.masu[pt2.x][pt2.y]).m_setstone != 0)return 0;

			//�΂��Ԃ��Ȃ��ꏊ��I�����Ă��������Ȃ�
			if ((ms.masu[pt2.x][pt2.y]).m_state != ms.player)return 0;

			if (ms.player == BLACK) {//�v���C���[�����Ȃ�
				(ms.masu[pt2.x][pt2.y]).m_setstone = BSTONE;
				ms.blackcnt++;//�}�X��ɂ��鍕�̐΂̑���
			}
			else {//�v���C���[�����Ȃ�
				(ms.masu[pt2.x][pt2.y]).m_setstone = WSTONE;
				ms.whitecnt++;//�}�X��ɂ��锒�̐΂̑���
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
		//�΂�u���ꏊ���L�[�{�[�h���g���đI�����鏈�������

		/*wParam�ɃL�[�R�[�h�̒l������

		�@�@vk_LEFT  vk_SPACE
		  vk_RIGHT vk_UP
		  vk_DOWN*/

		  //�I�����Ă���}�X�����点�鏈�������i���F�j

		InvalidateRect(hWnd, NULL, TRUE); //�ĕ`�悳���閽�߁i�E�C���h�E�̃n���h��,�����`�̍��W(NULL�őS��),TRUE�Ŕw�i��������������j

	case WM_PAINT: // �`��C�x���g�������@�@�i��ʂɕω����������ۂɕp�ɂɌĂяo�����)

				   //�ǂ���̔Ԃ��\�����鏈���@�S���� :�R�{

				   //�������ԁi�v���C���[���Ɓj�S���� :�R�{

				   //�u�����Ƃ��ł���}�X�����点�鏈��

				   // �y�C���g�J�n
		hdc = BeginPaint(hWnd, &ps);
		hFont1 = CreateFont(
			50,							// �t�H���g�̍���(�傫��)
			0,								// �t�H���g�̕�
			0,								// �������̊p�x�@�����v���ɉ��
			0,								// X���̊p�x�@�@ 
			FW_BOLD,						// �����̑���
			FALSE,							// �t�H���g���C�^���b�N�Ȃ�TRUE���w��
			FALSE,							// �����������Ȃ�TRUE
			FALSE,							// ���������������Ȃ�TRUE
			SHIFTJIS_CHARSET,				// �t�H���g�̕����Z�b�g
			OUT_DEFAULT_PRECIS,				// �o�͐��x�̐ݒ�
			CLIP_DEFAULT_PRECIS,			// �N���b�s���O���x
			DEFAULT_QUALITY,				// �t�H���g�̏o�͕i��
			VARIABLE_PITCH | FF_ROMAN,		// �t�H���g�̃s�b�`�ƃt�@�~�����w��
			NULL							/* �t�H���g�̃^�C�v�t�F�C�X���̎w��*/);

		hFont2 = CreateFont(
			40,								// �t�H���g�̍���(�傫��)
			0,								// �t�H���g�̕�
			0,								// �������̊p�x�@�����v���ɉ��
			0,								// X���̊p�x�@�@ 
			FW_BOLD,						// �����̑���
			FALSE,							// �t�H���g���C�^���b�N�Ȃ�TRUE���w��
			FALSE,							// �����������Ȃ�TRUE
			FALSE,							// ���������������Ȃ�TRUE
			SHIFTJIS_CHARSET,				// �t�H���g�̕����Z�b�g
			OUT_DEFAULT_PRECIS,				// �o�͐��x�̐ݒ�
			CLIP_DEFAULT_PRECIS,			// �N���b�s���O���x
			DEFAULT_QUALITY,				// �t�H���g�̏o�͕i��
			VARIABLE_PITCH | FF_ROMAN,		// �t�H���g�̃s�b�`�ƃt�@�~�����w��
			NULL							/* �t�H���g�̃^�C�v�t�F�C�X���̎w��*/);

		hFont3 = CreateFont(
			50,								// �t�H���g�̍���(�傫��)
			0,								// �t�H���g�̕�
			0,								// �������̊p�x�@�����v���ɉ��
			0,								// X���̊p�x�@�@ 
			FW_BOLD,						// �����̑���
			FALSE,							// �t�H���g���C�^���b�N�Ȃ�TRUE���w��
			FALSE,							// �����������Ȃ�TRUE
			FALSE,							// ���������������Ȃ�TRUE
			SHIFTJIS_CHARSET,				// �t�H���g�̕����Z�b�g
			OUT_DEFAULT_PRECIS,				// �o�͐��x�̐ݒ�
			CLIP_DEFAULT_PRECIS,			// �N���b�s���O���x
			DEFAULT_QUALITY,				// �t�H���g�̏o�͕i��
			VARIABLE_PITCH | FF_ROMAN,		// �t�H���g�̃s�b�`�ƃt�@�~�����w��
			NULL							/* �t�H���g�̃^�C�v�t�F�C�X���̎w��*/);//�t�H���g�����̏I��

		hFont4 = CreateFont(
			40,								// �t�H���g�̍���(�傫��)
			0,								// �t�H���g�̕�
			0,								// �������̊p�x�@�����v���ɉ��
			0,								// X���̊p�x�@�@ 
			FW_BOLD,						// �����̑���
			FALSE,							// �t�H���g���C�^���b�N�Ȃ�TRUE���w��
			FALSE,							// �����������Ȃ�TRUE
			FALSE,							// ���������������Ȃ�TRUE
			SHIFTJIS_CHARSET,				// �t�H���g�̕����Z�b�g
			OUT_DEFAULT_PRECIS,				// �o�͐��x�̐ݒ�
			CLIP_DEFAULT_PRECIS,			// �N���b�s���O���x
			DEFAULT_QUALITY,				// �t�H���g�̏o�͕i��
			VARIABLE_PITCH | FF_ROMAN,		// �t�H���g�̃s�b�`�ƃt�@�~�����w��
			NULL							/* �t�H���g�̃^�C�v�t�F�C�X���̎w��*/);//�t�H���g�����̏I��

		hFont5 = CreateFont(
			30,								// �t�H���g�̍���(�傫��)
			0,								// �t�H���g�̕�
			0,								// �������̊p�x�@�����v���ɉ��
			0,								// X���̊p�x�@�@ 
			FW_BOLD,						// �����̑���
			FALSE,							// �t�H���g���C�^���b�N�Ȃ�TRUE���w��
			FALSE,							// �����������Ȃ�TRUE
			FALSE,							// ���������������Ȃ�TRUE
			SHIFTJIS_CHARSET,				// �t�H���g�̕����Z�b�g
			OUT_DEFAULT_PRECIS,				// �o�͐��x�̐ݒ�
			CLIP_DEFAULT_PRECIS,			// �N���b�s���O���x
			DEFAULT_QUALITY,				// �t�H���g�̏o�͕i��
			VARIABLE_PITCH | FF_ROMAN,		// �t�H���g�̃s�b�`�ƃt�@�~�����w��
			NULL							/* �t�H���g�̃^�C�v�t�F�C�X���̎w��*/);//�t�H���g�����̏I��


	  //���^�[���`��
		if (ms.player == BPLAYER) {						//���̃^�[��
			SelectObject(hdc, hFont2);
			TextOut(hdc, 401, 0, BlackT, _tcslen(BlackT));
			SelectObject(hdc, GetStockObject(SYSTEM_FONT));	//���̃t�H���g�ɖ߂�
			DeleteObject(hFont2);
		}

		if (ms.player == WPLAYER) {						//���̃^�[��
			SetBkColor(hdc, RGB(0, 0, 0));			//�w�i�F������
			SetTextColor(hdc, RGB(255, 255, 255));	//�����F�𔒂�
			SelectObject(hdc, hFont2);
			TextOut(hdc, 401, 0, WhiteT, _tcslen(WhiteT));
			SelectObject(hdc, GetStockObject(SYSTEM_FONT));
			DeleteObject(hFont2);
		}
		//�΂̑����Ƒ҂����ԃe�L�X�g�̕`��
		SetBkColor(hdc, RGB(255, 255, 255));//�w�i�F�𔒂�
		SetTextColor(hdc, RGB(0, 0, 0));	//�����F�𔒂�
		SelectObject(hdc, hFont5);
		TextOut(hdc, 461, 45, TotalSt, _tcslen(TotalSt));
		TextOut(hdc, 461, 133, AllottedT, _tcslen(AllottedT));
		SelectObject(hdc, GetStockObject(SYSTEM_FONT));	//���̃t�H���g�ɖ߂�
		DeleteObject(hFont5);
		//���݃R�}���̕`��
		//���̑���
		SetBkColor(hdc, RGB(0, 0, 0));			//�w�i�F������
		SetTextColor(hdc, RGB(255, 255, 255));	//�����F�𔒂�
		SelectObject(hdc, hFont3);
		wsprintf(WC, TEXT("��%d"), ms.whitecnt);
		TextOut(hdc, 570, 80, WC, _tcslen(WC));
		DeleteObject(hFont3);
		//���̑���
		SetBkColor(hdc, RGB(255, 255, 255));
		SetTextColor(hdc, RGB(0, 0, 0));	//�����F�𔒂�
		SelectObject(hdc, hFont3);
		wsprintf(BC, TEXT("��%d"), ms.blackcnt);
		TextOut(hdc, 401, 80, BC, _tcslen(BC));
		SelectObject(hdc, GetStockObject(SYSTEM_FONT));	//���̃t�H���g�ɖ߂�
		DeleteObject(hFont3);

		//��������̕`��
		//���Z�b�g
		SetBkColor(hdc, RGB(235, 235, 235));
		SetTextColor(hdc, RGB(255, 0, 255));
		SelectObject(hdc, hFont3);
		TextOut(hdc, 401, 210, Restart, _tcslen(Restart));
		//�}�E�X����
		TextOut(hdc, 401, 230, Mouse, _tcslen(Mouse));
		//���L�[����
		TextOut(hdc, 401, 260, Yazirushi, _tcslen(Yazirushi));
		//�X�y�[�X�L�[����
		TextOut(hdc, 401, 290, Space, _tcslen(Space));


		//���^�C�}�[�̕`��
		SetBkColor(hdc, RGB(255, 255, 255));
		SetTextColor(hdc, RGB(0, 0, 0));
		SelectObject(hdc, hFont4);
		wsprintf(bstrCount, TEXT("%d"), bCount);
		TextOut(hdc, 406, 165, bstrCount, lstrlen(bstrCount));
		//���^�C�}�[�̕`��
		SetBkColor(hdc, RGB(255, 255, 255));
		SetTextColor(hdc, RGB(0, 0, 0));	//�����F�𔒂�
		SelectObject(hdc, hFont4);
		wsprintf(wstrCount, TEXT("%d"), wCount);
		TextOut(hdc, 575, 165, wstrCount, lstrlen(wstrCount));



		// �I�Z���Ղ̕`��
		SelectObject(hdc, hBrush[0]);
		Rectangle(hdc, 0, 0, 401, 401);


		for (i = 0; i < 7; i++) {
			MoveToEx(hdc, 50 * (i + 1), 0, NULL);
			LineTo(hdc, 50 * (i + 1), 400);
			MoveToEx(hdc, 0, 50 * (i + 1), NULL);
			LineTo(hdc, 400, 50 * (i + 1));
		}

		//�����ȍ��ۂ̕`��
		SelectObject(hdc, hBrush[2]);
		for (i = 0; i < 4; i++) {
			Ellipse(hdc, dot[i].x - 5, dot[i].y - 5,
				dot[i].x + 5, dot[i].y + 5);
		}

		//�I�����Ă���}�X�����F�ɂ���
		//	pt2.x = ((LOWORD(lParam)) / 50);
		//	pt2.y = ((HIWORD(lParam)) / 50);


		// ������}�X�����点��
		for (i = 1; i <= 8; i++) {
			for (j = 1; j <= 8; j++) {
				if ((ms.masu[i][j]).m_setstone == EMP) {
					if ((ms.masu[i][j]).m_state == ms.player) {
						SelectObject(hdc, hBrush[4]);  //����
						Rectangle(hdc, (i - 1) * 50, (j - 1) * 50, (i - 1) * 50 + 51, (j - 1) * 50 + 51);
					}
				}
			}
		}
		//�^�C�g����ʂ̕`��
		if (status == 1) {
			SelectObject(hdc, hFont1);
			SetBkColor(hdc, RGB(255, 255, 255));
			SetTextColor(hdc, RGB(0, 0, 0));
			TextOut(hdc, 120, 100, Title, _tcslen(Title));
			TextOut(hdc, 25, 250, Button, _tcslen(Button));
			SelectObject(hdc, GetStockObject(SYSTEM_FONT));	//���̃t�H���g�ɖ߂�
			DeleteObject(hFont1);
		}
	
		SelectObject(hdc, hBrush[3]);  //��
		Rectangle(hdc, (pt2.x - 1) * 50, (pt2.y - 1) * 50, (pt2.x - 1) * 50 + 51, (pt2.y - 1) * 50 + 51);

		// �I�Z���Ղ̕\��
		for (i = 1; i <= 8; i++) {
			for (j = 1; j <= 8; j++) {
				if ((ms.masu[i][j]).m_setstone != EMP) {
					if ((ms.masu[i][j]).m_setstone == WSTONE) {
						SelectObject(hdc, hBrush[1]);  //��
					}
					else if ((ms.masu[i][j]).m_setstone == BSTONE) {
						SelectObject(hdc, hBrush[2]);  //��
					}
					//�΂�`��
					Ellipse(hdc, (i - 1) * 50, (j - 1) * 50, i * 50, j * 50);
					//�΂�`��(�~�j)
					Ellipse(hdc, (i - 1) * 10 + 400, (j - 1) * 10 + 325, i * 10 + 400, j * 10 + 325);


				}
			}
		}
		//�|�[�Y�`��
		if (status == 0) {
			SelectObject(hdc, hFont1);
			SetBkColor(hdc, RGB(255, 255, 255));
			SetTextColor(hdc, RGB(0, 0, 0));
			TextOut(hdc, 100, 100, Pause, _tcslen(Pause));
			SelectObject(hdc, GetStockObject(SYSTEM_FONT));	//���̃t�H���g�ɖ߂�
			DeleteObject(hFont1);
		}
		if (status == 0) {
			SelectObject(hdc, hFont4);
			SetBkColor(hdc, RGB(255, 255, 255));
			SetTextColor(hdc, RGB(0, 0, 0));
			TextOut(hdc, 55, 250, Button, _tcslen(Button));
			SelectObject(hdc, GetStockObject(SYSTEM_FONT));	//���̃t�H���g�ɖ߂�
			DeleteObject(hFont4);
		}
		//�p�X�e�L�X�g�`��
		if (ms.passflg == 1) {
			if (status != 3) {
				SelectObject(hdc, hFont1);
				SetBkColor(hdc, RGB(255, 255, 255));
				SetTextColor(hdc, RGB(0, 0, 0));
				TextOut(hdc, 110, 160, Pass, _tcslen(Pass));
				SelectObject(hdc, GetStockObject(SYSTEM_FONT));	//���̃t�H���g�ɖ߂�
				DeleteObject(hFont1);
			}
			

		}
		if (status == 3) {//���U���g���

			if(wCount == 0) { //�����S�ł̏ꍇ
				ms.winner = BPLAYER;
			}

			else if (bCount == 0){ //�����S�ł̏ꍇ
				ms.winner = WPLAYER;
			}

			else if (ms.blackcnt > ms.whitecnt) {   //��������������
				ms.winner = BPLAYER;
			}
			else if(ms.whitecnt > ms.blackcnt) {//��������������
				ms.winner = WPLAYER;
		    }
		    else{ //���������Ȃ�
			ms.winner = 0;
			}
		if (ms.winner == BPLAYER) { //���̏����̏ꍇ
			SelectObject(hdc, hFont1);
			SetBkColor(hdc, RGB(255, 255, 255));
			SetTextColor(hdc, RGB(0, 0, 0));
			TextOut(hdc, 50, 100, Blackwin, _tcslen(Blackwin));
			TextOut(hdc, 25, 250, Button, _tcslen(Button));
			SelectObject(hdc, GetStockObject(SYSTEM_FONT));	//���̃t�H���g�ɖ߂�
		}
		else if (ms.winner == WPLAYER) { //���̏����̏ꍇ
			SelectObject(hdc, hFont1);
			SetBkColor(hdc, RGB(0, 0, 0));
			SetTextColor(hdc, RGB(255, 255, 255));
			TextOut(hdc, 50, 100, Whitewin, _tcslen(Whitewin));
			TextOut(hdc, 25, 250, Button, _tcslen(Button));
			SelectObject(hdc, GetStockObject(SYSTEM_FONT));	//���̃t�H���g�ɖ߂�
		}
		else { //���������Ȃ�
			SelectObject(hdc, hFont1);
			SetBkColor(hdc, RGB(255, 255, 255));
			SetTextColor(hdc, RGB(0, 0, 0));
			TextOut(hdc, 50, 100, Draw, _tcslen(Draw));
			TextOut(hdc, 25, 250, Button, _tcslen(Button));
			SelectObject(hdc, GetStockObject(SYSTEM_FONT));//���������ƕ\��
		}
	
		}return 0;
		EndPaint(hWnd, &ps);

		return 0;

	} // switch���̏I��


	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

/*******************************************************************************
�֐��� : WinMain
�@�\   : �Q�[���̏������ƃ��b�Z�[�W���[�v
����   : �ȗ�
�ԋp�l : �ȗ�
*******************************************************************************/

int WINAPI WinMain(
	HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR lpCmdLine, int nCmdShow
) {
	WNDCLASS wc;
	MSG msg;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;  //WindowProcedure�̊֐������w�肷��
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
		690, 450, // �����ŁA�E�B���h�E�̃T�C�Y�iWidth,Height)���w�肷�� 900
		NULL, NULL, hInstance, NULL
	) == NULL) return 0;

	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

/*******************************************************************************
�֐��� check �S���� :�v�q
�@�\ ���ׂẴ}�X��΂��u���邩�ǂ������f���A���f���ʂ�masu[i][j].m_state�Ɋi�[����
�����F&ms�@MASU (i/o)  //i =���́@o =�o��
�ԋp�l�@�Ȃ�
*******************************************************************************/
int check(GAMEMASTER *ms) {
	int i, j, k;
	int flg = 0;
	int wayno = 0;
	//�Ԃł͂Ȃ��ق��̃v���C���[
	int anplayer = ms->player * REVERSE;

	//���ݎQ�Ƃ��Ă�����W���炱�ꂩ��Q�Ƃ�����W�܂ł̍���
	POINT vect;
	vect.x = 0;
	vect.y = 0;

	/*�T��������p(�x�N�g��)��\���ϐ�
	�@   ��
		7 0 1
		��6 �@2�E
		5 4 3
		��
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

	for (i = 1; i <= 8; i++) {                                                               //���ׂẴ}�X�𒲂ׂ�
		for (j = 1; j <= 8; j++) {
			ms->masu[i][j].m_state = EMP;
		}
	}
	for (i = 1; i <= 8; i++) {                                                               //���ׂẴ}�X�𒲂ׂ�
		for (j = 1; j <= 8; j++) {
			if ((ms->masu[i][j]).m_setstone == EMP) {                                        //�΂��u���Ă���}�X�͔��肵�Ȃ�
				for (wayno = 0; wayno < 8; wayno++) {                                        //���ׂ�}�X�𒆐S��8�����̃}�X���m�F����
					vect.x = way[wayno].x;                                                   //�m�F������p(�x�N�g��)��������
					vect.y = way[wayno].y;
					if (ms->masu[i + vect.x][j + vect.y].m_setstone == anplayer) {           //�m�F�����}�X�ɑ���̐΂�����ꍇ
						for (k = 0; k < 6; k++) {  //�ő�6��
							vect.x += way[wayno].x;                                          //�m�F�������p�ւ���ɗׂ̃}�X�𒲂ׂ�
							vect.y += way[wayno].y;
							if (ms->masu[i + vect.x][j + vect.y].m_setstone == ms->player) { //���ׂ��}�X�Ɏ����̐΂�����ꍇ
								(ms->masu[i][j]).m_state = ms->player;                       //�}�X��΂��u�����Ԃɂ���
								flg = 1;
								break;
							}
							if (ms->masu[i + vect.x][j + vect.y].m_setstone == EMP) {         //���ׂ��}�X�ɐ΂��Ȃ��ꍇ
								break;                                                       //��������for���𔲂���
							}

						}
					}
				}
				vect.x = 0;                                                                  //�x�N�g��������������
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
	//�Ԃł͂Ȃ��ق��̃v���C���[
	int anplayer = ms->player * REVERSE;

	//���ݎQ�Ƃ��Ă�����W���炱�ꂩ��Q�Ƃ�����W�܂ł̍���
	POINT vect;
	vect.x = 0;
	vect.y = 0;

	/*�T��������p(�x�N�g��)��\���ϐ�
	�@   ��
		7 0 1
		��6 �@2�E
		5 4 3
		��
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


	if ((ms->masu[cp.x][cp.y]).m_setstone != EMP) {                                        //�΂��u���Ă���}�X�͔��肵�Ȃ�
		for (wayno = 0; wayno < 8; wayno++) {                                        //���ׂ�}�X�𒆐S��8�����̃}�X���m�F����
			vect.x = way[wayno].x;                                                   //�m�F������p(�x�N�g��)��������
			vect.y = way[wayno].y;
			if (ms->masu[cp.x + vect.x][cp.y + vect.y].m_setstone == anplayer) {           //�m�F�����}�X�ɑ���̐΂�����ꍇ
				for (i = 0; i < 6; i++) {  //�ő�6��
					vect.x += way[wayno].x;                                          //�m�F�������p�ւ���ɗׂ̃}�X�𒲂ׂ�
					vect.y += way[wayno].y;
					if (ms->masu[cp.x + vect.x][cp.y + vect.y].m_setstone == ms->player) { //���ׂ��}�X�Ɏ����̐΂�����ꍇ
						for (j = 0; j < 6; j++) {  //�ő�6��
							vect.x -= way[wayno].x;                                      //�ЂƂO�ɖ߂�
							vect.y -= way[wayno].y;
							if (ms->masu[cp.x + vect.x][cp.y + vect.y].m_setstone == anplayer) {
								ms->masu[cp.x + vect.x][cp.y + vect.y].m_setstone *= REVERSE;//�΂��Ђ�����Ԃ�
								if (ms->player == BSTONE) {
									ms->blackcnt++;//�}�X��ɂ��鍕�̐΂̑���
									ms->whitecnt--;
								}
								else {
									ms->blackcnt--;//�}�X��ɂ��鍕�̐΂̑���
									ms->whitecnt++;
								}
							}
							else {
								break;
							}
						}
					}
					if (ms->masu[cp.x + vect.x][cp.y + vect.y].m_setstone == EMP) {         //���ׂ��}�X�ɐ΂��Ȃ��ꍇ
						break;                                                       //��������for���𔲂���
					}

				}
			}
		}
		vect.x = 0;                                                                  //�x�N�g��������������
		vect.y = 0;

	}
}
//�R�s�y�p�f�o�b�N���b�Z�[�W
//MessageBox(NULL , TEXT(" MessageBox ") ,TEXT("�f�o�b�N"), MB_OK);

//�R�s�y�p�֐�������
/*
�֐���
�@�\
�����F�ϐ����@int (i)//i =���́@o =�o��
�ԋp�l�@NG�F�����Ȃ�
1�F���͒u����
�@�@�@�@ 2�F���͒u����
	 */