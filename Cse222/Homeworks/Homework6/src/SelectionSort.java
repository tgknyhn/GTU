import java.util.LinkedList;

/**
 *  Class for sorting products according to their discount percentage by using selection sort method
 */
public class SelectionSort {
    /**
     * Takes LinkedList Product list as parameter and sorts it by their discount percentage
     * @param array_ll Product list that contains searched products
     */
    public static void sort(LinkedList<Product> array_ll) {
        int minIndex;

        Product[] array = new Product[array_ll.size()];

        array = array_ll.toArray(array);

        for(int first=0; first<array.length-1; first++) {
            // reinitializing minIndex after swap
            minIndex = first;

            // searching for lowest value
            for(int second=first+1; second<array.length; second++)
                if(array[minIndex].compareDiscountPercentage(array[second]) > 0)
                    minIndex = second;

            // swapping current index with lowest value
            swap(array, first, minIndex);
        }

        for(int i=0; i<array.length; i++)
            array_ll.set(i, array[i]);
    }

    private static void swap(Product[] array, int first, int second) {
        Product temp = array[first];

        array[first] = array[second];

        array[second] = temp;
    }
}
