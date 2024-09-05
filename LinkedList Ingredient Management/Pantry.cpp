/*
CSCI235 Spring 2024
Project 5 - Ingredient Management with Linked Lists 
William Li
April 4th 2024
Pantry.cpp defines the constructors and private and public function implementation of the Pantry class
*/


#include "Pantry.hpp"

std::vector<std::string> split(const std::string& input, const char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(input);
    while(std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }

return tokens;
} 



/**
Default Constructor
*/
Pantry::Pantry(const std::string inputfile) : LinkedList<Ingredient*>(){
    //open the file
    std::ifstream recipesfile(inputfile);

    //if it fails to opent the file 
    if(recipesfile.fail()){
        std::cerr << "Failure to open file!" << std::endl;
        exit(1); // exit 
    }

    //consume the header 
    std::string line;
    std::getline(recipesfile, line);
    std::string name;
    std::string description;
    std::string quantitystr;
    std::string pricestr; 
    std::string recipe;

    while(std::getline(recipesfile, line)){
       int end = line.size();
        while (end > 0 && std::isspace(line[end - 1])) end--;
        if (end > 0) line = line.substr(0, end);

        //get the elements and split it between the ','
        std::istringstream ss(line);
        std::getline(ss, name, ',');
        std::getline(ss, description, ',');
        std::getline(ss, quantitystr, ',');
        std::getline(ss, pricestr, ',');
        std::getline(ss, recipe, ',');

        // Create a new Ingredient object with the extracted data
        int quantity = std::stoi(quantitystr);
        int price = std::stoi(pricestr);
        std::vector<std::string> recipeIngredients;
        if (recipe != "NONE") {
            std::stringstream recipeStream(recipe);
            std::string ingredientName;
            while (std::getline(recipeStream, ingredientName, ';')) {
                recipeIngredients.push_back(ingredientName);
            }
        }

        // Check if the ingredient already in the list 
        if (contains(name)) {
            // Update ingredients that is already in the list 
            int pos = getPosOf(name);
            Ingredient* ingredient = getEntry(pos);
            ingredient->description_ = description;
            ingredient->quantity_ = quantity;
            ingredient->price_ = price;
            ingredient->recipe_ = {}; // Clear existing recipe
            for (const auto& recipeIngredient : recipeIngredients) {
                // Add new ingredient if it doesn't exist in the list
                if (!contains(recipeIngredient)) {
                    addIngredient(recipeIngredient, "UNKNOWN", 0, 0, {});
                }
                ingredient->recipe_.push_back(getEntry(getPosOf(recipeIngredient)));
            }
        } else {
            // Create a new ingredient
            addIngredient(name, description, quantity, price, {});
            Ingredient* ingredient = getEntry(getPosOf(name));
            for (const auto& recipeIngredient : recipeIngredients) {
                // Add new ingredient if it doesn't exist in the list
                if (!contains(recipeIngredient)) {
                    addIngredient(recipeIngredient, "UNKNOWN", 0, 0, {});
                }
                ingredient->recipe_.push_back(getEntry(getPosOf(recipeIngredient)));
            }
        }
    }
    //close file when done 
    recipesfile.close();
}
    

/**
    Destructor
    @post: Explicitly deletes every dynamically allocated Ingredient object
*/
Pantry::~Pantry(){
    //we will loop through everything and delete the allocated Ingredient objects
    for(int i = 0; i < getLength(); i++){
        Ingredient* clear = getEntry(i);
        delete clear;
    }
    clear();
}



/**
    @param: A const string reference to a ingredient name
    @return: The integer position of the given ingredient if it is in the Pantry, -1 if not found.
*/
int Pantry::getPosOf(const std::string& name) const{
     for(int i = 0; i < item_count_; i++){
        if(getEntry(i)->name_ == name){
            return i;
        }
    }
    return -1;
}

/**
    @param: A const string reference to a ingredient name
    @return: True if the ingredient information is already in the Pantry
*/
bool Pantry::contains(const std::string& name) const{
    for(int i = 0; i < item_count_; i++){ //Loops through item count in pantry
        if(getEntry(i)->name_ == name){ //Checks if it is already in pantry
            return true;
        }
    }
    return false;
}

/**
    @param:  A pointer to an Ingredient object
    @post:  Inserts the given ingredient pointer into the Pantry, unless an ingredient of the same name is already in the pantry. 
            Each of its Ingredients in its recipe are also added to the Pantry IF not already in the list.
    @return: True if the ingredient was added successfully, false otherwise.
*/
bool Pantry::addIngredient(Ingredient* ingredient){
    if(!contains(ingredient->name_)){ // runs if does not contain
        insert(item_count_, ingredient);
        for(int i = 0; i < ingredient->recipe_.size(); i++){
            if(!contains(ingredient->recipe_[i]->name_)){
                insert(item_count_, ingredient->recipe_[i]);
            }
        }
        return true;
    }
    return false;
}

/**
    @param: A const string reference representing a ingredient name
    @param: A const string reference representing ingredient description
    @param: A const int reference representing the ingredient's quantity
    @param: A const int reference representing the ingredient's price
    @param: A const reference to a vector holding Ingredient pointers representing the ingredient's recipe
    @post:   Creates a new Ingredient object and inserts a pointer to it into the Pantry. 
            Each of its Ingredients in its recipe are also added to the Pantry IF not already in the list.
    @return: True if the ingredient was added successfully
*/
bool Pantry::addIngredient(const std::string& ingredientName, const std::string& ingredientDescription, 
    const int ingredientQuantity, const int ingredientPrice, const std::vector<Ingredient*>& ingredientRecipe){

    Ingredient* newIngredient = new Ingredient(ingredientName, ingredientDescription, ingredientQuantity, ingredientPrice, ingredientRecipe);
    return addIngredient(newIngredient);
    
}

/**
    @param:  A Ingredient pointer
    @return: A boolean indicating if all the given ingredient can be created (all of the ingredients in its recipe can be created, 
        or if you have enough of each ingredient in its recipe to create it)
*/
bool Pantry::canCreate(Ingredient* ingredient){
    if(ingredient->recipe_.empty()){
        return false;
    }
    for(Ingredient* smallIngredient : ingredient->recipe_){
        int pos = getPosOf(smallIngredient->name_);
        if(pos == -1){
            return false;
        }
        Ingredient* smallIngredientEntry = getEntry(pos);
        if(smallIngredientEntry->quantity_ < 1 && !canCreate(smallIngredientEntry)){
            return false;
        }
    }
    return true;
}



/**
    @param: A Ingredient pointer
    @post: Prints the ingredient name, quantity, and description.
    The output should be of the form:
    [Ingredient Name]: [Quantity]\n
    [Ingredient Description]\n
    Price: [Price]\n
    Recipe:\n
    [Ingredient0] [Ingredient1]\n


    If the ingredient has no recipe, print "Recipe:\nNONE\n\n" after the price.
*/
void Pantry::printIngredient(Ingredient* ingredient){
    
    std::cout << ingredient->name_ << ": " << ingredient->quantity_<< "\n" << //[Ingredient Name]: [Quantity]\n
    ingredient->description_ << "\n" << // [Ingredient Description]\n
    "Price: " << ingredient->price_ << "\n" << // Price: [Price]\n
    "Recipe:\n"; // Recipe: \n
    if((ingredient->recipe_).size() > 0){ //if more than 1 ingredient
        for(int i = 0; i < ingredient->recipe_.size(); i++){ //loops through each ingredient
            std::cout << (ingredient->recipe_[i])->name_ << " "; //prints out each ingredient
        }
    }
    else{
        std::cout << "NONE\n";
    }
    std::cout << std::endl;
}




/**
    @param: A const string reference to a ingredient name
    @post:  Prints a list of ingredients that must be created before the given ingredient can be created (missing ingredients for its recipe, where you have 0 of the needed ingredient).
            If the ingredient is already in the pantry, print "In the pantry([quantity])\n"
            If there are no instances of the ingredient, if it cannot be crafted because of insufficient ingredients, print "[Ingredient Name](0)\nMISSING INGREDIENTS"
            If it can be crafted, recursively print the ingredients that need to be used (if there are instances of them) or created (if there are no instances of them) in the order that the ingredients appear in the recipe, joined by "<-".
            If the ingredient has no recipe, print "UNCRAFTABLE\n" at the end of the function.

            Below are some of the expected forms. "Scenario: [scenario] is not part of the output. It is just to help you understand the expected output.":

            Scenario: The Ingredient does not exist in the list
            Query: [Ingredient Name]
            No such ingredient

            Scenario: The Ingredient exists in the list, and there are >0 instances of it
            Query: [Ingredient Name]
            In the pantry ([Quantity])

            Scenario: The Ingredient exists in the list, and there are 0 instances of it, and it is craftable by using an ingredient that is already in the pantry
            Query: [Ingredient Name0]
            [Ingredient Name0](C)
            [Ingredient Name1](1)

            Scenario: The Ingredient exists in the list, and there are 0 instances of it, and it is craftable by using an ingredient that has to be crafted
            Query: [Ingredient Name0]
            [Ingredient Name0](C)
            [Ingredient Name1](C) <- [Ingredient Name2](3)
            
            Scenario: The Ingredient exists in the list, and there are 0 instances of it, and there are multiple ingredients that have to be crafted (each row represents a different ingredient inå the original recipe)
            Query: [Ingredient Name0]
            [Ingredient Name0](C)
            [Ingredient Name1](C) <- [Ingredient Name2](3)
            [Ingredient Name3](C) <- [Ingredient Name4](C) <- [Ingredient Name5] (3)

            Scenario: The Ingredient exists in the list, and there are 0 instances of it, and it is not craftable (it has no recipe)
            Query: [Ingredient Name0]
            UNCRAFTABLE

            Scenario: The Ingredient exists in the list, and there are 0 instances of it, and it has a recipe, but you do not have enough of the ingredients to craft it
            Query: [Ingredient Name0]
            [Ingredient Name0](0)
            MISSING INGREDIENTS
    
    HINT: Use canCreate() to determine if the ingredient can be created.
*/

void Pantry::ingredientQuery(const std::string& name){
    int position = getPosOf(name);
    // If the ingredient does not exist in the list
    std::cout << "Query: " << name << std::endl;
    if (position == -1) 
    {
        std::cout << "No such ingredient\n\n\n";
        return;
    }
    //when the quantity is 0 and can't be crafted 
    else{
        Ingredient* QueryIngre = getNodeAt(position)-> getItem();
        if(QueryIngre -> quantity_ == 0 && !canCreate(QueryIngre) && QueryIngre->recipe_.size() > 0)
        {
            //missing ingredients
            std::cout << QueryIngre->name_<<"(0)\n";
            std::cout << "MISSING INGREDIENTS\n";
        }
        //when its uncraftable
        else if(QueryIngre -> quantity_ >0)
        {
            std::cout << "In the pantry (" << QueryIngre->quantity_<<")"<<"\n";
            if(!canCreate(QueryIngre))
            {
                std::cout << "UNCRAFTABLE\n";
            }
        }
        else
        {
            //when its uncraftable
            if(!canCreate(QueryIngre))
            {
                std::cout << "UNCRAFTABLE\n";
            }
            else
            {
                //when we can craft it 
                std::cout << QueryIngre-> name_ <<"(C)\n";
                displayCraftingChain(QueryIngre);
                std::cout << "\n";
            }
        }
    }

}




/**
    @return: An integer sum of the price of all the ingredients currently in the list.
    Note: This should only include price values from ingredients that you have 1 or more of. 
        Do not consider ingredients that you have 0 of, even if you have the ingredients to make them.
*/
int Pantry::calculatePantryValue(){
    int total = 0;
    for(int i = 0; i < item_count_; i++){
        if(getEntry(i)->quantity_ > 0){
            total += getEntry(i)->price_ * getEntry(i)->quantity_;
        }
    }
    return total;
}



/**
    @param: A const string reference to a filter with a default value of "NONE".
    @post: With default filter "NONE": Print out every ingredient in the list.
        With filter "CONTAINS":   Only print out the ingredients with >0 instances in the list.
        With filter "MISSING": Only print out the ingredients with 0 instances in the list.
        With filter "CRAFTABLE":  Only print out the ingredients where you have all the ingredients to craft them.
        If an invalid filter is passed, print "INVALID FILTER\n"
        Printing ingredients should be of the form:

        [Ingredient name]: [Quantity]
        [Ingredient description]\n
        Price: [price]\n
        Recipe:\n
        [Ingredient0] [Ingredient1]\n\n

        If the ingredient has no recipe, print "Recipe:\nNONE\n\n" after the price.
*/
void Pantry::pantryList(const std::string& filter) {
        
    bool Filter = false; //
    for (int i = 0; i < this->getLength(); i++) {  // loop through pantry
        Ingredient* theIngredient = this->getEntry(i);
    
        if (filter == "NONE") {
            Filter = true;
            printIngredient(theIngredient);
        } else if (filter == "CONTAINS" && theIngredient->quantity_ > 0) {
            Filter = true;
            printIngredient(theIngredient);
        } else if (filter == "MISSING" && theIngredient->quantity_ == 0) {
            Filter = true;
            printIngredient(theIngredient);
        } else if (filter == "CRAFTABLE" && canCreate(theIngredient)) {
            Filter = true;
            printIngredient(theIngredient);
        }
        
    }
    
    // if the filer was invalid 
    if (!Filter && filter != "NONE") {
        std::cout << "INVALID FILTER\n";
    }
    
}
