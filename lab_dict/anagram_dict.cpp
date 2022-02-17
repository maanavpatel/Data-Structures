/**
 * @file anagram_dict.cpp
 * Implementation of the AnagramDict class.
 *
 * @author Matt Joras
 * @date Winter 2013
 */

#include "anagram_dict.h"

#include <algorithm> /* I wonder why this is included... */
#include <fstream>

using std::string;
using std::vector;
using std::ifstream;

/**
 * Constructs an AnagramDict from a filename with newline-separated
 * words.
 * @param filename The name of the word list file.
 */
AnagramDict::AnagramDict(const string& filename)
{
    /* Your code goes here! */
    ifstream temp(filename);
    string w;
    //basic file reading
    if (temp.is_open()) {
        while (getline(temp, w)) {
            string a = w;

            sort(w.begin(), w.end());
            dict[a].push_back(w);
        }
    }
}

/**
 * Constructs an AnagramDict from a vector of words.
 * @param words The vector of strings to be used as source words.
 */
AnagramDict::AnagramDict(const vector<string>& words)
{
    for (auto itr = words.begin(); itr != words.end(); itr++) {
  	    string s = *itr;
  	    sort(s.begin(), s.end());
  	    dict[s].push_back(*itr);
    }
}

/**
 * @param word The word to find anagrams of.
 * @return A vector of strings of anagrams of the given word. Empty
 * vector returned if no anagrams are found or the word is not in the
 * word list.
 */
vector<string> AnagramDict::get_anagrams(const string& word) const
{
    /* Your code goes here! */
    string key = word;
  	sort(key.begin(), key.end());

  	auto curr = dict.find(key);
  	if (curr == dict.end()) {
  	    return vector <string> ();
  	} else {
  	    auto r = curr->second;
  	    return r;
  	}
}

/**
 * @return A vector of vectors of strings. Each inner vector contains
 * the "anagram siblings", i.e. words that are anagrams of one another.
 * NOTE: It is impossible to have one of these vectors have less than
 * two elements, i.e. words with no anagrams are ommitted.
 */
vector<vector<string>> AnagramDict::get_all_anagrams() const
{
    /* Your code goes here! */
    vector<vector<string>> anagrams;
  	for (auto & itr: dict) {
  	    if (itr.second.size() > 1) {
  	        anagrams.push_back(itr.second);
  	    }
  	}
  	return anagrams;
}
