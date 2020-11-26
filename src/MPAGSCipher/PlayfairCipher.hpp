#ifndef MPAGSCIPHER_PLAYFAIRCIPHER_HPP
#define MPAGSCIPHER_PLAYFAIRCIPHER_HPP

#include <string>
#include <map>

#include "CipherMode.hpp"

/**
 * \file PlayfairCipher.hpp
 * \brief Contains the declaration of the PlayfairCipher class
 */

/**
 * \class PlayfairCipher
 * \brief Encrypt or decrypt text using the Playfair cipher with the given key
 */
class PlayfairCipher{
    public:
        /**
         * Create a new PlayfairCipher with the given key
         *
         * \param key the key to use in the cipher
         */
        explicit PlayfairCipher(const std::string& key);

        /**
         * Creates the key map used for encrypting/decrypting
         */
        void setKey(const std::string& key);

        /**
         * Apply the cipher to the provided text
         *
         * \param inputText the text to encrypt or decrypt
         * \param cipherMode whether to encrypt or decrypt the input text
         * \return the result of applying the cipher to the input text
         */
        std::string applyCipher( const std::string& inputText, const CipherMode cipherMode ) const;

        /// \param coord Key map coordinates. Goes like (row, column)
        using coord = std::pair<int , int>;

        /// \param Letter2Coord Reference the key map by its coordinate
        using Letter2Coord = std::map<char, coord>;
        
        /// \param Coord2Letter Reference the keymap by its letter
        using Coord2Letter = std::map<coord, char>;
        
        /// \param l2c_ stores the key map from letter -> coordinate
        Letter2Coord l2c_;
        
        /// \param c2l_ stores the key map from coordinate -> letter
        Coord2Letter c2l_;
    private:
        /// \param key_ Is a keyword in which the 5x5 matrix is copnstructed from, used to encode the diagraphs
        std::string key_;
        /// \param alphabet_ created to refernce when constructing the key map
        const std::string alphabet_ = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        

};


#endif