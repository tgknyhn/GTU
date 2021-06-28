import java.util.LinkedList;

/**
 * Class for sorting products according to their size by using insertion sort method
 */
public class InsertionSort {
    /**
     * Takes LinkedList Product list as parameter and sorts it by their price
     * @param array_ll Product list that contains searched products
     */
    public static void sort(LinkedList<Product> array_ll) {
        int first, second;

        Product[] array = new Product[array_ll.size()];

        array = array_ll.toArray(array);

        for (first = 1; first < array.length; first++) {
            second = first;
            while(second > 0 && array[second].comparePrice(array[second-1]) < 0) {
                swap(array, second, second-1);
                second--;
            }
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
