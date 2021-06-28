/**
 * A data structure that holds data in a sorted and hierarchical way.
 * It also balances itself after balance is broken between left and right nodes
 * It can have more than 2 data in a node and can have more than 2 child
 * @param <E> Data type that B-Tree holds. It must extend comparable class.
 */
public class BTree<E extends Comparable<E>> {
    private static class Node<E>{
        private int size = 0;
        private E[] data;
        private Node<E>[] child;
        public Node(int order){
            data = (E[]) new Comparable[order-1];
            child = (Node<E>[]) new Node[order];
            size = 0;
        }

    }

    private Node<E> root = null;
    private int order;
    private Node<E> newChild = null;
    private E newParent = null;
    private int size = 0;

    /**
     * Initializes BTree object with given order. Order decides how many item is stored in a node.
     * @param order Order of the BTree
     */
    public BTree(int order){
        this.order = order;
        root = null;
    }

    private int binarySearch(E[] data,E item, int first , int last){
        if (first > last)
            return -1;
        else {
            int middle = (first + last) / 2;
            int compResult = 0;
            if(data[middle] != null) compResult = item.compareTo(data[middle]);
            if (compResult == 0)
                return middle;
            else if (compResult < 0) {
                if(middle < 1)
                    return middle;
                else if (data[middle - 1 ] != null && item.compareTo(data[middle - 1]) > 0)
                    return middle;
                else
                    return binarySearch(data, item, first, middle - 1);
            } else {
                if(middle+1 >=  root.size)
                    return middle+1;
                else if (data[middle + 1] != null && item.compareTo(data[middle + 1]) < 0)
                    return middle+1;
                else
                    return binarySearch(data, item, middle + 1, last);
            }
        }
    }

    /**
     * Returns true when given item is successfully inserted
     * @param item Item that will be inserted
     * @return True when insertion is successful
     */
    public boolean insert(E item){
        if (root == null) {
            root = new Node<E>(order);
            root.data[0] = item;
            root.size = 1;
            size++;
            return true;
        }
        newChild = null;
        boolean result = insert(root, item);
        if (newChild != null) {
            Node<E> newRoot = new Node<E>(order);
            newRoot.child[0] = root;
            newRoot.child[1] = newChild;
            newRoot.data[0] = newParent;
            newRoot.size = 1;
            root = newRoot;
        }
        if(result)
            size++;

        return result;
    }

    /**
     * Returns the size of the BTree class
     * @return Data size
     */
    public int size() { return size; }

    private boolean insert(Node<E> root , E item){
        int index = binarySearch(root.data,item , 0 , root.size);
        if(index != root.size && item.compareTo(root.data[index]) == 0){
            return false;
        }
        if(root.child[index] == null){
            if(root.size < order - 1){
                insertIntoNode(root , index , item , null);
                newChild = null;
            }
            else{
                splitNode(root , index , item , null);
            }
            return true;
        }
        else{
            boolean result = insert(root.child[index] , item);
            if(newChild != null){
                if(root.size < order - 1){
                    insertIntoNode(root,index,newParent,newChild);
                    newChild = null;
                }
                else{
                    splitNode(root,index,newParent,newChild);
                }
            }
            return result;
        }
    }

    private void insertIntoNode(Node<E> node , int index , E obj , Node<E> child){
        for(int i = node.size; i>index; i--){
            node.data[i] = node.data[i - 1];
            node.child[i + 1] = node.child[i];
        }
        node.data[index] = obj;
        node.child[index + 1] = child;
        node.size ++;
    }

    private void splitNode(Node<E> node, int index , E item, Node<E> child){
        newChild = new Node<E>(order);
        int numToMove = (order - 1) - ((order - 1) / 2);
        if(index > (order-1) / 2){
            numToMove --;
        }
        System.arraycopy(node.data,order-numToMove-1,newChild.data,0,numToMove);
        System.arraycopy(node.child,order-numToMove,newChild.child,1,numToMove);
        node.size = order - numToMove - 1;
        newChild.size = numToMove;

        if(index == ((order - 1 ) / 2)){
            newParent = item;
            newChild.child[0] = child;
        }
        else{
            if(index < ((order - 1) / 2))
                insertIntoNode(node,index,item,child);
            else
                insertIntoNode(newChild,index-((order - 1) / 2) -1,item,child);
            newParent = node.data[node.size - 1];
            newChild.child[0] = node.child[node.size];
            node.size --;
        }
        for(int i = node.size; i < node.data.length; i++){
            node.data[i] = null;
            node.child[i + 1] = null;
        }
    }

}
