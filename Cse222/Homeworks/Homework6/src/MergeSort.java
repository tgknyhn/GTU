import java.util.*;

/**
 *  Class for sorting products according to their name by using merge sort method
 */
public class MergeSort {
    /**
     * Takes LinkedList Product list as parameter and sorts it by their name
     * @param array Product list that contains searched products
     */
    public static void sort(LinkedList<Product> array) {
        if(array.size() == 1)
            return;

        int half = array.size() / 2;

        LinkedList<Product> firstHalf = new LinkedList<>();
        LinkedList<Product> secondHalf = new LinkedList<>();

        ListIterator<Product> myItr = array.listIterator();

        for(int i=0; i<half; i++)
            firstHalf.add(myItr.next());

        for(int i=0; i<array.size() - half; i++)
            secondHalf.add(myItr.next());

        // sorting
        sort(firstHalf);
        sort(secondHalf);

        // merging
        merge(array, firstHalf, secondHalf);
    }

    private static void merge(LinkedList<Product> mergedArr, LinkedList<Product> firstArr, LinkedList<Product> secondArr) {
        int mergeIndex = 0;


        while(firstArr.size() != 0 && secondArr.size() != 0) {
            Product first  = firstArr.get(0);
            Product second = secondArr.get(0);

            if(first.compareName(second) < 0) {
                mergedArr.set(mergeIndex++, first);
                firstArr.remove(0);
            }
            else {
                mergedArr.set(mergeIndex++, second);
                secondArr.remove(0);
            }
        }

        // one of them empty
        while(firstArr.size() != 0) {
            mergedArr.set(mergeIndex++, firstArr.get(0));
            firstArr.remove(0);
        }
        while(secondArr.size() != 0) {
            mergedArr.set(mergeIndex++, secondArr.get(0));
            secondArr.remove(0);
        }
    }
}
