/**
 * Class to categorize tree types.
 * @param <E> Data type that TreeCategory holds. It must extend comparable class.
 */
public class TreeCategory<E extends Comparable<E>> {
    /**
     * Returns string that contains type of the given BinarySearchTree
     * @param tree Tree which will be categorized
     * @return Type of the tree
     */
    public String isAVLorRBT(BinarySearchTree<E> tree) {
        boolean isAVL = isAVL(tree.root);
        boolean isRBT = isRBT(tree, tree.root);

        if(isAVL)
            return "This is an AVL Tree";
        else if(isRBT)
            return "This is a Red-Black Tree & rules are broken for AVL Tree";
        else
            return "This tree is neither an AVL Tree or a Red-Black Tree";
    }

    private boolean isAVL(BinaryTree.Node<E> node) {
        if(node == null)
             return true;

        int leftHeight  = treeHeight(node.left, 0);
        int rightHeight = treeHeight(node.right, 0);

        int difference;

        if(leftHeight >= rightHeight)
            difference = leftHeight-rightHeight;
        else
            difference = rightHeight-leftHeight;


        return (difference == 1 || difference == 0 || difference == -1) && isAVL(node.left) && isAVL(node.right);
    }

    private boolean isRBT(BinarySearchTree<E> tree, BinaryTree.Node<E> node) {
        if(!tree.isRed())
            return false;
        if(node == null)
            return true;

        int leftHeight  = treeHeight(node.left, 0);
        int rightHeight = treeHeight(node.right, 0);

        int difference;

        if(leftHeight >= rightHeight)
            difference = leftHeight-rightHeight;
        else
            difference = rightHeight-leftHeight;

        return (difference >= -2 && difference <= 2) && isRBT(tree, node.left) && isRBT(tree, node.right);
    }



    // Helper Method

    private int treeHeight(BinaryTree.Node<E> node, int height) {
        if(node == null)
            return 0;

        height = treeHeight(node.left, height);
        height = treeHeight(node.right, height);

        return height+1;
    }
}
