#pragma once
#include <iostream>
#include <string>

#define SHIFT 7

class Caesar{

public:
	static std::string Encrypt(std::string text);
	static std::string Decrypt(std::string text);
private:
	static char en_char(char c);
	static char de_char(char c);

};
/*Encrypt a string with the cipher.*/
inline std::string Caesar::Encrypt(std::string text){
	for (int i = 0; i < text.size(); i++){
		text[i] = en_char(text[i]);
	}
	return text;
}
/*Decrypt a string with the cipher.*/
inline std::string Caesar::Decrypt(std::string text){
	for (int i = 0; i < text.size(); i++){
		text[i] = de_char(text[i]);
	}
	return text;
}
inline char Caesar::en_char(char c){
	if (isalpha(c)){
		bool lower = false;
		if (islower(c)) { lower = true; }
		c = toupper(c);
		c += SHIFT;
		int shift_pos = static_cast<int>(c);
		if (shift_pos > 90){ c -= 26; }
		if (lower) { c = tolower(c); }
	}
	return c;
}
inline char Caesar::de_char(char c){
	if (isalpha(c)){
		bool lower = false;
		if (islower(c)) { lower = true; }
		c = toupper(c);
		c -= SHIFT;
		int shift_pos = static_cast<int>(c);
		if (shift_pos < 65){ c += 26; }
		if (lower) { c = tolower(c); }
	}
	return c;
}
