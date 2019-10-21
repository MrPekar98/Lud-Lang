// Struct for sending file to compiler server.
// TODO: Add description of fields.
typedef struct 
{
    int url_specified;
    char *url;
    int is_renamed;
    char *name;
    int is_offline;	// Tells whether to send package. Might not be needed.
} package;

// Converts struct into string format.
char *package_string(package p)
{
    return NULL;
}
