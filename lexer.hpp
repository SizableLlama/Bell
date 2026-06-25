#pragma once
#include "enums.hpp"
#include "shellToken.hpp"
#include <string>
#include <iostream>
#include <vector>
#include <any>

class Scanner final{
		private:
		std::string source;
		std::vector<Token> tokens;
		int start = 0;
		int current = 0;
		int line = 1;
	public:
		Scanner(std::string& source){
			this->source = source;
		}
	public:
		bool match(char expected);
		char advance();
		bool isAtEnd();
		bool isDigit(char c);
		void number();
		void identifier();
		bool isAlpha(char c);
		char peek();
		bool isAlphaNumeric(char c);
		void addToken(TokenType type, std::any literal);
		void addToken(TokenType type);
		std::vector<Token> scanTokens();
		void scanToken();
		char peekNext();
		void String();
		void word();

};

std::vector<Token> Scanner::scanTokens(){
	while(!isAtEnd()){
		//We are at the beggining of the next lexeme.
		start = current;

		scanToken();
	}

	tokens.push_back(Token(END_OF_FILE, "", std::any{}, line));
	return tokens;
};

void Scanner::scanToken(){
	char c = advance();
	switch (c) {
    case '|': addToken(match('|') ? OR_OR : PIPE); break;
    case '>': addToken(match('>') ? APPEND : RIGHT); break;
    case '<': addToken(LEFT); break;
    case '&': addToken(match('&') ? AND_AND : AMP); break;
    case ';': addToken(SEMICOLON); break;
    case '"': String(); break;       // scans to closing quote, calls addToken(STRING) internally
    case ' ': case '\t': break;      // skip whitespace (only when not inside quotes)
    case '\n': line++; break;

    default:

		word();
		break;

}
}


void Scanner::String(){
	while (peek() !='"' && !isAtEnd()){
		if (peek() == '\n') line++;
		advance();
	}

	if (isAtEnd()) {
		std::cerr<<line<< "Unterminated string.";
		return;
	}
	//The closing ".
	advance();

	//Trim the surrounding quotes.
	std::string value = source.substr(start + 1, current - start - 2);
	addToken(STRING, value);

}



//removed reference to expected.
bool Scanner::match(char expected){
	if (isAtEnd()) return false;
	//Replacing charAt
	if (source.at(current) != expected) return false;

	current++;
	return true;
}

char Scanner::peek(){
	if(isAtEnd()) return '\0';
	return source.at(current);
}

char Scanner::peekNext(){
	if(current + 1 >= source.length()) return '\0';
	return source.at(current + 1);
}

bool Scanner::isAtEnd(){
	return current >=source.length();
};

char Scanner::advance(){
	return source.at(current++);
}

void Scanner::addToken(TokenType type){
	std::any literal;
	addToken(type, literal);
}

void Scanner::addToken(TokenType type, std::any literal){
	std::string text = source.substr(start, current - start);
	tokens.push_back(Token(type, text, literal, line));
}

void Scanner::word() {
    // A shell word is any sequence of characters that IS NOT a space,
    while (!isAtEnd() &&
           peek() != ' ' &&
           peek() != '\t' &&
           peek() != '\n' &&
           peek() != '|' &&
           peek() != '<' &&
           peek() != '>' &&
           peek() != '&' &&
           peek() != ';' &&
           peek() != '"') {
        advance();
    }

    addToken(WORD);
}
