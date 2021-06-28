import java.util.ArrayList;

/**
 * A Binary Search Tree class which holds it's data as Max Heap and implements all methods for BST
 * @param <E> Data Type
 */
public class BSTHeapTree<E extends Comparable<E>> {
    /**
     * Holds heap as Generic Node
     */
    private Node<E> heapTree;

    /**
     * Shows how many elements every MaxHeap node can hold
     */
    private final int MAX_HEAP_SIZE = 7;

    /**
     * Holds mode of the BSTHeapTree class
     */
    private E mode;
    /**
     * Holds number of occurrence of the mode
     */
    private int modeOccurrence;

    /**
     * To count occurrence while adding items
     */
    private static int numOfDataOccurrence;


    /**
     * Class to hold children nodes and heap
     * @param <E> heap type
     */
    private class Node<E extends Comparable<E>> {
        // heap
        private MaxHeap<E> heap;
        // Left Child
        private Node<E> leftChild;
        // Right Child
        private Node<E> rightChild;

        // Constructor
        public Node(E item) {
            // initializes MaxHeap
            heap = new MaxHeap<>(MAX_HEAP_SIZE);
            // inserting first element
            heap.insert(item);
            // initializing children
            leftChild  = null;
            rightChild = null;
        }

        /**
         * Prints out current tree to the console
         * @param lines Indicates how lines will be aligned
         * @param isTail Since method recurse until rightChild is null it will be true because tree recurse d until a tail
         * @param tree Holds all tree representation
         * @return Current tree
         */
        public StringBuilder print(StringBuilder lines, boolean isTail, StringBuilder tree) {
            if(rightChild != null)
                rightChild.print(new StringBuilder().append(lines).append((isTail ? "|   " : "    ")), false, tree);

            tree.append(lines).append((isTail ? "└── " : "┌── ")).append(heap.toString()).append("\n");

            if(leftChild != null)
                leftChild.print(new StringBuilder().append(lines).append((isTail ? "    " : "|   ")), true, tree);

            return tree;
        }

        @Override
        public String toString() { return print(new StringBuilder(), true, new StringBuilder()).toString(); }
    }

    /**
     * Initializes BSTHeapTree class
     */
    public BSTHeapTree() {
        heapTree = null;
        mode = null;
        modeOccurrence = 0;
    }

    /**
     * Adds and item to the BSTHeapTree. If it is present, then add it to the occurrence number
     * @param item Item that will be added to the tree
     * @return Occurrence number of the item
     */
    public int add(E item) {
        // recurse down until it reaches every node.
        heapTree = addRecursive(heapTree, item);
        // number of occurrence of given item
        return numOfDataOccurrence;
    }

    /**
     * Recursively search through every node until it finds given element.
     * If element doesn't exist, then add it to an empty node or create a node then add it
     * @param currNode Node that addRecursive method currently searches for the given item
     * @param item Item that will be added to the tree
     * @return Node that contains given item
     */
    private Node<E> addRecursive(Node<E> currNode, E item) {
        // If item doesn't exist and current node is null than create a node and add item
        if(currNode == null) {
            currNode = new Node<>(item);
            numOfDataOccurrence = 1;
            if(modeOccurrence == 0) {
                mode = item;
                modeOccurrence = 1;
            }
            return currNode;
        }
        // If size of the current node does not exceed max size (which is 7 for this hw)
        // or given element is already in the current node than add it to this node or increase it's occurrence number.
        else if(currNode.heap.getSize() != MAX_HEAP_SIZE || currNode.heap.searchElement(item) != -1) {
            numOfDataOccurrence = currNode.heap.insert(item);
            if(modeOccurrence < numOfDataOccurrence) {
                mode = item;
                modeOccurrence = numOfDataOccurrence;
            }
        }
        // If item is not in the current node than recurse down until it reaches a null node or finds
        // a node that contains given item
        else {
            if(item.compareTo(currNode.heap.getMax()) < 0)
                currNode.leftChild = addRecursive(currNode.leftChild, item);
            else if(item.compareTo(currNode.heap.getMax()) > 0)
                currNode.rightChild = addRecursive(currNode.rightChild, item);
        }

        return currNode;
    }

    /**
     * Removes given item from the BSTHeapTree
     * If item's occurrence is more than 1 then method
     * decreases it's occurrence number by 1
     * @param item Item that will be removed or occurrence number decreased
     * @return occurrence number after removal
     */
    public int remove(E item) {
        return removeElement(heapTree, heapTree, item);
    }

    /**
     * Recursively searches through every node until it finds given item
     * If item exist and it is the last element in the current Heap node than calls removeRecursive
     * to remove that node from the BSTHeapTree. If it is not the last element then decrease it's occurrence value
     * or remove it if occurrence is 1
     * @param currNode Node that removeElement method currently searches through
     * @param item Item that will be removed or occurrence number decreased
     * @return occurrence number after removal
     */
    private int removeElement(Node<E> parentNode, Node<E> currNode, E item) {
        // occurrence is 0 if item is not found
        int occurrence = 0;
        // if current node is null then directly return 0
        if(currNode == null)
            return 0;

        // abbreviation for currNode.heap
        MaxHeap<E> heap = currNode.heap;

        // base conditions
        // if there is 1 occurrence in the heap for that element and if it is the only item
        // in the heap then call removeRecursive to remove that heap node
        if(heap.getOccurrence(item) == 1 && heap.getSize() == 2) {
            removeNode(parentNode, currNode, item);
            return 0;
        }
        // if there is more than 1 occurrence in the heap for that element and it it is NOT the only item
        // in the heap then only remove 1 occurrence from that element
        else if(heap.getOccurrence(item) > 0) {
            heap.removeWithPriority(heap.searchElement(item));
            heap.builtMaxHeap();
            return heap.getOccurrence(item);
        }
        // if there is no occurrence in the heap than go to leftChild until it's null
        if(currNode.leftChild != null)
            occurrence += removeElement(currNode, currNode.leftChild, item);
        // if there is no occurrence in the heap than go to rightChild until it's null
        if(currNode.rightChild != null)
            occurrence += removeElement(currNode, currNode.rightChild, item);

        // if the currNode still didn't find any element than it doesn't exist in the BSTHeapTree so return 0
        return occurrence;
    }

    /**
     * Called when given element is the last element in the current Heap node.
     * It recurse down until it finds node with least bigger value after given item
     * and links parent node to this node to remove currNode
     * @param parentNode Parent of the currNode
     * @param currNode Node that will be removed
     * @param item Last item in the currNode
     */
    private void removeNode(Node<E> parentNode, Node<E> currNode, E item) {
        // Base Condition
        if(currNode == null)
            return;

        // [Recurse Down Tree]

        /*
         *                                  [Example]
         *                    20                                  20
         *                  /    \                              /    \
         *                 /      \                            /      \
         *                10      30          ---->           10       24
         *               /  \    /  \                        /  \        \
         *              5   12  24  33                      5   12       33
         *
         */

        Node<E> nodeToRemove = currNode;

        if(currNode.rightChild != null) {
            // first getting right child
            nodeToRemove = nodeToRemove.rightChild;

            // then we reach most far left child to find least bigger value of given item
            while(currNode.leftChild != null)
                nodeToRemove = nodeToRemove.leftChild;

            // removing every element from current node
            while(currNode.heap.getSize() < 2) {
                currNode.heap.removeWithPriority(0);
            }

            // getting iterator from nodeToRemove
            Iterator<E> nodeItr = nodeToRemove.heap.iterator();

            // inserting all elements of nodeToRemove into currNode
            while(nodeItr.hasNext())
                currNode.heap.insert(nodeItr.next());

            nodeToRemove.heap = null;
        }
        else {
            if(isLeftChild(parentNode, item))
                parentNode.leftChild = currNode.leftChild;
            else
                parentNode.rightChild = currNode.leftChild;
        }
    }

    /**
     * If left child of the parent node contains given element return true, else false
     * @param parentNode Node that it's children searched for the item
     * @param item Searched data in the node
     * @return True when left child contains given item
     */
    private boolean isLeftChild(Node<E> parentNode, E item) {
        return parentNode.leftChild.heap.getMax() == item;
    }

    /**
     * Returns number of occurrence of given item. 0 if given item does not exist
     * @param item Item that searched in whole BSTHeapTree
     * @return Occurrence number of given item
     */
    public int find(E item) {
        MaxHeap<E> heap;
        // heap that contains given element
        try {
            heap = findRecursive(heapTree, item).heap;
        } catch (NullPointerException e) {
            return 0;
        }
        // return occurrence of the element
        return heap.getOccurrence(item);
    }

    /**
     * By first going until most left children then right children,
     * Method searches through every node in the BSTHeapTree.
     * When it finds returns node that contains given element. Else returns null
     * @param currNode Node that given item is searched through
     * @param item Data that method searches for
     * @return Node that contains given data
     */
    private Node<E> findRecursive(Node<E> currNode, E item) {
        Node<E>    returnNode = null;
        MaxHeap<E> heap;

        if(currNode == null)
            return null;

        // initializing abbreviation
        heap = currNode.heap;

        // if current node contains given element in the heap than return that node
        if(heap.searchElement(item) != -1)
            return currNode;

        // if current node doesn't contain given element than recurse down to leftChild
        if(currNode.leftChild != null)
             returnNode = findRecursive(currNode.leftChild, item);
        // if current node doesn't contain given element than recurse down to rightChild
        if(currNode.rightChild != null && returnNode == null)
            returnNode = findRecursive(currNode.rightChild, item);

        return returnNode;
    }

    /**
     * Returns mode(most occurred) element of the BSTHeapTree
     * @return Element which has biggest occurrence value
     */
    public E find_mode() {
        return mode;
    }

    /**
     * Return occurrence value of the mode element
     * @return occurrence value of the mode element
     */
    public int find_mode_occurrence() {
        return modeOccurrence;
    }

    @Override
    public String toString() { return heapTree.toString(); }
}
