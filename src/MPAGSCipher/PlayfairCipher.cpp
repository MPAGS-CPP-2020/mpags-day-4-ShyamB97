#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include "PlayfairCipher.hpp"
#include <iostream>

PlayfairCipher::PlayfairCipher(const std::string& key)
    : key_{key}
{

}

void PlayfairCipher::setKey(const std::string& key)
{
    // store the original key
    key_ = key;
    // Append the alphabet
    key_ += alphabet_;
    // Make sure the key is upper case
    std::transform(key_.begin(), key_.end(), key_.begin(), ::toupper);
    // Remove non-alpha characters
    auto remove_iter = std::remove_if(key_.begin(), key_.end(), [] (char i) {return !::isalpha(i);});
    key_.erase(remove_iter,key_.end());
    // Change J -> I
    std::transform(key_.begin(), key_.end(), key_.begin(), [] (char i) {return (i == 'J') ? 'I': i; });
    // Remove duplicated letter
    std::string encountered; // string we already found
    auto remove_iter1 = std::remove_if(key_.begin(), key_.end(), [&] (char i)
    {
        if(encountered.find(i)<encountered.length())
        {
            return true;
        }
        else
        {
            encountered += i;
            return false;
        }
    });
    key_.erase(remove_iter1, key_.end());
    // Store the coords of each letter
    std::vector<coord> coords;
    for(int c = 0; c < 5; c++)
    {
        for (int r = 0; r < 5; r++)
        {
            coord co {c, r};
            coords.push_back(co);
        }
        
    }

    // Store the playfair cihper key map
    for(int i = 0; i < 25; i++)
    {
        Letter2Coord::value_type p0 {key_[i], coords[i]};
        Coord2Letter::value_type p1 {coords[i], key_[i]};
        l2c_.insert(p0);
        c2l_.insert(p1);
    }
}

std::string PlayfairCipher::applyCipher( const std::string& inputText, const CipherMode cipherMode ) const
{
    // wheather we right-sfit or left-shift on the key map, depends on encryption
    int shift {1};
    if(cipherMode == CipherMode::Encrypt)
    {
        shift = 1;
    }
    if(cipherMode == CipherMode::Decrypt)
    {
        shift = -1;
    }

    std::vector<std::string> diagraphs;
    size_t textSize = inputText.size();
    std::string input {inputText};
    std::string outputText {""};
    // Change J → I
    std::transform(input.begin(), input.end(), input.begin(), [] (char i) {return (i == 'J') ? 'I': i; });
    
    // If repeated chars in a digraph add an X or Q if XX
    size_t j {0};
    while(j < textSize)
    {
        std::string diagraph {' '};
        std::string l1;
        l1.push_back(input[j]);
        if(j != textSize - 1)
        {
            std::string l2;
            l2.push_back(input[j+1]);
            if(l1 == l2 && l1 != "X")
            {
                l2 = "X";
                j++;
            }
            else if (l1 == l2 && l1 == "X")
            {
                l2 = "Q";
                j++;
            }
            else
            {
                j+=2;
            }
            diagraph = l1 + l2;
        }
        else
        {
            if(l1 != "Z")
            {
                diagraph = l1 + "Z";
            }
            else if(l1 == "Z")
            {
                diagraph = l1 + "X";
            }
            j++;
        }
        diagraphs.push_back(diagraph);
    }

    // Loop over the input in Digraphs and decrypt/encrypt
    for(std::string d : diagraphs)
    {
        coord c1 = l2c_.at(d[0]);
        coord c2 = l2c_.at(d[1]);
        if(c1.first == c2.first)
        {
            //they are on the same row
            c1.second = ((c1.second + shift)%5 + 5 )%5; //need to employ modulo formula which works with -ve numbers
            c2.second = ((c2.second + shift)%5 + 5 )%5;
        }
        else if (c1.second == c2.second)
        {
            //they are on the same column
            c1.first = ((c1.first + shift)%5 + 5 )%5;
            c2.first = ((c2.first + shift)%5 + 5 )%5;
        }
        else
        {
            //they are form a box
            int tmp {c2.second};
            c2.second = c1.second;
            c1.second = tmp;
        }
        char s1 = c2l_.at(c1);
        char s2 = c2l_.at(c2);
        std::string s = std::string(1, s1) + s2;
        outputText += s;
    }
    return outputText;
}