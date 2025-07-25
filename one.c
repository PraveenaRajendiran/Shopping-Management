#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Product {
    int categoryNumber;
    char name[50];
    int productId;
    float price;
    int quantity;
    struct Product *next;
};

struct Stack {
    struct Product *product;
    struct Stack *next;
};


struct BSTNode {
    int productId;
    struct Product *product;
    struct BSTNode *left, *right;
};

struct GraphNode {
    int categoryNumber;
    struct GraphNode *next;
};


struct Stack *deletedStack = NULL;

const char *getCategoryName(int categoryNumber) {
    switch (categoryNumber) {
        case 101: return "Electronics";
        case 102: return "Groceries";
        case 103: return "Books";
        case 104: return "Beauty Products";
        case 105: return "Clothing";
        default: return "Unknown Category";
    }
}


int getNextProductId(int categoryNumber) {
    static int electronicsId = 1000;
    static int groceriesId = 2000;
    static int booksId = 3000;
    static int beautyProductsId = 4000;
    static int clothingId = 5000;

    switch (categoryNumber) {
        case 101: return electronicsId++;
        case 102: return groceriesId++;
        case 103: return booksId++;
        case 104: return beautyProductsId++;
        case 105: return clothingId++;
        default: return -1;
    }
}


void addProduct(struct Product **head, int categoryNumber, char *name, float price, int quantity) {
    struct Product *newProduct = (struct Product *)malloc(sizeof(struct Product));
    newProduct->categoryNumber = categoryNumber;
    strcpy(newProduct->name, name);
    newProduct->productId = getNextProductId(categoryNumber);
    newProduct->price = price;
    newProduct->quantity = quantity;
    newProduct->next = *head;
    *head = newProduct;
    printf("Product added: %s, ID: %d, Price: %.2f, Quantity: %d\n", name, newProduct->productId, price, quantity);
}

void pushStack(struct Stack **stack, struct Product *product) {
    struct Stack *newNode = (struct Stack *)malloc(sizeof(struct Stack));
    newNode->product = product;
    newNode->next = *stack;
    *stack = newNode;
}

void displayStack(struct Stack *stack) {
    printf("Deleted Products (Stack):\n");
    if (!stack) {
        printf("  Stack is empty.\n");
        return;
    }

    while (stack) {
        printf("  Product ID: %d, Name: %s, Category: %s, Price: %.2f\n",
               stack->product->productId, stack->product->name,
               getCategoryName(stack->product->categoryNumber), stack->product->price);
        stack = stack->next;
    }
}

void deleteProduct(struct Product **head, int productId) {
    struct Product *current = *head, *prev = NULL;

    while (current) {
        if (current->productId == productId) {
            current->quantity--;
            printf("Decremented quantity of Product ID: %d. New Quantity: %d\n",
                   current->productId, current->quantity);

            if (current->quantity <= 0) {
                printf("Product ID: %d has 0 quantity. Removing from list.\n", current->productId);
                if (prev) {
                    prev->next = current->next;
                } else {
                    *head = current->next;
                }
                pushStack(&deletedStack, current);
                free(current);
            }
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("Product ID: %d not found.\n", productId);
}






struct BSTNode *insertBST(struct BSTNode *root, struct Product *product) {
    if (!root) {
        struct BSTNode *newNode = (struct BSTNode *)malloc(sizeof(struct BSTNode));
        newNode->productId = product->productId;
        newNode->product = product;
        newNode->left = newNode->right = NULL;
        return newNode;
    }

    if (product->productId < root->productId) {
        root->left = insertBST(root->left, product);
    } else if (product->productId > root->productId) {
        root->right = insertBST(root->right, product);
    }

    return root;
}

void inOrderTraversal(struct BSTNode *root) {
    if (root) {
        inOrderTraversal(root->left);
        printf("Category ID: %d, Product ID: %d, Name: %s, Price: %.2f, Quantity: %d\n",
               root->product->categoryNumber, root->product->productId, root->product->name,
               root->product->price, root->product->quantity);
        inOrderTraversal(root->right);
    }
}

void addEdge(struct GraphNode **graph, int fromCategory, int toCategory) {
    struct GraphNode *newNode = (struct GraphNode *)malloc(sizeof(struct GraphNode));
    newNode->categoryNumber = toCategory;
    newNode->next = graph[fromCategory];
    graph[fromCategory] = newNode;

    printf("Edge added: %s -> %s\n", getCategoryName(fromCategory), getCategoryName(toCategory));
}

void bfs(struct GraphNode **graph, int startCategory, int totalCategories) {
    int visited[totalCategories];
    for (int i = 0; i < totalCategories; i++) visited[i] = 0;

    int queue[totalCategories], front = 0, rear = 0;
    queue[rear++] = startCategory;
    visited[startCategory] = 1;

    printf("BFS Traversal starting from %s:\n", getCategoryName(startCategory));
    while (front < rear) {
        int current = queue[front++];
        printf("%s -> ", getCategoryName(current));

        struct GraphNode *temp = graph[current];
        while (temp) {
            if (!visited[temp->categoryNumber]) {
                visited[temp->categoryNumber] = 1;
                queue[rear++] = temp->categoryNumber;
            }
            temp = temp->next;
        }
    }
    printf("END\n");
}


void displayProductsByCategory(struct Product *head, int categoryNumber) {
    printf("Category: %s (ID: %d)\n", getCategoryName(categoryNumber), categoryNumber);
    int found = 0;

    while (head) {
        if (head->categoryNumber == categoryNumber) {
            printf("  Product ID: %d, Name: %s, Price: %.2f, Quantity: %d\n",
                   head->productId, head->name, head->price, head->quantity);
            found = 1;
        }
        head = head->next;
    }

    if (!found) {
        printf("  No products found in this category.\n");
    }
}


void displayMenu() {
    printf("\nOptions:\n");
    printf("1. Add Product\n");
    printf("2. Display Products by Category\n");
    printf("3. Delete Product\n");
    printf("4. Display Deleted Products (Stack)\n");
    printf("5. Insert Product into BST\n");
    printf("6. In-Order Traversal of BST\n");
    printf("7. Add Edge to Graph\n");
    printf("8. BFS Traversal of Graph\n");
    printf("9. Exit\n");
    printf("Enter your choice: ");
}

int main() {
    struct Product *productList = NULL;
    struct BSTNode *bstRoot = NULL;
    struct Queue *productQueue = NULL;
    struct GraphNode *categoryGraph[106] = { NULL };  // Assuming 105 categories

    int choice, categoryNumber, productId, quantity, fromCategory, toCategory;
    char name[50];
    float price;

    while (1) {
        displayMenu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter Category Number: ");
                scanf("%d", &categoryNumber);
                printf("Enter Product Name: ");
                scanf(" %[^\n]", name);
                printf("Enter Price: ");
                scanf("%f", &price);
                printf("Enter Quantity: ");
                scanf("%d", &quantity);
                addProduct(&productList, categoryNumber, name, price, quantity);
                break;

            case 2:
                printf("Enter Category Number to Display: ");
                scanf("%d", &categoryNumber);
                displayProductsByCategory(productList, categoryNumber);
                break;

            case 3:
                printf("Enter Product ID to Delete: ");
                scanf("%d", &productId);
                deleteProduct(&productList, productId);
                break;

            case 4:
                displayStack(deletedStack);
                break;

            case 5:
                printf("Enter Product ID to Insert into BST: ");
                scanf("%d", &productId);
                {
                    struct Product *prod = productList;
                    while (prod) {
                        if (prod->productId == productId) {
                            bstRoot = insertBST(bstRoot, prod);
                            printf("Inserted Product ID: %d into BST.\n", prod->productId);
                            break;
                        }
                        prod = prod->next;
                    }
                }
                break;

            case 6:
                inOrderTraversal(bstRoot);
                break;

            case 7:
                printf("Enter From Category and To Category: ");
                scanf("%d %d", &fromCategory, &toCategory);
                addEdge(categoryGraph, fromCategory, toCategory);
                break;

            case 8:
                printf("Enter Start Category for BFS: ");
                scanf("%d", &categoryNumber);
                bfs(categoryGraph, categoryNumber, 106);
                break;

            case 9:
                exit(0);

            default:
                printf("Invalid choice! Please try again.\n");
        }
    }

    return 0;
}
