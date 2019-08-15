// Struct for sending file to compiler server.
typedef struct 
{
    int url_specified;
    char *url;
    int is_renamed;
    char *name;
    int is_offline;
} package;
