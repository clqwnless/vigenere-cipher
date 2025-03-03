#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


char* align_key_word(char *key_word, char *encrypt) // aligns key word and encrypt
{
    /*
        Есть другая реализация:
        у меня же функция возвращает указатель на новую измененный ключ key_word.
        По сути, у меня три переменные: encrypt, key_word, key_word_changed
        
        Я могу в этой функции скопировать массив key_word в другой массив через strcpy,
        а дальше соответственно уже изменить основной массив в аргументе в указателе.
        Мне ведь нужно на основе этого массива изменяться, так вот этот скопированный массив из
        этой функции потом "умирает", и у меня выходит только две переменные: encrypt, key_word_changed
        
        Экономия памяти, и + не нужен malloc, и освобождать память тоже.
        
        Хотя с другой стороны, я щас подумал, я же выделяю память потому, что не знаю,
        какая должна быть величина массива, а соответственно при таком раскладе нужно будет задать статичный массив.
        Мне ведь придётся повторять ещё ключ в случае блока else, когда len_encrypt > len_keyword
    */
    
    
    int len_keyword = strlen(key_word);
    int len_encrypt = strlen(encrypt);
    char *ptr;
    char *ptr_start;

    ptr = (char*)malloc((size_t)len_encrypt + 1);

    if (ptr == NULL) 
    {       
        return NULL;
    }
    
    ptr_start = ptr;
    
    if (len_encrypt <= len_keyword)
    {
        // "h!v"    encrypt
        // "rabs"   keyword
        
        // h || r
        // ! || a
        // v || b
        
        for (int i = 0; i < len_encrypt; ++i, ++ptr, ++encrypt)
        {
            if (isalpha(*encrypt))
            {
               *ptr = *key_word;
               
               ++key_word;
            } else {
                *ptr = *encrypt;
            }
        }
    } else {
        // "rabb2i"
        // "hell2o"
        
        // "rabb2i" -> t
        
        // "rabbit"i          keyword
        // "hell2ow!orl"      encrypt
        // "encrypt > keyword"

        for (int i = 0; i < len_encrypt; ++i, ++ptr, ++encrypt)
        {
            if (*key_word == '\0') {
                key_word = key_word - len_keyword;
            }
            
            if (isalpha(*encrypt))
            {
                *ptr = *(key_word++);                
            } else {
                *ptr = *encrypt;
            }
        }
    }
    
    *ptr = '\0';

    return ptr_start;
    
    // h e l l o            encrypt
    // r a b b i t          keyword
    // encrypt < keyword
    // encrypt > keyword
    
    // h e l l o              encrypt
    // r a b b i t            keyword
    
    // h e l l o o o          encrypt (enc % key = 19 % 6 = 1)
    // r a b b i t(r)         keyword
    
    // a b b r e v i a t i o n (if a % b == 0 (int c = a / b))
    // r a b r a b r a b r a b
    
    // printf("%d\n", 19 % 6);
    
    // 19 % 6 = 1
    // повторить шестёрку (19 - 1) / 6 = 3 раз и 1 добавить в конце
    // - 1 необязательно, там оператор "/" вернёт тот же результат и без вычитания.
}


void get_alph(int start_char, char *copy)
{
    if (start_char == *copy)
    {
        // the case when alphabets equal
        
        return;
    }
    
    // works only in ascii
    
    for (int i = start_char; i <= 'z'; i++)
    {
        *(copy++) = (char)i;
    }
    
    for (int i = 'a'; i < start_char; i++)
    {
        *(copy++) = (char)i;
    }
    
    // *copy = '\0';
}


// inline
int get_index(int ch)
{
    return ch - 'a';
}

int contains_alpha(char *s)
{
    while (*s)
    {
        if (isalpha(*s) == 0 && *s != ' ' && *s != '\n')
        {
            // the string contains a char which is not alpha
            
            return 0;
        }
        
        ++s;
    }
    
    // the string does not contain a char which is not alpha (all chars are alpha)
    
    return 1;
}



int main(void)
{
    
    // не забыть, что key_word может содержать только символы (добавить проверку на это)
    
    char key_word[2048];
    char encrypt[2048];
    char *pKey_word = &key_word[0];
    char *pEncrypt = &encrypt[0];
    char copy[26];
    int index;
    
    printf("\nEnter a key word: ");
    fgets(pKey_word, 2047, stdin);
    
    if (contains_alpha(pKey_word) == 0)
    {
        fprintf(stderr, "\nERROR: The Key Word, which was entered, contains a char, which does not belong to english alphabet\n");
        
        return 1;
    }
    
    printf("\nEnter a message to encrypt: ");
    fgets(pEncrypt, 2047, stdin);
    
    key_word[strcspn(pKey_word, "\n")] = 0;
    encrypt[strcspn(pEncrypt, "\n")] = 0;
    
    if (key_word[0] == '\0' || encrypt[0] == '\0')
    {
        fprintf(stderr, "\nERROR: String(s) cannot be empty\n");
        
        return 2;
    }
    
    for (; *pKey_word != '\0'; ++pKey_word) *pKey_word = tolower(*pKey_word);
    for (; *pEncrypt != '\0'; ++pEncrypt) *pEncrypt = tolower(*pEncrypt);
    
    pKey_word = pKey_word - strlen(key_word);
    pEncrypt = pEncrypt - strlen(encrypt);
    
    char *ptr = align_key_word(pKey_word, pEncrypt);
    
    if (ptr != NULL)
    {
        char *result = (char*)malloc(strlen(encrypt));
        char *start_result = result;
        
        if (result != NULL)
        {
            while (*ptr)
            {
                // "rabbit"
                // "hell2ow!orl"
                
                // "rabb2it!rab"
                
                if (isalpha(*ptr))
                {
                    get_alph(*ptr, copy);
                    
                    index = get_index(*pEncrypt);
                    
                    *result = copy[index];
                } else {
                    *result = *ptr;
                }
                
                ++ptr;
                ++pEncrypt;
                ++result;
            }
            
            *result = '\0';
            
            printf("%s\n", start_result);
            
            free(ptr);
            
            return 0;
        } else {
            fprintf(stderr, "An error occurred while requesting memory for result\n");
            
            free(ptr);
            
            return 3;
        }
    } else {
        fprintf(stderr, "An error occurred while requesting memory for key_word pointer\n");
    }

    return 4;
}
