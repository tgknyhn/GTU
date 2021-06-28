/** An implementation of the 2-3 tree. A 2-3 tree is a
 *   search tree in which each node contains either one, two,
 *   or three data  items and (other than leaves) two, three
 *   or four children.  The tree is always balanced in that all
 *   leaves are on the same level, i.e., the length of the path
 *   from the root to a leaf is constant.  This data structure
 *   is a special case of the B-tree, and is the basis for the
 *   Red-Black tree.
 *   @author Koffman and Wolfgang
 */
public class TwoThreeTree<E extends Comparable<E>> {
    private static class Node<E>{
        private int size = 0;
        private E[] data;
        private TwoThreeTree.Node<E>[] child;
        public Node(int order){
            data = (E[]) new Comparable[order-1];
            child = (TwoThreeTree.Node<E>[]) new TwoThreeTree.Node[order];
            size = 0;
        }

    }

    private TwoThreeTree.Node<E> root = null;
    private final int order;
    private TwoThreeTree.Node<E> newChild = null;
    private E newParent = null;
    private int size = 0;

    /**
     * Initializes TwoThreeTree class.
     */
    public TwoThreeTree(){
        this.order = 3;
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
     * Inserts given item to the TwoThreeTree Class
     * @param item Item that will be inserted
     * @return True when insertion is successful
     */
    public boolean insert(E item){
        if (root == null) {
            root = new TwoThreeTree.Node<E>(order);
            root.data[0] = item;
            root.size = 1;
            size++;
            return true;
        }
        newChild = null;
        boolean result = insert(root, item);
        if (newChild != null) {
            TwoThreeTree.Node<E> newRoot = new TwoThreeTree.Node<E>(order);
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
     * Size of the TwoThreeTree class
     * @return Size of the data
     */
    public int size() { return size; }

    private boolean insert(TwoThreeTree.Node<E> root , E item){
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

    private void insertIntoNode(TwoThreeTree.Node<E> node , int index , E obj , TwoThreeTree.Node<E> child){
        for(int i = node.size; i>index; i--){
            node.data[i] = node.data[i - 1];
            node.child[i + 1] = node.child[i];
        }
        node.data[index] = obj;
        node.child[index + 1] = child;
        node.size ++;
    }

    private void splitNode(TwoThreeTree.Node<E> node, int index , E item, TwoThreeTree.Node<E> child){
        newChild = new TwoThreeTree.Node<E>(order);
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
