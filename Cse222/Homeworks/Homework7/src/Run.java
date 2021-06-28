import java.util.*;

/**
 * Driver class for the Homework7
 */
public class Run {
    // Data Structure ArrayLists (For Part3)
    private static final ArrayList<BinarySearchTree<Integer>> myBST = new ArrayList<>();
    private static final ArrayList<RedBlackTree<Integer>>     myRBT = new ArrayList<>();
    private static final ArrayList<TwoThreeTree<Integer>>     myTTT = new ArrayList<>();
    private static final ArrayList<BTree<Integer>>            myBT  = new ArrayList<>();
    private static final ArrayList<SkipList<Integer>>         mySL  = new ArrayList<>();

    public static void main(String[] args) {
        String breakLine = "------------------------------------";
        ArrayList<Integer> numbers = randomGenerator(20, 100);

        // ------------- PART 1 ------------- //
        NS_SkipList<Integer> nsSkipList = new NS_SkipList<>();
        NS_AVLTree<Integer>  nsAvlTree  = new NS_AVLTree<>();

        System.out.println();
        System.out.println(breakLine + " PART 1 " + breakLine);
        System.out.println();

        // Navigable Set - SkipList
        System.out.println("Navigable Set - SkipList");
        System.out.println("------------------------");

        // Insert
        System.out.println("-> Inserting 20 random numbers between 0-100");

        System.out.println("\nInserting...\n");
        for(int i=0; i<numbers.size(); i++)
            nsSkipList.add(numbers.get(i));

        System.out.print("Descending Iterator: ");
        Iterator<Integer> itrSL = nsSkipList.descendingIterator();

        while(itrSL.hasNext())
            System.out.print(itrSL.next() + " ");

        // Delete
        System.out.println("\n\n-> Removing 10 randomly selected number from the Set");

        System.out.println("\nRemoving...\n");
        for(int i=0; i<10; i++)
            nsSkipList.remove(numbers.get(i));

        System.out.print("Descending Iterator: ");
        itrSL = nsSkipList.descendingIterator();

        while(itrSL.hasNext())
            System.out.print(itrSL.next() + " ");


        // Navigable Set - AVL
        System.out.println("\n\nNavigable Set - AVL");
        System.out.println("-------------------");

        // Insert
        System.out.println("-> Inserting 20 random numbers between 0-100");

        System.out.println("\nInserting...\n");
        for(int i=0; i<numbers.size(); i++)
            nsAvlTree.add(numbers.get(i));


        System.out.print("Iterator: ");
        Iterator<Integer> itrAVL = nsAvlTree.iterator();

        while(itrAVL.hasNext())
            System.out.print(itrAVL.next() + " ");


        // TailSet - HeadSet

        // sets
        NavigableSet<Integer> tailSet = nsAvlTree.tailSet(50, false);
        NavigableSet<Integer> headSet = nsAvlTree.headSet(50, false);
        // their iterators
        Iterator<Integer> tailItr = tailSet.iterator();
        Iterator<Integer> headItr = headSet.iterator();

        // tailSet
        System.out.println("\n\n-> Showing numbers bigger than 50 using tailSet(not including 50)");

        System.out.print("Iterator: ");

        while(tailItr.hasNext())
            System.out.print(tailItr.next() + " ");

        // headSet
        System.out.println("\n\n-> Showing numbers less than 50 using headSet(not including 50)");

        System.out.print("Iterator: ");

        while(headItr.hasNext())
            System.out.print(headItr.next() + " ");




        // ------------- PART 2 ------------- //
        TreeCategory<Integer> category = new TreeCategory<>();

        System.out.println("\n");
        System.out.println(breakLine + " PART 2 " + breakLine);
        System.out.println();

        BinarySearchTree<Integer> bstTree1 = new BinarySearchTree<>();
        BinarySearchTree<Integer> bstTree2 = new BinarySearchTree<>();
        BinarySearchTree<Integer> bstTree3 = new BinarySearchTree<>();

        System.out.println("\n-> Inserting 10 numbers into 3 Binary Search Tree\n");

        int[] nums1 = new int[]{250, 200, 360, 100, 220, 300, 400, 120, 280, 380};
        int[] nums2 = new int[]{90, 30, 70, 150, 10, 60, 20, 80, 100, 80        };
        int[] nums3 = new int[]{110, 100, 120, 90, 130, 80, 140, 170, 40, 200   };

        for(int i=0; i<10; i++) {
            bstTree1.add(nums1[i]);
            bstTree2.add(nums2[i]);
            bstTree3.add(nums3[i]);
        }

        System.out.println();
        System.out.println("Tree 1");
        System.out.println("------");
        System.out.println(bstTree1);

        System.out.println();
        System.out.println("Tree 2");
        System.out.println("------");
        System.out.println(bstTree2);

        System.out.println();
        System.out.println("Tree 3");
        System.out.println("------");
        System.out.println(bstTree3);

        System.out.println("\n-> Deciding to types of the trees\n");
        System.out.println("Tree1 : " + category.isAVLorRBT(bstTree1));
        System.out.println("Tree2 : " + category.isAVLorRBT(bstTree2));
        System.out.println("Tree3 : " + category.isAVLorRBT(bstTree3));







        // ------------- PART 3 ------------- //

        System.out.println("\n");
        System.out.println(breakLine + " PART 3 " + breakLine);
        System.out.println();

        // Initializing Data Structures
        for(int i=0; i<40; i++) {
            myBST.add(new BinarySearchTree<>());
            myRBT.add(new RedBlackTree<>());
            myTTT.add(new TwoThreeTree<>());
            myBT.add(new BTree<>(10));
            mySL.add(new SkipList<>());
        }

        // Inserting Random Numbers into the Data Structures
        insertNumbers();

        // Inserting 100 numbers and returning running time
        long timeCollapsedBST_10000 = 0, timeCollapsedBST_20000 = 0, timeCollapsedBST_40000 = 0, timeCollapsedBST_80000 = 0;
        long timeCollapsedRBT_10000 = 0, timeCollapsedRBT_20000 = 0, timeCollapsedRBT_40000 = 0, timeCollapsedRBT_80000 = 0;
        long timeCollapsedTTT_10000 = 0, timeCollapsedTTT_20000 = 0, timeCollapsedTTT_40000 = 0, timeCollapsedTTT_80000 = 0;
        long timeCollapsedBT_10000  = 0, timeCollapsedBT_20000  = 0, timeCollapsedBT_40000  = 0, timeCollapsedBT_80000  = 0;
        long timeCollapsedSL_10000  = 0, timeCollapsedSL_20000  = 0, timeCollapsedSL_40000  = 0, timeCollapsedSL_80000  = 0;

        // Calculating collapsed time for each instance and summing total time collapsed
        for(int i=0; i<40; i++) {
            if(i < 10) {
                timeCollapsedBST_10000 += insertExtraBST(myBST.get(i));
                timeCollapsedRBT_10000 += insertExtraRBT(myRBT.get(i));
                timeCollapsedTTT_10000 += insertExtraTTT(myTTT.get(i));
                timeCollapsedBT_10000  += insertExtraBT (myBT.get(i) );
                timeCollapsedSL_10000  += insertExtraSL (mySL.get(i) );
            }
            else if(i < 20) {
                timeCollapsedBST_20000 += insertExtraBST(myBST.get(i));
                timeCollapsedRBT_20000 += insertExtraRBT(myRBT.get(i));
                timeCollapsedTTT_20000 += insertExtraTTT(myTTT.get(i));
                timeCollapsedBT_20000  += insertExtraBT (myBT.get(i) );
                timeCollapsedSL_20000  += insertExtraSL (mySL.get(i) );
            }
            else if(i < 30) {
                timeCollapsedBST_40000 += insertExtraBST(myBST.get(i));
                timeCollapsedRBT_40000 += insertExtraRBT(myRBT.get(i));
                timeCollapsedTTT_40000 += insertExtraTTT(myTTT.get(i));
                timeCollapsedBT_40000  += insertExtraBT (myBT.get(i) );
                timeCollapsedSL_40000  += insertExtraSL (mySL.get(i) );
            }
            else {
                timeCollapsedBST_80000 += insertExtraBST(myBST.get(i));
                timeCollapsedRBT_80000 += insertExtraRBT(myRBT.get(i));
                timeCollapsedTTT_80000 += insertExtraTTT(myTTT.get(i));
                timeCollapsedBT_80000  += insertExtraBT (myBT.get(i) );
                timeCollapsedSL_80000  += insertExtraSL (mySL.get(i) );
            }
        }

        // Calculating the average running time
        long averageBST_10000 = timeCollapsedBST_10000 / 10, averageBST_20000 = timeCollapsedBST_20000 / 10, averageBST_40000 = timeCollapsedBST_40000 / 10, averageBST_80000 = timeCollapsedBST_80000 / 10;
        long averageRBT_10000 = timeCollapsedRBT_10000 / 10, averageRBT_20000 = timeCollapsedRBT_20000 / 10, averageRBT_40000 = timeCollapsedRBT_40000 / 10, averageRBT_80000 = timeCollapsedRBT_80000 / 10;
        long averageTTT_10000 = timeCollapsedTTT_10000 / 10, averageTTT_20000 = timeCollapsedTTT_20000 / 10, averageTTT_40000 = timeCollapsedTTT_40000 / 10, averageTTT_80000 = timeCollapsedTTT_80000 / 10;
        long averageBT_10000  = timeCollapsedBT_10000  / 10, averageBT_20000  = timeCollapsedBT_20000  / 10, averageBT_40000  = timeCollapsedBT_40000  / 10, averageBT_80000  = timeCollapsedBT_80000  / 10;
        long averageSL_10000  = timeCollapsedSL_10000  / 10, averageSL_20000  = timeCollapsedSL_20000  / 10, averageSL_40000  = timeCollapsedSL_40000  / 10, averageSL_80000  = timeCollapsedSL_80000  / 10;

        // 10_000 item
        System.out.println();
        System.out.println("Average running times after inserting 100 elements in a 10000 element size data structure ");
        System.out.println("-------------------------------------(in nanosecond)------------------------------------- ");
        System.out.println("Binary Search Tree : " + averageBST_10000 + "ns");
        System.out.println("Red-Black Tree     : " + averageRBT_10000 + "ns");
        System.out.println("2-3 Tree           : " + averageTTT_10000 + "ns");
        System.out.println("B-Tree             : " + averageBT_10000  + "ns");
        System.out.println("Skip List          : " + averageSL_10000  + "ns");
        // 20_000 item
        System.out.println();
        System.out.println("Average running times after inserting 100 elements in a 20000 element size data structure ");
        System.out.println("-------------------------------------(in nanosecond)------------------------------------- ");
        System.out.println("Binary Search Tree : " + averageBST_20000 + "ns");
        System.out.println("Red-Black Tree     : " + averageRBT_20000 + "ns");
        System.out.println("2-3 Tree           : " + averageTTT_20000 + "ns");
        System.out.println("B-Tree             : " + averageBT_20000  + "ns");
        System.out.println("Skip List          : " + averageSL_20000  + "ns");
        // 40_000 item
        System.out.println();
        System.out.println("Average running times after inserting 100 elements in a 40000 element size data structure ");
        System.out.println("-------------------------------------(in nanosecond)------------------------------------- ");
        System.out.println("Binary Search Tree : " + averageBST_40000 + "ns");
        System.out.println("Red-Black Tree     : " + averageRBT_40000 + "ns");
        System.out.println("2-3 Tree           : " + averageTTT_40000 + "ns");
        System.out.println("B-Tree             : " + averageBT_40000  + "ns");
        System.out.println("Skip List          : " + averageSL_40000  + "ns");
        // 80_000 item
        System.out.println();
        System.out.println("Average running times after inserting 100 elements in a 80000 element size data structure ");
        System.out.println("-------------------------------------(in nanosecond)------------------------------------- ");
        System.out.println("Binary Search Tree : " + averageBST_80000 + "ns");
        System.out.println("Red-Black Tree     : " + averageRBT_80000 + "ns");
        System.out.println("2-3 Tree           : " + averageTTT_80000 + "ns");
        System.out.println("B-Tree             : " + averageBT_80000  + "ns");
        System.out.println("Skip List          : " + averageSL_80000  + "ns");

        // Comparing running times
        System.out.println();
        System.out.println("Running Time Comparison");
        System.out.println("-----------------------");
        System.out.println("B-Tree(order = 10) < Binary Search Tree < Red-Black Tree < 2-3 Tree < SkipList");
        System.out.println("Most  Time Consumed By : Skip List - (Total : " + (averageSL_10000+averageSL_20000+averageSL_40000+averageSL_80000) + "ns)" );
        System.out.println("Least Time Consumed By : B-Tree    - (Total : " + (averageBT_10000+averageBT_20000+averageBT_40000+averageBT_80000) + "ns)");

    }

    private static ArrayList<Integer> randomGenerator(int size) {
        // initializing random
        Random random = new Random();
        // holds random number
        int newNumber = random.nextInt();
        // allocating int array
        ArrayList<Integer> numbers = new ArrayList<>(size);

        for(int i=0; i<size; i++, newNumber = random.nextInt()) {
            if(!numbers.contains(newNumber))
                numbers.add(newNumber);
            else
                i--;
        }

        return numbers;
    }

    private static ArrayList<Integer> randomGenerator(int size, int bound) {
        // initializing random
        Random random = new Random();
        // holds random number
        int newNumber = random.nextInt(bound);
        // allocating int array
        ArrayList<Integer> numbers = new ArrayList<>();

        for(int i=0; i<size; i++, newNumber = random.nextInt(bound)) {
            if(!numbers.contains(newNumber))
                numbers.add(newNumber);
            else
                i--;
        }

        return numbers;
    }

    private static void insertNumbers() {
        for(int i=0; i<10; i++) {
            // Creating random numbers
            ArrayList<Integer> rand_10000 = randomGenerator(10_000);
            ArrayList<Integer> rand_20000 = randomGenerator(20_000);
            ArrayList<Integer> rand_40000 = randomGenerator(40_000);
            ArrayList<Integer> rand_80000 = randomGenerator(80_000);

            System.out.println("Inserting random numbers into instances between (" + (i)*20 + " - " + (i+1)*20 + ")");

            // Index of instances
            @SuppressWarnings("MultipleVariablesInDeclaration")
            int index1 = i, index2 = i+10, index4 = i+20, index8 = i+30;
            int number;

            // Inserting rand_10000
            for(int j=0; j<10_000; j++) {
                number = rand_10000.get(j);

                myBST.get(index1).add(number);
                myRBT.get(index1).add(number);
                myTTT.get(index1).insert(number);
                myBT.get(index1).insert(number);
                mySL.get(index1).add(number);
            }

            // Inserting rand_20000
            for(int j=0; j<20_000; j++) {
                number = rand_20000.get(j);

                myBST.get(index2).add(number);
                myRBT.get(index2).add(number);
                myTTT.get(index2).insert(number);
                myBT.get(index2).insert(number);
                mySL.get(index2).add(number);
            }
            // Inserting rand_40000
            for(int j=0; j<40_000; j++) {
                number = rand_40000.get(j);

                myBST.get(index4).add(number);
                myRBT.get(index4).add(number);
                myTTT.get(index4).insert(number);
                myBT.get(index4).insert(number);
                mySL.get(index4).add(number);
            }

            // Inserting rand_80000
            for(int j=0; j<80_000; j++) {
                number = rand_80000.get(j);

                myBST.get(index8).add(number);
                myRBT.get(index8).add(number);
                myTTT.get(index8).insert(number);
                myBT.get(index8).insert(number);
                mySL.get(index8).add(number);
            }
        }
    }

    private static long insertExtraBST(BinarySearchTree<Integer> tree) {
        final int size = 100;
        ArrayList<Integer> numbers = randomGenerator(size);

        long start = System.nanoTime();
        for(int i=0; i<size; i++)
            tree.add(numbers.get(i));
        long finish = System.nanoTime();


        return finish - start;
    }

    private static long insertExtraRBT(RedBlackTree<Integer> tree) {
        final int size = 100;
        ArrayList<Integer> numbers = randomGenerator(size);

        long start = System.nanoTime();
        for(int i=0; i<size; i++)
            tree.add(numbers.get(i));
        long finish = System.nanoTime();

        return finish - start;
    }

    private static long insertExtraTTT(TwoThreeTree<Integer> tree) {
        final int size = 100;
        ArrayList<Integer> numbers = randomGenerator(size);

        long start = System.nanoTime();
        for(int i=0; i<size; i++)
            tree.insert(numbers.get(i));
        long finish = System.nanoTime();

        return finish - start;
    }

    private static long insertExtraBT(BTree<Integer> tree) {
        final int size = 100;
        ArrayList<Integer> numbers = randomGenerator(size);

        long start = System.nanoTime();
        for(int i=0; i<size; i++)
            tree.insert(numbers.get(i));
        long finish = System.nanoTime();

        return finish - start;
    }

    private static long insertExtraSL(SkipList<Integer> tree) {
        final int size = 100;
        ArrayList<Integer> numbers = randomGenerator(size);

        long start = System.nanoTime();
        for(int i=0; i<size; i++)
            tree.add(numbers.get(i));
        long finish = System.nanoTime();

        return finish - start;
    }


}
