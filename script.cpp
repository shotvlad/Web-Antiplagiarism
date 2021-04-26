#include <iostream>
#include <string>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <clocale>
#include <cgicc/CgiDefs.h>
#include <cgicc/Cgicc.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>

using namespace std;
using namespace cgicc;

string keywordsToDelete[] = { "т.д.","т.п.","др.","пр.","см.","ср.","чтд","кг","км","м","г","с","А","К" };
string arrUnions[] = { "но", "и", "да"," не","или","либо", "же", "а", "что", "чтобы", "как", "так" };

double antiPlagiarism(string text, string fragment);
string getUnsignedText(string text);
string getTextWithoutNumbers(string text);
string getRussianText(string text);
char getRussianLetter(char text);
string getLowercaseText(string text);
char getLowercase(char element);
string getCorrectedText(string text, char element);
char getReplacingElements(char text, char element);
int getSizeStr(string text);
string deleteRepeatedWords(string text);
string checkRepeatWord(string text, string word, int lastIndex);
string deleteStr(string text, int firstIndex, int lastIndex);
string deleteSpaces(string text);
string deleteKeywords(string text, int index);
bool isKeywords(string word, int index);
string deletingSingleCharacterElements(string text);
double countingPlagiarism(string text, string fragment);
string getThreeWords(string words, int& lastIndex, string text, int& counter);

int getInt(string name);//get length of text putted in the form on your site(this function we use only for example)
string getDB();//get origin text from db.txt (don't modify tis function)

int main()
{
	setlocale(LC_ALL, "rus");
    Cgicc form;
    string name;

    cout << "Content-type:text/html\r\n\r\n";
    cout << "<html>\n";
    cout << "<head>\n";
    cout << "<title>Ggi Server</title>\n";
    cout << "</head>\n";
    cout << "<body>\n";
    cout << "<p>";
    
	name = form("name");



    if (!name.empty()) {
    	cout << antiPlagiarism(getDB(), name) << "\n";
    } else {
    	cout << "Text is not provided!\n";
    }	

    cout << "</p>\n";
    cout << "</body>\n";
    cout << "</html>\n";

    return 5;
}

int getInt(string name){
	return name.length();
}

string getDB() {
	string dbText = "", s1;

	ifstream in("db.txt");

	while (getline(in, s1)) {
		dbText += s1;
	}
	in.close();

	return dbText;
}

double antiPlagiarism(string text, string fragment) {

	text = getLowercaseText(text);
	fragment = getLowercaseText(fragment);

	text = deleteKeywords(text, 1);
	fragment = deleteKeywords(fragment, 1);

	text = deleteKeywords(text, 2);
	fragment = deleteKeywords(fragment, 2);


	text = getUnsignedText(text);
	fragment = getUnsignedText(fragment);

	text = getTextWithoutNumbers(text);
	fragment = getTextWithoutNumbers(fragment);

	text = getRussianText(text);
	fragment = getRussianText(fragment);

	text = deleteRepeatedWords(text);
	fragment = deleteRepeatedWords(fragment);

	text = deletingSingleCharacterElements(text);
	fragment = deletingSingleCharacterElements(fragment);

	text = deleteSpaces(text);
	fragment = deleteSpaces(fragment);

	return countingPlagiarism(text, fragment);
}

string getLowercaseText(string text)
{
	for (int i = 0; text[i] != '\0'; i++)
	{
		text[i] = getLowercase(text[i]);
	}

	return text;
}

char getLowercase(char element)
{
	if (element >= -64 && element <= -33)
	{
		int number = element;
		return number + 32;
	}
	else if (element == -88)
	{
		return 'ё';
	}
	else return element;
}

string getRussianText(string text)
{
	for (int i = 0; text[i] != '\0'; i++)
	{
		text[i] = getRussianLetter(text[i]);
	}

	return text;
}

char getRussianLetter(char text)
{
	if (text == 'y')
	{
		return 'у';
	}
	else if (text == 'K')
	{
		return 'К';
	}
	else if (text == 'E')
	{
		return 'Е';
	}
	else if (text == 'e')
	{
		return 'е';
	}
	else if (text == 'H')
	{
		return 'Н';
	}
	else if (text == 'X')
	{
		return 'Х';
	}
	else if (text == 'x')
	{
		return 'х';
	}
	else if (text == 'B')
	{
		return 'В';
	}
	else if (text == 'A')
	{
		return 'А';
	}
	else if (text == 'a')
	{
		return 'а';
	}
	else if (text == 'P')
	{
		return 'Р';
	}
	else if (text == 'p')
	{
		return 'р';
	}
	else if (text == 'O')
	{
		return 'О';
	}
	else if (text == 'o')
	{
		return 'о';
	}
	else if (text == 'C')
	{
		return 'C';
	}
	else if (text == 'c')
	{
		return 'c';
	}
	else if (text == 'T')
	{
		return 'Т';
	}
	else return text;
}

string getTextWithoutNumbers(string text)
{
	char numbers[] = "0123456789";

	for (int i = 0; numbers[i] != '\0'; i++)
	{
		text = getCorrectedText(text, numbers[i]);
	}

	return text;
}

string getUnsignedText(string text)
{
	char characters[] = "!.?,():;«»+-*/=%№";

	for (int i = 0; characters[i] != '\0'; i++)
	{
		text = getCorrectedText(text, characters[i]);
	}

	return text;
}

string getCorrectedText(string text, char element)
{
	for (int i = 0; text[i] != '\0'; i++)
	{
		text[i] = getReplacingElements(text[i], element);
	}

	return text;
}

char getReplacingElements(char text, char element)
{
	if (text == element)
	{
		return ' ';
	}
	else return text;
}

/*
5. проверка текст на повторяющиеся слова и удаляем если таковые есть(); +
6. удаляем все устоявшееся фразы и сокращения, согласно массиву выражений();
7. удаляем все союзы согласно массиву союзов();
*/

int getSizeStr(string text) {
	int counter = 0;

	while (text[counter] != '\0') counter++;

	return counter;
}

string deleteRepeatedWords(string text) {
	string word = "";

	for (int i = 0; text[i] != '\0'; i++) {
		if (text[i] != ' ') {
			word += text[i];
		}
		else {
			text = checkRepeatWord(text, word, i);
			word = "";
		}
	}

	return text;
}

string checkRepeatWord(string text, string word, int lastIndex) {
	string wordInText = "";
	int wordSize = 0;
	int textSize = getSizeStr(text);
	for (int i = lastIndex; i < textSize + 1; i++) {
		if (text[i] != ' ') {
			wordInText += text[i];
		}
		else if (word == wordInText) {
			wordSize = getSizeStr(wordInText);
			text = deleteStr(text, i - wordSize, i);
			wordInText = "";
		}
		else {
			wordInText = "";
		}


	}

	return text;
}

string deleteStr(string text, int firstIndex, int lastIndex) {
	for (int i = firstIndex; i < lastIndex; i++) {
		text[i] = ' ';
	}
	return text;
}

string deleteSpaces(string text) {
	int sizeText = getSizeStr(text);
	string newText = "";
	int counter = 0;
	for (int i = 0; i < sizeText - 1; i++) {
		if (text[i] == ' ' && counter == 0) {
			newText += ' ';
			counter = 1;
		}


		if (text[i] != ' ')
		{
			counter = 0;
			newText += text[i];
		}

	}
	return newText;
}

string deleteKeywords(string text, int index) {
	string word = "";

	for (int i = 0; text[i] != '\0'; i++) {
		if (text[i] != ' ') {
			word += text[i];
		}
		else if (isKeywords(word, index)) {
			int strSize = getSizeStr(word);
			text = deleteStr(text, i - strSize, i);
			word = "";
		}
		else {
			word = "";
		}
	}

	return text;
}

bool isKeywords(string word, int index) {
	int size = 0;
	switch (index)
	{
	case 1:
	{
		size = sizeof(keywordsToDelete) / sizeof(keywordsToDelete[0]);
		break;
	}
	case 2:
	{
		size = sizeof(arrUnions) / sizeof(arrUnions[0]);
		break;
	}
	default:
		size = 0;
		break;
	}

	for (int i = 0; i < size; i++) {
		if (word == keywordsToDelete[i]) return true;
	}

	return false;
}

string deletingSingleCharacterElements(string text) {
	for (int i = 0; text[i] != '\0'; i++) {
		if (text[i] != ' ' && text[i + 1] == ' ' && text[i - 1] == ' ') {
			text[i] = ' ';
		}
	}

	return text;
}

double countingPlagiarism(string text, string fragment) {
	int lastIndexFragment = 0, lastIndexText = 0;
	string threeWordsInText = text;
	string threeWordsInFragment;
	double possibleMatches = 0.0;
	double matches = 0.0;
	int counter = 0;

	while (true) {
		threeWordsInFragment = getThreeWords(threeWordsInFragment, lastIndexFragment, fragment, counter);
		if (counter != 3) break;
		possibleMatches++;
		counter = 0;
		lastIndexText = 0;
		for (int i = 0; text[lastIndexText + 1] != '\0'; i++) {
			threeWordsInText = getThreeWords(threeWordsInText, lastIndexText, text, counter);

			if (threeWordsInText == threeWordsInFragment)
			{
				matches++;
			}


			i = lastIndexText;

			if (counter != 3) break;
		}
	}

	return (100 * matches) / possibleMatches;
}

string getThreeWords(string words, int& lastIndex, string text, int& counter) {
	counter = 0;
	words = "";
	for (int i = lastIndex; counter < 3 && text[i] != '\0'; i++) {
		if (text[i] != ' ') {
			words += text[i];
		}
		else {
			words += ' ';
			counter++;
			if (counter == 1) {
				lastIndex = i + 1;
			}
		}
	}

	return words;
}
