import java.util.Arrays;
import java.util.Random;

public class Run {
    public static void main(String[] args) {

        // *** [Max Heap Test Cases] ***

        System.out.println("\n------------ [PART 1] ------------\n");

        int MaxSize1 = 10;
        int MaxSize2 = 15;

        // initializing 2 Max Heap object with integer as it's data type
        MaxHeap<Integer> myHeap1 = new MaxHeap<>(MaxSize1);
        MaxHeap<Integer> myHeap2 = new MaxHeap<>(MaxSize2);

        // creating 30 random number with bound 1000
        Random rand        = new Random();                 // seed
        int[]  randNumbers = new int[MaxSize1+MaxSize2+5]; // array

        // inserting random numbers into array
        for(int i=0; i<MaxSize1+MaxSize2+5; i++)
            randNumbers[i] = rand.nextInt(10000);

        // inserting random numbers into myHeap1
        for(int i=0; i<=MaxSize1; i++)
            myHeap1.insert(randNumbers[i]);

        // printing myHeap1
        System.out.println("** MyHeap1 ** ");
        myHeap1.print();

        // Current size of heaps
        System.out.println("Current Size(MyHeap1): " + myHeap1.getSize());
        System.out.println("Current Size(MyHeap2): " + myHeap2.getSize() + "\n");

        // inserting random numbers into myHeap2
        for(int i=MaxSize1; i<=MaxSize1+MaxSize2; i++)
            myHeap2.insert(randNumbers[i]);

        // printing myHeap1
        System.out.println("** MyHeap2 ** ");
        myHeap2.print();

        // Current size of heaps
        System.out.println("Current Size(MyHeap1): " + myHeap1.getSize());
        System.out.println("Current Size(MyHeap2): " + myHeap2.getSize() + "\n");

        // First trying to merge myHeap1 with myHeap2
        System.out.println("** Merging (myHeap2 + myHeap1 --> myHeap2) **");
        myHeap2.merge(myHeap1);

        // Creating another heap with enough size for both myHeap1 and myHeap2
        MaxHeap<Integer> myHeap3 = new MaxHeap<>(MaxSize1+MaxSize2);
        System.out.println("\nCreating another object named myHeap3");

        // Current size of myHeap3
        System.out.println("\nCurrent size(myHeap3): " + myHeap3.getSize());

        // Merging myHeap3 with myHeap1
        System.out.println("\n** Merging (myHeap3 + myHeap1 --> myHeap3)");
        myHeap3.merge(myHeap1);

        // printing the myHeap3
        System.out.println("\n** MyHeap3 **");
        myHeap3.print();

        // Current size of myHeap3
        System.out.println("Current size(myHeap3): " + myHeap3.getSize());

        // Merging myHeap3 with myHeap1
        System.out.println("\n** Merging (myHeap3 + myHeap2 --> myHeap3)");
        myHeap3.merge(myHeap2);

        // printing the myHeap3
        System.out.println("\n** MyHeap3 **");
        myHeap3.print();

        // Current size of myHeap3
        System.out.println("Current size(myHeap3): " + myHeap3.getSize());

        // Searching element 1234
        System.out.println("\nIndex of element '1234' (-1 if it doesn't exist in the heap) : " + myHeap3.searchElement(1234));

        // Removing leaf node
        System.out.println("\nRemoving least important node(also a leaf node) : " + myHeap3.removeWithPriority(MaxSize1+MaxSize2-1));

        // printing the myHeap3 after removal
        System.out.println("\n** MyHeap3 ** (after removing least important)");
        myHeap3.print();

        // Removing leaf node
        System.out.println("\nRemoving root node(also a two children node) : " + myHeap3.removeWithPriority(1));

        // printing the myHeap3 after removal
        System.out.println("\n** MyHeap3 ** (after removing root)");
        myHeap3.print();

        // initializing iterator
        Iterator<Integer> myItr = myHeap3.iterator();

        // going until last element and changing it into 1
        while(myItr.hasNext())
            myItr.next();

        myItr.setLast(1);

        // printing the myHeap3 after changing last element into 1
        System.out.println("\n** MyHeap3 ** (after iterating an iterator for 5 elements. It calls maxHeapify method inside so it doesn't break maxheap structure)");
        myHeap3.print();

        // removing every element in the heap
        int size = myHeap3.getSize();

        for(int i=0; i<size-1; i++)
            myHeap3.removeWithPriority(0);

        System.out.println("Printing class after removing every element inside");
        myHeap3.print();

        // trying remove while there is no element in the heap
        try {
            System.out.println("Trying to remove while there is no element inside");
            myHeap3.removeWithPriority(0);
        } catch (IndexOutOfBoundsException e) {
            System.out.println("Error : You are trying to remove element from an empty heap");
        }


        // *** [BSTHeapTree Test Cases] ***

        System.out.println("\n------------ [PART 2] ------------\n");

        // initializing BSTHeapTree
        BSTHeapTree<Integer> myBST = new BSTHeapTree<>();

        // getting random numbers
        int numberSize = 3000;
        int bound = 5000;

        // initializing randArray
        int[] randNumbers2 = new int[numberSize];

        // inserting random values into array
        for(int i=0; i<numberSize; i++)
            randNumbers2[i] = rand.nextInt(bound);

        // adding the values into BSTHeapTree
        for(int i=0; i<numberSize; i++)
            myBST.add(randNumbers2[i]);

        // sorting array
        Arrays.sort(randNumbers2);

        // printing myBST
        System.out.println(myBST);

        // searching 100 numbers in the array
        System.out.println("\nPrinting occurrences of 100 values that is in the array\n");
        for(int i=0; i<3000; i += 30)
            System.out.println("Number of occurrence of value " + randNumbers2[i] + ": " + myBST.find(randNumbers2[i]));


        // searching 10 numbers that is not in the array
        System.out.println("\nPrinting occurrences of 10 values that is not in the array\n");
        for(int i=0; i<10; i++)
            System.out.println("Number of occurrence of value " + (i+5001) + ": " + myBST.find(i+5001));

        System.out.println("\nMode of the BSTHeapTree: " + myBST.find_mode());
        System.out.println("It's occurrence value  : " + myBST.find_mode_occurrence());

        // getting occurrence values before removing elements
        int[] ocrBeforeRem = new int[100];
        int[] ocrAfterRem  = new int[100];

        // filling array before removal
        for(int i=0; i<100; i++)
            ocrBeforeRem[i] = myBST.find(randNumbers2[i+(i*20)]);
        // filling array after removal
        for(int i=0; i<100; i++)
            ocrAfterRem[i] = myBST.remove(randNumbers2[i+(i*20)]);

        // printing before & after occurrences of elements that are in the array
        System.out.println("\nOccurrence values of 100 elements that is in the array before removing and after removing them");
        for(int i=0; i<100; i++)
            System.out.println((i+1) + ") Value: " + randNumbers2[i+(i*20)] + " Occurrence before removing: " + ocrBeforeRem[i] + " Occurrence after removing: " + ocrAfterRem[i]);

        // getting occurrence values before removing elements
        int[] ocrBeforeRem2 = new int[10];
        int[] ocrAfterRem2  = new int[10];

        // filling array before removal
        for(int i=0; i<10; i++)
            ocrBeforeRem2[i] = myBST.find(i+5001);
        // filling array after removal
        for(int i=0; i<10; i++)
            ocrAfterRem2[i] = myBST.remove(i+5001);

        // printing before & after occurrences of elements that are in the array
        System.out.println("\nOccurrence values of 10 elements that is NOT in the array before removing and after removing them");
        for(int i=0; i<10; i++)
            System.out.println((i+1) + ") Value: " + (i+5001) + " Occurrence before removing: " + ocrBeforeRem2[i] + " Occurrence after removing: " + ocrAfterRem2[i]);

        // printing last shape of the BSTHeapTree
        System.out.println("\nAfter removing 100 elements from BSTHeapTree this is the last shape of it\n");
        System.out.println(myBST);

        // [END]
    }
}
