#include "SkillTree.hpp"
#include <fstream>
#include <sstream>
#include <vector>
/**
* Default Constructor
*/

SkillTree::SkillTree(){}

/**
 * @param: A const reference to string: the name of a csv file
 * @post: The SkillTree is populated with Skills from the csv file
 * The file format is as follows:
 * id,name,description,leveled
 * Ignore the first line. Each subsequent line represents a Skill to be added to the SkillTree.
*/

SkillTree::SkillTree(const std::string& fileName) {
    std::ifstream file(fileName); // Open the file
    std::string line;

    if (!file.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    std::getline(file, line); 

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string item;
        std::vector<std::string> tokens;

        while (std::getline(ss, item, ',')) {
            tokens.push_back(item);
        }

        if (tokens.size() == 4) {
            int id = std::stoi(tokens[0]); 
            std::string name = tokens[1];
            std::string description = tokens[2];
            bool leveled = std::stoi(tokens[3]); 

            Skill newSkill(id, name, description, leveled);
            this->add(newSkill); 
        }
    }

    file.close(); // Close the file
}

/**
 * @param: A const reference to int representing the id of the Skill to be found
 * @return: A pointer to the node containing the Skill with the given id, or nullptr if the Skill is not found
*/

std::shared_ptr<BinaryNode<Skill>> SkillTree::findSkill(const int& skill_id) const {
    Skill searchSkill(skill_id, "", "", false); 
    std::shared_ptr<BinaryNode<Skill>> resultNode = this->findNode(this->root_ptr_, searchSkill);  
    return resultNode;  
}

/**
 * @param: A const reference to Skill 
 * @post: The Skill is added to the tree (in BST order as implemented in the base class) only if it was not already in the tree. Note that all comparisons are id-based as implemented by the Skill comparison operators.
 * @return: True if the Skill is successfully added to the SkillTree, false otherwise
 */

bool SkillTree::addSkill(const Skill &add_skill)
{
    if(this->contains(add_skill)){
        return false;
    }
    this->add(add_skill);
    return true;
}

std::shared_ptr<BinaryNode<Skill>> SkillTree::findSkillHelper(std::shared_ptr<BinaryNode<Skill>> node, const std::string &name) const {
    if (!node) {
        return nullptr; 
    }
    if (node->getItem().name_ == name) {
        return node; 
    }
    std::shared_ptr<BinaryNode<Skill>> leftResult = findSkillHelper(node->getLeftChildPtr(), name);
    if (leftResult) return leftResult;
    return findSkillHelper(node->getRightChildPtr(), name);
}

/**
 * @param: A const reference to string: the name of a Skill
 * @return: True if the Skill is successfully removed from the SkillTree, false otherwise
 */

bool SkillTree::removeSkill(const std::string &skill_name)
{
    std::shared_ptr<BinaryNode<Skill>> foundNode = findSkillHelper(this->getRoot(), skill_name);
    if (foundNode) {
        Skill foundSkill = foundNode->getItem();
        return this->remove(foundSkill); 
    }
    return false; 
}

/**
 * @post: Clears the tree
 */

void SkillTree::clear() {
    root_ptr_ = nullptr;
}

int SkillTree::calculateSkillPointsHelper(std::shared_ptr<BinaryNode<Skill>> node, int skill_id, int depth) const {
    if (!node) {
        return -1;
    }
    if (node->getItem().id_ == skill_id) {
        return depth + 1;
    }
    int leftResult = calculateSkillPointsHelper(node->getLeftChildPtr(), skill_id, depth + 1);
    if (leftResult != -1) return leftResult;
    return calculateSkillPointsHelper(node->getRightChildPtr(), skill_id, depth + 1);
}

/**
 * @param: A const reference to int representing the id of a Skill
    * Note: For a Skill to be leveled up, its parent Skill must also be leveled up, thus the Skill points are the number of Skills that must be leveled up before and including the Skill with the given id.
* @return: an integer: the number of skill points needed to level up the Skill with the given id, starting from the root (i.e. the number of nodes from root to the given Skill).
* Include the Skill with the given id in the count. For example, if the tree contains the following Skills (represented by their ids):
*   5
*  / \
* 1   8
* and the function parameter queries for id 8, the function should return 2.
* Disregard the leveled_ field of the existing Skills in the tree.
* If the Skill with the given id is not found, return -1.
*/

int SkillTree::calculateSkillPoints(const int &skill_id) const {
    return calculateSkillPointsHelper(this->getRoot(), skill_id, 0);
}


/**
 * @post: Balances the tree. Recall the definition of a balanced tree:
 * A tree is balanced if for any node, its left and right subtrees differ in height by no more than 1. * All paths from root of subtrees to leaf differ in length by at most 1
 * Hint: You may sort the elements and build the tree recursively using a divide and conquer approach      
 */

void SkillTree::balance() {
    std::vector<Skill> nodes;
    balanceHelper(this->getRoot(), nodes);
    this->root_ptr_ = balanceBuilder(nodes, 0, nodes.size() - 1);
}

void SkillTree::balanceHelper(std::shared_ptr<BinaryNode<Skill>> node, std::vector<Skill>& nodes) {
    if (!node) return;
    balanceHelper(node->getLeftChildPtr(), nodes);
    nodes.push_back(node->getItem());
    balanceHelper(node->getRightChildPtr(), nodes);
}

std::shared_ptr<BinaryNode<Skill>> SkillTree::balanceBuilder(const std::vector<Skill>& nodes, int start, int end) {
    if (start > end) {
        return nullptr;
    }
    int mid = (start + end) / 2;
    std::shared_ptr<BinaryNode<Skill>> node = std::make_shared<BinaryNode<Skill>>(nodes[mid]);
    node->setLeftChildPtr(balanceBuilder(nodes, start, mid - 1));
    node->setRightChildPtr(balanceBuilder(nodes, mid + 1, end));
    return node;
}

/**
 * @post: prints the tree in preorder, in the format:
 [id_] [name_]\n
[description_]\n 
[leveled_]
*/

#include <iostream>

void SkillTree::preorderDisplay() {
    preorderDisplayHelper(this->getRoot());
}

void SkillTree::preorderDisplayHelper(std::shared_ptr<BinaryNode<Skill>> node) const {
    if (node != nullptr) {
        Skill skill = node->getItem();
        std::cout << skill.id_ << skill.name_ << "\n" << skill.description_ << "\n" << skill.leveled_ << "\n";

        preorderDisplayHelper(node->getLeftChildPtr());

        preorderDisplayHelper(node->getRightChildPtr());
    }
}