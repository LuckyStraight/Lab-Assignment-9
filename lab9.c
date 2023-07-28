#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
    int id;
    char name;
    int order;
    struct RecordType* next; // Pointer for chaining
};

// Define the HashType structure for separate chaining hash table
struct HashType
{
    struct RecordType* head; // Pointer to the first record in the chain
    int count; // Number of records in the chain
};

int hash(int x, int hashSz);
int parseData(char* inputFileName, struct RecordType** ppData);
void printRecords(struct RecordType pData[], int dataSz);
void displayRecordsInHash(struct HashType* pHashArray, int hashSz);

int main(void)
{
    struct RecordType* pRecords;
    int recordSz = 0;

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    // Define the hash table size
    int hashSz = 15; // Fixed size of the hash table

    // Create the hash table and initialize each entry
    struct HashType* pHashArray = (struct HashType*)malloc(sizeof(struct HashType) * hashSz);

    if (pHashArray == NULL)
    {
        printf("Cannot allocate memory for the hash table\n");
        exit(-1);
    }

    // Initialize each entry in the hash table
    int i;

    for (i = 0; i < hashSz; ++i)
    {
        pHashArray[i].head = NULL;
        pHashArray[i].count = 0;
    }

    // Insert records into the hash table
    for (i = 0; i < recordSz; ++i)
    {
        struct RecordType* pRecord = pRecords + i;
        int index = hash(pRecord->id, hashSz);

        // Insert the record at the beginning of the chain
        pRecord->next = pHashArray[index].head;
        pHashArray[index].head = pRecord;
        pHashArray[index].count++;
    }

    // Display the records in the hash table
    displayRecordsInHash(pHashArray, hashSz);

    // Free the dynamically allocated memory
    free(pRecords);
    free(pHashArray);

    return 0;
}


// Compute the hash function
int hash(int x, int hashSz)
{
    // Using modulo to convert the key into an index within the hash table size
    return x % hashSz;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType* pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*)malloc(sizeof(struct RecordType) * dataSz);

        // Implement parse data block
        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }

        for (i = 0; i < dataSz; ++i)
        {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
            pRecord->next = NULL; // Initialize the next pointer to NULL
        }

        fclose(inFile);
    }

    return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
    int i;
    printf("\nRecords:\n");

    for (i = 0; i < dataSz; ++i)
    {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType* pHashArray, int hashSz)
{
    int i;
    struct RecordType* pRecord;

    for (i = 0; i < hashSz; ++i)
    {
        pRecord = pHashArray[i].head;

        if (pRecord != NULL)
        {
            printf("Index %d -> ", i);
            
            while (pRecord != NULL)
            {
                printf("%d %c %d -> ", pRecord->id, pRecord->name, pRecord->order);
                pRecord = pRecord->next;
            }
            printf("NULL\n");
        }
    }
}

