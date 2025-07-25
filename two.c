#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Product structure
typedef struct Product {
    int id;
    char name[50];
    float price;
    struct Product *next; // Linked list pointer
} Product;

// Stack structure for deleted products
typedef struct Stack {
    Product *items[10];
    int top;
} Stack;

// Queue structure for searching products by category and ID
typedef struct Queue {
    Product *items[10];
    int front, rear;
} Queue;

// Binary Search Tree (BST) structure for products
typedef struct BSTNode {
    Product *product;
    struct BSTNode *left, *right;
} BSTNode;

// Graph structure for categories
typedef struct Graph {
    int adjMatrix[3][3]; // 3 categories
} Graph;

// Function declarations
void addProduct(Product **category, int id, const char *name, float price);
void deleteProduct(Product **category, int id, Stack *stack);
void displayProducts(Product *category);
void pushDeletedProduct(Stack *stack, Product *product);
void displayDeletedProducts(Stack *stack);
void insertInBST(BSTNode **root, Product *product);
void displayBSTProducts(BSTNode *root);
void addGraphEdge(Graph *graph, int source, int destination);
void traverseGraph(Graph *graph, int source, Product *categories[]);
void enqueue(Queue *queue, Product *product);
Product* dequeue(Queue *queue);
void searchProductByCategory(Queue *queue, int categoryId, int productId);

// Main function
int main() {
    Product *categories[3] = {NULL, NULL, NULL}; // Array to hold products for 3 categories
    Stack stack = {{NULL}, -1};
    BSTNode *bstRoot = NULL;
    Graph graph = {0};
    Queue queue = {{NULL}, -1, -1};

    int choice, categoryId, productId;
    char productName[50];
    float productPrice;

    while (1) {
        printf("\n1. Add Product\n");
        printf("2. Delete Product\n");
        printf("3. Display Products (Linked List)\n");
        printf("4. Display Deleted Products (Stack)\n");
        printf("5. Insert Product in BST\n");
        printf("6. Display BST Products\n");
        printf("7. Add Graph Edge\n");
        printf("8. Traverse Graph\n");
        printf("9. Search Product by Category and ID (Queue)\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                // Add product
                printf("Enter Category ID (101, 102, 103): ");
                scanf("%d", &categoryId);
                printf("Enter Product ID: ");
                scanf("%d", &productId);
                printf("Enter Product Name: ");
                getchar();  // To consume newline character left by scanf
                fgets(productName, sizeof(productName), stdin);
                productName[strcspn(productName, "\n")] = '\0';  // Remove newline character
                printf("Enter Product Price: ");
                scanf("%f", &productPrice);

                addProduct(&categories[categoryId - 101], productId, productName, productPrice);
                printf("Product added successfully.\n");
                break;

            case 2:
                // Delete product
                printf("Enter Category ID (101, 102, 103): ");
                scanf("%d", &categoryId);
                printf("Enter Product ID to delete: ");
                scanf("%d", &productId);

                deleteProduct(&categories[categoryId - 101], productId, &stack);  // Pass stack
                printf("Product deleted successfully.\n");
                break;

            case 3:
                // Display products in linked list
                printf("Enter Category ID (101, 102, 103) to display products: ");
                scanf("%d", &categoryId);
                displayProducts(categories[categoryId - 101]);
                break;

            case 4:
                // Display deleted products (stack)
                displayDeletedProducts(&stack);
                break;

            case 5:
                // Insert product into BST
                printf("Enter Product ID for BST: ");
                scanf("%d", &productId);
                printf("Enter Product Name for BST: ");
                getchar();  // To consume newline character left by scanf
                fgets(productName, sizeof(productName), stdin);
                productName[strcspn(productName, "\n")] = '\0';  // Remove newline character
                printf("Enter Product Price for BST: ");
                scanf("%f", &productPrice);

                Product newProduct = (Product)malloc(sizeof(Product));
                newProduct->id = productId;
                strcpy(newProduct->name, productName);
                newProduct->price = productPrice;
                insertInBST(&bstRoot, newProduct);
                break;

            case 6:
                // Display products in BST
                displayBSTProducts(bstRoot);
                break;

            case 7:
                // Add graph edge
                printf("Enter Source Category: ");
                scanf("%d", &categoryId);
                printf("Enter Destination Category: ");
                int destCategory;
                scanf("%d", &destCategory);

                addGraphEdge(&graph, categoryId - 101, destCategory - 101);
                printf("Edge added between Category %d and Category %d\n", categoryId, destCategory);
                break;

            case 8:
                // Traverse graph
                printf("Enter Source Category: ");
                scanf("%d", &categoryId);
                traverseGraph(&graph, categoryId - 101, categories);  // Pass categories array
                break;

            case 9:
                // Search for product by category and ID
                printf("Enter Category ID (101, 102, 103) to search: ");
                scanf("%d", &categoryId);
                printf("Enter Product ID to search: ");
                scanf("%d", &productId);
                searchProductByCategory(&queue, categoryId, productId);
                break;

            case 0:
                // Exit the program
                printf("Exiting the program...\n");
                return 0;

            default:
                printf("Invalid choice, try again.\n");
        }
    }
    return 0;
}

// Function definitions

// Add a product to a category (linked list)
void addProduct(Product **category, int id, const char *name, float price) {
    Product newProduct = (Product)malloc(sizeof(Product));
    newProduct->id = id;
    strcpy(newProduct->name, name);
    newProduct->price = price;
    newProduct->next = *category;
    *category = newProduct;
}

// Delete a product from a category (linked list)
void deleteProduct(Product **category, int id, Stack *stack) {
    Product *temp = *category, *prev = NULL;
    if (temp != NULL && temp->id == id) {
        *category = temp->next;
        pushDeletedProduct(stack, temp);  // Push to stack before freeing
        free(temp);
        return;
    }
    while (temp != NULL && temp->id != id) {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL) return;
    prev->next = temp->next;
    pushDeletedProduct(stack, temp);  // Push to stack before freeing
    free(temp);
}

// Display products of a category (linked list)
void displayProducts(Product *category) {
    if (category == NULL) {
        printf("No products found in this category.\n");
        return;
    }
    printf("Product List:\n");
    while (category != NULL) {
        printf("ID: %d, Name: %s, Price: %.2f\n", category->id, category->name, category->price);
        category = category->next;
    }
}

// Push a deleted product to the stack
void pushDeletedProduct(Stack *stack, Product *product) {
    if (stack->top < 9) {
        stack->items[++stack->top] = product;
    }
}


void displayDeletedProducts(Stack *stack) {
    if (stack->top == -1) {
        printf("No deleted products.\n");
        return;
    }
    printf("Deleted Product List:\n");
    for (int i = stack->top; i >= 0; i--) {
        printf("ID: %d, Name: %s\n", stack->items[i]->id, stack->items[i]->name);
    }
}

// Insert product into BST
void insertInBST(BSTNode **root, Product *product) {
    if (*root == NULL) {
        BSTNode newNode = (BSTNode)malloc(sizeof(BSTNode));
        newNode->product = product;
        newNode->left = newNode->right = NULL;
        *root = newNode;
    } else if (product->id < (*root)->product->id) {
        insertInBST(&(*root)->left, product);
    } else {
        insertInBST(&(*root)->right, product);
    }
}

// Display BST products (in-order traversal)
void displayBSTProducts(BSTNode *root) {
    if (root == NULL) return;
    displayBSTProducts(root->left);
    printf("ID: %d, Name: %s, Price: %.2f\n", root->product->id, root->product->name, root->product->price);
    displayBSTProducts(root->right);
}

// Add an edge in the graph
void addGraphEdge(Graph *graph, int source, int destination) {
    graph->adjMatrix[source][destination] = 1;
    graph->adjMatrix[destination][source] = 1;  // For undirected graph
}

// Traverse the graph and display products in connected categories
void traverseGraph(Graph *graph, int source, Product *categories[]) {
    printf("Displaying products of connected categories to Category %d:\n", source + 101);
    for (int i = 0; i < 3; i++) {
        if (graph->adjMatrix[source][i] == 1) {
            displayProducts(categories[i]);
        }
    }
}

// Enqueue a product for searching (queue)
void enqueue(Queue *queue, Product *product) {
    if ((queue->rear + 1) % 10 != queue->front) {
        queue->rear = (queue->rear + 1) % 10;
        queue->items[queue->rear] = product;
    }
}

// Dequeue a product from the queue
Product* dequeue(Queue *queue) {
    if (queue->front == queue->rear) {
        return NULL;
    }
    queue->front = (queue->front + 1) % 10;
    return queue->items[queue->front];
}

// Search for a product by category and ID
void searchProductByCategory(Queue *queue, int categoryId, int productId) {
    for (int i = 0; i < 3; i++) {
        if (queue->items[i]->id == productId) {
            printf("Product found: ID: %d, Name: %s, Price: %.2f\n", queue->items[i]->id, queue->items[i]->name, queue->items[i]->price);
            return;
        }
    }
    printf("Product not found.\n");
}