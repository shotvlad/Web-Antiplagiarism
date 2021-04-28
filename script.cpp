#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <cgicc/CgiDefs.h>
#include <cgicc/Cgicc.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>

using namespace std;
using namespace cgicc;

string keywordsToDelete[] = { "�.�.","�.�.","��.","��.","��.","��.","���","��","��","�","�","�","�","�" };
string arrUnions[] = { "��", "�", "��"," ��","���","����", "��", "�", "���", "�����", "���", "���" };

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

string getDB();//get origin text from db.txt (don't modify tis function)

int main()
{

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

	//

    if (!name.empty()) {
		cout << round(100 - antiPlagiarism(getDB(), name)) << "\n";
    } else {
    	cout << "Text is not provided!\n";
    }	

    cout << "</p>\n";
    cout << "</body>\n";
    cout << "</html>\n";

    return 5;
}

string getDB() {
	std::ifstream in("db1.txt");

	string dbText = "", s1;

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
		return '�';
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
		return '�';
	}
	else if (text == 'K')
	{
		return '�';
	}
	else if (text == 'E')
	{
		return '�';
	}
	else if (text == 'e')
	{
		return '�';
	}
	else if (text == 'H')
	{
		return '�';
	}
	else if (text == 'X')
	{
		return '�';
	}
	else if (text == 'x')
	{
		return '�';
	}
	else if (text == 'B')
	{
		return '�';
	}
	else if (text == 'A')
	{
		return '�';
	}
	else if (text == 'a')
	{
		return '�';
	}
	else if (text == 'P')
	{
		return '�';
	}
	else if (text == 'p')
	{
		return '�';
	}
	else if (text == 'O')
	{
		return '�';
	}
	else if (text == 'o')
	{
		return '�';
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
		return '�';
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
	char characters[] = "!.?,():;��+-*/=%�";

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
5. �������� ����� �� ������������� ����� � ������� ���� ������� ����(); +
6. ������� ��� ����������� ����� � ����������, �������� ������� ���������();
7. ������� ��� ����� �������� ������� ������();
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
