/************|************|************|
Program: TT01_A2_Baajajah_Ayman.cpp
Course: OOPDS
Year: 2018/19 Trimester 2
Name: Ba-Ajajah, Ayman Khaled Omar
ID: 1161302846
Lecture Section: TC01
Tutorial Section: TT01
Email: a.baajaja@gmail.com
Phone: 013-252 1340
************|************|************/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>

using namespace std;

// class responsible to build words from text files
class WordBuilder{

private:
//	word details
	string word;	//	input word
	int lineNumber;	// line number wher word exist in file
	string lineText;	// all line of text that has this word
	string fileName;	//	file name that contains the word

public:
//	word constructor by word details
	WordBuilder(string w, int ln, string lt, string fn){
		word=w;
		lineNumber=ln;
		lineText=lt;
		fileName=fn;
	}

// return word
	string getWord(){
		return word;
	}

//	return file name
	string getFileName(){
		return fileName;
	}

// print out word details
	void displayWord(){
		cout<<"  Line "<<lineNumber<<'\t'<<lineText<<endl;
	}
};

// class responsible for creating word indices
class WordIndices{

private:
	char index;	//	word index which is the first char of the word
	vector<WordBuilder> words;	//	list of word having same index

public:
// construct word group by index
	WordIndices(char i){
		index=i;
	}

// return word index
	char getIndex(){
		return index;
	}

// add word to this index
	void createWordIndex(WordBuilder word){
		words.push_back(word);
	}

// search for the word in this index
	bool search(string searchWord){

		bool matched=false;
		string fileName= ""; // file name where word is found

		for(int i=0; i<words.size(); i++){	// loop through words in this index
			if(words[i].getWord() == searchWord){	// check words if matches the required word
				if(fileName != words[i].getFileName()){
					fileName= words[i].getFileName();	//	print file name where word is found
					cout << endl << endl << "  File Name: "<< fileName <<endl;	 
				}
				words[i].displayWord();	//	print the matched word details
				matched=true;	
			}		
		}
		return matched;	// return true if match is found or return false if match is not found	
	}
};

// remove non alphanumeric from the passed word
void deleteNonAlph(string& word){
	for(int i=0; i < word.length(); i++){	// loop through word char
		if(!isalpha(word[i]) && !isdigit(word[i])){	//	chech if word has non alphanumeric
			word = word.substr(0, i) + word.substr(i+1, word.length());	//	remove the found non alphanumeric from word
			i--;
		}
	}
}

// return place of the index inside the indices list 
int getWordIndex(vector<WordIndices> wordIndices, char index){
	for(int i=0; i<wordIndices.size(); ++i)	// look for index in the indices list
		if(wordIndices[i].getIndex() == index)
			return i;	// return place of index if found
	return -1; // return -1 if place of index is not found in the indices list
}

// check if the passed word on of the stop words
bool isStopWord(set<string> stopWords, string word){
	if(stopWords.find(word) != stopWords.end())
		return true; // return true if word is a stop word
	return false;	// return false if word is not a stop word
}

// read word from input files and return list of indexed words
vector<WordIndices> inputWords(){

	vector<WordIndices> wordIndices;	// vector to index input words
	set<string> stopWords;	//	set to store stop words

	ifstream wordsReader;
	string line, word;

	wordsReader.open("StopWords.txt"); 	// open stop word text file
	  
    while (wordsReader >> word) 	// read stop words
    {
    	deleteNonAlph(word);	// remove non alphanumeric from stop words
		stopWords.insert(word);	// insert stop word in stop word set
    }

    wordsReader.close();	//	close stop word text file

	vector<string> fileNames = {"1to10.Text.Files/1.txt", "1to10.Text.Files/2.txt", "1to10.Text.Files/3.txt",
								"1to10.Text.Files/4.txt", "1to10.Text.Files/5.txt", "1to10.Text.Files/6.txt",
								"1to10.Text.Files/7.txt", "1to10.Text.Files/8.txt", "1to10.Text.Files/9.txt",
								"1to10.Text.Files/10.txt"};

	for(int i=0; i<10; i++){ // loop for 10 input files

		int lineNumber = 0;	// keep track of line number

		ifstream wordsReader(fileNames[i]);	// open input file

		while (getline(wordsReader, line))	// read lines in the file 
	    {	
	    	lineNumber++; // increase line number

	        if(line.empty())	// check if the line is empty
	        	continue;	// skip reading words if line is empty

        	istringstream ss(line);	// stream line

        	while(ss >> word){	// read words in line

        		deleteNonAlph(word);	//	remove alphanumeric from input word

	    		if(!word.empty() && !isStopWord(stopWords, word)){	// check if word is not empty and is not stop word
	    			WordBuilder newWord(word, lineNumber, line, fileNames[i]);	// create new word using given details
	    			int exist = getWordIndex(wordIndices, word[0]);
		    		if(exist != -1)
		    			wordIndices[exist].createWordIndex(newWord);	// insert words in the index

		    		else{
		    			WordIndices newWordIndex(word[0]);	// create new index
		    			wordIndices.push_back(newWordIndex);	// insert words in the new index
		    			wordIndices[wordIndices.size()-1].createWordIndex(newWord); //	add new index to the indices list
		    		}
	    		}
        	}
	    }
	    wordsReader.close();	// close input file
	}
	return wordIndices; // return list of word indices that store words by index
}

// search word in the indices 
bool searchForWord(vector<WordIndices> wordIndices, string searchWord){

	for(int i=0; i<wordIndices.size(); i++){ // look for the index that matches the index of the required word
		if(wordIndices[i].getIndex() == searchWord[0])
			return wordIndices[i].search(searchWord); // look for the word in the matched index
	}

	return false;	// return false if word not found
}

int main(){

	vector<WordIndices> wordIndices = inputWords();

	char loop = 'Y';

    while(loop == 'y' || loop == 'Y'){	// loop for another searches

    	string searchWord;

    	cout << endl << "Enter the word you are looking for: ";
    	cin >> searchWord;	// read required search word

    	if(!searchForWord(wordIndices, searchWord)){ // check if word is found
    		cout << endl << "All files are checked but no match is found!" << endl;
    	}

    	cout << endl << "Search for another Word (y/n) --> ";
    	cin >> loop; // do you want to search another word
 
    }
	
	return 0;
}