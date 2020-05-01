#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define READ "read"
#define LIST "list"
#define FIND "find"
#define TRACE "trace"
#define DELETE "delete"
#define SAVE "save"
#define EXIT "exit"
typedef struct personal {
    char name[256];
    char company[256];
    char address[256];
    char zipcode[256];
    char phone[256];
    char email[256];
}personal;

typedef struct Node {
    personal *per;
    struct Node *left;
    struct Node *right;
}Node;

Node* Min(Node *node)
{
    if (node == NULL) {
        return NULL;
    }
    if ( node->left) {
        return Min(node->left);
    }
    else return node;
}

Node* Max(Node *node)
{
    if ( node == NULL) {
        return NULL;
    }
    if ( node->right) {
        return Max(node->right);
    }
    else return node;
}

Node* insert(Node *node, personal *p)
{
    if (node == NULL) {
        Node* temp;
        temp = (Node *)malloc(sizeof(Node));
        temp->per = p;
        temp->left = NULL;
        temp->right = NULL;
        return temp;
    }
    if(strcmp(node->per->name, p->name) > 0) {
        node->left = insert(node->left, p);
    }
    else {
        node->right = insert(node->right, p);
    }
    return node;
}

Node* delete(Node* node, char* name)
{
    Node *temp;
    if (node == NULL) {
        return NULL;
    }
    if (name < (node->per->name)) {
        node->left = delete(node->left, name);
    }
    else if (name > (node->per->name)) {
        node->right = delete(node->right, name);
    }

    else {
        if (node->right && node->left) {
            temp = Min(node->right);
            node->per = temp->per;
            node->right = delete(node->right, temp->per->name);
        }
        else {
            temp = node;
            if (node->left == NULL) {
                node = node->right;
            }
            else if (node->right == NULL) {
                node = node->left;
            }
            free(temp);
        }
    }
    return node;
}
void inorder(Node *node)
{
    if(node == NULL) {
        return;
    }
    inorder(node->left);
    printf("%s\n", node->per->name);
    printf("    %s\n", node->per->company);
    printf("    %s\n", node->per->address);
    printf("    %s\n", node->per->zipcode);
    printf("    %s\n", node->per->phone);
    printf("    %s\n", node->per->email);
    inorder(node->right);
}

Node* find(Node *node, char* name)
{
    if(node == NULL || strcmp(node->per->name, name) == 0) {
        return node;
    }
    if(strcmp(node->per->name, name) > 0) {
        return find(node->left, name);
    }
    return find(node->right, name);
}


Node* trace(Node* node, char* name)
{
    printf("%s ", node->per->name);
    if(node == NULL || node->per->name == name) {
        return node;
    }
    if(strcmp(name, node->per->name) > 0) {
        return trace(node->right, name);
    }
    else return trace(node->left, name);
    
}

void savefile(FILE* fp, Node* node)
{
    if (node == NULL) {
        return;
    }
    savefile(fp, node->left);
    fprintf(fp, "%s", node->per->name);
    fprintf(fp, ",");
    fprintf(fp, "%s", node->per->company);
    fprintf(fp, ",");
    fprintf(fp, "%s", node->per->address);
    fprintf(fp, ",");
    fprintf(fp, "%s", node->per->zipcode);
    fprintf(fp, ",");
    fprintf(fp, "%s", node->per->phone);
    fprintf(fp, ",");
    fprintf(fp, "%s", node->per->email);
    fprintf(fp, "\n");
    savefile(fp, node->right);
}

void clearnode(Node *node)
{
    if(node == NULL) {
        return;
    }
    clearnode(node->left);
    clearnode(node->right);
    free(node);
    free(node->per);
}

int main()
{
    FILE *fp=NULL;
    Node *root=NULL;
    Node *temp=NULL;
    
    char command[100];
    char command_first[100];
    char command_second[100];
    char *p;

    int rowcount = 0;
    int fieldcount = 0;
    int i = 0;
    int in_double_quotes = 0;
    int tokenpos = 0;

    char buffer[1000];
    char token[1000];

    do {
        temp = NULL;
        printf("$  ");
        scanf("%[^\n]", command);
        getchar();

        p = strtok(command, " ");
        strcpy(command_first, p);
        p = strtok(NULL, " ");
        if(p != NULL) {
            strcpy(command_second, p);
        }
        else strcpy(command_second, "");
        
        if (strcmp(command_first, READ) == 0) {
            if (strcmp(command_second, "") == 0) {
                printf("error_no filename\n");
                continue;
            }
            personal* m = NULL;
            
            if (root != NULL) {
                clearnode(root);
                root = NULL;
            }

            fp = fopen(command_second, "r");

            if (fp != NULL) {
                while (fgets(buffer, 1024, fp)) {
                    rowcount++;

                    if (rowcount == 1) continue;

                    fieldcount = 0;
                    i = 0;
                    
                    m = (personal*)malloc(sizeof(personal));

                    do {
                        token[tokenpos++] = buffer[i];

                        if ( !in_double_quotes && (buffer[i] == ',' || buffer[i] == '\n') ) {
                            token[tokenpos - 1] = 0;
                            tokenpos = 0;

                            if (fieldcount == 0) {
                                strcpy(m->name, token);
                            } 
                            else if (fieldcount == 1) {
                                strcpy(m->company, token);
                            }
                            else if (fieldcount == 2) {
                                strcpy(m->address, token);
                            } 
                            else if (fieldcount == 3) {
                                strcpy(m->zipcode, token);
                            } 
                            else if (fieldcount == 4) {
                                strcpy(m->phone, token);
                            } 
                            else if (fieldcount == 5) {
                                strcpy(m->email, token);
                            }
                            fieldcount++;
                        }
                        if (buffer[i] == '"' && buffer[i+1] != '"') {
                            tokenpos--;
                            in_double_quotes = !in_double_quotes;
                        }
                        if (buffer[i] == '"' && buffer[i+1] == '"') {
                            i++;
                        }
                    } while (buffer[++i]);
                    
                    root = insert(root, m);
                }
            }
        }
        
        if(strcmp(command_first, LIST) == 0) {
            inorder(root);
        }

        if(strcmp(command_first, FIND) == 0) {
            if(strcmp(command_second, "") == 0) {
                printf("error_no target\n");
                continue;
            }
            temp = find(root, command_second);
            if(temp == NULL) {
                printf("There is no such name\n");
                continue;
            }
            printf("%s\n", temp->per->name);
            printf("    %s\n", temp->per->company);
            printf("    %s\n", temp->per->address);
            printf("    %s\n", temp->per->zipcode);
            printf("    %s\n", temp->per->phone);
            printf("    %s\n", temp->per->email);
        }
        
        if(strcmp(command_first, TRACE) == 0) {
            if(strcmp(command_second, "") == 0) {
                printf("error_no target\n");
                continue;
            }
            temp = trace(root, command_second);
            if(temp == NULL) {
                printf("Trace compelete, but there is no such name\n");
            }
        }
        
        if(strcmp(command_first, DELETE) == 0) {
            if(strcmp(command_second, "") == 0) {
                printf("error_no target\n");
                continue;
            }
            delete(root, command_second);
        }

        if(strcmp(command_first, SAVE) == 0) {
            if(strcmp(command_second, "") == 0) {
                printf("error_no filename\n");
                continue;
            }
            FILE *fpp = fopen(command_second, "w");

            fprintf(fpp, "name,company_name,address,zipcode,phone,email\n");
            savefile(fpp, root);
            fclose(fpp);
        }
        command[0] = '\0';
        command_second[0] = '\0';
    } while(strcmp(command_first, EXIT) != 0);
    return 0;
}