#include <windows.h>
#include <ctime>
#include <cstdio>

#pragma comment(lib,"ws2_32.lib")

#define START 0 //刚打开程序，显示主菜单
#define INPUT_PORT 1 //显示端口输入框
#define INPUT_BOTH 2 //显示IP输入框和端口输入框
#define PLAY 3 //游戏正式开始
#define FINAL 4 //显示胜负
#define WAITING_PORT 5 //服务端等待连接
#define WAITING_BOTH 6 //客户端等待连接
//以下定义各按钮和文本框的id
#define ID_PORTBUTTON 100 //“等待匹配”按钮
#define ID_BOTHBUTTON 101 //“主动匹配”按钮
#define ID_INPUTPORT 102 //端口输入框
#define ID_INPUTIP 103 //IP地址输入框
#define ID_ENSURE 104 //“确认”按钮

#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code)&0x8000)?1:0) //处理按键信息

int turn = 1; //0表示服务端行动，1表示客户端行动
int win = 0;//判断自己是否获胜
int is_client,port;//is_client表示是否为客户端，port为int型的端口信息
int pace1 = 1,pace2 = 1; //显示两人各走几步，其中pace1为自己，pace2为对方
int state = START; //游戏状态
int listened = 0;//表示客户端已经完成了listen前的各个步骤（含listen）

int board[10][10]; //棋盘状态，0表示空，1表示自己，2表示对方
int currentme[2],currentyou[2];

char ip[20],str_port[10]; //分别存放字符形式的IP地址和端口号
char action[10]; //用于与另一个玩家交换信息

HWND hportbutton, hbothbutton,hinputport,hinputip,hensure; //各按钮、输入框的句柄

SOCKET connSocket;//传递信息用的套接字
SOCKET serversocket;//服务端专有套接字

/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	HPEN pen; 
	HBRUSH brush;
	PAINTSTRUCT ps;
	switch (Message) {
		/* Upon destruction, tell the main thread to stop */
	case WM_DESTROY: {
		closesocket(connSocket);
		if (!is_client) closesocket(serversocket);
		WSACleanup();
		PostQuitMessage(0);
		break;
	}
	case WM_PAINT:
		hdc = BeginPaint(hwnd,&ps);
		switch (state)
		{
			case START:
				HFONT hfont;
				hfont=CreateFont(30, 15, 0, 0, 400, 0, 0, 0, ANSI_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, FF_MODERN, "楷体");
				SelectObject(hdc, hfont);
				SetTextColor(hdc, RGB(0, 0, 155));
				SetBkColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, TRANSPARENT);
				TextOut(hdc, 150, 100, "欢迎使用双人版益智游戏！", strlen("欢迎使用双人版益智游戏！"));
				break;
			case INPUT_PORT:
				TextOut(hdc, 145, 170, "请输入端口号：", strlen("请输入端口号："));
				break;
			case INPUT_BOTH:
				TextOut(hdc, 145, 170, "请输入端口号：", strlen("请输入端口号："));
				TextOut(hdc, 145, 205, "请输入IP号：", strlen("请输入IP号："));
				break;
			case PLAY:
			{
				pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
							SelectObject(hdc, pen);
							brush = CreateSolidBrush(RGB(0, 255, 0));
							SelectObject(hdc, brush);
							Rectangle(hdc, 25, 25, 275, 275);
							for (int i = 2; i <= 10; i++)
							{
								MoveToEx(hdc, 25, 25*i, NULL);
								LineTo(hdc, 275, 25*i);
								MoveToEx(hdc, 25*i, 25, NULL);
								LineTo(hdc, 25*i, 275);
							}
							for (int i = 0; i < 10; i++)
							{
								for (int j = 0; j < 10; j++)
								{
									if (board[j][i] == 1)
									{
										DeleteObject(brush);
										brush = CreateSolidBrush(RGB(0, 0, 255));
										SelectObject(hdc, brush);
										Rectangle(hdc, 25 * (i+1), 25 * (j+1), 25 * (i + 2), 25 * (j + 2));
										DeleteObject(brush);
										brush = CreateSolidBrush(RGB(0, 255, 0));
										SelectObject(hdc, brush);
									}
									if (board[j][i] == 2)
									{
										DeleteObject(brush);
										brush = CreateSolidBrush(RGB(255, 0, 0));
										SelectObject(hdc, brush);
										Rectangle(hdc, 25 * i+25, 25 * j+25, 25 * (i + 2), 25 * (j + 2));
										DeleteObject(brush);
										brush = CreateSolidBrush(RGB(0, 255, 0));
										SelectObject(hdc, brush);
									}
								}
							}
							char out[100];
							sprintf(out, "您已经走了%d步", pace1);
							TextOut(hdc, 350, 95, out,strlen(out));
							sprintf(out, "对手已经走了%d步", pace2);
							TextOut(hdc, 350, 145, out, strlen(out));
							break;
			}
			case WAITING_BOTH:case WAITING_PORT:
				TextOut(hdc, 250,200, "等待对手匹配……", strlen("等待对手匹配……"));
				break;
			case FINAL:
				HFONT hfont2;
				hfont2 = CreateFont(30, 15, 0, 0, 400, 0, 0, 0, ANSI_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, FF_MODERN, "楷体");
				SelectObject(hdc, hfont2);
				SetTextColor(hdc, RGB(0, 0, 155));
				SetBkColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, TRANSPARENT);
				if (win)
					TextOut(hdc, 230, 200, "恭喜获胜！", strlen("恭喜获胜！"));
				else
					TextOut(hdc, 205, 200, "很遗憾，失败了！", strlen("很遗憾，失败了！"));
				break;
		}
		EndPaint(hwnd, &ps);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_PORTBUTTON://选择等待匹配
			is_client = 0;//设为服务端
			ShowWindow(hportbutton, HIDE_WINDOW);
			ShowWindow(hbothbutton, HIDE_WINDOW);
			ShowWindow(hinputport, SW_SHOW);
			ShowWindow(hensure, SW_SHOW);
			state = INPUT_PORT;
			InvalidateRect(hwnd, NULL, TRUE);
			hdc = BeginPaint(hwnd, &ps);
			TextOut(hdc, 145, 170, "请输入端口号：", strlen("请输入端口号："));
			EndPaint(hwnd, &ps);
			break;
		case ID_BOTHBUTTON://选择主动匹配
			ShowWindow(hportbutton, HIDE_WINDOW);
			ShowWindow(hbothbutton, HIDE_WINDOW);
			ShowWindow(hinputport, SW_SHOW);
			ShowWindow(hinputip, SW_SHOW);
			ShowWindow(hensure, SW_SHOW);
			state = INPUT_BOTH;
			is_client = 1; //设为客户_pot端
			InvalidateRect(hwnd, NULL, TRUE);
			hdc = BeginPaint(hwnd, &ps);
			TextOut(hdc, 145, 170, "请输入端口号：", strlen("请输入端口号："));
			TextOut(hdc, 145, 205, "请输入IP号：", strlen("请输入IP号："));
			EndPaint(hwnd, &ps);
			break;
		case ID_ENSURE:
			GetWindowText(hinputport, str_port, 10);
			port = atoi(str_port);
			if (state == INPUT_PORT)
				state = WAITING_PORT;
			else
			{
				GetWindowText(hinputip, ip, 20);
				state = WAITING_BOTH;
				if (is_client)
				{
					srand((unsigned)time(NULL)); //初始化棋盘
					currentme[0] = rand() % 10;
					currentme[1] = rand() % 10;
					board[currentme[0]][currentme[1]] = 1;
					while (TRUE)
					{
						currentyou[0] = rand() % 10; currentyou[1] = rand() % 10;
						if (board[currentyou[0]][currentyou[1]] == 0)
						{
							board[currentyou[0]][currentyou[1]] = 2;
							break;
						}
					}
					action[0] = currentyou[0] + '0';
					action[1] = currentyou[1] + '0';
					action[2] = currentme[0] + '0';
					action[3] = currentme[1] + '0';
					action[4] = 0;
				}
			}
			ShowWindow(hensure, HIDE_WINDOW);
			ShowWindow(hinputip, HIDE_WINDOW);
			ShowWindow(hinputport, HIDE_WINDOW);
			InvalidateRect(hwnd, NULL, TRUE);
			UpdateWindow(hwnd);
			break;
		}
		break;
				   /* All other messages (a lot of them) are processed using default procedures */
	default:
		return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc; /* A properties struct of our window */
	HWND hwnd; /* A 'HANDLE', hence the H, or a pointer to our window */
	MSG msg; /* A temporary location for all messages */

	/* zero out the struct and set the stuff we want to modify */
	memset(&wc, 0, sizeof(wc));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = WndProc; /* This is where we will send messages to */
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	/* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszClassName = "WindowClass";
	wc.hIcon = LoadIcon(NULL,IDI_SHIELD); /* Load a standard icon */
	wc.hIconSm = LoadIcon(NULL, IDI_SHIELD); /* use the name "A" to use the project icon */

	if (!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, "WindowClass", "双人版益智游戏", WS_VISIBLE | WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, /* x */
		CW_USEDEFAULT, /* y */
		640, /* width */
		480, /* height */
		NULL, NULL, hInstance, NULL);

	if (hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	hportbutton = CreateWindowEx(NULL, "button", "等待匹配", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 250, 220, 125, 30, hwnd, (HMENU)(100), hInstance, NULL);
	hbothbutton = CreateWindowEx(NULL, "button", "主动匹配", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 250, 265, 125, 30, hwnd, (HMENU)(101), hInstance, NULL);
	hinputport = CreateWindowEx(NULL, "edit", NULL, WS_CHILD|BS_TEXT|WS_BORDER, 250, 170, 175, 20, hwnd, (HMENU)ID_INPUTPORT, hInstance, NULL);
	hinputip = CreateWindowEx(NULL, "edit", NULL, WS_CHILD |BS_TEXT | WS_BORDER, 250, 205, 175, 20, hwnd, (HMENU)ID_INPUTIP, hInstance, NULL);
	hensure = CreateWindowEx(NULL, "button", "确认", WS_CHILD | BS_PUSHBUTTON, 300, 250, 125, 30, hwnd, (HMENU)(ID_ENSURE), hInstance, NULL);
	//加载套接字库
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	serversocket = socket(AF_INET, SOCK_STREAM, 0);
	int mode = 1;
	int len = sizeof(SOCKADDR);
	sockaddr_in addrSrv, clientAddr;
	while(TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		switch (state)
		{
		case WAITING_PORT:
			if(!listened)
			{
				ioctlsocket(serversocket, FIONBIO, (u_long FAR*) & mode);
				addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
				addrSrv.sin_family = AF_INET;
				addrSrv.sin_port = htons(port);
				bind(serversocket, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
				listen(serversocket, 100);
				listened = 1;
			}
			if ((connSocket = accept(serversocket, (SOCKADDR*)&clientAddr, &len)) !=INVALID_SOCKET)
			{
				state = PLAY;
				while (recv(connSocket, action, sizeof(action),0) <= 0);
				currentme[0] =action[0]-'0';
				currentme[1] = action[1]-'0';
				currentyou[0] = action[2]-'0';
				currentyou[1] = action[3]-'0';
				board[currentme[0]][currentme[1]] = 1;
				board[currentyou[0]][currentyou[1]] = 2;
				InvalidateRect(hwnd, NULL, TRUE);
				UpdateWindow(hwnd);
			}
			break;
		case WAITING_BOTH:
			if (!listened)
			{
				connSocket = socket(AF_INET, SOCK_STREAM, 0);
				ioctlsocket(connSocket, FIONBIO, (u_long FAR*) & mode);
				addrSrv.sin_addr.S_un.S_addr = inet_addr(ip);
				addrSrv.sin_family = AF_INET;
				addrSrv.sin_port = htons(port);
				listened = 1;
			}
			if (connect(connSocket, (SOCKADDR*)&addrSrv, len) !=SOCKET_ERROR)
			{
				send(connSocket, action, sizeof(action), 0);
				memset(action, 0, sizeof(action));
				state = PLAY;
				InvalidateRect(hwnd, NULL, TRUE);
				UpdateWindow(hwnd);
			}
			else
			{
				int err = WSAGetLastError();
				if (err == WSAEISCONN)
				{
					send(connSocket, action, sizeof(action), 0);
					memset(action, 0, sizeof(action));
					state = PLAY;
					InvalidateRect(hwnd, NULL, TRUE);
					UpdateWindow(hwnd);
				}
			}
		break;
		case PLAY:
			if (turn == is_client)
			{
				if (KEYDOWN(VK_UP) && currentme[0] != 0 && board[currentme[0]-1][currentme[1] ] == 0)
				{
					strcpy(action, "u");
					pace1++;
					board[currentme[0] - 1][currentme[1]] = 1;
					currentme[0]--;
					send(connSocket, action, sizeof(action), 0);
					memset(action, 0, sizeof(action));
;					InvalidateRect(hwnd, NULL, TRUE);
					UpdateWindow(hwnd);
					turn = turn ? 0 : 1;
				}
				if(KEYDOWN(VK_LEFT) && currentme[1] != 0 && board[currentme[0]][currentme[1] - 1] == 0)
				{
					strcpy(action, "l");
					pace1++;
					board[currentme[0]][currentme[1] - 1] = 1;
					currentme[1]--;
					send(connSocket, action, sizeof(action), 0);
					memset(action, 0, sizeof(action));
					InvalidateRect(hwnd, NULL, TRUE);
					UpdateWindow(hwnd);
					turn = turn ? 0 : 1;
				}
				if (KEYDOWN(VK_RIGHT) && currentme[1] != 9 && board[currentme[0]][currentme[1]+1] == 0)
				{
					strcpy(action, "r");
					pace1++;
					board[currentme[0]][currentme[1] + 1] = 1;
					currentme[1]++;
					send(connSocket, action, sizeof(action), 0);
					memset(action, 0, sizeof(action));
					InvalidateRect(hwnd, NULL, TRUE);
					UpdateWindow(hwnd);
					turn = turn ? 0 : 1;
				}
				if (KEYDOWN(VK_DOWN) && currentme[0] != 9 && board[currentme[0]+1][currentme[1]] == 0)
				{
					strcpy(action, "d");
					pace1++;
					board[currentme[0] + 1][currentme[1]] = 1;
					currentme[0]++;
					send(connSocket, action, sizeof(action), 0);
					memset(action, 0, sizeof(action));
					InvalidateRect(hwnd, NULL, TRUE);
					UpdateWindow(hwnd);
					turn = turn ? 0 : 1;
				}
				if ((currentyou[0] == 0 || board[currentyou[0] - 1][currentyou[1]] != 0) && (currentyou[0] == 9 || board[currentyou[0] + 1][currentyou[1]] != 0) && (currentyou[1] == 0 || board[currentyou[0]][currentyou[1] - 1] != 0) && (currentyou[1] == 9 || board[currentyou[0]][currentyou[1] + 1] != 0))
				{
					state = FINAL;
					InvalidateRect(hwnd, NULL, TRUE);
					UpdateWindow(hwnd);
					win = 1;
				}
				if ((currentme[0] == 0 || board[currentme[0] - 1][currentme[1]] != 0) && (currentme[0] == 9 || board[currentme[0] + 1][currentme[1]] != 0) && (currentme[1] == 0 || board[currentme[0]][currentme[1] - 1] != 0) && (currentme[1] == 9 || board[currentme[0]][currentme[1] + 1] != 0))
				{
					state = FINAL;
					win = 0;
					InvalidateRect(hwnd, NULL, TRUE);
					UpdateWindow(hwnd);
				}
			}
			else
			{
				if (recv(connSocket, action, sizeof(action), 0) > 0)
				{
					if (strcmp(action, "u") == 0)
					{
						board[currentyou[0] - 1][currentyou[1]] = 2;
						currentyou[0]--;
					}
					else if (strcmp(action, "l") == 0)
					{
						board[currentyou[0]][currentyou[1] - 1] = 2;
						currentyou[1]--;
					}
					else if (strcmp(action, "r") == 0)
					{
						board[currentyou[0]][currentyou[1] + 1] = 2;
						currentyou[1]++;
					}
					else
					{
						board[currentyou[0] + 1][currentyou[1]] = 2;
						currentyou[0]++;
					}
					if ((currentme[0] == 0 || board[currentme[0] - 1][currentme[1]] != 0) && (currentme[0] == 9 || board[currentme[0] + 1][currentme[1]] != 0) && (currentme[1] == 0 || board[currentme[0]][currentme[1] - 1] != 0) && (currentme[1] == 9 || board[currentme[0]][currentme[1] + 1] != 0))
					{
						state = FINAL;
						win = 0;
					}
					if ((currentyou[0] == 0 || board[currentyou[0] - 1][currentyou[1]] != 0) && (currentyou[0] == 9 || board[currentyou[0] + 1][currentyou[1]] != 0) && (currentyou[1] == 0 || board[currentyou[0]][currentyou[1] - 1] != 0) && (currentyou[1] == 9 || board[currentyou[0]][currentyou[1] + 1] != 0))
					{
						state = FINAL;
						win = 1;
					}
					pace2++;
					memset(action, 0, sizeof(action));
					InvalidateRect(hwnd, NULL, TRUE);
					UpdateWindow(hwnd);
					turn = turn ? 0 : 1;
				}
			}
		default:break;
		}
	}
	return msg.wParam;
}