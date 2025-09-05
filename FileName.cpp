#include<iostream>
#include <codecvt>
#include <locale>
#include<string>
#include<Windows.h>
#pragma comment(lib,"winmm.lib")

using namespace std;

std::string unicodeToUtf8(const std::wstring& unicodeStr) {
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	return converter.to_bytes(unicodeStr);
}


// 1. 定义结构体：一段旋律 + 注释
struct Phrase {
	const int* notes;   // 指向数组首地址
	size_t      count;   // 数组元素个数
	const char* comment; // 注释
};


enum Scale
{
	Rest = 0, C8 = 108, B7 = 107, A7s = 106, A7 = 105, G7s = 104, G7 = 103, F7s = 102, F7 = 101, E7 = 100, D7s = 99, D7 = 98, C7s = 97, C7 = 96,
	B6 = 95, A6s = 94, A6 = 93, G6s = 92, G6 = 91, F6s = 90, F6 = 89, E6 = 88, D6s = 87, D6 = 86, C6s = 85, C6 = 84,
	B5 = 83, A5s = 82, A5 = 81, G5s = 80, G5 = 79, F5s = 78, F5 = 77, E5 = 76, D5s = 75, D5 = 74, C5s = 73, C5 = 72,
	B4 = 71, A4s = 70, A4 = 69, G4s = 68, G4 = 67, F4s = 66, F4 = 65, E4 = 64, D4s = 63, D4 = 62, C4s = 61, C4 = 60,
	B3 = 59, A3s = 58, A3 = 57, G3s = 56, G3 = 55, F3s = 54, F3 = 53, E3 = 52, D3s = 51, D3 = 50, C3s = 49, C3 = 48,
	B2 = 47, A2s = 46, A2 = 45, G2s = 44, G2 = 43, F2s = 42, F2 = 41, E2 = 40, D2s = 39, D2 = 38, C2s = 37, C2 = 36,
	B1 = 35, A1s = 34, A1 = 33, G1s = 32, G1 = 31, F1s = 30, F1 = 29, E1 = 28, D1s = 27, D1 = 26, C1s = 25, C1 = 24, B0 = 23, A0s = 22, A0 = 21
};

void HB()
{
	HMIDIOUT handle;
	midiOutOpen(&handle, 0, 0, 0, CALLBACK_NULL);
	int volume = 0x7f;
	int voice = 0x0;
	int sleep = 350;
	// 舒缓前奏（降八度、放慢、音量降低）
	const int prelude_vol = 0x40;        // 50% 音量
	const int prelude_sleep = 500;       // 每音 500 ms
	int prelude[] = {
		C4, E4, G4, B4, G4, E4, C4, Rest   // 随意写，可按需要改
	};
	// ---------- 先播放前奏 ----------
	printf("Are You Ready?");
	Sleep(1000);
	printf("\nLet's Start!");

	for (size_t i = 0; i < sizeof(prelude) / sizeof(int); ++i)
	{
		int n = prelude[i];
		if (n == Rest) { Sleep(prelude_sleep); continue; }

		int msg = (prelude_vol << 16) | (n << 8) | 0x90;
		midiOutShortMsg(handle, msg);
		Sleep(prelude_sleep);
	}
	// ---------- 前奏结束 ----------



	int vc[] = {
		//C4为do，以此类推
		//G3,G3,A3,0,G3,0,C4,B3,300,G3,G3,A3,0,G3,0,D4,C4,300,G3,G3,G4,0,E4,0,C4,
		//B3,0,A3,300,F4,F4,E4,0,C4,D4,C4,300,300,0,0,C4,D4,E4,C4,D4,E4,C4,D4,G4,
		//C4,A3,B3,C4,A3,B3,E4,A3,F3,G3,A3,F3,G3,C4,F3,E3,F3,G3,A3,G3,A3,B3,C4,B3,C4,D4,E4,D4,E4,F4,G4,300,0,0,
		E4,G4,G4,A4,G4,300,D4,G4,G4,A4,G4,0,C4,C4,C4,C4,C4,0,C5,B4,G4,G4,A4,G4,0,300,300,
		A4,C5,C5,D5,C5,0,C5,A4,G4,A4,A4,G4,E4,300,F4,F4,0,E4,F4,E4,0,C4,D4,300,300,0,
		//点燃了
		E4,G4,G4,A4,G4,0,0,0,D4,G4,G4,A4,G4,0,0,C4,C4,C4,C4,C4,0,C5,C5,B4,G4,G4,A4,G4,300,
		A4,C5,C5,D5,C5,0,C5,A4,G4,E5,E5,D5,C5,300,A4,A4,0,G4,A4,A4,0,C5,D5,300,300,300,
		D5,300,0,0,G4,E5,C5,D5,E5,0,C5,D5,0,G4,0,G4,0,E4,C5,0,A4,B4,C5,A4,A4,
		B4,0,G5,0,G5,300,F5,0,F5,E5,D5,E5,F5,E5,0,D5,0,C5,A4,A4,C5,0,C5,A4,C5,0,D5,E5,
		D5,300,300,G4,E5,0,C5,D5,E5,0,C5,C5,D5,0,G4,0,G4,0,E4,C5,0,A4,B4,C5,0,A4,A4,
		B4,0,G5,0,G5,300,F5,0,F5,E5,D5,0,E5,F5,E5,0,D5,0,C5,A4,A4,C5,0,C5,A4,C5,0,D5,E5,
		D5,300,B4,C5,0,C5,D5,C5,300,300,300,300,300,300,A4,F4,G4,A4,F4,G4,C5,F4,E4,D4,C4,D4,C4,B3,A3,G3,300,0,
		//重来，分界，点燃了蜡烛
		E4,G4,G4,A4,G4,0,0,D4,G4,G4,A4,G4,0,0,C4,C4,C4,C4,C4,0,C5,C5,B4,G4,G4,A4,G4,300,
		A4,C5,C5,D5,C5,0,C5,A4,G4,E5,E5,D5,C5,300,A4,A4,0,G4,A4,A4,0,C5,D5,300,300,300,
		D5,300,0,0,G4,E5,C5,D5,E5,0,C5,D5,0,G4,0,G4,0,E4,C5,0,A4,B4,C5,A4,A4,
		B4,0,G5,0,G5,300,F5,0,F5,E5,D5,E5,F5,E5,0,D5,0,C5,A4,A4,C5,0,C5,A4,C5,0,D5,E5,
		D5,300,300,G4,E5,0,C5,D5,E5,0,C5,C5,D5,0,G4,0,G4,0,E4,C5,0,A4,B4,C5,0,A4,A4,
		B4,0,G5,0,G5,300,F5,0,F5,E5,D5,0,E5,F5,E5,0,D5,0,C5,A4,A4,C5,0,C5,A4,C5,0,D5,E5,
		D5,300,B4,C5,0,C5,D5,C5,300,300,A4,F4,G4,A4,F4,G4,C5,F4,E4,D4,C4,D4,C4,B3,A3,G3,300,0,
		//说拜拜
		D5,300,0,0,G4,E5,C5,D5,E5,0,C5,D5,0,G4,0,G4,0,E4,C5,0,A4,B4,C5,A4,A4,
		B4,0,G5,0,G5,300,F5,0,F5,E5,D5,E5,F5,E5,0,D5,0,C5,A4,A4,C5,0,C5,A4,C5,0,D5,E5,
		D5,300,300,G4,E5,0,C5,D5,E5,0,C5,C5,D5,0,G4,0,G4,0,E4,C5,0,A4,B4,C5,0,A4,A4,
		B4,0,G5,0,G5,300,F5,0,F5,E5,D5,0,E5,F5,E5,0,D5,0,C5,A4,A4,C5,0,C5,A4,C5,0,D5,E5,
		D5,300,B4,C5,0,C5,D5,C5,300,300,300,0

	};
	//今天你生日
	int vd[] = { E4,G4,G4,A4,G4,300 };
	//送上我祝福
	int ve[] = { D4,G4,G4,A4,G4,300 };
	//特别的日子有灿烂的笑容
	int vf[] = { C4,C4,C4,C4,C4,0,C5,B4,G4,G4,A4,G4,300,300 };
	//我们来相聚
	int vg[] = { A4,C5,C5,D5,C5,0 };
	//带着满满的关爱
	int vh[] = { C5,A4,G4,A4,A4,G4,E4,300 };
	//祝福你好运常伴
	int vj[] = { F4,F4,0,E4,F4,E4,C4,D4,300,300,0 };
	//点燃了蜡烛
	int vk[] = { E4,G4,G4,A4,G4,300,300 };
	//许下你心愿
	int vl[] = { D4,G4,G4,A4,G4,0,0 };
	//未来的日子每个梦想都实现
	int vm[] = { C4,C4,C4,C4,C4,0,C5,C5,B4,G4,G4,A4,G4,300 };
	//唱首祝福歌
	int vn[] = { A4,C5,C5,D5,C5,0 };
	//我们相亲又相爱opqrstuvwxyz
	int vo[] = { C5,A4,G4,E5,E5,D5,C5,300 };
	//祝福你健康平安
	int vp[] = { A4,A4,0,G4,A4,A4,0,C5,D5,300,300,300,D5,300,0 };
	//对所有的烦恼说声拜拜
	int vq[] = { 300,G4,E5,C5,D5,E5,C5,D5,G4,G4 };
	//对所有的快乐说声嗨嗨
	int vr[] = { E4,C5,A4,B4,C5,A4,A4,B4,G5,G5,300 };
	//亲爱的亲爱的生日快乐
	int vs[] = { F5,0,F5,E5,D5,E5,F5,E5,D5,C5,A4,A4 };
	//每一天都非常精彩
	int vt[] = { C5,0,C5,A4,C5,0,D5,E5, D5,300,300 };
	//看幸福的花儿为你盛开
	int vu[] = { G4,E5,C5,D5,E5,C5,C5,D5,G4,G4,0 };
	//听美妙的音乐为你喝彩
	int vv[] = { E4,C5,A4,B4,C5,A4,A4,B4,G5,G5,300 };
	//亲爱的亲爱的生日快乐
	int vw[] = { F5,0,F5,E5,D5,0,E5,F5,E5,D5,C5,A4,A4 };
	//祝你幸福直到永远
	int vx[] = { C5,0,C5,A4,C5,0,D5,E5,D5,300 };
	//幸福永远
	int vy[] = { B4,C5,0,C5,D5,C5,300 };
	//长伴奏
	int vz[] = { 300,300,A4,F4,G4,A4,F4,G4,C5,F4,E4,D4,C4,D4,C4,B3,A3,G3,300,0 };
	int v_upbeat[] = {
	0,G4,B4,D5,G5,0,
	0,D5,B4,G4,D4,0,
	0,E4,G4,C5,E5,0,
	0,C5,G4,E4,C4,E4,G4,0
	};



	Phrase group[] = {
			{ vd, sizeof(vd) / sizeof(int), u8"\n今天你生日(^_−)☆" },
			{ ve, sizeof(ve) / sizeof(int), u8"送上我祝福o(*￣3￣)o  " },
			{ vf, sizeof(vf) / sizeof(int), u8"特别的日子有灿烂的笑容✧*｡٩(ˊᗜˋ*)و✧*｡" },
			{ vg, sizeof(vg) / sizeof(int), u8"我们来相聚ヾ(^∀^)ﾉ" },
			{ vh, sizeof(vh) / sizeof(int), u8"带着满满的关爱ヾ(^v^*)/" },
			{ vj, sizeof(vj) / sizeof(int), u8"祝福你好运常伴❥(ゝω・✿ฺ)" },
			{ vk, sizeof(vk) / sizeof(int), u8"点燃了蜡烛(≧ω≦)/" },
			{ vl, sizeof(vl) / sizeof(int), u8"许下你心愿(▰˘◡˘▰) " },
			{ vm, sizeof(vm) / sizeof(int), u8"未来的日子每个梦想都实现(/^▽^)/ ヾ(^∇^ヾ)))" },
			{ vn, sizeof(vn) / sizeof(int), u8"唱首祝福歌ㄟ(≧o≦)ㄏ " },
			{ vo, sizeof(vo) / sizeof(int), u8"我们相亲又相爱(*^o^)人(^o^*)" },
			{ vp, sizeof(vp) / sizeof(int), u8"祝福你健康平安＼(＾∀＾)メ(＾∀＾)ノ" },
			{ vq, sizeof(vq) / sizeof(int), u8"对所有的烦恼说声拜拜(^-^)/" },
			{ vr, sizeof(vr) / sizeof(int), u8"对所有的快乐说声嗨嗨(,,･∀･)ﾉ゛hello" },
			{ vs, sizeof(vs) / sizeof(int), u8"亲爱的亲爱的生日快乐*\\(^o^)/!!" },
			{ vt, sizeof(vt) / sizeof(int), u8"每一天都非常的精彩~" },
			{ vu, sizeof(vu) / sizeof(int), u8"看幸福的花儿为你盛开ヾ(๑╹◡╹)ﾉ✿\"" },
			{ vv, sizeof(vv) / sizeof(int), u8"听美妙的音乐为你喝彩♪（＾∀＾●）ﾉ" },
			{ vw, sizeof(vw) / sizeof(int), u8"亲爱的亲爱的生日快乐*\\(^o^)/*!!" },
			{ vx, sizeof(vx) / sizeof(int), u8"祝你幸福直到永远ლ(＾ω＾λ)" },
			{ vy, sizeof(vy) / sizeof(int), u8"幸福到永远｡◕ᴗ◕｡" },
			{ vz, sizeof(vz) / sizeof(int), u8"\n这是间奏~" },
			{ vk, sizeof(vk) / sizeof(int), u8"\n点燃了蜡烛(≧ω≦)/" },
			{ vl, sizeof(vl) / sizeof(int), u8"许下你心愿(▰˘◡˘▰) " },
			{ vm, sizeof(vm) / sizeof(int), u8"未来的日子每个梦想都实现(/^▽^)/ ヾ(^∇^ヾ)))" },
			{ vn, sizeof(vn) / sizeof(int), u8"唱首祝福歌ㄟ(≧o≦)ㄏ " },
			{ vo, sizeof(vo) / sizeof(int), u8"我们相亲又相爱(*^o^)人(^o^*)" },
			{ vp, sizeof(vp) / sizeof(int), u8"祝福你健康平安＼(＾∀＾)メ(＾∀＾)ノ" },
			{ vq, sizeof(vq) / sizeof(int), u8"对所有的烦恼说声拜拜(^-^)/" },
			{ vr, sizeof(vr) / sizeof(int), u8"对所有的快乐说声嗨嗨(,,･∀･)ﾉ゛hello" },
			{ vs, sizeof(vs) / sizeof(int), u8"亲爱的亲爱的生日快乐*\\(^o^)/!!" },
			{ vt, sizeof(vt) / sizeof(int), u8"每一天都非常的精彩~" },
			{ vu, sizeof(vu) / sizeof(int), u8"看幸福的花儿为你盛开ヾ(๑╹◡╹)ﾉ✿\"" },
			{ vv, sizeof(vv) / sizeof(int), u8"听美妙的音乐为你喝彩♪（＾∀＾●）ﾉ" },
			{ vw, sizeof(vw) / sizeof(int), u8"亲爱的亲爱的生日快乐*\\(^o^)/*!!" },
			{ vx, sizeof(vx) / sizeof(int), u8"祝你幸福直到永远ლ(＾ω＾λ)" },
			{ vy, sizeof(vy) / sizeof(int), u8"幸福到永远｡◕ᴗ◕｡" },
			{ v_upbeat, sizeof(v_upbeat) / sizeof(int), u8"\n是间奏哦~" },
			{ vq, sizeof(vq) / sizeof(int), u8"\n对所有的烦恼说声拜拜(^-^)/" },
			{ vr, sizeof(vr) / sizeof(int), u8"对所有的快乐说声嗨嗨(,,･∀･)ﾉ゛hello" },
			{ vs, sizeof(vs) / sizeof(int), u8"亲爱的亲爱的生日快乐*\\(^o^)/!!" },
			{ vt, sizeof(vt) / sizeof(int), u8"每一天都非常的精彩~" },
			{ vu, sizeof(vu) / sizeof(int), u8"看幸福的花儿为你盛开ヾ(๑╹◡╹)ﾉ\"" },
			{ vv, sizeof(vv) / sizeof(int), u8"听美妙的音乐为你喝彩♪（＾∀＾●）ﾉ" },
			{ vw, sizeof(vw) / sizeof(int), u8"亲爱的亲爱的生日快乐*\\(^o^)/*!!" },
			{ vx, sizeof(vx) / sizeof(int), u8"祝你幸福直到永远ლ(＾ω＾λ)" },
			{ vy, sizeof(vy) / sizeof(int), u8"幸福到永远｡◕ᴗ◕｡" }
	};

	// 3. 遍历 group：播放完一段就输出注释
	// 正式内容（group）
	for (const auto& phrase : group)
	{
		puts(phrase.comment);
		for (size_t j = 0; j < phrase.count; ++j)
		{
			int i = phrase.notes[j];
			if (i == 0) { sleep = 350; continue; }
			if (i == 300) { Sleep(450); continue; }

			int msg = (volume << 16) | (i << 8) | 0x90;
			midiOutShortMsg(handle, msg);
			Sleep(sleep);
		}
	}

	midiOutClose(handle);

}

int main()
{
	SetConsoleOutputCP(65001);
	SetConsoleCP(65001);
	int a;
	cin >> a;
	if (a == 1)
		HB();
	return 0;
}