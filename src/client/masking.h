#include <string.h>
#include <stdlib.h>

// Prototype.
char *decrypt(const char *data);

// Encrypts data.
char *encrypt(const char *data)
{
    const size_t length = strlen(data);
    unsigned i;
    const unsigned short FLIP = 2;
    char *encrypted = (char *) malloc(length);

    for (i = 0; i < length; i++)
    {
        if (i % 2 == 0)
            encrypted[i] = data[i] - FLIP;

        else
            encrypted[i] = data[i] - 2 * FLIP;
    }

    return encrypted;
}

// Decrypts data.
char *decrypt(const char *data)
{
    const size_t length = strlen(data);
    unsigned i;
    const unsigned short FLIP = 2;
    char *decrypted = (char *) malloc(length);

    for (i = 0; i < length; i++)
    {
        if (i % 2 == 0)
            decrypted[i] = data[i] + FLIP;

        else
            decrypted[i] = data[i] + 2 * FLIP;
    }

    return decrypted;
}
