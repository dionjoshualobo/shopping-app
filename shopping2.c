#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_BUCKETS 20

typedef struct Item {
    char name[50];
    double price;
    struct Item *next;
} Item;

typedef struct SubCategory {
    char name[50];
    Item *items;
    struct SubCategory *next;
} SubCategory;

typedef struct Category {
    char name[50];
    SubCategory *subCategories;
    struct Category *next;
} Category;

typedef struct CartItem {
    char name[50];
    double price;
    int quantity;
    struct CartItem *next;
} CartItem;

CartItem *cart[MAX_BUCKETS];

int hash(char *name) {
    int hash = 0;
    for (int i = 0; name[i] != '\0'; i++)
        hash = (hash + name[i]) % MAX_BUCKETS;
    return hash;
}

void addItemToCart(char *name, double price, int quantity) {
    int bucket = hash(name);
    CartItem *item = cart[bucket];
    while (item) {
        if (strcmp(item->name, name) == 0) {
            item->quantity += quantity;
            return;
        }
        item = item->next;
    }
    item = (CartItem *)malloc(sizeof(CartItem));
    strcpy(item->name, name);
    item->price = price;
    item->quantity = quantity;
    item->next = cart[bucket];
    cart[bucket] = item;
}

void showCart() {
    printf("\nShopping Cart:\n");
    double total = 0.0;
    for (int i = 0; i < MAX_BUCKETS; i++) {
        CartItem *item = cart[i];
        while (item) {
            printf("%s - ₹%.2f x %d\n", item->name, item->price, item->quantity);
            total += item->price * item->quantity;
            item = item->next;
        }
    }
    printf("Total Cost: ₹%.2f\n", total);
}

Category *createCategory(char *name) {
    Category *category = (Category *)malloc(sizeof(Category));
    strcpy(category->name, name);
    category->subCategories = NULL;
    category->next = NULL;
    return category;
}

SubCategory *createSubCategory(char *name) {
    SubCategory *subCategory = (SubCategory *)malloc(sizeof(SubCategory));
    strcpy(subCategory->name, name);
    subCategory->items = NULL;
    subCategory->next = NULL;
    return subCategory;
}

Item *createItem(char *name, double price) {
    Item *item = (Item *)malloc(sizeof(Item));
    strcpy(item->name, name);
    item->price = price;
    item->next = NULL;
    return item;
}

void addItemToSubCategory(SubCategory *subCategory, Item *item) {
    item->next = subCategory->items;
    subCategory->items = item;
}

void addSubCategoryToCategory(Category *category, SubCategory *subCategory) {
    subCategory->next = category->subCategories;
    category->subCategories = subCategory;
}

void addCategory(Category **head, Category *category) {
    category->next = *head;
    *head = category;
}

void displayItems(SubCategory *subCategory) {
    int choice, quantity;
    Item *item = subCategory->items;
    int itemNumber = 1;

    while (item) {
        printf("%d. %s - ₹%.2f\n", itemNumber++, item->name, item->price);
        item = item->next;
    }

    printf("Select an item to add to cart (0 to go back): ");
    scanf("%d", &choice);

    if (choice == 0) return;

    item = subCategory->items;
    itemNumber = 1;
    while (item && choice > 1) {
        item = item->next;
        choice--;
    }

    if (item) {
        printf("Enter quantity: ");
        scanf("%d", &quantity);
        addItemToCart(item->name, item->price, quantity);
        printf("Added %d x %s to cart.\n", quantity, item->name);
    } else {
        printf("Invalid choice! Please try again.\n");
    }
}

void displaySubCategories(Category *category) {
    SubCategory *subCategory;
    int choice;

    while (1) {
        int subcatNumber = 1;
        printf("\n%s:\n", category->name);

        subCategory = category->subCategories;
        while (subCategory) {
            printf("%d. %s\n", subcatNumber++, subCategory->name);
            subCategory = subCategory->next;
        }
        printf("Select a subcategory to view items (0 to go back): ");
        scanf("%d", &choice);
        
        if (choice == 0) return;

        subCategory = category->subCategories;
        subcatNumber = 1;
        while (subCategory && choice > 1) {
            subCategory = subCategory->next;
            choice--;
        }

        if (subCategory) {
            displayItems(subCategory);
        } else {
            printf("Invalid choice! Please try again.\n");
        }
    }
}

void mainMenu(Category *head) {
    int choice;
    Category *category;

    while (1) {
        printf("\nMain Menu:\n");
        int i = 1;
        category = head;
        while (category) {
            printf("%d. %s\n", i++, category->name);
            category = category->next;
        }
        printf("%d. Shopping Cart\n%d. Exit\n", i, i + 1);
        printf("Choose an option: ");
        scanf("%d", &choice);

        if (choice == i) {
            showCart();
        } else if (choice == i + 1) {
            printf("Exiting...\n");
            break;
        } else {
            category = head;
            for (int j = 1; j < choice && category; j++)
                category = category->next;

            if (category) {
                displaySubCategories(category);
            } else {
                printf("Invalid choice! Please try again.\n");
            }
        }
    }
}

int main() {
    Category *head = NULL;
    Category *fashion = createCategory("Fashion");
    SubCategory *mens = createSubCategory("Men's Wear");
    SubCategory *womens = createSubCategory("Women's Wear");
    addItemToSubCategory(mens, createItem("Levi's Jeans", 3999.0));
    addItemToSubCategory(mens, createItem("Nike T-Shirt", 1300.0));
    addItemToSubCategory(mens, createItem("Air Jordan 1 (Black/Pine Green)", 13995.0));
    addItemToSubCategory(mens, createItem("GAP Denim Jacket", 3499.0));
    addItemToSubCategory(mens, createItem("Cartier Sunglasses", 5999.0));
    addItemToSubCategory(womens, createItem("H&M Floral Sundress", 2999.0));
    addItemToSubCategory(womens, createItem("Vero Moda Jeans", 4999.0));
    addItemToSubCategory(womens, createItem("Christian Louboutin Heels", 67999.0));
    addItemToSubCategory(womens, createItem("Hermes Birkin Leather Handbag", 2499999.0));
    addSubCategoryToCategory(fashion, mens);
    addSubCategoryToCategory(fashion, womens);
    addCategory(&head, fashion);

    Category *electronics = createCategory("Electronics");
    SubCategory *mobiles = createSubCategory("Mobiles");
    SubCategory *laptops = createSubCategory("Laptops");
    SubCategory *audio = createSubCategory("Audio");
    addItemToSubCategory(mobiles, createItem("iPhone 16 Pro Max", 184900.0));
    addItemToSubCategory(mobiles, createItem("Samsung Galaxy S24", 129999.0));
    addItemToSubCategory(mobiles, createItem("Nothing Phone(2)", 27999.0));
    addItemToSubCategory(laptops, createItem("MacBook Pro M3", 140990.0));
    addItemToSubCategory(laptops, createItem("Asus ROG Zephyrus", 299990.0));
    addItemToSubCategory(audio, createItem("Sony WH-1000XM5", 29751.0));
    addItemToSubCategory(audio, createItem("MoonDrop Beautiful World IEM", 77999.0));
    addSubCategoryToCategory(electronics, mobiles);
    addSubCategoryToCategory(electronics, laptops);
    addSubCategoryToCategory(electronics, audio);
    addCategory(&head, electronics);

    Category *groceries = createCategory("Groceries");
    SubCategory *fruits = createSubCategory("Fruits");
    SubCategory *vegetables = createSubCategory("Vegetables");
    SubCategory *dairy = createSubCategory("Dairy Products");
    addItemToSubCategory(fruits, createItem("Apples (per kg)", 230));
    addItemToSubCategory(fruits, createItem("Bananas (per kg)", 100));
    addItemToSubCategory(fruits, createItem("Ruby Roman Grapes (per kg)", 5907));
    addItemToSubCategory(vegetables, createItem("Tomatoes (per kg)", 40));
    addItemToSubCategory(vegetables, createItem("Onions (per kg)", 60));
    addItemToSubCategory(vegetables, createItem("Potatoes (per kg)", 30));
    addItemToSubCategory(dairy, createItem("Mother Dairy Full Cream (1L)", 70));
    addItemToSubCategory(dairy, createItem("Amul Butter (500g)", 260));
    addSubCategoryToCategory(groceries, fruits);
    addSubCategoryToCategory(groceries, vegetables);
    addSubCategoryToCategory(groceries, dairy);
    addCategory(&head, groceries);

    mainMenu(head);
    return 0;
}
