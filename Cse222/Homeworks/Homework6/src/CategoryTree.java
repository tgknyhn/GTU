import java.util.ArrayList;
import java.util.Arrays;

/**
 * A tree class to hold product categories and make search through their parent or child categories.
 */
public class CategoryTree {
    // roots
    private Node root;
    private static Node foundNode; // returned by findRecursive method




    // [Constructor]

    /**
     * Initializes CategoryTree object for the categories of the products
     */
    public CategoryTree() {
        root = new Node("all", null);
        foundNode = null;
    }





    // [public Methods]

    /**
     * Adds given category to the Category Tree
     * @param categoryName Name of the new category
     * @param parentCategory Parent of that category if exist
     */
    public void add(String categoryName, String parentCategory) {
        // getting parent node
        Node parent = findNode(parentCategory);
        // when root is empty
        if(parent == null)
            root.children.add(new Node(categoryName, root));
        // when category already exist
        else if(!checkChildren(categoryName, parent))
            parent.children.add(new Node(categoryName, parent));
    }

    /**
     * Takes whole category line and adds every category name to the category tree
     * @param categoryNames category line which contains all categories
     */
    public void addAll(String categoryNames) {
        ArrayList<String> names = new ArrayList<>(Arrays.asList(categoryNames.split(" >> ")));

        add(names.get(0), "empty");

        for(int i=1; i<names.size(); i++)
            add(names.get(i), names.get(i-1));
    }

    /**
     * Returns one level up category of the given category
     * @param categoryName Name of the category which it's parent category will be returned
     * @return Parent category of the given category, returns "all" if it is the highest category
     */
    public String parentCategory(String categoryName) {
        // curr node
        Node currNode = findNode(categoryName);

        try {
            // parent
            assert currNode != null;
            Node parent = currNode.parent;

            return parent.category;
        } catch (NullPointerException exception) {
            return null;
        }
    }

    @Override
    public String toString() {
        StringBuilder string = new StringBuilder();

        printRecursive(root, string);

        return string.toString();
    }

    // [private Methods]

    private Node findNode(String categoryName) {
        if(root.category.equals(categoryName))
            return root;
        else if(root.children.size() != 0)
            return findRecursive(categoryName, root);
        else
            return null;
    }

    private Node findRecursive(String categoryName, Node currNode) {
        Node temp;

        if(currNode.category.equals(categoryName))
            foundNode = currNode;

        for(int i=0; i<currNode.children.size(); i++) {
            temp = findRecursive(categoryName, currNode.children.get(i));
            if(temp != null)
                foundNode = temp;
        }

        return foundNode;
    }

    private boolean checkChildren(String categoryName, Node currNode) {
        for(int i=0; i<currNode.children.size(); i++)
            if(currNode.children.get(i).category.equals(categoryName))
                return true;

        return false;
    }

    private void printRecursive(Node currNode, StringBuilder string) {
        for(int i=0; i<currNode.children.size(); i++) {
            string.append(currNode);
            printRecursive(currNode.children.get(i), string);
        }
    }

    // [Node]

    private class Node {
        // data
        private String category;
        // children
        private ArrayList<Node> children;
        // parent
        private Node parent;

        // [Node Constructors]
        public Node() {
            category = "";
            parent = null;
            children = new ArrayList<>();
        }
        public Node(String category, Node parent) {
            this.category = category;
            this.parent = parent;
            children = new ArrayList<>();
        }

        // to string

        @Override
        public String toString() {
            StringBuilder string = new StringBuilder();

            // current node
            string.append("Current: ").append(category).append("\n");
            // children
            for(int i=0; i<children.size(); i++)
                string.append("Child ").append(i + 1).append(": ").append(children.get(i).category).append("\n");
            string.append("\n");

            return string.toString();
        }
    }
}
