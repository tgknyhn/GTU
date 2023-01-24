// Author : Ahmet Tuğkan Ayhan

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define BLOCK_SIZE 240
#define KEY_SIZE 240

static uint8_t lfsr[BLOCK_SIZE];
static uint8_t nfsr[BLOCK_SIZE];

static inline uint8_t get_bit(const uint8_t *state, size_t index)
{
    return (state[index / 8] >> (index % 8)) & 1;
}

static inline void set_bit(uint8_t *state, size_t index, uint8_t value)
{
    state[index / 8] ^= (-value ^ state[index / 8]) & (1 << (index % 8));
}

/*
 * Initialize the Grain-128AEAD cipher with the given key.
 */
void grain128_init(const uint8_t *key)
{
    memcpy(lfsr, key, KEY_SIZE);
    memcpy(nfsr, key + KEY_SIZE, KEY_SIZE);
}

void grain128_encrypt_block(const uint8_t *input, uint8_t *output)
{
    uint8_t i, t[BLOCK_SIZE];
    uint8_t bit, feedback;

    // Generate keystream
    for (i = 0; i < BLOCK_SIZE; i++) {
        bit = get_bit(lfsr, 0) ^ get_bit(nfsr, 0);
        set_bit(t, i, bit);
        feedback = get_bit(lfsr, 8) ^ get_bit(nfsr, 10) ^ get_bit(nfsr, 11) ^ get_bit(nfsr, 16);
        set_bit(lfsr, 0, get_bit(lfsr, 15) ^ feedback);
        set_bit(nfsr, 0, get_bit(nfsr, 7) ^ feedback);
    }

    // XOR keystream with input to produce output
    for (i = 0; i < BLOCK_SIZE; i++) {
        output[i] = input[i] ^ t[i];
    }
}

void grain128_decrypt_block(const uint8_t *input, uint8_t *output)
{
    // Decryption is the same as encryption, so we can just call grain128_encrypt_block
    grain128_encrypt_block(input, output);
}


/*
 * Encrypt a message using Grain-128AEAD.
 *
 * The 'input' buffer contains the plaintext message, and 'output'
 * will be filled with the ciphertext. 'key' is the encryption key,
 * and 'nonce' is a unique value used to initialize the cipher.
 * 'ad' is a buffer containing associated data that will be
 * authenticated but not encrypted. 'ad_len' is the length of the
 * associated data.
 */
void grain128_encrypt(const uint8_t *input, uint8_t *output,
                      const uint8_t *key, const uint8_t *nonce,
                      const uint8_t *ad, size_t ad_len, size_t size)
{
    uint8_t block[BLOCK_SIZE];

    // Initialize the cipher with the key and nonce
    grain128_init(key);
    memcpy(lfsr, nonce, BLOCK_SIZE);

    // Process the associated data
    while (ad_len >= BLOCK_SIZE) {
        grain128_encrypt_block(ad, block);
        ad += BLOCK_SIZE;
        ad_len -= BLOCK_SIZE;
    }
    if (ad_len > 0) {
        memset(block, 0, BLOCK_SIZE);
        memcpy(block, ad, ad_len);
        grain128_encrypt_block(block, block);
    }

    // Process the message
    while (size >= BLOCK_SIZE) {
        grain128_encrypt_block(input, output);
        input += BLOCK_SIZE;
        output += BLOCK_SIZE;
        size -= BLOCK_SIZE;
    }
    if (size > 0) {
        // Encrypt the final block of the message
        memset(block, 0, BLOCK_SIZE);
        memcpy(block, input, size);
        grain128_encrypt_block(block, block);

        // Copy the encrypted final block to the output buffer
        memcpy(output, block, size);
    }
}


/*
 * Decrypt a message using Grain-128AEAD.
 *
 * The 'input' buffer contains the ciphertext, and 'output' will be
 * filled with the plaintext. 'key' is the decryption key, and
 * 'nonce' is the unique value used to initialize the cipher. 'ad'
 * is a buffer containing the associated data that was authenticated
*/

void grain128_decrypt(const uint8_t *input, uint8_t *output,
                      const uint8_t *key, const uint8_t *nonce,
                      const uint8_t *ad, size_t ad_len, size_t size)
{
    uint8_t block[BLOCK_SIZE];

    // Initialize the cipher with the key and nonce
    grain128_init(key);
    memcpy(lfsr, nonce, BLOCK_SIZE);

    // Process the associated data
    while (ad_len >= BLOCK_SIZE) {
        grain128_encrypt_block(ad, block);
        ad += BLOCK_SIZE;
        ad_len -= BLOCK_SIZE;
    }
    if (ad_len > 0) {
        memset(block, 0, BLOCK_SIZE);
        memcpy(block, ad, ad_len);
        grain128_encrypt_block(block, block);
    }

    // Process the message
    while (size >= BLOCK_SIZE) {
        grain128_decrypt_block(input, output);
        input += BLOCK_SIZE;
        output += BLOCK_SIZE;
        size -= BLOCK_SIZE;
    }
    if (size > 0) {
        // Decrypt the final block of the message
        memset(block, 0, BLOCK_SIZE);
        memcpy(block, input, size);
        grain128_decrypt_block(block, block);
        memcpy(output, block, size);
    }
}


int main(int argc, char **argv)
{
    // Set up the key, nonce, and message
    uint8_t key[KEY_SIZE] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                              0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 
                              0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
                              0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
                              0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
                              0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
                              0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
                              0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f,
                              0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
                              0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
                              0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
                              0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f,
                              0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
                              0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f,
                              0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,
                              0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f,
                              0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
                              0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
                              0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
                              0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f,
                              0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7};
    uint8_t nonce[BLOCK_SIZE] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                              0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 
                              0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
                              0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
                              0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
                              0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
                              0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
                              0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f,
                              0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
                              0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
                              0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
                              0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f,
                              0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
                              0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f,
                              0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,
                              0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f,
                              0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
                              0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
                              0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
                              0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f,
                              0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7};

    uint8_t message[BLOCK_SIZE] = "This is text message";
    size_t message_len = strlen((char *)message);


    // Allocate buffers for the encrypted and decrypted messages
    uint8_t *encrypted_message = malloc(message_len);
    uint8_t *decrypted_message = malloc(message_len);

    // Encrypt the message
    grain128_encrypt(message, encrypted_message, key, nonce, NULL, 0, message_len);

    // Decrypt the message
    grain128_decrypt(encrypted_message, decrypted_message, key, nonce, NULL, 0, message_len);


    // Print the original and decrypted messages
    printf("\nOriginal message: %s\n\n", message);
    printf("Encrypted message: {");
    for (size_t i = 0; i < message_len; i++)
    {
        printf("0x%02x", encrypted_message[i]);
        if (i < message_len - 1)
            printf(", ");
    }
    printf("}\n");
    printf("Encrypted message: %s\n\n", encrypted_message);

    printf("Decrypted message: %s\n\n", decrypted_message);

    free(encrypted_message);
    free(decrypted_message);

    return 0;
}

