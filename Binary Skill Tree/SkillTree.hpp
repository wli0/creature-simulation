#include "BinarySearchTree.hpp"
#include <fstream>
#include <sstream>
#include <string>
#ifndef SKILLTREE_HPP_

struct Skill{
public:
    int id_; // A unique identifier for the Skill
    std::string name_; // The name of the Skill
    std::string description_; // The description of the Skill
    bool leveled_; // Whether or not the Skill is leveled up

    // Default constructor

    Skill():id_(0), name_(""), description_(""), leveled_(false){};

    // Parameterized constructor
    /**
     * @param id: The unique identifier for the Skill
     * @param name: The name of the Skill
     * @param description: The description of the Skill
     * @param leveled: Whether or not the Skill is leveled up
     */
    Skill(int id, const std::string& name, const std::string& description, bool leveled):id_{id}, name_{name}, description_{description}, leveled_{leveled}{};

    /**
     * @param: A const reference to Skill 
     * @return: True if the id_ of the Skill is equal to that of the argument, false otherwise
     */
    bool operator==(const Skill& skill_id) const{
    if(id_ == skill_id.id_){
        return true;
    }
    return false;
    }

    /**
     * @param: A const reference to Skill 
     * @return: True if the id_ of the Skill is less than that of the argument, false otherwise
     */
    bool operator<(const Skill& skill_id) const{
    if(id_ < skill_id.id_){
        return true;
    }
    return false;
    }

    /**
     * @param: A const reference to Skill 
     * @return: True if the id_ of the Skill is greater than that of the argument, false otherwise
     */
    bool operator>(const Skill& skill_id) const{
    if(id_ > skill_id.id_){
        return true;
    }
    return false;
    }


};

class SkillTree : public BinarySearchTree<Skill>{
public:
    /**
     * Default Constructor
     */
    SkillTree();

    /**
     * @param: A const reference to string: the name of a csv file
     * @post: The SkillTree is populated with Skills from the csv file
     * The file format is as follows:
     * id,name,description,leveled
     * Ignore the first line. Each subsequent line represents a Skill to be added to the SkillTree.
    */
    SkillTree(const std::string& fileName);

   /**
     * @param: A const reference to int representing the id of the Skill to be found
     * @return: A pointer to the node containing the Skill with the given id, or nullptr if the Skill is not found
    */
    std::shared_ptr<BinaryNode<Skill>> findSkill(const int& skill_id) const;


    /**
     * @param: A const reference to Skill 
     * @post: The Skill is added to the tree (in BST order as implemented in the base class) only if it was not already in the tree. Note that all comparisons are id-based as implemented by the Skill comparison operators.
     * @return: True if the Skill is successfully added to the SkillTree, false otherwise
     */
    bool addSkill(const Skill& add_skill);


    /**
     * @param: A const reference to string: the name of a Skill
     * @return: True if the Skill is successfully removed from the SkillTree, false otherwise
     */
    bool removeSkill(const std::string& skill_name);

    std::shared_ptr<BinaryNode<Skill>> findSkillHelper(std::shared_ptr<BinaryNode<Skill>> node, const std::string &name) const;


    /**
     * @post: Clears the tree
     */
    void clear();



    int calculateSkillPointsHelper(std::shared_ptr<BinaryNode<Skill>> node, int skill_id, int depth) const;
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
    int calculateSkillPoints(const int& skill_id) const;


    /**
     * @post: Balances the tree. Recall the definition of a balanced tree:
     * A tree is balanced if for any node, its left and right subtrees differ in height by no more than 1. * All paths from root of subtrees to leaf differ in length by at most 1
     * Hint: You may sort the elements and build the tree recursively using a divide and conquer approach      
     */
    void balance();
    void balanceHelper(std::shared_ptr<BinaryNode<Skill>> node, std::vector<Skill>& nodes);
    std::shared_ptr<BinaryNode<Skill>> balanceBuilder(const std::vector<Skill>& nodes, int start, int end);

    /**
     * @post: prints the tree in preorder, in the format:
     [id_] [name_]\n
    [description_]\n 
    [leveled_]
    */
    void preorderDisplay();
    void preorderDisplayHelper(std::shared_ptr<BinaryNode<Skill>> node) const;

};

#endif