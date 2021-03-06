// Descriptor.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include "pch.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>
using namespace std;

int text_amount = 2436;

vector<string> stopwords = { "-","a", "about", "above", "after", "again", "against", "ain", "all", "am", "an", "and", "any", "are", "aren", "aren't", "as", "at", "be", "because", "been", "before", "being", "below", "between", "both", "but", "by", "can",
	"could", "couldn", "couldn't", "d", "did", "didn", "didn't", "do", "does", "doesn", "doesn't", "doing", "don", "don't", "down", "during", "each", "few", "for", "from", "further", "had", "hadn", "hadn't", "has", "hasn",
	"hasn't", "have", "haven", "haven't", "having", "he", "he'd", "he'll", "he's", "her", "here", "here's", "hers", "herself", "him", "himself", "his", "how", "how's", "i", "i'd", "i'll", "i'm", "i've", "if", "in", "into", "is",
	"isn", "isn't", "it", "it's", "its", "itself", "just", "let's", "ll", "m", "ma", "me", "mightn", "mightn't", "more", "most", "mustn", "mustn't", "my", "myself", "needn", "needn't", "no", "nor", "not", "now", "o", "of", "off",
	"on", "once", "only", "or", "other", "ought", "our", "ours", "ourselves", "out", "over", "own", "re", "s", "same", "shan", "shan't", "she", "she'd", "she'll", "she's", "should", "should've", "shouldn", "shouldn't", "so",
	"some", "such", "t", "than", "that", "that'll", "that's", "the", "their", "theirs", "them", "themselves", "then", "there", "there's", "these", "they", "they'd", "they'll", "they're", "they've", "this", "those", "through",
	"to", "too", "under", "until", "up", "ve", "very", "was", "wasn", "wasn't", "we", "we'd", "we'll", "we're", "we've", "were", "weren", "weren't", "what", "what's", "when", "when's", "where",
	"where's", "which", "while", "who", "who's", "whom", "why", "why's", "will", "with", "won", "won't", "would", "wouldn", "wouldn't", "y", "you", "you'd", "you'll", "you're", "you've", "your", "yours", "yourself", "yourselves" };

template<class T>
bool Search(vector<T> &vec, T x) {
	for (size_t i = 0; i < vec.size(); ++i) {
		if (vec[i] == x)
			return 1;
	}
	return 0;
}

bool stopword(string word) {
	return binary_search(stopwords.begin(), stopwords.end(), word);
}

void correct(string &word) {
	if (word.find("'") < word.length()) 
		word = word.substr(0, word.find("'"));
}

void readText(vector<string> &dictionary, string route) {
	ifstream text(route);
	string word;
	while (text >> word) {
		word.erase(remove_if(word.begin(), word.end(), ::ispunct), word.end());
		if (!stopword(word))
		{
			correct(word);
			transform(word.begin(), word.end(), word.begin(), ::tolower);
			if (!Search(dictionary, word))
				dictionary.push_back(word);
		}
	}
}

vector<string> initialize_dictionary() {
	vector<string> dictionary;
	for (int i = 0; i <= text_amount; ++i)
		readText(dictionary, "C:/Users/Luis/Downloads/Texts/text (" + to_string(i + 1) + ").txt");
	sort(dictionary.begin(), dictionary.end());
	return dictionary;
}

int find_word(vector<string> arr, int l, int r, string x)
{
	if (r >= l) {
		int mid = l + (r - l) / 2;
		if (arr[mid] == x)
			return mid;
		if (arr[mid] > x)
			return find_word(arr, l, mid - 1, x);
		return find_word(arr, mid + 1, r, x);
	}
	return -1;
}

void descript(vector<vector<int>> &result, vector<string> &dictionary, int i) {
	cout << i+1 << endl;
	ifstream text("C:/Users/Luis/Downloads/Texts/text (" + to_string(i + 1) + ").txt");
	string word;
	int idx;
	while (text >> word) {
		word.erase(remove_if(word.begin(), word.end(), ::ispunct), word.end());
		if (!stopword(word)) {
			transform(word.begin(), word.end(), word.begin(), ::tolower);
			idx = find_word(dictionary, 0, dictionary.size() - 1, word);
			if (idx >= 0)
				++result[i][idx];
		}
	}
}

int main(int argc, char *argv[]) {
	vector<string> dictionary = initialize_dictionary();
	int tam = dictionary.size();
	vector<vector<int>> result(text_amount);
	for (int i = 0; i < text_amount; ++i)
		result[i].assign(tam, 0);
	for (int i = 0; i < text_amount; ++i)
		descript(result, dictionary, i);
	ofstream text3("C:/Users/Luis/Downloads/Texts/results.txt");
	for (size_t i = 0; i < result.size(); ++i) {
		for (size_t j = 0; j < result[i].size(); ++j)
			text3 << result[i][j] << ";";
		text3 << endl;
	}
	return 0;
}
// Ejecutar programa: Ctrl + F5 o menú Depurar > Iniciar sin depurar
// Depurar programa: F5 o menú Depurar > Iniciar depuración

// Sugerencias para primeros pasos: 1. Use la ventana del Explorador de soluciones para agregar y administrar archivos
//   2. Use la ventana de Team Explorer para conectar con el control de código fuente
//   3. Use la ventana de salida para ver la salida de compilación y otros mensajes
//   4. Use la ventana Lista de errores para ver los errores
//   5. Vaya a Proyecto > Agregar nuevo elemento para crear nuevos archivos de código, o a Proyecto > Agregar elemento existente para agregar archivos de código existentes al proyecto
//   6. En el futuro, para volver a abrir este proyecto, vaya a Archivo > Abrir > Proyecto y seleccione el archivo .sln
