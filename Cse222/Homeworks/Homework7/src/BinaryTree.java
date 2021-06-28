import java.io.BufferedReader;
import java.io.IOException;
import java.io.Serializable;

/**
 * A data structure which holds data according to their hierarchy.
 * @param <E> Data type that binary tree holds
 */
public class BinaryTree<E> implements Serializable {
    /* Data Fields */
    protected Node <E> root;


    /* Constructors */

    /**
     * Initializes BinaryTree class with an empty root
     */
    public BinaryTree() { root = null; }

    /**
     * Initializes BinaryTree class with given root
     * @param root BinaryTree's root node
     */
    protected BinaryTree(Node<E> root) { this.root = root; }

    /**
     * Initializes BinaryTree class with given parameters
     * @param data Data that root holds
     * @param leftTree Left subtree of root
     * @param rightTree Right subtree of root
     */
    public BinaryTree(E data, BinaryTree <E> leftTree, BinaryTree <E> rightTree) {
        // new root
        root = new Node<E>(data);

        // Left Tree
        if(leftTree != null)
            root.left = leftTree.root;
        else
            root.left = null;
        // Right Tree
        if(rightTree != null)
            root.right = rightTree.root;
        else
            root.right = null;
    }


    /* Getters */

    /**
     * Returns left subtree of the root
     * @return Left subtree of the root
     */
    public BinaryTree<E> getLeftSubtree() {
        if (root != null && root.left != null)
            return new BinaryTree<E> (root.left);
        else
            return null;
    }

    /**
     * Returns right subtree of the root
     * @return Right subtree of the root
     */
    public BinaryTree<E> getRightSubtree() {
        if(root != null && root.right != null)
            return new BinaryTree<E>(root.right);
        else
            return null;

    }

    /**
     * Returns data value of the root node
     * @return Data value of the root node
     */
    public E getData() {
        if (root != null)
            return root.data;
        else
            return null;
    }


    /* Unique Methods */

    /**
     * Returns true when both left and right subtree is null
     * @return True when both left and right subtree is null
     */
    public boolean isLeaf() { return (root.left == null && root.right == null); }


    private void preOrderTraverse(Node<E> node, int depth, StringBuilder sb) {
        for(int i=1; i<depth; i++)
            sb.append("  ");

        if (node == null)
            sb.append("null\n");
        else {
            sb.append(node.toString());
            sb.append("\n");
            preOrderTraverse(node.left, depth + 1, sb);
            preOrderTraverse(node.right, depth + 1, sb);
        }
    }

    /**
     * Reads given string and creates a binary tree with the data within
     * @param bR String that contains all data
     * @return BinaryTree object that contains all data extracted from bR
     * @throws IOException When bR is null
     */
    public static BinaryTree<String> readBinaryTree(BufferedReader bR) throws IOException {
        // Read a line and trim leading and trailing spaces.
        String data = bR.readLine().trim();

        if (data.equals("null"))
            return null;
        else {
            BinaryTree<String> leftTree = readBinaryTree(bR);
            BinaryTree<String> rightTree = readBinaryTree(bR);
            return new BinaryTree<>(data, leftTree, rightTree);
        }
    }


    /* To String */

    /**
     * Converts object into a string an returns it
     * @return String that contains the object
     */
    public String toString() { return root.toString(); }


    /* INNER CLASS */

    protected static class Node<E> implements Serializable {
        protected E data;
        protected boolean isRed;
        protected Node<E> left;
        protected Node<E> right;

        public Node(E data) {
            this.data = data;
            isRed = true;
            left = null;
            right = null;
        }

        /**
         * Prints out current tree to the console
         * @param lines Indicates how lines will be aligned
         * @param isTail Since method recurse until rightChild is null it will be true because tree recurse d until a tail
         * @param tree Holds all tree representation
         * @return Current tree
         */
        public StringBuilder print(StringBuilder lines, boolean isTail, StringBuilder tree) {
            if(right != null)
                right.print(new StringBuilder().append(lines).append((isTail ? "|   " : "    ")), false, tree);

            tree.append(lines).append((isTail ? "└── " : "┌── ")).append(data.toString()).append("\n");

            if(left != null)
                left.print(new StringBuilder().append(lines).append((isTail ? "    " : "|   ")), true, tree);

            return tree;
        }

        @Override
        public String toString() { return print(new StringBuilder(), true, new StringBuilder()).toString(); }
    }
}