#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cstdio>
#include <algorithm>

using namespace std;

map<string, vector<int> > wordLocation;

bool isWord(const string& str);
string toLower(const string& str);
vector<string> readFile();
double sentenceValidProbability(const vector<string>& sentence);
int numberOfAdjacency(const string& first, const string& second);
void showProgressBar(int current, int total);
void endProgressBar();

int main() {
    printf("Reading files...\n");
    vector<string> words = readFile();
    printf("Reading finished\nProcessing files...\n");

    for (int i = 0; i < words.size(); i++) {
        showProgressBar(i + 1, words.size());
        if (wordLocation.find(words[i]) == wordLocation.end()) {
            wordLocation.insert(pair<string, vector<int> >(words[i], vector<int>()));
        } else {
            wordLocation[words[i]].push_back(i);
        }
    }
    endProgressBar();
    printf("Processing finished\n");

    string word;
    vector<string> sentence;
    while (cin >> word) {
        sentence.push_back(word);
    }

    double probability = sentenceValidProbability(sentence);
    cout << "Probability: " << probability << endl;
    
    return 0;
}

void showProgressBar(int progressBarCount, int progressBarTotal) {
    char bar[64] = "                                                  ";

    int percent = progressBarCount * 100 / progressBarTotal;
    for (int i = 0; i <= percent / 2 - 1; i++) {
        bar[i] = '#';
    }
    printf("[%s] %d%%\r", bar, percent);
    fflush(stdout);
}
void endProgressBar() {
    printf("\n");
}
double sentenceValidProbability(const vector<string>& sentence) {
    printf("Analyzing sentence...\n");
    double sum = 0.0;
    for (int i = 1; i < sentence.size(); i++) {
        showProgressBar(i + 1, sentence.size());
        int numberOfFirst;
        if (wordLocation.find(sentence[i - 1]) == wordLocation.end()) {
            continue;
        } else {
            numberOfFirst = wordLocation[sentence[i - 1]].size();
        }
        double probability = double(numberOfAdjacency(sentence[i - 1], sentence[i])) / double(numberOfFirst);
        sum += probability;
    }
    endProgressBar();
    if (sentence.size() == 1)
        return 0.0;
    return sum / double(sentence.size() - 1);
}
int numberOfAdjacency(const string& first, const string& second) {
    int ret = 0;
    auto iter1 = wordLocation.find(first);
    if (iter1 == wordLocation.end()) {
        return 0;
    }
    auto iter2 = wordLocation.find(second);
    if (iter2 == wordLocation.end()) {
        return 0;
    }
    const auto locations1 = iter1->second;
    const auto locations2 = iter2->second;
    for (int i = 0; i < locations1.size(); i++) {
        auto ptr = lower_bound(locations2.begin(), locations2.end(), locations1[i] + 1);
        if (*ptr == locations1[i] + 1) {
            ret++;
        }
    }
    return ret;
}
bool isWord(const string& str) {
    for (int i = 0; i < str.length(); i++) {
        if (!(('a' <= str[i] && str[i] <= 'z') ||
        ('A' <= str[i] && str[i] <= 'Z'))) {
            return false;
        }
    }
    return true;
}
string toLower(const string& str) {
    string ret = str;
    for (int i = 0; i < ret.length(); i++) {
        if ('A' <= ret[i] && ret[i] <= 'Z') {
            ret[i] = ret[i] + 'a' - 'A';
        }
    }
    return ret;
}
vector<string> readFile() {
    vector<string> fileList;
    fileList.push_back("text0/9053.txt");
    fileList.push_back("text0/12053.txt");
    fileList.push_back("text0/17053.txt");
    fileList.push_back("text0/19053.txt");
    fileList.push_back("text0/27053.txt");
    fileList.push_back("text0/33053.txt");
    fileList.push_back("text0/46053.txt");

    vector<string> words;
    for (auto fileName: fileList) {
        ifstream infile;
        infile.open(fileName);
        string word;
        while (infile >> word) {
            if (isWord(word)) {
                words.push_back(toLower(word));
            }
        }
        words.push_back("_");
        infile.close();
    }
    return words;
}