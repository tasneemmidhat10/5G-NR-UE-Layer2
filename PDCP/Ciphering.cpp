#include <iostream>
#include <vector>
#include <cstring>
#include <cstdint>

class PDCP_Cipher
{
public:
    PDCP_Cipher(const std::vector<uint8_t> &key, const std::vector<uint8_t> &iv)
        : key_(key), iv_(iv) {}

    std::vector<uint8_t> encrypt(const std::vector<uint8_t> &plaintext)
    {
        return aes_ctr_crypt(plaintext);
    }

    std::vector<uint8_t> decrypt(const std::vector<uint8_t> &ciphertext)
    {
        return aes_ctr_crypt(ciphertext);
    }
    void initCryptoVar(){
        std::vector<uint8_t> key(16, 0x01); // Example 128-bit key
        std::vector<uint8_t> iv(16, 0x02);  // Example IV
    }

private:
    std::vector<uint8_t> key_;
    std::vector<uint8_t> iv_;

    std::vector<uint8_t> aes_ctr_crypt(const std::vector<uint8_t> &input)
    {
        std::vector<uint8_t> output(input.size());
        std::vector<uint8_t> counter = iv_;

        for (size_t i = 0; i < input.size(); i++)
        {
            uint8_t keystream_byte = key_[i % key_.size()] ^ counter[i % counter.size()];
            output[i] = input[i] ^ keystream_byte;
            counter[i % counter.size()]++;
        }
        return output;
    }
};